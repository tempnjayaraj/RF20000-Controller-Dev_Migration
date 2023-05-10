#include "unity.h"
#include "unity_internals.h"
#include "MK26F18.h"
#include <stdint.h>
#include "rcm.h"
#include "Mockevent_buffer.h"
#include "Mocksystem.h"

event_table evt_tbl;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_reset_monitor (void)
{
   RCM_SRS0 &= ~RCM_SRS0_WDOG_MASK;
   uint8_t return_value = reset_monitor();
   TEST_ASSERT_EQUAL(0,return_value);
   RCM_SRS0 |= RCM_SRS0_WDOG_MASK;
   put_event_ExpectAndReturn(&evt_tbl,WDOG_COP_RESET,1);
   return_value = reset_monitor();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_software_reset (void)
{
   software_reset();
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
