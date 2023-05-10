#include "unity.h"
#include "math.h"
#include "unity_internals.h"
#include "MK26F18.h"
#include "common.h"
#include "wand.h"
#include "Mocksystem.h"
#include "cpu.h"
#include "Mockpit.h"
#include "Mockevent_buffer.h"
#include "Mockrtc.h"
#include <string.h>
#include <stdio.h>
#include "Mockcan_debug.h"
#include "Mocklegacy_wand.h"
#include "Mockprofiles.h"
#include "Mocknotifications.h"
#include "Mockgui.h"
#include "Mockrf.h"
#include "Mockpump.h"
#include "switches.h"

GEN_SETTINGS current_therapy_settings;

event_table evt_tbl;
extern SMART_WAND_DATABASE db;
SMART_WAND_PARAMETERS smart_wand_params;
SMART_WAND_PARAMETERS *const pSmartWandParams = &smart_wand_params;
COMMON_WAND_PARAMS smart_wand_common_params;
__root uint8_t serial_number[MFG_SERIAL_NUM_CHARS];
__root uint8_t mfg_date_time[MFG_TIME_NUM_CHARS];
legacy_wand_params legacy_wand_params_obj;
extern uint8_t throttle_therapy_on;
prime_mode_t PrimeState;
u8 AutoPrimeId = 0U;
u8 AutoPrimeTimeoutId = 0U;
uint8_t flow_val_apl_ent_bkp;
u8 PrimeId = 0U;
// DEBUG CANdebug;
// volatile DEBUG *pCANdebug = &CANdebug;

extern STATIC_LOG static_log;

extern WAND_STATE tx_state;
extern WAND_TX_PACKET_QUEUE packet_queue;
//uint8_t current_coag_setpoint;
extern uint16_t manta_pwm_mod;
extern uint16_t manta_pwm_C0V;
extern uint16_t manta_pwm_C1V;
extern uint16_t manta_pwm_C2V;
extern uint16_t manta_pwm_C3V;
extern uint16_t manta_pwm_C4V;
extern uint16_t manta_pwm_C5V;
extern uint16_t manta_pwm_C6V;
extern uint16_t manta_pwm_C7V;
void setUp(void)
{
}

void tearDown(void)
{
}



void test_wand_tx_STATIC_LOG_WRITE_1 (void)
{
   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[0].id = 0;

   uint8_t return_value = wand_tx();

   TEST_ASSERT_EQUAL(0,db.static_log.unit_num);

   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[0].id = 1;
   static_log.unit[1].id = 0;
   static_log.unit[0].start_idx = 0;
   static_log.unit[0].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(1,db.static_log.unit_num);

   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[1].id = 0;
   static_log.unit[0].start_idx = 10;
   static_log.unit[0].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(1,db.static_log.unit_num);


   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[1].id = 1;
   static_log.unit[2].id = 0;
   static_log.unit[1].start_idx = 0;
   static_log.unit[1].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(2,db.static_log.unit_num);

   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[1].id = 1;
   static_log.unit[2].id = 0;
   static_log.unit[1].start_idx = 10;
   static_log.unit[1].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(2,db.static_log.unit_num);

   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[2].id = 1;
   static_log.unit[3].id = 0;
   static_log.unit[2].start_idx = 0;
   static_log.unit[2].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(3,db.static_log.unit_num);

   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[2].id = 1;
   static_log.unit[3].id = 0;
   static_log.unit[2].start_idx = 10;
   static_log.unit[2].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(3,db.static_log.unit_num);

   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[3].id = 1;
   static_log.unit[4].id = 0;
   static_log.unit[3].start_idx = 0;
   static_log.unit[3].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(4,db.static_log.unit_num);

   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[3].id = 1;
   static_log.unit[4].id = 0;
   static_log.unit[3].start_idx = 10;
   static_log.unit[3].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(4,db.static_log.unit_num);


   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[4].id = 1;
   static_log.unit[4].start_idx = 0;
   static_log.unit[4].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(4,db.static_log.unit_num);

   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[4].id = 1;
   static_log.unit[4].start_idx = 10;
   static_log.unit[4].end_idx = 10;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(4,db.static_log.unit_num);
}

void test_start_idx_greater_than_382 (void)
{
   uint8_t return_value;
   tx_state = STATIC_LOG_WRITE_1;
   static_log.unit[4].id = 1;
   static_log.unit[4].start_idx = 10;
   static_log.unit[4].end_idx = 400;
   return_value = wand_tx();

   TEST_ASSERT_EQUAL(4,db.static_log.unit_num);

}



void test_wand_tx_STATIC_LOG_WRITE_2 (void)
{
   tx_state = STATIC_LOG_WRITE_2;
   uint8_t return_value = wand_tx();

   //TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.param[4],SMART_WAND_STATIC_READ_RX_PACKET_SIZE);
}

void test_wand_tx_CIRCLE_LOG_READ_0 (void)
{
   tx_state = CIRCLE_LOG_READ;
   db.static_log.unit_num = 1;
   static_log.unit[db.static_log.unit_num].start_idx  = 0;
   uint8_t return_value = wand_tx();

   TEST_ASSERT_EQUAL(0,db.circle_log.page);

}

void test_wand_tx_CIRCLE_LOG_READ_2 (void)
{
   tx_state = CIRCLE_LOG_READ;
   db.static_log.unit_num = 1;
   static_log.unit[db.static_log.unit_num].start_idx  = 2;
   uint8_t return_value = wand_tx();

   TEST_ASSERT_EQUAL(1,db.circle_log.page);
}

void test_wand_tx_ERASE_STATIC_LOGS (void)
{
   tx_state = ERASE_STATIC_LOGS;
   db.static_log.write_echo.idx = 1;
   uint8_t return_value = wand_tx();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_tx_ERASE_CIRCLE_LOGS (void)
{
   tx_state = ERASE_CIRCLE_LOGS;
   db.static_log.write_echo.idx = 2;
   uint8_t return_value = wand_tx();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_tx_SWITCH_BOOT_TO_LOG (void)
{
   tx_state = SWITCH_BOOT_TO_LOG;
   db.static_log.write_echo.idx = 2;
   uint8_t return_value = wand_tx();
   TEST_ASSERT_EQUAL(0,return_value);
}


// void test_wand_tx_default (void)
// {
//    tx_state = PERIODIC;
//    uint8_t return_value = wand_tx();
//    //TEST_ASSERT_EQUAL(LOCK,tx_state);
//    // No test as the default case does nothing
// }

void test_thermometer_color (void)
{
   uint8_t temperature = 25;
   current_therapy_settings.ambient.alarm_threshold = 20;
   uint8_t return_value = thermometer_color(temperature);
   TEST_ASSERT_EQUAL(0,return_value);

   temperature = 19;
   current_therapy_settings.ambient.alarm_threshold = 20;
   return_value = thermometer_color(temperature);
   TEST_ASSERT_EQUAL(1,return_value);

   temperature = 15;
   current_therapy_settings.ambient.alarm_threshold = 20;
   return_value = thermometer_color(temperature);
   TEST_ASSERT_EQUAL(1,return_value);

   temperature = 10;
   current_therapy_settings.ambient.alarm_threshold = 20;
   return_value = thermometer_color(temperature);
   TEST_ASSERT_EQUAL(2,return_value);
}

void test_wand_static_write(void)
{
   uint8_t return_value;
   db.static_log.page_read = STATIC_MAX_NUM_PAGES;
   db.static_log.page_write = 0x01;
   db.static_log.write_echo.idx = db.static_log.page_write + 1;
   wand_static_write();
   TEST_ASSERT_EQUAL(CIRCLE_LOG_READ,tx_state);
}

void test_wand_static_write_0(void)
{
   uint8_t return_value = wand_static_write();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_static_write_1(void)
{
   uint8_t return_value;
   db.static_log.page_read = STATIC_MAX_NUM_PAGES;
   db.static_log.page_write = 0x01;
   db.static_log.write_echo.idx = db.static_log.page_write;
   wand_static_write();
   TEST_ASSERT_EQUAL(STATIC_LOG_WRITE_2,tx_state);
}

void test_wand_static_write_2(void)
{
   uint8_t return_value;
   db.static_log.page_read = STATIC_MAX_NUM_PAGES;
   db.static_log.page_write = 0x01;
   db.static_log.write_echo.idx = 0;
   wand_static_write();
   TEST_ASSERT_EQUAL(STATIC_LOG_READ,tx_state);
}

void test_wand_static_write_3(void)
{
   uint8_t return_value;
   db.static_log.page_read = STATIC_MAX_NUM_PAGES;
   db.static_log.page_write = 0x05;
   db.static_log.write_echo.idx = 12;
   wand_static_write();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_extract_wand_params_default_ablate_mode_MED_5 (void)
{

   db.mfg.data[3] = 0x05;
   db.mfg.data[50] =  0xFF;
   db.mfg.data[51] = 0xff;
   db.mfg.data[46] = SMART_WAND_MFG_INDICATION_SPORTS_MED;
   db.mfg.data[62] =  0x4; 
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);  
   extract_wand_params ();
   db.mfg.data[46] = SMART_WAND_MFG_INDICATION_ENT;
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1); 
   extract_wand_params ();
}

void test_extract_wand_params (void)
{
   //smart_wand_params.mfg.format = 1;
   db.mfg.data[61] = 0x0F;
   (((*(uint8_t*)&db.mfg.data[50]) >> 4) & 0xF);
   db.mfg.data[43] =  0x30;
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_1 (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 1;
   db.mfg.data[43] =  0x00;
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_2 (void)
{
   ((*(uint8_t*)&db.mfg.data[51]) & 0xF);
   db.mfg.data[43] = 0x09;
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
}

void test_extract_wand_params_3 (void)
{
   ((*(uint8_t*)&db.mfg.data[51]) & 0xF);
   db.mfg.data[43] = 0x00;
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   TEST_ASSERT_EQUAL(NONE,current_therapy_settings.treatment.therapy_mode);
}

void test_extract_wand_params_4 (void)
{
   ((*(uint8_t*)&db.mfg.data[51]) & 0xF);
   db.mfg.data[43] = 0x03;
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   TEST_ASSERT_EQUAL(LO,current_therapy_settings.treatment.therapy_mode);
}

void test_extract_wand_params_MED (void)
{
   ((*(uint8_t*)&db.mfg.data[51]) & 0xF);
   db.mfg.data[43] = 0x05;
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   TEST_ASSERT_EQUAL(MED,current_therapy_settings.treatment.therapy_mode);
}

void test_extract_wand_params_5 (void)
{
   ((*(uint8_t*)&db.mfg.data[51]) & 0xF);
   db.mfg.data[43] = 0x07;
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
}

void test_extract_wand_params_6 (void)
{
   ((*(uint8_t*)&db.mfg.data[51]) & 0xF);
   db.mfg.data[43] = 0x07;
   load_profile_IgnoreAndReturn(1);
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.wand.valid = TRUE;
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
}

void test_extract_wand_params_7 (void)
{
   ((*(uint8_t*)&db.mfg.data[51]) & 0xF);
   db.mfg.data[43] = 0x07;
   db.mfg.data[62] = 0x05;
   db.mfg.data[3]  = 0x00;
   db.mfg.data[7]  = 0x00;
   smart_wand_params.mfg.work_order[0] = 0x01;
   smart_wand_params.mfg.unique_id[0]  = 0x01;  
   smart_wand_params.mfg.default_ablate_mode = 10;
   smart_wand_params.mfg.coag_enable = 0;
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.wand.valid = FALSE;
   
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
}

void test_extract_wand_params_8 (void)
{
   ((*(uint8_t*)&db.mfg.data[51]) & 0xF);
   db.mfg.data[43] = 0x07;
   db.mfg.data[62] = 0x05;
   db.mfg.data[3]  = 0x00;
   db.mfg.data[7]  = 0x00;
   smart_wand_params.mfg.work_order[0] = 0x00;
   smart_wand_params.mfg.work_order[1] = 0x02;
   smart_wand_params.mfg.unique_id[0]  = 0x01;  
   smart_wand_params.mfg.default_ablate_mode = 10;
   smart_wand_params.mfg.coag_enable = 0;
   load_profile_IgnoreAndReturn(1);
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.wand.valid = FALSE;
   smartwand_pwm_init_IgnoreAndReturn(1);
   smart_wand_params.mfg.default_coag_mode = 3;
   
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
} 


void test_extract_wand_params_default_coag_mode_true (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0;
   smart_wand_params.mfg.default_coag_mode = 3;
   db.mfg.data[43] =  0x32;  //This sets default_coag_mode = 3
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_default_ablate_mode_true (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0;
   db.mfg.data[43] =  0x7E;  //This sets default_ablate_mode  mode = 9
   load_profile_IgnoreAndReturn(1);
   smartwand_pwm_init_IgnoreAndReturn(1);   
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_wand_detect_duration_greater_30 (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0;
   db.mfg.data[62] =  0x3;  
   db.param[3].data[14]= 31; // Sets worn.detect.duration > 30
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_wand_detect_duration_lessthan_2 (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0;
   db.mfg.data[62] =  0x3;  
   db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_default_ablate_mode_MED (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[43] =  0x75;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_default_ablate_mode_MED_1 (void)
{
   smart_wand_params.mfg.coag_enable = 0;
   smart_wand_params.mfg.indication = 1;
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[43] =  0x75;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
}

void test_extract_wand_params_default_ablate_mode_MED_2 (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0x05;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[43] =  0x75;
   db.mfg.data[3] = 0x05;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   load_profile_IgnoreAndReturn(1);
	smartwand_pwm_init_IgnoreAndReturn(1);   
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_default_ablate_mode_MED_3 (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   db.mfg.data[7] = 1;
   smart_wand_params.mfg.work_order[0] = 0x05;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[43] =  0x75;
   db.mfg.data[3] = 0x05;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_default_ablate_mode_MED_4 (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.coag_enable = 0;
   db.mfg.data[7] = 1;
   smart_wand_params.mfg.work_order[0] = 0x05;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[43] =  0x75;
   db.mfg.data[3] = 0x05;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   load_profile_IgnoreAndReturn(1);
smartwand_pwm_init_IgnoreAndReturn(1);   
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
   
   
}

void test_extract_wand_params_manta_wand (void)
{
	db.mfg.data[46] = 0x02;   
	smart_wand_params.mfg.indication = 2; //Arthroplasty
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0;
   smart_wand_params.mfg.default_coag_mode = 6;
   db.mfg.data[43] =  0x62;  //This sets default_coag_mode = 6
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   change_flow_manta_IgnoreAndReturn(1);
   current_therapy_settings.wand.valid = TRUE;
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}
   
   void test_extract_wand_params_manta_wand_2 (void)
{
	db.mfg.data[3] =  0; //same wand
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   change_flow_manta_IgnoreAndReturn(1);
   extract_wand_params();
}
   
   void test_extract_wand_params_manta_wand_3 (void)
{
	current_therapy_settings.controller = ENT;
   legacy_wand_params_obj.wand_connected = TRUE;
   current_therapy_settings.active_wand.legacy_wand = ENT;
   change_flow_manta_IgnoreAndReturn(1);
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   change_flow_manta_IgnoreAndReturn(1);
   extract_wand_params();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.pump.ent_flow);
}
   
void test_extract_wand_params_manta_wand_4 (void)
{
	current_therapy_settings.controller = ENT;
   legacy_wand_params_obj.wand_connected = FALSE;
   current_therapy_settings.active_wand.legacy_wand = ENT;
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   change_flow_manta_IgnoreAndReturn(1);
   extract_wand_params();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.pump.ent_flow);
}

void test_extract_wand_params_manta_wand_5 (void)
{
	current_therapy_settings.controller = ENT;
   legacy_wand_params_obj.wand_connected = TRUE;
   current_therapy_settings.active_wand.legacy_wand = SPORTS_MED;
   load_profile_IgnoreAndReturn(1); 
   smartwand_pwm_init_IgnoreAndReturn(1);
   change_flow_manta_IgnoreAndReturn(1);
   extract_wand_params();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.pump.ent_flow);
}

void test_extract_wand_params_manta_wand_6 (void)
{
	current_therapy_settings.controller = SPORTS_MED;
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0x04;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[46] =  0x02;
   db.mfg.data[43] =  0x15;
   db.mfg.data[3] = 0x05;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   change_flow_manta_IgnoreAndReturn(1);
   load_profile_IgnoreAndReturn(1);
	smartwand_pwm_init_IgnoreAndReturn(1);   
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_manta_wand_7 (void)
{
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0x04;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[46] =  0x02;
   db.mfg.data[43] =  0x75;
   db.mfg.data[3] = 0x05;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   change_flow_manta_IgnoreAndReturn(1);
   load_profile_IgnoreAndReturn(1);
	smartwand_pwm_init_IgnoreAndReturn(1);   
   extract_wand_params ();
   //TEST_ASSERT_EQUAL(COAG_1,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_manta_wand_8 (void)
{
	current_therapy_settings.user_data.settings.active_idx = 1;
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0x04;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[46] =  0x02;
   db.mfg.data[43] =  0x75;
   db.mfg.data[3] = 0x05;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   change_flow_manta_IgnoreAndReturn(1);
   load_profile_IgnoreAndReturn(1);
	smartwand_pwm_init_IgnoreAndReturn(1);   
   extract_wand_params ();
   TEST_ASSERT_EQUAL(COAG_7,current_therapy_settings.setpoint.coag);
}

void test_extract_wand_params_manta_wand_9 (void)
{
	change_flow_manta_IgnoreAndReturn(1);
	flow_val_apl_ent_bkp = 3;
	current_therapy_settings.user_data.settings.active_idx = 1;
   smart_wand_params.mfg.unique_id[0] = 1;
   smart_wand_params.mfg.work_order[0] = 0x05;
   db.mfg.data[62] =  0x3; 
   db.mfg.data[46] =  0x02;
   db.mfg.data[43] =  0x75;
   db.mfg.data[3] = 0x05;
   db.mfg.data[51] = 0x03;
   //db.param[3].data[14]= 3; // Sets worn.detect.duration > 30
   change_flow_manta_IgnoreAndReturn(1);
   load_profile_IgnoreAndReturn(1);
	smartwand_pwm_init_IgnoreAndReturn(1);   
   extract_wand_params ();
   TEST_ASSERT_EQUAL(COAG_7,current_therapy_settings.setpoint.coag);
   TEST_ASSERT_EQUAL(3,current_therapy_settings.treatment.flow_rate);
   flow_val_apl_ent_bkp = 0;
}

void test_smartwand_send_packet(void)
{
   packet_queue.idx = 0;
   smartwand_send_packet();
}

void test_smartwand_timeout_less_than_25(void)
{
   current_therapy_settings.wand.timeout = 9;
   uint8_t return_value = smartwand_timeout();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_smartwand_timeout_greater_than_25(void)
{
   current_therapy_settings.wand.timeout = 27;
   put_event_ExpectAndReturn(&evt_tbl,WAND_UNPLUGGED,1);
   uint8_t return_value = smartwand_timeout();
   TEST_ASSERT_EQUAL(1,return_value);

}

void test_SmartWandUART0_Init(void)
{
   uint8_t return_value = smartwand_uart_init();
   // check to see if transmit was enabled
   TEST_ASSERT_EQUAL(UART_C2_TE_MASK,(UART0_C2&UART_C2_TE_MASK));
   // Check to see if receive was enabled
   TEST_ASSERT_EQUAL(UART_C2_RE_MASK,(UART0_C2&UART_C2_RE_MASK));

   //Check to see if receive interrrupt was enabled
   TEST_ASSERT_EQUAL(UART_C2_RIE_MASK,(UART0_C2&UART_C2_RIE_MASK));
}


void test_wand_validate(void)
{
	smartwand_pwm_init_IgnoreAndReturn(1);
   uint8_t chksum1 =0;
   uint8_t chksum2 =0;
   for (uint8_t j = 0; j <= PARAM_PAGE_4; j++)
   {
      for (uint8_t i = 0; i < PARAM_PAGE_SIZE; i++)
      {
         db.param[j].data[i] = 0;
         chksum1 += db.param[j].data[i];
      }

   }
   db.param[4].data[63] = ~chksum1+1;

   for (uint8_t i = 0; i < MFG_DATA_SIZE; i++)
   {
      db.mfg.data[i] = 0;
      if (i == 61)
      {
         db.mfg.data[i] = 1;
      }
      chksum2 += db.mfg.data[i];
   }
   db.mfg.data[63] = ~chksum2+1;
   load_profile_IgnoreAndReturn(1);
   uint8_t return_value = wand_validate();
   TEST_ASSERT_EQUAL(STATIC_LOG_READ,tx_state);
}

void test_wand_validate_2(void)
{
	smartwand_pwm_init_IgnoreAndReturn(1);
   uint8_t chksum1 =0;
   uint8_t chksum2 =0;
   for (uint8_t j = 0; j <= PARAM_PAGE_4; j++)
   {
      for (uint8_t i = 0; i < PARAM_PAGE_SIZE; i++)
      {
         db.param[j].data[i] = 0;
         chksum1 += db.param[j].data[i];
      }

   }
   db.param[4].data[63] = ~chksum1+1;
   db.mfg.data[61] = 0;
   for (uint8_t i = 0; i < MFG_DATA_SIZE; i++)
   {
      db.mfg.data[i] = 0;
      chksum2 += db.mfg.data[i];
   }
   db.mfg.data[63] = ~chksum2+1;
   //put_event_ExpectAndReturn(&evt_tbl,WAND_ID_ERROR,1);
   load_profile_IgnoreAndReturn(0);
   uint8_t return_value = wand_validate();
   TEST_ASSERT_EQUAL(STATIC_LOG_READ,tx_state);
}

void test_wand_validate_3(void)
{
	smartwand_pwm_init_IgnoreAndReturn(1);
   uint8_t chksum1 =0;
   uint8_t chksum2 =0;
   for (uint8_t j = 0; j <= PARAM_PAGE_4; j++)
   {
      for (uint8_t i = 0; i < PARAM_PAGE_SIZE; i++)
      {
         db.param[j].data[i] = 0;
         chksum1 += db.param[j].data[i];
      }

   }
   db.mfg.data[46] = 1; //mfg_indication
   db.mfg.data[61] = 0; //mfg_format
   db.param[4].data[63] = ~chksum1+1;

   for (uint8_t i = 0; i < MFG_DATA_SIZE; i++)
   {
      db.mfg.data[i] = 0;
      if (i == 46)
      {
         db.mfg.data[i] = 1;
      }
      chksum2 += db.mfg.data[i];
   }
   db.mfg.data[63] = ~chksum2+1;
   //put_event_ExpectAndReturn(&evt_tbl,WAND_ID_ERROR,1);
   load_profile_IgnoreAndReturn(1);
   uint8_t return_value = wand_validate();
   //TEST_ASSERT_EQUAL(PERIODIC,tx_state);
}


void test_wand_validate_4(void)
{
   uint8_t chksum1 =0;
   uint8_t chksum2 =0;
   for (uint8_t j = 0; j <= PARAM_PAGE_4; j++)
   {
      for (uint8_t i = 0; i < PARAM_PAGE_SIZE; i++)
      {
         db.param[j].data[i] = 0;
         chksum1 += db.param[j].data[i];
      }

   }
   db.mfg.data[46] = 1; //mfg_indication
   db.mfg.data[61] = 0; //mfg_format
   db.param[4].data[63] = ~chksum1+1;
   for (uint8_t i = 0; i < MFG_DATA_SIZE; i++)
   {
      db.mfg.data[i] = 0;
      if (i == 46)
      {
         db.mfg.data[i] = 1;
      }
      chksum2 += db.mfg.data[i];
   }
   db.mfg.data[63] = ~chksum2;//wrong checksum
   put_event_ExpectAndReturn(&evt_tbl,WAND_ID_ERROR,1);
   uint8_t return_value = wand_validate();
   TEST_ASSERT_EQUAL(PERIODIC,tx_state);
}


void test_wand_validate_5(void)
{
   uint8_t chksum1 =0;
   uint8_t chksum2 =0;
   for (uint8_t j = 0; j <= PARAM_PAGE_4; j++)
   {
      for (uint8_t i = 0; i < PARAM_PAGE_SIZE; i++)
      {
         db.param[j].data[i] = 0;
         chksum1 += db.param[j].data[i];
      }

   }
   db.param[4].data[63] = ~chksum1; // wrong checksum

   db.mfg.data[46] = 1; //mfg_indication
   db.mfg.data[61] = 0; //mfg_format

   for (uint8_t i = 0; i < MFG_DATA_SIZE; i++)
   {
      db.mfg.data[i] = 0;
      if (i == 46)
      {
         db.mfg.data[i] = 1;
      }
      chksum2 += db.mfg.data[i];
   }
   db.mfg.data[63] = ~chksum2+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_ID_ERROR,1);
   uint8_t return_value = wand_validate();
   TEST_ASSERT_EQUAL(PERIODIC,tx_state);
}

void test_wand_download_parm_pg_0(void)
{
   tx_state = LOCK;
   current_therapy_settings.wand.unlocked = FALSE;
   uint8_t return_value = wand_download_parm_pg_0();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_wand_download_parm_pg_1(void)
{
   tx_state = LOCK;
   uint8_t return_value = wand_download_parm_pg_1();
   TEST_ASSERT_EQUAL(PARAM_1,tx_state);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_wand_download_parm_pg_2(void)
{
   tx_state = LOCK;
   uint8_t return_value = wand_download_parm_pg_2();
   TEST_ASSERT_EQUAL(PARAM_2,tx_state);
}

void test_wand_download_parm_pg_3(void)
{
   tx_state = LOCK;
   uint8_t return_value = wand_download_parm_pg_3();
   TEST_ASSERT_EQUAL(PARAM_3,tx_state);
}

void test_wand_download_parm_pg_4(void)
{
   tx_state = LOCK;
   uint8_t return_value = wand_download_parm_pg_4();
   TEST_ASSERT_EQUAL(PARAM_4,tx_state);
}

void test_wand_unlock (void)
{
   tx_state = LOCK;
   uint8_t return_value = wand_unlock();
   TEST_ASSERT_EQUAL(UNLOCK,tx_state);
}

void test_wand_lock (void)
{
   tx_state = UNLOCK;
   current_therapy_settings.wand.unlocked = TRUE;
   uint8_t return_value = wand_lock();
   TEST_ASSERT_EQUAL(LOCK,tx_state);
}

void test_wand_lock_2 (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = FALSE;
   switch_to_legacy_wand_ExpectAndReturn(1);
   uint8_t return_value = wand_lock();
   TEST_ASSERT_EQUAL(1,return_value);
}


void test_wand_tx_lock (void)
{
   tx_state = LOCK;
   memset(&db.mfg,1,SMART_WAND_MFG_PACKET_SIZE);
   uint8_t check_array[SMART_WAND_MFG_PACKET_SIZE];
   memset(&check_array,0,SMART_WAND_MFG_PACKET_SIZE);

   uint8_t return_value = wand_tx();
   TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.mfg,SMART_WAND_MFG_PACKET_SIZE);
}

void test_wand_tx_unlock (void)
{
   tx_state = UNLOCK;
   memset(&db.passcode,1,SMART_WAND_PASSCODE_RX_PACKET_SIZE);
   uint8_t check_array[SMART_WAND_PASSCODE_RX_PACKET_SIZE];
   memset(&check_array,0,SMART_WAND_PASSCODE_RX_PACKET_SIZE);
   uint8_t return_value = wand_tx();
   TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.passcode,SMART_WAND_PASSCODE_RX_PACKET_SIZE);
}

void test_wand_tx_param0 (void)
{
   // No test for this function. Just coverage
   tx_state = PARAM_0;
   // memset(&db.param[0],1,SMART_WAND_PARAM_RX_PACKET_SIZE);
   // uint8_t check_array[SMART_WAND_PARAM_RX_PACKET_SIZE];
   // memset(&check_array,0,SMART_WAND_PARAM_RX_PACKET_SIZE);
   uint8_t return_value = wand_tx();
   //TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.param[0],SMART_WAND_PARAM_RX_PACKET_SIZE);
}

void test_wand_tx_param1 (void)
{
   // No test for this function. Just coverage
   tx_state = PARAM_1;
   // memset(&db.param[1],1,SMART_WAND_PARAM_RX_PACKET_SIZE);
   // uint8_t check_array[SMART_WAND_PARAM_RX_PACKET_SIZE];
   // memset(&check_array,0,SMART_WAND_PARAM_RX_PACKET_SIZE);
   uint8_t return_value = wand_tx();
   //TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.param[1],SMART_WAND_PARAM_RX_PACKET_SIZE);
}

void test_wand_tx_param2 (void)
{
   // No test for this function. Just coverage
   tx_state = PARAM_2;
   // memset(&db.param[2],1,SMART_WAND_PARAM_RX_PACKET_SIZE);
   // uint8_t check_array[SMART_WAND_PARAM_RX_PACKET_SIZE];
   // memset(&check_array,0,SMART_WAND_PARAM_RX_PACKET_SIZE);
   uint8_t return_value = wand_tx();
   //TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.param[2],SMART_WAND_PARAM_RX_PACKET_SIZE);
}

void test_wand_tx_param3 (void)
{
   // No test for this function. Just coverage
   tx_state = PARAM_3;
   // memset(&db.param[3],1,SMART_WAND_PARAM_RX_PACKET_SIZE);
   // uint8_t check_array[SMART_WAND_PARAM_RX_PACKET_SIZE];
   // memset(&check_array,0,SMART_WAND_PARAM_RX_PACKET_SIZE);
   uint8_t return_value = wand_tx();
   //TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.param[3],SMART_WAND_PARAM_RX_PACKET_SIZE);
}

void test_wand_tx_param4 (void)
{
   // No test for this function. Just coverage
   tx_state = PARAM_4;
   // memset(&db.param[4],1,SMART_WAND_PARAM_RX_PACKET_SIZE);
   // uint8_t check_array[SMART_WAND_PARAM_RX_PACKET_SIZE];
   // memset(&check_array,0,SMART_WAND_PARAM_RX_PACKET_SIZE);
   uint8_t return_value = wand_tx();
   //TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.param[4],SMART_WAND_PARAM_RX_PACKET_SIZE);
}

void test_wand_tx_STATIC_LOG_READ (void)
{
   // No test for this function. Just coverage
   tx_state = STATIC_LOG_READ;
   // memset(&db.param[4],1,SMART_WAND_STATIC_READ_RX_PACKET_SIZE);
   // uint8_t check_array[SMART_WAND_STATIC_READ_RX_PACKET_SIZE];
   // memset(&check_array,0,SMART_WAND_STATIC_READ_RX_PACKET_SIZE);
   uint8_t return_value = wand_tx();
   //TEST_ASSERT_EQUAL_MEMORY(&check_array,&db.param[4],SMART_WAND_STATIC_READ_RX_PACKET_SIZE);
}


void test_wand_static_read_0()
{
   uint8_t return_value = wand_static_read();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_use_limit_check(void)
{
   uint8_t return_value = wand_use_limit_check();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_use_limit_check_1(void)
{
   smart_wand_params.mfg.use_limit_time_en = 1;
   current_therapy_settings.wand.valid = TRUE;
   (*(uint32_t*)0x4003D000) = 0x10;
   smart_wand_params.mfg.use_limit_time = 1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
   uint8_t return_value = wand_use_limit_check();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_wand_use_limit_check_2(void)
{
   smart_wand_params.mfg.use_limit_time_en = 1;
   current_therapy_settings.wand.valid = TRUE;
   (*(uint32_t*)0x4003D000) = 0x10;
   smart_wand_params.mfg.use_limit_time = 1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
   static_log.unit[1].errors = WAND_LIFE_ERROR_TXT;
   db.static_log.page[11].data[5] = WAND_LIFE_ERROR_TXT;

   put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
   put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
   uint8_t return_value = wand_use_limit_check();
   TEST_ASSERT_EQUAL(1,return_value);
}


void test_wand_expired_check(void)
{
   put_event_ExpectAndReturn(&evt_tbl,WAND_EXPIRED,0);
   uint8_t return_value = wand_expired_check();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_smartwand_periodic_packet_handler(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl, WAND_PERIODIC_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_WRONG_TYPE(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x15;
   sum += UART0_D;
   // No functions called, it just exits out of the loop
   smartwand_periodic_packet_handler();
}




void test_smartwand_periodic_packet_handler_size_greater_than_0(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94; //1
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10; //2
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   // Needs a new put event expect function
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_size_greater_than_66(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x67; // hex 67 is greater than dec 67
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   // Needs a new put event expect function
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_size_greater_than_2(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x3; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //IDX=0
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //DATA
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //DATA
   sum += UART0_D;
   smartwand_periodic_packet_handler();

}

void test_smartwand_periodic_packet_handler_size_5(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x7; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //IDX=0
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //DATA0
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //DATA1
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //DATA2
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //DATA3
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //DATA4
   sum += UART0_D;
   smartwand_periodic_packet_handler();
}




void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_PARAM_DATA(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x3; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_PARAM_1_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_PARAM_DATA_KEY0_IDX0(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //IDX=0
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_PARAM_0_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_PARAM_DATA_KEY0_IDX1(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //IDX=0
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_PARAM_1_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_PARAM_DATA_KEY0_IDX2(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //IDX=0
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_PARAM_2_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_PARAM_DATA_KEY0_IDX3(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x3; //IDX=0
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;

   put_event_ExpectAndReturn(&evt_tbl,WAND_PARAM_3_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_PARAM_DATA_KEY0_IDX4(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x4; //IDX=4
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_PARAM_4_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_PARAM_DATA_KEY0_IDX5(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_PARAM_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x5; //IDX=4
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   // This is for the default case. No put event function is called
   smartwand_periodic_packet_handler();
}



void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_MFG_DATA(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x3; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_MFG_DATA; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_MFG_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_WRITE_CIRC_LOG(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x3; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_WRITE_CIRC_LOG; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;

   put_event_ExpectAndReturn(&evt_tbl,WAND_CIRCLE_WRITE_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_UNLOCK(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x3; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_UNLOCK; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_PASSCODE_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_UNLOCK_key_0(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_UNLOCK; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_PASSCODE_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_STATIC_LOG(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_STATIC_LOG; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_STATIC_READ_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_WRITE_STATIC_LOG(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_WRITE_STATIC_LOG; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_STATIC_WRITE_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_SMART_WAND_READ_CIRC_LOG(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_READ_CIRC_LOG; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   put_event_ExpectAndReturn(&evt_tbl,WAND_CIRCLE_READ_RX,1);
   smartwand_periodic_packet_handler();
}

void test_smartwand_periodic_packet_handler_cmd_BAD_CMD(void)
{
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x2; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = SMART_WAND_DEFAULT; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x1; //cmd = 0 = SMART_WAND_UNLOCK
   sum += UART0_D;
   smartwand_periodic_packet_handler();

   UART0_D = (~sum)+1;
   //put_event_ExpectAndReturn(&evt_tbl,WAND_CIRCLE_READ_RX,1);
   current_therapy_settings.wand.timeout = 0;
   smartwand_periodic_packet_handler();
}




void test_smartwand_periodic_packet_handler_bad_checksum (void)
{

   // False Checksum
   uint8_t sum = 0x0;
   smartwand_periodic_packet_handler();
   UART0_D = 0x94;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x10;
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Number
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Fluid temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Ambient temp
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //Keyboard status
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = 0x0; //size is 0 for a periodic update pkt
   sum += UART0_D;
   smartwand_periodic_packet_handler();
   UART0_D = (~sum); //Note no +1 here. So the checksum will be wrong
   smartwand_periodic_packet_handler();


}


void test_smartwand_send_packet_1(void)
{
   packet_queue.idx = 1;
   packet_queue.cmd[0].txcount = 0x5;
   packet_queue.cmd[0].size = 0x10;
   uint8_t byte = packet_queue.cmd[0].txcount;
   uint8_t packet_size = packet_queue.cmd[0].size - 1;
   packet_queue.idx = 1;
   smartwand_send_packet();
}

void test_smartwand_send_packet_2(void)
{
   packet_queue.idx = 1;
   //UART0_TCFIFO = 0xF<<UART_TCFIFO_TXCOUNT_MASK;
   packet_queue.cmd[0].txcount = 0x5;
   packet_queue.cmd[0].size = 0x10;
   uint8_t byte = packet_queue.cmd[0].txcount;
   uint8_t packet_size = packet_queue.cmd[0].size - 1;
   packet_queue.idx = 1;
   smartwand_send_packet();
}

void test_smartwand_send_packet_3(void)
{
   packet_queue.idx = 1;
   //UART0_TCFIFO = 0xF<<UART_TCFIFO_TXCOUNT_MASK;
   packet_queue.cmd[0].txcount = 0x15;
   packet_queue.cmd[0].size = 0x10;
   packet_queue.idx = 1;
   smartwand_send_packet();
}

void test_convert_q62_ambient_to_6bit(void)
{
   uint8_t q62_temperature = 62;
   uint8_t return_value = convert_q62_ambient_to_7bit(q62_temperature);
   TEST_ASSERT_EQUAL(26,return_value);

   q62_temperature = 218;
   return_value = convert_q62_ambient_to_7bit(q62_temperature);
   TEST_ASSERT_EQUAL(63,return_value);
}



void test_wand_static_read (void)
{
   static_log.type = 'B';
   wand_static_read ();
   //   TEST_ASSERT_EQUAL(ERASE_LOGS,tx_state);
}

void test_wand_static_read_1 (void)
{
   static_log.type = 'A';
   db.static_log.page[0].data[0] = 'A';
   db.static_log.page_read = STATIC_MAX_NUM_PAGES;
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);

   uint8_t return_value = wand_static_read ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_static_read_2 (void)
{
   static_log.type = 'A';
   db.static_log.page[0].data[0] = 'B';
   db.static_log.page_read = STATIC_MAX_NUM_PAGES;
   uint8_t return_value = wand_static_read ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_static_read_3 (void)
{
   static_log.type = 'A';
   db.static_log.page[0].data[0] = 'A';
   db.static_log.page_read = STATIC_MAX_NUM_PAGES-2;
   uint8_t return_value = wand_static_read ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_static_read_4 (void)
{
   static_log.type = 'A';
   db.static_log.page[0].data[0] = 'A';
   db.static_log.page_read = STATIC_MAX_NUM_PAGES;
   pSmartWandParams->mfg.expiration_date  = 0;
   (*(uint32_t*)0x4003D000) = 0x10; //wand
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);

   uint8_t return_value = wand_static_read ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_static_read_5 (void)
{
   static_log.type = 'A';
   db.static_log.page[0].data[0] = 'A';
   db.static_log.page_read = STATIC_MAX_NUM_PAGES;
   pSmartWandParams->mfg.use_limit_time_en = 0;
   pSmartWandParams->mfg.expiration_date  = 25;
   (*(uint32_t*)0x4003D000) = 0; //wand
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = wand_static_read ();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_wand_periodic_packet_parse (void)
{
   get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
   //db.periodic.temp2 = 0;
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_periodic_packet_parse_ablate_1 (void)
{
   current_therapy_settings.treatment.finger_switch_en = 1;
   smart_wand_params.mfg.finger_switch_enable = 1;
   db.periodic.key |=  ABLATE_KEY_MASK;
   current_therapy_settings.wand.cut_key = 0;
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();

}

void test_wand_periodic_packet_parse_ablate_1_disable (void)
{
   current_therapy_settings.treatment.finger_switch_en = 1;
   smart_wand_params.mfg.finger_switch_enable = 0;
   db.periodic.key |=  ABLATE_KEY_MASK;
   current_therapy_settings.wand.cut_key = 0;
   smart_wand_params.mfg.ambient_enable = 0;
   smart_wand_params.mfg.tube_temperature_enable = 0;
   
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
   current_therapy_settings.treatment.finger_switch_en = 1;
   current_therapy_settings.wand.cut_key = 1;
   smart_wand_params.mfg.ambient_enable = 1;
   smart_wand_params.mfg.tube_temperature_enable = 1;
}

void test_wand_periodic_packet_parse_ablate_0 (void)
{
   db.periodic.key &=  ~ABLATE_KEY_MASK;
   current_therapy_settings.wand.cut_key = 1;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_COAG_1 (void)
{
   db.periodic.key |=  COAG_KEY_MASK;
   current_therapy_settings.wand.cut_key = 0;
   throttle_therapy_on = 0;
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_COAG_0 (void)
{
   db.periodic.key &=  ~COAG_KEY_MASK;
   current_therapy_settings.wand.coag_key = 1;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_temp2_0 (void)
{
   db.periodic.key &=  ~COAG_KEY_MASK;
   current_therapy_settings.wand.coag_key = 1;
   uint8_t return_value;
   put_event_IgnoreAndReturn(1);
   db.periodic.temp2 = 1; // This resets the no_ambient_packet_count
   get_priming_status_IgnoreAndReturn(0);
 return_value = wand_periodic_packet_parse();
   // Set db.periodic.temp2 to 0 and call the function 51 times

   for (uint8_t i = 0; i < 50; i++)
   {
      put_event_IgnoreAndReturn(1);
      db.periodic.temp2 = 0;
      get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
   }
   
   put_event_ExpectAndReturn(&evt_tbl,NO_AMBIENT_SENSOR,1);
   db.periodic.temp2 = 0;
   get_priming_status_IgnoreAndReturn(0);
    return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_ablate_on (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 1;
   get_priming_status_IgnoreAndReturn(0);
 return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_coag_on (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   smart_wand_params.mfg.tube_temperature_enable = 1;
   current_therapy_settings.wand.handle_temp = 0x10;
   pSmartWandParams->handle.high_warn_en =0x01;
   get_priming_status_IgnoreAndReturn(0);
 return_value = wand_periodic_packet_parse();
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.wand.tube_alert);

}

void test_wand_periodic_packet_parse_coag_on_med_warn (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 1;

   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x07;
   pSmartWandParams->handle.high_warn_en =0x10;
   pSmartWandParams->handle.med_warn_en = 0x05;
   current_therapy_settings.wand.tube_alert =FALSE;

   get_priming_status_IgnoreAndReturn(0);
 return_value = wand_periodic_packet_parse();
   //TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.wand.tube_alert);

}

void test_wand_periodic_packet_parse_coag_on_med_warn_hysteresis (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 1;

   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;
   pSmartWandParams->handle.high_warn_en =0x10;
   pSmartWandParams->handle.med_warn_en = 0x07;
   current_therapy_settings.wand.tube_alert =FALSE;

   get_priming_status_IgnoreAndReturn(0);
 return_value = wand_periodic_packet_parse();
   //TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.wand.tube_alert);

}

void test_wand_periodic_packet_parse_coag_on_med_warn_hysteresis_1 (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 1;
   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;

   pSmartWandParams->handle.high_warn_en =0x10;
   pSmartWandParams->handle.med_warn_en = 0x07;
   current_therapy_settings.wand.tube_alert =TRUE;

   get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
   //TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.wand.tube_alert);

}

void test_wand_periodic_packet_parse_coag_on_med_warn_tube_count (void)
{
   uint8_t return_value;
   for (uint8_t i = 0; i < 110; i++)
   {

      put_event_IgnoreAndReturn(1);
      current_therapy_settings.treatment.ablate_on = 0;
      pSmartWandParams->handle.warn_disable =  0x02;
      current_therapy_settings.treatment.coag_on = 1;
      //handle = (uint16_t) db.periodic.temp1;
      db.periodic.temp1 = 0x08;

      pSmartWandParams->handle.high_warn_en =0x10;
      pSmartWandParams->handle.med_warn_en = 0x07;
      current_therapy_settings.wand.tube_alert =FALSE;
get_priming_status_IgnoreAndReturn(0);
      return_value = wand_periodic_packet_parse();
   }
}

void test_wand_periodic_packet_parse_coag_on_handle_warn_disable (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 1;
   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;
   pSmartWandParams->handle.warn_disable = 0x07;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_warn = TRUE;

   pSmartWandParams->handle.high_warn_en =0x10;
   pSmartWandParams->handle.med_warn_en = 0x07;
   current_therapy_settings.wand.tube_alert =TRUE;
get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.pump.ctrl_en);
}

void test_wand_periodic_packet_parse_coag_on_handle_warn_disable_pumpen_true (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 1;
   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;
   pSmartWandParams->handle.warn_disable = 0x07;
   current_therapy_settings.pump.ctrl_en = TRUE;
   current_therapy_settings.wand.tube_warn = TRUE;

   pSmartWandParams->handle.high_warn_en =0x10;
   pSmartWandParams->handle.med_warn_en = 0x07;
   current_therapy_settings.wand.tube_alert =TRUE;
get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
   TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.pump.ctrl_en);
}

void test_wand_periodic_packet_parse_coag_on_handle_warn_disable_pumpen_false (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 1;
   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;
   pSmartWandParams->handle.warn_disable = 0x07;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.tube_warn = FALSE;

   pSmartWandParams->handle.high_warn_en =0x10;
   pSmartWandParams->handle.med_warn_en = 0x07;
   current_therapy_settings.wand.tube_alert =TRUE;
get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.pump.ctrl_en);
}

void test_wand_periodic_packet_parse_mode_key_0 (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 0;
   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;
   db.periodic.key &= ~MODE_KEY_MASK;
   current_therapy_settings.wand.mode_key = 0;
   get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.wand.mode_key);
}

void test_wand_periodic_packet_parse_mode_key_1 (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 0;
   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;
   db.periodic.key |= MODE_KEY_MASK;
   current_therapy_settings.wand.mode_key = 0;
   get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.wand.mode_key);
}

void test_wand_periodic_packet_parse_mode_key_VAC_MODE_PACKET_DELAY (void)
{
   uint8_t return_value;
   for (uint32_t i = 0; i < VAC_MODE_PACKET_DELAY-3 ; i++)
   {
      put_event_IgnoreAndReturn(1);
      current_therapy_settings.treatment.ablate_on = 0;
      pSmartWandParams->handle.warn_disable =  0x02;
      current_therapy_settings.treatment.coag_on = 0;
      //handle = (uint16_t) db.periodic.temp1;
      db.periodic.temp1 = 0x06;
      db.periodic.key |= MODE_KEY_MASK;
      current_therapy_settings.wand.mode_key = 0;
	  get_priming_status_IgnoreAndReturn(0);
      return_value = wand_periodic_packet_parse();
   }

   put_event_ExpectAndReturn(&evt_tbl,CHANGE_MODE,1);
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 0;
   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;
   db.periodic.key &= ~MODE_KEY_MASK;
   current_therapy_settings.wand.mode_key = 0;
   get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_mode_key_VAC_MODE_PACKET_DELAY_high (void)
{
   uint8_t return_value;
   for (uint32_t i = 0; i < VAC_MODE_PACKET_DELAY+3 ; i++)
   {
      put_event_IgnoreAndReturn(1);
      current_therapy_settings.treatment.ablate_on = 0;
      pSmartWandParams->handle.warn_disable =  0x02;
      current_therapy_settings.treatment.coag_on = 0;
      //handle = (uint16_t) db.periodic.temp1;
      db.periodic.temp1 = 0x06;
      db.periodic.key |= MODE_KEY_MASK;
      current_therapy_settings.wand.mode_key = 0;
	  get_priming_status_IgnoreAndReturn(0);
      return_value = wand_periodic_packet_parse();
   }

   put_event_IgnoreAndReturn(1);
   current_therapy_settings.treatment.ablate_on = 0;
   pSmartWandParams->handle.warn_disable =  0x02;
   current_therapy_settings.treatment.coag_on = 0;
   //handle = (uint16_t) db.periodic.temp1;
   db.periodic.temp1 = 0x06;
   db.periodic.key &= ~MODE_KEY_MASK;
   current_therapy_settings.wand.mode_key = 0;
   get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_wand_locked (void)
{
   //db.periodic.key &=  ~COAG_KEY_MASK;
   current_therapy_settings.treatment.finger_switch_en = 0;
   current_therapy_settings.wand.coag_key = 1;
   current_therapy_settings.wand.unlocked = TRUE;
   current_therapy_settings.active_wand.lw_active = 0;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   
   uint8_t return_value;
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   //db.periodic.temp2 = 1; // This resets the no_ambient_packet_count
   return_value = wand_periodic_packet_parse();
   // Set db.periodic.temp2 to 0 and call the function 51 times

   for (uint8_t i = 0; i < 50; i++)
   {
      put_event_IgnoreAndReturn(1);
      //db.periodic.temp2 = 0;
	  get_priming_status_IgnoreAndReturn(0);
      return_value = wand_periodic_packet_parse();
   }
   
   //put_event_ExpectAndReturn(&evt_tbl,NO_AMBIENT_SENSOR,1);
   //db.periodic.temp2 = 0;
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_finger_switch_disable (void)
{
	uint8_t return_value;
	smart_wand_params.mfg.finger_switch_enable = 0;
	smart_wand_params.mfg.ambient_enable = 0;
	smart_wand_params.mfg.tube_temperature_enable = 0;
	current_therapy_settings.active_wand.lw_active = 1;
	current_therapy_settings.wand.mode_key = 1;
	get_priming_status_IgnoreAndReturn(0);
	return_value = wand_periodic_packet_parse();
	
}

void test_wand_periodic_packet_parse_one_sec_count_50 (void)
{
	uint8_t return_value;
	static uint8_t one_sec_count = 50;
	current_therapy_settings.active_wand.lw_active = 0;
	current_therapy_settings.wand.valid = TRUE;
	current_therapy_settings.wand.unlocked = TRUE;
	legacy_wand_params_obj.wand_connected = 0;
	current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
	for (uint8_t i = 0; i < 50; i++)
	{
		get_priming_status_IgnoreAndReturn(0);
		return_value = wand_periodic_packet_parse();
	}	
}

void test_wand_periodic_packet_parse_one_sec_count_50_2 (void)
{
	uint8_t return_value;
	static uint8_t one_sec_count = 50;
	current_therapy_settings.active_wand.lw_active = 0;
	current_therapy_settings.wand.valid = TRUE;
	current_therapy_settings.wand.unlocked = TRUE;
	legacy_wand_params_obj.wand_connected = 0;
	current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
	for (uint8_t i = 0; i < 50; i++)
	{
		get_priming_status_IgnoreAndReturn(0);
		return_value = wand_periodic_packet_parse();
	}	
}

void test_wand_periodic_packet_parse_one_sec_count_50_3 (void)
{
	uint8_t return_value;
	static uint8_t one_sec_count = 50;
	current_therapy_settings.active_wand.lw_active = 1;
	current_therapy_settings.wand.valid = FALSE;
	current_therapy_settings.wand.unlocked = TRUE;
	legacy_wand_params_obj.wand_connected = 0;
	current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
	for (uint8_t i = 0; i < 50; i++)
	{
		get_priming_status_IgnoreAndReturn(0);
		return_value = wand_periodic_packet_parse();
	}	
}

void test_wand_periodic_packet_parse_one_sec_count_50_4 (void)
{
	uint8_t return_value;
	static uint8_t one_sec_count = 50;
	current_therapy_settings.active_wand.lw_active = 1;
	current_therapy_settings.wand.valid = FALSE;
	current_therapy_settings.wand.unlocked = TRUE;
	legacy_wand_params_obj.wand_connected = 1;
	current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
	for (uint8_t i = 0; i < 50; i++)
	{
		get_priming_status_IgnoreAndReturn(0);
		return_value = wand_periodic_packet_parse();
	}	
}

void test_wand_periodic_packet_parse_mode_key_true_1 (void)
{
	current_therapy_settings.wand.mode_key = 1;
	current_therapy_settings.active_wand.lw_active = 0;
	smart_wand_params.mfg.ambient_enable = 0;
	smart_wand_params.mfg.tube_temperature_enable = 0;
	current_therapy_settings.treatment.ablate_on = 0;
	current_therapy_settings.treatment.coag_on = 0;
	current_therapy_settings.wand.valid = FALSE;
	current_therapy_settings.wand.unlocked = TRUE;
	legacy_wand_params_obj.wand_connected = 1;
	current_therapy_settings.treatment.finger_switch_en = 1;
	smart_wand_params.mfg.finger_switch_enable = 1;
	db.periodic.key &=  ~ABLATE_KEY_MASK;
	db.periodic.key |=  COAG_KEY_MASK;
	db.periodic.key |=  MODE_KEY_MASK;
	current_therapy_settings.wand.arm_buttons = 1;
	put_event_IgnoreAndReturn(1);
	get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
	
	for (uint8_t i = 0; i < 5; i++)
	{
		put_event_IgnoreAndReturn(1);
		get_priming_status_IgnoreAndReturn(0);
		wand_periodic_packet_parse();
	}
	current_therapy_settings.wand.arm_buttons = 0;
	current_therapy_settings.wand.mode_key = 1;
	// db.periodic.key &=  ~MODE_KEY_MASK;
	get_priming_status_IgnoreAndReturn(0);
	put_event_IgnoreAndReturn(1);
	wand_periodic_packet_parse();	
}

void test_wand_periodic_packet_parse_mode_key_true_2 (void)
{
	current_therapy_settings.active_wand.lw_active = 0;
	current_therapy_settings.wand.valid = FALSE;
	current_therapy_settings.wand.unlocked = TRUE;
	legacy_wand_params_obj.wand_connected = 1;
	current_therapy_settings.treatment.finger_switch_en = 1;
	smart_wand_params.mfg.finger_switch_enable = 1;
	db.periodic.key &=  ~ABLATE_KEY_MASK;
	db.periodic.key |=  COAG_KEY_MASK;
	db.periodic.key |=  MODE_KEY_MASK;
	current_therapy_settings.wand.arm_buttons = 1;
	put_event_IgnoreAndReturn(1);
	get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
	
	for (uint8_t i = 0; i < 255; i++)
	{
		put_event_IgnoreAndReturn(1);
		get_priming_status_IgnoreAndReturn(0);
		wand_periodic_packet_parse();
	}
	current_therapy_settings.wand.arm_buttons = 0;
	current_therapy_settings.wand.mode_key = 1;
	db.periodic.key &=  ~MODE_KEY_MASK;
	put_event_IgnoreAndReturn(1);
	get_priming_status_IgnoreAndReturn(0);
	wand_periodic_packet_parse();
}



void test_wand_periodic_packet_parse_mode_ablate_true (void)
{
	current_therapy_settings.treatment.finger_switch_en = 1;
	smart_wand_params.mfg.finger_switch_enable = 1;
	db.periodic.key |=  ABLATE_KEY_MASK;
	current_therapy_settings.wand.cut_key = 0;
	current_therapy_settings.wand.arm_buttons = 1;
	smart_wand_params.mfg.low_enable = TRUE;
	//put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_mode_all_key_false (void)
{
	current_therapy_settings.treatment.finger_switch_en = 1;
	smart_wand_params.mfg.finger_switch_enable = 1;
	db.periodic.key &=  ~ABLATE_KEY_MASK;
	db.periodic.key &=  ~COAG_KEY_MASK;
	db.periodic.key &=  ~MODE_KEY_MASK;
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);

 get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_mode_key_true_3 (void)
{
	current_therapy_settings.wand.mode_key = 1;
	smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
	current_therapy_settings.treatment.pre_prime = FALSE;
	current_therapy_settings.active_wand.lw_active = 0;
	smart_wand_params.mfg.ambient_enable = 0;
	smart_wand_params.mfg.tube_temperature_enable = 0;
	current_therapy_settings.treatment.ablate_on = 0;
	current_therapy_settings.treatment.coag_on = 0;
	current_therapy_settings.wand.valid = TRUE;
	current_therapy_settings.wand.unlocked = TRUE;
	legacy_wand_params_obj.wand_connected = 1;
	current_therapy_settings.treatment.finger_switch_en = 1;
	smart_wand_params.mfg.finger_switch_enable = 1;
	db.periodic.key &=  ~ABLATE_KEY_MASK;
	db.periodic.key |=  COAG_KEY_MASK;
	db.periodic.key |=  MODE_KEY_MASK;
	current_therapy_settings.wand.arm_buttons = 1;
	put_event_IgnoreAndReturn(1);
	get_priming_status_IgnoreAndReturn(0);
 uint8_t return_value = wand_periodic_packet_parse();
	
	for (uint8_t i = 0; i < 5; i++)
	{
		put_event_IgnoreAndReturn(1);
		get_priming_status_IgnoreAndReturn(0);
		wand_periodic_packet_parse();
	}
	current_therapy_settings.wand.arm_buttons = 0;
	current_therapy_settings.wand.mode_key = 1;
	// db.periodic.key &=  ~MODE_KEY_MASK;
	get_priming_status_IgnoreAndReturn(0);
	put_event_IgnoreAndReturn(1);
	wand_periodic_packet_parse();	
}

void test_set_use_limit_time (void)
{
	uint32_t *ptr;
	uint32_t wand_connect_time = *ptr;
	wand_connect_time = 1;
    uint32_t return_value = set_use_limit_time((&wand_connect_time));
}

void test_wand_time_limit_check (void)
{
	smart_wand_params.mfg.use_limit_activation_en = 1;
	smart_wand_params.mfg.max_activation_seconds = 0;
	db.mfg.data[18] = 1;
	uint8_t* active_time; 
	uint16_t* p_csum;
	active_time[0] = 2;
	
	put_event_IgnoreAndReturn(1);
	put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
	uint8_t return_value = wand_time_limit_check (active_time, p_csum);
	
}

void test_wand_time_limit_check_manta (void)
{
	smart_wand_params.mfg.use_limit_activation_en = 1;
	smart_wand_params.mfg.max_activation_seconds = 30;
	db.mfg.data[18] = 1;
	db.mfg.data[46] = 2;
	uint8_t* active_time; 
	uint16_t* p_csum;
	active_time[0] = 31;
	
	put_event_IgnoreAndReturn(1);
	put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
	uint8_t return_value = wand_time_limit_check (active_time, p_csum);
	
}

void test_validate_manta_pwm_values(void)
{
	manta_pwm_mod = 210;
	manta_pwm_C0V = 63;
	manta_pwm_C1V = 155;
	manta_pwm_C2V = 60;
	manta_pwm_C3V = 155;
	manta_pwm_C4V = 50;
	manta_pwm_C5V = 168;
	manta_pwm_C6V = 50;
	manta_pwm_C7V = 65;
	validate_manta_pwm_values();
	
	manta_pwm_C1V = 158;
	validate_manta_pwm_values();
	
	manta_pwm_C1V = 155;
	manta_pwm_C3V = 63;
	validate_manta_pwm_values();
	
	manta_pwm_mod = 740;
	manta_pwm_C3V = 155;
	validate_manta_pwm_values();
	
	manta_pwm_mod = 210;
	manta_pwm_C7V = 211;
	validate_manta_pwm_values();
	
	manta_pwm_C7V = 65;
}

void test_manta_boundary_limit_flow_vals(void)
{
	manta_boundary_limit_flow_vals();
}

void test_deserialize_manta_prime_params(void)
{
	//MANTA_PRIME_PARAMS_T *return_value;
	deserialize_manta_prime_params(&db.param[0].head, &smart_wand_params.manta.prime);
}

void test_deserialize_manta_z_params(void)
{
	deserialize_manta_z_params(&db.param[0].head, &smart_wand_params.impedance);
}

void test_deserialize_manta_coag_params(void)
{
	deserialize_manta_coag_params(&db.param[0].head, &smart_wand_params.manta.coag[1]);
}

void test_manta_coag_mode_increment(void)
{
	current_therapy_settings.setpoint.coag = 10;
	manta_coag_mode_increment();
	
	current_therapy_settings.setpoint.coag = 11;
	manta_coag_mode_increment();
}

void test_deserialize_manta_ipeak_params(void)
{
	deserialize_manta_ipeak_params(&db.param[0].head, &smart_wand_params.manta.ipeak);
}

void test_wand_periodic_packet_parse_ablate_on_manta (void)
{
   uint8_t return_value;
   db.mfg.data[46] = 2;
   PrimeState = PRIME_AUTO;
   current_therapy_settings.treatment.ablate_on = 1;
   get_priming_status_IgnoreAndReturn(0);
 return_value = wand_periodic_packet_parse();
}

void test_wand_periodic_packet_parse_coag_on_manta (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   smart_wand_params.mfg.tube_temperature_enable = 1;
   current_therapy_settings.wand.handle_temp = 0x10;
   pSmartWandParams->handle.high_warn_en =0x01;
   get_priming_status_IgnoreAndReturn(0);
 return_value = wand_periodic_packet_parse();
   TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.wand.tube_alert);
}


void test_wand_wear_monitor (void)
{
   uint8_t return_value = wand_wear_monitor();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_circle_log_read (void)
{
   uint8_t return_value = wand_circle_log_read();
   TEST_ASSERT_EQUAL(1,return_value);
}


void test_float2half(void)
{
   float32_t value = 3.1415;
   uint16_t return_value = float2half(value);
   float32_t converted_value = half2float(return_value);
   TEST_ASSERT_EQUAL_FLOAT(3.140625,converted_value);
}

void test_wand_circle_write(void)
{
   //db.circle_log.write_echo.idx = 1;
   uint8_t return_value = wand_circle_write();
   TEST_ASSERT_EQUAL(0,return_value);
   db.circle_log.write_echo.idx = 0;
   return_value = wand_circle_write();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_switch_to_ww_wand (void)
{
   uint8_t return_value = switch_to_ww_wand();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_periodic_log_errors_to_wand (void)
{
   current_therapy_settings.notify.status = 1;
   wand_error_type_decipher_ExpectAndReturn(current_therapy_settings.notify.status,1);
   uint8_t return_value =  periodic_log_errors2wand();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_worn_use_limit(void)
{
   current_therapy_settings.wand.worn_timer_en = 1;
   current_therapy_settings.wand.worn_active_time =  250;
   uint8_t return_value = wand_worn_use_limit();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_worn_use_limit_2(void)
{
   current_therapy_settings.wand.worn_timer_en = 1;
   current_therapy_settings.wand.worn_active_time =  250;
   //   current_therapy_settings.wand.worn = 5;
   //put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
   uint8_t return_value = wand_worn_use_limit();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_worn_use_limit_3(void)
{
   current_therapy_settings.wand.worn_timer_en = 1;
   current_therapy_settings.wand.worn_active_time =  250;
   current_therapy_settings.wand.thandle_wear = TRUE;
   current_therapy_settings.wand.irms_wear = TRUE;
   current_therapy_settings.wand.worn_timer_en = 0;
   //   current_therapy_settings.wand.worn = 5;
   //put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
   uint8_t return_value = wand_worn_use_limit();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_worn_use_limit_4(void)
{
   current_therapy_settings.wand.worn_timer_en = 1;
   smart_wand_params.worn.detect_duration = 0;
   current_therapy_settings.wand.irms_worn_count = 0;
   current_therapy_settings.wand.worn_active_time =  250;
   current_therapy_settings.wand.thandle_wear = TRUE;
   current_therapy_settings.wand.irms_wear = FALSE;
   current_therapy_settings.wand.worn_timer_en = 0;
   //   current_therapy_settings.wand.worn = 5;
   //put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
   uint8_t return_value = wand_worn_use_limit();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_worn_use_limit_5(void)
{
   smart_wand_params.worn.detect_duration = 0;
   current_therapy_settings.wand.irms_worn_count = 0;
   current_therapy_settings.wand.worn_active_time =  0xFF;
   current_therapy_settings.wand.thandle_wear = TRUE;
   current_therapy_settings.wand.irms_wear = TRUE;
   current_therapy_settings.wand.worn_timer_en = 1;
   //   current_therapy_settings.wand.worn = 5;
   //put_event_ExpectAndReturn(&evt_tbl,WAND_USE_LIMIT_ERROR,1);
   uint8_t return_value = wand_worn_use_limit();
   TEST_ASSERT_EQUAL(0,return_value);
   
   current_therapy_settings.wand.worn_active_time =  61;
   return_value = wand_worn_use_limit();
}

void test_wand_not_active_periodic_parse (void)
{
	put_event_IgnoreAndReturn(1);
   uint8_t return_value = wand_not_active_periodic_packet_parse();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_log_use_data (void)
{
   uint32_t start_time = 0;
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   log_use_data(start_time); // NO return, so no test right now
}

void test_wand_wear_irms (void)
{
   uint8_t mode_change = 0;
   
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.therapy_mode =  LO;
   current_therapy_settings.setpoint.dac = 0x100;
   smart_wand_params.impedance.z_level = 0x10;
   smart_wand_params.mfg.wand_wear_enable = 1;
   //arm_fir_q15_IgnoreAndReturn(1);
   uint8_t reset_flag = 0;
   uint8_t return_value;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
}

void test_wand_wear_irms_2 (void)
{
   uint8_t mode_change = 1;
   
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.therapy_mode =  LO;
   current_therapy_settings.setpoint.dac = 0x100;
   smart_wand_params.impedance.z_level = 0x10;
   smart_wand_params.mfg.wand_wear_enable = 1;
   //arm_fir_q15_IgnoreAndReturn(1);
   uint8_t reset_flag = 0;
   uint8_t return_value;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
   
   current_therapy_settings.treatment.therapy_mode = HI;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
   
   current_therapy_settings.treatment.therapy_mode = MED;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
   
   current_therapy_settings.treatment.therapy_mode = HI;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
   
   current_therapy_settings.treatment.therapy_mode = VAC;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
   
   reset_flag, mode_change = 0;
   
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.therapy_mode = COAG;
   
   current_therapy_settings.setpoint.dac = 0x100;
   smart_wand_params.mfg.wand_wear_enable = 0;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change); 
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.therapy_mode = VAC;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.therapy_mode = COAG;
   current_therapy_settings.setpoint.dac = 0x05;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change); 
}


void test_wand_wear_irms_3 (void)
{
   uint8_t reset_flag = 0;
   uint8_t mode_change = 0;
   
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.therapy_mode =  LO;
   current_therapy_settings.setpoint.dac = 0x100;
   smart_wand_params.impedance.z_level = 0x10;
   smart_wand_params.mfg.wand_wear_enable = 1;
   //arm_fir_q15_Ignore();
   
   uint8_t return_value;
   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
   
   for (uint8_t i = 0; i<100; i++)
   {
	   
	   return_value = wand_irms_wear_monitor(reset_flag,mode_change);
   }
}


void test_wand_thandle_wear_monitor (void)
{
   uint8_t handle_temp = 10;
   uint8_t reset_flag = 1;
   uint8_t mode_change = 0;
   
   current_therapy_settings.treatment.therapy_mode = LO;
   uint8_t return_value = wand_thandle_wear_monitor( handle_temp,
                                                     reset_flag,
                                                     mode_change);
   // Since reset flag is 1, worn_flag = 0, which is returned.
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_thandle_wear_monitor_2 (void)
{
   uint8_t handle_temp = 20;
   uint8_t reset_flag = 0;
   uint8_t mode_change = 0;
   current_therapy_settings.wand.thandle_wear = 0;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.therapy_mode =  LO;
   current_therapy_settings.setpoint.dac = 0x100;
   smart_wand_params.impedance.z_level = 0x10;
   smart_wand_params.mfg.wand_wear_enable = 1;
   uint8_t return_value = wand_thandle_wear_monitor( handle_temp,
                                                     reset_flag,
                                                     mode_change);

   // Since current_therapy_settings.wand.thandle_wear is 0,
   // worn_flag = 0, which is returned.
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_thandle_wear_monitor_3 (void)
{
   uint8_t handle_temp = 0;
   uint8_t reset_flag = 0;
   uint8_t mode_change = 0;
   current_therapy_settings.wand.thandle_wear = 0;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.therapy_mode =  LO;
   current_therapy_settings.setpoint.dac = 0x100;
   smart_wand_params.impedance.z_level = 0x10;
   smart_wand_params.mfg.wand_wear_enable = 1;
   uint8_t return_value;
   //put_event_ExpectAndReturn(&evt_tbl,CHECK_ELECTRODE_ERROR,1);
   //put_event_ExpectAndReturn(&evt_tbl,CHECK_ELECTRODE_ERROR,1);
   for (uint8_t j = 1; j < 8; j++)
   {
      for (uint8_t i = 10; i < 111; i++)
      {
         return_value = wand_thandle_wear_monitor(abs(i
                                                      -j),reset_flag, mode_change);
      }
   }
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_log_use_data_high_duration(void)
{
   uint32_t start_time = 10;
   *((uint32_t*)0x4003D000)= (uint32_t)0x3850;
   RTC_TSR = 0xFFFFF;
   //no test for this condition
   for (uint8_t i = 0; i < 20; i++)
   {
      *((uint32_t*)0x4003D000)= (uint32_t)0x3850;
      current_therapy_settings.log.mode = COAG;
      current_therapy_settings.log.level = 0;
      current_therapy_settings.log.start_time = 0;
   }
   TEST_IGNORE_MESSAGE("Need test");
}

void test_log_use_data_log_mode(void)
{
   put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   uint32_t start_time = 0;
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition

   uint8_t bin = 15;//mode_level_bins[current_therapy_settings.log.mode][current_therapy_settings.log.level];
   db.circle_log.mode_active_accum[bin] = 15;
   log_use_data(start_time);




}

void test_log_use_data_log_mode_idle_time_less_than_3F(void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //   no test for this condition
   uint8_t bin = 15;
   db.circle_log.mode_active_accum[bin] = 15;

   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 62;

   log_use_data(start_time);




}


void test_log_use_data_log_mode_idle_time_5x_less_than_3F(void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition
   uint8_t bin = 15;

   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 62*5;

   log_use_data(start_time);




}

void test_log_use_data_log_mode_idle_time_mins_less_than_3F(void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition
   uint8_t bin = 15;
   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 62*60;

   log_use_data(start_time);
}

void test_log_use_data_log_mode_idle_time_hours_less_than_3F(void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition
   uint8_t bin = 15;
   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 62*3600;

   log_use_data(start_time);
}


void test_log_use_data_log_mode_idle_time_hours_more_than_3F(void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition
   uint8_t bin = 15;
   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 65*3600;

   log_use_data(start_time);
}


void test_log_use_data_log_mode_circle_log(void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition
   uint8_t bin = 15;
   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 62;
   db.circle_log.idx = 0;
   log_use_data(start_time);

   db.circle_log.idx = 1;
   log_use_data(start_time);




}

void test_log_use_data_log_page_192(void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition
   uint8_t bin = 15;
   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 62;
   db.circle_log.idx = 0;
   log_use_data(start_time);

   db.circle_log.idx = 0;
   db.circle_log.page = 192;

   log_use_data(start_time);

}
void test_log_use_data_log_idx_2(void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition
   uint8_t bin = 15;
   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 62;
   db.circle_log.idx = 0;
   log_use_data(start_time);

   db.circle_log.idx = 2;
   db.circle_log.page = 191;

   log_use_data(start_time);



   TEST_ASSERT_EQUAL(2,db.circle_log.idx);
}

void test_log_use_data_log_csum(void)
{
	put_event_IgnoreAndReturn(1);
   (*(uint32_t*)0x4003D000)= 3850;
   //no test for this condition
   uint8_t bin = 15;
   current_therapy_settings.log.start_time = 0;
   uint32_t start_time = 62;
   db.circle_log.idx = 0;
   pSmartWandParams->mfg.use_limit_activation_en = 1;
   pSmartWandParams->mfg.max_activation_seconds = 70000;
   db.circle_log.idx = 2;
   db.circle_log.page = 191;

   log_use_data(start_time);



   TEST_ASSERT_EQUAL(2,db.circle_log.idx);
}


void test_wand_not_active_periodic_packet_parse (void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   uint8_t return_value = wand_not_active_periodic_packet_parse();
   //db.periodic.temp2 = 0;
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_wand_not_active_periodic_packet_parse_ablate_1 (void)
{
   current_therapy_settings.treatment.finger_switch_en = 1;
   smart_wand_params.mfg.finger_switch_enable = 1;
   db.periodic.key |=  ABLATE_KEY_MASK;
   current_therapy_settings.wand.cut_key = 0;
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   uint8_t return_value = wand_not_active_periodic_packet_parse();
}

void test_wand_not_active_periodic_packet_parse_ablate_0 (void)
{
   db.periodic.key &=  ~ABLATE_KEY_MASK;
   current_therapy_settings.wand.cut_key = 1;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = wand_not_active_periodic_packet_parse();
}

void test_wand_not_active_periodic_packet_parse_COAG_1 (void)
{
   db.periodic.key |=  COAG_KEY_MASK;
   current_therapy_settings.wand.cut_key = 0;
   put_event_IgnoreAndReturn(1);
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   uint8_t return_value = wand_not_active_periodic_packet_parse();
}

void test_wand_not_active_periodic_packet_parse_COAG_0 (void)
{
   db.periodic.key &=  ~COAG_KEY_MASK;
   current_therapy_settings.wand.coag_key = 1;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   uint8_t return_value = wand_not_active_periodic_packet_parse();
}

void test_switch_to_ww_wand_1(void)
{
   smart_wand_common_params.ambient_sensor_flag = 1;
   uint8_t return_value = switch_to_ww_wand();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_get_ww_temp (void)
{
   db.param[4].temp2 = 30;
   uint8_t return_value = get_ww_temp ();
   TEST_ASSERT_EQUAL(18,return_value);
}

// void test_validate_manta_pwm_values(void)
// {
	// manta_pwm_mod = 210;
	// manta_pwm_C0V = 63;
	// manta_pwm_C1V = 155;
	// manta_pwm_C2V = 60;
	// manta_pwm_C3V = 155;
	// manta_pwm_C4V = 50;
	// manta_pwm_C5V = 168;
	// manta_pwm_C6V = 50;
	// manta_pwm_C7V = 65;
	// validate_manta_pwm_values();
	
	// manta_pwm_C1V = 158;
	// validate_manta_pwm_values();
	
	// manta_pwm_C1V = 155;
	// manta_pwm_C3V = 63;
	// validate_manta_pwm_values();
	
	// manta_pwm_mod = 740;
	// manta_pwm_C3V = 155;
	// validate_manta_pwm_values();
	
	// manta_pwm_mod = 210;
	// manta_pwm_C7V = 211;
	// validate_manta_pwm_values();
	
	// manta_pwm_C7V = 65;
// }

// void test_manta_boundary_limit_flow_vals(void)
// {
	// manta_boundary_limit_flow_vals();
// }

// void test_deserialize_manta_prime_params(void)
// {
	// //MANTA_PRIME_PARAMS_T *return_value;
	// deserialize_manta_prime_params(&db.param[0].head, &smart_wand_params.manta.prime);
// }

// void test_deserialize_manta_z_params(void)
// {
	// deserialize_manta_z_params(&db.param[0].head, &smart_wand_params.impedance);
// }

// void test_deserialize_manta_coag_params(void)
// {
	// deserialize_manta_coag_params(&db.param[0].head, &smart_wand_params.manta.coag[1]);
// }

// void test_manta_coag_mode_increment(void)
// {
	// current_therapy_settings.setpoint.coag = 10;
	// manta_coag_mode_increment();
	
	// current_therapy_settings.setpoint.coag = 11;
	// manta_coag_mode_increment();
// }

// void test_deserialize_manta_ipeak_params(void)
// {
	// deserialize_manta_ipeak_params(&db.param[0].head, &smart_wand_params.manta.ipeak);
// }

void test_dummy (void)
{
#if _BullseyeCoverage
   cov_dumpData();
#endif

}
