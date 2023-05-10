#include "unity.h"
#include "unity_internals.h"
#include "MK26F18.h"
#include "Mockpit.h"
#include "time.h"
#include "rtc.h"
#include "cpu.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_RTC_Init (void)
{
   rtc_init();
   //TEST_FAIL_MESSAGE("NEED to WRITE TEST");
   TEST_ASSERT_EQUAL(RTC_CR_OSCE_MASK,(RTC_CR&RTC_CR_OSCE_MASK));
}
void test_rtc_check_errors_RTC_SR_01 (void)
{
   RTC_SR = RTC_SR_TIF_MASK;
   rtc_check_errors();
   TEST_ASSERT_EQUAL(0x0,RTC_TSR);
}

void test_rtc_check_errors_RTC_SR_02 (void)
{
   RTC_SR = RTC_SR_TOF_MASK;
   rtc_check_errors();
   TEST_ASSERT_EQUAL(0x0,RTC_TSR);
}

void test_rtc_check_errors_RTC_SR_03 (void)
{
   RTC_TAR = 0;
   RTC_SR = RTC_SR_TAF_MASK;
   rtc_check_errors();
   TEST_ASSERT_EQUAL(0xFFFFFFFF,RTC_TAR);
}

void test_rtc_check_errors_RTC_SR_04 (void)
{
   RTC_TAR = 0;
   RTC_SR = 0;
   rtc_check_errors();
   // No check for this empty else case
}



void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
