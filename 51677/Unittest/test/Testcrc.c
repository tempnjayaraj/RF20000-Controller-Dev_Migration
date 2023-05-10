#include "unity.h"
#include "MK26F18.h"
#include "crc.h"

void setUp(void)
{
}

void tearDown(void)
{
}



void test_slow_crc16(void)
{ 
	uint8_t a[4];
	for( int i = 0; i < 4; i++)
	{
		a[i] =i;
	}
	
	
	uint16_t x = 0;
	uint32_t y =  sizeof(a)/sizeof(a[0]);
	uint16_t return_value =  slow_crc16(x, a, y);
	TEST_ASSERT_EQUAL(4642, return_value);
	TEST_IGNORE_MESSAGE("need to test properly");

}

void test_fast_crc16(void)
{
	uint8_t a[4];
	for( int i = 0; i < 4; i++)
	{
		a[i] =i;
	}
	uint16_t x = 0;
	uint32_t y = sizeof(a)/sizeof(a[0]); ;
	uint16_t return_value =  fast_crc16(x, a, y);
	TEST_IGNORE_MESSAGE("need to test properly");
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
