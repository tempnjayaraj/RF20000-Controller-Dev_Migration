/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2846                 $:
$Date:: 2023-01-17 16:02:14#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      zigbee.h
\brief     Header file for Zigbee module driver.
\details   Zigbee module is used to interface with the wireless footpedal
This module is to interface with the OEM Zigbee card. The Zigbee card is
connected to UART5 RX/TX. The MK60 can send commands to the Zigbee module using
this interface. The UART specs are given below:

Data Rate: 38,400 baud
Data Bits: 8
Parity: None
Stop bits: 1
Flow Control: None
(From Receiver Communications Protocol Rev: 006
Product: Medical RF receiver 5/5/2011 )
The commands are listed below:
*******************************************************************************/
#include "Zigbee.h"
#include "MK26F18.h"
#include "intrinsics.h"
#include "system.h"
#include "cpu.h"

WIRELESS_CMD wireless;

STATIC_ARTC WIRELESS_STATUS wireless_status_state;
STATIC_ARTC uint8_t wireless_packet[3];

/***************************************************************************//**
Inits the Zingbee module
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t zigbee_init()
{
  /*
  The following are the pin connections to the Zigbee boards
  PTD14 - Zigbee_CNCT
  PTD13 - Wireless_3
  PTD11 - Wireless_1
  PTD7  - Wireless_2
  PTE8  - UART5_TX
  PTE9  - UART5_RX
  */

  /* Setup repestive PORTD pins as GPIO*/
  PORTD_PCR14 = PORT_PCR_MUX(1);
  PORTD_PCR13 = PORT_PCR_MUX(1);
  PORTD_PCR11 = PORT_PCR_MUX(1);
  PORTD_PCR7 = PORT_PCR_MUX(1);
  
  // Clearing the corresponding bit to set it as input */
  GPIOD_PDDR &= (~WIRELESS_1);
  GPIOD_PDDR &= (~WIRELESS_2);
  GPIOD_PDDR &= (~WIRELESS_3);
  GPIOD_PDDR &= (~ZIGBEE_CNCT);
   
  UART5_Init();
  
  //enable_datastream();
  
  return 1;
    
}
  

/***************************************************************************//**
Inits LPUART which is used to communicate with the zigbee module.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void UART5_Init(void) {

  uint8_t temp;
  NVIC_DisableIRQ(LPUART0_IRQn);
            
  /* Calculated set baud rate register value */
  uint16_t sbr;
  
  /* keep UART5 output pins high impedance during config */
  PORTE_PCR8 = PORT_PCR_MUX(5); // GPIO
  PORTE_PCR9 = PORT_PCR_MUX(5); // GPIO

  CLOCK_SetLpuartClock(1U);
  /* Enable gate clock */
  SIM_SCGC2 |= SIM_SCGC2_LPUART0_MASK;
  
  /* Disable transmitter and receiver while settings are modified. */
  LPUART0_CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK );
  
  /* Set baud rate register. OSR = 0x0f, Baud=38400, Clock = 54MHz */
  sbr = 78;//120//238; //For K26, Clock is 48MHz
    
  LPUART0_BAUD = LPUART_BAUD_SBR(sbr);
   
  /* Enable transmitter only and RDRF interrupt/DMA transfers */
 
  LPUART0_CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK );
  
  NVIC_EnableIRQ(LPUART0_IRQn); // was uart5 on K60
  
}
/***************************************************************************//**
Enables datastream between the processor and the zigbee module.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void enable_datastream (void)
{
  
//  uart_putchar(UART5_BASE_PTR,0xA5);
//  
//  uart_putchar(UART5_BASE_PTR,0x08);
// 
//  uart_putchar(UART5_BASE_PTR,0x01);
// 
//  uart_putchar(UART5_BASE_PTR,0x5A);
  
  uint8_t cmd[4] = {0xA5,0x08,0x01,0x5A};
  
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();

  memcpy(&wireless.cmd[wireless.idx].data[0],&cmd[0],4);      
  wireless.cmd[wireless.idx].size = 4;
  wireless.idx++;
  LPUART0_CTRL |= LPUART_CTRL_TCIE_MASK | LPUART_CTRL_RIE_MASK;        

  __set_interrupt_state(s);
      
}

/***************************************************************************//**
Reques the status byte from the Zigbee module/Wireless footswitch.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t request_statusbytes(void)
{
  
  uint8_t cmd[4] = {0xA5,0x07,0x30,0x5A};
  //static uint8_t attempts = 0;
    
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();
    
  if (current_therapy_settings.footpedal.wireless.attempts++ > 50)
  {
 
    current_therapy_settings.footpedal.wireless.attempts = 0;
    memset(&current_therapy_settings.footpedal.wireless,0,sizeof(current_therapy_settings.footpedal.wireless));
    current_therapy_settings.footpedal.wireless.present = 0;   
  }   
    memcpy(&wireless.cmd[wireless.idx].data[0],&cmd[0],4);      
    wireless.cmd[wireless.idx].size = 4;
    wireless.idx++;
    LPUART0_CTRL |= (LPUART_CTRL_TCIE_MASK | LPUART_CTRL_RIE_MASK);       
  __set_interrupt_state(s);
  
  return 0;
  
}

/***************************************************************************//**
Send command the to wireless footpedal/zigbee module.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void send_wireless_cmd(void)
{
  
  static uint8_t byte = 0;
  
  /* Transmit packet */
  
  if ((LPUART0_STAT & LPUART_STAT_TC_MASK)== LPUART_STAT_TC_MASK)
  {
    
    if ((wireless.idx == 0))
    {
      
      LPUART0_CTRL &= ~LPUART_CTRL_TCIE_MASK;
      
    }      
    
    else if (byte < 4)
    {
    
      LPUART0_DATA = (uint8_t) wireless.cmd[0].data[byte];
      byte++;
    
    }
              
    if (byte >= wireless.cmd[0].size)
    {
               
      memcpy(&wireless.cmd[0],&wireless.cmd[1],4*wireless.idx);
      
      wireless.idx--;
      
      byte = 0;
    
    }
   
  }
  
}

/***************************************************************************//**
Send status from the wireless footpedal/zigbee module.
@todo Check description.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void receive_wireless_cmd(void)
{

  
  uint8_t c = 0;
  uint8_t ablate=0,coag=0,mode=0,present=0;//,fs_error = 0;
#ifndef TEST         
  /* Wait until character has been received */
  while((LPUART0_STAT & LPUART_STAT_RDRF_MASK)== LPUART_STAT_RDRF_MASK)
  {
#endif
     c = LPUART0_DATA;
 
     switch (wireless_status_state)
     {
       
       case (SWITCH_STATUS):
       {
         
         if ((c & 0xC0) == 0)
         {
           
           wireless_packet[SWITCH_STATUS] = c;
           wireless_status_state++;
           
         }
         else
         {
           
           wireless_status_state = SWITCH_STATUS;
           memset(&wireless_packet[0],0,sizeof(wireless_packet));
           
         }
         
         break;
         
       }
       case (MARRIAGE_STATUS):
       {
         
         if ((c & 0xC0) == 0x40)
         {
           
           wireless_packet[MARRIAGE_STATUS] = c;
           wireless_status_state++;
           
         }
         else
         {
           
           wireless_status_state = SWITCH_STATUS;
           memset(&wireless_packet[0],0,sizeof(wireless_packet));
           
         }
         
         break;
         
       }
       case (SYSTEM_STATUS):
       {

         //uint8_t battery_status = 0;
         
         if ((c & 0xC0) == 0x80)
         {
           
           wireless_packet[SYSTEM_STATUS] = c;
           ablate = wireless_packet[0] & 0x1u;
           coag = (wireless_packet[0] & 0x2u) >> 1;
           mode = (wireless_packet[0] & 0x4u) >> 2;
           present = (wireless_packet[1] & 0x20u) >> 5;
           
           current_therapy_settings.footpedal.wireless.attempts = 0;
            current_therapy_settings.footpedal.wireless.ablate = ablate;
            current_therapy_settings.footpedal.wireless.coag = coag;
            current_therapy_settings.footpedal.wireless.mode = mode;
            current_therapy_settings.footpedal.wireless.safeguard = (wireless_packet[0] & 0x8u) >> 3;
            current_therapy_settings.footpedal.wireless.rf_signal_strength = (wireless_packet[0] & 0x30u) >> 4;
            current_therapy_settings.footpedal.wireless.byte_id0 = (wireless_packet[0] & 0xC0u) >> 6;
            current_therapy_settings.footpedal.wireless.marriage_err = (wireless_packet[1] & 0x1u);
            current_therapy_settings.footpedal.wireless.marriage_req = (wireless_packet[1] & 0x2u) >> 1;
            current_therapy_settings.footpedal.wireless.station2_response = (wireless_packet[1] & 0x4u) >> 2;
            current_therapy_settings.footpedal.wireless.station3_response = (wireless_packet[1] & 0x8u) >> 3;
            current_therapy_settings.footpedal.wireless.married2station12 = (wireless_packet[1] & 0x10u) >> 4;
            current_therapy_settings.footpedal.wireless.married2station1 = present;
            current_therapy_settings.footpedal.wireless.byte_id1 = (wireless_packet[1] & 0xC0u) >> 6;
            current_therapy_settings.footpedal.wireless.audible_alert = (wireless_packet[2] & 0x1u);
            current_therapy_settings.footpedal.wireless.xmit_batt = (wireless_packet[2] & 0x2u) >> 1;
            if (current_therapy_settings.footpedal.wireless.xmit_batt == 1)
            {  
              
              if (current_therapy_settings.footpedal.wireless.batt_low_count <= 100)
              {
                
                current_therapy_settings.footpedal.wireless.batt_low_count++;
                
              }
              
            }
            else
            {
              
              if (current_therapy_settings.footpedal.wireless.batt_low_count > 0)
              {
                
                current_therapy_settings.footpedal.wireless.batt_low_count--;
                
              }
              
            }
            current_therapy_settings.footpedal.wireless.locator_mode = (wireless_packet[2] & 0x4u) >> 2;
            current_therapy_settings.footpedal.wireless.marry_in_progress = (wireless_packet[2] & 0x10u) >> 4;
            current_therapy_settings.footpedal.wireless.married = (wireless_packet[2] & 0x8u) >> 3;
            /* latch married overcomes fs warm boot after 10 min idle */
            if (current_therapy_settings.footpedal.wireless.married_latch == 0)
            {
              
              current_therapy_settings.footpedal.wireless.married_latch = (wireless_packet[2] & 0x8u) >> 3;  
              
            }
            current_therapy_settings.footpedal.wireless.byte_id2 = (wireless_packet[2] & 0xC0u) >> 6;              
           
         }
          
         wireless_status_state = SWITCH_STATUS;
         memset(&wireless_packet[0],0,sizeof(wireless_packet));
                  
         break;
         
       }
     default:
       {
         // do nothing
       }
     
    }
#ifndef TEST
  }
#endif
}
/***************************************************************************//**
Perform handshake with the wireless footpedal module.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wireless_handshake(void)
{
  
  uint8_t cmd[4] = {0xA5,0x02,0x40,0x5A};
  
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();

  memcpy(&wireless.cmd[wireless.idx].data[0],&cmd[0],4);      
  wireless.cmd[wireless.idx].size = 4;
  wireless.idx++;
  LPUART0_CTRL |= LPUART_CTRL_TCIE_MASK | LPUART_CTRL_RIE_MASK;       

  __set_interrupt_state(s);
  
  return 0;
  
}

    
    
