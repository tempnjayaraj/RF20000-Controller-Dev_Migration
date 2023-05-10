#include "unity.h"
#include "unity_internals.h"
#include "state_machine.h"
#include "Mockpsu.h"
#include "Mocksystem.h"
#include "Mockevent_buffer.h"



GEN_SETTINGS current_therapy_settings;
event_table evt_tbl;
const ESDHC_META empty_sd_meta;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_do_nothing (void)
{
   uint8_t return_value = do_nothing();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_init_shutdown (void)
{
   high_volt_off_ExpectAndReturn(1);
   uint8_t return_value = init_shutdown();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_state_switch(void)
{
   put_event_ExpectAndReturn(&evt_tbl,LW_PLUGGED_IN,0);
   uint8_t return_value = state_switch();
   TEST_ASSERT_EQUAL(1,return_value);
}



void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
