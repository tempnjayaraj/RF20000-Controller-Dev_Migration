/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2802                 $:
$Date:: 2021-12-07 10:49:57#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/

/***************************************************************************//**
\file:     led.c
\brief     LED driver.
\details   N/A
*******************************************************************************/

#include "MK26F18.h"
#include "led.h"
#include "system.h"
#include "notifications.h"
#include "cpu.h"
#include "psu.h"
#include "gui.h"

extern legacy_wand_params legacy_wand_params_obj;

/****************************************************************************//**
Initializes the LED. Enable the I2C clock, enable I2C pins, I2C peripheral and
I2C irq
@param void
@return void
@outputs
@context
********************************************************************************/
uint8_t led_init(void) {

  uint8_t n = 255;  
  NVIC_DisableIRQ(I2C1_IRQn);
    
  /* Enable I2C1 gate clock */
  SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;
  
  /* disable i2c before mucking w/ registers */
  I2C1_C1 &= ~(I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK);
  
  /* briefly pull both sda & scl low to reset them */
  PORTC_PCR10 = PORT_PCR_MUX(1);
  PORTC_PCR11 = PORT_PCR_MUX(1);
  GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(10)|GPIO_PIN(11));
  GPIOC_PCOR |= GPIO_PDOR_PDO(GPIO_PIN(10)|GPIO_PIN(11));
  while(n--);
  
  /* Set I2C1 pins to control PCF8575 */
  PORTC_PCR10 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK; // I2C1_SCL
  PORTC_PCR11 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK; // I2C1_SDA
        
  /* Set I2C baud rate */
  I2C1_F = I2C_F_MULT(2) | I2C_F_ICR(21); // 210 kHz

  /* Enable I2C */
  I2C1_C1 |= I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK;
  
  NVIC_EnableIRQ(I2C1_IRQn);
  
  return 1;
    
}

/****************************************************************************//**
Start I2C transmission to LM48100Q amplifier.
@param Mode R/W bit at the end of Slave address
@return void
@outputs
@context
********************************************************************************/
void PCF8575_I2C_StartTransmission (uint8_t Mode)
{
  uint8_t SlaveID;
  
  /* Shift ID in right possition */
  SlaveID = (uint8_t) PCF8575_I2C_ADDRESS;

  /* Set R/W bit at end of Slave Address */
  SlaveID |= (uint8_t) Mode;

  /* Generate start signal */
  I2C1_START();

  /* Send ID with W/R bit */
  I2C1_D = SlaveID;
}

/****************************************************************************//**
Write appropriate value to I2C out (I2C_D) register.
@param state -uint8_t state of the system that needs to be indicated by the LEDs
@return void
@outputs
@context
********************************************************************************/
void pcf8575_iic_write(uint8_t state)
{
  
  uint8_t packet = 0;
   
  switch(state)
  {
    
    case(0):
    {
          
      if (current_therapy_settings.leds.pump == LED_RED)
      {
        
        packet &= ~PUMP_LED_RED;
        packet |= PUMP_LED_GRN;
        
      }

      else if (current_therapy_settings.leds.pump == LED_GRN)
      {
        
        packet &= ~PUMP_LED_GRN;
        packet |= PUMP_LED_RED;
        
      }

      else if (current_therapy_settings.leds.pump == LED_OFF)
      {
        
        packet |= PUMP_LED_RED | PUMP_LED_GRN;
        
      }  
        
        

      if (current_therapy_settings.leds.pwr == LED_RED)
      {
        
        packet &= ~PWR_LED_RED;
        packet |= PWR_LED_GRN;
        
      }

      else if (current_therapy_settings.leds.pwr == LED_GRN)
      {
        
        packet &= ~PWR_LED_GRN;
        packet |= PWR_LED_RED;
        
      }

      else if (current_therapy_settings.leds.pwr == LED_OFF)
      {
        
        packet |= PWR_LED_RED | PWR_LED_GRN;
        
      }

      if (current_therapy_settings.leds.odu == LED_RED)
      {
        
        packet &= ~ODU_LED_RED;
        packet |= ODU_LED_GRN;
        
      }

      else if (current_therapy_settings.leds.odu == LED_GRN)
      {
        
        packet &= ~ODU_LED_GRN;
        packet |= ODU_LED_RED;
        
      }

      else if (current_therapy_settings.leds.odu == LED_OFF)
      {
        
        packet |= ODU_LED_RED | ODU_LED_GRN;
        
      }

      if (current_therapy_settings.leds.lemo == LED_RED)
      {
        
        packet &= ~LEMO_LED_RED;
        packet |= LEMO_LED_GRN;
        
      }

      else if (current_therapy_settings.leds.lemo == LED_GRN)
      {
        
        packet &= ~LEMO_LED_GRN;
        packet |= LEMO_LED_RED;
        
      }

      else if (current_therapy_settings.leds.lemo == LED_OFF)
      {
        
        packet |= LEMO_LED_RED | LEMO_LED_GRN;
        
      }
      
      I2C1_D = packet; 
      break;
    }
    
    case(1):
    {
      
      /* Wait for transmit complete */
      //I2C1_WAIT();

      if (current_therapy_settings.leds.wand == LED_RED)
      {
        
        packet &= ~WAND_LED_RED;
        packet |= WAND_LED_GRN;
        
      }

      else if (current_therapy_settings.leds.wand == LED_GRN)
      {
        
        packet &= ~WAND_LED_GRN;
        packet |= WAND_LED_RED;
        
      }

      else if (current_therapy_settings.leds.wand == LED_OFF)
      {
        
        packet |= WAND_LED_RED | WAND_LED_GRN;
        
      }
      I2C1_D = packet;    
      break;
      
    }

    case(2):
    {
      
      /* Generate stop signal */
      I2C1_STOP();
      break;
      
    }
    
  }

}

/****************************************************************************//**
startup_led_init
@todo Add more description
@param
@return void
@outputs
@context
********************************************************************************/
uint8_t startup_led_init(void)
{
  
  uint8_t reset = 1;
  
  startup_led_sequence(reset);
  
  return 0;
  
}

/****************************************************************************//**
startup_leds
@todo Add more description
@param
@return void
@outputs
@context
********************************************************************************/
uint8_t startup_leds(void)
{
  
  startup_led_sequence(0);
  
  return 0;
  
}


/****************************************************************************//**
Startup sequence of LEDs. This function lights up the LEDs in the startup
sequence
@param void
@return 0
@outputs
@context
********************************************************************************/
void startup_led_sequence(uint8_t reset)
{
  
  static uint8_t count = 0;
  
  if (reset == 1)
  {
    
    count = 0;
    
  }
  else
  {
    
    count++;
    
  }
  
  if (current_therapy_settings.leds.startup == TRUE)
  {
    
    if (count < 2)
    {
      
      current_therapy_settings.leds.pwr = LED_GRN;
      current_therapy_settings.leds.wand = LED_OFF;
      current_therapy_settings.leds.odu = LED_OFF;
      current_therapy_settings.leds.lemo = LED_OFF;
      current_therapy_settings.leds.pump = LED_RED;
    
    }
    else if (count < 5)
    { 
      
      current_therapy_settings.leds.pump = LED_GRN;
      
    }
    else if (count < 6)
    {
      
      current_therapy_settings.leds.pump = LED_OFF;
      
    }
    else if (count < 7)
    {
      
      current_therapy_settings.leds.wand = LED_RED;
      current_therapy_settings.leds.pump = LED_OFF;
      
    }
    else if (count < 10)
    {
      
      current_therapy_settings.leds.wand = LED_GRN;
      
    }
    else if (count < 11)
    {  
      
      current_therapy_settings.leds.wand = LED_OFF;
      
    }
    else if (count < 12)
    {
      
      current_therapy_settings.leds.wand = LED_OFF;
      current_therapy_settings.leds.odu = LED_RED;
      
    }
    else if (count < 15)
    {

      current_therapy_settings.leds.odu = LED_GRN;
      
    }
    else if (count < 16)
    {
      
      current_therapy_settings.leds.odu = LED_OFF;
      
    }
    else if (count < 17)
    {

      current_therapy_settings.leds.odu = LED_OFF;
      current_therapy_settings.leds.lemo = LED_RED;
      
    }
    else if (count < 20)
    {
      
      current_therapy_settings.leds.lemo = LED_GRN;
      
    }
    else if (count < 21)
    {

      current_therapy_settings.leds.lemo = LED_OFF;
      
    }
    else if (count >= 21)
    {
     
      current_therapy_settings.leds.startup = FALSE;
      count = 0;
      
    }
    
  }
    
  //pcf8575_iic_write();
    /* reset interrupt */
  I2C1_S |= I2C_S_IICIF_MASK;
    
    /* Send address */
  PCF8575_I2C_StartTransmission(MWSR);
  
}


//uint8_t control_leds(void)
//{
//  
//  static uint8_t flag = 0;
//  
//  
//  if (current_therapy_settings.leds.startup == TRUE)
//  {
//    
//    flag = 0;
//    startup_leds();
//      
//  }
//  else if (current_therapy_settings.sw.shutdown_imminent == TRUE)
//  {
//    
//    flag ^= 1;
//    shutdown_led_sequence(flag);
//  
//  }
//  else
//  {
//    
//    flag = 0;
//    //GPIOA_PTOR = BLUE_LED;
//    update_leds();
//    
//    
//  }
//    
//}

/****************************************************************************//**
Update LEDs according to the system status
@param void
@return 0
@outputs
@context
********************************************************************************/
uint8_t update_leds(void)
{
  
//  uint8_t filter_cnt = 3;
  
  NOTIFY_TXT highest_priority_notify = NO_TXT;//0 is NO_TXT
  uint8_t wand_error_type = 0;
  
  static uint8_t wand_blink_duty_count = 0,
                 pump_blink_duty_count=0,
                 fp_blink_duty_count=0,
                 odu_blink_duty_count=0;
//                 pump_led_change_grn_filter = 0,
//                 pump_led_change_red_filter = 0,
//                 pump_led_change_off_filter = 0;
  
  LED_STATE pump_led_state_table[2][2][2][2] = 
  {
  
    /*  0 */ LED_OFF,
    /*  1 */ LED_OFF,
    
    /*  2 */ LED_OFF,
    /*  3 */ LED_OFF,
    
    /*  4 */ LED_FLASHING_GRN,
    /*  5 */ LED_FLASHING_GRN,
    
    /*  6 */ LED_FLASHING_GRN,
    /*  7 */ LED_GRN,
    
    /*  8 */ LED_OFF,
    /*  9 */ LED_OFF,
    
    /*  10*/ LED_FLASHING_GRN,
    /*  11*/ LED_FLASHING_GRN,
    
    /*  12*/ LED_OFF,
    /*  13*/ LED_OFF,
    
    /*  14*/ LED_FLASHING_GRN,
    /*  15*/ LED_FLASHING_GRN,
    
  }; 
  
  static LED_STATE last_pump_led_state = LED_OFF;
  LED_STATE pump_led_state = LED_OFF;
  uint8_t PrimeRunning = get_priming_status() != PRIME_OFF ? 1 : 0;  
  uint8_t ent_pump_wand = ((current_therapy_settings.controller == ENT) &&
                           (current_therapy_settings.legacy_wand_levels.topaz_timer == 0));
  
    
  pump_led_state = pump_led_state_table[PrimeRunning]
                                       [current_therapy_settings.wand.unlocked | ent_pump_wand]
                                       [current_therapy_settings.pump.door_present]
                                       [current_therapy_settings.pump.tube_present];
  
  highest_priority_notify = (NOTIFY_TXT) (current_therapy_settings.notify.status & 0xFF);
    
  wand_error_type = wand_error_type_decipher(highest_priority_notify);
  
  if ((highest_priority_notify == PUMP_FAULT_TXT) || 
      (highest_priority_notify == TUBE_ENGAGEMENT_ERROR_TXT))
  {
    current_therapy_settings.leds.pump = LED_RED;
    pump_blink_duty_count = 0;
    
  }
  else if ((current_therapy_settings.pump.on == 1) &&
           (PrimeRunning == 0))
  {
    
    current_therapy_settings.leds.pump = LED_GRN;
    pump_blink_duty_count = 0;
    
  }
  else if ((current_therapy_settings.active_wand.lw_active == 1) &&
           (ent_pump_wand == 0))
  {
    
    current_therapy_settings.leds.pump = LED_OFF;
    pump_blink_duty_count = 0;
    
  }
  else if (pump_led_state == last_pump_led_state)
  {
    
    if (pump_led_state == LED_FLASHING_GRN)
    {
      
      /* alternate off/green */
      if (pump_blink_duty_count < 1)
      {

        current_therapy_settings.leds.pump = LED_GRN;
        pump_blink_duty_count = 1;

      }
      else 
      {

        current_therapy_settings.leds.pump = LED_OFF;
        pump_blink_duty_count = 0;

      }
      
    }
    
    else
    {
      
      pump_blink_duty_count = 0;
      current_therapy_settings.leds.pump = last_pump_led_state;
      
    }
    
  }
  
  last_pump_led_state = pump_led_state;
  
    
  if (wand_error_type & 1)
  {
    
    current_therapy_settings.leds.wand = LED_RED;
    wand_blink_duty_count = 0;
    
  }
  else if (highest_priority_notify > 0)
  {
    
    current_therapy_settings.leds.wand = LED_OFF;
    wand_blink_duty_count = 0;
    
  }  
  else if (current_therapy_settings.sdhc.card_ready == TRUE)
  {
    
    current_therapy_settings.leds.wand = LED_OFF;
    wand_blink_duty_count = 0;
    
  }
  else if ((current_therapy_settings.wand.unlocked == 0) &&
           (current_therapy_settings.active_wand.lw_active == 0))
  {
        
    /* alternate off/green */
    if (wand_blink_duty_count < 1)
    {

      current_therapy_settings.leds.wand = LED_GRN;
      wand_blink_duty_count = 1;

    }
    else
    {

      current_therapy_settings.leds.wand = LED_OFF;
      wand_blink_duty_count = 0;

    }
   
  }
  else if (current_therapy_settings.active_wand.lw_active == 0)
  {
    
    wand_blink_duty_count = 0;
    current_therapy_settings.leds.wand = LED_GRN;
    
  }
  else
  {
    
    current_therapy_settings.leds.wand = LED_OFF;
    wand_blink_duty_count = 0;
      
  }
  
  
  /* legacy wand led */
  if (wand_error_type & 2)
  {
    
    current_therapy_settings.leds.odu = LED_RED;
    odu_blink_duty_count = 0;
    current_therapy_settings.notify.legacy_wand_short_led_sec_countdown = 0;
    
  }
  else if (highest_priority_notify > 0)
  {
    
    current_therapy_settings.leds.odu = LED_OFF;
    odu_blink_duty_count = 0;
    current_therapy_settings.notify.legacy_wand_short_led_sec_countdown = 0;
    
  }
  else if (current_therapy_settings.sdhc.card_ready == TRUE)
  {
    
    current_therapy_settings.leds.odu = LED_OFF;
    odu_blink_duty_count = 0;
    
  }
  else if (current_therapy_settings.notify.legacy_wand_short_led_sec_countdown > 0)
  {
    /* alternate red/green */
    if (odu_blink_duty_count < 1)
    {

      current_therapy_settings.leds.odu = LED_RED;
      odu_blink_duty_count = 1;

    }
    else
    {

      current_therapy_settings.leds.odu = LED_GRN;
      odu_blink_duty_count = 0;
      current_therapy_settings.notify.legacy_wand_short_led_sec_countdown--;

    }
    
    if (current_therapy_settings.active_wand.lw_active == 0)
    {
      current_therapy_settings.notify.legacy_wand_short_led_sec_countdown = 0;
    }
    
  }
    /* both wands connected finger sw force enable */
  else if ((legacy_wand_params_obj.wand_connected == 1) &&
      (current_therapy_settings.wand.unlocked == 1) &&
      (legacy_wand_params_obj.wand_909 == 1))
  {

    /* alternate off/green */
    if (odu_blink_duty_count < 1)
    {

      current_therapy_settings.leds.odu = LED_OFF;
      odu_blink_duty_count = 1;

    }
    else
    {

      current_therapy_settings.leds.odu = LED_GRN;
      odu_blink_duty_count = 0;

    }

  }
  else if ((current_therapy_settings.wand.unlocked == 0) &&
      (    current_therapy_settings.active_wand.lw_active == 0))
  {
    
    current_therapy_settings.notify.legacy_wand_short_led_sec_countdown = 0;
       
    /* alternate off/green */
    if (odu_blink_duty_count < 1)
    {

      current_therapy_settings.leds.odu = LED_OFF;
      odu_blink_duty_count = 1;

    }
    else
    {

      current_therapy_settings.leds.odu = LED_GRN;
      odu_blink_duty_count = 0;

    }
   
  }
  else if (current_therapy_settings.active_wand.lw_active == 1)
  {
    
    odu_blink_duty_count = 0;
    current_therapy_settings.leds.odu = LED_GRN;
    
  }
  else
  {
    
    current_therapy_settings.leds.odu = LED_OFF;
    odu_blink_duty_count = 0;
      
  }
  
    
  if (current_therapy_settings.notify.fp_switch_stuck == 1)
  {
    
    current_therapy_settings.leds.lemo = LED_RED;
    fp_blink_duty_count = 0;
    
  }
  else if (highest_priority_notify > 0)
  {
    
    current_therapy_settings.leds.lemo = LED_OFF;
    fp_blink_duty_count = 0;
    
  }
  else if (current_therapy_settings.sdhc.card_ready == TRUE)
  {
    
    current_therapy_settings.leds.lemo = LED_OFF;
    fp_blink_duty_count = 0;
    
  }
  else if (current_therapy_settings.footpedal.present == 0)
  {
    if (legacy_wand_params_obj.wand_connected == 0 &&
        current_therapy_settings.wand.unlocked == FALSE)
    {
       current_therapy_settings.leds.lemo = LED_OFF;
       fp_blink_duty_count = 0;
    }
    else if (((current_therapy_settings.treatment.finger_switch_en == 0) &&
              (current_therapy_settings.footpedal.wireless.present == 0) &&
              (current_therapy_settings.footpedal.wired.present == 0))
             ||
               ((legacy_wand_params_obj.wand_connected == 1) && 
                (current_therapy_settings.wand.unlocked == FALSE) &&
                ((legacy_wand_params_obj.wand_909 == 1) || (current_therapy_settings.controller == ENT)) &&
                (current_therapy_settings.footpedal.wired.present == 0) &&
                (current_therapy_settings.footpedal.wireless.present == 0)))
    {
      /* alternate off/green */
      if (fp_blink_duty_count < 1)
      {
        
        current_therapy_settings.leds.lemo = LED_GRN;
        fp_blink_duty_count = 1;
        
      }
      else
      {
        
        current_therapy_settings.leds.lemo = LED_OFF;
        fp_blink_duty_count = 0;
        
      }    
    }
    else
    {
      current_therapy_settings.leds.lemo = LED_OFF;
      fp_blink_duty_count = 0;
    }
   
  }
  else if (current_therapy_settings.footpedal.wired.present == 1)
  {
    
    current_therapy_settings.leds.lemo = LED_GRN;
    fp_blink_duty_count = 0;
   
  }
  else if ((current_therapy_settings.treatment.finger_switch_en == 0) &&
           (current_therapy_settings.footpedal.wireless.present == 0))
  {
    
     /* alternate off/green */
    if (fp_blink_duty_count < 1)
    {

      current_therapy_settings.leds.lemo = LED_GRN;
      fp_blink_duty_count = 1;

    }
    else
    {

      current_therapy_settings.leds.lemo = LED_OFF;
      fp_blink_duty_count = 0;

    }    
    
  }
  else
  {
    
    current_therapy_settings.leds.lemo = LED_OFF;
    fp_blink_duty_count = 0;
    
  }
  
  current_therapy_settings.leds.pwr = LED_GRN;
    
  //pcf8575_iic_write();
    /* reset interrupt */
  I2C1_S |= I2C_S_IICIF_MASK;
  
  /* Send address */
  PCF8575_I2C_StartTransmission(MWSR);
  
  return 0;
    
}

/****************************************************************************//**
Shutdown sequence of LEDs. This function lights up the LEDs in the shudown
sequence
@param void
@return 0
@outputs
@context
********************************************************************************/
uint8_t shutdown_led_sequence(void)
{
 
    static uint8_t alt = 0;
    uint8_t button_status = 0;

    led_init();
    
    button_status = power_button_status();
    
    alt ^= 1;
    
   /* alternate red/green during shutdown */
   if ((alt == 1) && (button_status == 1))
   {
     
     current_therapy_settings.leds.pwr = LED_RED;
     
   }
   else if (alt == 0) 
   {
     
     current_therapy_settings.leds.pwr = LED_GRN;
     
   }
   else
   {
     
     current_therapy_settings.leds.pwr = LED_OFF;
     
   }
     
  current_therapy_settings.leds.wand = LED_OFF;
  current_therapy_settings.leds.odu = LED_OFF;
  current_therapy_settings.leds.lemo = LED_OFF;
  current_therapy_settings.leds.pump = LED_OFF;
    
  //pcf8575_iic_write();
    /* reset interrupt */
  I2C1_S |= I2C_S_IICIF_MASK;
  
  /* Send address */
  PCF8575_I2C_StartTransmission(MWSR);
  
  return 0;
  
}
