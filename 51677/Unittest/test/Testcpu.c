#include "unity.h"
#include "MK26F18.h"
#include "cpu.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_system_reset (void)
{
   uint8_t return_value;
   return_value = system_reset();
   TEST_ASSERT_EQUAL(0,return_value);
}

// void test_system_reset (void)
// {
//    uint8_t return_value = system_reset();
//    TEST_ASSERT_EQUAL(0,return_value);
// }

void test_Write_Vtor (void)
{
   int random_value = 8;
   Write_Vtor (random_value);
   TEST_ASSERT_EQUAL(random_value,SCB_VTOR);
}


void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
