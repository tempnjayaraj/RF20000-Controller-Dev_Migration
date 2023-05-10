/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2830                 $:
$Date:: 2022-04-15 09:55:01#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      post.c
\brief     Power on self test code
\details   NA
*******************************************************************************/

#include "MK26F18.h"
#include "post.h"
#include "pit.h"
#include "common.h"
#include "pump.h"
#include "system.h"
#include "buck.h"
#include "rcm.h"
#include "ftfe.h"
#include "audio.h"
#include "sensors.h"
#include "lpt.h"
#include "led.h"
#include "esdhc.h"
#include "notifications.h"

 
/* latch ensures motor test critera only evaluated once, in case door closes
   just before post exit, global scope so reset each init & read during post */
uint8_t  pump_test_latch = 0;

/***************************************************************************//**
Power on self test.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t post(void)
{
  
  static uint8_t pump_test_active = 0, pump_test_passed = 0;
  uint8_t door_closed = ((current_therapy_settings.pump.sensor_status & DOOR_SENSOR_ON) == DOOR_SENSOR_ON);
  uint8_t discharge_tube = ~(GPIOC_PDIR >> DISCHARGE_TUBE_SENSOR_SHIFT) & 1;
  uint8_t suction_tube = ~(GPIOC_PDIR >> SUCTION_TUBE_SENSOR_SHIFT) & 1;
  uint8_t vmod_passed = 0;
//  const uint16_t vmod_ulimit = 2560,
//                 vmod_llimit = 1536;
  
  uint8_t gui_update_jmpr = ~(GPIOB_PDIR >> 9) & 1;
  
  /* post_init resets latch so we know 1st time reset test variables */
  if (pump_test_latch == 0)
  {
    pump_test_active = 0;
    pump_test_passed = 0;
  }
  
  if (gui_update_jmpr == 1)
  {
    
    DISABLE_PIT(1);
    
  }
  
//  if (((current_therapy_settings.pump.on == 0) ||
//      ((current_therapy_settings.pump.status & PUMP_DISABLED) == PUMP_DISABLED)) &&
//       (pump.state == DRIVE_STATUS))
//  {
//    
//    pump.state = MOTOR_ENABLE;
//
//  }

  if ((door_closed == 1) && ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON) && (pump_test_passed == 0))
  {     
  
    pump.state = IDLE_SPEED;
    current_therapy_settings.pump.idle_rps = 1.0f;
    
  }

  else if (((current_therapy_settings.pump.status & PUMP_DISABLE_ALARM_FAULT) == PUMP_DISABLED) &&
            (pump.state == DRIVE_STATUS))  
  {
      if ((discharge_tube == 0) &&
         (suction_tube == 0) &&
         (door_closed == 1) &&
         (pump_test_active == 0) &&
         (pump_test_latch == 0))
      {
  
        current_therapy_settings.pump.status = 0;
        pump.state = MOTOR_ENABLE;
        pump_test_active = 1;
      }
      pump_test_latch = 1;
  }
  
  /* read rotor sensor */
  LPTMR0_CNR = 0;
  
  if ((LPTMR0_CNR >= 2) && (pump_test_passed == 0))
  {
    
    pump_test_passed = 1;
    pump.state = STOP_KILL;
    
  }
  
  /* cancel test if user opens door */
  if ((pump_test_active == 1) && (door_closed == 0))
  {
    
    pump_test_active = 0;
    
  }
//  else if ((current_therapy_settings.pump.status & PUMP_MOVING) == PUMP_MOVING)
//  {
//    
//    pump.state == STOP_KILL;
//        
//  }


  if ((current_therapy_settings.gui.rx_wdog.tx_cntr_idx != 0) &&
       (current_therapy_settings.leds.startup == FALSE) &&
       (vmod.vmod2_test_status == VMOD_TEST_DONE))
  {
        
//    /* test if vmod is within 25% */
//    if ((vmod.vmod2_meas[2048] > vmod_ulimit) ||
//        (vmod.vmod2_meas[2048] < vmod_llimit))
//    {
//      
//      put_event(&evt_tbl,VMOD_FAULT);
//      
//    }
//    else
//    {
//      
      vmod_passed = 1;
//      
//    }
            
    if ((pump_test_active == 1) && (pump_test_passed == 0))
    {
      
      put_event(&evt_tbl,PUMP_COM_FAULT);
      
    }
    
    if ((current_therapy_settings.pump.status & PUMP_DISABLED) != PUMP_DISABLED)
    {
      
      pump.state = STOP_KILL;
      
    }
    

        
  }
  else
  {
    put_event(&evt_tbl,POST);
    
  }

  return vmod_passed;
}

/***************************************************************************//**
Initializes the variables and devices before post
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t post_init(void)
{
  /* clear faults */
  current_therapy_settings.notify.status = 0;
  current_therapy_settings.notify.smart_wand_short = 0;
  current_therapy_settings.notify.legacy_wand_short = 0;
  current_therapy_settings.notify.legacy_wand_short_led_sec_countdown = 0;
  current_therapy_settings.notify.smart_wand_use_limit = 0;
  current_therapy_settings.notify.legacy_wand_use_limit = 0;
  current_therapy_settings.notify.smart_wand_no_ambient = 0;
  current_therapy_settings.notify.legacy_wand_no_ambient = 0;
  current_therapy_settings.notify.smart_wand_id_error = 0;
  current_therapy_settings.notify.legacy_wand_id_error = 0;
  current_therapy_settings.notify.smart_wand_expired = 0;
  current_therapy_settings.notify.legacy_wand_expired = 0;
  current_therapy_settings.notify.smart_wand_stuck_switch = 0;
  current_therapy_settings.notify.legacy_wand_stuck_switch = 0;
  current_therapy_settings.notify.smart_wand_disconnected = 0;
  current_therapy_settings.notify.legacy_wand_disconnected = 0;
  current_therapy_settings.notify.fp_switch_stuck = 0;
  current_therapy_settings.pump.status &= ~SENSOR_STATES;
  current_therapy_settings.audio.ambient_alarm_page = 0;
  current_therapy_settings.footpedal.coag = 0;
  current_therapy_settings.footpedal.ablate = 0;
  current_therapy_settings.footpedal.mode = 0;
  current_therapy_settings.footpedal.mode_key = 0;
  current_therapy_settings.controller = UNKNOWN_GEN;
  current_therapy_settings.active_wand.smart_wand = UNKNOWN_GEN;

  startup_led_init();
  
  reset_pulse_cnt();
  
  current_therapy_settings.audio.splash_volume = 0;
  current_therapy_settings.audio.p_sound_file = lib_sounds[12].p;
  FTM2_MOD = 3355;
  //current_therapy_settings.audio.nsamples = lib_sounds[11].n;
        
  DISABLE_AUDIO;
  
  Audio_PWM_DMA_Init();
  
  reset_monitor();
    
  
  put_event(&evt_tbl,GET_MFG_DATE_TIME);
  put_event(&evt_tbl,GET_SERIAL_NUM);
            
  /* switch on buck for vmod test */
  GPIOE_PCOR |= GPIO_PDOR_PDO(BUCK_WW_ON | BUCK_18PIN_ON);

  /* set both outputs to max */
  //SET_DAC(pSmartWandDAC,0x7FF);
  //SET_DAC(pLegacyWandDAC,0x7FF);
  for (int i = 0; i < MAX_12BIT_NUM; i++)
  {
  
    vmod.vmod2_meas[i] = 0u;
    vmod.vmod2_lookup[i] = 0u;
  
  }
  vmod.vmod2_test_status = vmod2_ramp_test(1);
  vmod.vmod1_test_status = vmod1_ramp_test(1);
  SET_DAC(pSmartWandDAC,0);
  SET_DAC(pLegacyWandDAC,0);
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();
  vmod.mod1_meas = 0;
  vmod.mod2_meas = 0;
  __set_interrupt_state(s);
  
  
  pump.state = PROTOCOL;
  
  /* kick-off init manager */
  put_event(&evt_tbl,POST);
  
  pump_test_latch = 0;
          
  return 1;
  
}

/***************************************************************************//**
This functions perfoms the activities after post
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t post_exit(void)
{
  
  uint8_t checksum = 0;
  uint32_t *clock;
  uint8_t reset_vbatt_tamper_jmpr = ~(GPIOB_PDIR >> 9) & 1;
  uint32_t current_time = RTC_TSR & RTC_TSR_TSR_MASK;
  static uint8_t wireless_pair_test_complete = 0;
    
  /* switch off buck */
  GPIOE_PSOR |= GPIO_PDOR_PDO(BUCK_WW_ON | BUCK_18PIN_ON);
  SET_DAC(pSmartWandDAC,0);
  SET_DAC(pLegacyWandDAC,0);
  
  checksum = 0;
  
  /* copy mfg date time code to ram */
  for (uint8_t i = 0; i < sizeof(serial_number)-1; i++)
  {

    /* count erased bytes */
    checksum += serial_number[i];
    
  }
  
  checksum = ~checksum + 1;
  
  /* determine if there is a valid date/time code in flash */
  if (checksum != serial_number[15])
  {
    
    put_event(&evt_tbl,MFG_SN_FAULT);
    
  }
  
  if (current_therapy_settings.sdhc.card_fault == 1)
  {
    current_therapy_settings.notify.status2 |= GENERIC_ERROR_2_STATUS_2_MASK;
    //put_event(&evt_tbl,MFG_DATE_TIME_FAULT);
    
  }
  
  //erased_bytes = 0;
  checksum = 0;
  
  /* copy mfg date time code to ram */
  for (uint8_t i = 0; i < sizeof(mfg_date_time)-1; i++)
  {

    /* count erased bytes */
    checksum += mfg_date_time[i];
    
  }
  
  checksum = ~checksum + 1;
  swap_4bytes(&mfg_date_time[0])
  clock = (uint32_t*)&mfg_date_time[0];
  
  /* determine if there is a valid date/time code in flash */
  if (checksum != mfg_date_time[4])
  {
    
    put_event(&evt_tbl,MFG_DATE_TIME_FAULT);
    
  }
  else if ((current_time) < *clock)
  {
    
    /* copy date/time to rtc tsr */
    RTC_SR &= ~RTC_SR_TCE_MASK;
    memcpy(((uint32_t*)0x4003D000),&mfg_date_time,4);
    RTC_SR |= RTC_SR_TCE_MASK;
    
  }
  
  if (((current_therapy_settings.rtc.power_down_time) > current_time) ||
       (current_therapy_settings.rtc.tamper_detected == 1))
  {
    
    current_therapy_settings.notify.status2 |= GENERIC_ERROR_2_STATUS_2_MASK;
    current_therapy_settings.rtc.tamper_detected = 1;
    current_therapy_settings.footpedal.wireless.married_latch = 0;
    current_therapy_settings.treatment.wireless_enable = (bool) 0;
        
  }
  else if (((current_time-current_therapy_settings.rtc.power_down_time) >
             current_therapy_settings.footpedal.wireless.pair_timeout) &&
            (wireless_pair_test_complete == 0))
  {
    current_therapy_settings.footpedal.wireless.married_latch = 0;
    current_therapy_settings.treatment.wireless_enable = (bool) 0;
  }
  
  /* only conduct 15-min wireless pair timeout test once */
  wireless_pair_test_complete = 1;
    
  if (reset_vbatt_tamper_jmpr == 1)
  {
    
    current_therapy_settings.rtc.power_down_time = (RTC_TSR & RTC_TSR_TSR_MASK);
    current_therapy_settings.rtc.tamper_detected = 0;
    
  }
    
  //current_therapy_settings.audio.p_sound_file = 0;

    
  return 1;
  
}

/***************************************************************************//**
Get manufacturing date
@param void
@return 0
@outputs
@context
*******************************************************************************/

uint8_t get_mfg_date(void)
{
  
  uint8_t checksum = 0;
  nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
    
  /* copy mfg date time code to ram */
  for (uint8_t i = 0; i < sizeof(mfg_date_time) - 1; i++)
  {

    mfg_date_time[i] = p_data->mfg_date_time[i];
                  
    checksum += mfg_date_time[i];
    
  }
  
  checksum = ~checksum + 1;
    
  /* determine if there is date/time code to burn */
  if (checksum == p_data->mfg_date_time[4])
  {
    
    mfg_date_time[4] = checksum;
    
    /* burn date/time code in one-time flash */
    program_ftfe_once(&mfg_date_time[0],sizeof(mfg_date_time),7);
    
  }
  
  /* read date/time code from flash */
  read_ftfe_once(&mfg_date_time[0],sizeof(mfg_date_time),7);
  
//  //erased_bytes = 0;
//  checksum = 0;
//  
//  /* copy mfg date time code to ram */
//  for (uint8_t i = 0; i < sizeof(mfg_date_time)-1; i++)
//  {
//
//    /* count erased bytes */
//    checksum += mfg_date_time[i];
//    
//  }
//  
//  checksum = ~checksum + 1;
//  
//  /* determine if there is a valid date/time code in flash */
//  if (checksum != mfg_date_time[4])
//  {
//    
//    put_event(&evt_tbl,MFG_DATE_TIME_FAULT);
//    
//  }
//  else
//  {
//    
//    /* copy date/time to rtc tsr */
//    RTC_SR &= ~RTC_SR_TCE_MASK;
//    swap_4bytes(&mfg_date_time[0])
//    memcpy(((uint32_t*)0x4003D000),&mfg_date_time,4);
//    RTC_SR |= RTC_SR_TCE_MASK;
//    
//  }
  
  return 0;
  
}

/***************************************************************************//**
Get serial number of the controller which is stored in a NVM memory location
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t get_serial_num(void)
{
  
  uint8_t checksum = 0;
  nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;

  /* copy serial number to ram */
  for (uint8_t i = 0; i < sizeof(serial_number)-1; i++)
  {

    serial_number[i] = p_data->serial_number[i];
        
    checksum += serial_number[i];
    
  }
  
  checksum = ~checksum + 1;
  
  /* determine if there is serial number to burn */
  if (checksum == p_data->serial_number[15])
  {
    
    serial_number[15] = checksum;
    
    /* burn date/time code in one-time flash */
    program_ftfe_once(&serial_number[0],8,3);
    program_ftfe_once(&serial_number[8],8,4);
    
  }
  
  /* read date/time code from flash */
  read_ftfe_once(&serial_number[0],8,3);
  read_ftfe_once(&serial_number[8],8,4);
  
  sn_acii2bcd(&serial_number[0],&bcd_serial_number[0]);
  
//  checksum = 0;
  
//  /* copy mfg date time code to ram */
//  for (uint8_t i = 0; i < sizeof(serial_number)-1; i++)
//  {
//
//    /* count erased bytes */
//    checksum += serial_number[i];
//    
//  }
//  
//  checksum = ~checksum + 1;
//  
//  /* determine if there is a valid date/time code in flash */
//  if (checksum != serial_number[15])
//  {
//    
//    put_event(&evt_tbl,MFG_SN_FAULT);
//    
//  }
  
  return 0;
  
}

/***************************************************************************//**
Converts the serial number from ASCII format to BCD format
@param uint8_t pointer uint8_t pointer
@return 0
@outputs
@context
*******************************************************************************/
void sn_acii2bcd(uint8_t *p_sn, uint8_t *p_bcd_sn)
{
   
  for (int i = 0; i < MFG_SERIAL_NUM_CHARS; i++)
  {
    
    *p_bcd_sn++ = ((*p_sn++ - 0x20) & 0x3F);
        
  }
      
}
  
  
