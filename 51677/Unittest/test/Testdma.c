#include "unity.h"
#include "MK26F18.h"
#include "dma.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_DMAMUX_Init()
{
   uint8_t return_value = dma_mux_init();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
