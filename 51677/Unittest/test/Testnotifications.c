#include "unity.h"
#include "notifications.h"
#include "common.h"
#include "Mockaudio.h"
#include "MK26F18.h"
#include "Mockevent_buffer.h"
#include "Mocksystem.h"
#include "Mockpsu.h"
#include "Mockpit.h"
#include "Mocklegacy_wand.h"
#include "Mockwand.h"
#include "Mockgui.h"
#include "Mocksystick.h"
#include "Mockpump.h"

GEN_SETTINGS current_therapy_settings;
event_table evt_tbl;
legacy_wand_params legacy_wand_params_obj;
COMMON_WAND_PARAMS smart_wand_common_params;
SMART_WAND_PARAMETERS smart_wand_params;
extern uint8_t smart_wand_peak_hit_cnt;
extern uint8_t smart_wand_short_tracker_f;
extern uint8_t smart_wand_consecutive_shorts;
manta_prime prime;
u8 PrimeId;
prime_mode_t PrimeState;
u8 AutoPrimeId;
u8 AutoPrimeTimeoutId;

#define DEBUG_CAN 0

enum types_of_wand_errors {NO_WAND_ERROR,
SMART_WAND_ERROR,
LEGACY_WAND_ERROR,
TWO_WAND_ERROR};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_notification_manager (void)
{
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.ambient.temperature = 50;
   current_therapy_settings.ambient.alarm_threshold = 25;
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.ambient.capable = 1;
   current_therapy_settings.ambient.enable = 1;

   notification_manager();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.ambient.alarm_active);
   current_therapy_settings.wand.unlocked = 0;

   current_therapy_settings.ambient.enable =0;
   current_therapy_settings.ambient.alarm_active = 1;

   notification_manager();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.ambient.alarm_active);

   /**
    *current_therapy_settings.treatment.ablate_on = 1;
    *current_therapy_settings.wand.tube_warn_pulse = TRUE;
    *current_therapy_settings.pump.ctrl_en = TRUE;
    *put_event_ExpectAndReturn(&evt_tbl,RF_OFF,1);
    *notification_manager();
    */


   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.wand.tube_warn = TRUE;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_alert = FALSE;
   notification_manager();
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.pump.ctrl_en);

   current_therapy_settings.wand.coag_key = 1;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.footpedal.ablate =1;
   current_therapy_settings.wand.cut_key = 1;
   current_therapy_settings.notify.status &= ~MULTIPLE_BUTTONS_NOTIFICATION_MASK;
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   //put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,1);
   notification_manager();

//   current_therapy_settings.footpedal.mode = 1;
//   current_therapy_settings.wand.unlocked = FALSE;
     notification_manager();
//   TEST_ASSERT_EQUAL(WAND_DISCONNECTED_NOTIFICATION_MASK, (current_therapy_settings.notify.status&WAND_DISCONNECTED_NOTIFICATION_MASK)) ;

}

void test_gui_com_fault (void)
{
   current_therapy_settings.gui.tx_wdog_err = FALSE;
   uint8_t return_value = gui_com_fault();
   TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.gui.tx_wdog_err);
}

void test_reset_error (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.notify.status  = 0;
   uint8_t return_value = no_ambient_error();
   notification_manager();
   uint8_t return_value2 = reset_error();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.notify.status&AMBIENT_ALARM_NOTIFICATION_MASK);
}

void test_reset_error_2 (void)
{
   current_therapy_settings.notify.status  = 0xA;
   put_event_ExpectAndReturn (&evt_tbl,WAND_UNPLUGGED,1);
   uint8_t return_value2 = reset_error();
}



void test_suction_tube_error (void)
{
   current_therapy_settings.notify.status  = 0;
   uint8_t return_value = suction_tube_error();
   TEST_ASSERT_EQUAL(TUBE_ENGAGEMENT_NOTIFICATION_MASK, current_therapy_settings.notify.status & TUBE_ENGAGEMENT_NOTIFICATION_MASK);
}

void test_no_ambient_error(void)
{
   current_therapy_settings.notify.status &= ~ AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
   uint8_t return_value = no_ambient_error();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_clear_internal_temp_error(void)
{
   current_therapy_settings.notify.status  |= INTERNAL_TEMPERATURE_NOTIFICATION_MASK;
   uint8_t return_value = clear_internal_temp_error();
   TEST_ASSERT_EQUAL(0, current_therapy_settings.notify.status&INTERNAL_TEMPERATURE_NOTIFICATION_MASK);
}

void test_internal_temp_error(void)
{
   current_therapy_settings.notify.status  = 0;
   uint8_t return_value = internal_temp_error();
   TEST_ASSERT_EQUAL(INTERNAL_TEMPERATURE_NOTIFICATION_MASK, current_therapy_settings.notify.status&INTERNAL_TEMPERATURE_NOTIFICATION_MASK);
}

void test_adc_boundary_fault (void)
{
   current_therapy_settings.notify.status &= ~ADC_BOUNDARY_FAULT_MASK;
   uint8_t return_value =  adc_boundary_fault();
   TEST_ASSERT_EQUAL(ADC_BOUNDARY_FAULT_MASK, current_therapy_settings.notify.status&ADC_BOUNDARY_FAULT_MASK);
}

void test_crc_fault(void)
{
   put_event_ExpectAndReturn(&evt_tbl,CRC_FAULT,1);
   uint8_t return_value = crc_fault();
}

void test_wand_switch_stuck_fault(void)
{
   current_therapy_settings.notify.status &= ~SWITCH_STUCK_FAULT_MASK;
   uint8_t return_value = wand_switch_stuck_fault();
   //TEST_ASSERT_EQUAL(1,current_therapy_settings.notify.wand_switch_stuck);
   // Need test
}

void test_fp_switch_stuck_fault(void)
{
   current_therapy_settings.notify.status &= ~SWITCH_STUCK_FAULT_MASK;
   uint8_t return_value = fp_switch_stuck_fault();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.notify.fp_switch_stuck);
}

void test_pump_fault(void)
{
   current_therapy_settings.notify.status &= ~PUMP_FAULT_MASK;
   uint8_t return_value = pump_fault();
   TEST_ASSERT_EQUAL(PUMP_FAULT_MASK,current_therapy_settings.notify.status & PUMP_FAULT_MASK);
}

void test_vmod_fault(void)
{
   current_therapy_settings.notify.status &= ~VMOD_FAULT_MASK;
   uint8_t return_value = vmod_fault();
   TEST_ASSERT_EQUAL(VMOD_FAULT_MASK,current_therapy_settings.notify.status& VMOD_FAULT_MASK);
}

void test_internal_temp_fault(void)
{
   current_therapy_settings.notify.status &= ~SYS_OVERTEMP_FAULT_MASK;
   uint8_t return_value =  internal_temp_fault();
   TEST_ASSERT_EQUAL(SYS_OVERTEMP_FAULT_MASK,current_therapy_settings.notify.status&SYS_OVERTEMP_FAULT_MASK);
}

void test_can_timeout(void)
{
   uint8_t return_value;
   current_therapy_settings.gui.tx_wdog_err = FALSE;

   for (uint8_t i = 0 ; i < 30; i++)
   {
      return_value =can_timeout();
   }
   TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.gui.tx_wdog_err);
}

void test_notification_manager_2 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.ambient.temperature =  90;
   current_therapy_settings.ambient.alarm_threshold = 100;
   notification_manager();
}

void test_notification_manager_3 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.ambient.temperature =  100;
   current_therapy_settings.ambient.alarm_threshold = 90;
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.ambient.capable = 0;
   notification_manager();
}


void test_notification_manager_4 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.ambient.temperature =  90;
   current_therapy_settings.ambient.alarm_threshold = 100;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.ambient.capable = 0;
   notification_manager();
}

void test_notification_manager_5 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.ambient.temperature =  90;
   current_therapy_settings.ambient.alarm_threshold = 100;
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.ambient.enable = 1;
   current_therapy_settings.ambient.capable = 0;
   notification_manager();
}

void test_notification_manager_6 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_alert = TRUE;
   current_therapy_settings.pump.ctrl_en = TRUE;
   notification_manager();
}

void test_notification_manager_7 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.wand.tube_warn_pulse = TRUE;
   current_therapy_settings.wand.tube_alert = TRUE;
   current_therapy_settings.pump.ctrl_en = FALSE;
   notification_manager();
}

void test_notification_manager_8 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_alert = FALSE;
   current_therapy_settings.pump.ctrl_en = TRUE;
   notification_manager();
}

void test_notification_manager_9 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_alert = FALSE;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.wand.tube_warn = FALSE;
   notification_manager();
}

void test_notification_manager_10 (void)
{
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_alert = FALSE;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.wand.tube_warn = FALSE;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.mode = 1;
   current_therapy_settings.wand.unlocked = TRUE;
   notification_manager();
}

void test_notification_manager_11 (void)
{
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
   //put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_alert = FALSE;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.wand.tube_warn = FALSE;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.footpedal.mode = 1;
   current_therapy_settings.wand.unlocked = FALSE;
   notification_manager();
}

void test_notification_manager_12 (void)
{
   //put_event_IgnoreAndReturn(1);
   //   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
   //put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_alert = FALSE;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.wand.tube_warn = FALSE;
   current_therapy_settings.footpedal.ablate = 1;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.mode = 1;
   current_therapy_settings.wand.unlocked = TRUE;
   notification_manager();
}

void test_date_time_fault(void)
{
   uint8_t return_value = date_time_fault();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_wdog_fault(void)
{
   uint8_t return_value = wdog_fault();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_serial_num_fault(void)
{
   uint8_t return_value = serial_num_fault();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_fault_init(void)
{
   high_volt_off_ExpectAndReturn(1);
   uint8_t return_value = fault_init();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_wand_disconnected_error(void)
{
   uint8_t return_value = wand_disconnected_error();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_wand_expired_error(void)
{
   uint8_t return_value = wand_expired_error();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_wand_id_error(void)
{
   uint8_t return_value = wand_id_error();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_wand_use_limit_error(void)
{
   uint8_t return_value = wand_use_limit_error();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_lw_incompatible_error(void)
{
   uint8_t return_value = lw_incompatible_error();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_smart_wand_incompatible_error(void)
{
	current_therapy_settings.notify.status2 = 0;
   uint8_t return_value = smart_wand_incompatible_error();
   TEST_ASSERT_EQUAL(1,return_value);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.notify.smart_wand_incompatible_error);
   TEST_ASSERT_EQUAL(WAND_NOT_COMPATIBLE__NOTIFICATION_MASK,current_therapy_settings.notify.status2);
}

void test_lw_id_error(void)
{
   uint8_t return_value = lw_id_error();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_legacy_wand_disconnected_error(void)
{
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,0);
   uint8_t return_value = legacy_wand_disconnected_error();
   TEST_ASSERT_EQUAL(1,return_value);
   
   current_therapy_settings.wand.valid = TRUE;
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED_LOAD_SMARTWAND,0);
   uint8_t return_value1 = legacy_wand_disconnected_error();
   TEST_ASSERT_EQUAL(1,return_value1);
}

void test_legacy_wand_short_error(void)
{
   uint8_t return_value;// = legacy_wand_short_error();
   //RTC_TSR= 0;
   //return_value = legacy_wand_short_error();
   //RTC_TSR= 1;
   //return_value = legacy_wand_short_error();
   //RTC_TSR= 2;
   //return_value = legacy_wand_short_error();
   //RTC_TSR= 3;
   //return_value = legacy_wand_short_error();
   for (uint8_t j = 0; j <= 25; j++)
   {
     RTC_TSR = j;
     for (uint8_t i = 0; i < 16; i++)
     {
        return_value = legacy_wand_short_error();
     }
   }
   //TEST_ASSERT_EQUAL(1, return_value);
   TEST_ASSERT_EQUAL(256,current_therapy_settings.notify.status&WAND_SHORTED_NOTIFICATION_MASK);
   RTC_TSR= 4;
   return_value = legacy_wand_short_error();
   
   
}

void test_check_electrode_error (void)
{
   wand_irms_wear_monitor_ExpectAndReturn(1,0,0);
   wand_thandle_wear_monitor_ExpectAndReturn(0,1,0,1);
   current_therapy_settings.wand.thandle_wear = FALSE;
   current_therapy_settings.wand.irms_wear = TRUE;
   uint8_t return_value;
   return_value = check_electrode_error ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_check_electrode_error_2 (void)
{
   wand_irms_wear_monitor_ExpectAndReturn(1,0,0);
   wand_thandle_wear_monitor_ExpectAndReturn(0,1,0,1);
   current_therapy_settings.wand.thandle_wear = TRUE;
   current_therapy_settings.wand.irms_wear = FALSE;
   uint8_t return_value;
   return_value = check_electrode_error ();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_check_electrode_error_3 (void)
{
   current_therapy_settings.wand.thandle_wear = TRUE;
   current_therapy_settings.wand.irms_wear = TRUE;
   //   current_therapy_settings.wand.worn = 0;
   uint8_t return_value;
   wand_irms_wear_monitor_ExpectAndReturn(1,0,0);
   wand_thandle_wear_monitor_ExpectAndReturn(0,1,0,1);
   return_value = check_electrode_error ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_check_electrode_error_4 (void)
{
   //   current_therapy_settings.wand.worn = 10;
   uint8_t return_value;
   wand_irms_wear_monitor_ExpectAndReturn(1,0,0);
   wand_thandle_wear_monitor_ExpectAndReturn(0,1,0,1);
   return_value = check_electrode_error ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_check_electrode_error_5 (void)
{
   current_therapy_settings.wand.thandle_wear = TRUE;
   current_therapy_settings.wand.irms_wear = TRUE;
   //   current_therapy_settings.wand.worn = 255; // CHAR_MAX
   uint8_t return_value;
   wand_irms_wear_monitor_ExpectAndReturn(1,0,0);
   wand_thandle_wear_monitor_ExpectAndReturn(0,1,0,1);
   return_value = check_electrode_error ();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_wand_error_type_decipher (void)
{
   NOTIFY_TXT error = WAND_SHORT_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_short = 1;
   current_therapy_settings.notify.legacy_wand_short = 0;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(SMART_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_1 (void)
{
   NOTIFY_TXT error = WAND_SHORT_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_short = 0;
   current_therapy_settings.notify.legacy_wand_short = 1;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(LEGACY_WAND_ERROR, return_value);
}


void test_wand_error_type_decipher_2 (void)
{
   NOTIFY_TXT error = WAND_DISCONNECT_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_disconnected = 1;
   current_therapy_settings.notify.legacy_wand_disconnected = 0;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(SMART_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_3 (void)
{
   NOTIFY_TXT error = WAND_DISCONNECT_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_disconnected = 0;
   current_therapy_settings.notify.legacy_wand_disconnected = 1;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(LEGACY_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_4 (void)
{
   NOTIFY_TXT error = WAND_DISCONNECT_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_disconnected = 0;
   current_therapy_settings.notify.legacy_wand_disconnected = 0;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(TWO_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_5 (void)
{
   NOTIFY_TXT error = WAND_LIFE_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_use_limit = 1;
   current_therapy_settings.notify.legacy_wand_use_limit = 0;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(SMART_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_6 (void)
{
   NOTIFY_TXT error = WAND_LIFE_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_use_limit = 0;
   current_therapy_settings.notify.legacy_wand_use_limit = 1;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(LEGACY_WAND_ERROR, return_value);
}


void test_wand_error_type_decipher_7 (void)
{
   NOTIFY_TXT error = AMBIENT_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_no_ambient = 1;
   current_therapy_settings.notify.legacy_wand_no_ambient = 0;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(SMART_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_8 (void)
{
   NOTIFY_TXT error = AMBIENT_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_no_ambient = 0;
   current_therapy_settings.notify.legacy_wand_no_ambient = 1;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(LEGACY_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_9 (void)
{
   NOTIFY_TXT error = WAND_ID_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_id_error = 1;
   current_therapy_settings.notify.legacy_wand_id_error = 0;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(SMART_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_10 (void)
{
   NOTIFY_TXT error = WAND_ID_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_id_error = 0;
   current_therapy_settings.notify.legacy_wand_id_error = 1;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(LEGACY_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_11 (void)
{
   NOTIFY_TXT error =WAND_EXPIRED_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_expired = 1;
   current_therapy_settings.notify.legacy_wand_expired = 0;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(SMART_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_12 (void)
{
   NOTIFY_TXT error =WAND_EXPIRED_ERROR_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_expired = 0;
   current_therapy_settings.notify.legacy_wand_expired = 1;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(LEGACY_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_13 (void)
{
   NOTIFY_TXT error = SWITCH_STUCK_FAULT_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_stuck_switch = 1;
   current_therapy_settings.notify.legacy_wand_stuck_switch = 0;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(SMART_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_14 (void)
{
   NOTIFY_TXT error = SWITCH_STUCK_FAULT_TXT;
   uint8_t return_value;
   current_therapy_settings.notify.smart_wand_stuck_switch = 0;
   current_therapy_settings.notify.legacy_wand_stuck_switch = 1;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(LEGACY_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_15 (void)
{
   NOTIFY_TXT error = CHECK_ELECTRODE_ERROR_TXT;
   uint8_t return_value;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(SMART_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_16 (void)
{
   NOTIFY_TXT error = WAND_NOT_COMPATIBLE_ERROR_TXT;
   uint8_t return_value;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(LEGACY_WAND_ERROR, return_value);
}

void test_wand_error_type_decipher_17 (void)
{
   NOTIFY_TXT error = 0;
   uint8_t return_value;
   return_value = wand_error_type_decipher(error);
   TEST_ASSERT_EQUAL(0, return_value);
}

void test_clear_wand_errors1 (void)
{
   uint32_t return_value;
   current_therapy_settings.wand.unlocked = FALSE;
   current_therapy_settings.notify.smart_wand_no_ambient = 1;
   current_therapy_settings.notify.legacy_wand_no_ambient = 0;
   return_value = clear_wand_errors();
   TEST_ASSERT_EQUAL(AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK, ((~return_value) & AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK));
}

void test_clear_wand_errors2 (void)
{
   uint32_t return_value;
   current_therapy_settings.wand.unlocked = FALSE;
   current_therapy_settings.notify.legacy_wand_no_ambient = 1;
   current_therapy_settings.notify.smart_wand_id_error = 1;
   current_therapy_settings.notify.legacy_wand_id_error = 0;
   return_value = clear_wand_errors();
   TEST_ASSERT_EQUAL(WAND_ID_READ_NOTIFICATION_MASK , ((~return_value) &WAND_ID_READ_NOTIFICATION_MASK ));
}

void test_clear_wand_errors3 (void)
{
   uint32_t return_value;
   current_therapy_settings.wand.unlocked = FALSE;
    current_therapy_settings.notify.legacy_wand_id_error = 1;
   current_therapy_settings.notify.smart_wand_expired = 1;
   current_therapy_settings.notify.legacy_wand_expired = 0;
   return_value = clear_wand_errors();
   TEST_ASSERT_EQUAL(WAND_REUSE_NOTIFICATION_MASK , ((~return_value) &WAND_REUSE_NOTIFICATION_MASK ));
}

void test_clear_wand_errors4 (void)
{
   uint32_t return_value;
   current_therapy_settings.wand.unlocked = FALSE;
   current_therapy_settings.notify.legacy_wand_expired = 1;
   current_therapy_settings.notify.smart_wand_use_limit = 1;
   current_therapy_settings.notify.legacy_wand_use_limit = 0;
   return_value = clear_wand_errors();
   TEST_ASSERT_EQUAL(WAND_USE_LIMIT_LIFE_EXPIRED_NOTIFICATION_MASK , ((~return_value) &WAND_USE_LIMIT_LIFE_EXPIRED_NOTIFICATION_MASK ));
}

void test_clear_wand_errors5 (void)
{
   uint32_t return_value;
   current_therapy_settings.wand.unlocked = TRUE;
   current_therapy_settings.notify.smart_wand_disconnected = 1;
   return_value = clear_wand_errors();
   TEST_ASSERT_EQUAL(WAND_DISCONNECTED_NOTIFICATION_MASK , ((~return_value) &WAND_DISCONNECTED_NOTIFICATION_MASK ));
}

void test_clear_wand_errors6 (void)
{
   uint32_t return_value;
   current_therapy_settings.wand.unlocked = TRUE;
   legacy_wand_params_obj.wand_connected = 0;
   current_therapy_settings.notify.smart_wand_no_ambient = 0;
   current_therapy_settings.notify.legacy_wand_no_ambient = 1;
   return_value = clear_wand_errors();
   TEST_ASSERT_EQUAL(AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK , ((~return_value) & AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK ));
}

void test_clear_wand_errors7 (void)
{
   uint32_t return_value;
   current_therapy_settings.wand.unlocked = TRUE;
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.notify.legacy_wand_disconnected = 1;
   return_value = clear_wand_errors();
   TEST_ASSERT_EQUAL(WAND_DISCONNECTED_NOTIFICATION_MASK , ((~return_value) & WAND_DISCONNECTED_NOTIFICATION_MASK ));
}

//void test_nm1 (void)
//{
//   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
//   put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
//   current_therapy_settings.notify.status &= ~MULTIPLE_BUTTONS_NOTIFICATION_MASK;
//   current_therapy_settings.wand.coag_key = 1;
//   current_therapy_settings.wand.cut_key = 1;
//   current_therapy_settings.footpedal.coag = 1;
//   current_therapy_settings.footpedal.mode = 0;
//   notification_manager();
//   TEST_ASSERT_EQUAL(MULTIPLE_BUTTONS_NOTIFICATION_MASK, current_therapy_settings.notify.status &  MULTIPLE_BUTTONS_NOTIFICATION_MASK);
//   current_therapy_settings.footpedal.mode = 1;
//}
//
//void test_nm2 (void)
//{
//   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
//   put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
//   current_therapy_settings.notify.status &= ~MULTIPLE_BUTTONS_NOTIFICATION_MASK;
//   current_therapy_settings.wand.coag_key = 0;
//   current_therapy_settings.wand.cut_key = 0;
//   current_therapy_settings.footpedal.coag = 1;
//   current_therapy_settings.footpedal.mode = 0;
//   current_therapy_settings.wand.unlocked = FALSE;
//   legacy_wand_params_obj.wand_connected = 0;
//   notification_manager();
//   TEST_ASSERT_EQUAL( MULTIPLE_BUTTONS_NOTIFICATION_MASK, current_therapy_settings.notify.status &  MULTIPLE_BUTTONS_NOTIFICATION_MASK);
//   current_therapy_settings.footpedal.mode = 1;
//}

void test_nm3 (void)
{
   // put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
   // put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
   current_therapy_settings.notify.status2 = 1;
   notification_manager();
}


void test_nm4 (void)
{
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
   //   put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.notify.status2 = 1;
   current_therapy_settings.treatment.finger_switch_en = FALSE;
   notification_manager();
   TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.treatment.finger_switch_en);
}

void test_nm5 (void)
{
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
   //put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.notify.status2 = 1;
   current_therapy_settings.treatment.finger_switch_en = FALSE;
   current_therapy_settings.treatment.wireless_enable = TRUE;
   notification_manager();
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.treatment.wireless_enable);
}

void test_nm6 (void)
{
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
   //put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.treatment.auto_finger_switch_en = TRUE;
   current_therapy_settings.notify.status2 = 1;
   notification_manager();
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.treatment.auto_finger_switch_en);
}

void test_nm7 (void)
{
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
   //put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.treatment.auto_finger_switch_en = TRUE;
   current_therapy_settings.treatment.auto_wireless_en = TRUE;
   current_therapy_settings.notify.status2 = 1;
   notification_manager();
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.treatment.auto_wireless_en);
}
void test_nm8 (void){
	current_therapy_settings.gui.depth_idx = 5;
	current_therapy_settings.active_wand.smart_wand = ENT;
	current_therapy_settings.active_wand.legacy_wand = ENT;
	legacy_wand_params_obj.wand_connected = 1;
	notification_manager();
}	
	
void test_two_wand_disconnected_error (void)
{
  // set the notify.status bit to anything other than wand disconnected mask
  current_therapy_settings.notify.status |= WAND_SHORTED_NOTIFICATION_MASK;
  two_wand_disconnected_error();
  TEST_ASSERT_EQUAL(WAND_DISCONNECTED_NOTIFICATION_MASK, current_therapy_settings.notify.status&WAND_DISCONNECTED_NOTIFICATION_MASK);
}  

void test_gen_com_fault (void)
{
  // set the notify.status bit to anything other than SYS_COM_FAULT_MASK
  current_therapy_settings.notify.status |= WAND_SHORTED_NOTIFICATION_MASK;
  gen_com_fault();
  TEST_ASSERT_EQUAL(SYS_COM_FAULT_MASK,current_therapy_settings.notify.status&SYS_COM_FAULT_MASK);
}

//***Multiple button press test***//
// MULTIPLE_BUTTONS_NOTIFICATION_MASK = 0x1000

void test_multiple_button_pressed_test (void)
{
  put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
  put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
  current_therapy_settings.wand.coag_key = 1;
  current_therapy_settings.wand.cut_key = 1;
  current_therapy_settings.notify.status = WAND_SHORTED_NOTIFICATION_MASK;
  get_priming_status_IgnoreAndReturn(0);
  multiple_button_pressed_test(current_therapy_settings.notify.status);
}

void test_multiple_button_pressed_test_2 (void)
{

  uint32_t  return_value;
  current_therapy_settings.wand.coag_key = 0;
  current_therapy_settings.wand.cut_key = 0;
  current_therapy_settings.wand.mode_key = 0;
  current_therapy_settings.legacy_wand_levels.cut_key = 0;
  current_therapy_settings.legacy_wand_levels.coag_key = 0;
  current_therapy_settings.legacy_wand_levels.mode_key = 0;
  current_therapy_settings.footpedal.coag = 0;
  current_therapy_settings.footpedal.ablate = 0;
  current_therapy_settings.footpedal.mode = 0;
  current_therapy_settings.notify.status = MULTIPLE_BUTTONS_NOTIFICATION_MASK;
  get_priming_status_IgnoreAndReturn(1);
  cbtmr_reg_callback_IgnoreAndReturn(0);
  cbtmr_reg_callback_IgnoreAndReturn(0);
  return_value = multiple_button_pressed_test(current_therapy_settings.notify.status);
  TEST_ASSERT_EQUAL(4096,return_value);
  // call the function twice to clear the previous flag
  current_therapy_settings.legacy_wand_levels.mode_key = 1;
  RTC_TSR= 3;
  get_priming_status_IgnoreAndReturn(0);
  cbtmr_unreg_callback_IgnoreAndReturn(0);
  //cbtmr_reg_callback_IgnoreAndReturn(0);
  return_value = multiple_button_pressed_test(current_therapy_settings.notify.status);
  TEST_ASSERT_EQUAL(4096,return_value&MULTIPLE_BUTTONS_NOTIFICATION_MASK);
}

void test_multiple_button_pressed_test_3 (void)
{

  current_therapy_settings.wand.coag_key = 0;
  current_therapy_settings.wand.cut_key = 0;
  current_therapy_settings.wand.mode_key = 0;
  current_therapy_settings.legacy_wand_levels.cut_key = 0;
  current_therapy_settings.legacy_wand_levels.coag_key = 0;
  current_therapy_settings.legacy_wand_levels.mode_key = 0;
  current_therapy_settings.footpedal.coag = 0;
  current_therapy_settings.footpedal.ablate = 0;
  current_therapy_settings.footpedal.mode = 0;
  current_therapy_settings.notify.status = MULTIPLE_BUTTONS_NOTIFICATION_MASK;
  get_priming_status_IgnoreAndReturn(0);
  cbtmr_reg_callback_IgnoreAndReturn(0);
  multiple_button_pressed_test(current_therapy_settings.notify.status);
  current_therapy_settings.legacy_wand_levels.mode_key = 0;
  RTC_TSR= 15;
  get_priming_status_IgnoreAndReturn(0);
  cbtmr_reg_callback_IgnoreAndReturn(0);
  uint32_t  return_value = multiple_button_pressed_test(current_therapy_settings.notify.status);
  //TEST_ASSERT_EQUAL(0,return_value&MULTIPLE_BUTTONS_NOTIFICATION_MASK);
}

void test_multiple_button_pressed_test_4 (void)
{

  cbtmr_unreg_callback_IgnoreAndReturn(0);
  current_therapy_settings.wand.coag_key = 0;
  current_therapy_settings.wand.cut_key = 0;
  current_therapy_settings.wand.mode_key = 0;
  current_therapy_settings.legacy_wand_levels.cut_key = 0;
  current_therapy_settings.legacy_wand_levels.coag_key = 0;
  current_therapy_settings.legacy_wand_levels.mode_key = 0;
  current_therapy_settings.footpedal.coag = 1;
  current_therapy_settings.footpedal.ablate = 1;
  current_therapy_settings.footpedal.mode = 1;
  current_therapy_settings.notify.status = 0;
  get_priming_status_IgnoreAndReturn(1);
  cbtmr_unreg_callback_IgnoreAndReturn(0);
  multiple_button_pressed_test(current_therapy_settings.notify.status);
  // call the function twice to clear the previous flag
  current_therapy_settings.legacy_wand_levels.mode_key = 1;
  RTC_TSR= 3;
  get_priming_status_IgnoreAndReturn(0);
  cbtmr_unreg_callback_IgnoreAndReturn(0);
  uint32_t  return_value = multiple_button_pressed_test(current_therapy_settings.notify.status);
  TEST_ASSERT_EQUAL(0,return_value&MULTIPLE_BUTTONS_NOTIFICATION_MASK);
}

void test_multiple_button_pressed_test_5 (void)
{

  current_therapy_settings.wand.coag_key = 0;
  current_therapy_settings.wand.cut_key = 0;
  current_therapy_settings.wand.mode_key = 0;
  current_therapy_settings.legacy_wand_levels.cut_key = 0;
  current_therapy_settings.legacy_wand_levels.coag_key = 0;
  current_therapy_settings.legacy_wand_levels.mode_key = 0;
  current_therapy_settings.footpedal.coag = 0;
  current_therapy_settings.footpedal.ablate = 1;
  current_therapy_settings.footpedal.mode = 0;
  current_therapy_settings.notify.status = MULTIPLE_BUTTONS_NOTIFICATION_MASK;
  get_priming_status_IgnoreAndReturn(0);
  cbtmr_reg_callback_IgnoreAndReturn(0);
  multiple_button_pressed_test(current_therapy_settings.notify.status);
  current_therapy_settings.notify.status = MULTIPLE_BUTTONS_NOTIFICATION_MASK;
  get_priming_status_IgnoreAndReturn(0);
  multiple_button_pressed_test(current_therapy_settings.notify.status);
}


void test_ui_fault (void)
{
	uint8_t return_value = ui_fault();
	TEST_ASSERT_EQUAL(1,return_value);
}


void test_wand_short_error (void)
{
   cbtmr_unreg_callback_IgnoreAndReturn(0);
   wand_short_error();

   smart_wand_peak_hit_cnt = 16;
   wand_short_error();

   smart_wand_peak_hit_cnt = 16;
   smart_wand_consecutive_shorts = 5;
   put_event_IgnoreAndReturn(1);
   cbtmr_unreg_callback_IgnoreAndReturn(0);
   wand_short_error();
   
   
}

void test_peak_hit_cntr (void)
{
	
	cbtmr_reg_callback_IgnoreAndReturn(0);
	smart_wand_short_tracker_f = 0;
	uint8_t return_value = peak_hit_cntr();
	
	smart_wand_peak_hit_cnt = 0;
	return_value = peak_hit_cntr();
	
	smart_wand_peak_hit_cnt = 16;
	smart_wand_consecutive_shorts = 5;
	cbtmr_unreg_callback_IgnoreAndReturn(0);
	put_event_IgnoreAndReturn(1);
	return_value = peak_hit_cntr();
	
}



void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
