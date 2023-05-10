#include "unity.h"
#include "unity_internals.h"
#include "MK26F18.h"
#include "common.h"
#include "wdog.h"
#include <intrinsics.h>



void setUp(void)
{
}

void tearDown(void)
{
   //WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

void test_wdog_enable(void)
{
   wdog_enable();
}

void test_wdog_disable(void)
{
   wdog_disable ();
}

void test_wdog_refresh (void)
{
   uint8_t return_value;
   return_value = wdog_refresh();
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
