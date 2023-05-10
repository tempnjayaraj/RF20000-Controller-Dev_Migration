/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2813                 $:
$Date:: 2022-02-16 08:40:23#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      pump.c
\brief     Driver to control the pump
\details   NA
*******************************************************************************/

#include "MK26F18.h"
#include "common.h"
#include "pump.h"
#include <stdio.h>
#include "system.h"
#include "pit.h"
#include "cpu.h"
#include "buck.h"
#include "rf.h"
#include <stdint.h>
#include <string.h>
#include "notifications.h"
#include "sensors.h"
#include "psu.h"

#ifdef DEBUG_CAN
  
  #include "can_debug.h"

#endif

/* FIR data structure */
arm_fir_instance_q15 fir;

/* Specification for speed command */
const char SPEED_CMD_SPEC[] = "CSxxx.xxxxxxx";

/* Specification for truncated speed command */
const char FMT_SPEED_CMD_SPEC[] = "CSxx.xx";

const char *pcmd[] =
{      
/* 0:  Protocol 4 - return ack byte */      "PR4\r",
/* 1:  Request power-up mode */             "PM\r",
/* 2:  Set baud rate @ 38,400 */            "BR3\r",
/* 3:  Point-to-point positioning */        "CM21\r",
/* 4:  Motor disable */                     "MD\r",
/* 5:  Maximum rms current (1.2 * 4.24A) */ "MC5\r",
/* 6:  Continuous current @ 5 amps */       "CC5\r",
/* 7:  Anti-resonance filter frequency */   "CF162\r",
/* 8:  Anti-resonance filter gain */        "CG13000\r",
/* 9:  Motor probe resonance parameters */  "PN\r",
/* 10: Delay time @ 10 sec */               "CD10\r",
/* 11: Idle current @ 4.5 amps */           "CI4.5\r",
/* 12: 4th Harmonic Filter Phase */         "HP-110\r",
/* 13: 4th Harmonic Filter Gain */          "HG627\r",
/* 14: Jog acceleration 300 rps */          "JA150\r",
/* 15: Jog decelerlation 300 rps */         "JL150\r",
/* 16: Set jog speed @ 0 rps */             "JS0\r",
/* 17: Request Status */                    "RS\r",
/* 18: Motor Enable */                      "ME\r",
/* 19: Commence jog */                      "CJ\r",
/* 20: Idle speed */                        "\r",
/* 21: Alarm reset */                       "AR\r",
/* 22: Stop and Kill and Disable */         "SKD\r",
/* 23: Set speed to zero */                 "CS0\r",
/* 24: VAC pulse speed */                   "\r",
/* 25: Soft reset */                        "RE\r",
/* 26: Power-up in SCL mode */              "PM2\r",
/* 27: Forced SCL mode */                   "00",
///* Stop and Kill */                     "SK\r",
///* Idle current delay time @ 10 secs */ "CD10\r",
///* Idle current @ 5.0 amps */           "CI5\r",
///* Stop jog */                          "SJ\r",					
//                                        "RS\r",

};

PUMP_CMD pump;
SPEED_CTRL_STATES spd_ctrl_state;

/***************************************************************************//**
Fixed point, 16-bit PID controller for adjusting suction pump speed
to maximize RF performance based on measured RMS measurement of AC current.
@param pv- current value (x[i] input to the filter), reset - reset flag
@return PID controller output
@outputs
@context
*******************************************************************************/
float32_t PumpPID(q15_t pv, bool reset)
{

  static PID pid_history;  

  PID pid;
          
  q15_t filt_pv_q15 = 0;
  
  float32_t filt_pv_f32 = 0;
  
  uint32_t blockSize = 1;
   
  /* Setpoint controller will attempt to achieve */
  float32_t sp = current_therapy_settings.pump.pid.sp_amps;
  
  /* Last pump command saved to avoid sending duplicate commands */
  static uint8_t last_pump_cmd[sizeof(FMT_SPEED_CMD_SPEC)];
    
  /* Flag indicating current command is identical to previous */
  //bool f_identical_cmds = FALSE;
  
  /* Last therapy mode saved so PID can reinitialize when change occurs */
  //static MODES last_therapy_mode = NONE;
  
  
  
//  /* Overflow check before calculating error */
//  if ((sp >= 0) && (pv <= (int16_t) (INT_MIN + sp))) {
//    
//    //printf("overflow\n");
//    
//  }
//  
//  else if ((sp < 0) && (pv > (int16_t) (INT_MAX + sp))) {
//    
//    //printf("overflow\n");
//    
//  }
  
//  else {

//    arm_biquad_cascade_df1_q15 (&iir,
//                            (q15_t *) &pv,
//                            (q15_t *) &filt_pv,
//                            blockSize);

   
    __istate_t s = __get_interrupt_state();
    __disable_interrupt();
    /* filter process variable */
    arm_fir_q15 (&fir, &pv, &filt_pv_q15, blockSize);     
    __set_interrupt_state(s);

    /* convert pv counts to amps */
    filt_pv_f32 = ((float32_t)(filt_pv_q15 << 1)*0.0001007f);
    
    /* calculate error */
    pid.error = sp - filt_pv_f32;
        
    /* Piecewise gain schedule based on calculated error*/
    if (pid.error <= current_therapy_settings.pump.pid.e1) {
      pid.Kp = current_therapy_settings.pump.pid.kp1;
      pid.Ki = current_therapy_settings.pump.pid.ki1;
    }
    else if (pid.error <= current_therapy_settings.pump.pid.e2)
    {
      pid.Kp = current_therapy_settings.pump.pid.kp2;
      pid.Ki = current_therapy_settings.pump.pid.ki2;
    }
    else 
    {
      pid.Kp = current_therapy_settings.pump.pid.kp3;
      pid.Ki = current_therapy_settings.pump.pid.ki3;
    }
    
    if (reset == TRUE)
    {
      
      pid_history.error = 0;
      pid_history.output = 0;
      pid_history.Ui = 0;
      pid_history.Ki = pid.Ki;
      pid_history.Kp = pid.Kp;
      
    }

    /* First time through you want last cmd to always be zero */
    if ((pid_history.Kp == 0.0f) && (pid_history.Ki == 0.0f))
    {
      
      memset(&last_pump_cmd[0],0,sizeof(FMT_SPEED_CMD_SPEC));
        
    }   
    
    /* Calculate proportional term */
    pid.Up = pid.error * pid.Kp;
       
    if (pid.Ki == 0)
    {
      
      pid.output = pid.Up;
      
      if (pid.output > current_therapy_settings.pump.pid.max_rps)
      {
        
        pid.output = current_therapy_settings.pump.pid.max_rps;
        
      }
      
      else if (pid.output < current_therapy_settings.pump.pid.min_rps)
      {
        
        pid.output = current_therapy_settings.pump.pid.min_rps;
        
      }
           
    }
    
    else if ((pid.Kp == pid_history.Kp) && (pid.Ki == pid_history.Ki))
    {
           
      if (reset == TRUE)
      {
        
        pid.Ui = 0;
        
      }
      
      else
      {
        
        pid.Ui = pid_history.Ui + (pid.Ki * ((pid.error + pid_history.error) / 2));
        
      }
      
      pid.output = pid.Ui + pid.Up;
      
      if (pid.output > current_therapy_settings.pump.pid.max_rps) 
      {
        
        pid.output = current_therapy_settings.pump.pid.max_rps;
        pid.Ui = current_therapy_settings.pump.pid.max_rps - pid.Up;
        
      }
      
      else if (pid.output < current_therapy_settings.pump.pid.min_rps)
      {
        
        pid.output = current_therapy_settings.pump.pid.min_rps;
        pid.Ui = current_therapy_settings.pump.pid.min_rps - pid.Up;
        
      }
                  
    }
    
    /* bumpless transfer */
    else
    {
      
      pid.output = pid_history.output;
      pid.Ui = pid.output - pid.Up;
      
    }
    

    pid_history = pid;
    
    s = __get_interrupt_state();
    __disable_interrupt();
    
    memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
    sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",pid.output);
    
    
    /* Prepare the current PID output for transmission to pump drive */
    //PreparePumpSendCMD(&pump_cmd[0],PIDdata.state[2]);
      
     
    /* Replace 6th character w/ terminator since pump drive only processes two
     * significant digits (CSnnn.nn). */
    //*(&pump_cmd[0]+strlen(FMT_SPEED_CMD_SPEC)) = '\0';
    
    /* Scan cmd for identical characters */
//    if (strncmp(&pump.cmd[pump.idx].data[0],&last_pump_cmd[0],sizeof(FMT_SPEED_CMD_SPEC)) == 0)
//    {
//      
//      f_identical_cmds = TRUE;
//      
//    }
//    
//    memcpy(&last_pump_cmd[0],&pump.cmd[pump.idx].data,sizeof(FMT_SPEED_CMD_SPEC));
           
    /* Skip sending cmd if any characters are not identical */ 
//    if ((f_identical_cmds == FALSE) && (pump.cmd[pump.idx].data != '\0'))
//    {
      
      /* Transmit speed command to pump drive */
      //PumpSendCMD(&pump_cmd[0],sizeof(FMT_SPEED_CMD_SPEC));
      
      pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
  
      #ifdef DEBUG_CAN
      
      CANdebug.pump.rps[0] = pump.cmd[pump.idx].data[2];
      CANdebug.pump.rps[1] = pump.cmd[pump.idx].data[3];
      CANdebug.pump.rps[2] = pump.cmd[pump.idx].data[5];
      CANdebug.pump.rps[3] = pump.cmd[pump.idx].data[6];
      CANdebug.pump.pv = pv;
      CANdebug.pump.filt_pv = filt_pv_f32;
      CANdebug.pump.error = pid.error;
      CANdebug.pump.pid_output = pid.output;
                
      #endif

      pump.idx++;
      UART4_C2 |= UART_C2_TCIE_MASK;

//}

  //} 
      
    __set_interrupt_state(s);    
  
  /* Return current PID output state */
  return pid.output;

}


///******************************************************************************
//* Function: PumpPID_Init
//*
//* Purpose: Initilaize PID controller.
//******************************************************************************/
//
//void PumpPID_Init(void) {
//
//  /* Proportional gain */
//  pid.Kp = 0;
//
//  /* Intigral gain */
//  pid.Ki = 0;
//
//  /* Derivative gain */
//  pid.Kd = 0;
//
//  /* Initialize PID data structures */
//  //arm_pid_init_q15(&PIDdata,1);
//  arm_pid_init_f32(&pid,(int32_t) 1);
//  
//}

/***************************************************************************//**
Initilaize PID controller.
@param the initial state of the filter
@return 0
@outputs
@context
*******************************************************************************/
void PumpFIR_Init(q15_t init_value) {

  uint16_t numTaps = 8;
  /* state size is numTaps + blockSize */
  static q15_t pump_fir_state[9];
  uint32_t blockSize = 1;
  arm_status status;
    
  status = arm_fir_init_q15 (&fir,
                            numTaps, 
                            &smart_wand_params.filter.coeffs[0], 
                            &pump_fir_state[0], 
                            blockSize);
  
  if (status == ARM_MATH_SUCCESS)
  {
    
    //memset(&state[0],init_value,sizeof(state));
    pump_fir_state[0] = init_value;
    pump_fir_state[1] = init_value;
    pump_fir_state[2] = init_value;
    pump_fir_state[3] = init_value;
    pump_fir_state[4] = init_value;
    pump_fir_state[5] = init_value;
    pump_fir_state[6] = init_value;
    pump_fir_state[7] = init_value;
    
  }
    
}

/***************************************************************************//**
Configure Universal Asynchronous Receiver/Transmitter Port 4 (UART4)
which is used for RS-232 serial port to communicate for the ST5-S step motor
control drive controller.
@param The baud rate at which the pump operates is the input
@return 0
@outputs
@context
* NOTE: UART4 is clocked off bus clock (74MHz).  Baud rate calculated using
* the following equation: 38,401.6   = 74 MHz / [16 * (120 + 0.4375)]
*                          9,599.792 = 74 MHz / [16 * (481 + 0.78125)]

*******************************************************************************/
void PumpUART4_Init(BAUD_RATE bps) {
  
  uint8_t temp;
  
  /* Calculated set baud rate register value */
  uint16_t sbr;
  
  uint8_t brfa;
  
  NVIC_DisableIRQ(UART4_RX_TX_IRQn);
  
  PORTC_PCR9 = PORT_PCR_MUX(1);
  
  /* ctrl for shutdown pin on p/n ADM101E rs-232 transceiver */
  GPIOC_PDDR |= GPIO_PDDR_PDD(PUMP_DSBL);
  
  /* disable level-shift chip during uart config */
  GPIOC_PSOR = GPIO_PDOR_PDO(PUMP_DSBL);
    
  /* keep UART4 output pins high impedance during config */
  PORTC_PCR14 = PORT_PCR_MUX(1); // GPIO
  PORTC_PCR15 = PORT_PCR_MUX(1); // GPIO
  
  /* Enable gate clock */
  SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
  
  /* Disable transmitter and receiver while settings are modified. */
  UART4_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
  
  /* Configure UART default 8-bit mode, no parity */
  UART4_C1 = 0;
    
  /* Target baud rate */
  if (bps == BAUD_9600)
  {
    
    //sbr = (uint16_t) ((BUS_CLK_MHZ * 1e6) / (bps * 16));
    sbr = 481;
    
    brfa = 25;
    
  }
  
  else if (bps == BAUD_38400)
  {
  
    sbr = 120;
    
    brfa = 14;
    
  }
    
  /* Store current value of the UARTx_BDH except for the SBR field */
  temp = UART_BDH_REG(UART4_BASE_PTR) & ~(UART_BDH_SBR(0x1F));
  UART4_BDH = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
  UART4_BDL = (uint8_t) (sbr & UART_BDL_SBR_MASK);
  
  /* Store current value of the UARTx_C4 register except for the BRFA field */
  temp = UART_C4_REG(UART4_BASE_PTR) & ~(UART_C4_BRFA(0x1F));

  /* Set baud rate fine adjustment to 14/32 = 0.4375 */
  UART4_C4 = temp | UART_C4_BRFA(brfa);
  
  /* Enable transmit buffer */
  UART4_PFIFO |= UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK;
  
  /* Flush transmit buffer */
  UART4_CFIFO |= UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK;
    
  /* Enable transmit and receive DMA requests */
  //UART4_C5 |= UART_C5_RDMAS_MASK;// | UART_C5_TDMAS_MASK; 
  
  /* Flush transmit and receive buffers */
  UART4_CFIFO |= UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK;
  
  /* Enable receiver and transmitter and RDRF interrupt/DMA transfers */
  UART4_C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;// | UART_C2_RIE_MASK;// | UART_C2_TIE_MASK;
  
  /* re-enable level-shift chip */
  GPIOC_PCOR = GPIO_PDOR_PDO(PUMP_DSBL);
  
  /* Configure UART4 output pins */
  PORTC_PCR14 = PORT_PCR_MUX(3); // UART4_RX
  PORTC_PCR15 = PORT_PCR_MUX(3); // UART4_TX
  
  NVIC_EnableIRQ(UART4_RX_TX_IRQn);
  
}

/***************************************************************************//**
Set flag after 3 minutes of no therapy. 
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_inactivity_monitor(void)
{
      
//  if ((current_therapy_settings.treatment.ablate_on == 1) ||
//      (current_therapy_settings.treatment.coag_on == 1))
//  {
//    
//    current_therapy_settings.pump.inactivity_seconds = 0;
//    
//  }
//  else
//  {
//  
//    current_therapy_settings.pump.inactivity_seconds++;
//                
//  }
//  
//  if ((current_therapy_settings.pump.inactivity_seconds % 180 == 0) &&
//  (current_therapy_settings.pump.inactivity_seconds != 0))
//  {
//
//    //Pump_Config(STOP);
//    //Pump_Config(DISABLE);
//    current_therapy_settings.pump.on = 0;
//    
//  }
  
  return 0;
    
}

/***************************************************************************//**
Controls the speed of the pump depending on the state of the RF state machine
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_speed_ctrl(void)
{
  
  volatile float32_t pid_output = 0;
  uint8_t coblation_type;
  static uint32_t counter;
  
  const SPEED_CTRL_STATES State_Table[2][5][2][NUM_SPEED_CTRL_STATES] = 
    /* ctrl off,   coag on */  {      OFF,  STOPPING, STOPPING, STOPPING,     STOPPING,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING,    STOPPING,
    /* ctrl on,    coag on */   QUIESCENT, COAG_MODE, STOPPING, STOPPING,    QUIESCENT,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING,COAG_MODE, STOPPING, STOPPING, STOPPING, STOPPING,    STOPPING,
    /* ctrl off, ablate on */         OFF,  STOPPING, STOPPING, STOPPING,     STOPPING,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING,    STOPPING,
    /* ctrl on,  ablate on */   QUIESCENT,  STARTING,       ON,       ON,    QUIESCENT,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING,    STOPPING,
    /* ctrl off,    vac on */         OFF,  STOPPING, STOPPING, STOPPING,     STOPPING,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING,    STOPPING,
    /* ctrl on,     vac on */   QUIESCENT, VAC_START, STOPPING, STOPPING,    VAC_START, PULSING_DELAY, PULSING_DELAY, PEAK_DELAY, PEAK_DELAY, VAC_CTRL, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING,    STOPPING,
    /* ctrl off,    ent on */         OFF,  STOPPING, STOPPING,  STOPPING,    STOPPING,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING,    STOPPING,
    /* ctrl on,     ent on */   PRE_PRIME,  STOPPING, STOPPING,  STOPPING,ENT_PAUSE_RF,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING, STOPPING, STOPPING,   ON_ENT,   ON_ENT,   ON_ENT,      ON_ENT,
    /* ctrl off, 8p_ent on */         OFF,  STOPPING, STOPPING,  STOPPING,    STOPPING,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING,    STOPPING,
    /* ctrl on,  8p_ent on */ ON_ENT_8PIN,  STOPPING, STOPPING,  STOPPING,    STOPPING,      STOPPING,      STOPPING,   STOPPING,   STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, STOPPING, ON_ENT_8PIN,
    
    /* ctrl off,   coag on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,
    /* ctrl on,    coag on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,
    /* ctrl off, ablate on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,
    /* ctrl on,  ablate on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,
    /* ctrl off,    vac on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,
    /* ctrl on,     vac on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,
    /* ctrl off,    ent on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,
    /* ctrl on,     ent on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,
    /* ctrl off, 8p_ent on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,       
    /* ctrl on,  8p_ent on */ OFF, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING, CYCLING,CYCLING,CYCLING,};
  
  if ((current_therapy_settings.controller == ENT) && (current_therapy_settings.active_wand.smart_wand == ENT))
  {
    coblation_type = 3;
  }
  else if ((current_therapy_settings.controller == SPORTS_MED) && (current_therapy_settings.active_wand.smart_wand == ARTHROPLASTY))
  {
    coblation_type = 3;
  }
  else if ((current_therapy_settings.controller == ENT) && (current_therapy_settings.active_wand.legacy_wand == ENT))
  {
    coblation_type = 4;
  }
  else if (current_therapy_settings.treatment.therapy_mode == VAC)
  {
    coblation_type = 2;
  }
  else if (current_therapy_settings.treatment.ablate_on == 1)
  {
    coblation_type = 1;
  }
  else
  {
    coblation_type = 0;
  }
  
  spd_ctrl_state = State_Table[current_therapy_settings.wand.tube_warn][coblation_type][current_therapy_settings.pump.ctrl_en][spd_ctrl_state];
  
  switch (spd_ctrl_state)
  { 
  case (OFF):
    {  
      /* if QUIESCENT did not complete count != 0 so set idle spd */
      if (counter != 0)
      {
        
        pump.state = IDLE_SPEED;
        
        float32_t rps = current_therapy_settings.pump.idle_rps;
        
        __istate_t s = __get_interrupt_state();
        __disable_interrupt();
        memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
        
        sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
        
        /* Change pump speed to idle */
        pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK;
        __set_interrupt_state(s);
        
      }      
      counter = 0;
      break;
    }  
  case (QUIESCENT):
    {   
      
      uint8_t size = strlen(pcmd[DRIVE_STATUS]);
      pump.state = CONTROL;
      /* Initiate pump controller "quiescent" period */
      if (counter++ < current_therapy_settings.pump.quiescent_delay)
      {
        
        spd_ctrl_state = OFF;
        //Pump_Config(PAUSE);
        pump.state = ZERO_SPEED;
        uint8_t size = strlen(pcmd[ZERO_SPEED]);
        
        __istate_t s = __get_interrupt_state();
        __disable_interrupt();
        
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[ZERO_SPEED],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
        __set_interrupt_state(s);
        
      }
      else //if (current_therapy_settings.treatment.therapy_mode == LO)
      {
        
        //put_event(&evt_tbl,RF_ON);
        put_event(&evt_tbl, MEASURE_IMPEDANCE);
        counter = 0;
        /* request status b/c if stuck in overcurrent loop pump times out */
        __istate_t s = __get_interrupt_state();
        __disable_interrupt();
        memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[DRIVE_STATUS],size);         
        pump.cmd[pump.idx].size = size; 
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK;
        __set_interrupt_state(s);
        /* turn rf on here */
        //RF_ON(pSmartWandFTM);
        
      }
      
      break;
    }  
  case (STARTING):
    {
      /* compute average */
      //arm_mean_q15(&current_therapy_settings.pump.irms[0], 20, (q15_t *)&mean);
      
      /* flush */
      //memset(&current_therapy_settings.pump.irms[0], 0, 40);
      
      /* Reinitialize pump controller */
      //PumpPID_Init();
      
      /* Reinitialize filter controller */
      PumpFIR_Init(current_therapy_settings.pump.mean_irms);
      
      if (pump.state == CONTROL)
      {
        /* speed control algorithm */
        pid_output = PumpPID(current_therapy_settings.pump.mean_irms,TRUE); 
      }
      
      break;
    }  
  case (ON):
    {  
      
      if (pump.state == CONTROL)
      {
        /* speed control algorithm */
        pid_output = PumpPID(current_therapy_settings.pump.mean_irms,FALSE);
      }
      
      break;
    }
  case (STOPPING):
    {  
      PumpFIR_Init(0);
      
      //Pump_Config(PUMP_IDLE);
      pump.state = IDLE_SPEED;
      
      float32_t rps = current_therapy_settings.pump.idle_rps;
      //float32_t rps = 0.83f;
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
      
      sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
      
      /* Change pump speed to idle */
      pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
      pump.idx++;
      UART4_C2 |= UART_C2_TCIE_MASK;
      __set_interrupt_state(s);
      
      
      spd_ctrl_state = OFF;
      counter = 0;
      
      //arm_pid_init_f32(&pid,(int32_t) 1);
      
      break;
    }  
  case (VAC_START):
    {  
      /* Change pump speed to idle */
      //Pump_Config(VAC_PULSE);
      pump.state = VAC_PULSE_SPEED;
      
      current_therapy_settings.setpoint.level = smart_wand_params.vac.pulsing_voltage;
      
      //put_event(&evt_tbl, MEASURE_IMPEDANCE);
      
      //      if (current_therapy_settings.pump.pulse_flag == 1)
      //      {
      //        current_therapy_settings.pump.pulse_flag = 0;
      //      }
      
      counter = 0;
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
      
      float32_t rps = current_therapy_settings.pump.pulse_rps;
      
      memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
      
      sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
      
      /* Change pump speed to idle */
      pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
      pump.idx++;
      UART4_C2 |= UART_C2_TCIE_MASK;
      
      __set_interrupt_state(s);
      
      //state = PULSING_DELAY;
      
      break;
    }  
  case (PULSING_DELAY):
    {
      counter++;
      
      //      if (current_therapy_settings.pump.pulse_flag == 1)
      //      {
      //        
      //        spd_ctrl_state = OFF;
      //        break;
      //        
      //      }
      
      /* Set timeout value */
      if (counter != current_therapy_settings.pump.vac.pulse_delay)
      {
        
        break;
        
      }
      else
      {
        
        /* change state & fall through to PEAK_START */
        spd_ctrl_state = PEAK_START;
        
        
      }
    }        
  case (PEAK_START):
    {
      current_therapy_settings.setpoint.level = current_therapy_settings.pump.vac.peak_voltage;     
      //SET_DAC(pSmartWandDAC,current_therapy_settings.pump.vac.peak_voltage);
      
      /* compute average */
      //arm_mean_q15(&current_therapy_settings.pump.irms[0], 20, &mean);
      
      /* flush */
      //memset(&current_therapy_settings.pump.irms[0], 0, 40);
      
      /* Reinitialize pump controller */
      //PumpPID_Init();
      
      /* Reinitialize filter controller */
      PumpFIR_Init(current_therapy_settings.pump.mean_irms);
      
      if (pump.state == CONTROL)
      {
        /* speed control algorithm */
        pid_output = PumpPID(current_therapy_settings.pump.mean_irms,TRUE); 
      }
      
      
      counter = 0;
      
      
      break; 
    }  
  case (PEAK_DELAY):
    {
      counter++;
      
      //      if (current_therapy_settings.pump.pulse_flag == 1)
      //      {
      //        
      //        spd_ctrl_state = OFF;
      //        break;
      //        
      //      }
      
      /* compute average */
      //arm_mean_q15(&current_therapy_settings.pump.irms[0], 20, &mean);
      
      /* flush */
      //memset(&current_therapy_settings.pump.irms[0], 0, 40);
      
      if (pump.state == CONTROL)
      {
        /* speed control algorithm */
        pid_output = PumpPID(current_therapy_settings.pump.mean_irms,FALSE); 
      }
      
      /* Set timeout value */
      if (counter != current_therapy_settings.pump.vac.peak_time)
      {
        
        break;
        
      }
      
      else
      {
        current_therapy_settings.setpoint.level = current_therapy_settings.pump.vac.voltage_sp_default;
        //SET_DAC(pSmartWandDAC,current_therapy_settings.pump.vac.voltage_sp_default);
        spd_ctrl_state = VAC_CTRL;
        counter = 0;
        
        break;
        
      }
    }       
  case (VAC_CTRL):
    {
      counter++;
      
      //      if (current_therapy_settings.pump.pulse_flag == 1)
      //      {
      //        
      //        spd_ctrl_state = OFF;
      //        break;
      //        
      //      }
      
      /* compute average */
      //arm_mean_q15(&current_therapy_settings.pump.irms[0], 20, &mean);
      
      /* flush */
      //memset(&current_therapy_settings.pump.irms[0], 0, 40);
      
      if (pump.state == CONTROL)
      {
        /* speed control algorithm */
        pid_output = PumpPID(current_therapy_settings.pump.mean_irms,FALSE); 
      }
      
      /* Set timeout value */
      if (counter >= smart_wand_params.vac.max_time)
      {
        
        //RF_OFF(pSmartWandFTM);
        //SET_DAC(pSmartWandDAC,0);
        //current_therapy_settings.pump.ctrl_en = FALSE;
        
        spd_ctrl_state = QUIESCENT;
        current_therapy_settings.setpoint.level = smart_wand_params.vac.pulsing_voltage;
        
      }
      
      break;  
      
    }
  case (COAG_MODE):
    {  
      PumpFIR_Init(0);
      
      //Pump_Config(PUMP_IDLE);
      pump.state = IDLE_SPEED;
      
      float32_t rps = current_therapy_settings.pump.idle_rps;
      //float32_t rps = 0.83f;
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
      
      sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
      
      /* Change pump speed to idle */
      pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
      pump.idx++;
      UART4_C2 |= UART_C2_TCIE_MASK;
      __set_interrupt_state(s);
      
      break;
    }
  case (CYCLING):
    {  
      PumpFIR_Init(0);
      
      put_event(&evt_tbl,RF_OFF);
      
      //Pump_Config(PUMP_IDLE);
      pump.state = IDLE_SPEED;
      
      float32_t rps = current_therapy_settings.pump.idle_rps;
      //float32_t rps = 0.83f;
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
      
      sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
      
      /* Change pump speed to idle */
      pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
      pump.idx++;
      UART4_C2 |= UART_C2_TCIE_MASK;
      __set_interrupt_state(s);
      spd_ctrl_state = OFF;
      counter = 0;
      //arm_pid_init_f32(&pid,(int32_t) 1);
      break;
    }
  case (PRE_PRIME):
    {
      pump.state = CONTROL;
      if (counter < current_therapy_settings.pump.pulse_delay)
      {        
        if (get_priming_status() == PRIME_AUTO)
        {
          if (*(uint32_t*)&smart_wand_params.manta.prime.autom.final_spd < *(uint32_t*)&current_therapy_settings.pump.pulse_rps)
          {
            float spd_diff = (current_therapy_settings.pump.pulse_rps - smart_wand_params.manta.prime.autom.final_spd);
            if (spd_diff > smart_wand_params.manta.prime.autom.adj)
            {
              current_therapy_settings.pump.pulse_rps -= smart_wand_params.manta.prime.autom.adj;
            }
            else
            {
              current_therapy_settings.pump.pulse_rps = smart_wand_params.manta.prime.autom.final_spd;
            }
          }
          else if (*(uint32_t*)&smart_wand_params.manta.prime.autom.final_spd > *(uint32_t*)&current_therapy_settings.pump.pulse_rps)
          {
            float spd_diff = (smart_wand_params.manta.prime.autom.final_spd - current_therapy_settings.pump.pulse_rps);
            if (spd_diff > smart_wand_params.manta.prime.autom.adj)
            {
              current_therapy_settings.pump.pulse_rps += smart_wand_params.manta.prime.autom.adj;
            }
            else
            {
              current_therapy_settings.pump.pulse_rps = smart_wand_params.manta.prime.autom.final_spd;
            }
          }
        }            
        float32_t rps = current_therapy_settings.pump.pulse_rps;
        __istate_t s = __get_interrupt_state();
        __disable_interrupt();
        memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
        sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
        pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
        __set_interrupt_state(s);
      }
      else
      {
        float32_t rps = current_therapy_settings.pump.ent_flow;
        __istate_t s = __get_interrupt_state();
        __disable_interrupt();
        memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
        sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
        pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
        __set_interrupt_state(s);
      }
      if (counter == current_therapy_settings.pump.quiescent_delay)
      {
        put_event(&evt_tbl, MEASURE_IMPEDANCE);        
      }
      if ((counter < current_therapy_settings.pump.quiescent_delay) ||
          (counter < current_therapy_settings.pump.pulse_delay))
      {
        counter++;
        spd_ctrl_state = OFF;
      }
      break;  
    }
  case (ON_ENT):
    {      
      uint8_t size = strlen(pcmd[DRIVE_STATUS]);
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
      memcpy(&pump.cmd[pump.idx].data[0],pcmd[DRIVE_STATUS],size);         
      pump.cmd[pump.idx].size = size; 
      pump.idx++;
      UART4_C2 |= UART_C2_TCIE_MASK;
      __set_interrupt_state(s); 
      break;
    }
  case (ON_ENT_8PIN):
    {       
      float32_t rps = current_therapy_settings.pump.idle_rps;
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
      sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
      pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
      pump.idx++;
      UART4_C2 |= UART_C2_TCIE_MASK; 
      __set_interrupt_state(s);
      break;
    }
  case (ENT_PAUSE_RF):
    {
      static uint8_t rf_pause_cnt = 0;
      
      if (rf_pause_cnt++>0)
      {
        put_event(&evt_tbl, MEASURE_IMPEDANCE);
        rf_pause_cnt = 0;
      }
      else
      {
        spd_ctrl_state = STOPPING;
      }
      break;
    }
  } 
  return 0; 
}

/***************************************************************************//**
Sends pump commands the pump uart
@param void
@return 0
@outputs
@context
*******************************************************************************/
void send_pump_cmd(void)
{
  
  static uint8_t byte = 0;
  
  /* Transmit packet */
  if ((UART4_S1 & UART_S1_TC_MASK) == UART_S1_TC_MASK)
  {
    
    //if ((pump.idx == 0) && ((UART4_S1 & UART_S1_TC_MASK) == UART_S1_TC_MASK))
    if ((pump.idx == 0))// || (pump.cmd[0].size == 0))
    {
      
      UART4_C2 &= ~UART_C2_TCIE_MASK;
      
    }      
    
    else if (byte < 9)
    {
    
      UART4_D = (uint8_t) pump.cmd[0].data[byte];
      byte++;
    
    }
              
    if (byte >= pump.cmd[0].size)
    {
               
      memcpy(&pump.cmd[0],&pump.cmd[1],8*pump.idx);
      
      pump.idx--;
      
      byte = 0;
    
    }
   
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
uint8_t receive_pump_cmd(void)
{
  
  static uint8_t pump_packet[MAX_NUM_CHARS];
  static uint8_t c = 0;
  //uint8_t power_packet[1] = {0xFF};
#ifndef TEST     
  /* Wait until character has been received */
  while (((UART4_S1 & UART_S1_RDRF_MASK)==UART_S1_RDRF_MASK) &&
         (c < MAX_NUM_CHARS))
  {
#endif
   pump_packet[c++] = UART4_D;

   //memcmp(&packet[0],*pcmd[0],strlen(pcmd[0]));
  
   /* nack */
   if (memchr (&pump_packet[0], '?', MAX_NUM_CHARS) != NULL)
    {
      
      /* remove for production--countermeasure for motor firmware < ver 105N */
      if (pump.state == MAX_CURRENT)
      {
        
        put_event(&evt_tbl,PUMP_CMD_ACK);
        
      }
      else
      {
        
        put_event(&evt_tbl,PUMP_CMD_NACK);
        
      }
      memset(pump_packet,0,MAX_NUM_CHARS);
      c = 0;
      
    } 
   /* acknowledge exception */
    else if (memchr (&pump_packet[0], '*', MAX_NUM_CHARS) != NULL)
    {
      put_event(&evt_tbl,PUMP_CMD_EACK);
      memset(pump_packet,0,MAX_NUM_CHARS);
    }
    else if (memchr (&pump_packet[0], '\r', MAX_NUM_CHARS) != NULL)
    {  
      
      if (strstr((char*)&pump_packet[0],"RS=") != NULL)
      {
      
        memset(&pump.status_data[0],0,20);
        memcpy(&pump.status_data[0],&pump_packet[0],20);
        put_event(&evt_tbl,DRIVE_STATUS_RX);
        
      }
      else if (strstr((char*)&pump_packet[0],"PM=") != NULL)
      {
        
        memset(&pump.status_data[0],0,20);
        memcpy(&pump.status_data[0],&pump_packet[0],20);
        put_event(&evt_tbl,DRIVE_POWER_MODE_RX);
        
      }
      else if (memchr (&pump_packet[0], '%', MAX_NUM_CHARS) != NULL)
      {

        put_event(&evt_tbl,PUMP_CMD_ACK);


      }

      memset(&pump_packet[0],0,MAX_NUM_CHARS);
      c = 0;

    }
        
    /* power up packet */
    else if (memchr (&pump_packet[0], 0xFF, MAX_NUM_CHARS) != NULL)
    {
//      /* only way to get here is @ 9,600 buad */
//      put_event(&evt_tbl,PUMP_RESET);
      SYST_CSR |= SysTick_CSR_ENABLE_MASK;
      memset(pump_packet,0,MAX_NUM_CHARS);
      c = 0;
      
    }
    
#ifndef TEST    
  }
#endif  
  
  if (c >= MAX_NUM_CHARS)
  {
    
    memset(pump_packet,0,MAX_NUM_CHARS);
    c = 0;
    
  }
    
  return 0;
  

}

/***************************************************************************//**
Resets pump
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_reset(void)
{

  uint8_t size = 2;
  
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();

  memset(&pump.state,0,sizeof(PUMP_CMD));
  memcpy(&pump.cmd[pump.idx].data[0],pcmd[SCL_MODE],size);      
  pump.cmd[pump.idx].size = size;
  pump.idx++;
  UART4_C2 |= UART_C2_TCIE_MASK; 
  __set_interrupt_state(s);

  pump.state = PROTOCOL;
  
  RESET_PIT(0);
  
  return 0;

}

/***************************************************************************//**
pump command not acknowledged
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_cmd_nack(void)
{

//pump.state = SOFT_RESET;
return 0;

}

/***************************************************************************//**
pump command exception acknowledged
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_cmd_eack(void)
{
  
  if (pump.state == COMMENCE_JOG)
  {
    
    pump.state = DRIVE_STATUS;
    
  }
  else if (pump.state == MOTOR_ENABLE)
  {
    
    pump.state=COMMENCE_JOG;
    
  }
pump.attempts = 0;
pump.inactive_count = 0;
//RESET_PIT(1);
//put_event(&evt_tbl,PUMP_TX);
return 0;

}

/***************************************************************************//**
pump command acknowledged
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_cmd_ack(void)
{

  RESET_PIT(0);
  
  if ((pump.state == COMMENCE_JOG) ||
       (pump.state == IDLE_SPEED) ||
       (pump.state == STOP_KILL))
  {
    
    pump.state = DRIVE_STATUS;
    
  }
  else if (pump.state == PROBE_MOTOR) 
  {
    
    SYST_CSR |= SysTick_CSR_ENABLE_MASK;
    pump.state = WAIT_PROBE_TEST_COMPLETE;
    
  }
  else if ((pump.state == ZERO_SPEED) ||
          (pump.state == VAC_PULSE_SPEED)||
          (pump.state == CONTROL))// ||
          //(pump.state == COMMENCE_JOG))
  {
    
    pump.state = CONTROL;
    
  }
//  else if (pump.state == SOFT_RESET)
//  {
//    
//    pump.state = CHANGE_BAUD;
//    
//  }
  else if (pump.state == PWR_MODE_SCL)
  {
    
    pump.state = REQUEST_PWR_MODE;
    
  }
  else if (pump.state < DRIVE_STATUS)
  {
    
    pump.state++;
    
  }
  else if (pump.state == MOTOR_ENABLE)
  {
    
    pump.state=COMMENCE_JOG;
    
  }
pump.attempts = 0;
pump.inactive_count = 0;
//RESET_PIT(1);
//put_event(&evt_tbl,PUMP_TX);
return 0;

}

/***************************************************************************//**
Pump probe test complete
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t probe_test_complete(void)
{
  
  pump.state = DELAY_TIME;
  
  return 0;
  
}

/***************************************************************************//**
Processes the received status response by switching flags and pump state
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t drive_status(void)
{
  
  uint8_t *ptok;
  //A = An Alarm code is present (use AL command to see code, AR command to clear code)
  //D = Disabled (the drive is disabled)
  //E = Drive Fault (drive must be reset by AR command to clear this fault)
  //F = Motor moving
  //H = Homing (SH in progress)
  //J = Jogging (CJ in progress)
  //M = Motion in progress (Feed & Jog Commands)
  //P = In position
  //R = Ready (Drive is enabled and ready)
  //S = Stopping a motion (ST or SK command executing)
  //T = Wait Time (WT command executing)
  //W = Wait Input (WI command executing)

  pump.attempts = 0;
  
  RESET_PIT(0);
  
  /* tokenize to cleave 'RS=' */
  ptok = memchr(&pump.status_data[0], '=', MAX_NUM_CHARS);
  
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();  
    
  if (memchr (ptok, 'A', &pump.status_data[MAX_NUM_CHARS] - ptok) != NULL)
  {
    
    current_therapy_settings.pump.status |= PUMP_ALARM;
    
  }
  else
  {
    
    if ((current_therapy_settings.pump.status & PUMP_ALARM) == PUMP_ALARM)
    {
      
      pump.state = DRIVE_STATUS;
      
    }
    
    current_therapy_settings.pump.status &= ~PUMP_ALARM;
    
  }
  
  if (memchr (ptok, 'D', &pump.status_data[MAX_NUM_CHARS] - ptok) != NULL)
  {
    
    current_therapy_settings.pump.status |= PUMP_DISABLED;
    current_therapy_settings.pump.on = 0;
    
  }
  else
  {
    
    current_therapy_settings.pump.status &= ~PUMP_DISABLED;
    
  }
  
  if (memchr (ptok, 'E', &pump.status_data[MAX_NUM_CHARS] - ptok) != NULL)
  {
    
    current_therapy_settings.pump.status |= PUMP_FAULT;
    
  }
  else
  {
    
    if ((current_therapy_settings.pump.status & PUMP_FAULT) == PUMP_FAULT)
    {
      
      pump.state = DRIVE_STATUS;
      
    }
       
    current_therapy_settings.pump.status &= ~PUMP_FAULT;
    
  }
  
  /* for either 'A' or 'E' status bytes send alarm reset command */
  if ((current_therapy_settings.pump.status & PUMP_ALARM_FAULT) > 0) 
  {
    
    uint8_t size = strlen(pcmd[ALARM_RESET]);
    
    memcpy(&pump.cmd[pump.idx].data[0],pcmd[ALARM_RESET],size);      
    pump.cmd[pump.idx].size = size;
    pump.idx++;
    
  }
  
  if (memchr (ptok, 'F', &pump.status_data[MAX_NUM_CHARS] - ptok) != NULL)
  {
    
    current_therapy_settings.pump.status |= PUMP_MOVING;
    
  }
  else
  {
    
    current_therapy_settings.pump.status &= ~PUMP_MOVING;
    
  }
  
  if (memchr (ptok, 'J', &pump.status_data[MAX_NUM_CHARS] - ptok) != NULL)
  {
    
    current_therapy_settings.pump.status |= PUMP_JOGGING;
    if ((current_therapy_settings.treatment.ablate_on == 0) &&
        (current_therapy_settings.treatment.coag_on == 0))
    {
      pump.inactive_count++;
      
      /* stop pump after 3 min of inactivity */
      if (pump.inactive_count >= 1800)
      {
        
        /* cap the count */
        pump.inactive_count = 0;
        
        uint8_t size = strlen(pcmd[STOP_KILL]);

        pump.state = STOP_KILL;

        // __istate_t s = __get_interrupt_state();
        // __disable_interrupt();
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[STOP_KILL],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK;
        
        put_event(&evt_tbl,BUCK_INACTIVE_SHUTDOWN);
        
        if (get_priming_status() == PRIME_AUTO)
        {
          put_event(&evt_tbl,THERAPY_OFF);
        }
      
      }
    }
      
    
  }
  else
  {
    
    current_therapy_settings.pump.status &= ~PUMP_JOGGING;
    
  }
  
  if (memchr (ptok, 'M', &pump.status_data[MAX_NUM_CHARS] - ptok) != NULL)
  {
    
    current_therapy_settings.pump.status |= PUMP_MOTION;
    
  }
  else
  {
    
    current_therapy_settings.pump.status &= ~PUMP_MOTION;
    
  }
       
  if (memchr (ptok, 'R', &pump.status_data[MAX_NUM_CHARS] - ptok) != NULL)
  {
    
    current_therapy_settings.pump.status |= PUMP_READY;
    
  }
  else
  {
    
    current_therapy_settings.pump.status &= ~PUMP_READY;
    
  }
   
  if ((current_therapy_settings.notify.status & PUMP_FAULT_MASK) == PUMP_FAULT_MASK)
  {
    
    uint8_t size = strlen(pcmd[STOP_KILL]);
    
    pump.state = STOP_KILL;
    
    memcpy(&pump.cmd[pump.idx].data[0],pcmd[STOP_KILL],size);      
    pump.cmd[pump.idx].size = size;
    pump.idx++;
    UART4_C2 |= UART_C2_TCIE_MASK;

  }
  else if ((current_therapy_settings.pump.status & 0x7F) == PUMP_READY)
  {
    
    uint8_t size = strlen(pcmd[MOTOR_DISABLE]);
      
    memcpy(&pump.cmd[pump.idx].data[0],pcmd[MOTOR_DISABLE],size);      
    pump.cmd[pump.idx].size = size;
    pump.idx++;  
    
  }
  else if ((current_therapy_settings.pump.on == 0) && 
     ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON))
  {
    
    current_therapy_settings.pump.on = 1;
    put_event(&evt_tbl,THERAPY_ON);
    
  }
  else if (((current_therapy_settings.pump.on == 1) && 
            (current_therapy_settings.pump.status & PUMP_DISABLED) != PUMP_DISABLED) &&
          (((current_therapy_settings.pump.status & PUMP_FAULT) == PUMP_FAULT) ||
           ((current_therapy_settings.pump.status & PUMP_ALARM) == PUMP_ALARM) ||
           ((current_therapy_settings.pump.sensor_status & DOOR_SENSOR_ON) != DOOR_SENSOR_ON)||
           ((current_therapy_settings.pump.status & ERROR) == ERROR))) 
            //(current_therapy_settings.pump.sensor_check == FALSE)))
 
  {
    
    uint8_t size = strlen(pcmd[STOP_KILL]);
    
    pump.state = STOP_KILL;
    
    memcpy(&pump.cmd[pump.idx].data[0],pcmd[STOP_KILL],size);      
    pump.cmd[pump.idx].size = size;
    pump.idx++;
    UART4_C2 |= UART_C2_TCIE_MASK; 
    put_event(&evt_tbl,THERAPY_OFF);
    
  }
  else if ((current_therapy_settings.pump.on == 1) &&
           ((current_therapy_settings.pump.status & PUMP_RUNNING) != PUMP_RUNNING))
  {
    
    current_therapy_settings.pump.on = 0;
    
  } 
    
  __set_interrupt_state(s);
  
  return 0;
  
}

/***************************************************************************//**
When the pump boots up, it can be in any state. This function makes sure the
pump is switched to SCL state to receive commands.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t drive_power_mode(void)
{
  
  uint8_t *ptok;
  //1 = Si Program (Si versions only)
  //2 = Q / SCL (drive enabled)
  //3 = Quick Tuner (servos) or Configurator (steppers)
  //4 = SiNet Hub
  //5 = Q / SCL (drive disabled)
  //6 = not used
  //7 = Q Program, Auto-execute (Q drives only)

  pump.attempts = 0;
  
  RESET_PIT(0);
  
  /* tokenize to cleave 'PM=' */
  ptok = memchr(&pump.status_data[0], '=', MAX_NUM_CHARS);
    
  if (memchr (ptok, '2', &pump.status_data[MAX_NUM_CHARS] - ptok) != NULL)
  {
    
    pump.state = CHANGE_BAUD;
     
  }
  else
  {
    
    pump.state = PWR_MODE_SCL;
    
  }
  
  return 0;
  
}


/***************************************************************************//**
This function assesmbles the packets to be sent out to the PUMP. This function
is called every 20msec.
@param void
@return 0
@outputs
@context Service interrupt triggered by received packet.  The context of this
function is limited to periodic packets only.
*******************************************************************************/
uint8_t pump_packetizer(void) 
{ 
  
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();
  if (pump.idx >= 14)
  {
    
    memset(&pump.state,0,sizeof(PUMP_CMD));
    //pump.state = SOFT_RESET;
    
  }
  __set_interrupt_state(s);
  
  switch (pump.state)
  {
    
    case(PROTOCOL):
    {
      
      uint8_t size = strlen(pcmd[PROTOCOL]);

      PumpUART4_Init(BAUD_9600);
      UART4_C2 |= UART_C2_RIE_MASK;
      current_therapy_settings.pump.status &= ~DRIVE_STATUS_CMD_FIELDS;        

      pump.attempts++;
      
      if ((pump.attempts % 10) == 0)
      {

        //pump.attempts = 0;
                
        __istate_t s = __get_interrupt_state();
        __disable_interrupt();

        //pump.state = REQUEST_PWR_MODE;
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[PROTOCOL],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
        __set_interrupt_state(s);
      
      }
      else if (pump.attempts == 45)
      {
        
        GPIOA_PCOR = GPIO_PDOR_PDO(MOTOR_PWR_ON_MASK);
        
      }
      else if (pump.attempts == 46)
      {
        
        GPIOA_PSOR = GPIO_PDOR_PDO(MOTOR_PWR_ON_MASK);
        pump.attempts = 0;
        
      }
            
      break;
      
    }
    case(REQUEST_PWR_MODE):
    {
      
      uint8_t size = strlen(pcmd[REQUEST_PWR_MODE]);
                    
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
    
      if (pump.attempts++ > 5)
      {
        
        memset(&pump.state,0,sizeof(PUMP_CMD));
        //pump.state = SOFT_RESET;
        pump.state = PROTOCOL;
        
      }
      else
      {

        memcpy(&pump.cmd[pump.idx].data[0],pcmd[REQUEST_PWR_MODE],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
        
      }
      
      __set_interrupt_state(s);
            
      break;
      
    }
    case(CHANGE_BAUD):
    {
      
      uint8_t size = strlen(pcmd[CHANGE_BAUD]);
            
      current_therapy_settings.pump.status &= ~DRIVE_STATUS_CMD_FIELDS;
      
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      pump.state = CMD_MODE;
      memcpy(&pump.cmd[pump.idx].data[0],pcmd[CHANGE_BAUD],size);      
      pump.cmd[pump.idx].size = size;
      pump.idx++;
      UART4_C2 |= UART_C2_TCIE_MASK; 
      __set_interrupt_state(s);
      
      pump.state = CMD_MODE;
            
      break;
    }
    case(CMD_MODE):
    {
      
      uint8_t size = strlen(pcmd[CMD_MODE]);
      
      PumpUART4_Init(BAUD_38400);
            
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 14)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
              
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[CMD_MODE],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
        
//      }
      
      __set_interrupt_state(s);
            
      break;
      
    }
    case(MOTOR_DISABLE):
    {
    
      uint8_t size = strlen(pcmd[MOTOR_DISABLE]);
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
              
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[MOTOR_DISABLE],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK;       
             
//      }

      __set_interrupt_state(s);      
            
      break;
      
    }
    case(MAX_CURRENT):
    {
    
      uint8_t size = strlen(pcmd[MAX_CURRENT]);
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++>5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
              
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[MAX_CURRENT],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
              
//      } 
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(CONT_CURRENT):
    {
    
      uint8_t size = strlen(pcmd[CONT_CURRENT]);
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
        
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[CONT_CURRENT],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
      
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(AR_FILT_FREQ):
    {

      uint8_t size = strlen(pcmd[AR_FILT_FREQ]);
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
        
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[AR_FILT_FREQ],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
      
//      }
      
      __set_interrupt_state(s);
      
      break;
      
      
    }
    case(AR_FILT_GAIN):
    {
      
      uint8_t size = strlen(pcmd[AR_FILT_GAIN]);
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
        
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
        
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[AR_FILT_GAIN],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
              
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(PROBE_MOTOR):
    {
      
      uint8_t size = strlen(pcmd[PROBE_MOTOR]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();      

//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
        
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[PROBE_MOTOR],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
              
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(DELAY_TIME):
    {
    
      uint8_t size = strlen(pcmd[DELAY_TIME]);
      
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
                   
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[DELAY_TIME],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
              
      //} 
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(IDLE_CURRENT):
    {
    
      uint8_t size = strlen(pcmd[IDLE_CURRENT]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
        
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[IDLE_CURRENT],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
      
//      }      
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(HARMONIC_FILT_PHASE):
    {
    
      uint8_t size = strlen(pcmd[HARMONIC_FILT_PHASE]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();

//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
      
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[HARMONIC_FILT_PHASE],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
     
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(HARMONIC_FILT_GAIN):
    {
    
      uint8_t size = strlen(pcmd[HARMONIC_FILT_GAIN]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//                
//      }
//      else
//      {
        
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[HARMONIC_FILT_GAIN],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
                
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(JOG_ACCEL):
    {
    
      uint8_t size = strlen(pcmd[JOG_ACCEL]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
              
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[JOG_ACCEL],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
        
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(JOG_DECEL):
    {
    
      uint8_t size = strlen(pcmd[JOG_DECEL]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();      
    
//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
              
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[JOG_DECEL],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
      
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    
    case(JOG_SPD):
    {
    
      uint8_t size = strlen(pcmd[JOG_SPD]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();    

//      if (pump.attempts++ > 5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
              
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[JOG_SPD],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
        
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(DRIVE_STATUS):
    {
    
      uint8_t size = strlen(pcmd[DRIVE_STATUS]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++ > 20)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }   
//      else
//      {
                
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[DRIVE_STATUS],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK;       
      
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(MOTOR_ENABLE):
    {
    
      uint8_t size = strlen(pcmd[MOTOR_ENABLE]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();

//      if (pump.attempts++>5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
        
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[MOTOR_ENABLE],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
      
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(COMMENCE_JOG):
    {
    
      uint8_t size = strlen(pcmd[COMMENCE_JOG]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();

//      if (pump.attempts++>5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
                
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[COMMENCE_JOG],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
      
//      }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case (IDLE_SPEED):
    {
    
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++>5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
      
        float32_t rps = current_therapy_settings.pump.idle_rps;
        //float32_t rps = 0.83f;
        
        memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
        
        sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
        
        /* Change pump speed to idle */
        pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK;
      
 //     }
      
      __set_interrupt_state(s);
      
      break;
      
    }
    case (STOP_KILL):
    {
    
      uint8_t size = strlen(pcmd[STOP_KILL]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++>5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
              
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[STOP_KILL],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
     
//      }
      
      __set_interrupt_state(s);
            
      break;
      
    }
    case (ZERO_SPEED):
    {
    
      uint8_t size = strlen(pcmd[ZERO_SPEED]);

      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++>5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
                
        memcpy(&pump.cmd[pump.idx].data[0],pcmd[ZERO_SPEED],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
           
 //     }
      
      __set_interrupt_state(s);
            
      break;
      
    }
    case (VAC_PULSE_SPEED):
    {
    
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++>5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {
//        
//        float32_t rps = current_therapy_settings.pump.pulse_rps;
//        
//        memset(&pump.cmd[pump.idx].data[0],0,sizeof(FMT_SPEED_CMD_SPEC));
//        
//        sprintf((char*)&pump.cmd[pump.idx].data[0],"CS%05.2f\r",rps);
//        
//        /* Change pump speed to idle */
//        pump.cmd[pump.idx].size = sizeof(FMT_SPEED_CMD_SPEC);
//        pump.idx++;
//        UART4_C2 |= UART_C2_TCIE_MASK;
//      
//      }
//      
      __set_interrupt_state(s);
      
      break;
      
    }
    case(SOFT_RESET):
    {
    
//      uint8_t size;

//      __istate_t s = __get_interrupt_state();
//      __disable_interrupt();

//      if (pump.attempts++ == 125)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        size = strlen(pcmd[PROTOCOL]);
//        current_therapy_settings.pump.status &= ~DRIVE_STATUS_CMD_FIELDS;        
//        memcpy(&pump.cmd[pump.idx].data[0],pcmd[PROTOCOL],size);      
//        pump.cmd[pump.idx].size = size;
//        pump.idx++;
//        UART4_C2 |= UART_C2_TCIE_MASK;    
//        pump.state = CMD_MODE;
//        
//      }
//      else if (pump.attempts == 25)
//      {
//
//        size = strlen(pcmd[SOFT_RESET]);
//        memcpy(&pump.cmd[pump.idx].data[0],pcmd[SOFT_RESET],size);      
//        pump.cmd[pump.idx].size = size;
//        pump.idx++;
//        UART4_C2 |= UART_C2_TCIE_MASK; 
//      }
      
//      __set_interrupt_state(s);
        
      break;
      
    }
    case(PWR_MODE_SCL):
    {
      
      uint8_t size = strlen(pcmd[PWR_MODE_SCL]);
              
      __istate_t s = __get_interrupt_state();
      __disable_interrupt();
      
//      if (pump.attempts++>5)
//      {
//        
//        memset(&pump.state,0,sizeof(PUMP_CMD));
//        //pump.state = SOFT_RESET;  
//        
//      }
//      else
//      {

        memcpy(&pump.cmd[pump.idx].data[0],pcmd[PWR_MODE_SCL],size);      
        pump.cmd[pump.idx].size = size;
        pump.idx++;
        UART4_C2 |= UART_C2_TCIE_MASK; 
     
 //     }
      
      __set_interrupt_state(s);
            
      break;
      
    }
    case(CONTROL):
    {
         
      break;
      
    }
    case(WAIT_PROBE_TEST_COMPLETE):
    {
         
      break;
      
    }

        
    
  }
  
  return 0;

}

/***************************************************************************//**
Sets the pump state if an over current condition occurs.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t over_current(void)
{
  
  if (current_therapy_settings.treatment.therapy_mode != VAC)
  {
    
    spd_ctrl_state = STOPPING;
        
  }
  else
  {
    
    spd_ctrl_state = OFF;
    
  }
  
  return 0;
  
}

/***************************************************************************//**
Sets the pump state if an over energy condition occurs.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t over_energy(void)
{
  
  if (current_therapy_settings.treatment.therapy_mode == VAC)
  {
    
    spd_ctrl_state = QUIESCENT;
        
  }
  
  return 0;
  
}

/***************************************************************************//**
Inits the pump serial port at 9600 Baud
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_uart_init(void)
{
  
  BAUD_RATE br = BAUD_9600;
    
  PumpUART4_Init(br);
  
  return 1;
  
}

/***************************************************************************//**
@ENT - run pump
pump_set_speed
@param void
@return 0
@outputs
@context
*******************************************************************************/
void pump_set_speed(float Speed)
{
  if(Speed > 0.0f)
  {
    current_therapy_settings.pump.idle_rps = Speed;
  }
  else
  {
    current_therapy_settings.pump.idle_rps = 0.0f;
  }
  pump.state = IDLE_SPEED;
}

/***************************************************************************//**
Handle systick during pump start up to introduce necessary delay while certain
commands finish executing.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_startup_delay(void)
{
  
  static uint16_t systick_count_probe_complete = 0;
  
  if (pump.state == WAIT_PROBE_TEST_COMPLETE)
  {
    
    if (systick_count_probe_complete++ == 1000)
    {
      
      systick_count_probe_complete = 0;
      put_event(&evt_tbl,PROBE_TEST_COMPLETE);
      
    }
    else
    {
      SYST_CSR |= SysTick_CSR_ENABLE_MASK;
    }
    
  }
  else
  {
    
    put_event(&evt_tbl,PUMP_RESET);
    
  }

  return 0;
    
}
