#include "unity.h"
#include "unity_internals.h"
#include "mcg.h"
#include "MK26F18.h"

//GEN_SETTINGS CurrentTherapySettings;
//GEN_SETTINGS *const pCurrentTherapySettings = &CurrentTherapySettings;


void setUp(void)
{
}

void tearDown(void)
{
}

void test_PLL_Init (void)
{
   //PLL_Init();
    TEST_IGNORE_MESSAGE("CANNOT TEST PLL_Init because of while loop waits");
}
void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
