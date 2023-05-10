#include "unity.h"
#include "MK26F18.h"
#include "Mocksystem.h"
#include "psu.h"
#include "Mockevent_buffer.h"

GEN_SETTINGS current_therapy_settings;
bool soft_reboot_detected;

event_table evt_tbl;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_psu_init(void)
{
   psu_init();
   TEST_ASSERT_EQUAL(SIM_SCGC5_PORTB_MASK,(SIM_SCGC5&SIM_SCGC5_PORTB_MASK));
}

void test_power_board_on (void)
{
   uint8_t return_value =  power_board_on();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_disable_power_failure (void)
{
	current_therapy_settings.pump.stator = 8050;
	poll_pwr_button ();
}


void test_poll_pwr_button_0 (void)
{
   uint8_t power_on = 0;
   uint8_t button_on = 0;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT;
   current_therapy_settings.sw.shutdown = TRUE;
   uint8_t return_value = poll_pwr_button();
   return_value = poll_pwr_button();
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK;
   current_therapy_settings.sw.shutdown = TRUE;
   return_value = poll_pwr_button();
   GPIOB_PDOR = GLB_PWR_EN_MASK;
   current_therapy_settings.sw.shutdown = TRUE;
   return_value = poll_pwr_button();
   //put_event_ExpectAndReturn(&evt_tbl, POWER_UP,0);
   return_value = poll_pwr_button();
}

void test_poll_pwr_button_1 (void)
{
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on
   uint8_t return_value;
   current_therapy_settings.sw.shutdown = FALSE;
   //button_status = 1, power_on =1
   for (uint8_t i = 0; i < 29;i++)
   {
      // To get T/F condition for button_status and power_on
      if (i > 5)
      {
         GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status
         GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on
      }
      if (i > 10)
      {
         GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status
         GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on
      }

      return_value = poll_pwr_button();
      TEST_ASSERT_EQUAL(1,return_value);
   }
   return_value = poll_pwr_button();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_2 (void)
{
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
   uint8_t return_value;
   current_therapy_settings.sw.shutdown = FALSE;
   //button_status = 1, power_on =1
   for (uint8_t i = 0; i < 140;i++)
   {
      return_value = poll_pwr_button();
      TEST_ASSERT_EQUAL(1,return_value);
      }
   // Having no call to put_event is the test for this case
}
void test_poll_pwr_button_3 (void)
{
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status = 1
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on = 0
   uint8_t return_value;
   current_therapy_settings.sw.shutdown = FALSE;
   return_value = poll_pwr_button();
   TEST_ASSERT_EQUAL(1,return_value);
   // Having no call to put_event is the test for this case
}

void test_psu_off (void)
{
   put_event_ExpectAndReturn(&evt_tbl,SYSTEM_RESET,1);
   uint8_t return_value = psu_off();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_high_volt_off(void)
{
   uint8_t return_value =  high_volt_off();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_4 (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT;
   current_therapy_settings.sw.shutdown = TRUE;
   put_event_ExpectAndReturn (&evt_tbl, POWER_UP,1);
   for (uint8_t i = 0; i < 25; i++)
   {
      return_value = poll_pwr_button ();
   }
}

void test_poll_pwr_button_5 (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status = 1
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
   put_event_ExpectAndReturn(&evt_tbl,SAVE_PARAMS_TO_FLASH,1);
   for (uint8_t i = 0; i < 150; i++)
   {
      return_value = poll_pwr_button ();
   }
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_6 (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status = 1
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on = 0
   return_value = poll_pwr_button ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_poll_pwr_button_7 (void)
{
   uint8_t return_value;
   put_event_IgnoreAndReturn(1);
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status = 1
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
   for (uint8_t i = 0; i < 160; i++)
   {
      return_value = poll_pwr_button ();
      if (i > 15)
      {
         current_therapy_settings.sw.shutdown = FALSE;
         GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
         GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
      }
   }
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_poll_pwr_button_8 (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
   for (uint8_t i = 0; i < 105; i++)
   {
      return_value = poll_pwr_button ();
   }
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_9 (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on = 0
   GPIOB_PDIR |=  0x1<<20|0x1<<22|0x1<<23; //power_failure =0
   GPIOB_PDOR |= GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
   current_therapy_settings.sw.shutdown = TRUE;
   return_value = poll_pwr_button (); // this call is to clear shutdown_delay
   current_therapy_settings.sw.shutdown = FALSE;
   for (uint8_t i = 0; i < 105; i++)
   {
      return_value = poll_pwr_button ();
   }
   TEST_ASSERT_EQUAL(1,return_value);
}


void test_enable_power_failure (void)
{
	current_therapy_settings.pump.stator = 7000;
	poll_pwr_button ();
}


void test_poll_pwr_button_0_pfail_true (void)
{
   uint8_t power_on = 0;
   uint8_t button_on = 0;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT;
   current_therapy_settings.sw.shutdown = TRUE;
   uint8_t return_value = poll_pwr_button();
   return_value = poll_pwr_button();
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK;
   current_therapy_settings.sw.shutdown = TRUE;
   return_value = poll_pwr_button();
   GPIOB_PDOR = GLB_PWR_EN_MASK;
   current_therapy_settings.sw.shutdown = TRUE;
   return_value = poll_pwr_button();
   //put_event_ExpectAndReturn(&evt_tbl, POWER_UP,0);
   return_value = poll_pwr_button();
}

void test_poll_pwr_button_1_pfail_true (void)
{
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on
   uint8_t return_value;
   current_therapy_settings.sw.shutdown = FALSE;
   //button_status = 1, power_on =1
   for (uint8_t i = 0; i < 29;i++)
   {
      // To get T/F condition for button_status and power_on
      if (i > 5)
      {
         GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status
         GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on
      }
      if (i > 10)
      {
         GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status
         GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on
      }

      return_value = poll_pwr_button();
      TEST_ASSERT_EQUAL(1,return_value);
   }
   return_value = poll_pwr_button();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_2_pfail_true (void)
{
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
   uint8_t return_value;
   current_therapy_settings.sw.shutdown = FALSE;
   //button_status = 1, power_on =1
   for (uint8_t i = 0; i < 140;i++)
   {
      return_value = poll_pwr_button();
      TEST_ASSERT_EQUAL(1,return_value);
      }
   // Having no call to put_event is the test for this case
}
void test_poll_pwr_button_3_pfail_true (void)
{
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status = 1
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on = 0
   uint8_t return_value;
   current_therapy_settings.sw.shutdown = FALSE;
   return_value = poll_pwr_button();
   TEST_ASSERT_EQUAL(1,return_value);
   // Having no call to put_event is the test for this case
}

void test_psu_off_pfail_true (void)
{
   put_event_ExpectAndReturn(&evt_tbl,SYSTEM_RESET,1);
   uint8_t return_value = psu_off();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_high_volt_off_pfail_true(void)
{
   uint8_t return_value =  high_volt_off();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_4_pfail_true (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT;
   current_therapy_settings.sw.shutdown = TRUE;
   put_event_ExpectAndReturn (&evt_tbl, POWER_UP,1);
   for (uint8_t i = 0; i < 25; i++)
   {
      return_value = poll_pwr_button ();
   }
}

void test_poll_pwr_button_5_pfail_true (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status = 1
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
   put_event_ExpectAndReturn(&evt_tbl,SAVE_PARAMS_TO_FLASH,1);
   for (uint8_t i = 0; i < 150; i++)
   {
      return_value = poll_pwr_button ();
   }
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_6_pfail_true (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status = 1
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on = 0
   return_value = poll_pwr_button ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_poll_pwr_button_7_pfail_true (void)
{
   uint8_t return_value;
   put_event_IgnoreAndReturn(1);
   GPIOB_PDIR = 0<< PWR_BUTTON_ON_SHIFT; //button_status = 1
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
   for (uint8_t i = 0; i < 160; i++)
   {
      return_value = poll_pwr_button ();
      if (i > 15)
      {
         current_therapy_settings.sw.shutdown = FALSE;
         GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
         GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
      }
   }
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_poll_pwr_button_8_pfail_true (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
   GPIOB_PDOR |= GLB_PWR_EN_MASK; //power_on = 1
   for (uint8_t i = 0; i < 105; i++)
   {
      return_value = poll_pwr_button ();
   }
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_9_pfail_true (void)
{
   uint8_t return_value;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on = 0
   GPIOB_PDIR |=  0x1<<20|0x1<<22|0x1<<23; //power_failure =0
   GPIOB_PDOR |= GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
   current_therapy_settings.sw.shutdown = TRUE;
   return_value = poll_pwr_button (); // this call is to clear shutdown_delay
   current_therapy_settings.sw.shutdown = FALSE;
   for (uint8_t i = 0; i < 105; i++)
   {
      return_value = poll_pwr_button ();
   }
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_pwr_button_10 (void)
{
	 for (uint8_t i = 0; i < 3; i++)
   {
	   put_event_ExpectAndReturn (&evt_tbl, POWER_UP, 1);
      //return_value = poll_pwr_button ();
   }
	//put_event_ExpectAndReturn (&evt_tbl, POWER_UP, 1);
	//put_event_ExpectAndReturn(&evt_tbl,SAVE_PARAMS_TO_FLASH, 1);
   uint8_t return_value;
   current_therapy_settings.pump.stator = 50001;
   GPIOB_PDIR = 1<< PWR_BUTTON_ON_SHIFT; //button_status = 0
   GPIOB_PDOR &= ~GLB_PWR_EN_MASK; //power_on = 0
   GPIOB_PDIR |=  0x1<<20|0x1<<22|0x1<<23; //power_failure =0
   GPIOB_PDOR |= GPIO_PDOR_PDO(GLB_PWR_EN_MASK);
   current_therapy_settings.sw.shutdown = FALSE;
   return_value = poll_pwr_button (); // this call is to clear shutdown_delay
   //current_therapy_settings.sw.shutdown = FALSE;
   for (uint8_t i = 0; i < 105; i++)
   {
	   //put_event_ExpectAndReturn (&evt_tbl, POWER_UP, 1);
      return_value = poll_pwr_button ();
   }
   //TEST_ASSERT_EQUAL(0,powerup_delay);
   //TEST_ASSERT_EQUAL(0,return_value);
}


void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
