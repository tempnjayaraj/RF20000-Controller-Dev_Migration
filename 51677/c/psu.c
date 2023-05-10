/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2844                 $:
$Date:: 2023-01-17 16:00:04#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     psu.c
\brief     Power supply unit driver
\details   N/A
*******************************************************************************/

#include "MK26F18.h"
#include "system.h"
#include "psu.h"
//#include "cpu.h"
#include "event_buffer.h"
#include "buck.h"
#include "notifications.h"

extern bool soft_reboot_detected;
/****************************************************************************//**
Initializes the power supply unit
@param void
@return void
@outputs
@context
********************************************************************************/
void psu_init(void)
{
  
  /* Enable gate clock */
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
  
  /* Configure port pin control registers */
  PORTB_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK;// | PORT_PCR_IRQC(11);
  PORTB_PCR3 = PORT_PCR_MUX(1);
  PORTA_PCR29 = PORT_PCR_MUX(1);
  PORTA_PCR28 = PORT_PCR_MUX(1);
  PORTA_PCR27 = PORT_PCR_MUX(1);
  
  /* imx53 reset line */
  PORTD_PCR15 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
  
  /* configure inputs */
  GPIOB_PDDR &= ~GPIO_PDDR_PDD(PWR_BUTTON_MASK);
  
  /* configure outputs */
  GPIOB_PDDR |= GPIO_PDDR_PDD(GLB_PWR_EN_MASK);
  GPIOA_PDDR |= GPIO_PDDR_PDD(RF_PWR_ON_MASK | MOTOR_PWR_ON_MASK | TWELVE_VOLT_ON_MASK);
  GPIOD_PDDR |= GPIO_PDDR_PDD(GUI_RESET_MASK);

  /* Clear pending interrupts */
  PORTB_PCR7 |= PORT_PCR_ISF_MASK;
  
  /* switch power OFF */
  GPIOB_PCOR |= GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
  GPIOA_PCOR |= GPIO_PDOR_PDO(RF_PWR_ON_MASK |
                              MOTOR_PWR_ON_MASK); 
  GPIOA_PSOR |= GPIO_PDOR_PDO(TWELVE_VOLT_ON_MASK);                              
  GPIOD_PCOR |= GPIO_PDOR_PDO(GUI_RESET_MASK);
    
  /* PortB pin detect interrupt */
  //Enable_IRQ(88);

}

///****************************************************************************//**
//Polls for power button status
//@todo - CHECK TO SEE IF THIS IS CORRECT.The context of this function is limited
//* to 100 ms interrupt in order to meet correct delay specification between
//* when the user intiates controlled shutdown and when the power is actually 
//* switched OFF.
//@param void
//@return void
//@outputs
//@context
//********************************************************************************/
//void Poll_Pwr_Button_Status(void) {
//  
//  uint8_t button_status = 0;
//  const uint8_t FIVE_SECONDS = 50;
//  static uint8_t shutdown_delay = 0;
//    
//  button_status = ~(GPIOB_PDIR >> PWR_BUTTON_ON_SHIFT) & 1;
//    
//  if (button_status == 0)
//  {  
//   
//    //current_therapy_settings.psu.on ^= 1;
//    //current_therapy_settings.sw.shutdown ^= TRUE;
//    
//    current_therapy_settings.sw.shutdown = FALSE;
//    current_therapy_settings.sw.shutdown_imminent = FALSE;
//    
//    shutdown_delay = 0;
//        
//    GPIOD_PSOR |= GPIO_PDOR_PDO(GUI_RESET_MASK);
//    
//    /* Turn power on */
//    GPIOB_PSOR |= GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
//    GPIOA_PSOR |= GPIO_PDOR_PDO(RF_PWR_ON_MASK |
//                                MOTOR_PWR_ON_MASK);
//    GPIOA_PCOR |= GPIO_PDOR_PDO(TWELVE_VOLT_ON_MASK); 
//    
//    
//  }
//  
//  else
//  {
//    
//    
////    /* Turn power on */
////    GPIOB_PCOR |= GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
////    GPIOA_PCOR |= GPIO_PDOR_PDO(RF_PWR_ON_MASK |
////                                MOTOR_PWR_ON_MASK | 
////                                TWELVE_VOLT_ON_MASK);
////    
////    current_therapy_settings.psu.on = 0;
//    
//    
//    shutdown_delay++;
//    
//    current_therapy_settings.sw.shutdown_imminent = TRUE;
//    
//    if (shutdown_delay >= FIVE_SECONDS)
//    {
//      
//      shutdown_delay = FIVE_SECONDS;
//      
//      /* Turn power OFF */
//      GPIOB_PCOR |= GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
//      GPIOA_PCOR |= GPIO_PDOR_PDO(RF_PWR_ON_MASK |
//                                  MOTOR_PWR_ON_MASK);
//      
//      GPIOA_PSOR |= GPIO_PDOR_PDO(TWELVE_VOLT_ON_MASK); 
//      
//      current_therapy_settings.psu.on = 0;
//      
//      current_therapy_settings.sw.shutdown = TRUE;
//      
//    }
//    
//  }
//    
//}

/****************************************************************************//**
polls power button
@TODO two functions with very similar functionality - CHECK
@param void
@return 1 - returns 1 when shutting down
@outputs
@context
********************************************************************************/
uint8_t poll_pwr_button(void)
{

  uint8_t button_status = 0;
  static uint8_t shutdown_delay = 0;
  static uint8_t debounce_switch = 0;
  static uint8_t powerup_delay = 0;
  static uint8_t gui_startup_delay = 0;
  /* only time this will be zero is booting for first time */
  uint8_t power_on = ((GPIOB_PDOR & GPIO_PDOR_PDO(GLB_PWR_EN_MASK)) == GPIO_PDOR_PDO(GLB_PWR_EN_MASK));
  //uint8_t power_failure = ((~(GPIOB_PDIR >> 22) & 1) &&
  //                        (~(GPIOB_PDIR >> 20) & 1) &&
  //                        (~(GPIOB_PDIR >> 23) & 1));   
  
  uint8_t power_failure = current_therapy_settings.pump.stator < STATOR_POWER_DOWN ? 1 : 0; //On K26 it goes to around 37000 counts.
  bool fw_update_done = current_therapy_settings.sw.update_complete;
 
  button_status = power_button_status();
  
  //GPIOA_PTOR |= GPIO_PDOR_PDO(BLUE_LED);
  
  if(power_failure == 1)
  {
    asm("nop");
  }
  
  if ((button_status == 0) &&
     (((power_failure == 1) && (power_on == 0) && (fw_update_done == FALSE)) || 
     ((power_failure == 0) && (current_therapy_settings.sw.shutdown == TRUE) && (fw_update_done == FALSE))))
  {
    
    debounce_switch++;

    shutdown_delay = 0;
    gui_startup_delay = 0;

    /* hold imx in reset */
    //GPIOD_PCOR = GPIO_PDOR_PDO(GUI_RESET_MASK);
    
    if (debounce_switch >= 25)
    {
      
      put_event (&evt_tbl, POWER_UP);
      current_therapy_settings.gui.rx_wdog.wdog_err_cnt = 0;
      current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 0;
      current_therapy_settings.sdhc.card_ready = 0;
      debounce_switch = 0;
      powerup_delay = 100;
      
    }
        
    return 0;
     
  }
  else if (((button_status == 1) && (power_on == 1)) ||
           (shutdown_delay > 0) ||
           (power_failure == 1) && (powerup_delay == 0) ||
           (fw_update_done == TRUE))
  {
    
    
  
      
    shutdown_delay++;
    
    debounce_switch = 0;
    gui_startup_delay = 0;
    
    current_therapy_settings.sw.shutdown_imminent = TRUE;
    soft_reboot_detected = TRUE;
    
    if (shutdown_delay >= 150)
    {
      
      shutdown_delay = 0;
      current_therapy_settings.pump.status = 0;
      
      /* hold imx in reset */
      GPIOD_PCOR = GPIO_PDOR_PDO(GUI_RESET_MASK);
      
      if ((button_status == 1) ||
          (fw_update_done == TRUE))
      {
        
        put_event(&evt_tbl,SAVE_PARAMS_TO_FLASH);
        
      }
      else
      {
        
        current_therapy_settings.sw.shutdown = TRUE;
        
      }
            
    }
    
    return 1;
    
  }
  else if (powerup_delay > 0)
  {
    
    powerup_delay--;
    return 0;
    
  }
  else if ((button_status == 0) &&
           (power_on == 1) &&
           (gui_startup_delay <= 99) &&
           (power_failure == 0))
  {    
    
    gui_startup_delay++;
    current_therapy_settings.gui.rx_wdog.wdog_err_cnt = 0;      
    
    if (gui_startup_delay == 100)
    {
      
      GPIOD_PSOR = GPIO_PDOR_PDO(GUI_RESET_MASK);
      if (((current_therapy_settings.notify.status & FAULT_MASK_1) == 0) &&
         ((current_therapy_settings.notify.status2 & FAULT_MASK_2) == 0))
      {
        GPIOA_PSOR = GPIO_PDOR_PDO(MOTOR_PWR_ON_MASK);
      }
    }
    
    return 0;
    
  }
  else
  {
    
    return 0;
    
  }
  
}

/****************************************************************************//**
Turns on the power for the board
@param void
@return 1
@outputs
@context
********************************************************************************/
uint8_t power_board_on(void)
{
  
  current_therapy_settings.sw.shutdown = FALSE;
  current_therapy_settings.sw.shutdown_imminent = FALSE;
  current_therapy_settings.leds.startup = TRUE;

  /* Turn power on */
  GPIOB_PSOR = GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
  GPIOA_PSOR = GPIO_PDOR_PDO(RF_PWR_ON_MASK);
  GPIOA_PCOR = GPIO_PDOR_PDO(TWELVE_VOLT_ON_MASK); 
    
  return 1;
  
}

/****************************************************************************//**
Turns off the high voltage circuit
@param void
@return 1
@outputs
@context
********************************************************************************/
uint8_t high_volt_off(void)
{
  
  /* switch off buck */
  GPIOE_PSOR |= GPIO_PDOR_PDO(BUCK_WW_ON | BUCK_18PIN_ON);  
  
  GPIOA_PCOR = GPIO_PDOR_PDO(RF_PWR_ON_MASK |
                              MOTOR_PWR_ON_MASK);
    
    
  return 1;
  
}


///******************************************************************************
//* Function: IRQ FootPedal_Status
//*
//* Purpose: Service interrupt if triggered by changed status of foot pedals.
//******************************************************************************/    
//
//void IRQ_PSU_Status(void) {
//  
//  /* Check footpedal trigger and update status */
//  if (PORTB_ISFR & PWR_BUTTON_ON_MASK) {
//   
//    Poll_Pwr_Button_Status();
//   
//    /* Reset interrupt(s) */
//    PORTB_ISFR |= PWR_BUTTON_ON_MASK;
//    
//  }
//    
//}

/****************************************************************************//**
This function returns the power button status
@param void
@return 1
@outputs
@context
********************************************************************************/
uint8_t power_button_status(void)
{
  
  return ~(GPIOB_PDIR >> PWR_BUTTON_ON_SHIFT) & 1;
  
}

/****************************************************************************//**
This function shuts off the whole power supply unit and does a system reset
@param void
@return 1
@outputs
@context
********************************************************************************/
uint8_t psu_off(void)
{
  
  /* hold imx in reset */
  GPIOD_PCOR = GPIO_PDOR_PDO(GUI_RESET_MASK);
  
  GPIOA_PCOR |= GPIO_PDOR_PDO(RF_PWR_ON_MASK |
                              MOTOR_PWR_ON_MASK);

  GPIOA_PSOR |= GPIO_PDOR_PDO(TWELVE_VOLT_ON_MASK);
  
  /* Turn power OFF */
  GPIOB_PCOR |= GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
  
  /* initiate software reset */
  put_event(&evt_tbl,SYSTEM_RESET);
      
  return 0;
  
}

