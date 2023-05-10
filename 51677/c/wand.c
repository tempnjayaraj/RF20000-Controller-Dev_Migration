/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2842                 $:
$Date:: 2022-10-13 14:27:34#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      wand.h
\brief     Smart wand driver.
\details   NA
*******************************************************************************/
#include "MK26F18.h"
#include "common.h"
#include "wand.h"
#include "system.h"
#include "cpu.h"
#include "pit.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "legacy_wand.h"
#include "state_machine.h"
#include "notifications.h"
#include "ent_wand_config.h"
#include "manta_wand_config.h"
#include "gui.h"
#include "audio.h"
#include "rf.h"
#ifdef DEBUG_CAN
#include "can_debug.h"
#endif

/* get u16-bit number from byte array; used for deserializing wand data  */
#define GET_TWO_BYTES(x)  (uint16_t)(((((uint8_t)((x)[1])) << 8) | (uint8_t)((x)[0])) & 0xFFFFu)

STATIC_ARTC uint8_t throttle_therapy_on = 0;

SMART_WAND_DATABASE db;

WAND_STATE tx_state;

UART_MemMapPtr SmartWandUART = UART0_BASE_PTR;

WAND_TX_PACKET_QUEUE packet_queue;

STATIC_LOG static_log;

extern legacy_wand_params legacy_wand_params_obj;
extern u8 AutoPrimeId;
extern prime_mode_t PrimeState;

/* FIR data structure */
arm_fir_instance_q15 wand_wear_fir;

IMPEDANCE_PARAMETERS *deserialize_ent_z_params(const uint8_t *data, IMPEDANCE_PARAMETERS *const params);
ENT_PRIME_PARAMS_T *deserialize_ent_prime_params(const uint8_t *data, ENT_PRIME_PARAMS_T *const params);
ENT_ABLATE_MODE_PARAMS_T *deserialize_ent_ablate_params(const uint8_t *data, ENT_ABLATE_MODE_PARAMS_T *params);
uint16_t get_contiguous_params(const uint8_t **data);
uint16_t dac_check(const uint16_t dac);
float pump_spd_check(float spd);
ENT_COAG_MODE_PARAMS_T *deserialize_ent_coag_params(const uint8_t *data, ENT_COAG_MODE_PARAMS_T params[]);
SCOPE_SAVER_PARAMETERS *deserialize_ent_ipeak_params(const uint8_t *data, SCOPE_SAVER_PARAMETERS *const params);
ABLATE_RAMP_RATE_PARAMS_T *deserialize_ramp_rate_params(const uint8_t *data, ABLATE_RAMP_RATE_PARAMS_T *const params);
static uint8_t current_page_static_log;
bool sm_wand_disconnect = FALSE;
bool static_log_retry = FALSE;
bool soft_reboot_detected = FALSE;
manta_prime prime;
IMPEDANCE_PARAMETERS *deserialize_manta_z_params(const uint8_t *data, IMPEDANCE_PARAMETERS *const params);
MANTA_PRIME_PARAMS_T *deserialize_manta_prime_params(const uint8_t *data, MANTA_PRIME_PARAMS_T *const params);
uint16_t *deserialize_manta_coag_params(const uint8_t *data, uint16_t params[]);
SCOPE_SAVER_PARAMETERS *deserialize_manta_ipeak_params(const uint8_t *data, SCOPE_SAVER_PARAMETERS *const params);
extern uint8_t current_coag_setpoint;
extern uint8_t change_flow_manta(uint8_t coag_level, uint8_t new_flow);
uint16_t manta_pwm_mod;
uint16_t manta_pwm_C0V;
uint16_t manta_pwm_C1V;
uint16_t manta_pwm_C2V;
uint16_t manta_pwm_C3V;
uint16_t manta_pwm_C4V;
uint16_t manta_pwm_C5V;
uint16_t manta_pwm_C6V;
uint16_t manta_pwm_C7V;
bool manta_pwm_values_valid = FALSE;

uint16_t primary_voltage_current_limit[4];
uint16_t secondary_voltage_current_limit[4];
extern uint8_t flow_val_apl_ent_bkp;
extern u8 AutoPrimeTimeoutId;
extern u8 PrimeId;


/***************************************************************************//**
Send command to the Smart Wand instrument.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void smartwand_send_packet(void)
{
  
  uint8_t byte = packet_queue.cmd[0].txcount;
  uint8_t packet_size = packet_queue.cmd[0].size - 1;
  
  /* check is there is data queue'd up */  
  if (packet_queue.idx == 0) 
  {
    
    SmartWandUART->C2 &= ~UART_C2_TIE_MASK;
    
  }
  else 
  {
    
    /* fill buffer w/ data */
    while ((byte < packet_size) &&
           ((UART0_TCFIFO & UART_TCFIFO_TXCOUNT_MASK) < 7))
    {
      
      SmartWandUART->D = (uint8_t) packet_queue.cmd[0].data[byte];
      byte++;
      
    }
    
    /* read S1 & write last byte to clear TDRE */
    if ((SmartWandUART->S1 & UART_S1_TDRE_MASK) == UART_S1_TDRE_MASK)
    {
      
      SmartWandUART->D = (uint8_t) packet_queue.cmd[0].data[byte];
      byte++;
      
    }
    
    packet_queue.cmd[0].txcount = byte;             
    
    /* maintain fifo if packet complete */
    if (byte >= packet_queue.cmd[0].size)
    {
      
      memmove(&packet_queue.cmd[0],&packet_queue.cmd[1],64*packet_queue.idx);
      
      packet_queue.idx--;
      
    }
    
  }
  
}

/***************************************************************************//**
Construct packet to transmit.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint32_t smartwand_prepare_tx_packet(uint8_t cmd, uint8_t idx, uint8_t *pdata, uint8_t size)
{
  
  uint8_t checksum = 0;
  volatile TX_PACKET tx_packet;
  
  tx_packet.head = PACKET_HEAD_TX;
  tx_packet.type = PACKET_TYPE;
  checksum = tx_packet.head;
  checksum += tx_packet.type;
  tx_packet.number = 0;
  checksum += tx_packet.number;
  tx_packet.size = size;
  checksum += tx_packet.size;
  tx_packet.cmd = cmd;
  checksum += tx_packet.cmd;
  tx_packet.idx = idx;
  checksum += tx_packet.idx;
  if (size > 0)
  {
    for (int i = 0; i < size; i++)
    {
      tx_packet.data[i] = *pdata++;
      checksum += tx_packet.data[i];
    }
    tx_packet.data[size] = (~checksum) + 1;
  }
  else
  {
    tx_packet.data[0] = (~checksum) + 1;
  }
  
  
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();
  packet_queue.cmd[packet_queue.idx].size = sizeof(TX_PACKET) - MAX_TX_DATA_BYTES + tx_packet.size;
  memcpy(&packet_queue.cmd[packet_queue.idx].data[0],(uint8_t*)&tx_packet,packet_queue.cmd[packet_queue.idx].size);      
  packet_queue.cmd[packet_queue.idx].txcount = 0;
  if (packet_queue.idx < 10)
  {
    
    packet_queue.idx++;
    
  }
  else
  {
    
    packet_queue.idx = 0;
    
  }
  UART0_C2 |= UART_C2_TIE_MASK; 
  __set_interrupt_state(s);
  
  return 0;
  
}

/***************************************************************************//**
Configure Universal Asynchronous Receiver/Transmitter Port: 0 (UART0)
UART0 is used as an RS-232 serial communication port for the smart wand.
@param void
@return 0
@outputs
@context
NOTE: UART0 is clocked off core clock (148MHz).  Baud rate calculated using
the following method 9,600.1 = 148 MHz / [16 * (312 + 0.53125)]
*******************************************************************************/
uint8_t smartwand_uart_init(void)
{
  
  uint8_t temp;
  
  /* Target baud rate */
  uint16_t rs232_baud = 9600;
  
  /* Calculated set baud rate register value */
  uint16_t sbr;
  
  /* Enable gate clock */
  SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
  
  /* Disable transmitter and receiver while settings are modified. */
  UART0_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
  
  /* Configure UART default 8-bit mode, no parity */
  UART0_C1 = 0;
  
  /* Set baud rate register to 625 */
  sbr = (uint16_t) ((CORE_CLK_MHZ * 1e6) / (rs232_baud * 16));
  
  /* Store current value of the UARTx_BDH except for the SBR field */
  temp = UART_BDH_REG(UART0_BASE_PTR) & ~(UART_BDH_SBR(0x1F));
  UART0_BDH = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
  UART0_BDL = (uint8_t) (sbr & UART_BDL_SBR_MASK);
  
  /* Store current value of the UARTx_C4 register except for the BRFA field */
  temp = UART_C4_REG(UART0_BASE_PTR) & ~(UART_C4_BRFA(0x1F));
  
  /* Set baud rate fine adjustment to 17/32 = 0.53125 */
  UART0_C4 = temp | UART_C4_BRFA(17);
  
  /* Enable transmit and receive DMA requests */
  //UART0_C5 |= UART_C5_RDMAS_MASK;// | UART_C5_TDMAS_MASK;
  
  /* Enable transmit buffer */
  UART0_PFIFO |= UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK;
  
  /* Flush transmit and receive buffers */
  UART0_CFIFO |= UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK;
  
  /* Enable transmitter */
  UART0_C2 = UART_C2_TE_MASK;
  
  /* Enable receiver & interrupt */
  UART0_C2 |= UART_C2_RIE_MASK | UART_C2_RE_MASK;
  
  /* Configure UART0_TX pin */
  PORTB_PCR17 = PORT_PCR_MUX(3);
  
  /* Configure UART0_RX pin */
  PORTB_PCR16 = PORT_PCR_MUX(3);
  
   NVIC_EnableIRQ(UART0_RX_TX_IRQn);
  
  return 1;
  
}

/***************************************************************************//**
Service interrupt triggered after wand comm times out. Also clears Priming if 
wand disconnected while Prime in progress.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t smartwand_timeout(void) 
{
  
  
  /* Flush transmit and receive buffers */
  UART0_CFIFO |= UART_CFIFO_RXFLUSH_MASK;
  
  /* Toggle red indicator */
  GPIOA_PTOR = RED_LED;
  
  /* Update therapy settings effect by wand comm timeout */
  if (current_therapy_settings.wand.timeout < 10) 
  {
    
    if (current_therapy_settings.wand.timeout == 1)
    {
      sm_wand_disconnect = TRUE;
      if (PrimeState != PRIME_OFF)
      {
        if (PrimeState == PRIME_AUTO)
        {
          gui_clear_auto_prime_event();
        }
        pump_set_speed(lw_get_pump_speed(0));
        //prime.release_detected = TRUE;
        PrimeState = PRIME_OFF;
        PrimeId = 0U;
        AutoPrimeId = 0U;
      }
    }
    current_therapy_settings.wand.timeout++;
    
    return 0;
  }
  
  else
  {
    current_therapy_settings.wand.unlocked = FALSE;     
    current_therapy_settings.wand.valid = FALSE;
    current_therapy_settings.gui.graphics.GraphicNumber = 0;
    current_therapy_settings.wand.vac_key = 0;
    current_therapy_settings.wand.cut_key = 0;
    current_therapy_settings.wand.coag_key = 0;
    current_therapy_settings.wand.mode_key = 0;
    current_therapy_settings.wand.handle_temp = 0;
    current_therapy_settings.wand.arm_buttons = 0;
    if (legacy_wand_params_obj.wand_connected == 0) 
    {
      current_therapy_settings.ambient.capable = 1;
      current_therapy_settings.ambient.enable = current_therapy_settings.ambient.enable_user_pref;
    }
    
    
    /* erase entire record */
    memset(&db.passcode,0,sizeof(SMART_WAND_DATABASE));
    
    current_therapy_settings.active_wand.smart_wand = UNKNOWN_GEN;
    
    put_event(&evt_tbl,WAND_UNPLUGGED);
    // To make sure the ambient is enabled even if the WW wand is not plugged in
    smart_wand_params.mfg.ambient_enable = 1;
    return 1;
  }
}

/***************************************************************************//**
Service interrupt triggered by received packet.  The context of this
function is limited to periodic packets only.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void smartwand_periodic_packet_handler(void)
{
  
  static uint8_t packet[SMART_WAND_PARAM_RX_PACKET_SIZE],byte;
  
  volatile uint8_t temp;
  
  static PACKET_STATE state = HEAD;
  static uint8_t checksum;
  
  uint8_t c;
#ifndef TEST
  /* Wait until character has been received */
  while ((UART0_S1 & UART_S1_RDRF_MASK)==UART_S1_RDRF_MASK)
  {
#endif
    c = UART0_D;
    
    switch (state)
    {
      case(HEAD):
        {
          byte = 0;
          
          if (c == 0x94)
          {
            packet[state] = c;
            checksum += packet[state];
            state = TYPE;
            
          }
          else
          {
            state = HEAD;
            checksum = 0;
            memset(&packet,0,sizeof(SMART_WAND_PARAM_RX_PACKET_SIZE));
          }
          break;
        }
        case(TYPE):
          {
            if (c == 0x10)
            {
              packet[state] = c;
              checksum += packet[state];
              state++;
            }
            else
            {
              state = HEAD;
              checksum = 0;
              memset(&packet,0,sizeof(SMART_WAND_PARAM_RX_PACKET_SIZE));
            }
            break;
          }
          case(SIZE):
            {
              
              packet[state] = c;
              checksum += packet[state];
              
              if (packet[state] > 0)
              {
                
                state = CMD;
                
              }
              
              else
              {
                
                state = CHECKSUM;
                
              }
              break;
              
            }
            case(CMD):
              {
                
                packet[state] = c;
                checksum += packet[state];
                state = IDX;
                break;
                
              }
              case(IDX):
                {
                  
                  /* strobe */
                  RESET_PIT(2);
                  packet[state] = c;
                  checksum += packet[state];
                  if (packet[SIZE] == 2)
                  {
                    state = CHECKSUM;
                  }
                  else
                  {
                    state = DATA;
                  }
                  byte = 0;
                  break;
                  
                }
                case(DATA):
                  {
                    
                    /* size greater than max indicates problem so start over */
                    if ((packet[SIZE] > MAX_TX_DATA_BYTES + 2) ||
                        (packet[SIZE] < 2))
                    {
                      
                      state = HEAD;
                      checksum = 0;
                      memset(&packet,0,sizeof(SMART_WAND_PARAM_RX_PACKET_SIZE));
                      
                    }
                    else if (byte < (packet[SIZE] - 3))
                    {
                      
                      packet[byte+state] = c;
                      checksum += packet[byte+state];
                      byte++;
                      
                    }
                    else
                    {
                      
                      packet[byte+state] = c;
                      checksum += packet[byte+state];
                      state = CHECKSUM;
                      
                    }
                    break;
                    
                  }
                  case(CHECKSUM):
                    {
                      
                      packet[byte+state] = c;
                      checksum = (~checksum) + 1;
                      if (checksum != packet[byte+state])
                      {
                        /*
                        
                                       if (packet[SIZE] == 0)
                                       {
                                          memset(&db.periodic,0,8);
                                       }
                                       else if (packet[CMD] == SMART_WAND_UNLOCK)
                                       {
                        
                                          memset(&db.passcode,0,SMART_WAND_PASSCODE_RX_PACKET_SIZE);
                        
                                       }
                                       else if (packet[CMD] == SMART_WAND_READ_PARAM_DATA)
                                       {
                        
                                          memset(&db.param[packet[IDX]],0,SMART_WAND_PARAM_RX_PACKET_SIZE);
                        
                                       }
                                       else if (packet[CMD] == SMART_WAND_WRITE_CIRC_LOG)
                                       {
                        
                                          memset(&db.circle_log,0,SMART_WAND_CIRC_LOG_WRITE_RX_PACKET_SIZE);
                        
                                       }
                                       else if (packet[CMD] == SMART_WAND_READ_MFG_DATA)
                                       {
                        
                                          memset(&db.mfg,0,SMART_WAND_MFG_PACKET_SIZE);
                        
                                       }
                        */
                        
                        
                      }
                      else
                      {
                        
                        /* strobe */
                        RESET_PIT(2);
                        
                        if (packet[SIZE] == 0)
                        {
                          
                          memset(&db.periodic,0,8);
                          packet[7] = checksum;
                          /* file away in database */
                          memcpy(&db.periodic,&packet[0],SMART_WAND_PERIODIC_PACKET_SIZE);
                          
                          put_event(&evt_tbl, WAND_PERIODIC_RX);
                          
                        }
                        
                        else if (packet[CMD] == SMART_WAND_READ_PARAM_DATA)
                        {
                          packet[SMART_WAND_PARAM_RX_PACKET_SIZE-1] = checksum;
                          memcpy(&db.param[packet[IDX]],&packet[0],SMART_WAND_PARAM_RX_PACKET_SIZE);
                          switch (packet[IDX])
                          {
                          case (0):
                            
                            put_event(&evt_tbl,WAND_PARAM_0_RX);
                            break;
                            
                          case (1):
                            
                            put_event(&evt_tbl,WAND_PARAM_1_RX);
                            break;
                            
                          case (2):
                            
                            put_event(&evt_tbl,WAND_PARAM_2_RX);
                            break;
                            
                          case (3):
                            
                            put_event(&evt_tbl,WAND_PARAM_3_RX);
                            break;
                            
                          case (4):
                            
                            put_event(&evt_tbl,WAND_PARAM_4_RX);
                            break;
                            
                          }
                          
                        }
                        
                        else if (packet[CMD] == SMART_WAND_READ_MFG_DATA)
                        {
                          
                          packet[73] = checksum;
                          memcpy(&db.mfg,&packet[0],SMART_WAND_MFG_PACKET_SIZE);
                          
                          put_event(&evt_tbl,WAND_MFG_RX);
                          
                        }
                        
                        else if (packet[CMD] == SMART_WAND_WRITE_CIRC_LOG)
                        {
                          
                          packet[9] = checksum;
                          memcpy(&db.circle_log.write_echo,&packet[0],SMART_WAND_CIRC_LOG_WRITE_RX_PACKET_SIZE);
                          put_event(&evt_tbl,WAND_CIRCLE_WRITE_RX);
                          
                        }
                        
                        else if (packet[CMD] == SMART_WAND_UNLOCK)
                        {
                                                 
                          packet[SMART_WAND_PASSCODE_RX_PACKET_SIZE-1] = checksum;
                          memcpy(&db.passcode,&packet[0],SMART_WAND_PASSCODE_RX_PACKET_SIZE);
                          put_event(&evt_tbl,WAND_PASSCODE_RX);
                          
                        }
                        
                        else if (packet[CMD] == SMART_WAND_READ_STATIC_LOG)
                        {
                          
                          packet[SMART_WAND_STATIC_READ_RX_PACKET_SIZE-1] = checksum;
                          memcpy(&db.static_log.page[packet[IDX]],&packet[0],SMART_WAND_STATIC_READ_RX_PACKET_SIZE);
                          memcpy(((uint8_t*)&static_log.type+(8*packet[IDX])),&db.static_log.page[packet[IDX]].data[0],8);
                          if ((current_page_static_log - 1) == packet[IDX]) 
                          {
                            static_log_retry = TRUE;
                          }
                          else
                          {
                            static_log_retry = FALSE;
                          }
                          
                          current_page_static_log = packet[IDX];
                          put_event(&evt_tbl,WAND_STATIC_READ_RX);
                          
                        } 
                        
                        else if (packet[CMD] == SMART_WAND_WRITE_STATIC_LOG)
                        {
                          
                          packet[SMART_WAND_STATIC_WRITE_RX_PACKET_SIZE-1] = checksum;
                          memcpy(&db.static_log.write_echo,&packet[0],SMART_WAND_STATIC_WRITE_RX_PACKET_SIZE);
                          put_event(&evt_tbl,WAND_STATIC_WRITE_RX);
                          
                        } 
                        
                        else if (packet[CMD] == SMART_WAND_READ_CIRC_LOG)
                        {
                          
                          packet[SMART_WAND_CIRC_LOG_READ_RX_PACKET_SIZE-1] = checksum;
                          memcpy(&db.circle_log.segment,&packet[0],SMART_WAND_CIRC_LOG_READ_RX_PACKET_SIZE);
                          put_event(&evt_tbl,WAND_CIRCLE_READ_RX);
                          
                        } 
                        
                        if (current_therapy_settings.wand.timeout > 0)
                        {
                          
                          current_therapy_settings.wand.timeout--;
                          
                        }
                        
                        /* flush rx buffer to rid of any queue'd data */
                        UART0_CFIFO |= UART_CFIFO_RXFLUSH_MASK;
                        
                      }
                      state = HEAD;
                      checksum = 0;
                      memset(&packet,0,sizeof(SMART_WAND_PARAM_RX_PACKET_SIZE));
                      break;
                      
                    }
    default:
      {
        packet[state] = c;
        checksum += packet[state];
        state++;
        break;
      }
      
    }
#ifndef TEST
  }
#endif
}

/***************************************************************************//**
Route valid packet signals to appropriate data structures.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wand_periodic_packet_parse(void)
{
  
  uint8_t ablate = 0,
  coag = 0,
  vac = 0;
  
  static uint8_t tube_overtemp_pause_cnt = 0,
  hysteresis_on = 0,
  no_ambient_packet_count = 0,
  no_handle_packet_count = 0,
  one_sec_count = 0;
  uint8_t PrimeRunning = get_priming_status();
  
  /* Count IRQ's with mode key pressed to ctrl VAC mode */
  static uint8_t mode_key_cnt = 0,
  mode_lock = 0;
  
  uint8_t cut_enable = smart_wand_params.mfg.low_enable |
    smart_wand_params.mfg.med_enable |
      smart_wand_params.mfg.hi_enable;
  uint8_t vac_enable = smart_wand_params.mfg.vac_enable;
  uint8_t coag_enable = smart_wand_params.mfg.coag_enable;
  
  tx_state = PERIODIC;
  
  /* Mode key status */
  uint8_t mode_key = 0;
  uint16_t ambient, handle;
  
  if ((current_therapy_settings.treatment.finger_switch_en == 1) &&
      (smart_wand_params.mfg.finger_switch_enable == 1))
  {
    
    /* Parse & copy packet contents into global structure */
    ablate = (uint8_t) ((db.periodic.key & ABLATE_KEY_MASK) == ABLATE_KEY_MASK) ? 1 : 0;
    coag = (uint8_t) ((db.periodic.key & COAG_KEY_MASK) == COAG_KEY_MASK) ? 1 : 0;
    /* Mode key is handled differently since duration will dictate function */
    mode_key = (uint8_t) ((db.periodic.key & MODE_KEY_MASK) == MODE_KEY_MASK) ? 1 : 0;     
    
    if (ablate + coag + mode_key == 0)
    {
      current_therapy_settings.wand.arm_buttons = 1;
    }
    
    if (current_therapy_settings.wand.arm_buttons == 0)
    {
      ablate = 0;
      coag = 0;
      mode_key = 0;
    }
    
  }
  
  handle = (uint16_t) db.periodic.temp1;
  
  if (db.periodic.key > 0)
  {
    
    GPIOE_PSOR = EXT_CTRL_2;
    
  }
  else
  {
    
    GPIOE_PCOR = EXT_CTRL_2;
    
  }
  
  
  if (smart_wand_params.mfg.ambient_enable == 1)
  {
    /* test if ambient sensor is connected */
    if (db.periodic.temp2 == 0)
    {
      
      no_ambient_packet_count++;
      
    }
    else
    {
      
      no_ambient_packet_count = 0;
      
    }
    
    /* trigger error if 0 for >1 sec */
    if (no_ambient_packet_count == 51)
    {
      
      put_event(&evt_tbl,NO_AMBIENT_SENSOR);
      no_ambient_packet_count = 0;
      
    }
    
    ambient = convert_q62_ambient_to_7bit(db.periodic.temp2);
    
  }
  
  if (smart_wand_params.mfg.tube_temperature_enable == 1)
  {
    /* test if handle sensor is connected */
    if (db.periodic.temp1 == 0)
    {
      
      no_handle_packet_count++;
      
    }
    else
    {
      
      no_handle_packet_count = 0;
      
    }
    
    /* trigger error if 0 for >1 sec */
    if (no_handle_packet_count == 51)
    {
      
      put_event(&evt_tbl,NO_AMBIENT_SENSOR);
      no_handle_packet_count = 0;
      
    }
  }
  
  /* update value to display once per second */
  if (one_sec_count == 50)
  {
    
    one_sec_count = 0;
    current_therapy_settings.ambient.smart_wand_temperature = ambient;
    if (((current_therapy_settings.wand.unlocked == TRUE) &&
         (current_therapy_settings.active_wand.lw_active == 0) &&
           (current_therapy_settings.wand.valid == TRUE)) ||
        ((current_therapy_settings.wand.unlocked == TRUE) &&
         (legacy_wand_params_obj.wand_connected == 0)))
    {
      if(current_therapy_settings.gui.screenID == RF_2000_WAND_TREATMENT_SCREEN)
      {
        current_therapy_settings.ambient.temperature = ambient;
      }
      
    }
    
  }
  else
  {
    
    one_sec_count++;
    
  }
  
  smart_wand_params.impedance.z_high_mod = z_limit_adjust4temp(db.periodic.temp2);
  
#ifdef DEBUG_CAN
  
  CANdebug.wand.handle = db.periodic.temp1;
  CANdebug.wand.ambient = db.periodic.temp2;
  CANdebug.wand.z_high_mod = smart_wand_params.impedance.z_high_mod;
  
#endif
  
  current_therapy_settings.wand.handle_temp = handle;
  current_therapy_settings.wand.thandle_wear = (bool)wand_thandle_wear_monitor(handle,0,0);
  
  if ((current_therapy_settings.treatment.ablate_on == 1) || (current_therapy_settings.treatment.coag_on == 1))
  {
    if (current_therapy_settings.wand.handle_temp >= smart_wand_params.handle.high_warn_en)
    {
      
      current_therapy_settings.wand.tube_alert = TRUE;
      current_therapy_settings.wand.tube_warn = TRUE;
      GPIOA_PCOR = RED_LED;
      
    }
    else if (((current_therapy_settings.wand.handle_temp >= smart_wand_params.handle.med_warn_en) ||
              (hysteresis_on == 1)) &&
             (current_therapy_settings.wand.tube_alert == FALSE))
    {
      hysteresis_on = 1;
      
      if (tube_overtemp_pause_cnt <= 50)
      {
        
        current_therapy_settings.wand.tube_warn_pulse = FALSE;
        current_therapy_settings.wand.tube_warn = FALSE;
        tube_overtemp_pause_cnt++;
        
      }
      else if (tube_overtemp_pause_cnt <= 100)
      {
        
        current_therapy_settings.wand.tube_warn_pulse = TRUE;
        current_therapy_settings.wand.tube_warn = TRUE;
        tube_overtemp_pause_cnt++;
        
      }
      else
      {
        
        tube_overtemp_pause_cnt = 0;
        
      }
      
    }
    else
    {
      
      tube_overtemp_pause_cnt = 0;
      
    }
  }
  else
  {
    
    tube_overtemp_pause_cnt = 0;
    current_therapy_settings.wand.tube_alert = FALSE;
    current_therapy_settings.wand.tube_warn_pulse = FALSE;
    current_therapy_settings.wand.tube_warn = FALSE;
    
  }
  
  if (current_therapy_settings.wand.handle_temp < smart_wand_params.handle.warn_disable)
  {
    
    hysteresis_on = 0;
    current_therapy_settings.wand.tube_alert = FALSE;
    current_therapy_settings.wand.tube_warn_pulse = FALSE;
    current_therapy_settings.wand.tube_warn = FALSE;
    
  }
  if (current_therapy_settings.active_wand.lw_active == 0) 
  {
    current_therapy_settings.ambient.thermometer_color = 
      thermometer_color(current_therapy_settings.ambient.temperature);
  }
  
  /* Increment mode key counter if pressed */
  if (mode_key)
  {
    if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA)
    {
      if(mode_key != current_therapy_settings.wand.mode_key) 
      {
        if ((throttle_therapy_on == 0) && (coag_enable == 1) && (PrimeRunning == 0) && 
      (current_therapy_settings.treatment.pre_prime == FALSE))
        {
          put_event(&evt_tbl,THERAPY_ON);
          throttle_therapy_on = 15;
        }
        else if (PrimeRunning == PRIME_AUTO) 
        {
          gui_stop_auto_prime();
          current_therapy_settings.treatment.pre_prime = FALSE;
          prime.blink = FALSE;
          AutoPrimeId = 0U;
        }
      }
    }
    else
    {
      if (mode_key_cnt < 255)
      {
        mode_key_cnt++;
      }
    }
    
  }
    else if ((mode_key == 0) && (current_therapy_settings.treatment.ent_turbo_mode == TRUE))
    {
      DISABLE_AUDIO;
      FTM2_MOD = 1678-1;
      put_event(&evt_tbl,THERAPY_OFF);
      current_therapy_settings.treatment.ent_turbo_mode = FALSE;
    }
    /* When mode key not pressed & count isn't > delay mode key is intended */
    else if ((mode_key_cnt > 1) && (mode_key_cnt <= VAC_MODE_PACKET_DELAY) && (mode_lock == 0) &&
             (cut_enable == 1)) 
    {
      
      current_therapy_settings.wand.mode_key = 0;
      put_event(&evt_tbl,CHANGE_MODE);
      mode_key_cnt = 0;
    }
    else if (mode_key_cnt > 16)
    {
      
      current_therapy_settings.wand.mode_key = 0;
      mode_key_cnt--;
      
    }
    else
    {
      
      current_therapy_settings.wand.mode_key = 0;
      mode_key_cnt = 0;
      mode_lock = 0;
      
    }
    
    /* So long as mode key is held pressed longer than delay, stay in VAC mode */
    if ((mode_key_cnt > VAC_MODE_PACKET_DELAY) ||
        ((mode_lock == 1) && (current_therapy_settings.wand.mode_key == 1)))
    {
      
      /* Limit counter value to one more than max value that changes mode*/
      mode_key_cnt = VAC_MODE_PACKET_DELAY + 1;
      mode_lock = 1;
      vac = 1;
      
    }
    else
    {
      
      vac = 0;
      
    }
    
    
    
    if (ablate != current_therapy_settings.wand.cut_key)
    {
      if ((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
          (current_therapy_settings.treatment.pre_prime == FALSE))
      {
        if ((ablate == 1) &&
            (throttle_therapy_on == 0) &&
              (cut_enable == 1))
        {
          
          put_event(&evt_tbl,THERAPY_OFF);
        }
      }
      else if((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) && (PrimeRunning == PRIME_AUTO))
      {
        gui_stop_auto_prime();
        current_therapy_settings.treatment.pre_prime = FALSE;
        prime.blink = FALSE;
        AutoPrimeId = 0U;
      }   
      else if (smart_wand_params.mfg.indication != SMART_WAND_MFG_INDICATION_MANTA)
      {
        if ((ablate == 1) &&
            (throttle_therapy_on == 0) &&
              (cut_enable == 1))
        {
          
          put_event(&evt_tbl,THERAPY_ON);
          throttle_therapy_on = 15;
          
        }
        else if (ablate == 0)
        {
          
          put_event(&evt_tbl,THERAPY_OFF);
          
        }
      }
    }
    
    if (coag != current_therapy_settings.wand.coag_key)
    {
      if((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
         (current_therapy_settings.treatment.pre_prime == FALSE))
      {
        if ((coag == 1) &&
            (throttle_therapy_on == 0) &&
              (coag_enable == 1))
        {
          
          put_event(&evt_tbl,THERAPY_ON);
          throttle_therapy_on = 15;
          
        }
        else if (coag == 0)
        {
          
          put_event(&evt_tbl,THERAPY_OFF);
          
        }
      }
      else if((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) && (PrimeRunning == PRIME_AUTO))
      {
        gui_stop_auto_prime();
        current_therapy_settings.treatment.pre_prime = FALSE;
        prime.blink = FALSE;
        AutoPrimeId = 0U;
      } 
      else if (smart_wand_params.mfg.indication != SMART_WAND_MFG_INDICATION_MANTA)
      {
        
        if ((coag == 1) &&
            (throttle_therapy_on == 0) &&
              (coag_enable == 1))
        {
          
          put_event(&evt_tbl,THERAPY_ON);
          throttle_therapy_on = 15;
          
        }
        else if (coag == 0)
        {
          
          put_event(&evt_tbl,THERAPY_OFF);
          
        }
      }
      
    }
    
    if ((vac != current_therapy_settings.wand.vac_key) && !(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA))
    {
      
      if ((vac == 1) &&
          (vac_enable == 1))
      {
        
        put_event(&evt_tbl,THERAPY_ON);
        
      }
      else
      {
        
        put_event(&evt_tbl,THERAPY_OFF);
        
      }
      
    }
    current_therapy_settings.wand.cut_key = ablate;
    current_therapy_settings.wand.coag_key = coag;
    current_therapy_settings.wand.vac_key = vac;
    current_therapy_settings.wand.mode_key = mode_key;
    
    if (throttle_therapy_on > 0)
    {
      
      throttle_therapy_on--;
      
    }
    
    return 0;
    
  }
  
  /***************************************************************************//**
  validate wand
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_validate(void)
  {  
    
    uint8_t checksum_1 = 0, checksum_2 = 0;
    
    db.static_log.page_read = 0;
    
    for (uint8_t j = 0; j <= PARAM_PAGE_4; j++)
    {
      
      for (uint8_t i = 0; i < PARAM_PAGE_SIZE; i++)
      {
        
        checksum_1 += db.param[j].data[i];
        
      }
      
    }
    
    /* subtract downloaded checksum from sum b/c it is not included */
    checksum_1 = (uint8_t)(checksum_1 - db.param[4].data[63]);
    checksum_1 = ~(checksum_1) + 1;
    
    /* mfg data only one page so summation should skip last byte */
    for (uint8_t i = 0; i < MFG_DATA_SIZE-1; i++)
    {
      
      checksum_2 += db.mfg.data[i];
      
    }

    checksum_2 = ~(checksum_2) + 1;
    
    /* Only save indication value if valid MFG and CFG file downloaded*/
    if((checksum_1 == db.param[4].data[63]) &&
      (checksum_2 == db.mfg.data[63]))
    {
      smart_wand_params.mfg.indication = db.mfg.data[46];
    }
    
    
    if (checksum_1 != db.param[4].data[63])
    {
      
      put_event(&evt_tbl,WAND_ID_ERROR);
      tx_state = PERIODIC;
      
    }
    else if (checksum_2 != db.mfg.data[63])
    {
      
      put_event(&evt_tbl,WAND_ID_ERROR);
      tx_state = PERIODIC;
      
    }
    /* check sports med wand */
    else if (smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_SPORTS_MED)
    {
      current_therapy_settings.active_wand.smart_wand = SPORTS_MED;
      extract_wand_params();
      tx_state = STATIC_LOG_READ;    
      if (current_therapy_settings.setpoint.coag > COAG_2)
      {
        current_therapy_settings.setpoint.coag = COAG_1;
      }
    }
    else if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_ENT)
    {
      current_therapy_settings.active_wand.smart_wand = ENT;
      extract_wand_params();
      tx_state = STATIC_LOG_READ;
    }
    else if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA)
    {
      current_therapy_settings.active_wand.smart_wand = ARTHROPLASTY;
      extract_wand_params();
      tx_state = STATIC_LOG_READ;
    }
    else
    {
      put_event(&evt_tbl,WAND_ID_ERROR);
      tx_state = PERIODIC;
    }
    
    return 1;
    
  }
  
  /***************************************************************************//**
  download wand parameters from page 0
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_download_parm_pg_0(void)
  {
    
    tx_state = PARAM_0;
    
    GPIOA_PCOR = BLUE_LED;
    
    /* following unlock we jump right to treatment screen so show a temperature (default is green) */

    if (legacy_wand_params_obj.wand_connected == 0) 
    {
      current_therapy_settings.ambient.capable = 0;
    }
    current_therapy_settings.wand.unlocked = TRUE;
    
    /* time stamp initial plug-in */
    current_therapy_settings.log.start_time = (*(uint32_t*)0x4003D000);
    
    
    
    
    return 1;
    
  }
  
  /***************************************************************************//**
  download wand parameters from page 1
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_download_parm_pg_1(void)
  {
    
    tx_state = PARAM_1;
    
    return 1;
    
  }
  
  /***************************************************************************//**
  download wand parameters from page 2
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_download_parm_pg_2(void)
  {
    
    tx_state = PARAM_2;
    
    return 1;
    
  }
  
  /***************************************************************************//**
  download wand parameters from page 3
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_download_parm_pg_3(void)
  {
    
    tx_state = PARAM_3;
    
    return 1;
    
  }
  
  /***************************************************************************//**
  download wand parameters from page 4
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_download_parm_pg_4(void)
  {
    
    tx_state = PARAM_4;
    
    return 1;
    
  }
  
  /***************************************************************************//**
  unlocks the wand
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_unlock(void)
  {
    current_therapy_settings.wand.valid = FALSE;
    tx_state = UNLOCK;
    
    if(sm_wand_disconnect == TRUE)
    {
      memset(&db.circle_log.mode_active_accum,0,64);
      tx_state = PERIODIC;
      sm_wand_disconnect = FALSE;
    }
    
    return 1;
  }
  
  /***************************************************************************//**
  Locks the wand
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_lock(void)
  {
    
    tx_state = LOCK;
    
    if ((legacy_wand_params_obj.wand_connected) &&
        (current_therapy_settings.wand.unlocked == FALSE)&&
          (current_therapy_settings.gui.screenID != LEGACY_6_8_WAND_TREATMENT_SCREEN))
    {
      switch_to_legacy_wand();
    }
    
    return 1;
    
  }
  /***************************************************************************//**
  Read the smart wand static log.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_static_read(void)
  {
    
    uint8_t static_log_hash = 'A';
    uint8_t wand_used = 0;
    uint16_t csum = 0;
    uint8_t bin = 0;
    
    /* determine boot loader or log data */
    if (static_log.type != static_log_hash)
    {
      
      tx_state = ERASE_STATIC_LOGS;
      memset(&db.static_log.write_echo,0,SMART_WAND_STATIC_WRITE_RX_PACKET_SIZE);
      db.static_log.page_read = 0;
      
    }
    else if (db.static_log.page[0].data[0] == static_log_hash)
    {
      
      if (db.static_log.page_read == STATIC_MAX_NUM_PAGES)
      {
        
        smart_wand_params.mfg.use_limit_time = set_use_limit_time(&static_log.unit[0].use_time);
        wand_used = wand_use_limit_check();
        wand_used |= wand_expired_check();
        wand_used |= wand_time_limit_check(&static_log.mode_active_time[0],&csum);
        
        if (wand_used == 1)
        {
          
          tx_state = PERIODIC;
          
        }
        else
        {
          
          tx_state = STATIC_LOG_WRITE_1;
          memcpy(&static_log.original_mode_active_time[0],&static_log.mode_active_time[0],16);
          /* Clearing any residual active time values stored in the controller */
          for (bin = 0; bin <= 15; bin++)
          {
            if((db.circle_log.mode_active_accum[bin] >= 15) && soft_reboot_detected)
            {
              soft_reboot_detected = FALSE;
              db.circle_log.mode_active_accum[bin] = 0;
            }
          }
          wand_mode_life_histogram(&db.circle_log.mode_active_accum[0],&static_log.original_mode_active_time[0]);
          
        }
        
      }
      else
      {
        
        if (!static_log_retry)
        {
          db.static_log.page_read++;
        }
        current_page_static_log++;
        tx_state = STATIC_LOG_READ;
        
      }
      
    }
    
    return 0;
    
    
    
  }
  /***************************************************************************//**
  Write to the smart wand static log.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_static_write(void)
  {
    
    
    if ((db.static_log.page_read == STATIC_MAX_NUM_PAGES) &&
        (db.static_log.write_echo.idx == db.static_log.page_write + 1))
    {
      
      tx_state = CIRCLE_LOG_READ;
      
    }
    
    else if ((db.static_log.page_read == STATIC_MAX_NUM_PAGES) &&
             (db.static_log.write_echo.idx == db.static_log.page_write))
    {
      
      tx_state = STATIC_LOG_WRITE_2;
      
    }
    
    else if (db.static_log.write_echo.idx == 1) 
    {
      
      tx_state = ERASE_CIRCLE_LOGS;
      
    }
    
    else if (db.static_log.write_echo.idx == 0) 
    {
      
      tx_state = STATIC_LOG_READ;
      db.static_log.page_read = 0;
      
    }
    
    return 0;
    
  }
  /***************************************************************************//**
  Write to the smart wand circle log.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_circle_write(void)
  {
    
    if (db.circle_log.write_echo.idx == 0) 
    {
      
      tx_state = SWITCH_BOOT_TO_LOG;
      
    }
    
    return 0;
    
  }
  
  /***************************************************************************//**
  Read from the smart wand circle log.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_circle_log_read(void)
  {
    
    GPIOA_PSOR = BLUE_LED;
    
    tx_state = PERIODIC;
    current_therapy_settings.wand.valid = TRUE;
    
    return 1;
    
  }
  /***************************************************************************//**
  Logs wand usage data
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  void log_use_data(uint32_t start_time)
  {
    
    uint8_t active_count = 0;
    uint8_t idle_duration = 0;
    uint8_t bin;
    
    uint16_t csum = 0;
    
    LOG_TIME_UNITS time_units;
    
    const uint8_t mode_level_bins[NUM_OF_MODES][3] = {
      /* n/a  */ {0,0,0},
      /* lo   */ {1,2,3},
      /* med  */ {4,5,6},
      /* hi   */ {7,8,9},
      /* vac  */ {10,11,12},
      /* coag */ {13,14,15} 
    };
    
    /* stop time and duration of activation */
    uint32_t stop_time,
    duration_seconds,
    idle_sec,
    idle_hours,
    idle_five_sec,
    idle_minutes;
    
    const uint16_t MAX_STATIC_DURATION = 3825;
    
    /* copy RTC_TSR to ram */
    stop_time = (*(uint32_t*)0x4003D000);
    
    duration_seconds = stop_time - start_time;
    idle_sec = start_time - current_therapy_settings.log.start_time;
    
    /* max minutes is 63.75 = 3825 sec */
    if (duration_seconds > MAX_STATIC_DURATION)
    {
      
      duration_seconds = MAX_STATIC_DURATION;
      
    }
    
    /* determine which bin for data storage */
    bin = mode_level_bins[current_therapy_settings.log.mode][current_therapy_settings.log.level];
    
    if (bin > 0)
    {
      
      bin -= 1;
      
    }
    
    db.circle_log.mode_active_accum[bin] += duration_seconds;
    
    /* 15 sec is spec'd threshold for one count */
    if (db.circle_log.mode_active_accum[bin] >= MAX_STATIC_DURATION)
    {
      
      active_count = UCHAR_MAX;
      
    }
    else if (db.circle_log.mode_active_accum[bin] >= 15)
    {
      
      active_count =  db.circle_log.mode_active_accum[bin] / 15;
      
    }
    
    /* add to history */
    if ((UCHAR_MAX - active_count) < static_log.original_mode_active_time[bin])
    {
      
      static_log.mode_active_time[bin] = UCHAR_MAX;
      
      
    }
    else
    {
      static_log.mode_active_time[bin] = 
        (active_count + static_log.original_mode_active_time[bin]);
    }
    
    /* log static usage data */
    smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG,
                                0,
                                (uint8_t*)&static_log.type,
                                STATIC_PAGE_SIZE);
    smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG,
                                1,
                                (uint8_t*)&static_log.mode_active_time[7],
                                STATIC_PAGE_SIZE);
    smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG,
                                2,
                                (uint8_t*)&static_log.mode_active_time[15],
                                STATIC_PAGE_SIZE);
    
    /* compress idle time duration into 1 byte */
    idle_hours = idle_sec / 3600U;
    idle_five_sec = idle_sec / 5U;
    idle_minutes = idle_sec / 60U;
    
    if (idle_sec <= 0x3F)
    {
      
      time_units = LOG_UNIT_SECONDS;
      idle_duration = (uint8_t) idle_sec & 0x3F;
      
    }
    else if (idle_five_sec <= 0x3F)
    {
      
      time_units = LOG_UNIT_5_SECONDS;
      idle_duration = (uint8_t) idle_five_sec & 0x3F;
      
    }
    else if (idle_minutes <= 0x3F)
    {
      
      time_units = LOG_UNIT_MINUTES;
      idle_duration = (uint8_t) idle_minutes & 0x3F;
      
    }
    else if (idle_hours <= 0x3F)
    {
      
      time_units = LOG_UNIT_HOURS;
      idle_duration = (uint8_t) idle_hours & 0x3F;
      
    }  
    
    if (db.circle_log.idx == 0)
    {
      
      db.circle_log.segment.data[0] = (((idle_duration << 2) | time_units) & 0xFF);
      db.circle_log.segment.data[1] = bin;
      memcpy(&db.circle_log.segment.data[2],&duration_seconds,sizeof(uint16_t));
      
    }
    else
    {
      db.circle_log.segment.data[4] = (((idle_duration << 2) | time_units) & 0xFF);
      db.circle_log.segment.data[5] = bin;
      memcpy(&db.circle_log.segment.data[6],&duration_seconds,sizeof(uint16_t));
    }
    
    
    wand_time_limit_check(&static_log.mode_active_time[0],&csum);

    /* only update circle log if 15 sec accumulated activation */
    if (csum >= 15)
    {
      
      smartwand_prepare_tx_packet(SMART_WAND_WRITE_CIRC_LOG,
                                  db.circle_log.page,
                                  &db.circle_log.segment.data[0],
                                  CIRC_LOG_PAGE_SIZE);
      
      
      if (db.circle_log.page == 192)
      {
        
        db.circle_log.roll = 1;
        db.circle_log.page = 0;
        db.circle_log.idx = 0;
        
      }
      
      
      /* new end index */
      static_log.unit[db.static_log.unit_num].end_idx = (db.circle_log.page << 1) + db.circle_log.idx;
      
      if ((static_log.unit[db.static_log.unit_num].end_idx == static_log.unit[db.static_log.unit_num].start_idx) &&
          (db.circle_log.roll == 1))
      {
        
        static_log.unit[db.static_log.unit_num].start_idx++;
        
      }
      
      if (static_log.unit[db.static_log.unit_num].start_idx > 383)
      {
        
        static_log.unit[db.static_log.unit_num].start_idx = 0;
        
      }
      
      smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG,
                                  db.static_log.page_write,
                                  (uint8_t*)&static_log.unit[db.static_log.unit_num].id,
                                  STATIC_PAGE_SIZE);
      
      /* write new end index */
      smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG,
                                  (db.static_log.page_write + 1),
                                  (uint8_t*)&static_log.unit[db.static_log.unit_num].use_time,
                                  STATIC_PAGE_SIZE);
      
      
      if ((db.circle_log.page < 192) && (db.circle_log.idx == 1))
      {
        
        db.circle_log.page++;
        db.circle_log.idx = 0;
        
      }
      else if ((db.circle_log.page < 192) && (db.circle_log.idx == 0))
      {
        
        db.circle_log.idx = 1;
        
      }
      
    }
    
    /* update start time since last activation */
    current_therapy_settings.log.start_time = (*(uint32_t*)0x4003D000);
    
    if (current_therapy_settings.wand.worn_latch == 1)
    {
      
      put_event(&evt_tbl,CHECK_ELECTRODE_ERROR);
      
    }
    
    wand_mode_life_histogram(&db.circle_log.mode_active_accum[0],&static_log.original_mode_active_time[0]);

    if (csum >= 15)
    {
      
      current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][0] = STATIC_LOG_THERAPY_DATA;
      current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][1] = (SDHC_LOGGING_CMDS)(((idle_duration << 2) | time_units) & 0xFF);
      current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][2] = (SDHC_LOGGING_CMDS)bin;
      current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][3] = (SDHC_LOGGING_CMDS)(uint8_t)(duration_seconds & 0x00FF);
      current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][4] = (SDHC_LOGGING_CMDS)(uint8_t)((duration_seconds & 0xFF00) >> 8);
      current_therapy_settings.sdhc.cmd_que_idx++;
      
    }
    
  }
  
  /***************************************************************************//**
  Checks to see if the wand is within the use limit bounds.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_use_limit_check(void)
  {
    
    uint32_t rtc_sec = (*(uint32_t*)0x4003D000);
    uint8_t error = 0;
    
    if ((smart_wand_params.mfg.use_limit_time_en == 1) &&
        (current_therapy_settings.wand.valid == TRUE))
    {
      
      if (rtc_sec > smart_wand_params.mfg.use_limit_time)
      {
        
        put_event(&evt_tbl,WAND_USE_LIMIT_ERROR);
        error = 1;
        
      } 
      
      for (uint8_t i = 0; i < MAX_STATIC_LOG_UNITS; i++)
      {
        
        if (static_log.unit[i].errors == WAND_LIFE_ERROR_TXT)
        {
          
          put_event(&evt_tbl,WAND_USE_LIMIT_ERROR);
          error = 1;
          
        }
        
      }
      
      /* check last record in case its been overwritten by current gen */
      if (db.static_log.page[11].data[5] == WAND_LIFE_ERROR_TXT)
      {
        
        put_event(&evt_tbl,WAND_USE_LIMIT_ERROR);
        error = 1;
        
      }
      
    }
    
    return error;
    
  }
  
  /***************************************************************************//**
  Checks to see if the wand is within the use limit time bounds.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_time_limit_check(uint8_t* active_time, uint16_t* p_csum)
  {
    
    uint8_t bin = 0;
    uint16_t csum_ablate = 0;
    uint16_t csum_coag = 0;
    uint8_t error = 0;
    
    for (bin = 0; bin < 12; bin++)
    {
      
      csum_ablate += *active_time++;
      
    }
    
    csum_ablate *= 15;
    
    /* verify wand active life has not expired */
    if (smart_wand_params.mfg.use_limit_activation_en == 1)
    {
      
      if (csum_ablate > smart_wand_params.mfg.max_activation_seconds)
      {
        
        put_event(&evt_tbl,WAND_USE_LIMIT_ERROR);
        error = 1;
        
      }
      
    }
    
    for (bin = 12; bin < 16; bin++)
    {
      
      csum_coag += *active_time++;
      
    }
    
    csum_coag *= 15;
    
    /* verify wand active life has not expired */

    if (( db.mfg.data[46] == SMART_WAND_MFG_INDICATION_MANTA) && 
      (smart_wand_params.mfg.use_limit_activation_en == 1))
    {
      if (csum_coag > smart_wand_params.mfg.max_activation_seconds)
      {
        
        put_event(&evt_tbl,WAND_USE_LIMIT_ERROR);
        error = 1;
      }
      
    }
    
    *p_csum = (csum_ablate + csum_coag);
    
    return error;
    
  }
  
  /***************************************************************************//**
  Checks to see if wand is expired.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_expired_check(void)
  {
    
    uint8_t error = 0;
    
    if (smart_wand_params.mfg.expiration_date < (*(uint32_t*)0x4003D000))
    {
      
      put_event(&evt_tbl,WAND_EXPIRED);
      error = 1;
      
    }
    
    return error;
    
  }
  
  
  
  /***************************************************************************//**
  transmit data to wand
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_tx(void)
  {
    
    
    switch(tx_state)
    {
      
      uint8_t data[MAX_TX_DATA_BYTES];
      
      case(LOCK):
        {
          
          memset(&db.mfg,0,SMART_WAND_MFG_PACKET_SIZE);
          memset(&packet_queue,0,sizeof(packet_queue));
          UART0_CFIFO |= UART_CFIFO_TXFLUSH_MASK;
          smartwand_prepare_tx_packet(SMART_WAND_LOCK, 0, &data[0], 0);
          break;
          
        }
        case(UNLOCK):
          {
            
            uint8_t passcode = 0;
            memset(&db.passcode,0,SMART_WAND_PASSCODE_RX_PACKET_SIZE);
            
            for (int i = 0; i < 14; i++)
            {
              
              passcode ^= db.mfg.data[i];
              
            }
            
            smartwand_prepare_tx_packet(SMART_WAND_UNLOCK, passcode, &data[0], 0);
            break;
            
          }
          case(PARAM_0):
            {
              
              uint8_t page = 0;
              smartwand_prepare_tx_packet(SMART_WAND_READ_PARAM_DATA, page, &data[0], 0);
              break;
              
            }
            case(PARAM_1):
              {
                
                uint8_t page = 1;
                smartwand_prepare_tx_packet(SMART_WAND_READ_PARAM_DATA, page, &data[0], 0);
                break;
                
              }
              case(PARAM_2):
                {
                  
                  uint8_t page = 2;
                  smartwand_prepare_tx_packet(SMART_WAND_READ_PARAM_DATA, page, &data[0], 0);
                  break;
                  
                }
                case(PARAM_3):
                  {
                    
                    uint8_t page = 3;
                    smartwand_prepare_tx_packet(SMART_WAND_READ_PARAM_DATA, page, &data[0], 0);
                    break;
                    
                  }
                  case(PARAM_4):
                    {
                      
                      uint8_t page = 4;
                      smartwand_prepare_tx_packet(SMART_WAND_READ_PARAM_DATA, page, &data[0], 0);
                      break;
                      
                    }
                    case(STATIC_LOG_READ):
                      {
                        
                        memset(&db.static_log.page[current_page_static_log],0,SMART_WAND_STATIC_READ_RX_PACKET_SIZE);
                        smartwand_prepare_tx_packet(SMART_WAND_READ_STATIC_LOG, current_page_static_log, &data[0], 0);
                        break;
                        
                      }
                      case(STATIC_LOG_WRITE_1):
                        {
                          
                          uint16_t start_idx;
                          const uint8_t GEN_ID = 1;
                          
                          /* append static log */
                          if (static_log.unit[0].id == 0)
                          {
                            
                            db.static_log.page_write = 3;
                            db.static_log.unit_num = 0;
                            start_idx = 0;
                            
                          }
                          else if (static_log.unit[1].id == 0)
                          {
                            
                            db.static_log.page_write = 5;
                            db.static_log.unit_num = 1;
                            /* previous end idx */
                            start_idx = static_log.unit[0].end_idx;
                            start_idx++;
                            static_log.unit[1].end_idx = start_idx;                      
                          }
                          else if (static_log.unit[2].id == 0)
                          {
                            
                            db.static_log.page_write = 7;
                            db.static_log.unit_num = 2;
                            /* previous end idx */
                            start_idx = static_log.unit[1].end_idx;
                            start_idx++;
                            static_log.unit[2].end_idx = start_idx;
                          }
                          else if (static_log.unit[3].id == 0)
                          {
                            
                            db.static_log.page_write = 9;
                            db.static_log.unit_num = 3;
                            /* previous end idx */
                            start_idx = static_log.unit[2].end_idx;
                            start_idx++;
                            static_log.unit[3].end_idx = start_idx;
                          }
                          else if (static_log.unit[4].id == 0)
                          {
                            
                            db.static_log.page_write = 11;
                            db.static_log.unit_num = 4;
                            /* previous end idx */
                            start_idx = static_log.unit[3].end_idx;
                            start_idx++;
                            static_log.unit[4].end_idx = start_idx;
                          }
                          else
                          {
                            /* overwrite last log */
                            db.static_log.page_write = 11;
                            db.static_log.unit_num = 4;
                            /* previous end idx */
                            
                            start_idx = static_log.unit[4].start_idx;
                            static_log.unit[4].end_idx = start_idx;
                            
                            /* erase fault/error */
                            static_log.unit[4].errors = 0;
                            
                          }
                          
                          /* roll idx if exceed max */
                          if (start_idx > 383)
                          {
                            
                            start_idx = 0;
                            
                          }
                          
                          /* log new start idx */
                          static_log.unit[db.static_log.unit_num].start_idx = start_idx;
                          /* log gen id */
                          static_log.unit[db.static_log.unit_num].id = GEN_ID;
                          /* log serial number */
                          static_log.unit[db.static_log.unit_num].sn = (*(uint32_t*)&serial_number[6]);
                          
                          /* clear echo */
                          memset(&db.static_log.write_echo,
                                 0,
                                 SMART_WAND_STATIC_WRITE_RX_PACKET_SIZE);
                          
                          /* force to next state */
                          tx_state = STATIC_LOG_WRITE_2;
                          
                          break;
                          
                        }
                        case(STATIC_LOG_WRITE_2):
                          {
                            
                            /* log rtc tsr as use time */
                            static_log.unit[db.static_log.unit_num].use_time = (*(uint32_t*)0x4003D000);
                            
                            /* clear echo */
                            memset(&db.static_log.write_echo,
                                   0,
                                   SMART_WAND_STATIC_WRITE_RX_PACKET_SIZE);
                            
                            
                            tx_state = CIRCLE_LOG_READ;
                            
                            break;
                            
                          }
                          case(CIRCLE_LOG_READ):
                            {
                              
                              memset(&db.circle_log.segment,0,SMART_WAND_CIRC_LOG_READ_RX_PACKET_SIZE);
                              
                              /* download last segment */
                              if (static_log.unit[db.static_log.unit_num].start_idx > 0)
                              {
                                
                                db.circle_log.page = static_log.unit[db.static_log.unit_num].start_idx / 2;
                                
                              }
                              else
                              {
                                
                                db.circle_log.page = 0;
                                
                              }
                              
                              db.circle_log.idx = (static_log.unit[db.static_log.unit_num].start_idx % 2);
                              
                              smartwand_prepare_tx_packet(SMART_WAND_READ_CIRC_LOG,
                                                          db.circle_log.page,
                                                          &data[0],
                                                          0);
                              
                              break;
                              
                            }
                            case(ERASE_STATIC_LOGS):
                              {
                                
                                uint8_t page;
                                
                                memset(&data[0],0,8);
                                
                                if (db.static_log.write_echo.idx > 1)
                                {
                                  
                                  page = db.static_log.write_echo.idx - 1;
                                  
                                }
                                else
                                {
                                  
                                  page = 15;
                                  
                                }
                                
                                memset(&db.static_log.write_echo,0,SMART_WAND_STATIC_WRITE_RX_PACKET_SIZE);
                                
                                /* write zeros to log */
                                smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG, page, &data[0], 8);
                                
                                break;
                                
                              }
                              case(ERASE_CIRCLE_LOGS):
                                {
                                  
                                  uint8_t page;
                                  
                                  memset(&data[0],0,8);
                                  
                                  if (db.circle_log.write_echo.idx > 0)
                                  {
                                    
                                    page = db.circle_log.write_echo.idx - 1;
                                    
                                  }
                                  else
                                  {
                                    
                                    page = 191;
                                    
                                  }
                                  
                                  smartwand_prepare_tx_packet(SMART_WAND_WRITE_CIRC_LOG,
                                                              page,
                                                              &data[0],
                                                              8);
                                  
                                  if (page > 0)
                                  {
                                    
                                    smartwand_prepare_tx_packet(SMART_WAND_WRITE_CIRC_LOG,
                                                                page-1,
                                                                &data[0],
                                                                8);
                                    
                                  }
                                  
                                  if (page > 1)
                                  {
                                    
                                    smartwand_prepare_tx_packet(SMART_WAND_WRITE_CIRC_LOG,
                                                                page-2,
                                                                &data[0],
                                                                8);
                                    
                                  }
                                  
                                  break;
                                  
                                }
    case (SWITCH_BOOT_TO_LOG):
      {
        
        uint8_t page = 0;
        
        memset(&data[0],0,8);
        
        data[0] = 'A';
        
        memset(&db.static_log.write_echo,0,SMART_WAND_STATIC_WRITE_RX_PACKET_SIZE);
        
        /* write zeros to log */
        smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG, page, &data[0], 8);
        
        break;
        
      }
    default:
      {
        
        break;
        
      }
      
    }
    
    return 0;
    
  }
  
  /***************************************************************************//**
  This function determines the thermometer display color based on the input
  temperature
  @param unit8_t wand temperature
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t thermometer_color(uint8_t temperature)
  {
    
    uint8_t color = 0;
    
    if (temperature >= current_therapy_settings.ambient.alarm_threshold)
    {
      
      /* red */ 
      color = 0;
      
    }
    else if (temperature >= (current_therapy_settings.ambient.alarm_threshold - 5))
    {
      
      /* orange */
      color = 1;
      
    }
    else 
    {
      
      /* green */
      color = 2;
      
    }
    
    return color;
    
  }
  /***************************************************************************//**
  Converts temperature in q62 bit format to 7bit number.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t convert_q62_ambient_to_7bit(uint8_t q62_temperature)
  {
    
    uint8_t temperature;
    
    temperature = (uint8_t) ((q62_temperature + Q62_TEMPERATURE_SCALE) >> 2) & 0xFF;
    
    /* limit temperature to 6-bits */
    if (temperature >= 63)
    {
      
      temperature = 63;
      
    }
    
    return temperature;
    
  }
  
  /***************************************************************************//**
  converts half float numbers to float
  @param void
  @return 0
  @outputs
  @context The half float format is used to store the date in smart wands
  *******************************************************************************/
  float32_t half2float(uint16_t value)
  {
    
    uint32_t bits;
    float32_t out;
    
    static const uint32_t mantissa_table[2048] = {
      0x00000000, 0x33800000, 0x34000000, 0x34400000, 0x34800000, 0x34A00000, 0x34C00000, 0x34E00000, 0x35000000, 0x35100000, 0x35200000, 0x35300000, 0x35400000, 0x35500000, 0x35600000, 0x35700000,
      0x35800000, 0x35880000, 0x35900000, 0x35980000, 0x35A00000, 0x35A80000, 0x35B00000, 0x35B80000, 0x35C00000, 0x35C80000, 0x35D00000, 0x35D80000, 0x35E00000, 0x35E80000, 0x35F00000, 0x35F80000,
      0x36000000, 0x36040000, 0x36080000, 0x360C0000, 0x36100000, 0x36140000, 0x36180000, 0x361C0000, 0x36200000, 0x36240000, 0x36280000, 0x362C0000, 0x36300000, 0x36340000, 0x36380000, 0x363C0000,
      0x36400000, 0x36440000, 0x36480000, 0x364C0000, 0x36500000, 0x36540000, 0x36580000, 0x365C0000, 0x36600000, 0x36640000, 0x36680000, 0x366C0000, 0x36700000, 0x36740000, 0x36780000, 0x367C0000,
      0x36800000, 0x36820000, 0x36840000, 0x36860000, 0x36880000, 0x368A0000, 0x368C0000, 0x368E0000, 0x36900000, 0x36920000, 0x36940000, 0x36960000, 0x36980000, 0x369A0000, 0x369C0000, 0x369E0000,
      0x36A00000, 0x36A20000, 0x36A40000, 0x36A60000, 0x36A80000, 0x36AA0000, 0x36AC0000, 0x36AE0000, 0x36B00000, 0x36B20000, 0x36B40000, 0x36B60000, 0x36B80000, 0x36BA0000, 0x36BC0000, 0x36BE0000,
      0x36C00000, 0x36C20000, 0x36C40000, 0x36C60000, 0x36C80000, 0x36CA0000, 0x36CC0000, 0x36CE0000, 0x36D00000, 0x36D20000, 0x36D40000, 0x36D60000, 0x36D80000, 0x36DA0000, 0x36DC0000, 0x36DE0000,
      0x36E00000, 0x36E20000, 0x36E40000, 0x36E60000, 0x36E80000, 0x36EA0000, 0x36EC0000, 0x36EE0000, 0x36F00000, 0x36F20000, 0x36F40000, 0x36F60000, 0x36F80000, 0x36FA0000, 0x36FC0000, 0x36FE0000,
      0x37000000, 0x37010000, 0x37020000, 0x37030000, 0x37040000, 0x37050000, 0x37060000, 0x37070000, 0x37080000, 0x37090000, 0x370A0000, 0x370B0000, 0x370C0000, 0x370D0000, 0x370E0000, 0x370F0000,
      0x37100000, 0x37110000, 0x37120000, 0x37130000, 0x37140000, 0x37150000, 0x37160000, 0x37170000, 0x37180000, 0x37190000, 0x371A0000, 0x371B0000, 0x371C0000, 0x371D0000, 0x371E0000, 0x371F0000,
      0x37200000, 0x37210000, 0x37220000, 0x37230000, 0x37240000, 0x37250000, 0x37260000, 0x37270000, 0x37280000, 0x37290000, 0x372A0000, 0x372B0000, 0x372C0000, 0x372D0000, 0x372E0000, 0x372F0000,
      0x37300000, 0x37310000, 0x37320000, 0x37330000, 0x37340000, 0x37350000, 0x37360000, 0x37370000, 0x37380000, 0x37390000, 0x373A0000, 0x373B0000, 0x373C0000, 0x373D0000, 0x373E0000, 0x373F0000,
      0x37400000, 0x37410000, 0x37420000, 0x37430000, 0x37440000, 0x37450000, 0x37460000, 0x37470000, 0x37480000, 0x37490000, 0x374A0000, 0x374B0000, 0x374C0000, 0x374D0000, 0x374E0000, 0x374F0000,
      0x37500000, 0x37510000, 0x37520000, 0x37530000, 0x37540000, 0x37550000, 0x37560000, 0x37570000, 0x37580000, 0x37590000, 0x375A0000, 0x375B0000, 0x375C0000, 0x375D0000, 0x375E0000, 0x375F0000,
      0x37600000, 0x37610000, 0x37620000, 0x37630000, 0x37640000, 0x37650000, 0x37660000, 0x37670000, 0x37680000, 0x37690000, 0x376A0000, 0x376B0000, 0x376C0000, 0x376D0000, 0x376E0000, 0x376F0000,
      0x37700000, 0x37710000, 0x37720000, 0x37730000, 0x37740000, 0x37750000, 0x37760000, 0x37770000, 0x37780000, 0x37790000, 0x377A0000, 0x377B0000, 0x377C0000, 0x377D0000, 0x377E0000, 0x377F0000,
      0x37800000, 0x37808000, 0x37810000, 0x37818000, 0x37820000, 0x37828000, 0x37830000, 0x37838000, 0x37840000, 0x37848000, 0x37850000, 0x37858000, 0x37860000, 0x37868000, 0x37870000, 0x37878000,
      0x37880000, 0x37888000, 0x37890000, 0x37898000, 0x378A0000, 0x378A8000, 0x378B0000, 0x378B8000, 0x378C0000, 0x378C8000, 0x378D0000, 0x378D8000, 0x378E0000, 0x378E8000, 0x378F0000, 0x378F8000,
      0x37900000, 0x37908000, 0x37910000, 0x37918000, 0x37920000, 0x37928000, 0x37930000, 0x37938000, 0x37940000, 0x37948000, 0x37950000, 0x37958000, 0x37960000, 0x37968000, 0x37970000, 0x37978000,
      0x37980000, 0x37988000, 0x37990000, 0x37998000, 0x379A0000, 0x379A8000, 0x379B0000, 0x379B8000, 0x379C0000, 0x379C8000, 0x379D0000, 0x379D8000, 0x379E0000, 0x379E8000, 0x379F0000, 0x379F8000,
      0x37A00000, 0x37A08000, 0x37A10000, 0x37A18000, 0x37A20000, 0x37A28000, 0x37A30000, 0x37A38000, 0x37A40000, 0x37A48000, 0x37A50000, 0x37A58000, 0x37A60000, 0x37A68000, 0x37A70000, 0x37A78000,
      0x37A80000, 0x37A88000, 0x37A90000, 0x37A98000, 0x37AA0000, 0x37AA8000, 0x37AB0000, 0x37AB8000, 0x37AC0000, 0x37AC8000, 0x37AD0000, 0x37AD8000, 0x37AE0000, 0x37AE8000, 0x37AF0000, 0x37AF8000,
      0x37B00000, 0x37B08000, 0x37B10000, 0x37B18000, 0x37B20000, 0x37B28000, 0x37B30000, 0x37B38000, 0x37B40000, 0x37B48000, 0x37B50000, 0x37B58000, 0x37B60000, 0x37B68000, 0x37B70000, 0x37B78000,
      0x37B80000, 0x37B88000, 0x37B90000, 0x37B98000, 0x37BA0000, 0x37BA8000, 0x37BB0000, 0x37BB8000, 0x37BC0000, 0x37BC8000, 0x37BD0000, 0x37BD8000, 0x37BE0000, 0x37BE8000, 0x37BF0000, 0x37BF8000,
      0x37C00000, 0x37C08000, 0x37C10000, 0x37C18000, 0x37C20000, 0x37C28000, 0x37C30000, 0x37C38000, 0x37C40000, 0x37C48000, 0x37C50000, 0x37C58000, 0x37C60000, 0x37C68000, 0x37C70000, 0x37C78000,
      0x37C80000, 0x37C88000, 0x37C90000, 0x37C98000, 0x37CA0000, 0x37CA8000, 0x37CB0000, 0x37CB8000, 0x37CC0000, 0x37CC8000, 0x37CD0000, 0x37CD8000, 0x37CE0000, 0x37CE8000, 0x37CF0000, 0x37CF8000,
      0x37D00000, 0x37D08000, 0x37D10000, 0x37D18000, 0x37D20000, 0x37D28000, 0x37D30000, 0x37D38000, 0x37D40000, 0x37D48000, 0x37D50000, 0x37D58000, 0x37D60000, 0x37D68000, 0x37D70000, 0x37D78000,
      0x37D80000, 0x37D88000, 0x37D90000, 0x37D98000, 0x37DA0000, 0x37DA8000, 0x37DB0000, 0x37DB8000, 0x37DC0000, 0x37DC8000, 0x37DD0000, 0x37DD8000, 0x37DE0000, 0x37DE8000, 0x37DF0000, 0x37DF8000,
      0x37E00000, 0x37E08000, 0x37E10000, 0x37E18000, 0x37E20000, 0x37E28000, 0x37E30000, 0x37E38000, 0x37E40000, 0x37E48000, 0x37E50000, 0x37E58000, 0x37E60000, 0x37E68000, 0x37E70000, 0x37E78000,
      0x37E80000, 0x37E88000, 0x37E90000, 0x37E98000, 0x37EA0000, 0x37EA8000, 0x37EB0000, 0x37EB8000, 0x37EC0000, 0x37EC8000, 0x37ED0000, 0x37ED8000, 0x37EE0000, 0x37EE8000, 0x37EF0000, 0x37EF8000,
      0x37F00000, 0x37F08000, 0x37F10000, 0x37F18000, 0x37F20000, 0x37F28000, 0x37F30000, 0x37F38000, 0x37F40000, 0x37F48000, 0x37F50000, 0x37F58000, 0x37F60000, 0x37F68000, 0x37F70000, 0x37F78000,
      0x37F80000, 0x37F88000, 0x37F90000, 0x37F98000, 0x37FA0000, 0x37FA8000, 0x37FB0000, 0x37FB8000, 0x37FC0000, 0x37FC8000, 0x37FD0000, 0x37FD8000, 0x37FE0000, 0x37FE8000, 0x37FF0000, 0x37FF8000,
      0x38000000, 0x38004000, 0x38008000, 0x3800C000, 0x38010000, 0x38014000, 0x38018000, 0x3801C000, 0x38020000, 0x38024000, 0x38028000, 0x3802C000, 0x38030000, 0x38034000, 0x38038000, 0x3803C000,
      0x38040000, 0x38044000, 0x38048000, 0x3804C000, 0x38050000, 0x38054000, 0x38058000, 0x3805C000, 0x38060000, 0x38064000, 0x38068000, 0x3806C000, 0x38070000, 0x38074000, 0x38078000, 0x3807C000,
      0x38080000, 0x38084000, 0x38088000, 0x3808C000, 0x38090000, 0x38094000, 0x38098000, 0x3809C000, 0x380A0000, 0x380A4000, 0x380A8000, 0x380AC000, 0x380B0000, 0x380B4000, 0x380B8000, 0x380BC000,
      0x380C0000, 0x380C4000, 0x380C8000, 0x380CC000, 0x380D0000, 0x380D4000, 0x380D8000, 0x380DC000, 0x380E0000, 0x380E4000, 0x380E8000, 0x380EC000, 0x380F0000, 0x380F4000, 0x380F8000, 0x380FC000,
      0x38100000, 0x38104000, 0x38108000, 0x3810C000, 0x38110000, 0x38114000, 0x38118000, 0x3811C000, 0x38120000, 0x38124000, 0x38128000, 0x3812C000, 0x38130000, 0x38134000, 0x38138000, 0x3813C000,
      0x38140000, 0x38144000, 0x38148000, 0x3814C000, 0x38150000, 0x38154000, 0x38158000, 0x3815C000, 0x38160000, 0x38164000, 0x38168000, 0x3816C000, 0x38170000, 0x38174000, 0x38178000, 0x3817C000,
      0x38180000, 0x38184000, 0x38188000, 0x3818C000, 0x38190000, 0x38194000, 0x38198000, 0x3819C000, 0x381A0000, 0x381A4000, 0x381A8000, 0x381AC000, 0x381B0000, 0x381B4000, 0x381B8000, 0x381BC000,
      0x381C0000, 0x381C4000, 0x381C8000, 0x381CC000, 0x381D0000, 0x381D4000, 0x381D8000, 0x381DC000, 0x381E0000, 0x381E4000, 0x381E8000, 0x381EC000, 0x381F0000, 0x381F4000, 0x381F8000, 0x381FC000,
      0x38200000, 0x38204000, 0x38208000, 0x3820C000, 0x38210000, 0x38214000, 0x38218000, 0x3821C000, 0x38220000, 0x38224000, 0x38228000, 0x3822C000, 0x38230000, 0x38234000, 0x38238000, 0x3823C000,
      0x38240000, 0x38244000, 0x38248000, 0x3824C000, 0x38250000, 0x38254000, 0x38258000, 0x3825C000, 0x38260000, 0x38264000, 0x38268000, 0x3826C000, 0x38270000, 0x38274000, 0x38278000, 0x3827C000,
      0x38280000, 0x38284000, 0x38288000, 0x3828C000, 0x38290000, 0x38294000, 0x38298000, 0x3829C000, 0x382A0000, 0x382A4000, 0x382A8000, 0x382AC000, 0x382B0000, 0x382B4000, 0x382B8000, 0x382BC000,
      0x382C0000, 0x382C4000, 0x382C8000, 0x382CC000, 0x382D0000, 0x382D4000, 0x382D8000, 0x382DC000, 0x382E0000, 0x382E4000, 0x382E8000, 0x382EC000, 0x382F0000, 0x382F4000, 0x382F8000, 0x382FC000,
      0x38300000, 0x38304000, 0x38308000, 0x3830C000, 0x38310000, 0x38314000, 0x38318000, 0x3831C000, 0x38320000, 0x38324000, 0x38328000, 0x3832C000, 0x38330000, 0x38334000, 0x38338000, 0x3833C000,
      0x38340000, 0x38344000, 0x38348000, 0x3834C000, 0x38350000, 0x38354000, 0x38358000, 0x3835C000, 0x38360000, 0x38364000, 0x38368000, 0x3836C000, 0x38370000, 0x38374000, 0x38378000, 0x3837C000,
      0x38380000, 0x38384000, 0x38388000, 0x3838C000, 0x38390000, 0x38394000, 0x38398000, 0x3839C000, 0x383A0000, 0x383A4000, 0x383A8000, 0x383AC000, 0x383B0000, 0x383B4000, 0x383B8000, 0x383BC000,
      0x383C0000, 0x383C4000, 0x383C8000, 0x383CC000, 0x383D0000, 0x383D4000, 0x383D8000, 0x383DC000, 0x383E0000, 0x383E4000, 0x383E8000, 0x383EC000, 0x383F0000, 0x383F4000, 0x383F8000, 0x383FC000,
      0x38400000, 0x38404000, 0x38408000, 0x3840C000, 0x38410000, 0x38414000, 0x38418000, 0x3841C000, 0x38420000, 0x38424000, 0x38428000, 0x3842C000, 0x38430000, 0x38434000, 0x38438000, 0x3843C000,
      0x38440000, 0x38444000, 0x38448000, 0x3844C000, 0x38450000, 0x38454000, 0x38458000, 0x3845C000, 0x38460000, 0x38464000, 0x38468000, 0x3846C000, 0x38470000, 0x38474000, 0x38478000, 0x3847C000,
      0x38480000, 0x38484000, 0x38488000, 0x3848C000, 0x38490000, 0x38494000, 0x38498000, 0x3849C000, 0x384A0000, 0x384A4000, 0x384A8000, 0x384AC000, 0x384B0000, 0x384B4000, 0x384B8000, 0x384BC000,
      0x384C0000, 0x384C4000, 0x384C8000, 0x384CC000, 0x384D0000, 0x384D4000, 0x384D8000, 0x384DC000, 0x384E0000, 0x384E4000, 0x384E8000, 0x384EC000, 0x384F0000, 0x384F4000, 0x384F8000, 0x384FC000,
      0x38500000, 0x38504000, 0x38508000, 0x3850C000, 0x38510000, 0x38514000, 0x38518000, 0x3851C000, 0x38520000, 0x38524000, 0x38528000, 0x3852C000, 0x38530000, 0x38534000, 0x38538000, 0x3853C000,
      0x38540000, 0x38544000, 0x38548000, 0x3854C000, 0x38550000, 0x38554000, 0x38558000, 0x3855C000, 0x38560000, 0x38564000, 0x38568000, 0x3856C000, 0x38570000, 0x38574000, 0x38578000, 0x3857C000,
      0x38580000, 0x38584000, 0x38588000, 0x3858C000, 0x38590000, 0x38594000, 0x38598000, 0x3859C000, 0x385A0000, 0x385A4000, 0x385A8000, 0x385AC000, 0x385B0000, 0x385B4000, 0x385B8000, 0x385BC000,
      0x385C0000, 0x385C4000, 0x385C8000, 0x385CC000, 0x385D0000, 0x385D4000, 0x385D8000, 0x385DC000, 0x385E0000, 0x385E4000, 0x385E8000, 0x385EC000, 0x385F0000, 0x385F4000, 0x385F8000, 0x385FC000,
      0x38600000, 0x38604000, 0x38608000, 0x3860C000, 0x38610000, 0x38614000, 0x38618000, 0x3861C000, 0x38620000, 0x38624000, 0x38628000, 0x3862C000, 0x38630000, 0x38634000, 0x38638000, 0x3863C000,
      0x38640000, 0x38644000, 0x38648000, 0x3864C000, 0x38650000, 0x38654000, 0x38658000, 0x3865C000, 0x38660000, 0x38664000, 0x38668000, 0x3866C000, 0x38670000, 0x38674000, 0x38678000, 0x3867C000,
      0x38680000, 0x38684000, 0x38688000, 0x3868C000, 0x38690000, 0x38694000, 0x38698000, 0x3869C000, 0x386A0000, 0x386A4000, 0x386A8000, 0x386AC000, 0x386B0000, 0x386B4000, 0x386B8000, 0x386BC000,
      0x386C0000, 0x386C4000, 0x386C8000, 0x386CC000, 0x386D0000, 0x386D4000, 0x386D8000, 0x386DC000, 0x386E0000, 0x386E4000, 0x386E8000, 0x386EC000, 0x386F0000, 0x386F4000, 0x386F8000, 0x386FC000,
      0x38700000, 0x38704000, 0x38708000, 0x3870C000, 0x38710000, 0x38714000, 0x38718000, 0x3871C000, 0x38720000, 0x38724000, 0x38728000, 0x3872C000, 0x38730000, 0x38734000, 0x38738000, 0x3873C000,
      0x38740000, 0x38744000, 0x38748000, 0x3874C000, 0x38750000, 0x38754000, 0x38758000, 0x3875C000, 0x38760000, 0x38764000, 0x38768000, 0x3876C000, 0x38770000, 0x38774000, 0x38778000, 0x3877C000,
      0x38780000, 0x38784000, 0x38788000, 0x3878C000, 0x38790000, 0x38794000, 0x38798000, 0x3879C000, 0x387A0000, 0x387A4000, 0x387A8000, 0x387AC000, 0x387B0000, 0x387B4000, 0x387B8000, 0x387BC000,
      0x387C0000, 0x387C4000, 0x387C8000, 0x387CC000, 0x387D0000, 0x387D4000, 0x387D8000, 0x387DC000, 0x387E0000, 0x387E4000, 0x387E8000, 0x387EC000, 0x387F0000, 0x387F4000, 0x387F8000, 0x387FC000,
      0x38000000, 0x38002000, 0x38004000, 0x38006000, 0x38008000, 0x3800A000, 0x3800C000, 0x3800E000, 0x38010000, 0x38012000, 0x38014000, 0x38016000, 0x38018000, 0x3801A000, 0x3801C000, 0x3801E000,
      0x38020000, 0x38022000, 0x38024000, 0x38026000, 0x38028000, 0x3802A000, 0x3802C000, 0x3802E000, 0x38030000, 0x38032000, 0x38034000, 0x38036000, 0x38038000, 0x3803A000, 0x3803C000, 0x3803E000,
      0x38040000, 0x38042000, 0x38044000, 0x38046000, 0x38048000, 0x3804A000, 0x3804C000, 0x3804E000, 0x38050000, 0x38052000, 0x38054000, 0x38056000, 0x38058000, 0x3805A000, 0x3805C000, 0x3805E000,
      0x38060000, 0x38062000, 0x38064000, 0x38066000, 0x38068000, 0x3806A000, 0x3806C000, 0x3806E000, 0x38070000, 0x38072000, 0x38074000, 0x38076000, 0x38078000, 0x3807A000, 0x3807C000, 0x3807E000,
      0x38080000, 0x38082000, 0x38084000, 0x38086000, 0x38088000, 0x3808A000, 0x3808C000, 0x3808E000, 0x38090000, 0x38092000, 0x38094000, 0x38096000, 0x38098000, 0x3809A000, 0x3809C000, 0x3809E000,
      0x380A0000, 0x380A2000, 0x380A4000, 0x380A6000, 0x380A8000, 0x380AA000, 0x380AC000, 0x380AE000, 0x380B0000, 0x380B2000, 0x380B4000, 0x380B6000, 0x380B8000, 0x380BA000, 0x380BC000, 0x380BE000,
      0x380C0000, 0x380C2000, 0x380C4000, 0x380C6000, 0x380C8000, 0x380CA000, 0x380CC000, 0x380CE000, 0x380D0000, 0x380D2000, 0x380D4000, 0x380D6000, 0x380D8000, 0x380DA000, 0x380DC000, 0x380DE000,
      0x380E0000, 0x380E2000, 0x380E4000, 0x380E6000, 0x380E8000, 0x380EA000, 0x380EC000, 0x380EE000, 0x380F0000, 0x380F2000, 0x380F4000, 0x380F6000, 0x380F8000, 0x380FA000, 0x380FC000, 0x380FE000,
      0x38100000, 0x38102000, 0x38104000, 0x38106000, 0x38108000, 0x3810A000, 0x3810C000, 0x3810E000, 0x38110000, 0x38112000, 0x38114000, 0x38116000, 0x38118000, 0x3811A000, 0x3811C000, 0x3811E000,
      0x38120000, 0x38122000, 0x38124000, 0x38126000, 0x38128000, 0x3812A000, 0x3812C000, 0x3812E000, 0x38130000, 0x38132000, 0x38134000, 0x38136000, 0x38138000, 0x3813A000, 0x3813C000, 0x3813E000,
      0x38140000, 0x38142000, 0x38144000, 0x38146000, 0x38148000, 0x3814A000, 0x3814C000, 0x3814E000, 0x38150000, 0x38152000, 0x38154000, 0x38156000, 0x38158000, 0x3815A000, 0x3815C000, 0x3815E000,
      0x38160000, 0x38162000, 0x38164000, 0x38166000, 0x38168000, 0x3816A000, 0x3816C000, 0x3816E000, 0x38170000, 0x38172000, 0x38174000, 0x38176000, 0x38178000, 0x3817A000, 0x3817C000, 0x3817E000,
      0x38180000, 0x38182000, 0x38184000, 0x38186000, 0x38188000, 0x3818A000, 0x3818C000, 0x3818E000, 0x38190000, 0x38192000, 0x38194000, 0x38196000, 0x38198000, 0x3819A000, 0x3819C000, 0x3819E000,
      0x381A0000, 0x381A2000, 0x381A4000, 0x381A6000, 0x381A8000, 0x381AA000, 0x381AC000, 0x381AE000, 0x381B0000, 0x381B2000, 0x381B4000, 0x381B6000, 0x381B8000, 0x381BA000, 0x381BC000, 0x381BE000,
      0x381C0000, 0x381C2000, 0x381C4000, 0x381C6000, 0x381C8000, 0x381CA000, 0x381CC000, 0x381CE000, 0x381D0000, 0x381D2000, 0x381D4000, 0x381D6000, 0x381D8000, 0x381DA000, 0x381DC000, 0x381DE000,
      0x381E0000, 0x381E2000, 0x381E4000, 0x381E6000, 0x381E8000, 0x381EA000, 0x381EC000, 0x381EE000, 0x381F0000, 0x381F2000, 0x381F4000, 0x381F6000, 0x381F8000, 0x381FA000, 0x381FC000, 0x381FE000,
      0x38200000, 0x38202000, 0x38204000, 0x38206000, 0x38208000, 0x3820A000, 0x3820C000, 0x3820E000, 0x38210000, 0x38212000, 0x38214000, 0x38216000, 0x38218000, 0x3821A000, 0x3821C000, 0x3821E000,
      0x38220000, 0x38222000, 0x38224000, 0x38226000, 0x38228000, 0x3822A000, 0x3822C000, 0x3822E000, 0x38230000, 0x38232000, 0x38234000, 0x38236000, 0x38238000, 0x3823A000, 0x3823C000, 0x3823E000,
      0x38240000, 0x38242000, 0x38244000, 0x38246000, 0x38248000, 0x3824A000, 0x3824C000, 0x3824E000, 0x38250000, 0x38252000, 0x38254000, 0x38256000, 0x38258000, 0x3825A000, 0x3825C000, 0x3825E000,
      0x38260000, 0x38262000, 0x38264000, 0x38266000, 0x38268000, 0x3826A000, 0x3826C000, 0x3826E000, 0x38270000, 0x38272000, 0x38274000, 0x38276000, 0x38278000, 0x3827A000, 0x3827C000, 0x3827E000,
      0x38280000, 0x38282000, 0x38284000, 0x38286000, 0x38288000, 0x3828A000, 0x3828C000, 0x3828E000, 0x38290000, 0x38292000, 0x38294000, 0x38296000, 0x38298000, 0x3829A000, 0x3829C000, 0x3829E000,
      0x382A0000, 0x382A2000, 0x382A4000, 0x382A6000, 0x382A8000, 0x382AA000, 0x382AC000, 0x382AE000, 0x382B0000, 0x382B2000, 0x382B4000, 0x382B6000, 0x382B8000, 0x382BA000, 0x382BC000, 0x382BE000,
      0x382C0000, 0x382C2000, 0x382C4000, 0x382C6000, 0x382C8000, 0x382CA000, 0x382CC000, 0x382CE000, 0x382D0000, 0x382D2000, 0x382D4000, 0x382D6000, 0x382D8000, 0x382DA000, 0x382DC000, 0x382DE000,
      0x382E0000, 0x382E2000, 0x382E4000, 0x382E6000, 0x382E8000, 0x382EA000, 0x382EC000, 0x382EE000, 0x382F0000, 0x382F2000, 0x382F4000, 0x382F6000, 0x382F8000, 0x382FA000, 0x382FC000, 0x382FE000,
      0x38300000, 0x38302000, 0x38304000, 0x38306000, 0x38308000, 0x3830A000, 0x3830C000, 0x3830E000, 0x38310000, 0x38312000, 0x38314000, 0x38316000, 0x38318000, 0x3831A000, 0x3831C000, 0x3831E000,
      0x38320000, 0x38322000, 0x38324000, 0x38326000, 0x38328000, 0x3832A000, 0x3832C000, 0x3832E000, 0x38330000, 0x38332000, 0x38334000, 0x38336000, 0x38338000, 0x3833A000, 0x3833C000, 0x3833E000,
      0x38340000, 0x38342000, 0x38344000, 0x38346000, 0x38348000, 0x3834A000, 0x3834C000, 0x3834E000, 0x38350000, 0x38352000, 0x38354000, 0x38356000, 0x38358000, 0x3835A000, 0x3835C000, 0x3835E000,
      0x38360000, 0x38362000, 0x38364000, 0x38366000, 0x38368000, 0x3836A000, 0x3836C000, 0x3836E000, 0x38370000, 0x38372000, 0x38374000, 0x38376000, 0x38378000, 0x3837A000, 0x3837C000, 0x3837E000,
      0x38380000, 0x38382000, 0x38384000, 0x38386000, 0x38388000, 0x3838A000, 0x3838C000, 0x3838E000, 0x38390000, 0x38392000, 0x38394000, 0x38396000, 0x38398000, 0x3839A000, 0x3839C000, 0x3839E000,
      0x383A0000, 0x383A2000, 0x383A4000, 0x383A6000, 0x383A8000, 0x383AA000, 0x383AC000, 0x383AE000, 0x383B0000, 0x383B2000, 0x383B4000, 0x383B6000, 0x383B8000, 0x383BA000, 0x383BC000, 0x383BE000,
      0x383C0000, 0x383C2000, 0x383C4000, 0x383C6000, 0x383C8000, 0x383CA000, 0x383CC000, 0x383CE000, 0x383D0000, 0x383D2000, 0x383D4000, 0x383D6000, 0x383D8000, 0x383DA000, 0x383DC000, 0x383DE000,
      0x383E0000, 0x383E2000, 0x383E4000, 0x383E6000, 0x383E8000, 0x383EA000, 0x383EC000, 0x383EE000, 0x383F0000, 0x383F2000, 0x383F4000, 0x383F6000, 0x383F8000, 0x383FA000, 0x383FC000, 0x383FE000,
      0x38400000, 0x38402000, 0x38404000, 0x38406000, 0x38408000, 0x3840A000, 0x3840C000, 0x3840E000, 0x38410000, 0x38412000, 0x38414000, 0x38416000, 0x38418000, 0x3841A000, 0x3841C000, 0x3841E000,
      0x38420000, 0x38422000, 0x38424000, 0x38426000, 0x38428000, 0x3842A000, 0x3842C000, 0x3842E000, 0x38430000, 0x38432000, 0x38434000, 0x38436000, 0x38438000, 0x3843A000, 0x3843C000, 0x3843E000,
      0x38440000, 0x38442000, 0x38444000, 0x38446000, 0x38448000, 0x3844A000, 0x3844C000, 0x3844E000, 0x38450000, 0x38452000, 0x38454000, 0x38456000, 0x38458000, 0x3845A000, 0x3845C000, 0x3845E000,
      0x38460000, 0x38462000, 0x38464000, 0x38466000, 0x38468000, 0x3846A000, 0x3846C000, 0x3846E000, 0x38470000, 0x38472000, 0x38474000, 0x38476000, 0x38478000, 0x3847A000, 0x3847C000, 0x3847E000,
      0x38480000, 0x38482000, 0x38484000, 0x38486000, 0x38488000, 0x3848A000, 0x3848C000, 0x3848E000, 0x38490000, 0x38492000, 0x38494000, 0x38496000, 0x38498000, 0x3849A000, 0x3849C000, 0x3849E000,
      0x384A0000, 0x384A2000, 0x384A4000, 0x384A6000, 0x384A8000, 0x384AA000, 0x384AC000, 0x384AE000, 0x384B0000, 0x384B2000, 0x384B4000, 0x384B6000, 0x384B8000, 0x384BA000, 0x384BC000, 0x384BE000,
      0x384C0000, 0x384C2000, 0x384C4000, 0x384C6000, 0x384C8000, 0x384CA000, 0x384CC000, 0x384CE000, 0x384D0000, 0x384D2000, 0x384D4000, 0x384D6000, 0x384D8000, 0x384DA000, 0x384DC000, 0x384DE000,
      0x384E0000, 0x384E2000, 0x384E4000, 0x384E6000, 0x384E8000, 0x384EA000, 0x384EC000, 0x384EE000, 0x384F0000, 0x384F2000, 0x384F4000, 0x384F6000, 0x384F8000, 0x384FA000, 0x384FC000, 0x384FE000,
      0x38500000, 0x38502000, 0x38504000, 0x38506000, 0x38508000, 0x3850A000, 0x3850C000, 0x3850E000, 0x38510000, 0x38512000, 0x38514000, 0x38516000, 0x38518000, 0x3851A000, 0x3851C000, 0x3851E000,
      0x38520000, 0x38522000, 0x38524000, 0x38526000, 0x38528000, 0x3852A000, 0x3852C000, 0x3852E000, 0x38530000, 0x38532000, 0x38534000, 0x38536000, 0x38538000, 0x3853A000, 0x3853C000, 0x3853E000,
      0x38540000, 0x38542000, 0x38544000, 0x38546000, 0x38548000, 0x3854A000, 0x3854C000, 0x3854E000, 0x38550000, 0x38552000, 0x38554000, 0x38556000, 0x38558000, 0x3855A000, 0x3855C000, 0x3855E000,
      0x38560000, 0x38562000, 0x38564000, 0x38566000, 0x38568000, 0x3856A000, 0x3856C000, 0x3856E000, 0x38570000, 0x38572000, 0x38574000, 0x38576000, 0x38578000, 0x3857A000, 0x3857C000, 0x3857E000,
      0x38580000, 0x38582000, 0x38584000, 0x38586000, 0x38588000, 0x3858A000, 0x3858C000, 0x3858E000, 0x38590000, 0x38592000, 0x38594000, 0x38596000, 0x38598000, 0x3859A000, 0x3859C000, 0x3859E000,
      0x385A0000, 0x385A2000, 0x385A4000, 0x385A6000, 0x385A8000, 0x385AA000, 0x385AC000, 0x385AE000, 0x385B0000, 0x385B2000, 0x385B4000, 0x385B6000, 0x385B8000, 0x385BA000, 0x385BC000, 0x385BE000,
      0x385C0000, 0x385C2000, 0x385C4000, 0x385C6000, 0x385C8000, 0x385CA000, 0x385CC000, 0x385CE000, 0x385D0000, 0x385D2000, 0x385D4000, 0x385D6000, 0x385D8000, 0x385DA000, 0x385DC000, 0x385DE000,
      0x385E0000, 0x385E2000, 0x385E4000, 0x385E6000, 0x385E8000, 0x385EA000, 0x385EC000, 0x385EE000, 0x385F0000, 0x385F2000, 0x385F4000, 0x385F6000, 0x385F8000, 0x385FA000, 0x385FC000, 0x385FE000,
      0x38600000, 0x38602000, 0x38604000, 0x38606000, 0x38608000, 0x3860A000, 0x3860C000, 0x3860E000, 0x38610000, 0x38612000, 0x38614000, 0x38616000, 0x38618000, 0x3861A000, 0x3861C000, 0x3861E000,
      0x38620000, 0x38622000, 0x38624000, 0x38626000, 0x38628000, 0x3862A000, 0x3862C000, 0x3862E000, 0x38630000, 0x38632000, 0x38634000, 0x38636000, 0x38638000, 0x3863A000, 0x3863C000, 0x3863E000,
      0x38640000, 0x38642000, 0x38644000, 0x38646000, 0x38648000, 0x3864A000, 0x3864C000, 0x3864E000, 0x38650000, 0x38652000, 0x38654000, 0x38656000, 0x38658000, 0x3865A000, 0x3865C000, 0x3865E000,
      0x38660000, 0x38662000, 0x38664000, 0x38666000, 0x38668000, 0x3866A000, 0x3866C000, 0x3866E000, 0x38670000, 0x38672000, 0x38674000, 0x38676000, 0x38678000, 0x3867A000, 0x3867C000, 0x3867E000,
      0x38680000, 0x38682000, 0x38684000, 0x38686000, 0x38688000, 0x3868A000, 0x3868C000, 0x3868E000, 0x38690000, 0x38692000, 0x38694000, 0x38696000, 0x38698000, 0x3869A000, 0x3869C000, 0x3869E000,
      0x386A0000, 0x386A2000, 0x386A4000, 0x386A6000, 0x386A8000, 0x386AA000, 0x386AC000, 0x386AE000, 0x386B0000, 0x386B2000, 0x386B4000, 0x386B6000, 0x386B8000, 0x386BA000, 0x386BC000, 0x386BE000,
      0x386C0000, 0x386C2000, 0x386C4000, 0x386C6000, 0x386C8000, 0x386CA000, 0x386CC000, 0x386CE000, 0x386D0000, 0x386D2000, 0x386D4000, 0x386D6000, 0x386D8000, 0x386DA000, 0x386DC000, 0x386DE000,
      0x386E0000, 0x386E2000, 0x386E4000, 0x386E6000, 0x386E8000, 0x386EA000, 0x386EC000, 0x386EE000, 0x386F0000, 0x386F2000, 0x386F4000, 0x386F6000, 0x386F8000, 0x386FA000, 0x386FC000, 0x386FE000,
      0x38700000, 0x38702000, 0x38704000, 0x38706000, 0x38708000, 0x3870A000, 0x3870C000, 0x3870E000, 0x38710000, 0x38712000, 0x38714000, 0x38716000, 0x38718000, 0x3871A000, 0x3871C000, 0x3871E000,
      0x38720000, 0x38722000, 0x38724000, 0x38726000, 0x38728000, 0x3872A000, 0x3872C000, 0x3872E000, 0x38730000, 0x38732000, 0x38734000, 0x38736000, 0x38738000, 0x3873A000, 0x3873C000, 0x3873E000,
      0x38740000, 0x38742000, 0x38744000, 0x38746000, 0x38748000, 0x3874A000, 0x3874C000, 0x3874E000, 0x38750000, 0x38752000, 0x38754000, 0x38756000, 0x38758000, 0x3875A000, 0x3875C000, 0x3875E000,
      0x38760000, 0x38762000, 0x38764000, 0x38766000, 0x38768000, 0x3876A000, 0x3876C000, 0x3876E000, 0x38770000, 0x38772000, 0x38774000, 0x38776000, 0x38778000, 0x3877A000, 0x3877C000, 0x3877E000,
      0x38780000, 0x38782000, 0x38784000, 0x38786000, 0x38788000, 0x3878A000, 0x3878C000, 0x3878E000, 0x38790000, 0x38792000, 0x38794000, 0x38796000, 0x38798000, 0x3879A000, 0x3879C000, 0x3879E000,
      0x387A0000, 0x387A2000, 0x387A4000, 0x387A6000, 0x387A8000, 0x387AA000, 0x387AC000, 0x387AE000, 0x387B0000, 0x387B2000, 0x387B4000, 0x387B6000, 0x387B8000, 0x387BA000, 0x387BC000, 0x387BE000,
      0x387C0000, 0x387C2000, 0x387C4000, 0x387C6000, 0x387C8000, 0x387CA000, 0x387CC000, 0x387CE000, 0x387D0000, 0x387D2000, 0x387D4000, 0x387D6000, 0x387D8000, 0x387DA000, 0x387DC000, 0x387DE000,
      0x387E0000, 0x387E2000, 0x387E4000, 0x387E6000, 0x387E8000, 0x387EA000, 0x387EC000, 0x387EE000, 0x387F0000, 0x387F2000, 0x387F4000, 0x387F6000, 0x387F8000, 0x387FA000, 0x387FC000, 0x387FE000 };
    static const uint32_t exponent_table[64] = {
      0x00000000, 0x00800000, 0x01000000, 0x01800000, 0x02000000, 0x02800000, 0x03000000, 0x03800000, 0x04000000, 0x04800000, 0x05000000, 0x05800000, 0x06000000, 0x06800000, 0x07000000, 0x07800000,
      0x08000000, 0x08800000, 0x09000000, 0x09800000, 0x0A000000, 0x0A800000, 0x0B000000, 0x0B800000, 0x0C000000, 0x0C800000, 0x0D000000, 0x0D800000, 0x0E000000, 0x0E800000, 0x0F000000, 0x47800000,
      0x80000000, 0x80800000, 0x81000000, 0x81800000, 0x82000000, 0x82800000, 0x83000000, 0x83800000, 0x84000000, 0x84800000, 0x85000000, 0x85800000, 0x86000000, 0x86800000, 0x87000000, 0x87800000,
      0x88000000, 0x88800000, 0x89000000, 0x89800000, 0x8A000000, 0x8A800000, 0x8B000000, 0x8B800000, 0x8C000000, 0x8C800000, 0x8D000000, 0x8D800000, 0x8E000000, 0x8E800000, 0x8F000000, 0xC7800000 };
    static const unsigned short offset_table[64] = {
      0, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
      0, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024 };
    
    bits = mantissa_table[offset_table[value>>10]+(value&0x3FF)] + exponent_table[value>>10];
    
    memcpy(&out, &bits, sizeof(float32_t));
    
    if (out != out)
    {
      
      out = 0.0f;
      
    }
    
    return out;
    
  }
  
  /***************************************************************************//**
  converts floats to half floats
  @param void
  @return 0
  @outputs
  @context The half float format is used to store the date in smart wands
  *******************************************************************************/
  uint16_t float2half(float32_t value)
  {
    
    uint16_t hbits;
    
    static const uint16_t base_table[512] = {
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100,
      0x0200, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00, 0x2000, 0x2400, 0x2800, 0x2C00, 0x3000, 0x3400, 0x3800, 0x3C00,
      0x4000, 0x4400, 0x4800, 0x4C00, 0x5000, 0x5400, 0x5800, 0x5C00, 0x6000, 0x6400, 0x6800, 0x6C00, 0x7000, 0x7400, 0x7800, 0x7C00,
      0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
      0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
      0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
      0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
      0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
      0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
      0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
      0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
      0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
      0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
      0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
      0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
      0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
      0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100,
      0x8200, 0x8400, 0x8800, 0x8C00, 0x9000, 0x9400, 0x9800, 0x9C00, 0xA000, 0xA400, 0xA800, 0xAC00, 0xB000, 0xB400, 0xB800, 0xBC00,
      0xC000, 0xC400, 0xC800, 0xCC00, 0xD000, 0xD400, 0xD800, 0xDC00, 0xE000, 0xE400, 0xE800, 0xEC00, 0xF000, 0xF400, 0xF800, 0xFC00,
      0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
      0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
      0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
      0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
      0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
      0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
      0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00 };
    static const unsigned char shift_table[512] = {
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 13,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 13 };
    
    uint32_t bits;// = *reinterpret_cast<uint32*>(&value);		//violating strict aliasing!
    
    memcpy(&bits, &value, sizeof(float32_t));
    
    hbits = base_table[bits>>23] + ((bits&0x7FFFFF)>>shift_table[bits>>23]);
    
    hbits += (((bits&0x7FFFFF)>>(shift_table[bits>>23]-1))|(((bits>>23)&0xFF)==102)) & ((hbits&0x7C00)!=0x7C00);
    
    return hbits;
  }
  
  bool validate_manta_pwm_values()
  {  
    /* uint16_t temp = manta_pwm_C1V - manta_pwm_C0V; */
    bool check_dead_time_rf = FALSE;
    bool check_valid_freq_rf = FALSE;
    bool check_duty_cycle_rf = FALSE;

    /* Condition for balancing o/p channels */

    /*  Rule 1 */
    if(((manta_pwm_C3V - manta_pwm_C2V) != (manta_pwm_C1V - manta_pwm_C0V)) && 
       ((manta_pwm_C5V - manta_pwm_C4V) != (manta_pwm_C1V - manta_pwm_C0V)) && 
         ((manta_pwm_C7V - manta_pwm_C6V) != (manta_pwm_C1V - manta_pwm_C0V)) && 
           ((manta_pwm_C3V - manta_pwm_C2V) != (manta_pwm_C5V - manta_pwm_C4V)) &&
             ((manta_pwm_C3V - manta_pwm_C2V) != (manta_pwm_C7V - manta_pwm_C6V)) &&
               ((manta_pwm_C5V - manta_pwm_C4V) != (manta_pwm_C7V - manta_pwm_C6V)))
    {
      check_dead_time_rf = TRUE;
    }
    
    /* Dead time check to prevent FET damage */
    /* Rule 2 */
    if(check_dead_time_rf)
    {
      if((manta_pwm_C3V > manta_pwm_C0V) && (manta_pwm_C2V < manta_pwm_C1V))
      {
        check_valid_freq_rf = TRUE;
      }
      else
        check_valid_freq_rf = FALSE;
    }
    
    
    /* Boundary check for valid frequency values 10 - 370kHz */
    /* Rule 3 */
    if(check_valid_freq_rf)
    {
      if((manta_pwm_mod < 740) && (manta_pwm_mod > 199))
      {
        check_duty_cycle_rf = TRUE;
      }
    }
    
    
    /* Boundary check for duty cycle <= 100% */
    /* Rule 4 */
    if(check_duty_cycle_rf)
    {
      if((manta_pwm_mod >= manta_pwm_C0V) && (manta_pwm_mod >= manta_pwm_C1V) && (manta_pwm_mod >= manta_pwm_C2V) && (manta_pwm_mod >= manta_pwm_C3V) &&
         (manta_pwm_mod >= manta_pwm_C4V) && (manta_pwm_mod >= manta_pwm_C5V) && (manta_pwm_mod >= manta_pwm_C6V) && (manta_pwm_mod >= manta_pwm_C7V))
      {
        manta_pwm_values_valid = TRUE;
        return manta_pwm_values_valid;
      }
    }
    return manta_pwm_values_valid;
    
  }
  
  
  /***************************************************************************//**
  Extracts wand parameters from the wand
  @param void
  @return 0
  @outputs
  @context 
  *******************************************************************************/
  void extract_wand_params(void)
  {
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;
    uint8_t l = 0;
    uint8_t mode, level;
    const uint8_t mode_level_lookup[10][2] = {
      {NONE,ABLATE_ZILCH},
      {LO,ABLATE_MINUS},
      {LO,ABLATE_DEFAULT},
      {LO,ABLATE_PLUS},
      {MED,ABLATE_MINUS},
      {MED,ABLATE_DEFAULT},
      {MED,ABLATE_PLUS},
      {HI,ABLATE_MINUS},
      {HI,ABLATE_DEFAULT},
      {HI,ABLATE_PLUS}
    };
    
    uint8_t wo_ram_match = 0,
    id_ram_match = 0,
    wo_flash_match = 0,
    id_flash_match = 0,
    match_temp = 0;
    manta_pwm_values_valid = FALSE;
    
    nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
    uint8_t profile_idx = current_therapy_settings.user_data.settings.idx_b4_pwr_cycle;
    current_page_static_log = 0;
    wo_ram_match = memcmp(&smart_wand_params.mfg.work_order[0],
                          &db.mfg.data[3],
                          sizeof(smart_wand_params.mfg.work_order));
    
    id_ram_match = memcmp(&smart_wand_params.mfg.unique_id[0],
                          &db.mfg.data[7],
                          sizeof(smart_wand_params.mfg.unique_id));
    
    match_temp = (smart_wand_params.mfg.work_order[0] |
                  smart_wand_params.mfg.work_order[1] |
                    smart_wand_params.mfg.work_order[2] |
                      smart_wand_params.mfg.work_order[3]);
    
    /* don't trust wo of zero, 1 = no match */ 
    wo_ram_match |= ((match_temp > 0) ? 0 : 1);
    
    match_temp = 0;
    
    match_temp = (smart_wand_params.mfg.unique_id[0] |
                  smart_wand_params.mfg.unique_id[1]);
    
    /* don't trust id of zeros, 1 = no match */
    id_ram_match |= match_temp > 0 ? 0 : 1; 
    
    /* extract name */
    memcpy(&smart_wand_params.mfg.final_assy[0],&db.mfg.data[0],sizeof(smart_wand_params.mfg.final_assy));
    memcpy(&smart_wand_params.mfg.unique_id[0],&db.mfg.data[7],sizeof(smart_wand_params.mfg.unique_id));
    memcpy(&smart_wand_params.mfg.name[0],&db.mfg.data[19],sizeof(smart_wand_params.mfg.name));
    memcpy(&smart_wand_params.mfg.catalog[0],&db.mfg.data[31],sizeof(smart_wand_params.mfg.catalog));
    memcpy(&smart_wand_params.mfg.work_order[0],&db.mfg.data[3],sizeof(smart_wand_params.mfg.work_order));
    smart_wand_params.mfg.born_date = (*(uint32_t*)&db.mfg.data[9]);
    smart_wand_params.mfg.expiration_date = (*(uint32_t*)&db.mfg.data[13]) + smart_wand_params.mfg.born_date;
    smart_wand_params.mfg.use_limit_time_en = ((db.mfg.data[17] >> 7) & 0x1);
    smart_wand_params.mfg.use_limit_activation_en = ((db.mfg.data[17] >> 6) & 0x1);
    smart_wand_params.mfg.use_limit_time_allowance = ((db.mfg.data[17] & 0x3F) * 3600);
    smart_wand_params.mfg.max_activation_seconds = (db.mfg.data[18]*15);
    smart_wand_params.mfg.default_coag_mode = ((db.mfg.data[43] >> 4) & 0xF)+1;
    smart_wand_params.mfg.default_ablate_mode = (db.mfg.data[43] & 0xF);
    if(current_therapy_settings.gui.screenID == RF_2000_WAND_TREATMENT_SCREEN && 
       current_therapy_settings.wand.valid == TRUE)
    {
      current_therapy_settings.ambient.temperature = convert_q62_ambient_to_7bit(db.param[4].temp2);
      current_therapy_settings.ambient.thermometer_color = thermometer_color(current_therapy_settings.ambient.temperature);
    }
    smart_wand_params.mfg.indication = db.mfg.data[46];
    smart_wand_params.mfg.version = (db.mfg.data[61] & 0xFF);
    smart_wand_params.mfg.format = (db.mfg.data[62] & 0xFF);
    smart_wand_params.mfg.flow_enable = (db.mfg.data[50] & 0x7C);
    smart_wand_params.mfg.default_flow_setting = (ENT_FLOW_SP_T)(db.mfg.data[51] & 0xF);
    
    if (smart_wand_params.mfg.format < 4)
    {
      smart_wand_params.mfg.flow_enable = 0;
      smart_wand_params.mfg.default_flow_setting = ENT_FLOW_0;
      smart_wand_params.mfg.tube_temperature_enable = 1;
    }
    else
    {
      smart_wand_params.mfg.tube_temperature_enable = (db.mfg.data[45] & 0x1);
    }
    
    if (smart_wand_params.mfg.format > 2)
    {
      
      smart_wand_params.mfg.wand_wear_enable = (db.mfg.data[44] & 0x40) >> 6;
      smart_wand_params.mfg.suction_enable = (db.mfg.data[45] & 0x80) >> 7;
      smart_wand_params.mfg.finger_switch_enable = (db.mfg.data[45] & 0x40) >> 6;
      smart_wand_params.mfg.ambient_enable = (db.mfg.data[45] & 0x20) >> 5;
      smart_wand_params.mfg.low_enable = (db.mfg.data[45] & 0x10) >> 4;
      smart_wand_params.mfg.med_enable = (db.mfg.data[45] & 0x8) >> 3;
      smart_wand_params.mfg.hi_enable = (db.mfg.data[45] & 0x4) >> 2;
      smart_wand_params.mfg.vac_enable = (db.mfg.data[45] & 0x2) >> 1;
      smart_wand_params.mfg.coag_enable = 1;//(db.mfg.data[45] & 0x1);
      smart_wand_params.worn.detect_duration = db.param[3].data[14];
      if (smart_wand_params.worn.detect_duration > MAX_WORN_WAND_DETECTION_WINDOW)
      {
        
        smart_wand_params.worn.detect_duration = MAX_WORN_WAND_DETECTION_WINDOW;
        
      }
      else if (smart_wand_params.worn.detect_duration < MIN_WORN_WAND_DETECTION_WINDOW)
      {
        
        smart_wand_params.worn.detect_duration = MIN_WORN_WAND_DETECTION_WINDOW;
        
      }
      smart_wand_params.worn.lo_irms_threshold = (*(uint32_t*)&db.param[3].data[16]); 
      smart_wand_params.worn.med_irms_threshold = (*(uint32_t*)&db.param[3].data[20]);
      smart_wand_params.worn.lo_temp_threshold = (*(uint32_t*)&db.param[3].data[24]);
      smart_wand_params.worn.med_temp_threshold = (*(uint32_t*)&db.param[3].data[28]);
      smart_wand_params.worn.hi_temp_threshold = (*(uint32_t*)&db.param[3].data[34]);
      smart_wand_params.worn.hi_irms_threshold = (*(uint32_t*)&db.param[3].data[38]);
      smart_wand_params.impedance.temp_comp_gain = db.param[3].data[32];
//      prime.started = FALSE;
//      prime.completed = FALSE;
      //prime.start_count = 0;
      AutoPrimeId = 0U;
      PrimeId = 0U;
      AutoPrimeTimeoutId = 0U;
      
    }
    else
    {
      
      smart_wand_params.mfg.wand_wear_enable = 0;
      smart_wand_params.mfg.suction_enable = 1;
      smart_wand_params.mfg.finger_switch_enable = 1;
      smart_wand_params.mfg.ambient_enable = 1;
      smart_wand_params.mfg.low_enable = 1;
      smart_wand_params.mfg.med_enable = 1;
      smart_wand_params.mfg.hi_enable = 1;
      smart_wand_params.mfg.vac_enable = 1;
      smart_wand_params.mfg.coag_enable = 1;
      smart_wand_params.worn.detect_duration = 7;
      smart_wand_params.worn.lo_irms_threshold = 9929; 
      smart_wand_params.worn.med_irms_threshold = 29789;
      smart_wand_params.worn.hi_irms_threshold = 29789;
      smart_wand_params.worn.lo_temp_threshold = 8000;
      smart_wand_params.worn.med_temp_threshold = 6800;
      smart_wand_params.worn.hi_temp_threshold = 6800;
      smart_wand_params.impedance.temp_comp_gain = 0;
      
    }
    
/*   if((current_therapy_settings.controller == ENT) && 
       (legacy_wand_params_obj.wand_connected) && 
         (current_therapy_settings.active_wand.legacy_wand == ENT)) */
    if(flow_val_apl_ent_bkp)
    {
      current_therapy_settings.treatment.flow_rate = flow_val_apl_ent_bkp;
      current_therapy_settings.pump.ent_flow = change_flow_manta(current_therapy_settings.setpoint.coag, 
                                                                     current_therapy_settings.treatment.flow_rate);
      flow_val_apl_ent_bkp = 0;
    }
    
    /* match = 0 means there is a match so load defaults if no match */
    if ((wo_ram_match != 0) || (id_ram_match != 0))
    {
      
      uint8_t selected_profile = 
        current_therapy_settings.user_data.settings.active_idx;
      
      current_therapy_settings.wand.worn_latch = 0;
      current_therapy_settings.wand.worn_active_time = 0;
      current_therapy_settings.wand.worn_timer_en = 0;
      current_therapy_settings.wand.irms_wear = (bool)wand_irms_wear_monitor(1,0);
      current_therapy_settings.wand.thandle_wear = (bool)wand_thandle_wear_monitor(0,1,0);
      
      /* by default always enable ambient when wand is connected */
      current_therapy_settings.ambient.enable = (current_therapy_settings.ambient.capable &
                                                 current_therapy_settings.ambient.enable_user_pref);
      
      if (smart_wand_params.mfg.default_coag_mode > COAG_3)
      {
        
        if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA)
        {
          //current_therapy_settings.setpoint.coag = smart_wand_params.mfg.default_coag_mode - 1;
        }
        else
        {
          current_therapy_settings.setpoint.coag = COAG_1;
        }
        
      }
      else 
      {
        
        if(smart_wand_params.mfg.indication != SMART_WAND_MFG_INDICATION_MANTA)
        {
          current_therapy_settings.user_data.profile[0].setpoint.coag = 
            (COAG_LEVELS) smart_wand_params.mfg.default_coag_mode;
          current_therapy_settings.setpoint.coag = 
            current_therapy_settings.user_data.profile[selected_profile].setpoint.coag;
        }
        
      }
      
      if (smart_wand_params.mfg.default_ablate_mode > 9)
      {
        
        current_therapy_settings.treatment.therapy_mode = LO;
        current_therapy_settings.treatment.previous_therapy_mode = LO;
        current_therapy_settings.setpoint.lo = ABLATE_DEFAULT;
        current_therapy_settings.setpoint.med = ABLATE_DEFAULT;
        current_therapy_settings.setpoint.hi = ABLATE_DEFAULT;
        current_therapy_settings.user_data.profile[0].setpoint.lo = ABLATE_DEFAULT;
        current_therapy_settings.user_data.profile[0].setpoint.med = ABLATE_DEFAULT;
        current_therapy_settings.user_data.profile[0].setpoint.hi = ABLATE_DEFAULT;
        
      }
      else
      {
        
        mode = (MODES) mode_level_lookup[smart_wand_params.mfg.default_ablate_mode][0];
        level = (CUT_LEVELS) mode_level_lookup[smart_wand_params.mfg.default_ablate_mode][1];
        
        current_therapy_settings.treatment.therapy_mode = (MODES) mode;
        current_therapy_settings.treatment.previous_therapy_mode = (MODES) mode;
        
        switch(mode)
        {
          
          case(LO):
            {
              
              //current_therapy_settings.setpoint.lo = (CUT_LEVELS) level;
              //current_therapy_settings.setpoint.med = ABLATE_DEFAULT;
              //current_therapy_settings.setpoint.hi = ABLATE_DEFAULT;
              current_therapy_settings.user_data.profile[0].setpoint.lo = (CUT_LEVELS) level;
              current_therapy_settings.user_data.profile[0].setpoint.med = ABLATE_DEFAULT;
              current_therapy_settings.user_data.profile[0].setpoint.hi = ABLATE_DEFAULT;
              break;
              
            }
            case(MED):
              {
                
                //current_therapy_settings.setpoint.med = (CUT_LEVELS) level;
                //current_therapy_settings.setpoint.lo = ABLATE_DEFAULT;
                //current_therapy_settings.setpoint.hi = ABLATE_DEFAULT;
                current_therapy_settings.user_data.profile[0].setpoint.lo = ABLATE_DEFAULT;
                current_therapy_settings.user_data.profile[0].setpoint.med = (CUT_LEVELS) level;
                current_therapy_settings.user_data.profile[0].setpoint.hi = ABLATE_DEFAULT;
                
                break;
                
              }
              case(HI):
                {
                  
                  //current_therapy_settings.setpoint.hi = (CUT_LEVELS) level;
                  //current_therapy_settings.setpoint.med = ABLATE_DEFAULT;
                  //current_therapy_settings.setpoint.lo = ABLATE_DEFAULT;
                  current_therapy_settings.user_data.profile[0].setpoint.lo = ABLATE_DEFAULT;
                  current_therapy_settings.user_data.profile[0].setpoint.med = ABLATE_DEFAULT;
                  current_therapy_settings.user_data.profile[0].setpoint.hi = (CUT_LEVELS) level;
                  break;
                  
                }
        default:
          {
            
            current_therapy_settings.setpoint.lo = ABLATE_DEFAULT;
            current_therapy_settings.setpoint.med = ABLATE_DEFAULT;
            current_therapy_settings.setpoint.hi = ABLATE_DEFAULT;
            current_therapy_settings.user_data.profile[0].setpoint.lo = ABLATE_DEFAULT;
            current_therapy_settings.user_data.profile[0].setpoint.med = ABLATE_DEFAULT;
            current_therapy_settings.user_data.profile[0].setpoint.hi = ABLATE_DEFAULT;
            
          }
          
        }
        
        current_therapy_settings.setpoint.lo = 
          current_therapy_settings.user_data.profile[selected_profile].setpoint.lo;
        current_therapy_settings.setpoint.med = 
          current_therapy_settings.user_data.profile[selected_profile].setpoint.med;
        current_therapy_settings.setpoint.hi = 
          current_therapy_settings.user_data.profile[selected_profile].setpoint.hi;
        
      }
      
      wo_flash_match = memcmp(&smart_wand_params.mfg.work_order[0],
                              &p_data->smart_wand.work_order[0],
                              sizeof(smart_wand_params.mfg.work_order));
      
      id_flash_match = memcmp(&smart_wand_params.mfg.unique_id[0],
                              &p_data->smart_wand.unique_id[0],
                              sizeof(smart_wand_params.mfg.unique_id));
      
      /* For same non-APL wand, load therapy values from profile */
      if ((wo_flash_match == 0) && (id_flash_match == 0) && 
          (smart_wand_params.mfg.indication != SMART_WAND_MFG_INDICATION_MANTA))
      {
        if (load_profile(profile_idx) == 0);
        {
          current_therapy_settings.user_data.settings.active_idx = profile_idx;
        }
      }
      
      /* For same APL wand connected after reset will retreive value from NV RAM */
      if ((wo_flash_match == 0) && (id_flash_match == 0) && 
          (current_therapy_settings.previous_wand_indication == SMART_WAND_MFG_INDICATION_MANTA))
      {
        current_therapy_settings.log.level = COAG_0;
      }
      /* Else load Default values since different APL wand connected*/
      else if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA)
      {
        current_therapy_settings.setpoint.coag = smart_wand_params.mfg.default_coag_mode - 1;
        current_coag_setpoint = current_therapy_settings.setpoint.coag;
        
        current_therapy_settings.treatment.flow_rate = smart_wand_params.mfg.default_flow_setting;
        current_therapy_settings.pump.ent_flow = change_flow_manta(current_therapy_settings.setpoint.coag, 
                                                                   current_therapy_settings.treatment.flow_rate);
        current_therapy_settings.log.level = COAG_0;
      }
          
      
      for (ENT_FLOW_SP_T i = ENT_FLOW_0; i < ENT_FLOW_5; i++)
      {
        uint8_t flow_bit = 1<<(MAX_ENT_FLOW_SP-i);
        if ((smart_wand_params.mfg.flow_enable & flow_bit) == flow_bit)
        {
          current_therapy_settings.treatment.ent_flow_sp_enable[i+1] = 1;
        }
        else
        {
          current_therapy_settings.treatment.ent_flow_sp_enable[i+1] = 0;
        }
      }
      
      if (smart_wand_params.mfg.default_flow_setting > ENT_FLOW_5)
      {
        smart_wand_params.mfg.default_flow_setting = ENT_FLOW_0;
      }
      
      for (int m = 0; m < NUM_OF_MODES; m++)
      {
        for (int l = 0; l < NUM_CUT_LEVELS; l++)
        {
          current_therapy_settings.treatment.ent_flow_sp[m][l] = 
            smart_wand_params.mfg.default_flow_setting;
        }
      }
      
    }
    
    if (smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_SPORTS_MED)
    {
      
      smart_wand_params.impedance.z_high  = (*(uint16_t*)&db.param[0].data[0]);
      smart_wand_params.impedance.z_low  = (*(uint16_t*)&db.param[0].data[2]);
      smart_wand_params.impedance.z_level  = (*(uint16_t*)&db.param[0].data[4]);
      smart_wand_params.impedance.z_time  = (*(uint16_t*)&db.param[0].data[6]);
      smart_wand_params.filter.coeffs[0]  =  (*(q15_t*)& db.param[0].data[8]);
      smart_wand_params.filter.coeffs[1]  =  (*(q15_t*)& db.param[0].data[10]);
      smart_wand_params.filter.coeffs[2]  =  (*(q15_t*)& db.param[0].data[12]);
      smart_wand_params.filter.coeffs[3]  =  (*(q15_t*)& db.param[0].data[14]);
      smart_wand_params.filter.coeffs[4]  =  (*(q15_t*)& db.param[0].data[16]);
      smart_wand_params.filter.coeffs[5]  =  (*(q15_t*)& db.param[0].data[18]);
      smart_wand_params.filter.coeffs[6]  =  (*(q15_t*)& db.param[0].data[20]);
      smart_wand_params.filter.coeffs[7]  =  (*(q15_t*)& db.param[0].data[22]);
      smart_wand_params.coag.coag1_voltage  = ( (*(uint16_t*)&db.param[0].data[34]) & 0xFFF);
      smart_wand_params.coag.coag1_rpm  =  half2float((*(uint16_t*)&db.param[0].data[36]));
      smart_wand_params.coag.coag2_voltage  = ( (*(uint16_t*)&db.param[0].data[38]) & 0xFFF);
      smart_wand_params.coag.coag2_rpm  =  half2float((*(uint16_t*)&db.param[0].data[40]));
      smart_wand_params.lo.min_rps_default  =  half2float((*(uint16_t*)&db.param[0].data[42]));
      smart_wand_params.lo.max_rps_default  =  half2float((*(uint16_t*)&db.param[0].data[44]));
      smart_wand_params.lo.kp1  =  half2float((*(uint16_t*)&db.param[0].data[46]));
      smart_wand_params.lo.kp2  =  half2float((*(uint16_t*)&db.param[0].data[48]));
      smart_wand_params.lo.kp3  =  half2float((*(uint16_t*)&db.param[0].data[50]));
      smart_wand_params.lo.ki1  =  half2float((*(uint16_t*)&db.param[0].data[52]));
      smart_wand_params.lo.ki2  =  half2float((*(uint16_t*)&db.param[0].data[54]));
      smart_wand_params.lo.ki3  =  half2float((*(uint16_t*)&db.param[0].data[56]));
      smart_wand_params.lo.e1  =  half2float((*(uint16_t*)&db.param[0].data[58]));
      smart_wand_params.lo.e2  =  half2float((*(uint16_t*)&db.param[0].data[60]));
      smart_wand_params.lo.voltage_sp_default  = ( (*(uint16_t*)&db.param[0].data[62]) & 0xFFF);
      smart_wand_params.lo.current_sp_default  =  half2float((*(uint16_t*)&db.param[1].data[0]));
      smart_wand_params.lo.idle_rps  =  half2float((*(uint16_t*)&db.param[1].data[2]));
      smart_wand_params.lo.ss_time  = (*(uint16_t*)&db.param[1].data[4]);
      smart_wand_params.lo.pulse_window  = (*(uint16_t*)&db.param[1].data[6]);
      smart_wand_params.lo.pulse_energy  = (*(uint16_t*)&db.param[1].data[8]);
      smart_wand_params.lo.quiescent_delay  = (*(uint16_t*)&db.param[1].data[10]);
      smart_wand_params.lo.current_sp_plus  =  half2float((*(uint16_t*)&db.param[1].data[12]));
      smart_wand_params.lo.voltage_sp_plus  = ( (*(uint16_t*)&db.param[1].data[14]) & 0xFFF);
      smart_wand_params.lo.current_sp_minus  =  half2float((*(uint16_t*)&db.param[1].data[16]));
      smart_wand_params.lo.voltage_sp_minus  = ( (*(uint16_t*)&db.param[1].data[18]) & 0xFFF);
      smart_wand_params.med.min_rps_default  =  half2float((*(uint16_t*)&db.param[1].data[20]));
      smart_wand_params.med.max_rps_default  =  half2float((*(uint16_t*)&db.param[1].data[22]));
      smart_wand_params.med.kp1  =  half2float((*(uint16_t*)&db.param[1].data[24]));
      smart_wand_params.med.kp2  =  half2float((*(uint16_t*)&db.param[1].data[26]));
      smart_wand_params.med.kp3  =  half2float((*(uint16_t*)&db.param[1].data[28]));
      smart_wand_params.med.ki1  =  half2float((*(uint16_t*)&db.param[1].data[30]));
      smart_wand_params.med.ki2  =  half2float((*(uint16_t*)&db.param[1].data[32]));
      smart_wand_params.med.ki3  =  half2float((*(uint16_t*)&db.param[1].data[34]));
      smart_wand_params.med.e1  =  half2float((*(uint16_t*)&db.param[1].data[36]));
      smart_wand_params.med.e2  =  half2float((*(uint16_t*)&db.param[1].data[38]));
      smart_wand_params.med.voltage_sp_default  = ( (*(uint16_t*)&db.param[1].data[40]) & 0xFFF);
      smart_wand_params.med.current_sp_default  =  half2float((*(uint16_t*)&db.param[1].data[42]));
      smart_wand_params.med.idle_rps  =  half2float((*(uint16_t*)&db.param[1].data[44]));
      smart_wand_params.med.ss_time  = (*(uint16_t*)&db.param[1].data[46]);
      smart_wand_params.med.pulse_window  = (*(uint16_t*)&db.param[1].data[48]);
      smart_wand_params.med.pulse_energy  = (*(uint16_t*)&db.param[1].data[50]);
      smart_wand_params.med.voltage_sp_plus  = ( (*(uint16_t*)&db.param[1].data[52]) & 0xFFF);
      smart_wand_params.med.current_sp_plus  =  half2float((*(uint16_t*)&db.param[1].data[54]));
      smart_wand_params.med.voltage_sp_minus  = ( (*(uint16_t*)&db.param[1].data[56]) & 0xFFF);
      smart_wand_params.med.current_sp_minus  =  half2float((*(uint16_t*)&db.param[1].data[58]));
      smart_wand_params.hi.min_rps_default  =  half2float((*(uint16_t*)&db.param[1].data[60]));
      smart_wand_params.hi.max_rps_default  =  half2float((*(uint16_t*)&db.param[1].data[62]));
      smart_wand_params.hi.kp1  =  half2float((*(uint16_t*)&db.param[2].data[0]));
      smart_wand_params.hi.kp2  =  half2float((*(uint16_t*)&db.param[2].data[2]));
      smart_wand_params.hi.kp3  =  half2float((*(uint16_t*)&db.param[2].data[4]));
      smart_wand_params.hi.ki1  =  half2float((*(uint16_t*)&db.param[2].data[6]));
      smart_wand_params.hi.ki2  =  half2float((*(uint16_t*)&db.param[2].data[8]));
      smart_wand_params.hi.ki3  =  half2float((*(uint16_t*)&db.param[2].data[10]));
      smart_wand_params.hi.e1  =  half2float((*(uint16_t*)&db.param[2].data[12]));
      smart_wand_params.hi.e2  =  half2float((*(uint16_t*)&db.param[2].data[14]));
      smart_wand_params.hi.voltage_sp_default  = ( (*(uint16_t*)&db.param[2].data[16]) & 0xFFF);
      smart_wand_params.hi.current_sp_default  =  half2float((*(uint16_t*)&db.param[2].data[18]));
      smart_wand_params.hi.idle_rps  =  half2float((*(uint16_t*)&db.param[2].data[20]));
      smart_wand_params.hi.pulse_window  = (*(uint16_t*)&db.param[2].data[22]);
      smart_wand_params.hi.pulse_energy  = (*(uint16_t*)&db.param[2].data[24]);
      smart_wand_params.hi.ss_time  = (*(uint16_t*)&db.param[2].data[26]);
      smart_wand_params.hi.voltage_sp_plus  = ( (*(uint16_t*)&db.param[2].data[28]) & 0xFFF);
      smart_wand_params.hi.max_rps_plus  =  half2float((*(uint16_t*)&db.param[2].data[30]));
      smart_wand_params.hi.voltage_sp_minus  = ( (*(uint16_t*)&db.param[2].data[32]) & 0xFFF);
      smart_wand_params.hi.max_rps_minus  =  half2float((*(uint16_t*)&db.param[2].data[34]));
      smart_wand_params.vac.min_rps_default  =  half2float((*(uint16_t*)&db.param[2].data[36]));
      smart_wand_params.vac.max_rps_default  =  half2float((*(uint16_t*)&db.param[2].data[38]));
      smart_wand_params.vac.kp1  =  half2float((*(uint16_t*)&db.param[2].data[40]));
      smart_wand_params.vac.ki1  =  half2float((*(uint16_t*)&db.param[2].data[42]));
      smart_wand_params.vac.voltage_sp_default  = ( (*(uint16_t*)&db.param[2].data[44]) & 0xFFF);
      smart_wand_params.vac.current_sp_default  =  half2float((*(uint16_t*)&db.param[2].data[46]));
      smart_wand_params.vac.idle_rps  =  half2float((*(uint16_t*)&db.param[2].data[48]));
      smart_wand_params.vac.pulse_window  = (*(uint16_t*)&db.param[2].data[50]);
      smart_wand_params.vac.pulse_energy  = (*(uint16_t*)&db.param[2].data[52]);
      smart_wand_params.vac.ss_time  = (*(uint16_t*)&db.param[2].data[54]);
      smart_wand_params.vac.pulse_delay  = (*(uint16_t*)&db.param[2].data[56]);
      smart_wand_params.vac.pulse_rps  =  half2float((*(uint16_t*)&db.param[2].data[58]));
      smart_wand_params.vac.peak_voltage  = ( (*(uint16_t*)&db.param[2].data[60]) & 0xFFF);
      smart_wand_params.vac.pulsing_voltage  = ( (*(uint16_t*)&db.param[2].data[62]) & 0xFFF);
      smart_wand_params.vac.peak_time  = (*(uint16_t*)&db.param[3].data[0]);
      smart_wand_params.vac.max_time = (*(uint16_t*)&db.param[3].data[2]);
      smart_wand_params.handle.high_warn_en =  (*(uint16_t*)&db.param[3].data[4]);
      smart_wand_params.handle.med_warn_en = (*(uint16_t*)&db.param[3].data[6]);
      smart_wand_params.handle.warn_disable = (*(uint16_t*)&db.param[3].data[8]);
      smart_wand_params.ipeak.voltage = (*(uint16_t*)&db.param[3].data[10]);
      smart_wand_params.ipeak.trip_load = (*(uint16_t*)&db.param[3].data[12]);
      smart_wand_params.ramp_rate.increase = 80;
      smart_wand_params.ramp_rate.decrease = 240;
      
      smart_wand_params.impedance.z_high_mod = 
      smart_wand_params.impedance.z_high;
      smartwand_pwm_init();
      current_therapy_settings.previous_wand_indication = SMART_WAND_MFG_INDICATION_SPORTS_MED;
    }
    else if (smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_ENT)
    {
      
      deserialize_ent_z_params(&db.param[0].head, &smart_wand_params.impedance);
      deserialize_ent_prime_params(&db.param[0].head, &smart_wand_params.ent.prime);
      deserialize_ent_ablate_params(&db.param[0].head, &smart_wand_params.ent.lo);
      deserialize_ent_coag_params(&db.param[0].head, &smart_wand_params.ent.coag[0]);
      deserialize_ent_ipeak_params(&db.param[0].head, &smart_wand_params.ent.ipeak);
      deserialize_ramp_rate_params(&db.param[0].head, &smart_wand_params.ramp_rate);
      smart_wand_params.handle.high_warn_en = 0xFFFF;
      smart_wand_params.handle.med_warn_en = 0xFFFF;
      smart_wand_params.handle.warn_disable = 0xFFFF;
      smartwand_pwm_init();
      current_therapy_settings.previous_wand_indication = SMART_WAND_MFG_INDICATION_ENT;
      
    }
    else if (smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA)
    {
      
      current_therapy_settings.treatment.therapy_mode = COAG;
      deserialize_manta_z_params(&db.param[0].head, &smart_wand_params.impedance);
      deserialize_manta_prime_params(&db.param[0].head, &smart_wand_params.manta.prime);
      deserialize_manta_coag_params(&db.param[0].head, &smart_wand_params.manta.coag[1]);
      deserialize_manta_ipeak_params(&db.param[0].head, &smart_wand_params.manta.ipeak);
      smart_wand_params.coag.coag1_rpm  =  half2float((*(uint16_t*)&db.param[0].data[28]));
      smart_wand_params.coag.coag2_rpm  =  half2float((*(uint16_t*)&db.param[0].data[30]));
      smart_wand_params.coag.coag3_rpm  =  half2float((*(uint16_t*)&db.param[0].data[32]));
      smart_wand_params.coag.coag4_rpm  =  half2float((*(uint16_t*)&db.param[0].data[34]));
      smart_wand_params.coag.coag5_rpm  =  half2float((*(uint16_t*)&db.param[0].data[36]));
      
      
      for (j = 1;j < 9;j++)
      {
        for (i = 1;i < MAX_MANTA_FLOW_SP;i++)
        {
          if(j < 3)
          {
            smart_wand_params.manta.manta_flow_sp[j][i] = ( half2float(*(uint16_t*)&db.param[1].data[44 + k]));
            k += 2;
          }
          else if (j < 9)
          {
            smart_wand_params.manta.manta_flow_sp[j][i] = ( half2float(*(uint16_t*)&db.param[2].data[0 + l]));
            l += 2;
          } 
        }
      }
      smart_wand_params.manta.manta_flow_sp[9][1] = ( half2float(*(uint16_t*)&db.param[2].data[60]));
      smart_wand_params.manta.manta_flow_sp[9][2] = ( half2float(*(uint16_t*)&db.param[2].data[62]));
      smart_wand_params.manta.manta_flow_sp[9][3] = ( half2float(*(uint16_t*)&db.param[3].data[0]));
      smart_wand_params.manta.manta_flow_sp[9][4] = ( half2float(*(uint16_t*)&db.param[3].data[2]));
      smart_wand_params.manta.manta_flow_sp[9][5] = ( half2float(*(uint16_t*)&db.param[3].data[4]));
      smart_wand_params.manta.manta_flow_sp[10][1] = ( half2float(*(uint16_t*)&db.param[3].data[6]));
      smart_wand_params.manta.manta_flow_sp[10][2] = ( half2float(*(uint16_t*)&db.param[3].data[8]));
      smart_wand_params.manta.manta_flow_sp[10][3] = ( half2float(*(uint16_t*)&db.param[3].data[10]));
      smart_wand_params.manta.manta_flow_sp[10][4] = ( half2float(*(uint16_t*)&db.param[3].data[12]));
      smart_wand_params.manta.manta_flow_sp[10][5] = ( half2float(*(uint16_t*)&db.param[3].data[14]));
      
      manta_boundary_limit_flow_vals();
      
      manta_pwm_mod = ( (*(uint16_t*)&db.param[0].data[62]) & 0xFFF);
      manta_pwm_C0V = ( (*(uint16_t*)&db.param[1].data[0]) & 0xFFF);
      manta_pwm_C1V = ( (*(uint16_t*)&db.param[1].data[2]) & 0xFFF);
      manta_pwm_C2V = ( (*(uint16_t*)&db.param[1].data[4]) & 0xFFF);
      manta_pwm_C3V = ( (*(uint16_t*)&db.param[1].data[6]) & 0xFFF);
      manta_pwm_C4V = ( (*(uint16_t*)&db.param[1].data[8]) & 0xFFF);
      manta_pwm_C5V = ( (*(uint16_t*)&db.param[1].data[10]) & 0xFFF);
      manta_pwm_C6V = ( (*(uint16_t*)&db.param[1].data[12]) & 0xFFF);
      manta_pwm_C7V = ( (*(uint16_t*)&db.param[1].data[14]) & 0xFFF);
      prime.blink = TRUE;
      current_therapy_settings.treatment.pre_prime = FALSE;
      smart_wand_params.handle.high_warn_en = 0xFFFF;
      smart_wand_params.handle.med_warn_en = 0xFFFF;
      smart_wand_params.handle.warn_disable = 0xFFFF;
      
      /* Scenario when legacy not connected, APL re-connected on different profile */
      if ((current_therapy_settings.wand.valid == TRUE) &&
        (smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
          (!current_therapy_settings.active_wand.lw_active) &&
            (current_therapy_settings.user_data.settings.active_idx != 0))
    {
      current_therapy_settings.setpoint.coag = current_coag_setpoint;
    }

      current_therapy_settings.treatment.flow_auto = FALSE;
      smart_wand_params.ramp_rate.increase = ( (*(uint16_t*)&db.param[1].data[38]) & 0xFFF); 
      smart_wand_params.ramp_rate.decrease = ( (*(uint16_t*)&db.param[1].data[40]) & 0xFFF);
      smart_wand_params.coag.ss_time = ( (*(uint16_t*)&db.param[1].data[42]) & 0xFFFF);
      
      smart_wand_params.cfg_version = ( (*(uint16_t*)&db.param[4].data[58]) & 0xFFFF);

      /* Boundary limit down ramp-rate */
      if (smart_wand_params.ramp_rate.decrease < 1) 
      {
        smart_wand_params.ramp_rate.decrease = 1;
      }
      else if (smart_wand_params.ramp_rate.decrease > 240)
      {
        smart_wand_params.ramp_rate.decrease = 240;
      }
      else
      {
      }
      if (smart_wand_params.ramp_rate.increase < 1) 
      {
        smart_wand_params.ramp_rate.increase = 1;
      }
      else if (smart_wand_params.ramp_rate.increase >= MAX_12BIT_NUM)
      {
        smart_wand_params.ramp_rate.increase = MAX_12BIT_NUM - 1;
      }
      else
      {
      }
      
      validate_manta_pwm_values();
      smartwand_pwm_init();
      prime.blink = TRUE;
      current_therapy_settings.previous_wand_indication = SMART_WAND_MFG_INDICATION_MANTA;
    }
    
    for (int i = 0; i < 8; i++)
    {
      
      smart_wand_params.header.name[i] = db.passcode.data[i];
      
    }
    
    smart_wand_params.header.hw_version = db.passcode.data[8];
    smart_wand_params.header.sw_version = db.passcode.data[9];
    smart_wand_params.header.year[0] = db.passcode.data[10];
    smart_wand_params.header.year[1] = db.passcode.data[11];
    smart_wand_params.header.month = db.passcode.data[12];
    smart_wand_params.header.day = db.passcode.data[13];
    smart_wand_params.header.checksum = (*(uint16_t*)&db.passcode.data[14]);
    
  }
  
  
  /***************************************************************************//**
  This function periodically logs the errors to the wand
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t periodic_log_errors2wand(void)
  {
    
    uint8_t priority_notify = (current_therapy_settings.notify.status & 0xFF);
    uint8_t wand_error_type = 0;
    
    wand_error_type = wand_error_type_decipher((NOTIFY_TXT)priority_notify);
    
    /* type 2 means notification only applies to legacy wand */
    if ((priority_notify != 0) & (wand_error_type != 2))
    {
      
      static_log.unit[db.static_log.unit_num].errors = priority_notify;
      
      smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG,
                                  db.static_log.page_write,
                                  (uint8_t*)&static_log.unit[db.static_log.unit_num].id,
                                  STATIC_PAGE_SIZE);
      
      smartwand_prepare_tx_packet(SMART_WAND_WRITE_STATIC_LOG,
                                  (db.static_log.page_write + 1),
                                  (uint8_t*)&static_log.unit[db.static_log.unit_num].use_time,
                                  STATIC_PAGE_SIZE);
      
    }
    
    return 0;
    
  }
  
  /***************************************************************************//**
  Sets the use limit time based on wand connect time and time allowances
  programmed in.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint32_t set_use_limit_time(uint32_t *ptr)
  {
    
    uint32_t use_time_limit = 0;
    uint32_t wand_connect_time = *ptr;
    
    if (wand_connect_time == 0)
    {
      
      wand_connect_time = (*(uint32_t*)0x4003D000);
      
    }
    
    use_time_limit = wand_connect_time + smart_wand_params.mfg.use_limit_time_allowance;
    
    return use_time_limit;
    
  }
  
  /***************************************************************************//**
  This function collects data on the number of minutes the wand spends in High,
  Med and low mode. This data is rounded down to a second and is used to display
  wand information on the screen.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  void wand_mode_life_histogram(uint32_t *ptr_1, uint8_t *ptr_2)
  {
    
    smart_wand_params.life.lo = *ptr_1;
    smart_wand_params.life.lo += (*ptr_2)*15;
    smart_wand_params.life.lo += *(ptr_1+1);
    smart_wand_params.life.lo += (*(ptr_2+1))*15;
    smart_wand_params.life.lo += *(ptr_1+2);
    smart_wand_params.life.lo += (*(ptr_2+2))*15;
    
    smart_wand_params.life.med = *(ptr_1+3);
    smart_wand_params.life.med += (*(ptr_2+3))*15;
    smart_wand_params.life.med += *(ptr_1+4);
    smart_wand_params.life.med += (*(ptr_2+4))*15;
    smart_wand_params.life.med += *(ptr_1+5);  
    smart_wand_params.life.med += (*(ptr_2+5))*15;  
    
    smart_wand_params.life.hi = *(ptr_1+6);
    smart_wand_params.life.hi += (*(ptr_2+6))*15;
    smart_wand_params.life.hi += *(ptr_1+7);
    smart_wand_params.life.hi += (*(ptr_2+7))*15;
    smart_wand_params.life.hi += *(ptr_1+8);
    smart_wand_params.life.hi += (*(ptr_2+8))*15;
    
    smart_wand_params.life.vac = *(ptr_1+9);
    smart_wand_params.life.vac += (*(ptr_2+9))*15;
    
    smart_wand_params.coag_life = *(ptr_1+12);
    smart_wand_params.coag_life += (*(ptr_2+12))*15;
    
    
  }  
  
  
  /***************************************************************************//**
  This function is a part of the wand wear algorithm. It checks to see if the
  handle temperature is monotonically decreasing or it is below thershold
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_thandle_wear_monitor(uint8_t handle_temp, uint8_t reset_flag, uint8_t mode_change)
  {
    
    uint8_t worn_flag = current_therapy_settings.wand.thandle_wear,
    sum = 0;
    static uint32_t mean_thandle_wear_fifo[MAX_WORN_WAND_DETECTION_WINDOW],
    thandle_wear_threshold = 0,
    thandle_wear_fifo[MAX_WORN_WAND_DETECTION_WINDOW];
    static uint8_t accum_idx = 0,
    wear_idx = 0;
    
    if (reset_flag == 1)
    {
      
      if (current_therapy_settings.treatment.therapy_mode == LO)
      {
        
        thandle_wear_threshold = smart_wand_params.worn.lo_temp_threshold;
        
      }
      else if (current_therapy_settings.treatment.therapy_mode == MED)
      {
        
        thandle_wear_threshold = smart_wand_params.worn.med_temp_threshold;
        
      }
      else if (current_therapy_settings.treatment.therapy_mode == HI)
      {
        
        thandle_wear_threshold = smart_wand_params.worn.hi_temp_threshold;
        
      }
      
      for(int j = 0; j < MAX_WORN_WAND_DETECTION_WINDOW; j++)
      {
        
        mean_thandle_wear_fifo[j] = thandle_wear_threshold + 1;
        thandle_wear_fifo[j] = 0;
        
#ifdef DEBUG_CAN
        
        CANdebug.wand.thandle_wear[0] = mean_thandle_wear_fifo[0];
        
#endif
        
      }
      
      accum_idx = 0;
      wear_idx = 0;
      worn_flag = 0;
      
    }
    else if (mode_change == 1)
    {
      
      if (current_therapy_settings.treatment.therapy_mode == LO)
      {
        
        thandle_wear_threshold = smart_wand_params.worn.lo_temp_threshold;
        
      }
      else if (current_therapy_settings.treatment.therapy_mode == MED)
      {
        
        thandle_wear_threshold = smart_wand_params.worn.med_temp_threshold;
        
      }
      else if (current_therapy_settings.treatment.therapy_mode == HI)
      {
        
        thandle_wear_threshold = smart_wand_params.worn.hi_temp_threshold;
        
      }
      mean_thandle_wear_fifo[wear_idx] = thandle_wear_threshold + 1;
      thandle_wear_fifo[wear_idx] = 0;
      accum_idx = 0;
      worn_flag = 0;
      
    }
    else if ((current_therapy_settings.treatment.ablate_on == 1) &&
             (current_therapy_settings.treatment.therapy_mode != VAC) &&
               (current_therapy_settings.setpoint.dac > smart_wand_params.impedance.z_level) &&
                 (smart_wand_params.mfg.wand_wear_enable == 1))
    {
      
      if (accum_idx == 0)
      {
        
        mean_thandle_wear_fifo[wear_idx] = handle_temp;
        
      }
      else
      {
        
        mean_thandle_wear_fifo[wear_idx] += handle_temp;
        
      }
      
      if (accum_idx++ > 99)
      { 
        
        worn_flag = 1;
        
        /* test whether monotonically decreasing */
        for (int i = 1; i < smart_wand_params.worn.detect_duration; i++)
        {
          
          worn_flag &= (mean_thandle_wear_fifo[i-1] > mean_thandle_wear_fifo[i]);
          
        }
        
        /* not decreasing then check if any are above threshold */
        if (worn_flag == 0)
        {
          
          thandle_wear_fifo[wear_idx] = 1;
          
          if (mean_thandle_wear_fifo[wear_idx] > thandle_wear_threshold)
          {
            
            thandle_wear_fifo[wear_idx] = 0;
            
          }
          
          for (int i = 0; i < MAX_WORN_WAND_DETECTION_WINDOW; i++)
          {
            
            sum += thandle_wear_fifo[i];
            
          }
          
          worn_flag = sum >= smart_wand_params.worn.detect_duration ? 1 : 0;
          
        }
        else
        {
          
          asm("nop");
          
        }
        
#ifdef DEBUG_CAN
        
        CANdebug.wand.thandle_wear[0] = (mean_thandle_wear_fifo[wear_idx] / 100) & 0xFFFF;
        CANdebug.wand.thandle_worn = worn_flag;
        CANdebug.wand.thandle_wear_threshold_lo = smart_wand_params.worn.lo_temp_threshold;
        CANdebug.wand.thandle_wear_threshold_medhi = smart_wand_params.worn.med_temp_threshold;
        
#endif
        
        accum_idx = 0;
        wear_idx = (wear_idx + 1) % smart_wand_params.worn.detect_duration;
        
      }
      
      
    }
    
#ifdef DEBUG_CAN
    
    //  for (int i = 0; i < 7; i++)
    //  {
    //    
    //    if (i != wear_idx)
    //    {
    //      
    //      CANdebug.wand.thandle_wear[i] = (mean_thandle_wear_fifo[i] / 100) & 0xFF;
    //      
    //    }
    //    CANdebug.wand.thandle_worn = worn_flag;
    
    //}
    
#endif
    
    return worn_flag;
    
  }
  
  /***************************************************************************//**
  calls the wand wear function
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_wear_monitor(void)
  {
    
    current_therapy_settings.wand.irms_wear = (bool)wand_irms_wear_monitor(0,0);
    
    return 0;
    
  }
  
  /***************************************************************************//**
  Implements the algorithm to monitor wand wear. It tests to see if the rms
  current is below the threshold (part of the wand wear algorithm)
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_irms_wear_monitor(uint8_t reset_flag, uint8_t mode_change)
  {
    
    uint8_t worn_flag = current_therapy_settings.wand.irms_wear,
    sum = 0;  
    uint16_t irms_diff = 0;
    static q15_t last_mean_irms = 0;
    static uint32_t mean_irms_wear_fifo[MAX_WORN_WAND_DETECTION_WINDOW],
    irms_wear_threshold = 0,
    irms_wear_fifo[MAX_WORN_WAND_DETECTION_WINDOW];
    static uint8_t accum_idx = 0,
    wear_idx = 0;
    q15_t filt_mean_irms_q15 = 0; 
    uint32_t blockSize = 1;
    
    
    if (reset_flag == 1)
    {
      
      if (current_therapy_settings.treatment.therapy_mode == LO)
      {
        
        irms_wear_threshold = smart_wand_params.worn.lo_irms_threshold;
        
      }
      else if (current_therapy_settings.treatment.therapy_mode == MED)
      {
        
        irms_wear_threshold = smart_wand_params.worn.med_irms_threshold;
        
      }
      else if (current_therapy_settings.treatment.therapy_mode == HI)
      {
        
        irms_wear_threshold = smart_wand_params.worn.hi_irms_threshold;
        
      }
      
      for(int j = 0; j < MAX_WORN_WAND_DETECTION_WINDOW; j++)
      {
        
        mean_irms_wear_fifo[j] = irms_wear_threshold + 1;
        irms_wear_fifo[j] = 0;
        
#ifdef DEBUG_CAN
        
        CANdebug.wand.irms_wear[0] = mean_irms_wear_fifo[0];
        CANdebug.wand.irms_wear_threshold_lo = smart_wand_params.worn.lo_irms_threshold;
        CANdebug.wand.irms_wear_threshold_medhi = smart_wand_params.worn.med_irms_threshold;
        
#endif
        
      }
      
      accum_idx = 0;
      wear_idx = 0;
      worn_flag = 0;
      last_mean_irms = 0;
      current_therapy_settings.wand.irms_worn_count = 0;
      
      WandFIR_Init(0);
      
    }
    else if (mode_change == 1)
    {
      
      if (current_therapy_settings.treatment.therapy_mode == LO)
      {
        
        irms_wear_threshold = smart_wand_params.worn.lo_irms_threshold;
        
      }
      else if (current_therapy_settings.treatment.therapy_mode == MED)
      {
        
        irms_wear_threshold = smart_wand_params.worn.med_irms_threshold;
        
      }
      else if (current_therapy_settings.treatment.therapy_mode == HI)
      {
        
        irms_wear_threshold = smart_wand_params.worn.hi_irms_threshold;
        
      }
      mean_irms_wear_fifo[wear_idx] = irms_wear_threshold + 1;
      irms_wear_fifo[wear_idx] = 0;
      accum_idx = 0;
      worn_flag = 0;
      last_mean_irms = 0;
      
    }  
    else if ((current_therapy_settings.treatment.ablate_on == 1) &&
             (current_therapy_settings.treatment.therapy_mode != VAC) &&
               (current_therapy_settings.setpoint.dac > smart_wand_params.impedance.z_level) &&
                 (smart_wand_params.mfg.wand_wear_enable == 1))
    {
      
       __istate_t s = __get_interrupt_state();
    __disable_interrupt();
    /* filter process variable */
    //PUBLIC_CAN_SDO_MSG_T *sdo = (PUBLIC_CAN_SDO_MSG_T*)((unsigned char*)msg+offsetof(CAN_MB_T,word_0));
    arm_fir_q15 (&wand_wear_fir, 
                 &current_therapy_settings.pump.mean_irms,
                 &filt_mean_irms_q15,
                 blockSize);    
    __set_interrupt_state(s);
      
      if (last_mean_irms >= filt_mean_irms_q15)
      {
        
        irms_diff = last_mean_irms - filt_mean_irms_q15;
        
      }
      else
      {
        
        irms_diff = filt_mean_irms_q15 - last_mean_irms;
        
      }
      
      last_mean_irms = filt_mean_irms_q15;
      
      if (accum_idx == 0)
      {
        
        mean_irms_wear_fifo[wear_idx] = irms_diff;
        
      }
      else
      {
        
        mean_irms_wear_fifo[wear_idx] += irms_diff;
        
      }
      
      if (accum_idx++ > 99)
      { 
        
        irms_wear_fifo[wear_idx] = 1;
        
        if (mean_irms_wear_fifo[wear_idx] > irms_wear_threshold)
        {
          
          current_therapy_settings.wand.irms_worn_count = 0;
          irms_wear_fifo[wear_idx] = 0;
          
        }
        else if (current_therapy_settings.wand.irms_worn_count < MAX_WORN_WAND_DETECTION_WINDOW)
        {
          
          current_therapy_settings.wand.irms_worn_count++;
          
        }
        
        for (int i = 0; i < MAX_WORN_WAND_DETECTION_WINDOW; i++)
        {
          
          sum += irms_wear_fifo[i];
          
        }
        
        worn_flag = sum >=  smart_wand_params.worn.detect_duration ? 1 : 0;
        
#ifdef DEBUG_CAN
        
        CANdebug.wand.irms_wear[0] = (mean_irms_wear_fifo[wear_idx] & 0xFFFFFFFF);
        CANdebug.wand.filt_wear_irms = (uint16_t) filt_mean_irms_q15;
        CANdebug.wand.irms_worn = worn_flag;
        
#endif
        
        accum_idx = 0;
        wear_idx = (wear_idx + 1) % smart_wand_params.worn.detect_duration;
        
      }
      
      
    }
    
#ifdef DEBUG_CAN
    
    //  for (int i = 0; i < 7; i++)
    //  {
    //    
    //    if (i != wear_idx)
    //    {
    //      
    //      CANdebug.wand.irms_wear[i] = (mean_irms_wear_fifo[i] / 100) & 0xFFFF;
    //    
    //    }
    //    CANdebug.wand.irms_worn = worn_flag;
    //    
    //  }
    
#endif
    
    return worn_flag;
    
  }
  
  
  /***************************************************************************//**
  Inits the FIR Filter used in pump pid algorithm.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  void WandFIR_Init(q15_t init_value)
  {
    
    uint16_t numTaps = 8;
    uint32_t blockSize = 1;
    /* state size is numTaps + blockSize */
    static q15_t wand_fir_state[9],
    coeffs[8] = {0,205,2454,7988,11474,7988,2454,205};
    arm_status status;
    
    status = arm_fir_init_q15 (&wand_wear_fir,
                               numTaps, 
                               &coeffs[0], 
                               &wand_fir_state[0], 
                               blockSize);
    
    if (status == ARM_MATH_SUCCESS)
    {
      
      //memset(&state[0],init_value,sizeof(state));
      wand_fir_state[0] = init_value;
      wand_fir_state[1] = init_value;
      wand_fir_state[2] = init_value;
      wand_fir_state[3] = init_value;
      wand_fir_state[4] = init_value;
      wand_fir_state[5] = init_value;
      wand_fir_state[6] = init_value;
      wand_fir_state[7] = init_value;
      
    }
    
  }
  
  /***************************************************************************//**
  This function is used to switch to WW wand.
  @none
  @return 1
  @outputs
  @context: 
  *******************************************************************************/
  uint8_t switch_to_ww_wand(void)
  {
    current_therapy_settings.active_wand.lw_active = 0;
    
    current_therapy_settings.ambient.temp_sync = 0;
    
    // update the temp
    if (current_therapy_settings.wand.valid == TRUE &&
        current_therapy_settings.gui.screenID == RF_2000_WAND_TREATMENT_SCREEN)
    {
      current_therapy_settings.ambient.temperature = convert_q62_ambient_to_7bit(db.param[4].temp2);
      current_therapy_settings.ambient.thermometer_color = thermometer_color(current_therapy_settings.ambient.temperature);
    }
    current_therapy_settings.ambient.capable = smart_wand_params.mfg.ambient_enable;
    current_therapy_settings.ambient.enable = (current_therapy_settings.ambient.capable &
                                               current_therapy_settings.ambient.enable_user_pref);
    
    current_therapy_settings.notify.status &= 
      ~AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
    current_therapy_settings.notify.smart_wand_no_ambient = 0;
    
    return 1;
  }
  
  
  /***************************************************************************//**
  Route valid packet signals to appropriate data structures.
  @param void
  @return 0
  @outputs
  @context
  *******************************************************************************/
  uint8_t wand_not_active_periodic_packet_parse(void)
  {
    
    uint8_t ablate = 0,
    coag = 0,
    vac = 0;
    
    /* Mode key status */
    uint8_t mode_key = 0;
    
    if (current_therapy_settings.wand.valid == TRUE)
    {
      /* Parse & copy packet contents into global structure */
      ablate = (uint8_t) ((db.periodic.key & ABLATE_KEY_MASK) == ABLATE_KEY_MASK) ? 1 : 0;
      coag = (uint8_t) ((db.periodic.key & COAG_KEY_MASK) == COAG_KEY_MASK) ? 1 : 0;
      /* Mode key is handled differently since duration will dictate function */
      mode_key = (uint8_t) ((db.periodic.key & MODE_KEY_MASK) == MODE_KEY_MASK) ? 1 : 0;
      
      current_therapy_settings.ambient.smart_wand_temperature = convert_q62_ambient_to_7bit(db.periodic.temp2);
      
      if (mode_key != current_therapy_settings.wand.mode_key)
      {
        
        if (mode_key == 0)
        {
          
          put_event(&evt_tbl,CHANGE_MODE);
          
        }
        
      }
      
      if (ablate != current_therapy_settings.wand.cut_key)
      {
        
        if (ablate == 1)
        {
          
          put_event(&evt_tbl,SWITCH_WAND);
          
        }
        else
        {
          
          put_event(&evt_tbl,THERAPY_OFF);
          
        }
        
      }
      
      if (coag != current_therapy_settings.wand.coag_key)
      {
        
        if (coag == 1)
        {
          
          put_event(&evt_tbl,SWITCH_WAND);
          
        }
        else
        {
          
          put_event(&evt_tbl,THERAPY_OFF);
          
        }
        
      }
      
      if (vac != current_therapy_settings.wand.vac_key)
      {
        
        if (vac == 1)
        {
          
          put_event(&evt_tbl,SWITCH_WAND);
          
        }
        else
        {
          
          put_event(&evt_tbl,THERAPY_OFF);
          
        }
        
      }
      current_therapy_settings.wand.cut_key = ablate;
      current_therapy_settings.wand.coag_key = coag;
      current_therapy_settings.wand.vac_key = vac;
      current_therapy_settings.wand.mode_key = mode_key;
    }
    
    return 0;
    
  }
  
  
  /***************************************************************************//**
  This function limits the use of the wand if it detects a worn wand
  @param void
  @return 0
  @outputs
  @context
  use-limited due to degradation can not be disabled b/c safety feature so no
  check use-limiting feature enabled bit in mfg data necessary.
  *******************************************************************************/
  uint8_t wand_worn_use_limit(void)
  {
    const uint8_t ONE_MINUTE = 60;
    uint8_t use_limit_flag = 0;
    
    uint8_t long_detection_count = (smart_wand_params.worn.detect_duration  << 1);
    uint8_t worn_flag = (((current_therapy_settings.wand.thandle_wear == TRUE) &&
                          (current_therapy_settings.wand.irms_wear == TRUE)) ||
                         (current_therapy_settings.wand.irms_worn_count >= long_detection_count));
    
    if (current_therapy_settings.wand.worn_timer_en == 1)
    {
      
      if (current_therapy_settings.wand.worn_active_time<UCHAR_MAX)
      {
        
        current_therapy_settings.wand.worn_active_time++;
        
      }
      
      if ((worn_flag == 1) && (current_therapy_settings.wand.worn_active_time > ONE_MINUTE))
      {
        
        current_therapy_settings.wand.worn_latch = 1;
        
      }
      
      /*
            max_time_diff = current_therapy_settings.wand.worn_timestamp[3] -
                        current_therapy_settings.wand.worn_timestamp[0];
        
          if ((max_time_diff < 120) &&
              (current_therapy_settings.wand.worn_active_time > 121) &&
              (current_therapy_settings.wand.worn > 4))
          {
            
            for (int i = 0; i < 4; i++)
            {
              
              current_therapy_settings.wand.worn_timestamp[i] = 0;
              
            }
            //put_event(&evt_tbl,WAND_USE_LIMIT_ERROR);
            use_limit_flag = 1;   
      
          }
      */
      
      
    }
    else if (worn_flag == 1)
    {
      
      current_therapy_settings.wand.worn_latch = 1;
      
    }
    
    return use_limit_flag;
    
  }
  
  /***************************************************************************//**
  Returns the impedence limit adjusted for temp
  @param ambient temp
  @return impedance value
  @outputs
  @context
  *******************************************************************************/
  uint16_t z_limit_adjust4temp(uint8_t ambient)
  {
    
    const uint16_t SIXTY_THREE_DEGREES = 212;
    const uint16_t NO_SENSOR_DETECTED = 0;
    uint32_t z_original = smart_wand_params.impedance.z_high << 3;
    uint16_t adjustment = 0,
    z_high_mod = 0;
    
    if ((ambient > SIXTY_THREE_DEGREES) || (ambient == NO_SENSOR_DETECTED) ||
        (smart_wand_params.mfg.ambient_enable == 0))
    {
      z_high_mod = smart_wand_params.impedance.z_high;
    }
    else
    {
      
      adjustment = SIXTY_THREE_DEGREES - ambient;
      adjustment *= smart_wand_params.impedance.temp_comp_gain;
      if (z_original > adjustment) 
      {
        
        z_high_mod = z_original - adjustment;
        z_high_mod >>= 3;
        
      }
      else
      {
        
        z_high_mod = 0;
        
      }
      
    }
    
    return z_high_mod;
    
  }
  
  
  /***************************************************************************//**
  Returns the WW wand temp to be used elsewhere in the code
  @param void
  @return 0
  @outputs
  @context
  use-limited due to degradation can not be disabled b/c safety feature so no
  check use-limiting feature enabled bit in mfg data necessary.
  *******************************************************************************/
  uint8_t get_ww_temp (void)
  {
    return (convert_q62_ambient_to_7bit(db.param[4].temp2));
  }
  
  /***************************************************************************//**
  Deserializes raw wand parameter data into the low voltage impedance test object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing the low voltage impedance test.
  @return params pointer to the same input struct.
  *******************************************************************************/
  IMPEDANCE_PARAMETERS *deserialize_ent_z_params(const uint8_t *data, IMPEDANCE_PARAMETERS *const params)
  {
    uint16_t temp_comp_gain = GET_TWO_BYTES(data + ENT_PARAM_Z_GAIN_LSB);
    
    if (temp_comp_gain > UINT8_MAX)
    {
      temp_comp_gain = UINT8_MAX;
    }
    params->z_high = GET_TWO_BYTES(data + ENT_PARAM_HIGH_Z_LIMIT_LSB);
    params->z_low = GET_TWO_BYTES(data + ENT_PARAM_LOW_Z_LIMIT_LSB);
    params->z_level = GET_TWO_BYTES(data + ENT_PARAM_Z_VOLTAGE_LSB);
    params->z_time = (uint32_t)GET_TWO_BYTES(data + ENT_PARAM_Z_TIME_LSB);
    if (smart_wand_params.mfg.format > 2)
    {
      params->temp_comp_gain = temp_comp_gain;
    }
    params->z_high_mod = params->z_high;
    
    return params;
  }
  
  /***************************************************************************//**
  @MANTA
  Deserializes raw wand parameter data into the low voltage impedance test object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing the low voltage impedance test.
  @return params pointer to the same input struct.
  *******************************************************************************/
  IMPEDANCE_PARAMETERS *deserialize_manta_z_params(const uint8_t *data, IMPEDANCE_PARAMETERS *const params)
  {
    uint16_t temp_comp_gain = GET_TWO_BYTES(data + MANTA_PARAM_Z_GAIN_LSB);
    
    if (temp_comp_gain > UINT8_MAX)
    {
      temp_comp_gain = UINT8_MAX;
    }
    params->z_high = GET_TWO_BYTES(data + MANTA_PARAM_HIGH_Z_LIMIT_LSB);
    params->z_low = GET_TWO_BYTES(data + MANTA_PARAM_LOW_Z_LIMIT_LSB);
    params->z_level = GET_TWO_BYTES(data + MANTA_PARAM_Z_VOLTAGE_LSB);
    params->z_time = (uint32_t)GET_TWO_BYTES(data + MANTA_PARAM_Z_TIME_LSB);
    params->z_high_mod = params->z_high;
    if (smart_wand_params.mfg.format > 2)
    {
      params->temp_comp_gain = temp_comp_gain;
    }
    return params;
  }
  
  /***************************************************************************//**
  Deserializes raw wand parameter data into the inflow pump priming object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing the inflow priming.
  @return params pointer to the same input struct.
  *******************************************************************************/
  ENT_PRIME_PARAMS_T *deserialize_ent_prime_params(const uint8_t *data, ENT_PRIME_PARAMS_T *const params)
  {
    uint16_t spd = GET_TWO_BYTES(data + ENT_PARAM_PRE_PRIME_SPD_LSB);
    params->pre.spd = pump_spd_check(half2float(spd));
    uint16_t pre_prime_time = GET_TWO_BYTES(data + ENT_PARAM_PRE_PRIME_TIME_LSB);
    /* sr2109: pre-prime limited to 5 second or less */
    if (pre_prime_time > 250)
    {
      params->pre.time = 250;
    }
    else
    {
      params->pre.time = pre_prime_time;
    }
    uint16_t rf_delay = GET_TWO_BYTES(data + ENT_PARAM_PRE_PRIME_RF_DELAY_LSB);
    /* sr2129: rf-delay limited to 5s or less */
    if (rf_delay > 250)
    {
      params->pre.rf_delay = 250;
    }
    else
    {
      params->pre.rf_delay = rf_delay;
    }
    spd = GET_TWO_BYTES(data + ENT_PARAM_AUTO_PRIME_SPD_LSB);
    params->autom.spd = pump_spd_check(half2float(spd));
    params->autom.time = GET_TWO_BYTES(data + ENT_PARAM_AUTO_PRIME_TIME_LSB);
    return params;
  }
  
  /***************************************************************************//**
  @MANTA
  Deserializes raw wand parameter data into the inflow pump priming object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing the inflow priming.
  @return params pointer to the same input struct.
  *******************************************************************************/
  MANTA_PRIME_PARAMS_T *deserialize_manta_prime_params(const uint8_t *data, MANTA_PRIME_PARAMS_T *const params)
  {
    uint16_t spd = GET_TWO_BYTES(data + MANTA_PARAM_PRE_PRIME_SPD_LSB);
    params->pre.spd = pump_spd_check(half2float(spd));
    uint16_t pre_prime_time = GET_TWO_BYTES(data + MANTA_PARAM_PRE_PRIME_TIME_LSB);
    /* sr2109: pre-prime limited to 5 second or less */
    if (pre_prime_time > 250)
    {
      params->pre.time = 250;
    }
    else
    {
      params->pre.time = pre_prime_time;
    }
    uint16_t rf_delay = GET_TWO_BYTES(data + MANTA_PARAM_PRE_PRIME_RF_DELAY_LSB);
    /* sr2129: rf-delay limited to 5s or less */
    if (rf_delay > 250)
    {
      params->pre.rf_delay = 250;
    }
    else
    {
      params->pre.rf_delay = rf_delay;
    }
    spd = GET_TWO_BYTES(data + MANTA_PARAM_MANUAL_PRIME_SPD_LSB);
    params->man.spd = pump_spd_check(half2float(spd));
    spd = GET_TWO_BYTES(data + MANTA_PARAM_AUTO_PRIME_ADJ_LSB);
    params->autom.adj = pump_spd_check(half2float(spd));
    spd = GET_TWO_BYTES(data + MANTA_PARAM_AUTO_PRIME_INIT_SPD_LSB);
    params->autom.initial_spd = pump_spd_check(half2float(spd));
    spd = GET_TWO_BYTES(data + MANTA_PARAM_AUTO_PRIME_FINAL_SPD_LSB);
    params->autom.final_spd = pump_spd_check(half2float(spd));
    params->autom.time = GET_TWO_BYTES(data + MANTA_PARAM_AUTO_PRIME_TIMEOUT_LSB);
    if(params->autom.time == 0)
    {
      params->autom.time = 1000; // 1s default timeout if config file value is 0
    }
    params->autom.rms_threshold = GET_TWO_BYTES(data + MANTA_PARAM_AUTO_PRIME_RMS_THRESH_LSB);
    return params;
  }
  
  /***************************************************************************//**
  Deserializes raw wand parameter data into the ablate object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing ablate mode.
  @return params pointer to the same input struct.
  *******************************************************************************/
  ENT_ABLATE_MODE_PARAMS_T *deserialize_ent_ablate_params(const uint8_t *data, ENT_ABLATE_MODE_PARAMS_T *params)
  {
    data += ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_LO_MINUS_LSB;
    
    /* NOTE: The order of decoding here is important because the array pointer, data, increments each time get_contiguous_params is called; this for-loop assumes param blocks are contiguous */
    for (MODES m = (MODES)0; m < HI; m++)
    {
      for (uint8_t l = 0; l < MAX_NUM_OF_CUT_LEVELS_PER_MODE; l++)
      {
        params->level[l].primary_voltage = dac_check(get_contiguous_params(&data));
        params->level[l].secondary_voltage = dac_check(get_contiguous_params(&data));
        params->level[l].blend_primary_voltage_time = get_contiguous_params(&data);
        params->level[l].blend_total_time = get_contiguous_params(&data);
        params->level[l].flow[ENT_FLOW_0] = 0.0f;
        for (ENT_FLOW_SP_T f = ENT_FLOW_1; f < MAX_ENT_FLOW_SP; f++)
        {
          uint16_t spd = get_contiguous_params(&data);
          params->level[l].flow[f] = pump_spd_check(half2float(spd));
        }
      }
      params->rms_current_limit = get_contiguous_params(&data);
      params->pulse_window = get_contiguous_params(&data);
      params->pulse_energy = get_contiguous_params(&data);
      params++;
    }
    
    return params;
  }
  
  /***************************************************************************//**
  Used to extract wand parameters from a contiguous data array. 
  @param[in] data pointer to a pointer where the bytes will be copied from.
  @return r number that was extracted.
  *******************************************************************************/
  uint16_t get_contiguous_params(const uint8_t **data)
  {
    uint16_t r = 0;
    r = GET_TWO_BYTES(*data);
    
    if (*data < &db.param[4].checksum)
    {
      *data += 2;
    }
    
    if (*data == &db.param[0].checksum)
    {
      *data = &db.param[1].data[0];
    }
    else if (*data == &db.param[1].checksum)
    {
      *data = &db.param[2].data[0];
    }
    else if (*data == &db.param[2].checksum)
    {
      *data = &db.param[3].data[0];
    }
    else if (*data == &db.param[3].checksum)
    {
      *data = &db.param[4].data[0];
    }
    
    return r;
  }
  
  /***************************************************************************//**
  Used to check that the DAC values are w/in 12-bit bounds. 
  @param[in] dac the value that will be checked.
  @return r same value as input with 12-bit max value.
  *******************************************************************************/
  uint16_t dac_check(const uint16_t dac)
  {
    return (dac >= MAX_12BIT_NUM) ? (MAX_12BIT_NUM-1) : dac;
  }
  
  /***************************************************************************//**
  Used to check pump speed values are w/in bounds. 
  @param[in] spd the value that will be checked.
  @return spd same value as input with 10 max & 0 min.
  *******************************************************************************/
  float pump_spd_check(float spd)
  {
    if (spd > 10.0f)
    {
      spd = 10.0f;
    }
    else if (spd < 0.0f)
    {
      spd = 0.0f;
    }
    return spd;
  }
  
  /***************************************************************************//**
  Deserializes raw wand parameter data into the ablate object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing ablate mode.
  @return params pointer to the same input struct.
  *******************************************************************************/
  ENT_COAG_MODE_PARAMS_T *deserialize_ent_coag_params(const uint8_t *data, ENT_COAG_MODE_PARAMS_T params[])
  {
    data += ENT_PARAM_COAG_1_PRIMARY_PWR_LSB;
    /* NOTE: The order of decoding here is important because the array pointer, data, increments each time get_contiguous_params is called; this for-loop assumes param blocks are contiguous */
    for (uint8_t n = 0; n < MAX_NUM_OF_ENT_COAG_LEVELS; n++)
    {
      params[n].primary = get_contiguous_params(&data);
      params[n].secondary = get_contiguous_params(&data);
      params[n].blend_primary_time = get_contiguous_params(&data);
      params[n].blend_total_time = get_contiguous_params(&data);
      params[n].primary_v_limit = dac_check(get_contiguous_params(&data));
      params[n].secondary_v_limit = dac_check(get_contiguous_params(&data));
      uint16_t spd = get_contiguous_params(&data);
      params[n].pump_spd = half2float(spd);
    }
    return &params[0];
  }
  
  /***************************************************************************//**
  @MANTA
  Deserializes raw wand parameter data into the coag object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing coag levels.
  @return params pointer to the same input struct.
  *******************************************************************************/
  uint16_t *deserialize_manta_coag_params(const uint8_t *data, uint16_t params[])
  {
    data += MANTA_PARAM_COAG_1_PRIMARY_PWR_LSB;
    /* NOTE: The order of decoding here is important because the array pointer, data, increments each time get_contiguous_params is called; this for-loop assumes param blocks are contiguous */
    for (uint8_t n = 0; n < NUM_COAG_LEVELS; n++)
    {
      params[n] = get_contiguous_params(&data);
      
    }
    return &params[0];
  }
  
  /***************************************************************************//**
  Deserializes raw wand parameter data into the over current (ipeak) object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing ipeak.
  @return params pointer to the same input struct.
  *******************************************************************************/
  SCOPE_SAVER_PARAMETERS *deserialize_ent_ipeak_params(const uint8_t *data, SCOPE_SAVER_PARAMETERS *const params)
  {
    params->voltage = GET_TWO_BYTES(data + ENT_PARAM_IPEAK_VOLTAGE_LSB);
    params->trip_load = GET_TWO_BYTES(data + ENT_PARAM_IPEAK_LOAD_LSB);
    return params;
  }

  /***************************************************************************//**
  @MANTA
  Deserializes raw wand parameter data into the over current (ipeak) object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing ipeak.
  @return params pointer to the same input struct.
  *******************************************************************************/
  SCOPE_SAVER_PARAMETERS *deserialize_manta_ipeak_params(const uint8_t *data, SCOPE_SAVER_PARAMETERS *const params)
  {
    params->voltage = GET_TWO_BYTES(data + MANTA_PARAM_IPEAK_VOLTAGE_LSB);
    params->trip_load = GET_TWO_BYTES(data + MANTA_PARAM_IPEAK_LOAD_LSB);
    return params;
  }
  
  /***************************************************************************//**
  Deserializes raw wand parameter data into the ramp rate object.
  @param[in] data pointer to start of data array where the wand parameters reside.
  @param[in] params pointer to object describing ramp rate.
  @return params pointer to the same input struct.
  *******************************************************************************/
  ABLATE_RAMP_RATE_PARAMS_T *deserialize_ramp_rate_params(const uint8_t *data, ABLATE_RAMP_RATE_PARAMS_T *const params)
  {
    uint16_t increase = GET_TWO_BYTES(data + ENT_PARAM_ABLATE_RAMP_UP_RATE_LSB);
    uint16_t decrease = GET_TWO_BYTES(data + ENT_PARAM_ABLATE_RAMP_DOWN_RATE_LSB);
    
    /* the dac is 12-bit */
    if (increase >= MAX_12BIT_NUM)
    {
      increase = MAX_12BIT_NUM - 1;
    }
    else if (increase == 0)
    {
      increase = 1;
    }
    
    /* 240 is a hardware limitation */
    if (decrease > 240)
    {
      decrease = 240;
    }
    else if (decrease == 0)
    {
      decrease = 1;
    }
    
    params->increase = increase;  
    params->decrease = decrease;
    
    return params;
  }
  
  
  /***************************************************************************//**
  @MANTA
  This function is used to increment the coag level. If the level is incremented
  below the max level, it rolls over to min.
  @param void
  @return 0
  @outputs
  @context: 
  *******************************************************************************/
  uint8_t manta_coag_mode_increment()
  {
    uint8_t level = current_therapy_settings.setpoint.coag;
    level++;
    if (level > 10)
    {
      // No rolling over
      level = 1;
    }
    current_therapy_settings.setpoint.coag = level;
    current_coag_setpoint = level;
    return 0;
  }

  /***************************************************************************//**
  @MANTA
  This function is used to boundary limit MANTA FLOW values between 0 and 9.99
  @param void
  @return void
  @outputs
  @context: 
  *******************************************************************************/
  void manta_boundary_limit_flow_vals(void)
  {
    uint8_t i = 0;
    uint8_t j = 0;
    for (i = 1;i < NUM_COAG_LEVELS;i++)
    {
      for (j = 1;j < MAX_MANTA_FLOW_SP;j++)
      {
        if(smart_wand_params.manta.manta_flow_sp[i][j] >= 10.0f)
        {
          smart_wand_params.manta.manta_flow_sp[i][j] = 9.99f;
        }
        else if (smart_wand_params.manta.manta_flow_sp[i][j] < 0.0f)
        {
          smart_wand_params.manta.manta_flow_sp[i][j] = 0.0f;
        }
      }
    }
  }
  
  