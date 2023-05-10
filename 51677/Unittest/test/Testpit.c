#include "unity.h"
#include "MK26F18.h"
#include "pit.h"
#include "cpu.h"


void setUp(void)
{
}

void tearDown(void)
{
}




void test_PIT_Init(void)
{
   pit_init();
   TEST_IGNORE_MESSAGE("Need test");
}

void test_PIT_Config (void)
{
   uint8_t channel = 0;
   uint32_t start_value = 7400000;
   PIT_Config(channel, start_value);
   PIT_MemMapPtr pPIT = PIT_BASE_PTR;
   TEST_ASSERT_EQUAL_UINT32(start_value, pPIT->CHANNEL[channel].LDVAL);
}
void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
