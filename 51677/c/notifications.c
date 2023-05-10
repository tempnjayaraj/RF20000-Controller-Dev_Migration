/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2798                 $:
$Date:: 2021-11-17 02:10:26#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      notifications.h
\brief     Notification manager
\details   Process notifications (including alarms).
*******************************************************************************/
#include "notifications.h"
#include "systick.h"
#include "system.h"
#include "audio.h"
#include "MK26F18.h"
#include "psu.h"
#include "pit.h"
#include <math.h>
#include <limits.h>
#include "legacy_wand.h"
#include "wand.h"

#ifdef DEBUG_CAN
#include "can_debug.h"
#endif

static uint8_t SmartWandShortId = 0x10;
extern legacy_wand_params legacy_wand_params_obj;
extern manta_prime prime;
extern u8 PrimeId;
extern prime_mode_t PrimeState;
extern u8 AutoPrimeId;
extern u8 AutoPrimeTimeoutId;
void wand_short_error(void);

uint8_t smart_wand_peak_hit_cnt = 0;
uint8_t smart_wand_short_tracker_f = 0;
uint8_t smart_wand_consecutive_shorts = 0;


/***************************************************************************//**
NotificationManager. Determine if an alarm or other notification condition
exists.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void notification_manager(void) {

  uint32_t temp1 = 0,
           temp2 = 0;
  uint8_t priority = 0;
  
  if (current_therapy_settings.gui.depth_idx > 4)
  {
    
    asm("nop");
    
  }
  
  if ((current_therapy_settings.active_wand.smart_wand == ENT) ||
      ((current_therapy_settings.active_wand.legacy_wand == ENT) && 
       (legacy_wand_params_obj.wand_connected == 1)))
  {
       current_therapy_settings.controller = ENT;
  }
  else if (((current_therapy_settings.active_wand.smart_wand == SPORTS_MED) ||
           (current_therapy_settings.active_wand.smart_wand == ARTHROPLASTY)) ||
           ((current_therapy_settings.active_wand.legacy_wand == SPORTS_MED) &&
            (legacy_wand_params_obj.wand_connected == 1)))
  {
       current_therapy_settings.controller = SPORTS_MED;
  }
//  else if ((current_therapy_settings.active_wand.smart_wand == ARTHROPLASTY) ||
//           ((current_therapy_settings.active_wand.legacy_wand == ENT) && 
//            (legacy_wand_params_obj.wand_connected == 1))
//  {
//    current_therapy_settings.controller = ENT;//ARTHROPLASTY;
//  }
  else
  {
    current_therapy_settings.controller = UNKNOWN_GEN;
  }
            
  
  if ((current_therapy_settings.wand.valid == TRUE) &&
      (current_therapy_settings.active_wand.lw_active == 0))
  {
    
    if (smart_wand_params.mfg.ambient_enable == 1)
    {
      current_therapy_settings.ambient.capable = 1;
       
      if (current_therapy_settings.ambient.enable_user_pref == 1)
      {
         
        current_therapy_settings.ambient.enable = 1;
         
      }
    
    }
    
    else
    {

      current_therapy_settings.ambient.capable = 0;
      current_therapy_settings.ambient.enable = 0;

    }
    
  }
    
  /* no wands connected temperature is zero */
  if (current_therapy_settings.wand.unlocked == 0)
  {
    
    current_therapy_settings.notify.status2 &= ~CHECK_ELECTRODE_NOTIFICATION_MASK;
    
  }
  
  /* no wands connected temperature is zero */
  if ((legacy_wand_params_obj.wand_connected == 0) &&
      (current_therapy_settings.wand.unlocked == 0))
  {
    
    current_therapy_settings.ambient.temperature = 0;
    
  }
  
  if (legacy_wand_params_obj.wand_connected == 0)
  {
    current_therapy_settings.ambient.legacy_temperature = 0;
  }
  
  if (current_therapy_settings.wand.unlocked == 0)
  {
    current_therapy_settings.ambient.smart_wand_temperature = 0;
  }
  
  if ((current_therapy_settings.ambient.temperature >= current_therapy_settings.ambient.alarm_threshold) && (current_therapy_settings.ambient.alarm_active == 0) &&
      (current_therapy_settings.ambient.capable == 1) && (current_therapy_settings.ambient.enable == 1))
  {


    if ((legacy_wand_params_obj.wand_connected == 1) ||
        (current_therapy_settings.wand.unlocked == 1))
    {
      current_therapy_settings.ambient.alarm_active = 1;
      current_therapy_settings.notify.status |= AMBIENT_ALARM_NOTIFICATION_MASK;
      current_therapy_settings.audio.ambient_alarm_page = 0;
    }
    

    //return 1;


  }
  //else if ((current_therapy_settings.ambient.temperature < current_therapy_settings.ambient.alarm_threshold) && (current_therapy_settings.ambient.alarm_active == 1))
  else if (((current_therapy_settings.ambient.temperature < current_therapy_settings.ambient.alarm_threshold) &&
            (current_therapy_settings.ambient.alarm_active == 1)) ||
            ((current_therapy_settings.ambient.enable == 0) &&
            (current_therapy_settings.ambient.alarm_active == 1)) ||
              ((current_therapy_settings.ambient.audio_enabled == 0) && (current_therapy_settings.ambient.alarm_active == 1)))
  {

    //DISABLE_AUDIO;

    current_therapy_settings.ambient.alarm_active = 0;
    current_therapy_settings.notify.status &= ~AMBIENT_ALARM_NOTIFICATION_MASK;

    //return 1;

  }

//  else
//  {
//    //return 0;
//  }
//
//  if ((current_therapy_settings.treatment.ablate_on == 1) ||
//     (current_therapy_settings.treatment.coag_on == 1))
//  {
//
//    if ((((current_therapy_settings.wand.tube_warn_pulse == TRUE) &&
//         (current_therapy_settings.pump.ctrl_en == TRUE)) ||
//         ((current_therapy_settings.wand.tube_alert == TRUE) &&
//          (current_therapy_settings.pump.ctrl_en == TRUE))) &&
//          (current_therapy_settings.wand.tube_warn == FALSE))
//    {
//
//      //current_therapy_settings.pump.ctrl_en = FALSE;
//      //current_therapy_settings.wand.tube_warn = TRUE;
//      put_event(&evt_tbl,RF_OFF);
//
//    }
////    else if ((current_therapy_settings.wand.tube_warn_pulse == FALSE) &&
////             (current_therapy_settings.wand.tube_warn == TRUE) &&
////             (current_therapy_settings.pump.ctrl_en == FALSE) &&
////             (current_therapy_settings.wand.tube_alert == FALSE))
////    {
////
////      //current_therapy_settings.pump.ctrl_en = TRUE;
////      //put_event(&evt_tbl,RF_ON);
////
////    }
//
//  }


  if ((SIM_SCGC6 & SIM_SCGC6_RTC_MASK) == SIM_SCGC6_RTC_MASK)
  {
    current_therapy_settings.notify.status =
      multiple_button_pressed_test(current_therapy_settings.notify.status);
  }
  
  

  current_therapy_settings.notify.status &= clear_wand_errors();

  if (((current_therapy_settings.notify.status & FAULT_MASK_1) > 0) ||
      ((current_therapy_settings.notify.status2 & FAULT_MASK_2) > 0))
  {
    current_therapy_settings.notify.status2 &= ~(CHECK_ELECTRODE_NOTIFICATION_MASK |
                                                 WAND_NOT_COMPATIBLE__NOTIFICATION_MASK |
                                                 GENERIC_ERROR_1_STATUS_2_MASK |
                                                 GENERIC_ERROR_2_STATUS_2_MASK | 
                                                 GENERIC_ERROR_3_STATUS_2_MASK);
  }
  
  /* don't include ambient alarm in priority */
  temp1 = ((current_therapy_settings.notify.status & ~AMBIENT_ALARM_NOTIFICATION_MASK) >> 8);
  temp2 = (current_therapy_settings.notify.status2);

  if (current_therapy_settings.notify.status2 > 0)
  {

    priority += 24;

    do
    {

      priority++;

    }while(temp2 >>= 1);

  }
  else
  {

    do
    {

      priority++;

    }while(temp1 >>= 1);

  }

  temp1 = (current_therapy_settings.notify.status & 0xFFFFF700);
  temp2 = (current_therapy_settings.notify.status2);

  if ((temp1 == 0) && (temp2 == 0))
  {

    priority = 0;

  }

  //current_therapy_settings.notify.status = (notify_txt_map[priority] | temp);
  current_therapy_settings.notify.status = priority | temp1;

  /* both wands connected finger sw force enable */
  if ((legacy_wand_params_obj.wand_connected == 1) &&
      (current_therapy_settings.wand.unlocked == 1))
  {

    if (current_therapy_settings.treatment.finger_switch_en == FALSE)
    {

      current_therapy_settings.treatment.auto_finger_switch_en = TRUE;
      current_therapy_settings.treatment.finger_switch_en = TRUE;

    }
    
    if (current_therapy_settings.treatment.wireless_enable == TRUE)
    {
      
      current_therapy_settings.treatment.auto_wireless_en = TRUE;
      current_therapy_settings.treatment.wireless_enable = FALSE;
      
    }
    /* <<ENT>> uncomment this code for ENT */
    if (current_therapy_settings.controller == ENT)
    {
      current_therapy_settings.notify.status2 |= GENERIC_ERROR_3_STATUS_2_MASK;
    }   

  }
  else 
  {    
    if (current_therapy_settings.treatment.auto_finger_switch_en == TRUE)
    {

      current_therapy_settings.treatment.auto_finger_switch_en = FALSE;
      current_therapy_settings.treatment.finger_switch_en = FALSE;
      
    }
    
    if (current_therapy_settings.treatment.auto_wireless_en == TRUE)
    {
      
      current_therapy_settings.treatment.auto_wireless_en = FALSE;
      current_therapy_settings.treatment.wireless_enable = TRUE;
      
    }

  }

}

/***************************************************************************//**
This function is called to set the wand disconnected mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wand_disconnected_error(void)
{

  current_therapy_settings.notify.status |= WAND_DISCONNECTED_NOTIFICATION_MASK;
  current_therapy_settings.notify.smart_wand_disconnected = 1;
  //smartwand_timeout();

  return 1;

}
/***************************************************************************//**
This function is called to handle dual wand disconnect error
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t two_wand_disconnected_error(void)
{

  current_therapy_settings.notify.status |= WAND_DISCONNECTED_NOTIFICATION_MASK;
  current_therapy_settings.notify.smart_wand_disconnected = 1;
  current_therapy_settings.notify.legacy_wand_disconnected = 1;

  return 1;

}

/***************************************************************************//**
This function is called handle the legacy wand unplugged error
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t legacy_wand_disconnected_error(void)
{  
  
  current_therapy_settings.notify.status |= WAND_DISCONNECTED_NOTIFICATION_MASK;
  current_therapy_settings.notify.legacy_wand_disconnected = 1;
  if (current_therapy_settings.wand.valid == TRUE)
  {
    put_event(&evt_tbl,LW_UNPLUGGED_LOAD_SMARTWAND);
  }
  else
  {
    put_event(&evt_tbl,LW_UNPLUGGED);
  }
  
  return 1;

}

/***************************************************************************//**
This function is called to set the GUI communication fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t gui_com_fault(void)
{


  current_therapy_settings.gui.tx_wdog_err = TRUE;
  current_therapy_settings.notify.status |= SYS_COM_FAULT_MASK;

  /* hold gui in reset */
  //GPIOD_PCOR = GPIO_PDOR_PDO(GUI_RESET_MASK);

  return 1;

}

/***************************************************************************//**
This function is called to set the generator communication fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t gen_com_fault(void)
{


  //current_therapy_settings.gui.tx_wdog_err = TRUE;
  current_therapy_settings.notify.status |= SYS_COM_FAULT_MASK;

  /* hold gui in reset */
  //GPIOD_PCOR = GPIO_PDOR_PDO(GUI_RESET_MASK);

  return 1;

}

/***************************************************************************//**
This function is called to set the wand expired error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wand_expired_error(void)
{

  current_therapy_settings.notify.status |= WAND_REUSE_NOTIFICATION_MASK;
  current_therapy_settings.notify.smart_wand_expired = 1;
  //logerror2wand((uint8_t)WAND_EXPIRED_ERROR_TXT);

  return 1;

}

/***************************************************************************//**
This function is called to set the wand id error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wand_id_error(void)
{

  current_therapy_settings.notify.status |= WAND_ID_READ_NOTIFICATION_MASK;
  current_therapy_settings.notify.smart_wand_id_error = 1;
  //logerror2wand((uint8_t)WAND_ID_ERROR_TXT);

  return 1;

}

/***************************************************************************//**
This function is called to set the wand id error mask for legacy wands. This
function is called if the sw detects bad R1 resistor value.
@param void
@return 0
@outputs returns 1 always
@context
*******************************************************************************/
uint8_t lw_id_error(void)
{
  current_therapy_settings.notify.status |= WAND_ID_READ_NOTIFICATION_MASK;
  current_therapy_settings.notify.legacy_wand_id_error = 1;
  return 1;
}

/***************************************************************************//**
This function is called to set the wand id error mask for legacy wands. This
function is called if the sw detects bad R1 resistor value.
@param void
@return 0
@outputs returns 1 always
@context
*******************************************************************************/
uint8_t lw_incompatible_error(void)
{
  current_therapy_settings.notify.status2 |= WAND_NOT_COMPATIBLE__NOTIFICATION_MASK;
  current_therapy_settings.notify.legacy_wand_incompatible_error = 1;
  return 1;
}

/***************************************************************************//**
This function is called to set the wand not compatible error mask for smart
wands. This function is called if ENT indication is detected.
@param void
@return 0
@outputs returns 1 always
@context
*******************************************************************************/
uint8_t smart_wand_incompatible_error(void)
{
  current_therapy_settings.notify.status2 |= WAND_NOT_COMPATIBLE__NOTIFICATION_MASK;
  current_therapy_settings.notify.smart_wand_incompatible_error = 1;
  return 1;
}

/***************************************************************************//**
This function is called to set the wand use limit error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wand_use_limit_error(void)
{

  current_therapy_settings.notify.status |= WAND_USE_LIMIT_LIFE_EXPIRED_NOTIFICATION_MASK;
  current_therapy_settings.notify.smart_wand_use_limit = 1;
  //logerror2wand((uint8_t)WAND_LIFE_ERROR_TXT);

  return 1;

}

/***************************************************************************//**
This function is called to set the GUI communication error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t can_timeout(void)
{

  //static uint8_t wdog_err_count = 0;

  current_therapy_settings.gui.rx_wdog.wdog_err_cnt++;

  if (current_therapy_settings.gui.rx_wdog.wdog_err_cnt >= 20)
  {

    gui_com_fault();
    /* hold gui in reset */
    GPIOD_PCOR = GPIO_PDOR_PDO(GUI_RESET_MASK);
    current_therapy_settings.gui.rx_wdog.wdog_err_cnt = 0;
    return 1;

  }
  else
  {

    PIT_TFLG1 |= PIT_TFLG_TIF_MASK;
    PIT_TCTRL1 &= ~PIT_TCTRL_TEN_MASK;
    PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK;
    return 0;

  }

}

/***************************************************************************//**
This function is called to set the wand short error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
void wand_short_error(void)
{   
    if (smart_wand_peak_hit_cnt > 15)
    {
            if (smart_wand_consecutive_shorts < 4)
            {
                    smart_wand_consecutive_shorts++;
            }
            else
            {
                    current_therapy_settings.notify.status |= WAND_SHORTED_NOTIFICATION_MASK;
                    current_therapy_settings.notify.smart_wand_short = 1;
                    smart_wand_consecutive_shorts = 0;
                    put_event(&evt_tbl,THERAPY_OFF);
                    cbtmr_unreg_callback(SmartWandShortId);
                    smart_wand_short_tracker_f = 0;
            }
    }
    else
    {
            smart_wand_consecutive_shorts = 0;
            cbtmr_unreg_callback(SmartWandShortId);
            smart_wand_short_tracker_f = 0;
    }
    smart_wand_peak_hit_cnt = 0;
}





/***************************************************************************//**
This function is called to set the suction tube error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t suction_tube_error(void)
{

  current_therapy_settings.notify.status |= TUBE_ENGAGEMENT_NOTIFICATION_MASK;


  return 1;

}

/***************************************************************************//**
This function is called to set the internal temperature error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t internal_temp_error(void)
{

  current_therapy_settings.notify.status |= INTERNAL_TEMPERATURE_NOTIFICATION_MASK;

  return 1;

}

/***************************************************************************//**
This function is called to clear the internal temperature error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t clear_internal_temp_error(void)
{

  current_therapy_settings.notify.status &= ~INTERNAL_TEMPERATURE_NOTIFICATION_MASK;

  return 1;

}

/***************************************************************************//**
This function is called to set the ADC boundary error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t adc_boundary_fault(void)
{

  current_therapy_settings.notify.status |= ADC_BOUNDARY_FAULT_MASK;

  return 1;

}

/***************************************************************************//**
This function is called to set the CRC fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t crc_fault(void)
{

  current_therapy_settings.notify.status |= MEMORY_FAULT_MASK;

  put_event(&evt_tbl,CRC_FAULT);

  return 1;

}

/***************************************************************************//**
This function is called to set the wand switch stuck fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wand_switch_stuck_fault(void)
{

  current_therapy_settings.notify.status |= SWITCH_STUCK_FAULT_MASK;

  current_therapy_settings.notify.smart_wand_stuck_switch = 1;

  return 1;

}

/***************************************************************************//**
This function is called to set the footpedal switch stuck fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t fp_switch_stuck_fault(void)
{

  current_therapy_settings.notify.status |= SWITCH_STUCK_FAULT_MASK;

  current_therapy_settings.notify.fp_switch_stuck = 1;

  return 1;

}

/***************************************************************************//**
This function is called to set the pump fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t pump_fault(void)
{

  current_therapy_settings.notify.status |= PUMP_FAULT_MASK;

  DISABLE_PIT(0);

  return 1;

}

/***************************************************************************//**
This function is called to set vmod voltage fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t vmod_fault(void)
{

  current_therapy_settings.notify.status |= VMOD_FAULT_MASK;

  return 1;

}

/***************************************************************************//**
This function is called to set the system over temperature fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t internal_temp_fault(void)
{

  current_therapy_settings.notify.status |= SYS_OVERTEMP_FAULT_MASK;

  return 1;

}

/***************************************************************************//**
This function is called to set the watch dog fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wdog_fault(void)
{

  current_therapy_settings.notify.status |= WDOG_COP_FAULT_MASK;

  return 1;

}

/***************************************************************************//**
This function is called during a initial step of a fault handler. It turns off
the high voltage section of the controller.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t fault_init(void)
{

  DISABLE_AUDIO;
  /* Set Period */
  //FTM2_MOD = AUDIO_PWM_MODULO-1;
  /* turn off high voltage */
  high_volt_off();
  /* the pump is now off */
  current_therapy_settings.pump.on = 0;

  return 1;

}

/***************************************************************************//**
This function is called to set serial number fault mask.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t date_time_fault(void)
{

  current_therapy_settings.notify.status |= SERIAL_NUMBER_FAULT_MASK;

  return 1;

}

/***************************************************************************//**
This function is called to set date time fault mask.
@todo the name and the mask are different
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t serial_num_fault(void)
{

  current_therapy_settings.notify.status |= DATE_TIME_FAULT_MASK;

  return 1;

}

/***************************************************************************//**
This function is called to reset all the errors
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t reset_error(void)
{

  uint8_t temp = 0;

  const uint32_t reset_mask[33] = {
    /* 0  */ 0,
    /* 1  */ WAND_SHORTED_NOTIFICATION_MASK,
    /* 2  */ TUBE_ENGAGEMENT_NOTIFICATION_MASK,
    /* 3  */ 0,
    /* 4  */ AMBIENT_ALARM_NOTIFICATION_MASK,
    /* 5  */ MULTIPLE_BUTTONS_NOTIFICATION_MASK,
    /* 6  */ WAND_DISCONNECTED_NOTIFICATION_MASK,
    /* 7  */ 0,
    /* 8  */ AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK,
    /* 9  */ INTERNAL_TEMPERATURE_NOTIFICATION_MASK,
    /* 10  */ WAND_ID_READ_NOTIFICATION_MASK,
    /* 11  */ 0,
    /* 12  */ 0,
    /* 13  */ 0,
    /* 14  */ 0,
    /* 15  */ 0,
    /* 16  */ 0,
    /* 17  */ 0,
    /* 18  */ 0,
    /* 19  */ 0,
    /* 20  */ 0,
    /* 21  */ 0,
    /* 22  */ 0,
    /* 23  */ 0,
    /* 24  */ 0,
    /* 25  */ CHECK_ELECTRODE_NOTIFICATION_MASK,
    /* 26  */ 0,
    /* 27  */ GENERIC_ERROR_1_STATUS_2_MASK,
    /* 28  */ GENERIC_ERROR_2_STATUS_2_MASK,
    /* 29  */ 0,
    /* 30  */ 0,
    /* 31  */ 0,
    /* 32  */ 0,

  };

  temp = (current_therapy_settings.notify.status & 0xFF);

  if (temp > WAIT_FOR_RINSE_ERROR_TXT)
  {

    current_therapy_settings.notify.status2 &= ~reset_mask[temp];

  }
  else
  {

    current_therapy_settings.notify.status &= ~reset_mask[temp];

  }

  if (reset_mask[temp] == WAND_SHORTED_NOTIFICATION_MASK)
  {
    
    current_therapy_settings.notify.smart_wand_short = 0;
    current_therapy_settings.notify.legacy_wand_short = 0;
    
  } 
  else if (reset_mask[temp] == AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK)
  {
  
    current_therapy_settings.notify.smart_wand_no_ambient = 0;
    current_therapy_settings.notify.legacy_wand_no_ambient = 0;
  
  }
  else if (reset_mask[temp] == WAND_ID_READ_NOTIFICATION_MASK)
  {
    
    current_therapy_settings.notify.smart_wand_id_error = 0;
    current_therapy_settings.notify.legacy_wand_id_error = 0;
    
  }
      
  if (reset_mask[temp]==WAND_ID_READ_NOTIFICATION_MASK)
  {

    put_event(&evt_tbl,WAND_UNPLUGGED);

  }

  return 0;

}

/***************************************************************************//**
This function is called to set the ambient sensor flag to 1
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t no_ambient_error(void)
{
//  smart_wand_common_params.ambient_sensor_flag = 1;
  current_therapy_settings.notify.status |= 
      AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
    current_therapy_settings.notify.smart_wand_no_ambient = 1;
  return 1;

}

/***************************************************************************//**
This function is called to check for electrode error
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t check_electrode_error(void)
{

  uint8_t assert_error = 0;
  

    current_therapy_settings.wand.worn_latch = 0;

    //if (current_therapy_settings.wand.worn == 0)
    //{

      current_therapy_settings.notify.status2 |= CHECK_ELECTRODE_NOTIFICATION_MASK;
      //assert_error = 1;

    //}
    
//    if (current_therapy_settings.wand.worn > 0)
//    {
//      
//      memcpy(&current_therapy_settings.wand.worn_timestamp[0],
//      &current_therapy_settings.wand.worn_timestamp[1],
//      sizeof(uint32_t)*3);
//
//      current_therapy_settings.wand.worn_timestamp[3] = RTC_TSR;
//  
//    }
//
//    if (current_therapy_settings.wand.worn < CHAR_MAX)
//    {
//
//      current_therapy_settings.wand.worn++;
//
//    }

    /* reset wear monitors */
    current_therapy_settings.wand.irms_wear = (bool)wand_irms_wear_monitor(1,0);
    current_therapy_settings.wand.thandle_wear = (bool)wand_thandle_wear_monitor(0,1,0);

    #ifdef DEBUG_CAN

    //CANdebug.wand.worn_count = current_therapy_settings.wand.worn;

    #endif

  current_therapy_settings.wand.worn_timer_en = 1;
  current_therapy_settings.wand.worn_active_time = 0;
    
  

  return assert_error;

}

/***************************************************************************//**
This function is checks for legacy wand short error
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t legacy_wand_short_error(void)
{

  static uint32_t time_stamp = 0;
  static uint8_t peak_hit_counter = 0;
  uint8_t error = 0;
  const uint8_t led_blink_duration_secs = 5; 
  const uint8_t consecutive_short_error_limit = 20;
  const uint8_t wand_short_threshold = 14;
  const uint8_t consecutive_short_warn_threshold = 4;
  static uint8_t consecutive_short = 0;
  uint32_t rtc_tsr = RTC_TSR;
  uint32_t elapsed_time = rtc_tsr - time_stamp;
  
  /* previous peak hit occured less than 1 second ago so start counting */
  if (elapsed_time == 0)
  {  
    /* 16Hz required to equal threshold; only one threshold breach per sec */
    if (peak_hit_counter++ == wand_short_threshold)
    {
      /* warn after 5 consecutive or extend if already blinking ie hysteresis */
      if ((consecutive_short++ >= consecutive_short_warn_threshold) ||
          (current_therapy_settings.notify.legacy_wand_short_led_sec_countdown > 0))
      {
        current_therapy_settings.notify.legacy_wand_short_led_sec_countdown =
          led_blink_duration_secs;
      }
    }
  }
  else
  {
    /* new 1s window w/o threshold breach means non-consecutive */
    if ((peak_hit_counter <= wand_short_threshold) ||
        (elapsed_time > 1))
    {
      consecutive_short = 0;
    }
    peak_hit_counter = 0;
    /* time stamp first peak hit of new 1s window */
    time_stamp = rtc_tsr;
  }
  
  /* defensive programming--not practical to get more than ~32Hz */
  if (peak_hit_counter >= 254)
  {
    peak_hit_counter = 254;
  }
     
  if(consecutive_short>=consecutive_short_error_limit)
  {
    current_therapy_settings.notify.status |= WAND_SHORTED_NOTIFICATION_MASK;
    error = 1;
    current_therapy_settings.notify.legacy_wand_short = 1;
    current_therapy_settings.notify.legacy_wand_short_led_sec_countdown = 0;
    consecutive_short = 0;
    peak_hit_counter = 0;
  }

  return error;
}

/***************************************************************************//**
Decodes the wand error code
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t wand_error_type_decipher(NOTIFY_TXT error)
{
  
  enum types_of_wand_errors {NO_WAND_ERROR,
                             SMART_WAND_ERROR,
                             LEGACY_WAND_ERROR,
                             TWO_WAND_ERROR};
  
  uint8_t wand_error_type = (uint8_t) NO_WAND_ERROR;
    
  switch(error)
  {

    case(WAND_SHORT_ERROR_TXT):
    {
      
      if (current_therapy_settings.notify.smart_wand_short == 1)
      {
        
        wand_error_type |= SMART_WAND_ERROR;
        
      }
      
      if (current_therapy_settings.notify.legacy_wand_short == 1)
      {
        
        wand_error_type |= LEGACY_WAND_ERROR;
        
      }
              
      break;
      
    }
    case(WAND_DISCONNECT_ERROR_TXT):
    {
      
      if (current_therapy_settings.notify.smart_wand_disconnected == 1)
      {
        
        wand_error_type |= SMART_WAND_ERROR;
        
      }
      
      if (current_therapy_settings.notify.legacy_wand_disconnected == 1)
      {
        
        wand_error_type |= LEGACY_WAND_ERROR;
        
      }
      
      if ((current_therapy_settings.notify.legacy_wand_disconnected == 0) &&
          (current_therapy_settings.notify.smart_wand_disconnected == 0))
      {
        
        wand_error_type |= TWO_WAND_ERROR;
        
      }
              
      break;
            
    }
    case(WAND_LIFE_ERROR_TXT):
    {
      
      if (current_therapy_settings.notify.smart_wand_use_limit == 1)
      {
        
        wand_error_type |= SMART_WAND_ERROR;
        
      }
      
      if (current_therapy_settings.notify.legacy_wand_use_limit == 1)
      {
        
        wand_error_type |= LEGACY_WAND_ERROR;

      }
  
      break;
      
    }

    case(AMBIENT_ERROR_TXT):
    {

      if (current_therapy_settings.notify.smart_wand_no_ambient == 1)
      {
        
        wand_error_type |= SMART_WAND_ERROR;
        
      }
      
      if (current_therapy_settings.notify.legacy_wand_no_ambient == 1)
      {
        
        wand_error_type |= LEGACY_WAND_ERROR;
        
      }
  
      break;
      
    }

    case(WAND_ID_ERROR_TXT):
    {

      if (current_therapy_settings.notify.smart_wand_id_error == 1)
      {
        
        wand_error_type |= SMART_WAND_ERROR;
        
      }
      
      if (current_therapy_settings.notify.legacy_wand_id_error == 1)
      {
        
        wand_error_type |= LEGACY_WAND_ERROR;
        
      }
  
      break;
      
    }

    case(WAND_EXPIRED_ERROR_TXT):
    {
      
      if (current_therapy_settings.notify.smart_wand_expired == 1)
      {
        
        wand_error_type |= SMART_WAND_ERROR;
        
      }
      
      if (current_therapy_settings.notify.legacy_wand_expired == 1)
      {
        
        wand_error_type |= LEGACY_WAND_ERROR;
        
      }
        
      break;
      
    }
    case(SWITCH_STUCK_FAULT_TXT):
    {
      
      if (current_therapy_settings.notify.smart_wand_stuck_switch == 1)
      {
        
        wand_error_type |= SMART_WAND_ERROR;
        
      }
      
      if (current_therapy_settings.notify.legacy_wand_stuck_switch == 1)
      {
        
        wand_error_type |= LEGACY_WAND_ERROR;
        
      }
        
      break;
      
    }
    case(CHECK_ELECTRODE_ERROR_TXT):
    {
      
      wand_error_type |= SMART_WAND_ERROR;
      
      break;
      
    }

    case(WAND_NOT_COMPATIBLE_ERROR_TXT):
    {
      
      wand_error_type |= LEGACY_WAND_ERROR;
        
      break;
      
    }
    
    case (TWO_WANDS_CONNECTED_TXT):
    {

      wand_error_type |= TWO_WAND_ERROR;
      
      break;
      
    }
 
  }
  
  return wand_error_type;
  
}

/***************************************************************************//**
Clears the wand errors. This function is called when the wand is unplugged
@param void
@return void
@outputs
@context
*******************************************************************************/
uint32_t clear_wand_errors(void)
{
    
  uint32_t error_reset_mask = 0;

  /* errors that clear automatically when wand is unplugged */
  if (current_therapy_settings.wand.unlocked == FALSE)
  {

    if ((current_therapy_settings.notify.smart_wand_no_ambient == 1) &&
        (current_therapy_settings.notify.legacy_wand_no_ambient == 0))
    {
      
      error_reset_mask |= AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
      
    }
    
    if ((current_therapy_settings.notify.smart_wand_id_error == 1) &&
        (current_therapy_settings.notify.legacy_wand_id_error == 0))
    {
      
      error_reset_mask |= WAND_ID_READ_NOTIFICATION_MASK;
      
    }

    if ((current_therapy_settings.notify.smart_wand_expired == 1) &&
        (current_therapy_settings.notify.legacy_wand_expired == 0))
    {
      
      error_reset_mask |= WAND_REUSE_NOTIFICATION_MASK;
      
    }
    
    if ((current_therapy_settings.notify.smart_wand_use_limit == 1) &&
        (current_therapy_settings.notify.legacy_wand_use_limit == 0))
    {
      
      error_reset_mask |= WAND_USE_LIMIT_LIFE_EXPIRED_NOTIFICATION_MASK;
      
    }
    
    current_therapy_settings.notify.status2 &= ~GENERIC_ERROR_3_STATUS_2_MASK;   
    current_therapy_settings.notify.smart_wand_no_ambient = 0;
    current_therapy_settings.notify.smart_wand_id_error = 0;
    current_therapy_settings.notify.smart_wand_expired = 0;
    current_therapy_settings.notify.smart_wand_use_limit = 0;
    current_therapy_settings.notify.legacy_wand_incompatible_error = 0;
      
  }
  else if (current_therapy_settings.notify.smart_wand_disconnected == 1)
  {        
   
    error_reset_mask |= WAND_DISCONNECTED_NOTIFICATION_MASK;
    current_therapy_settings.notify.smart_wand_disconnected = 0;

  } 
  
  if (legacy_wand_params_obj.wand_connected == 0)
  {

    if ((current_therapy_settings.notify.legacy_wand_no_ambient == 1) &&
        (current_therapy_settings.notify.smart_wand_no_ambient == 0))
    {
      
      error_reset_mask |= AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
      
    }
    
    if ((current_therapy_settings.notify.legacy_wand_id_error == 1) &&
        (current_therapy_settings.notify.smart_wand_id_error == 0))
    {
      
      error_reset_mask |= WAND_ID_READ_NOTIFICATION_MASK;
      
    }

    if ((current_therapy_settings.notify.legacy_wand_expired == 1) &&
        (current_therapy_settings.notify.smart_wand_expired == 0))
    {
      
      error_reset_mask |= WAND_REUSE_NOTIFICATION_MASK;
      
    }
    
    if ((current_therapy_settings.notify.legacy_wand_use_limit == 1) &&
        (current_therapy_settings.notify.smart_wand_use_limit == 0))
    {
      
      error_reset_mask |= WAND_USE_LIMIT_LIFE_EXPIRED_NOTIFICATION_MASK;
      
    }
    current_therapy_settings.notify.status2 &= ~(WAND_NOT_COMPATIBLE__NOTIFICATION_MASK |
                                                 GENERIC_ERROR_3_STATUS_2_MASK);
    current_therapy_settings.notify.legacy_wand_incompatible_error = 0;

    current_therapy_settings.notify.legacy_wand_no_ambient = 0;
    current_therapy_settings.notify.legacy_wand_id_error = 0;
    current_therapy_settings.notify.legacy_wand_expired = 0;
    current_therapy_settings.notify.legacy_wand_use_limit = 0;

    
  }
  else if (current_therapy_settings.notify.legacy_wand_disconnected == 1)
  {
    
    error_reset_mask |= WAND_DISCONNECTED_NOTIFICATION_MASK;
    current_therapy_settings.notify.legacy_wand_disconnected = 0;

  }  
        
  return ~error_reset_mask;

}

uint32_t multiple_button_pressed_test(uint32_t notify_status)
{

  uint8_t flag = 0,
                  wand_button_flag = 0,
                  foot_switch_flag = 0;
  uint32_t error = notify_status;
  static u8 timer_5s_id = 0;
  static u8 timer_100ms_id = 0;
  static FlagStatus timer_5s_expired_f = RESET;
  static FlagStatus timer_100ms_expired_f = RESET;
  static uint8_t timer_5s_f = 0;
  static uint8_t timer_100ms_f = 0;
  static uint8_t ManualPrimeRunning = 0;
  /*
  @ENT
  check if running manual prime is active
  this will count as a multi button press
  */
  
  if(current_therapy_settings.active_wand.smart_wand == ARTHROPLASTY)
  {
    ManualPrimeRunning = (current_therapy_settings.gui.graphics.GraphicNumber == IV_BAG_AUTO) ? 1 : 0;
  }
  else
  {
    ManualPrimeRunning = get_priming_status() == (PRIME_MANUAL) ? 1 : 0;
  }

  /* test whether more than one input pressed */
  wand_button_flag = current_therapy_settings.wand.coag_key +
                                          current_therapy_settings.wand.cut_key +
                                          current_therapy_settings.wand.mode_key +
                                          current_therapy_settings.legacy_wand_levels.coag_key +
                                          current_therapy_settings.legacy_wand_levels.cut_key +
                                          current_therapy_settings.legacy_wand_levels.mode_key;

  /* all 3 footpedals pressed probably means power failure */
  foot_switch_flag = current_therapy_settings.footpedal.coag +
                                          current_therapy_settings.footpedal.ablate +
                                          current_therapy_settings.footpedal.mode;
  flag = wand_button_flag + foot_switch_flag + ManualPrimeRunning;

  /* more than 1 switch pressed. all 3 footpedals is probably power failure rather than error */
  if ((flag > 1) && (foot_switch_flag < 3))
  {
    if ((error & MULTIPLE_BUTTONS_NOTIFICATION_MASK) != MULTIPLE_BUTTONS_NOTIFICATION_MASK)
    {
      if (ManualPrimeRunning)
      {
        pump_set_speed(lw_get_pump_speed(0));
        //prime.release_detected = TRUE;
        PrimeId = 0U;
        AutoPrimeId = 0U;
        AutoPrimeTimeoutId = 0U;
        PrimeState = PRIME_OFF;
        current_therapy_settings.treatment.pre_prime = FALSE;
        current_therapy_settings.treatment.prime_glow = FALSE;
        prime.blink = FALSE;
        current_therapy_settings.gui.graphics.GraphicNumber = NULL_GRAPHIC_ID;
      }
      put_event(&evt_tbl,THERAPY_OFF);
      put_event(&evt_tbl,LW_ABLATE_OFF_EVENT);
    }

    error |= MULTIPLE_BUTTONS_NOTIFICATION_MASK;

    /* 100ms timer already running so it will restart once 2+ switches are released */
    if (timer_100ms_f == 1)
    {
      cbtmr_unreg_callback(timer_100ms_id);
      timer_100ms_expired_f = RESET;
      timer_100ms_f = 0;
    }

    /* 5s timer already running so it will restart */
    if (timer_5s_f == 1)
    {
      cbtmr_unreg_callback(timer_5s_id);
      timer_5s_expired_f = RESET;
      timer_5s_f = 0;
    }
  }
  else if ((error & MULTIPLE_BUTTONS_NOTIFICATION_MASK) == MULTIPLE_BUTTONS_NOTIFICATION_MASK)
  {
    /* 5s timer expired so clear the error */
    if (timer_5s_expired_f == SET)
    {
      error &= ~MULTIPLE_BUTTONS_NOTIFICATION_MASK;
    }
    /* 5s timer is not running so start it */
    else if (timer_5s_f == 0)
    {
      timer_5s_expired_f = RESET;
      (void)cbtmr_reg_callback(NULL,
                  &timer_5s_expired_f,
                  5000,
                  SINGLE_CBTMR,
                  &timer_5s_id);
                  timer_5s_f = 1;
    }

    /* only 1 switch is pressed */
    if (flag == 1)
    {
      /* 100ms timer expired so clear the error */
      if (timer_100ms_expired_f == SET)
      {
        error &= ~MULTIPLE_BUTTONS_NOTIFICATION_MASK;
      }
      /* 100ms timer is running so it will restart once switch is released */
      else if (timer_100ms_f == 1)
      {
        cbtmr_unreg_callback(timer_100ms_id);
        timer_100ms_expired_f = RESET;
        timer_100ms_f = 0;
      }
    }
    /* no switches are currently pressed */
    else
    {
      /* 100ms timer is not running so start it */
      if (timer_100ms_f == 0)
      {
        timer_100ms_expired_f = RESET;
        /* 80ms is between 75-125ms tol & buys time for delays/debouncing */
        (void)cbtmr_reg_callback(NULL,
                    &timer_100ms_expired_f,
                    80,
                    SINGLE_CBTMR,
                    &timer_100ms_id);
        timer_100ms_f = 1;
      }
    }
  }

  /* once the error is clear stop any remaining timers */
  if ((error & MULTIPLE_BUTTONS_NOTIFICATION_MASK) != MULTIPLE_BUTTONS_NOTIFICATION_MASK)
  {
    if (timer_100ms_f == 1)
    {
      cbtmr_unreg_callback(timer_100ms_id);
      timer_100ms_f = 0;
      timer_100ms_expired_f = RESET;
    }

    if (timer_5s_f == 1)
    {
      cbtmr_unreg_callback(timer_5s_id);
      timer_5s_f = 0;
      timer_5s_expired_f = RESET;
    }
  }

  return error;
  
}

uint8_t ui_fault(void)
{
  
  current_therapy_settings.notify.status2 |= UI_FAULT_MASK;
  
  return 1;
    
}

/***************************************************************************//**
This function is called to set the wand short error mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t peak_hit_cntr(void)
{

  if (smart_wand_short_tracker_f == 0)
  {
    (void)cbtmr_reg_callback(wand_short_error,
                             NULL,
                             1000,
                             CONTINUOUS_CBTMR,
                             &SmartWandShortId);
    smart_wand_short_tracker_f = 1;
  }

  if (smart_wand_peak_hit_cnt <= 15)
  {
    smart_wand_peak_hit_cnt++;
  }

  if ((smart_wand_peak_hit_cnt > 15) && 
      (smart_wand_consecutive_shorts >= 4))
  {
    wand_short_error();
  }
  
  return 0;

}
  
