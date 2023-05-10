#include "unity.h"
#include "MK26F18.h"
#include "Mockad7265.h"
#include "fuse_blow.h"



void setUp(void)
{
}

void tearDown(void)
{
}

void test_poll_adc (void)
{
   //uint8_t poll_adc (void);
   //uint8_t switch_debounce (uint16_t );
   // void read_adc_value(void);
   TEST_IGNORE_MESSAGE("Need to remove the above three functions, not used anymore");
}

void test_blow_fuse (void)
{
   blow_fuse ();
   TEST_ASSERT_EQUAL(0x00010000,GPIOC_PSOR & 0x00010000);
}

void test_clear_fuse_blow (void)
{
   clear_fuse_blow ();
   TEST_ASSERT_EQUAL(0x00010000,GPIOC_PCOR & 0x00010000);
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
