#include "unity.h"
#include "MK26F18.h"
#include "Mocksystem.h"
#include "profiles.h"

GEN_SETTINGS current_therapy_settings;
event_table evt_tbl;
SMART_WAND_PARAMETERS smart_wand_params;
uint8_t current_profile_active = 0;

void setUp(void)
{
	
}

void tearDown(void)
{
	
}

void test_profile_manager (void)
{
   profile_manager();
}

void test_save_profile(void)
{
   uint8_t value = 1;
   smart_wand_params.mfg.low_enable = 1;
   smart_wand_params.mfg.med_enable = 1;
   smart_wand_params.mfg.hi_enable = 1;
   smart_wand_params.mfg.coag_enable = 1;
   
   uint8_t return_value = save_profile(value);
}

// void test_load_profile (void)
// {
//    uint8_t value = 1;
//    uint8_t return_value;
//    return_value = load_profile(value);
// }

void test_save_profile_2(void)
{
   uint8_t value = 1;
   smart_wand_params.mfg.low_enable = 0;
   smart_wand_params.mfg.med_enable = 0;
   smart_wand_params.mfg.hi_enable = 0;
   smart_wand_params.mfg.coag_enable = 0;
   
   uint8_t return_value = save_profile(value);
}



void test_load_profile (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = 1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 63;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_load_profile_2 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = 1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 59;
   test_profile_record.ambient.audio_enabled = 2;
   
   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   //TEST_ASSERT_EQUAL(1,return_value);

}

void test_load_profile_3 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = 1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 2;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   //TEST_ASSERT_EQUAL(1,return_value);

}

void test_load_profile_4 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en =2;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = 1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   //   TEST_ASSERT_EQUAL(1,return_value);

}

void test_load_profile_5 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 2;
   test_profile_record.languageID = 1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   //TEST_ASSERT_EQUAL(1,return_value);
   //   TEST_IGNORE_MESSAGE("wireless enable cannot be set to 2");
}

void test_load_profile_6 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_load_profile_7 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = NUM_CUT_LEVELS;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_load_profile_8 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = ABLATE_ZILCH;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}


void test_load_profile_9 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.med = NUM_CUT_LEVELS;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_load_profile_10 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.med = ABLATE_ZILCH;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}


void test_load_profile_11 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.lo = NUM_CUT_LEVELS;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_load_profile_12 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.lo = ABLATE_ZILCH;
   test_profile_record.setpoint.coag = 2;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_load_profile_13 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.coag = MAX_NUM_OF_ENT_COAG_LEVELS;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   //TEST_ASSERT_EQUAL(1,return_value);
}

void test_load_profile_14 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   ///   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.coag = COAG_0;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;

   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_load_profile_15 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.coag = 1;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;
   current_therapy_settings.footpedal.wireless.married = TRUE;
   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(0,return_value);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.ambient.capable);

}

void test_load_profile_16 (void)
{
   uint8_t value = 1;
   uint8_t return_value;
   // Settiing up a test profile
   profile_record test_profile_record;
   test_profile_record.record_full = 1;
   test_profile_record.finger_switch_en = 1;
   //   test_profile_record.wireless_enable = 1;
   test_profile_record.languageID = MAX_NUM_LANGUAGES-1;
   test_profile_record.setpoint.med = 3;
   test_profile_record.setpoint.hi = 3;
   test_profile_record.setpoint.lo = 3;
   test_profile_record.setpoint.coag = 1;
   test_profile_record.ambient.enable = 1;
   test_profile_record.ambient.alarm_threshold = 60;
   test_profile_record.ambient.audio_enabled = 1;
   current_therapy_settings.footpedal.wireless.married = FALSE;
   current_therapy_settings.user_data.profile[value] = test_profile_record;
   return_value = load_profile(value);
   TEST_ASSERT_EQUAL(0,return_value);
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.treatment.wireless_enable);
}

void test_cancel_new_profile (void)
{
	uint8_t return_value;
	current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
	cancel_new_profile();
	
	current_therapy_settings.gui.screenID = ENTER_NEW_USER_PROFILE_NAME_SCREEN;
	current_therapy_settings.gui.depth_idx = 1;
	cancel_new_profile();
	
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
