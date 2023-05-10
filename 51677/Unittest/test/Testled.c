#include "unity.h"
#include "MK26F18.h"
#include "Mocksystem.h"
#include "led.h"
#include "cpu.h"
#include "Mocknotifications.h"
#include "Mockpsu.h"
#include "Mockgui.h"


GEN_SETTINGS current_therapy_settings;
SMART_WAND_PARAMETERS smart_wand_params;
event_table evt_tbl;
legacy_wand_params legacy_wand_params_obj;

void setUp(void)
{
        I2C1_S |= I2C_S_IICIF_MASK;
}

void tearDown(void)
{
}


void test_pcf8575_iic_write_LED_RED(void)
{
   uint8_t value = 0;
   uint8_t packet = 0;

   current_therapy_settings.leds.pump = LED_RED;
   current_therapy_settings.leds.pwr = LED_RED;
   current_therapy_settings.leds.lemo = LED_RED;
   current_therapy_settings.leds.odu = LED_RED;
   pcf8575_iic_write(value);
   packet &= ~PUMP_LED_RED;
   packet |= PUMP_LED_GRN;
   packet &= ~PWR_LED_RED;
   packet |= PWR_LED_GRN;
   packet &= ~ODU_LED_RED;
   packet |= ODU_LED_GRN;
   packet &= ~LEMO_LED_RED;
   packet |= LEMO_LED_GRN;
   TEST_ASSERT_EQUAL(packet,I2C1_D);

}

void test_pcf8575_iic_write_LED_GRN(void)
{
   uint8_t value = 0;
   uint8_t packet = 0;

   current_therapy_settings.leds.pump = LED_GRN;
   current_therapy_settings.leds.pwr = LED_GRN;
   current_therapy_settings.leds.lemo = LED_GRN;
   current_therapy_settings.leds.odu = LED_GRN;
   pcf8575_iic_write(value);
   packet &= ~PUMP_LED_GRN;
   packet |= PUMP_LED_RED;
   packet &= ~PWR_LED_GRN;
   packet |= PWR_LED_RED;
   packet &= ~ODU_LED_GRN;
   packet |= ODU_LED_RED;
   packet &= ~LEMO_LED_GRN;
   packet |= LEMO_LED_RED;
   TEST_ASSERT_EQUAL(packet,I2C1_D);

}

void test_pcf8575_iic_write_LED_OFF(void)
{
   uint8_t value = 0;
   uint8_t packet = 0;

   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.lemo = LED_OFF;
   current_therapy_settings.leds.odu = LED_OFF;
   pcf8575_iic_write(value);
   packet |= PUMP_LED_RED | PUMP_LED_GRN;
   packet |= PWR_LED_RED | PWR_LED_GRN;
   packet |= ODU_LED_RED | ODU_LED_GRN;
   packet |= LEMO_LED_RED | LEMO_LED_GRN;
   TEST_ASSERT_EQUAL(packet,I2C1_D);

}

/* This test is to test the default case, ie, neither GRN, nor RED
   nor OFF */
void test_pcf8575_iic_write_LED_FLASHING_GRN(void)
{
   uint8_t value = 0;
   uint8_t packet = 0;

   current_therapy_settings.leds.pump = LED_FLASHING_GRN;
   current_therapy_settings.leds.pwr = LED_FLASHING_GRN;
   current_therapy_settings.leds.lemo = LED_FLASHING_GRN;
   current_therapy_settings.leds.odu = LED_FLASHING_GRN;
   pcf8575_iic_write(value);
   TEST_ASSERT_EQUAL(packet,I2C1_D);
}

void test_pcf8575_iic_write_case1_LED_RED(void)
{
   uint8_t value = 1;
   uint8_t packet = 0;
   current_therapy_settings.leds.wand = LED_RED;

   pcf8575_iic_write(value);
   packet &= ~WAND_LED_RED;
   packet |= WAND_LED_GRN;

   TEST_ASSERT_EQUAL(packet,I2C1_D);
}

void test_pcf8575_iic_write_case1_LED_GRN(void)
{
   uint8_t value = 1;
   uint8_t packet = 0;
   current_therapy_settings.leds.wand = LED_GRN;

   pcf8575_iic_write(value);
   packet &= ~WAND_LED_GRN;
   packet |= WAND_LED_RED;

   TEST_ASSERT_EQUAL(packet,I2C1_D);
}

void test_pcf8575_iic_write_case1_LED_OFF(void)
{
   uint8_t value = 1;
   uint8_t packet = 0;
   current_therapy_settings.leds.wand = LED_OFF;

   pcf8575_iic_write(value);
   packet |= WAND_LED_RED | WAND_LED_GRN;

   TEST_ASSERT_EQUAL(packet,I2C1_D);
}

/* This test is to test the default case, ie, neither GRN, nor RED
   nor OFF */
void test_pcf8575_iic_write_case1_LED_FLASHING_GRN(void)
{
   uint8_t value = 1;
   uint8_t packet = 0;
   current_therapy_settings.leds.wand = LED_FLASHING_GRN;
   pcf8575_iic_write(value);
   TEST_ASSERT_EQUAL(packet,I2C1_D);
}


void test_pcf8575_iic_write_case2(void)
{
   uint8_t value = 2;
   uint8_t packet = 0;
   current_therapy_settings.leds.wand = LED_OFF;

   pcf8575_iic_write(value);
   TEST_ASSERT_EQUAL(0,I2C1_C1&(I2C_C1_MST_MASK|I2C_C1_TX_MASK));
}

/* This is a test for default case of the switch statement, so there is no test */

void test_pcf8575_iic_write_case3(void)
{
   uint8_t value = 3;
   uint8_t packet = 0;
   pcf8575_iic_write(value);
}


void test_PCF8575_I2C_StartTransmission (void)
{
   uint8_t x = 1;
   PCF8575_I2C_StartTransmission (x);
   TEST_ASSERT_EQUAL(PCF8575_I2C_ADDRESS|x,I2C1_D);
   TEST_IGNORE_MESSAGE("Need to check to incoming x for mode values");

}

void test_LED_Init(void)
{
   led_init();

   //TEST_FAIL_MESSAGE("Need test");
   TEST_ASSERT_EQUAL((I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK),I2C1_C1&(I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK));
}

void test_update_leds_0(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(1);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   get_priming_status_IgnoreAndReturn(0); 
 update_leds();
   TEST_ASSERT_EQUAL(LED_GRN,current_therapy_settings.leds.pwr);
}

void test_update_leds_1(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(1);
   wand_error_type_decipher_IgnoreAndReturn(1);
   wand_error_type_decipher_IgnoreAndReturn(1);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   get_priming_status_IgnoreAndReturn(0); 
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.pump);
   get_priming_status_IgnoreAndReturn(0); 
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.pump);
   get_priming_status_IgnoreAndReturn(0);
update_leds();
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.pump);
}

void test_update_leds_led_red(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   // LED_FLASHING_GRN
   wand_error_type_decipher_IgnoreAndReturn(1);
   wand_error_type_decipher_IgnoreAndReturn(1);
   current_therapy_settings.pump.tube_present = 1;
   current_therapy_settings.pump.door_present = 1;
   current_therapy_settings.pump.sensor_status |= 0x00;
   get_priming_status_IgnoreAndReturn(0);
update_leds();
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.pump);
}


void test_update_leds_2(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   // LED_FLASHING_GRN
   wand_error_type_decipher_IgnoreAndReturn(1);
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = WAND_SHORT_ERROR_TXT;
   //current_therapy_settings.notify.status |= AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
   //current_therapy_settings.notify.status |= WAND_DISCONNECTED_NOTIFICATION_MASK;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_RED, current_therapy_settings.leds.wand);

}
void test_update_leds_3(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(1);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = AMBIENT_ERROR_TXT;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_RED, current_therapy_settings.leds.wand);

}

void test_update_leds_4(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   //current_therapy_settings.notify.wand_switch_stuck = 1;
   current_therapy_settings.notify.status = NO_TXT;
   wand_error_type_decipher_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_RED, current_therapy_settings.leds.wand);

}

void test_update_leds_5(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(2);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_GRN, current_therapy_settings.leds.wand);
}

void test_update_leds_6(void)
{
   wand_error_type_decipher_IgnoreAndReturn(2);
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_GRN;
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;

   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.notify.fp_switch_stuck = 1;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_RED, current_therapy_settings.leds.lemo);
}

void test_update_leds_7(void)
{
   wand_error_type_decipher_IgnoreAndReturn(2);
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_GRN;
   current_therapy_settings.leds.lemo = LED_OFF;
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.wand.unlocked = 1;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.footpedal.present = 0;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_RED, current_therapy_settings.leds.lemo);
}

void test_update_leds_8(void)
{
   wand_error_type_decipher_IgnoreAndReturn(2);
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_GRN;
   current_therapy_settings.leds.lemo = LED_OFF;
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.notify.status = 0;
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.notify.fp_switch_stuck = 0;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_GRN, current_therapy_settings.leds.lemo);
}

void test_update_leds_8_fp_blink_duty_count(void)
{
   wand_error_type_decipher_IgnoreAndReturn(2);
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_GRN;
   current_therapy_settings.leds.lemo = LED_OFF;
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.notify.status = 0;
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.notify.fp_switch_stuck = 0;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF, current_therapy_settings.leds.lemo);
}

void test_update_leds_9(void)
{
   wand_error_type_decipher_IgnoreAndReturn(2);
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_GRN;
   current_therapy_settings.leds.lemo = LED_OFF;
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.notify.status = 0;
   current_therapy_settings.footpedal.present = 0;
   current_therapy_settings.notify.fp_switch_stuck = 0;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   legacy_wand_params_obj.wand_909 = 0;
   current_therapy_settings.treatment.finger_switch_en = 1;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF, current_therapy_settings.leds.lemo);
}


// void test_update_leds_9(void)
// {
//    current_therapy_settings.leds.pwr = LED_OFF;
//    current_therapy_settings.leds.pump = LED_OFF;
//    current_therapy_settings.leds.wand = LED_GRN;
//    current_therapy_settings.leds.lemo = LED_GRN;
//    // LED_FLASHING_GRN
//    current_therapy_settings.pump.tube_present = 0;
//    current_therapy_settings.pump.door_present = 0;
//    current_therapy_settings.pump.sensor_status |= 0x00;
//    //current_therapy_settings.notify.status = WAND_SHORTED_NOTIFICATION_MASK;
//    //current_therapy_settings.notify.status |= AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
//    current_therapy_settings.notify.status = 0;
//    current_therapy_settings.wand.unlocked = 0;
//    current_therapy_settings.notify.status = 0;
//    current_therapy_settings.footpedal.present = 0;
//    get_priming_status_IgnoreAndReturn(0);
 // update_leds();
//    TEST_ASSERT_EQUAL(LED_OFF, current_therapy_settings.leds.lemo);
// }


void test_startup_leds(void)
{
   current_therapy_settings.leds.startup = TRUE;

   startup_leds();//count = 0
   // Calling startup_leds for the first time, so count should be zero
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.wand);
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.odu);
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.lemo);
   TEST_ASSERT_EQUAL(LED_RED,current_therapy_settings.leds.pump);
   TEST_ASSERT_EQUAL(LED_GRN,current_therapy_settings.leds.pwr);

   startup_leds(); //count =1
   TEST_ASSERT_EQUAL(LED_GRN,current_therapy_settings.leds.pump);
   startup_leds(); //count =2
   startup_leds(); //count =3
   startup_leds(); //count =4
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.pump);
   startup_leds(); //count =5
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.pump);
   TEST_ASSERT_EQUAL(LED_RED,current_therapy_settings.leds.wand);
   startup_leds(); //count =6
   TEST_ASSERT_EQUAL(LED_GRN,current_therapy_settings.leds.wand);
   startup_leds(); //count =7
   startup_leds(); //count =8
   startup_leds(); //count =9
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.wand);
   startup_leds(); //count =10
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.wand);
   TEST_ASSERT_EQUAL(LED_RED,current_therapy_settings.leds.odu);
   startup_leds(); //count =11
   TEST_ASSERT_EQUAL(LED_GRN,current_therapy_settings.leds.odu);
   startup_leds(); //count =12
   startup_leds(); //count =13
   startup_leds(); //count =14
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.odu);
   startup_leds(); //count =15
   TEST_ASSERT_EQUAL(LED_RED,current_therapy_settings.leds.lemo);
   startup_leds(); //count =16
   TEST_ASSERT_EQUAL(LED_GRN,current_therapy_settings.leds.lemo);
   startup_leds(); //count =17
   startup_leds(); //count =18
   startup_leds(); //count =19
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.lemo);
   startup_leds(); //count =20
   startup_leds(); //count =21
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.leds.startup);
}

void test_shutdown_led_sequence(void)
{
   uint8_t value = 0;
   power_button_status_ExpectAndReturn(1);
   uint8_t return_value =  shutdown_led_sequence(); //alt 1
   TEST_ASSERT_EQUAL(LED_RED,current_therapy_settings.leds.pwr);
   power_button_status_ExpectAndReturn(1);
   return_value = shutdown_led_sequence(); //alt 0
   TEST_ASSERT_EQUAL(LED_GRN,current_therapy_settings.leds.pwr);
   power_button_status_ExpectAndReturn(0);
   return_value = shutdown_led_sequence(); //alt 1
   TEST_ASSERT_EQUAL(LED_OFF,current_therapy_settings.leds.pwr);
}

// void test_control_leds(void)
// {
//    uint8_t return_value = control_leds();
//    TEST_FAIL_MESSAGE("Need test");
// }

void test_startup_led_init (void)
{
   uint8_t return_value = startup_led_init();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_update_leds_lw(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(2);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_GRN, current_therapy_settings.leds.wand);
}


void test_update_leds_lw_wand_blink_duty_count(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(2);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.active_wand.lw_active = 0;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF, current_therapy_settings.leds.wand);
}

void test_update_leds_lw_2(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(0);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_GRN, current_therapy_settings.leds.odu);
}

void test_update_leds_lw_3(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(0);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.active_wand.lw_active = 1;
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF, current_therapy_settings.leds.wand);
}

void test_update_leds_lw_wired_footpedal(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(0);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.wired.present = 1;
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.active_wand.lw_active = 0;
   current_therapy_settings.footpedal.wired.present = 1;
   
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_GRN, current_therapy_settings.leds.wand);
}

void test_update_leds_lw_wired_footpedal_2(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(0);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.wired.present = 1;
   current_therapy_settings.footpedal.present = 1;
   
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.wired.present = 1;
   
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF, current_therapy_settings.leds.wand);
}

void test_update_leds_lw_wired_footpedal_3(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(0);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.wired.present = 0;
   current_therapy_settings.footpedal.present = 1;
   
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.wired.present = 1;
   
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF, current_therapy_settings.leds.wand);
}

void test_update_leds_pump_fault_last(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(0);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = PUMP_FAULT_TXT;
   
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.wired.present = 0;
   current_therapy_settings.footpedal.present = 1;
   
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.wired.present = 1;
   
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_RED, current_therapy_settings.leds.pump);
}


void test_update_leds_no_fw_no_wireless (void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(0);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = PUMP_FAULT_TXT;
   
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.wired.present = 0;
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.treatment.finger_switch_en = 0;
   current_therapy_settings.footpedal.wireless.present = 1;
   
   
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.wired.present = 0;
   
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_OFF, current_therapy_settings.leds.lemo);
}

void test_update_leds_pump_on_last(void)
{
   current_therapy_settings.leds.pwr = LED_OFF;
   current_therapy_settings.leds.pump = LED_OFF;
   current_therapy_settings.leds.wand = LED_OFF;
   wand_error_type_decipher_IgnoreAndReturn(0);
   // LED_FLASHING_GRN
   current_therapy_settings.pump.tube_present = 0;
   current_therapy_settings.pump.door_present = 0;
   current_therapy_settings.pump.sensor_status |= 0x00;
   current_therapy_settings.notify.status = NO_TXT;
   current_therapy_settings.pump.on = 1;
   
   
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.wired.present = 0;
   current_therapy_settings.footpedal.present = 1;
   
   //   current_therapy_settings.notify.wand_switch_stuck = 0;
   current_therapy_settings.wand.unlocked = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.wired.present = 1;
   
   get_priming_status_IgnoreAndReturn(0);
 update_leds();
   // current_therapy_settings.wand.unlocked = 0;
   // get_priming_status_IgnoreAndReturn(0);
 update_leds();
   TEST_ASSERT_EQUAL(LED_GRN, current_therapy_settings.leds.pump);
}



void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
