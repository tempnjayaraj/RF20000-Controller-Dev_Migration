#include "unity.h"
#include "unity_internals.h"

#include "MK26F18.h"
#include "system.h"
#include "common.h"
#include <stdlib.h>
#include "Mockstate_machine.h"
#include "Mockpsu.h"
#include "Mockinit.h"
#include "Mockwand.h"
#include "Mocknotifications.h"
#include "Mockswitches.h"
#include "Mockfootpedal.h"
#include "Mockaudio.h"
#include "Mockled.h"
#include "Mockgui.h"
#include "Mocksensors.h"
#include "Mockadc.h"
#include "Mockcan.h"
#include "Mockpump.h"
#include "Mockpost.h"
#include "Mocklpt.h"
#include "Mockdma.h"
#include "Mocksystick.h"
#include "Mockbuck.h"
#include "Mockrf.h"
#include "Mockwdog.h"
#include "Mockftfe.h"
#include "cpu.h"
#include "Mockevent_buffer.h"
#include "Mocklegacy_wand.h"
#include "Mockprofiles.h"
#include "MockZigbee.h"
#include "Mockesdhc.h"
#include "Mocksw_manager.h"
#include "Mockbdm_ww_main.h"


#ifdef DEBUG_CAN
 #include "can_debug.h"
 SMART_WAND_DATABASE db;
#endif


extern state current_state;

void setUp(void)
{
}


void tearDown(void)
{
}


// void test_startup_exit (void)
// {
//    init_ExpectAndReturn(1);
//    uint8_t return_value =  startup_exit();
//    TEST_ASSERT_EQUAL(1,return_value);
// }

// void test_idle_state_null_event (void)
// {
//    event e = NULL_EVENT;
//    // expect do_nothing to be called once
//    do_nothing_ExpectAndReturn(0);
//    uint8_t return_value =  idle_state_null_event(e);
//    TEST_ASSERT_EQUAL(0,return_value);
// }

// void test_app_main_startup_state_poll_pwr_switch_event(void)
// {
//    state s = current_state;
//    // Poll power switch button function must be called once
//    poll_pwr_button_ExpectAndReturn(0);
//    // notification manager is called once
//    notification_manager_Expect();
//    put_event(&evt_tbl,POLL_PWR_SWITCH);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_startup_state_power_up_event(void)
// {
//    state s = current_state;
//    // power_board_on transition function is executed
//    power_board_on_ExpectAndReturn(1);

//    // do_nothing is expected to be called once for idle_state entry
//    do_nothing_ExpectAndReturn(0);

//    // statup_exit calls init, so init is expected to be called once
//    sys_init_ExpectAndReturn(1);
   

//    // notification manager is called once after event processing
//    // to deal with notifications
//    notification_manager_Expect();

//    // Poll power switch button function must be called once
//    put_event (&evt_tbl, POWER_UP);
//    app_main();

//    // Must have transitioned to IDLE state
//    TEST_ASSERT_EQUAL(IDLE_STATE,current_state);
// }

// void test_app_main_startup_state_startup_leds_inputs(void)
// {
//    current_state = STARTUP_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // control leds tran function is called if this event occurs
//    startup_leds_ExpectAndReturn(0);

//    put_event(&evt_tbl,CTRL_LED_INPUTS);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_startup_state_poll_pump_sensors(void)
// {
//    current_state = STARTUP_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // 
//    poll_pump_sensors_ExpectAndReturn(0);

//    put_event(&evt_tbl,POLL_PUMP_SENSORS);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_startup_state_load_screen01(void)
// {
//    current_state = STARTUP_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // 
//    load_screen01_ExpectAndReturn(0);

//    put_event(&evt_tbl,UPDATE_SCREEN);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_startup_state_irms_overcurrent(void)
// {
//    current_state = STARTUP_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // 
//    irms_overcurrent_ExpectAndReturn(0);

//    put_event(&evt_tbl,RUN_IEC_OVERCURRENT);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_idle_state_WAND_PARAM_0_RX(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);


//    // 
//    wand_download_parm_pg_1_ExpectAndReturn(1);

//    put_event(&evt_tbl,WAND_PARAM_0_RX);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_idle_state_WAND_PARAM_1_RX(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);


//    // 
//    wand_download_parm_pg_2_ExpectAndReturn(1);

//    put_event(&evt_tbl,WAND_PARAM_1_RX);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_idle_state_WAND_PARAM_2_RX(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);


//    // 
//    wand_download_parm_pg_3_ExpectAndReturn(1);

//    put_event(&evt_tbl,WAND_PARAM_2_RX);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_idle_state_WAND_PARAM_3_RX(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);


//    // 
//    wand_download_parm_pg_4_ExpectAndReturn(1);

//    put_event(&evt_tbl,WAND_PARAM_3_RX);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }


// void test_app_main_idle_state_do_nothing(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // since this is a NULL event no state transition function is called
//    put_event(&evt_tbl,NULL_EVENT);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }


// void test_app_main_idle_state_WAND_PARAM_4_RX(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);

//    // This is called during exit()
//    switch_ablate_parameters_ExpectAndReturn(0);

//    // 
//    wand_validate_ExpectAndReturn(1);

//    put_event(&evt_tbl,WAND_PARAM_4_RX);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(USER_INPUT_STATE,current_state);
// }

// void test_app_main_idle_state_WAND_MFG_RX(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);
//    do_nothing_ExpectAndReturn(0);

//    wand_unlock_ExpectAndReturn(1);
//    put_event(&evt_tbl,WAND_MFG_RX);
//    app_main();
// // No state change is expected
//    TEST_ASSERT_EQUAL(IDLE_STATE,current_state);
// }

// void test_app_main_idle_state_WAND_PASSCODE_RX(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);
//    do_nothing_ExpectAndReturn(0);

//    wand_download_parm_pg_0_ExpectAndReturn(1);
//    put_event(&evt_tbl,WAND_PASSCODE_RX);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(IDLE_STATE,current_state);
// }

// void test_app_main_idle_state_RESET_ALARM_AUDIO(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    reset_alarm_audio_ExpectAndReturn(1);

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);
//    do_nothing_ExpectAndReturn(0);

//    put_event(&evt_tbl,RESET_ALARM_AUDIO);
//    app_main();

//    // No state change is expected
//    TEST_ASSERT_EQUAL(IDLE_STATE,current_state);
// }

// void test_app_main_idle_state_poll_pwr_switch_event(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // Poll power switch button function must be called once
//    poll_pwr_button_ExpectAndReturn(0);
//    // notification manager is called once
//    notification_manager_Expect();
//    put_event(&evt_tbl,POLL_PWR_SWITCH);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(IDLE_STATE,current_state);
// }

// void test_app_main_idle_state_update_leds_inputs(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;

//    // notification manager is called once
//    notification_manager_Expect();

//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);
//    do_nothing_ExpectAndReturn(0);


//    // control leds tran function is called if this event occurs
//    update_leds_ExpectAndReturn(1);

//    put_event(&evt_tbl,CTRL_LED_INPUTS);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_idle_state_wand_lock(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;
//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);
//    do_nothing_ExpectAndReturn(0);
//       // notification manager is called once
//    notification_manager_Expect();

//    wand_lock_ExpectAndReturn(1);
//    put_event(&evt_tbl,WAND_UNPLUGGED);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }


// void test_app_main_idle_state_wand_tx(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;
//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);
//    do_nothing_ExpectAndReturn(0);
//    // notification manager is called once
//    notification_manager_Expect();

//    wand_tx_ExpectAndReturn(1);
//    put_event(&evt_tbl,WAND_TX);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }


// void test_app_main_idle_state_wand_timeout_return0(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;
//    // do_nothing is expected to be called twice for idle_state entry
//    //do_nothing_ExpectAndReturn(0);
//    //do_nothing_ExpectAndReturn(0);
//    // notification manager is called once
//    notification_manager_Expect();

//    smartwand_timeout_ExpectAndReturn(0);
//    put_event(&evt_tbl,WAND_TIMEOUT);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

// void test_app_main_idle_state_wand_timeout_return1(void)
// {
//    current_state = IDLE_STATE;
//    state s = current_state;
//    // do_nothing is expected to be called twice for idle_state entry
//    do_nothing_ExpectAndReturn(0);
//    do_nothing_ExpectAndReturn(0);
//    // notification manager is called once
//    notification_manager_Expect();

//    smartwand_timeout_ExpectAndReturn(1);
//    put_event(&evt_tbl,WAND_TIMEOUT);
//    app_main();
//    // No state change is expected
//    TEST_ASSERT_EQUAL(s,current_state);
// }

void test_app_main (void)
{
   get_event_ExpectAndReturn(&evt_tbl,NULL_EVENT);
   notification_manager_Expect();
   profile_manager_Expect();
   app_main();
}

void test_app_main_1 (void)
{
   get_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER);
   init_manager_ExpectAndReturn(0);
   notification_manager_Expect();
   profile_manager_Expect();
   app_main();
}

void test_app_main_2 (void)
{
   get_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER);
   init_manager_ExpectAndReturn(1);
   //do_nothing_ExpectAndReturn(1); // exit for the powerup state
   post_init_ExpectAndReturn(1); // entry function for the STARTUP state
   notification_manager_Expect();
   profile_manager_Expect();
   app_main();
}

void test_app_main_3 (void)
{
   get_event_ExpectAndReturn(&evt_tbl,SAVE_PARAMS_TO_FLASH);
   //do_nothing_ExpectAndReturn(1);
   //do_nothing_ExpectAndReturn(1);
   notification_manager_Expect();
   profile_manager_Expect();
   app_main();
}

/* 	uint8_t exit_value (void)
	{
		return 1;
	}
	 */

void test_app_main_4 (void)
{
	uint8_t transition_flag = 0;
	//state_object state_exec;

	//tran_function exit_value_func = &exit_value;
	/* Case 1 */
	transition_flag = 1;	
	get_event_ExpectAndReturn(&evt_tbl,INIT_AUDIO_AMP);
	put_event_IgnoreAndReturn(1);
	audio_amp_init_IgnoreAndReturn(1);
	init_manager_IgnoreAndReturn(1);
	notification_manager_Expect();
	profile_manager_Expect();
	app_main();
	/* Case 2 */
	get_event_ExpectAndReturn(&evt_tbl,SAVE_PARAMS_TO_FLASH);
	notification_manager_Expect();
	profile_manager_Expect();
    app_main();
	/* Case 3 */	
	//current_state = POWERUP_STATE;
	get_event_ExpectAndReturn(&evt_tbl,INIT_AUDIO_AMP);
	put_event_IgnoreAndReturn(1);
	audio_amp_init_IgnoreAndReturn(1);
	init_manager_IgnoreAndReturn(1);
	notification_manager_Expect();
	profile_manager_Expect();
	app_main();
	/* Case 4 */	
	//current_state = STARTUP_STATE;
	//current_state = IDLE_STATE;
	//current_state = POWERUP_STATE;
	get_event_ExpectAndReturn(&evt_tbl,WAND_PARAM_0_RX);
	put_event_IgnoreAndReturn(1);
	audio_amp_init_IgnoreAndReturn(1);
	init_manager_IgnoreAndReturn(1);
	notification_manager_Expect();
	profile_manager_Expect();
	app_main();
	
	//current_state = USER_INPUT_STATE;
	get_event_ExpectAndReturn(&evt_tbl,SAVE_PARAMS_TO_FLASH);
	put_event_IgnoreAndReturn(1);
	audio_amp_init_IgnoreAndReturn(1);
	init_manager_IgnoreAndReturn(1);
	load_screen01_IgnoreAndReturn(1);
	notification_manager_Expect();
	profile_manager_Expect();
	app_main();
	
	/* Case 5 */
	get_event_ExpectAndReturn(&evt_tbl,NULL_EVENT);
	notification_manager_Expect();
	profile_manager_Expect();
	//state_exec.exit = exit_value_func;
	app_main();
	
	
}

void test_IDLE_state_entry (void)
{
   uint8_t return_value = IDLE_state_entry();
   TEST_ASSERT_EQUAL(1,return_value);
}





void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
