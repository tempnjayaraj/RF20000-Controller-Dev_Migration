#include "unity.h"
#include "unity_internals.h"
#include "sw_manager.h"
#include "Mocksystem.h"
#include "Mockevent_buffer.h"
#include "cpu.h"
//#include "esdhc.h"
#include "head.h"

GEN_SETTINGS current_therapy_settings;
event_table evt_tbl;
program_header head;
extern __root program_header *pheader;
//
void setUp(void)
{
   pheader = &head;
}

void tearDown(void)
{
}

void test_commence_uis_sw_update(void)
{
   put_event_ExpectAndReturn(&evt_tbl,UI_UPDATE_STATUS,1);
   uint8_t rv = commence_uis_sw_update();
}

void test_cancel_update (void)
{
   put_event_ExpectAndReturn(&evt_tbl,COMMENCE_GS_SW_UPDATE,1);
   uint8_t rv = cancel_sw_update();
   TEST_ASSERT_EQUAL(0,rv);
}

void test_commence_gs_sw_update(void)
{
   current_therapy_settings.sdhc.gsfw_update_avaliable = 0;
   current_therapy_settings.sdhc.card_ready = 1;
   uint8_t rv = commence_gs_sw_update();
   TEST_ASSERT_EQUAL(0,rv);
}

void test_commence_gs_sw_update_1(void)
{
   current_therapy_settings.sdhc.gsfw_update_avaliable = 0;
   current_therapy_settings.sdhc.card_ready = 0;
   uint8_t rv = commence_gs_sw_update();
   TEST_ASSERT_EQUAL(1,rv);
}

void test_commence_gs_sw_update_2(void)
{
   current_therapy_settings.sdhc.gsfw_update_avaliable = 1;
   current_therapy_settings.sdhc.card_ready = 1;
   put_event_ExpectAndReturn(&evt_tbl,SDHC_DOWNLOAD,1);
   uint8_t rv = commence_gs_sw_update();
   TEST_ASSERT_EQUAL(0,rv);
}

void test_ui_update_status (void)
{
   current_therapy_settings.gui.sw.update_status = UPDATE_COMPLETED;
   uint8_t rv = ui_update_status();
   TEST_ASSERT_EQUAL(1,rv);
}

void test_check4gsfw(void)
{
   uint8_t major= 2;
   uint8_t minor=0;
   head.meta.app_version_major = 1;
   head.meta.app_version_minor = 1;
   uint8_t rv= check4gsfw(major,minor);
   TEST_ASSERT_EQUAL(1,rv);
}

void test_check4gsfw_1(void)
{
   uint8_t major= 2;
   uint8_t minor=0;
   head.meta.app_version_major = 3;
   head.meta.app_version_minor = 1;
   uint8_t rv= check4gsfw(major,minor);
   TEST_ASSERT_EQUAL(0,rv);
}


void test_check4gsfw_2(void)
{
   uint8_t major= 0;
   uint8_t minor=0;
   head.meta.app_version_major = 1;
   head.meta.app_version_minor = 1;
   uint8_t rv= check4gsfw(major,minor);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_check4uifw(void)
{
   uint8_t major= 2;
   uint8_t minor=0;
   current_therapy_settings.sdhc.uifw_major= 3;
   current_therapy_settings.sdhc.uifw_minor= 1;
   uint8_t rv= check4uifw(major,minor);
   TEST_ASSERT_EQUAL(1,rv);
}

void test_check4uifw_1(void)
{
   uint8_t major= 1;
   uint8_t minor=3;
   current_therapy_settings.sdhc.uifw_major= 0;
   current_therapy_settings.sdhc.uifw_minor= 0;
   current_therapy_settings.sdhc.gsfw_update_avaliable = 1;
   uint8_t rv= check4uifw(major,minor);
   TEST_ASSERT_EQUAL(1,rv);
}


void test_check4uifw_2(void)
{
   uint8_t major= 1;
   uint8_t minor=0;
   current_therapy_settings.sdhc.gsfw_update_avaliable = 0;
   uint8_t rv= check4uifw(major,minor);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_check4uifw_4(void)
{
   uint8_t major= 1;
   uint8_t minor=0;
   current_therapy_settings.sdhc.uifw_major= 3;
   current_therapy_settings.sdhc.uifw_minor= 1;
   uint8_t rv= check4uifw(major,minor);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_ui_gs_firmware_update_pause (void)
{
	current_therapy_settings.sdhc.card_loaded = TRUE;
	
	//put_event_ExpectAndReturn(&evt_tbl,COMMENCE_GS_SW_UPDATE,1);
	uint8_t return_value = ui_gs_firmware_update_pause();
	TEST_ASSERT_EQUAL(0,return_value);
	
	
	
	current_therapy_settings.sdhc.card_loaded = FALSE;
	return_value = ui_gs_firmware_update_pause();
	TEST_ASSERT_EQUAL(0,return_value);
	
}
void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}


