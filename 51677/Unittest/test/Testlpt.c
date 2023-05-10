#include "unity.h"
#include "MK26F18.h"
#include "lpt.h"
#include "cpu.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_LPT_Init (void)
{
   LPTMR0_CMR = 0;
   uint8_t return_value = lpt_init ();
   // Testing for proper setup of LPTMR0_CMR
   TEST_ASSERT_EQUAL(11,LPTMR0_CMR);
}

void test_reset_pulse_cnt (void)
{
   uint8_t return_value = reset_pulse_cnt();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}


