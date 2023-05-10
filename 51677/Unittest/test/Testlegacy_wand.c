#include "unity.h"
#include "MK26F18.h"
#include "system.h"
#include "dma.h"
#include "legacy_wand.h"

#include "Mockad7265.h"
#include "Mockfuse_blow.h"
#include "Mockgui.h"
#include "Mockwand.h"
#include "Mockrf.h"
#include "Mocknotifications.h"
#include "Mockevent_buffer.h"
#include "Mockpump.h"


extern legacy_wand_params legacy_wand_params_obj;
extern uint8_t wand_type_value;
extern uint16_t buffer_r2[];
extern uint16_t buffer_r1[];
extern uint8_t wait_counter;
extern uint32_t LW_OVERALL_TIME_LIMIT;
extern previous_wands previous_wands_database[LW_DB_SZ];
extern uint8_t previous_wands_database_index;
extern uint16_t AD7265_read_value (CHAN_NUM channel_num);
//extern uint32_t LW_ACTIVATION_TIME_LIMIT;
extern prime_mode_t PrimeState;

uint32_t LW_40_MIN_ACTIVE_TIME_LIMIT  = 50*60*40;
uint32_t LW_7_MIN_ACTIVE_TIME_LIMIT = 50*60*7;

void setUp(void)
{
   // setting up the time
   (*(uint32_t*)0x4003D000) = 0x1234;
}

void tearDown(void)
{
}

void fill_buff_r2 (uint16_t r2_value)
{
   for (uint8_t i = 0; i < 64; i++)
   {
      buffer_r2[i] = r2_value;
   }
}

void fill_buff_r1 (uint16_t r1_value)
{
   for (uint8_t i = 0; i < 64; i++)
   {
      buffer_r1[i] = r1_value;
   }
}

void setup_legacy_wand (uint32_t r1_value,uint32_t r2_value)
{
   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);

   // this function is used to call the
   // legacy_wand_read_adc_values many time to
   // setup the resistor values
   // First read to make sure ADC is within Range
    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);
   AD7265_read_value_ExpectAndReturn(B1,1000);
   AD7265_read_value_ExpectAndReturn(B2,1000);
   AD7265_read_value_ExpectAndReturn(B3,1000);
   AD7265_read_value_ExpectAndReturn(A3,100);
}

void test_switch_stuck_mode(void)
{
   AD7265_read_value_ExpectAndReturn(B1,1000);
   AD7265_read_value_ExpectAndReturn(B2,100);
   AD7265_read_value_ExpectAndReturn(B3,1000);

   put_event_ExpectAndReturn(&evt_tbl,LW_SWITCH_STUCK,1);
   uint8_t return_value = check_18p_fs_stuck();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_switch_stuck_coag(void)
{
   AD7265_read_value_ExpectAndReturn(B1,1000);
   AD7265_read_value_ExpectAndReturn(B2,1000);
   AD7265_read_value_ExpectAndReturn(B3,100);

   put_event_ExpectAndReturn(&evt_tbl,LW_SWITCH_STUCK,1);
   uint8_t return_value = check_18p_fs_stuck();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_switch_stuck_ablate(void)
{
   AD7265_read_value_ExpectAndReturn(B1,100);
   AD7265_read_value_ExpectAndReturn(B2,1000);
   AD7265_read_value_ExpectAndReturn(B3,1000);

   put_event_ExpectAndReturn(&evt_tbl,LW_SWITCH_STUCK,1);
   uint8_t return_value = check_18p_fs_stuck();
   TEST_ASSERT_EQUAL(1,return_value);
}





void test_ent_timer_true (void)   //line 595 v4.11
{
	setup_legacy_wand(909,909);
	//current_therapy_settings.pump.sensor_status = 0xFFFF;
	//current_therapy_settings.legacy_wand_levels.topaz_timer = 1;
	put_event_IgnoreAndReturn(1);
	current_therapy_settings.controller = ENT;
	legacy_wand_params_obj.R2 = 2400;
	get_priming_status_IgnoreAndReturn(0);
    legacy_wand_read_adc_values();
}


void test_filter_unsettle_legacy_wand (void)
{
   uint32_t r1_value = 121;
   uint32_t r2_value = 909;
   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);
   // throwing a random value in the filter to make it unsettle
   // since the new filter ignores the lowest two values, we need
   // a lot of spurious values to unsettle the filter
   buffer_r1[10] = 0;
   buffer_r1[11] = 0;
   buffer_r1[12] = 0;
   buffer_r1[13] = 0;
   buffer_r1[14] = 0;
   buffer_r1[15] = 0;
   buffer_r1[16] = 0;
   buffer_r1[17] = 0;
   buffer_r1[18] = 0;
   buffer_r1[19] = 0;
   buffer_r1[20] = 0;
   buffer_r1[21] = 0;
   buffer_r1[22] = 0;
   buffer_r1[23] = 0;
   buffer_r1[24] = 0;
   buffer_r1[25] = 0;
   buffer_r1[26] = 0;
   buffer_r1[27] = 0;
   buffer_r1[28] = 0;
   buffer_r1[29] = 0;

    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_adc_values_1 (void)
{
   setup_legacy_wand(1650,909);
   put_event_IgnoreAndReturn(1);
   //put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   
   setup_legacy_wand(1650,10500);
   put_event_IgnoreAndReturn(1);
   legacy_wand_params_obj.wand_connected = 0;
   //pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values(); 
   
}

void test_adc_values_2 (void)
{
   setup_legacy_wand(121,909);
   put_event_IgnoreAndReturn(1);
   //put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_adc_values_3 (void)
{
   setup_legacy_wand(100000,909);
//   clear_fuse_blow_Expect();
   put_event_IgnoreAndReturn(1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_adc_values_4 (void)
{
   setup_legacy_wand(25000,9090);
   clear_fuse_blow_Expect();
   put_event_IgnoreAndReturn(1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_adc_values_5 (void)
{
   setup_legacy_wand(121,499);
   put_event_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,LW_EXPIRED,1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_adc_values_6 (void)
{
   setup_legacy_wand(147,909); // test for incompatible wands
   put_event_ExpectAndReturn(&evt_tbl,LW_INCOMPATIBLE,1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_adc_values_7 (void)
{
   // R1 = 66 ohms, it is less than 5600 ohms but at the same time
   // is not one of the defined R1 values.
   setup_legacy_wand(66,909); //test_wand_id_error
   put_event_ExpectAndReturn(&evt_tbl,LW_ID_ERROR,1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_adc_values_8 (void)
{
   setup_legacy_wand(1650,909);
   put_event_IgnoreAndReturn(1);
   //put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   pump_set_speed_Ignore();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_adc_values_9 (void)
{
   setup_legacy_wand(1650,10500);
   put_event_IgnoreAndReturn(1);
   legacy_wand_params_obj.wand_connected = 1;
   pump_set_speed_Ignore();
   clear_fuse_blow_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}



void test_used_new_fuse_wand1 (void)
{

   setup_legacy_wand(121 ,3);
   blow_fuse_Ignore();
   clear_fuse_blow_Ignore();
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   
   for (uint8_t i =0; i <9; i++)
   {
      setup_legacy_wand(121,3);
	  //put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
	  put_event_IgnoreAndReturn(1);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   
   for (uint8_t i =0; i <11; i++)
   {
      setup_legacy_wand(121,750);
	  put_event_IgnoreAndReturn(1);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }

}

// Simulate disconnecting the wands
void test_wand_disconnect_3 (void)
{
   setup_legacy_wand(100000,9090);
   clear_fuse_blow_Expect();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}


void test_used_new_fuse_wand2 (void)
{
   setup_legacy_wand(576,3);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   setup_legacy_wand(576 ,3);
   blow_fuse_Expect();
   clear_fuse_blow_Expect();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   for (uint8_t i =0; i <9; i++)
   {
      setup_legacy_wand(576,3);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   for (uint8_t i =0; i <11; i++)
   {
      setup_legacy_wand(576,750);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }

}

// Simulate disconnecting the wands
void test_wand_disconnect_4 (void)
{
   setup_legacy_wand(100000,9090);
   clear_fuse_blow_Ignore();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_used_new_fuse_wand3 (void)
{
   setup_legacy_wand(200,3);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   setup_legacy_wand(200 ,3);
   blow_fuse_Ignore();
   clear_fuse_blow_Ignore();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   for (uint8_t i =0; i <9; i++)
   {
      setup_legacy_wand(200,3);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   for (uint8_t i =0; i <11; i++)
   {
      setup_legacy_wand(200,750);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }

}

// Simulate disconnecting the wands
void test_wand_disconnect_5 (void)
{
   setup_legacy_wand(100000,9090);
   clear_fuse_blow_Ignore();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_used_new_fuse_wand4 (void)
{
   setup_legacy_wand(412,3);
   put_event_IgnoreAndReturn(1);
   clear_fuse_blow_Ignore();
   blow_fuse_Ignore();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   setup_legacy_wand(412 ,3);
   
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   for (uint8_t i =0; i <9; i++)
   {
      setup_legacy_wand(412,3);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   for (uint8_t i =0; i <11; i++)
   {
      setup_legacy_wand(412,750);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }

}
// Simulate disconnecting the wands
void test_wand_disconnect_6 (void)
{
   setup_legacy_wand(100000,9090);
   clear_fuse_blow_Ignore();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

// Now put in a new wand, but the same R1 and R2 values as W2
void test_used_new_fuse_wand2_b (void)
{
   setup_legacy_wand(576,3);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   setup_legacy_wand(576 ,3);
   blow_fuse_Ignore();
   clear_fuse_blow_Ignore();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   for (uint8_t i =0; i <9; i++)
   {
      setup_legacy_wand(576,3);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   //put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   for (uint8_t i =0; i <11; i++)
   {
      setup_legacy_wand(576,750);
	  AD7265_read_value_IgnoreAndReturn(1);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
}

// Simulate disconnecting the wands
void test_wand_disconnect_7 (void)
{
   setup_legacy_wand(100000,9090);
   clear_fuse_blow_Ignore();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

// Now put in a new wand, but the same R1 but different R2
void test_used_new_fuse_wand2_c (void)
{
   setup_legacy_wand(576,3);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   setup_legacy_wand(576 ,3);
   blow_fuse_Ignore();
   clear_fuse_blow_Ignore();
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   for (uint8_t i =0; i <9; i++)
   {
      setup_legacy_wand(576,3);
	  put_event_IgnoreAndReturn(1);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   //put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   for (uint8_t i =0; i <11; i++)
   {
      setup_legacy_wand(576,450);
	  put_event_IgnoreAndReturn(1);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
}

// Simulate disconnecting the wands
void test_wand_disconnect_8 (void)
{
   setup_legacy_wand(100000,9090);
   clear_fuse_blow_Ignore();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

// Simulate disconnecting the wands
void test_wand_disconnect_9 (void)
{
   setup_legacy_wand(100000,9090);
   clear_fuse_blow_Ignore();
   //put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   AD7265_read_value_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}



// Simulate disconnecting the wands
void test_wand_disconnect_10 (void)
{
   uint32_t r1_value = 100000;
   uint32_t r2_value = 9090;

   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);

   // this function is used to call the
   // legacy_wand_read_adc_values many time to
   // setup the resistor values
   // First read to make sure ADC is within Range
    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);

   //clear_fuse_blow_Expect();
   //put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

// Plug a wand with same R1, but a R2 off
void test_previously_pluggedin_diffR2_wand (void)
{
   setup_legacy_wand(576,320);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();

}

// Simulate disconnecting the wands
void test_wand_disconnect_11 (void)
{
   uint32_t r1_value = 100000;
   uint32_t r2_value = 9090;

   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);

   // this function is used to call the
   // legacy_wand_read_adc_values many time to
   // setup the resistor values
   // First read to make sure ADC is within Range
    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);

   clear_fuse_blow_Expect();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

// Plug a wand with diff R1, same R2
void test_previously_pluggedin_diffR1_wand (void)
{
   setup_legacy_wand(576,320);
   put_event_ExpectAndReturn(&evt_tbl,LW_EXPIRED,1);
   pump_set_speed_Ignore();
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();

}

// Simulate disconnecting the wands
void test_wand_disconnect_12 (void)
{
   uint32_t r1_value = 100000;
   uint32_t r2_value = 9090;

   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);

   // this function is used to call the
   // legacy_wand_read_adc_values many time to
   // setup the resistor values
   // First read to make sure ADC is within Range
    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);

   clear_fuse_blow_Expect();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

// This section deals with used wands.
// USE once of the wands used above, so that we can check the
// stop time logic


void test_used_wand (void)
{
   setup_legacy_wand(576,450);
   // Setup stop time
   // This code should clear up all the wands in the previous wands
   // database because more than a day has passed.
   (*(uint32_t*)0x4003D000)= 0x163b5;
   // since all the wands are past one day, this will throw out
   // all the previous values and will reject the current wand
   // because it is not a new wand
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();

}

// Now simulate a new wand, blow the fuse, disconnect the wand and
// then plug it back again

// Now put in a new wand, but the same R1 but different R2
void test_used_new_fuse_wand2_d (void)
{
   setup_legacy_wand(576,3);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   setup_legacy_wand(576 ,3);
   blow_fuse_Ignore();
   clear_fuse_blow_Ignore();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   for (uint8_t i =0; i <9; i++)
   {
      setup_legacy_wand(576,3);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   for (uint8_t i =0; i <11; i++)
   {
      setup_legacy_wand(576,450);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   //put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
}



// Plug the wand wand in
void test_previously_pluggedin_wand (void)
{
   //setup_legacy_wand(576,450);
   uint32_t r1_value = 576;
   uint32_t r2_value = 450;
   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);
   AD7265_read_value_ExpectAndReturn(B1,1000);
   AD7265_read_value_ExpectAndReturn(B2,1000);
   AD7265_read_value_ExpectAndReturn(B3,1000);
   AD7265_read_value_ExpectAndReturn(A3,100);
   //put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   // to get the new_wand_flag == 0 true case, call the
   // function one more time
   //AD7265_read_value_IgnoreAndReturn(1);
   //legacy_wand_read_adc_values();

}

// This function tests bad fuse blow,i.e even after the fuse is blown
// the value is less than 5 ohms
void test_bad_fuse_blow (void)
{
   setup_legacy_wand(412,3);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   setup_legacy_wand(412,3);
   //blow_fuse_Expect();
   //clear_fuse_blow_Expect();
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();

   for (uint8_t i =0; i <9; i++)
   {
      setup_legacy_wand(412,3);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }
   
   //put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);

   for (uint8_t i =0; i <11; i++)
   {
      setup_legacy_wand(412,3);
	  get_priming_status_IgnoreAndReturn(0);
      legacy_wand_read_adc_values();
   }

}

/* // Simulate disconnecting the wands
void test_wand_disconnect_14 (void)
{
   uint32_t r1_value = 100000;
   uint32_t r2_value = 9090;

   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);

   // this function is used to call the
   // legacy_wand_read_adc_values many time to
   // setup the resistor values
   // First read to make sure ADC is within Range
    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);
   pump_set_speed_Ignore();
   legacy_wand_read_adc_values();
} */

// Simulate disconnecting the wands
void test_wand_disconnect_15 (void)
{
   uint32_t r1_value = 100000;
   uint32_t r2_value = 9090;
   legacy_wand_params_obj.wand_connected = 1;
   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);

   // this function is used to call the
   // legacy_wand_read_adc_values many time to
   // setup the resistor values
   // First read to make sure ADC is within Range
    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);
   AD7265_read_value_ExpectAndReturn(B1,1000);
   AD7265_read_value_ExpectAndReturn(B2,1000);
   AD7265_read_value_ExpectAndReturn(B3,1000);
   AD7265_read_value_ExpectAndReturn(A3,100);
   
   clear_fuse_blow_Expect();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}



// Simulate disconnecting the wands
void test_wand_disconnect_13 (void)
{
   legacy_wand_params_obj.wand_connected = 1;

   uint32_t r1_value = 100000;
   uint32_t r2_value = 9090;

   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);

   // this function is used to call the
   // legacy_wand_read_adc_values many time to
   // setup the resistor values
   // First read to make sure ADC is within Range
    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);
   clear_fuse_blow_Expect();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

// void test_wand_r1_is_zero (void)
// {
//    // Put R1 = 5Ohms, it passes through the resistor table check but fails the
//    // next test
//    //setup_legacy_wand(3,909);
//    //put_event_ExpectAndReturn(&evt_tbl,LW_ID_ERROR,1);
//    //legacy_wand_read_adc_values();
//    TEST_IGNORE_MESSAGE("No need to test for wand_type_value >0");
// }

// void test_wand_r1_is_24 (void)
// {
//    // Put R1 = 9090 Ohms, it passes through the resistor table check but fails the
//    // next test
//    //setup_legacy_wand(11500,909);
//    //put_event_ExpectAndReturn(&evt_tbl,LW_ID_ERROR,1);
//    //legacy_wand_read_adc_values();
//    TEST_IGNORE_MESSAGE("No need to test for wand_type_value == 25 ");
// }

// NO need for fuse blow wait state code anymore becuase the legacy wand
// code was redesigned to not even get into resistor evaluation without the
// filter settling

// void test_used_wand_fuse_blow_wait (void)
// {
//    setup_legacy_wand(121,909);
//    // Just throwing off the filter settling of R2
//    buffer_r2[10]= 4000;
//    buffer_r2[11]= 4000;
//    buffer_r2[12]= 4000;
//    buffer_r2[13]= 4000;
//    buffer_r2[14]= 4000;
//    buffer_r2[15]= 4000;

//    legacy_wand_read_adc_values();
// }



void test_legacy_wand_init (void)
{
        AD7265_Init_Expect();
        LegacyWand_PWMInit_Expect();
        clear_fuse_blow_Expect();
        uint8_t return_value = legacy_wand_init ();
        TEST_ASSERT_EQUAL(1,return_value);
}

void test_reset_therapy_settings ()
{
        uint8_t return_value = reset_therapy_settings ();
        TEST_ASSERT_EQUAL(0,return_value);

}

void test_legacy_wand_adc_fault (void)
{
        uint8_t return_value = legacy_wand_adc_fault ();
        TEST_ASSERT_EQUAL(1,return_value);
}

void test_equal_adc_value (void)
{
        uint16_t val1, val2, range;
        val1 = 10; val2 = 20; range = 2;
        uint8_t return_value;

        return_value = equal_adc_value(val1, val2, range);
        TEST_ASSERT_EQUAL(0,return_value);
        range = 10;
        return_value = equal_adc_value(val1, val2, range);
        TEST_ASSERT_EQUAL(1,return_value);
        val1 = 20; val2 = 10; range = 2;
        return_value = equal_adc_value(val1, val2, range);
        TEST_ASSERT_EQUAL(0,return_value);
        range = 10;
        return_value = equal_adc_value(val1, val2, range);
        TEST_ASSERT_EQUAL(1,return_value);
}

void test_legacy_coag_increment (void)
{
   legacy_wand_params_obj.coag_level = 0;
   legacy_wand_params_obj.max_coag = 2;
   uint8_t return_value =  legacy_coag_increment();
   TEST_ASSERT_EQUAL(1,legacy_wand_params_obj.coag_level);
}


void test_legacy_coag_increment_2 (void)
{
   legacy_wand_params_obj.coag_level = 4;
   legacy_wand_params_obj.max_coag = 2;
   uint8_t return_value =  legacy_coag_increment();
   TEST_ASSERT_EQUAL(legacy_wand_params_obj.max_coag,legacy_wand_params_obj.coag_level);
}


void test_legacy_coag_decrement (void)
{
   legacy_wand_params_obj.coag_level = 1;
   legacy_wand_params_obj.min_coag = 2;
   uint8_t return_value =  legacy_coag_decrement();
   TEST_ASSERT_EQUAL(legacy_wand_params_obj.min_coag,legacy_wand_params_obj.coag_level);
}


void test_legacy_coag_decrement_2 (void)
{
   legacy_wand_params_obj.coag_level = 5;
   legacy_wand_params_obj.min_coag = 2;
   uint8_t return_value =  legacy_coag_decrement();
   TEST_ASSERT_EQUAL(4,legacy_wand_params_obj.coag_level);
}

void test_legacy_ablate_increment (void)
{
   legacy_wand_params_obj.ablate_level = 0;
   legacy_wand_params_obj.max_ablate = 2;
   uint8_t return_value =  legacy_ablate_increment();
   TEST_ASSERT_EQUAL(1,legacy_wand_params_obj.ablate_level);
}


void test_legacy_ablate_increment_2 (void)
{
   legacy_wand_params_obj.ablate_level = 4;
   legacy_wand_params_obj.max_ablate = 2;
   uint8_t return_value =  legacy_ablate_increment();
   TEST_ASSERT_EQUAL(legacy_wand_params_obj.max_ablate,legacy_wand_params_obj.ablate_level);
}


void test_legacy_ablate_decrement (void)
{
   legacy_wand_params_obj.ablate_level = 1;
   legacy_wand_params_obj.min_ablate = 2;
   uint8_t return_value =  legacy_ablate_decrement();
   TEST_ASSERT_EQUAL(legacy_wand_params_obj.min_ablate,legacy_wand_params_obj.ablate_level);
}


void test_legacy_ablate_decrement_2 (void)
{
   legacy_wand_params_obj.ablate_level = 5;
   legacy_wand_params_obj.min_ablate = 2;
   uint8_t return_value =  legacy_ablate_decrement();
   TEST_ASSERT_EQUAL(4,legacy_wand_params_obj.ablate_level);
}

void test_legacy_wand_unpolugged (void)
{
   switch_to_ww_wand_ExpectAndReturn(1);
   uint8_t return_value = legacy_wand_unplugged ();
}

void test_legacy_wand_UI_state_entry (void)
{
   current_therapy_settings.active_wand.lw_active = 0;
   thermometer_color_ExpectAndReturn(0,0);
   current_therapy_settings.sw.update_progress = 0;
   put_event_ExpectAndReturn(&evt_tbl,CANCEL_SW_UPDATE,1);
   uint8_t return_value = legacy_wand_UI_state_entry ();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.active_wand.lw_active);
}

void test_lw_wand_switch_stuck_fault(void)
{
   current_therapy_settings.notify.legacy_wand_stuck_switch = 0;
   uint8_t return_value = lw_wand_switch_stuck_fault();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.notify.legacy_wand_stuck_switch);
}

void test_lw_wand_expired_error (void)
{
   current_therapy_settings.notify.legacy_wand_expired = 0;
   uint8_t return_value = lw_wand_expired_error();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.notify.legacy_wand_expired);
}

void test_lw_wand_use_limit_error (void)
{
   current_therapy_settings.notify.legacy_wand_use_limit = 0;
   uint8_t return_value = lw_wand_use_limit_error();
   TEST_ASSERT_EQUAL(1, current_therapy_settings.notify.legacy_wand_use_limit);
}

void test_lw_ambient_sensor (void)
{
   current_therapy_settings.active_wand.lw_active = 0;
   current_therapy_settings.notify.legacy_wand_no_ambient = 0;
   uint8_t return_value = lw_ambient_sensor ();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.notify.legacy_wand_no_ambient);
}

void test_lw_ambient_sensor_2 (void)
{
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.notify.legacy_wand_no_ambient = 0;
   uint8_t return_value = lw_ambient_sensor ();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.notify.legacy_wand_no_ambient);
}


void test_switch_to_legacy_wand(void)
{
   legacy_wand_common_params.ambient_sensor_flag = 0;
   thermometer_color_ExpectAndReturn(0,0);
   uint8_t return_value = switch_to_legacy_wand();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.notify.legacy_wand_no_ambient);
}

void test_switch_to_legacy_wand_1(void)
{
   legacy_wand_common_params.ambient_sensor_flag = 1;
   thermometer_color_ExpectAndReturn(0,0);
   uint8_t return_value = switch_to_legacy_wand();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.notify.legacy_wand_no_ambient);
}


void test_legacy_wand_button_read (void)
{
   legacy_wand_params_obj.wand_connected = 0;
   uint8_t return_value = legacy_wand_button_read(0,0,0,0);
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_legacy_wand_button_read_expired_time (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   (*(uint32_t*)0x4003D000)= 0x163b5;
   put_event_ExpectAndReturn(&evt_tbl,LW_USE_LIMIT_ERROR,1);
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_wand_button_read(0,0,0,0);
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_legacy_wand_button_read_increment_during_RF (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   legacy_wand_params_obj.therapy_on = 1;
   previous_wands_database_index = 0;

   previous_wands_database[previous_wands_database_index].activation_time_sum = 0;
   
   put_event_IgnoreAndReturn(1);

   uint8_t return_value = legacy_wand_button_read(0,0,0,0);

   TEST_ASSERT_EQUAL(1,previous_wands_database[previous_wands_database_index].activation_time_sum);
}

void test_legacy_wand_button_read_no_RF (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   legacy_wand_params_obj.therapy_on = 0;
   previous_wands_database_index = 0;

   previous_wands_database[previous_wands_database_index].activation_time_sum = 0;
   
   put_event_IgnoreAndReturn(1);

   uint8_t return_value = legacy_wand_button_read(0,0,0,0);

   TEST_ASSERT_EQUAL(0,previous_wands_database[previous_wands_database_index].activation_time_sum);
}

void test_legacy_wand_button_read_expired_activation_time (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   legacy_wand_params_obj.therapy_on = 0;
   previous_wands_database_index = 0;

   previous_wands_database[previous_wands_database_index].activation_time_sum = LW_40_MIN_ACTIVE_TIME_LIMIT+1;
   //put_event_ExpectAndReturn(&evt_tbl,LW_USE_LIMIT_ERROR,1);
   
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   
   uint8_t return_value = legacy_wand_button_read(0,0,0,0);

   TEST_ASSERT_EQUAL(0,previous_wands_database[previous_wands_database_index].activation_time_sum);
}

void test_legacy_wand_button_read_ablate_button_press (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.ablate_flag = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   current_therapy_settings.legacy_wand_levels.keys_active = 1;
   put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_ON_EVENT,1);
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_wand_button_read(200,1000,1000,0);
}

void test_legacy_wand_button_read_ablate_button_press_no (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.ablate_flag = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   //put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_ON_EVENT,0);
   // NO put event function should be called, that is the test
   // for this case
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_wand_button_read(1000,1000,1000,0);
}

void test_legacy_wand_button_read_ablate_button_press_release (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.ablate_flag = 1;
   current_therapy_settings.active_wand.lw_active = 0;
   put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,0);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_wand_button_read(1000,1000,1000,0);
}

void test_legacy_wand_button_read_coag_button_press (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.coag_flag = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   put_event_ExpectAndReturn(&evt_tbl,LW_COAG_ON_EVENT,0);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_wand_button_read(1000,200,1000,0);
}

void test_legacy_wand_button_read_coag_button_press2 (void)
{
   current_therapy_settings.legacy_wand_levels.keys_active = 0;
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.coag_flag = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   put_event_ExpectAndReturn(&evt_tbl,LW_COAG_ON_EVENT,0);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_wand_button_read(1000,200,1000,0);
   current_therapy_settings.legacy_wand_levels.keys_active = 1;
}

void test_legacy_wand_button_read_coag_button_press_no (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.coag_flag = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   //put_event_ExpectAndReturn(&evt_tbl,LW_COAG_ON_EVENT,0);
   // NO put event function should be called, that is the test
   // for this case
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_wand_button_read(1000,1000,1000,0);
}

void test_legacy_wand_button_read_coag_button_press_release (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.coag_flag = 1;
   current_therapy_settings.active_wand.lw_active = 0;
   put_event_ExpectAndReturn(&evt_tbl,LW_COAG_OFF_EVENT,0);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_wand_button_read(1000,1000,1000,0);
}

void test_legacy_wand_button_read_mode_button_press (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_MODE_ON_EVENT,0);
   uint8_t return_value = legacy_wand_button_read(1000,1000,200,0);
}

void test_legacy_wand_button_read_mode_button_press_no (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   //put_event_ExpectAndReturn(&evt_tbl,LW_MODE_ON_EVENT,0);
   // NO put event function should be called, that is the test
   // for this case
   uint8_t return_value = legacy_wand_button_read(1000,1000,1000,0);
}



void test_legacy_wand_button_read_temp (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   legacy_wand_params_obj.ambient_capable = 1;
   thermometer_color_ExpectAndReturn(0,0);
   uint8_t return_value;
   // for (uint8_t i=0; i < 20; i++)
   // {
      //thermometer_color_ExpectAndReturn(0,0);
      return_value = legacy_wand_button_read(1000,1000,1000,0);
      //}
}

void test_legacy_wand_button_read_temp_one_sec_delay (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.treatment.finger_switch_en = 1;
   legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   legacy_wand_params_obj.ambient_capable = 1;
   uint8_t return_value;
   for (uint8_t i=0; i < 100; i++)
   {
      thermometer_color_ExpectAndReturn(0,0);
      return_value = legacy_wand_button_read(1000,1000,1000,0);
   }

}



void test_legacy_wand_button_read_2 (void)
{
	current_therapy_settings.treatment.finger_switch_en = 1;
	legacy_wand_params_obj.wand_connected = 1;
	legacy_wand_params_obj.ablate_flag = 0;
	legacy_wand_params_obj.coag_flag = 0;
	legacy_wand_params_obj.mode_flag = 1;
	current_therapy_settings.legacy_wand_levels.keys_active = 0;
	put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,1);
	put_event_ExpectAndReturn(&evt_tbl,LW_MODE_OFF_EVENT,1);
	thermometer_color_ExpectAndReturn(0,1);
	legacy_wand_button_read(800,1000,1000,25);
}


void test_incompatible_wands_147 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 147;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(100,return_value);
}


void test_incompatible_wands_169 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 169;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(100,return_value);
}

void test_incompatible_wands_243 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 243;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(100,return_value);
}

void test_incompatible_wands_365 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 365;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(100,return_value);
}

void test_incompatible_wands_1400 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 1400;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(100,return_value);
}

void test_incompatible_wands_1650 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 1650;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(18,return_value);
}

void test_incompatible_wands_1960 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 1960;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(100,return_value);
}

void test_incompatible_wands_2400 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 2400;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(20,return_value);
}

void test_incompatible_wands_3920 (void)
{
   uint32_t pullup_r = 499;
   uint16_t r1_value = 3920;
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint8_t return_value = legacy_wand_type_recognizer (r1_adc_value);
   TEST_ASSERT_EQUAL(100,return_value);
}


void test_lw_defaults_loader (void)
{
   uint8_t entry_number = 0;
   put_event_ExpectAndReturn(&evt_tbl,LW_ID_ERROR,0);
   legacy_wand_defaults_loader (entry_number);
}

void test_lw_defaults_loader_25 (void)
{
   uint8_t entry_number = 25;
   put_event_ExpectAndReturn(&evt_tbl,LW_ID_ERROR,0);
   legacy_wand_defaults_loader (entry_number);
}


void test_lw_defaults_loader_16 (void)
{
   uint8_t entry_number = 16;
   legacy_wand_params_obj.max_coag = 0;
   legacy_wand_defaults_loader (entry_number);
   TEST_ASSERT_EQUAL(2,legacy_wand_params_obj.max_coag);
}


void test_lw_defaults_loader_timer (void)
{
   uint8_t entry_number = 6;
   current_therapy_settings.legacy_wand_levels.topaz_timer = 0;
   legacy_wand_defaults_loader (entry_number);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.legacy_wand_levels.topaz_timer);

}

void test_test_temp_curve_fit (void)
{

   legacy_wand_params_obj.ambient_capable = 0;
   uint8_t lw_temp_value;
   uint8_t return_value;
   return_value = legacy_adc_temp_curve_fit ((float)500, &lw_temp_value);

   for (uint8_t i = 0; i <55; i++)
   {
      return_value = legacy_adc_temp_curve_fit ((float)500, &lw_temp_value);
   }
   //TEST_ASSERT_EQUAL(127,return_value);
   TEST_IGNORE_MESSAGE("Cannot test this using the simlator because floating point is not correctly simulated");
}

void test_increment_mode (void)
{
   current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
   legacy_wand_params_obj.ablate_level = 7;
   legacy_wand_params_obj.max_ablate = 10;
   legacy_wand_params_obj.min_ablate = 0;
   uint8_t return_value = increment_mode ();
   TEST_ASSERT_EQUAL(8,legacy_wand_params_obj.ablate_level);
}

void test_increment_mode_rollover (void)
{
   current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
   legacy_wand_params_obj.ablate_level = 10;
   legacy_wand_params_obj.max_ablate = 10;
   legacy_wand_params_obj.min_ablate = 0;
   uint8_t return_value = increment_mode ();
   TEST_ASSERT_EQUAL(1,legacy_wand_params_obj.ablate_level);
}

void test_increment_mode_rollover_2 (void)
{
   current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
   legacy_wand_params_obj.ablate_level = 10;
   legacy_wand_params_obj.max_ablate = 10;
   legacy_wand_params_obj.min_ablate = 3;
   uint8_t return_value = increment_mode ();
   TEST_ASSERT_EQUAL(3,legacy_wand_params_obj.ablate_level);
}

void test_increment_mode_rollover_3 (void)
{
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   current_therapy_settings.controller = ENT;
   legacy_wand_params_obj.ablate_level = 10;
   legacy_wand_params_obj.max_ablate = 10;
   legacy_wand_params_obj.min_ablate = 3;
   uint8_t return_value = increment_mode ();
   TEST_ASSERT_EQUAL(3,legacy_wand_params_obj.ablate_level);
}

// uint8_t reset_therapy_settings (void);
// uint8_t increment_mode (void);
//
//
//
//
// uint8_t legacy_adc_temp_curve_fit (uint16_t);
// uint8_t legacy_wand_type_recognizer (uint16_t);
// uint16_t filter_r1 (uint16_t);
// uint16_t filter_adc_value_r1 (uint16_t);
// uint16_t filter_adc_value_r2 (uint16_t);
// uint8_t filter_temp (uint8_t);
// uint8_t legacy_wand_button_read (uint16_t, uint16_t,uint16_t,uint16_t);
// R2_TYPES legacy_wand_use_limit_check (uint16_t);
// void legacy_wand_defaults_loader (uint8_t);


void test_lw_ent_get_index (void)
{
	R2_TYPES entry_number = WAND_750;
	uint8_t return_value = lw_ent_get_index(entry_number);
	TEST_ASSERT_EQUAL(0,return_value);
	
	entry_number = WAND_909;
	return_value = lw_ent_get_index(entry_number);
	TEST_ASSERT_EQUAL(1,return_value);
	
	entry_number = WAND_1400;
	return_value = lw_ent_get_index(entry_number);
	TEST_ASSERT_EQUAL(2,return_value);
}

void test_legacy_ent_wand_defaults_loader_1 (void)
{
	R2_TYPES entry_number = WAND_750;
	legacy_ent_wand_defaults_loader(entry_number);
	
}

void test_legacy_ent_wand_defaults_loader_2 (void)
{
	
	R2_TYPES entry_number = WAND_909;
	smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_SPORTS_MED;
	current_therapy_settings.previous_wand_indication = SMART_WAND_MFG_INDICATION_MANTA;
	current_therapy_settings.wand.valid = FALSE;
	//put_event_ExpectAndReturn(&evt_tbl,LW_ID_ERROR,1);
	pump_set_speed_Ignore();
	legacy_ent_wand_defaults_loader(entry_number);
	
	current_therapy_settings.wand.valid = TRUE;
	pump_set_speed_Ignore();
	legacy_ent_wand_defaults_loader(entry_number);
	
	smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
	pump_set_speed_Ignore();
	legacy_ent_wand_defaults_loader(entry_number);
	
	current_therapy_settings.wand.valid = FALSE;
	pump_set_speed_Ignore();
	legacy_ent_wand_defaults_loader(entry_number);
	
}

void test_legacy_ent_wand_defaults_loader_3 (void)
{
	
	R2_TYPES entry_number = NON_909;
	put_event_ExpectAndReturn(&evt_tbl,LW_ID_ERROR,1);
	pump_set_speed_Ignore();
	legacy_ent_wand_defaults_loader(entry_number);
	
}



void test_lw_ent_reflex_setup (void)
{
	StateAction Setup = RESET;
	pump_set_speed_Ignore();
	lw_ent_reflex_setup(Setup);
	
	Setup = SET;
	lw_ent_reflex_setup(Setup);
}


void test_lw_ent_timer_needed (void)
{
	current_therapy_settings.controller = ENT;
	legacy_wand_params_obj.R2 = 2400;
	
	bool return_value = lw_ent_timer_needed();
}

void test_ent_timer_true_2 (void)   //line 595 v4.11
{
	setup_legacy_wand(909,909);
	R2_TYPES entry_number = WAND_750;
	current_therapy_settings.pump.sensor_status = 0xFFFF;
	current_therapy_settings.legacy_wand_levels.topaz_timer = 1;
	put_event_IgnoreAndReturn(1);
	current_therapy_settings.controller = ENT;
	legacy_wand_params_obj.R2 = 2400;
	get_priming_status_IgnoreAndReturn(0);
	legacy_wand_read_adc_values();
	
	setup_legacy_wand(750,750);
	entry_number = WAND_750;
	current_therapy_settings.pump.sensor_status = 0xFFFF;
	current_therapy_settings.legacy_wand_levels.topaz_timer = 1;
	put_event_IgnoreAndReturn(1);
	current_therapy_settings.controller = ENT;
	legacy_wand_params_obj.R2 = 2400;
	pump_set_speed_Ignore();
	get_priming_status_IgnoreAndReturn(0);
	legacy_wand_read_adc_values();
	
}

void test_lw_use_limit_time_loader ()
{
   R2_TYPES wand_r2_type = WAND_909;
   wand_type_value = 2;
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   lw_use_limit_time_loader (wand_r2_type,1);
}

void test_lw_use_limit_time_loader_1 ()
{
   R2_TYPES wand_r2_type = NON_909;
   wand_type_value = 1;
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   lw_use_limit_time_loader (wand_r2_type,1);
}

void test_lw_use_limit_time_loader_2 ()
{
   R2_TYPES wand_r2_type = NON_909;
   wand_type_value = 1;
   put_event_ExpectAndReturn(&evt_tbl,LW_EXPIRED,1);
   lw_use_limit_time_loader (wand_r2_type,10);
}

void test_le_use_limit_time_loader_3 ()
{
   legacy_wand_params_obj.ambient_capable = 0;
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   setup_legacy_wand(576,909);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_le_use_limit_time_loader_4 ()
{
   uint32_t pullup_r = 499;
   uint32_t r1_value = 1210;
   uint32_t r2_value = 750;

   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));

   previous_wands_database[0].R1_wand_type = 16;
   previous_wands_database[0].R2_adc_value = r2_adc_value;

   legacy_wand_params_obj.ambient_capable = 0;
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   setup_legacy_wand(1210,750);
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
   // setup_legacy_wand(1210,750);
   // legacy_wand_read_adc_values();
   AD7265_read_value_IgnoreAndReturn(1);
   //TEST_ASSERT_EQUAL((50*7*60),LW_7_MIN_ACTIVE_TIME_LIMIT);

}

void test_lw_use_limit_time_loader_5()
{
   R2_TYPES wand_r2_type = WAND_909;
   wand_type_value = 3;
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   lw_use_limit_time_loader (wand_r2_type,1);
}

void test_lw_use_limit_time_loader_6()
{
   R2_TYPES wand_r2_type = WAND_1400;
   wand_type_value = 16;
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,1);
   lw_use_limit_time_loader (wand_r2_type,1);
}

// Simulate disconnecting the wands
void test_wand_disconnect_14 (void)
{
	current_therapy_settings.wand.valid = TRUE;
	smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
	current_therapy_settings.active_wand.smart_wand = ARTHROPLASTY;
	get_priming_status_ExpectAndReturn(PRIME_MANUAL);
   legacy_wand_params_obj.wand_connected = 1;

   uint32_t r1_value = 100000;
   uint32_t r2_value = 9090;

   uint32_t pullup_r = 499;
   // Convert resistor values to adc value
   uint16_t r1_adc_value = (uint16_t)((r1_value*4095)/
                                      (pullup_r+r1_value));
   uint16_t r2_adc_value = (uint16_t)((r2_value*4095)/
                                      (pullup_r+r2_value));
   fill_buff_r2(r2_adc_value);
   fill_buff_r1(r1_adc_value);

   // this function is used to call the
   // legacy_wand_read_adc_values many time to
   // setup the resistor values
   // First read to make sure ADC is within Range
    
   wand_type_value = 7;
   AD7265_read_value_ExpectAndReturn(A6,2048);
   // This is the adc value for R1
   AD7265_read_value_ExpectAndReturn(B5,r1_adc_value);
   AD7265_read_value_ExpectAndReturn(B6,r2_adc_value);
   clear_fuse_blow_Expect();
   put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED,1);
   pump_set_speed_Ignore();
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   legacy_wand_read_adc_values();
}

void test_lw_disconnect (void)
{
	current_therapy_settings.active_wand.legacy_wand = ENT;
    current_therapy_settings.active_wand.smart_wand = ARTHROPLASTY;
    current_therapy_settings.wand.valid = TRUE;
	clear_fuse_blow_Expect();
	change_flow_manta_IgnoreAndReturn(1);
	put_event_ExpectAndReturn(&evt_tbl,LW_UNPLUGGED_LOAD_SMARTWAND,1);
	lw_disconnect();
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
