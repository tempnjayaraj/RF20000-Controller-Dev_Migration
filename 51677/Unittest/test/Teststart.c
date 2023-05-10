#include "unity.h"
#include <stdint.h>
#include "cpu.h"
#include "Mockcrc.h"
#include "Mocksystem.h"
#include "event_buffer.h"
#include "Mockmcg.h"
#include "Mockpsu.h"
#include "start.h"

event_table evt_tbl;
//uint8_t __checksum_begin_main[10];

void setUp(void)
{
}

void tearDown(void)
{
}

void test_start (void)
{
   //start();
   TEST_IGNORE_MESSAGE("This file uses exact main app locations, so it is not unit tested");

}

void test_rom2ram (void)
{
   //rom2ram();
   TEST_IGNORE_MESSAGE("This file uses exact main app locations, so it is not unit tested");
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
