#include "unity.h"
#include "MK26F18.h"
#include "ad7265.h"
#include "cpu.h"
#include "Mockevent_buffer.h"

event_table evt_tbl;
bool unitTest;
void setUp(void)
{
}

void tearDown(void)
{
}

void test_AD7265_Init(void)
{
   AD7265_Init();
   //TEST_ASSERT_EQUAL(0x90,DMAMUX0_CHCFG4);
}

void test_IRQ_AD7265_Read()
{
   uint16_t return_value;
   unitTest = TRUE;
   SPI0_BASE_PTR->SR = 0xFF;
   for (uint8_t channel = 0; channel <= 6; channel++)
   {
	   if (channel == 6) put_event_IgnoreAndReturn (1);
      return_value  = IRQ_AD7265_Read((char)channel);
   }
   SPI0_BASE_PTR->SR = 0x00;
   put_event_IgnoreAndReturn (1);
   return_value  = IRQ_AD7265_Read(6u);
}

// DOn't need this because AD7265_Init calls this function
// void test_AD7265_DMA_Init(void)
// {
//    AD7265_DMA_Init();
// }

void test_AD7265_read_value (void)
{
   CHAN_NUM channel_num = 0;
   uint16_t return_value = AD7265_read_value (channel_num);
}




void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
