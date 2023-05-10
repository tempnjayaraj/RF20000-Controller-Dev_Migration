#include "unity.h"
#include "common.h"
#include "MK26F18.h"
#include "Mocksystem.h"
#include "Mockevent_buffer.h"
#include "can.h"
#include "cpu.h"
#include "Mockrf.h"
#include "Mockwand.h"
#include "Mocksw_manager.h"
#include <stdlib.h>
#include <string.h>
#include "head.h"

#define MFG_SERIAL_NUM_CHARS            16                                   

legacy_wand_params               legacy_wand_params_obj;
GEN_SETTINGS current_therapy_settings;
SMART_WAND_PARAMETERS smart_wand_params;
GEN_SETTINGS *const pcurrent_therapy_settings = &current_therapy_settings;
SMART_WAND_PARAMETERS *const psmart_wand_params = &smart_wand_params;
event_table evt_tbl;
static CAN_MemMapPtr pFlexCAN = CAN0_BASE_PTR;
const FTM_MemMapPtr pSmartWandFTM = FTM3_BASE_PTR;

program_header head;
program_header *pheader;
uint8_t bcd_serial_number[MFG_SERIAL_NUM_CHARS];

void setUp(void)
{
	pFlexCAN->MCR = 0; 
	pFlexCAN->MCR |=  CAN_MCR_SOFTRST_MASK;
	pFlexCAN->MCR |=  CAN_MCR_FRZACK_MASK;
	
	// Trying to set these masks to get by the "while" settling loops in can.c
	//CAN_MCR_LPMACK_MASK = 0 0x100000u
	//CAN_MCR_SOFTRST_MASK = 0 0x2000000u
	//CAN_MCR_FRZACK_MASK = 1  0x1000000u
	//CAN_MCR_NOTRDY_MASK = 0
	//CAN_MCR_FRZACK_MASK = 0
        pheader = &head;
}

void tearDown(void)
{
}


// void test_flexCAN_readMB(void)
// {
   // flexCAN_readMB();
   // TEST_ASSERT_EQUAL(0,pFlexCAN->IFLAG1);
// }

void test_read_flexcan(void)
{
   uint8_t return_value = read_flexcan();
   TEST_ASSERT_EQUAL(0,return_value);
}

/**
 *void test_flexCAN_write_all_msgs(void)
 *{
 *   uint8_t return_value = flexCAN_write_all_msgs();
 *   TEST_ASSERT_EQUAL(0,return_value);
 *}
 */


void test_FlexCAN_Write0x82Msg(void)
{
   //FlexCAN_Write0x82Msg();
   TEST_IGNORE_MESSAGE("No function definition");
}

void test_FlexCAN_Init(void)
{
   TEST_IGNORE_MESSAGE("5 while waits in flexcan_init");
   // There are two "while" wait conditions, in the first one
   // CAN_MCR_FRZACK_MASK needs to cleared and in the second one
   // CAN_MCR_FRZACK_MASK needs to be set. So this function never
   // returns unless the HW resets the bits

   //FlexCAN_Init();

}


void test_can_wdog_rx (void)
{
   uint8_t return_value = can_wdog_rx();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_flexCAN_write_fast_msgs (void)
{
   GPIOB_PDIR  = (1<<9);
   uint8_t return_value  = flexCAN_write_fast_msgs();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_flexCAN_write_slow_msgs (void)
{
   uint8_t return_value  = flexCAN_write_slow_msgs();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_flexcan_init(void)
{
   uint8_t return_value = flexcan_init();
   TEST_ASSERT_EQUAL(1,return_value);
}


void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
