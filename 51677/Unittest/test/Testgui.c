#include "unity.h"
#include "common.h"
#include "gui.h"
#include "Mocksystem.h"
#include <stdlib.h>
#include "Mockpit.h"
#include "Mockcpu.h"
#include "Mockevent_buffer.h"
#include "Mockaudio.h"
#include "MK26F18.h"
#include "../../48386/lib/lib.h"
#include "Mockswitches.h"
#include "Mockprofiles.h"
#include "Mocklegacy_wand.h"
#include "Mockwand.h"
#include "systick.h"
#include "Mockpump.h"
#include "Mocknotifications.h"
#include "Mockfootpedal.h"

extern void st(uint8_t);
extern void in(uint8_t);
extern void us(uint8_t);
extern void ad(uint8_t);
extern void NA(uint8_t);
extern void hm(uint8_t);
extern void mm(uint8_t);
extern void lm(uint8_t);

extern void re(uint8_t);
extern void la(uint8_t);
extern void dl(uint8_t);
extern void bt(uint8_t);
extern void hn(uint8_t);
extern void hd(uint8_t);
extern void hp(uint8_t);
extern void mn(uint8_t);
extern void md(uint8_t);
extern void mp(uint8_t);
extern void ln(uint8_t);
extern void ld(uint8_t);
extern void lp(uint8_t);
extern void fi(uint8_t);
extern void fd(uint8_t);
extern void ia(uint8_t);
extern void iv(uint8_t);
extern void c2(uint8_t);
extern void c1(uint8_t);
extern void wn(uint8_t);
extern void lg(uint8_t);
extern void fp(uint8_t);
extern void pr(uint8_t);
extern void ba(uint8_t);
extern void lt(uint8_t);
extern void us(uint8_t);
extern void ti(uint8_t);
extern void ea(uint8_t);
extern void da(uint8_t);
extern void ds(uint8_t);
extern void ai(uint8_t);
extern void ad(uint8_t);
extern void ui(uint8_t);
extern void ud(uint8_t);
extern void es(uint8_t);
extern void td(uint8_t);
extern void wh(uint8_t);
extern void ew(uint8_t);
extern void dw(uint8_t);
extern void ef(uint8_t);
extern void df(uint8_t);

extern void kb(uint8_t);
extern void sv(uint8_t);
extern void dp(uint8_t);
extern void ap(uint8_t);
extern void cd(uint8_t);
extern void rc(uint8_t);
extern void ps(uint8_t);
extern void of(uint8_t);
extern void ls(uint8_t);

extern void leap (uint8_t);
extern void leam (uint8_t);
extern void lecm (uint8_t);
extern void lecp (uint8_t);

extern void sn (uint8_t);
extern void cu (uint8_t);

extern uint8_t m_prime(float spd);

extern void c3(uint8_t);
extern void pm(uint8_t);
extern void pa(uint8_t);
extern void ce(uint8_t);
extern void fil (uint8_t);
extern void fdl (uint8_t);
extern void pml(uint8_t value);

extern void mute_timer(uint8_t value);
extern void gui_stop_auto_prime(void);
extern void gui_clear_auto_prime_event(void);
extern uint8_t change_flow_sp(uint8_t new_flow);
extern uint8_t change_screenID(uint8_t);
extern void pump_set_speed(float Speed);

GEN_SETTINGS current_therapy_settings;
SMART_WAND_PARAMETERS smart_wand_params;
legacy_wand_params legacy_wand_params_obj;
user_data_record user_data;

SMART_WAND_PARAMETERS * const pSmartWandParams = &smart_wand_params;
event_table evt_tbl;
const uint16_t lib_ambient_alarm_sound[6643];
SOUND_LIB lib_sounds[13];
PUMP_CMD pump;

FlagStatus StopAutoPrime = RESET;
extern uint8_t AutoPrimeId;
manta_prime prime;
uint8_t current_coag_setpoint;
extern uint8_t PrimeId;
extern prime_mode_t PrimeState;

extern ErrorStatus cbtmr_reg_callback(cbtmr_func_t cbtmr_func,
                                      FlagStatus *cbtmr_flag,
                                      u32 cbtmr_time_ms,
                                      cbtmr_mode_t cbtmr_mode,
                                      u8 *cbtmr_id);
extern ErrorStatus cbtmr_unreg_callback(u8 UnregID);

void setUp(void) {
}

void tearDown(void) {
}

// Testing function la which adjusts levels (la = level adjustment)
void test_la (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	la (value);
}

// Testing function re which adjusts levels (re = reset error)
void test_re (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, RESET_ERROR, 1);
	re (value);
}


void test_wh(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	put_event_ExpectAndReturn(&evt_tbl, WIRELESS_HANDSHAKE, 1);
	wh(value);
}

void test_us(void)
{
	uint8_t value = 0;
	current_therapy_settings.treatment.ablate_on = 1;
	current_therapy_settings.treatment.coag_on = 1;	
		us(value);
	
	current_therapy_settings.treatment.ablate_on = 0;
	current_therapy_settings.treatment.coag_on = 0; 
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	us(value);
}

void test_ti(void)
{
	uint8_t value = 0;
	current_therapy_settings.ambient.alarm_threshold = 59;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	ti(value);
	// The value is getting incremented till it gets to 60
	TEST_ASSERT_EQUAL(60, current_therapy_settings.ambient.alarm_threshold);
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	ti(value);
	// The value should not get incremented once it hits 60
	TEST_ASSERT_EQUAL(60, current_therapy_settings.ambient.alarm_threshold);
}

void test_ea (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	ea (value);
}


void test_da (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	da (value);
}

void test_es (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	es (value);
}

void test_ds (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	ds (value);
}

void test_td(void)
{
	uint8_t value = 0;
	current_therapy_settings.ambient.alarm_threshold = 21;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	td(value);
	// The value is getting incremented till it gets to 60
	TEST_ASSERT_EQUAL(20, current_therapy_settings.ambient.alarm_threshold);
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	td(value);
	// The value should not get incremented once it hits 60
	TEST_ASSERT_EQUAL(20, current_therapy_settings.ambient.alarm_threshold);
}


void test_ai_1(void)
{
	uint8_t value = 0;
	current_therapy_settings.gui.alarm_test_sound = 0;
	current_therapy_settings.ambient.alarm_active = 0;
	current_therapy_settings.ambient.audio_enabled = 1;
	current_therapy_settings.ambient.volume = 9;
	audio_alarmsound_Expect();
	ai (value);
	TEST_ASSERT_EQUAL(10, current_therapy_settings.ambient.volume);
	ai (value);
	TEST_ASSERT_EQUAL(10, current_therapy_settings.ambient.volume);
}

void test_ai_2(void)
{
	uint8_t value = 0;
	current_therapy_settings.gui.alarm_test_sound = 1;
	ai (value);
	
	current_therapy_settings.gui.alarm_test_sound = 0;
	current_therapy_settings.ambient.alarm_active = 0;
	current_therapy_settings.ambient.audio_enabled = 1;
	audio_alarmsound_Expect();
	ai (value);
	
	current_therapy_settings.gui.alarm_test_sound = 0;
	current_therapy_settings.ambient.alarm_active = 1;
	current_therapy_settings.ambient.audio_enabled = 0;
	audio_alarmsound_Expect();
	ai (value);
	
	current_therapy_settings.gui.alarm_test_sound = 0;
	current_therapy_settings.ambient.alarm_active = 0;
	current_therapy_settings.ambient.audio_enabled = 0;
	audio_alarmsound_Expect();
	ai (value);
}

void test_ad_1(void)
{
	uint8_t value = 0;
	current_therapy_settings.gui.alarm_test_sound = 0;
	current_therapy_settings.ambient.alarm_active = 0;
	current_therapy_settings.ambient.audio_enabled = 1;
	current_therapy_settings.ambient.volume = 2;
	audio_alarmsound_Expect();
	ad (value);
	TEST_ASSERT_EQUAL(1, current_therapy_settings.ambient.volume);
	ad (value);
	TEST_ASSERT_EQUAL(1, current_therapy_settings.ambient.volume);
}

void test_ad_2(void)
{
	uint8_t value = 0;
	current_therapy_settings.gui.alarm_test_sound = 1;
	ad (value);
	
	current_therapy_settings.gui.alarm_test_sound = 0;
	current_therapy_settings.ambient.alarm_active = 0;
	current_therapy_settings.ambient.audio_enabled = 1;
	audio_alarmsound_Expect();
	ad (value);
	
	current_therapy_settings.gui.alarm_test_sound = 0;
	current_therapy_settings.ambient.alarm_active = 1;
	current_therapy_settings.ambient.audio_enabled = 0;
	audio_alarmsound_Expect();
	ad (value);
	
	current_therapy_settings.gui.alarm_test_sound = 0;
	current_therapy_settings.ambient.alarm_active = 0;
	current_therapy_settings.ambient.audio_enabled = 0;
	audio_alarmsound_Expect();
	ad (value);
}

void test_ui (void)
{
   uint8_t value = 0;
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
   ui(value);
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
   ui(value);
   put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
   current_therapy_settings.audio.volume = 9;
   ui (value);
   TEST_ASSERT_EQUAL(10, current_therapy_settings.audio.volume);
   put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
   ui (value);
   TEST_ASSERT_EQUAL(10, current_therapy_settings.audio.volume);
}
	
void test_ud (void)
{
	uint8_t value = 0;
    current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	ud(value);
	//current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	//ud(value);
	
	current_therapy_settings.audio.volume = 2;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	ud (value);
	TEST_ASSERT_EQUAL(1, current_therapy_settings.audio.volume);
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	ud (value);
	TEST_ASSERT_EQUAL(1, current_therapy_settings.audio.volume);
}

void test_fi (void)
{
	uint8_t value = 0;
	current_therapy_settings.wand.valid = TRUE;
	current_therapy_settings.active_wand.smart_wand = ENT;
	current_therapy_settings.controller = ENT;
   	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.flow_rate = 4;
	for (ENT_FLOW_SP_T f = ENT_FLOW_0; f < MAX_ENT_FLOW_SP; f++)
	{
		current_therapy_settings.treatment.ent_flow_sp_enable[f] = 1;
	}
	current_therapy_settings.treatment.therapy_mode = LO;
	current_therapy_settings.setpoint.lo = 0;
	fi (value);
	TEST_ASSERT_EQUAL(ENT,current_therapy_settings.active_wand.smart_wand);
	TEST_ASSERT_EQUAL(ENT,current_therapy_settings.controller);
	TEST_ASSERT_EQUAL(5, current_therapy_settings.treatment.flow_rate);
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	fi (value);
	TEST_ASSERT_EQUAL(5, current_therapy_settings.treatment.flow_rate);
}
	
void test_fd (void){ // edited by Ryan Cho 16 Aug 2018
	uint8_t value = 0;
    current_therapy_settings.treatment.flow_rate = 2;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	for (ENT_FLOW_SP_T f = ENT_FLOW_0; f < MAX_ENT_FLOW_SP; f++)
	{
		current_therapy_settings.treatment.ent_flow_sp_enable[f] = 1;
	}
	current_therapy_settings.treatment.therapy_mode = LO;
	current_therapy_settings.setpoint.lo = 0;
	fd (value);
	TEST_ASSERT_EQUAL(1, current_therapy_settings.treatment.flow_rate);
	
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	fd (value);
	TEST_ASSERT_EQUAL(1, current_therapy_settings.treatment.flow_rate);

}

void test_ia (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.flow_auto = FALSE;
	
	ia(value);
	TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
	
}

void test_iv_1 (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.flow_auto = TRUE;
	
	iv(value);
	TEST_ASSERT_EQUAL(TRUE, current_therapy_settings.treatment.flow_auto);
}

void test_iv_2 (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.flow_auto = FALSE;
	
	iv(value);
	TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
}

void test_c3 (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.coag = COAG_1;
	
	c3(value);
	TEST_ASSERT_EQUAL(COAG_3, current_therapy_settings.setpoint.coag);
}

void test_c2 (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.coag = COAG_1;
	
	c2(value);
	TEST_ASSERT_EQUAL(COAG_2, current_therapy_settings.setpoint.coag);
}

void test_c1 (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.coag = COAG_2;
	
	c1(value);
	TEST_ASSERT_EQUAL(COAG_1, current_therapy_settings.setpoint.coag);
}

void test_wn (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	
	wn(value);
	TEST_ASSERT_EQUAL(WAND_INFO_SCREEN,current_therapy_settings.gui.screenID);
}

void test_lg (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	
	lg(value);
	TEST_ASSERT_EQUAL(LANGUAGES_SCREEN,current_therapy_settings.gui.screenID);
}

void test_fp (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	
	fp(value);
	TEST_ASSERT_EQUAL(SWITCHING_SETTINGS_SCREEN,current_therapy_settings.gui.screenID);
}

void test_pr (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	
	pr(value);
	TEST_ASSERT_EQUAL(USER_PROFILES_SCREEN,current_therapy_settings.gui.screenID);
}

void test_lt (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	
	lt(value);
	TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
}

void test_ew (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
    current_therapy_settings.footpedal.wireless.married = 1;
    current_therapy_settings.footpedal.wireless.married_latch = 1;
    ew(value);
	TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.wireless_enable);

	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
    current_therapy_settings.footpedal.wireless.married = 1;
    current_therapy_settings.footpedal.wireless.married_latch = 0;
    ew(value);
}

void test_dw (void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.wireless_enable = (bool)1;
	current_therapy_settings.footpedal.wireless.present = 1;
	
	dw(value);
	TEST_ASSERT_EQUAL(0,current_therapy_settings.treatment.wireless_enable);
	TEST_ASSERT_EQUAL(0,current_therapy_settings.footpedal.wireless.present);
}

//
//void test_err_ack(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    put_event_ExpectAndReturn(&evt_tbl, RESET_ERROR, 1);
//    err_ack(id);
//}
//
//void test_AblateLevels_setpoint_lo(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    current_therapy_settings.treatment.therapy_mode = LO;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//
//    AblateLevels(LEVEL_1_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_MINUS, current_therapy_settings.setpoint.lo);
//    AblateLevels(LEVEL_2_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_DEFAULT, current_therapy_settings.setpoint.lo);
//    AblateLevels(LEVEL_3_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_PLUS, current_therapy_settings.setpoint.lo);
//    AblateLevels(NULL_GRAPHIC_ID);
//    current_therapy_settings.treatment.therapy_mode = VAC;
//    AblateLevels(NULL_GRAPHIC_ID);
//}
//
//void test_AblateLevels_setpoint_med(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    current_therapy_settings.treatment.therapy_mode = MED;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//
//    AblateLevels(LEVEL_1_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_MINUS, current_therapy_settings.setpoint.med);
//    AblateLevels(LEVEL_2_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_DEFAULT, current_therapy_settings.setpoint.med);
//    AblateLevels(LEVEL_3_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_PLUS, current_therapy_settings.setpoint.med);
//}
//
//void test_AblateLevels_setpoint_hi(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    current_therapy_settings.treatment.therapy_mode = HI;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//
//    AblateLevels(LEVEL_1_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_MINUS, current_therapy_settings.setpoint.hi);
//    AblateLevels(LEVEL_2_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_DEFAULT, current_therapy_settings.setpoint.hi);
//    AblateLevels(LEVEL_3_BUTTON);
//    TEST_ASSERT_EQUAL(ABLATE_PLUS, current_therapy_settings.setpoint.hi);
//}
//
//void test_CoagLevels_zilch(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    current_therapy_settings.setpoint.coag = COAG_1;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    CoagLevels(id);
//    TEST_ASSERT_EQUAL(COAG_ZILCH, current_therapy_settings.setpoint.coag);
//}
//
//void test_CoagLevels_COAG_2(void) {
//    GRAPHIC_ID id = COAG_LEVEL_2_BUTTON;
//    current_therapy_settings.setpoint.coag = COAG_ZILCH;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    CoagLevels(id);
//    TEST_ASSERT_EQUAL(COAG_2, current_therapy_settings.setpoint.coag);
//}
//
//void test_CoagLevels_COAG_1(void) {
//    GRAPHIC_ID id = COAG_LEVEL_1_BUTTON;
//    current_therapy_settings.setpoint.coag = COAG_ZILCH;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    CoagLevels(id);
//    TEST_ASSERT_EQUAL(COAG_1, current_therapy_settings.setpoint.coag);
//}
//
//void test_Mode(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    //Mode(id);
//    TEST_IGNORE_MESSAGE("No function definition - Mode()");
//}
//
void test_Settings_NullCase(void)
{
    uint8_t id = 0;
    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
    st(id);
    TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID, current_therapy_settings.gui.graphics.GraphicNumber);
	TEST_ASSERT_EQUAL(MAIN_SETTINGS_SCREEN, current_therapy_settings.gui.screenID);
}

//void test_Settings_BACK_BUTTON(void) {
//    GRAPHIC_ID id = BACK_BUTTON;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
//    current_therapy_settings.gui.previous_screenID[0] = SPLASH_SCREEN;
//    st(id);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//    TEST_ASSERT_EQUAL(SPLASH_SCREEN, current_therapy_settings.gui.screenID);
//}
//
//void test_Settings_BACK_BUTTON_MAIN_SETTINGS_SCREEN(void) {
//    GRAPHIC_ID id = BACK_BUTTON;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    current_therapy_settings.gui.screenID = MAIN_SETTINGS_SCREEN;
//    st(id);
//    TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.screenID);
//}
//
//void test_Settings_BACK_BUTTON_MAIN_SETTINGS_SCREEN_FALSE(void) {
//    GRAPHIC_ID id = BACK_BUTTON;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
//    st(id);
//    TEST_ASSERT_EQUAL(SPLASH_SCREEN, current_therapy_settings.gui.screenID);
//}
//
//void test_Settings_SETTINGS_BUTTON(void) {
//    GRAPHIC_ID id = SETTINGS_BUTTON;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
//    current_therapy_settings.gui.previous_screenID[0] = SPLASH_SCREEN;
//    st(id);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//    TEST_ASSERT_EQUAL(MAIN_SETTINGS_SCREEN, current_therapy_settings.gui.screenID);
//    TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.previous_screenID[0]);
//}
//
//void test_Indicators(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    Indicators(id);
//    TEST_IGNORE_MESSAGE("NOP - No test needed");
//}
//
//void test_LangPref(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    LangPref(id);
//    TEST_IGNORE_MESSAGE("NOP - No test needed");
//}
//
//void test_UserSettings_NullCase(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    UserSettings(id);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//}
//
//void test_UserSettings_USER_SETTINGS_BUTTON(void) {
//    GRAPHIC_ID id = USER_SETTINGS_BUTTON;
//    current_therapy_settings.gui.screenID = 0;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    UserSettings(id);
//    TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.screenID);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//}
//
//void test_UserSettings_AMBIENT_THRESHOLD_PLUS(void) {
//    GRAPHIC_ID id = AMBIENT_THRESHOLD_PLUS;
//    current_therapy_settings.ambient.alarm_threshold = 59;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    UserSettings(id);
//    TEST_ASSERT_EQUAL(60, current_therapy_settings.ambient.alarm_threshold);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//}
//
//void test_UserSettings_AMBIENT_THRESHOLD_MINUS(void) {
//    GRAPHIC_ID id = AMBIENT_THERSHOLD_MINUS;
//    current_therapy_settings.ambient.alarm_threshold = 21;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    UserSettings(id);
//    TEST_ASSERT_EQUAL(20, current_therapy_settings.ambient.alarm_threshold);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//    TEST_IGNORE_MESSAGE("AMBIENT_THERSHOLD_MINUS - misspelled threshold");
//}
//
//void test_UserSettings_ENABLE_AMBIENT_BUTTON(void) {
//    GRAPHIC_ID id = ENABLE_AMBIENT_BUTTON;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.ambient.enable = 0;
//    UserSettings(id);
//    TEST_ASSERT_EQUAL(1, current_therapy_settings.ambient.enable);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//}
//
//void test_UserSettings_DISABLE_AMBIENT_BUTTON(void) {
//    GRAPHIC_ID id = DISABLE_AMBIENT_BUTTON;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.ambient.enable = 1;
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    UserSettings(id);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.ambient.enable);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//}
//
//void test_UserSettings_ENABLE_AMBIENT_ALARM(void) {
//    GRAPHIC_ID id = ENABLE_AMBIENT_ALARM;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.ambient.audio_enabled = 0;
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    UserSettings(id);
//    TEST_ASSERT_EQUAL(1, current_therapy_settings.ambient.audio_enabled);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//}
//
//void test_UserSettings_DISABLE_AMBIENT_ALARM(void) {
//    GRAPHIC_ID id = DISABLE_AMBIENT_ALARM;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.ambient.audio_enabled = 1;
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    UserSettings(id);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.ambient.audio_enabled);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//}
//
//void test_AudioDisplay_default(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    AudioDisplay(id);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//}
//
//void test_AudioDisplay_USER_TONE_MINUS_BUTTON(void) {
//    GRAPHIC_ID id = USER_TONE_MINUS_BUTTON;
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.audio.volume = 2;
//    AudioDisplay(id);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//    TEST_ASSERT_EQUAL(1, current_therapy_settings.audio.volume);
//}
//
//void test_AudioDisplay_USER_TONE_PLUS_BUTTON(void) {
//    GRAPHIC_ID id = USER_TONE_PLUS_BUTTON;
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    current_therapy_settings.audio.volume = 9;
//    AudioDisplay(id);
//    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.graphics.GraphicNumber);
//    TEST_ASSERT_EQUAL(10, current_therapy_settings.audio.volume);
//}
//
//void test_AudioDisplay_AMBIENT_ALARM_MINUS_BUTTON(void) {
//    GRAPHIC_ID id = AMBIENT_ALARM_MINUS_BUTTON;
//    current_therapy_settings.ambient.volume = 2;
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    audio_alarmsound_Expect();
//    AudioDisplay(id);
//    TEST_ASSERT_EQUAL(1, current_therapy_settings.ambient.volume);
//    current_therapy_settings.gui.alarm_test_sound = 0;
//    current_therapy_settings.ambient.alarm_active = 0;
//    current_therapy_settings.ambient.audio_enabled = 1;
//    audio_alarmsound_Expect();
//    AudioDisplay(id);
//}
//
//void test_AudioDisplay_USER_AMBIENT_ALARM_PLUS_BUTTON(void) {
//    GRAPHIC_ID id = AMBIENT_ALARM_PLUS_BUTTON;
//    current_therapy_settings.ambient.volume = 9;
//    current_therapy_settings.gui.graphics.GraphicNumber = CUT_PLUS_BUTTON;
//    //audio_alarmsound_Expect();
//    AudioDisplay(id);
//    TEST_ASSERT_EQUAL(10, current_therapy_settings.ambient.volume);
//    current_therapy_settings.gui.alarm_test_sound = 0;
//    current_therapy_settings.ambient.alarm_active = 0;
//    current_therapy_settings.ambient.audio_enabled = 1;
//    audio_alarmsound_Expect();
//    AudioDisplay(id);
//}
//
//void test_AudioDisplay_USER_AMBIENT_ALARM_PLUS_BUTTON_2(void) {
//    GRAPHIC_ID id = AMBIENT_ALARM_PLUS_BUTTON;
//    current_therapy_settings.gui.alarm_test_sound = 0;
//    current_therapy_settings.ambient.alarm_active = 0;
//    current_therapy_settings.ambient.audio_enabled = 0;
//    audio_alarmsound_Expect();
//    AudioDisplay(id);
//}
//
//void test_AudioDisplay_USER_AMBIENT_ALARM_PLUS_BUTTON_3(void) {
//    GRAPHIC_ID id = AMBIENT_ALARM_PLUS_BUTTON;
//    current_therapy_settings.gui.alarm_test_sound = 0;
//    current_therapy_settings.ambient.alarm_active = 1;
//    current_therapy_settings.ambient.audio_enabled = 1;
//    AudioDisplay(id);
//}
//
//void test_AudioDisplay_USER_AMBIENT_ALARM_MINUS_BUTTON_2(void) {
//
//    GRAPHIC_ID id = AMBIENT_ALARM_MINUS_BUTTON;
//    current_therapy_settings.gui.alarm_test_sound = 0;
//    current_therapy_settings.ambient.alarm_active = 0;
//    current_therapy_settings.ambient.audio_enabled = 0;
//    audio_alarmsound_Expect();
//    AudioDisplay(id);
//}
//
//void test_AudioDisplay_USER_AMBIENT_ALARM_MINUS_BUTTON_3(void) {
//
//    GRAPHIC_ID id = AMBIENT_ALARM_MINUS_BUTTON;
//    current_therapy_settings.gui.alarm_test_sound = 0;
//    current_therapy_settings.ambient.alarm_active = 1;
//    current_therapy_settings.ambient.audio_enabled = 1;
//    AudioDisplay(id);
//}
//
//void test_AudioDisplay_USER_AMBIENT_ALARM_MINUS_BUTTON_4(void) {
//
//    GRAPHIC_ID id = AMBIENT_ALARM_MINUS_BUTTON;
//    current_therapy_settings.gui.alarm_test_sound = 1;
//    current_therapy_settings.ambient.alarm_active = 1;
//    current_therapy_settings.ambient.audio_enabled = 1;
//    AudioDisplay(id);
//}
//
//void test_AudioDisplay_USER_AMBIENT_ALARM_MINUS_BUTTON_5(void) {
//    put_event_IgnoreAndReturn(1);
//    audio_alarmsound_Ignore();
//    GRAPHIC_ID id = AMBIENT_ALARM_MINUS_BUTTON;
//    current_therapy_settings.gui.alarm_test_sound = 0;
//    current_therapy_settings.ambient.alarm_active = 0;
//    current_therapy_settings.ambient.audio_enabled = 1;
//    for (uint8_t i = 0; i < 12; i++) {
//        AudioDisplay(id);
//    }
//    id = USER_TONE_PLUS_BUTTON;
//    AudioDisplay(id);
//}
//
//void test_AudioDisplay_USER_AMBIENT_ALARM_MINUS_BUTTON_6(void) {
//    audio_alarmsound_Ignore();
//
//    GRAPHIC_ID id = USER_TONE_MINUS_BUTTON;
//    current_therapy_settings.gui.alarm_test_sound = 0;
//    current_therapy_settings.ambient.alarm_active = 0;
//    current_therapy_settings.ambient.audio_enabled = 1;
//    for (uint8_t i = 0; i < 12; i++) {
//        put_event_IgnoreAndReturn(1);
//        AudioDisplay(id);
//    }
//    put_event_IgnoreAndReturn(1);
//    AudioDisplay(id);
//}
//
//void test_Profiles(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    Profiles(id);
//    TEST_IGNORE_MESSAGE("NOP - No test needed");
//}
//
//void test_NA(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    NA(id);
//    TEST_IGNORE_MESSAGE("NOP - No test needed");
//}
//
//void test_UI_ParseRxCAN_Message(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    //UI_ParseRxCAN_Message(uint8_t *, uint8_t *, uint8_t);
//    TEST_IGNORE_MESSAGE("No function definition - UI_ParseRxCAN_Message()");
//}
//
//void test_UpdateScreen(void) {
//    //UpdateScreenID();
//    TEST_IGNORE_MESSAGE("No function definition - UpdateScreenID()");
//}
//
//// void test_hi_mode_button_mode_key_0 (void)
//// {
////    GRAPHIC_ID id = NULL_GRAPHIC_ID;
////    //current_therapy_settings.gui.mode_key = 0;
////    put_event_ExpectAndReturn(&evt_tbl,CHANGE_MODE,1);
////    current_therapy_settings.treatment.therapy_mode = LO;
////    hi_mode_button(id);
////    TEST_ASSERT_EQUAL(1,current_therapy_settings.gui.mode_key);
////    TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
//// }
//
//// void test_hi_mode_button_mode_key_1 (void)
//// {
////    GRAPHIC_ID id = NULL_GRAPHIC_ID;
////    //current_therapy_settings.gui.mode_key = 1;
////    current_therapy_settings.treatment.therapy_mode = LO;
////    hi_mode_button(id);
////    TEST_ASSERT_EQUAL(1,current_therapy_settings.gui.mode_key);
////    TEST_ASSERT_EQUAL(LO,current_therapy_settings.treatment.therapy_mode);
//// }
//
//// void test_med_mode_button_mode_key_0 (void)
//// {
////    GRAPHIC_ID id = NULL_GRAPHIC_ID;
////    current_therapy_settings.gui.mode_key = 0;
////    put_event_ExpectAndReturn(&evt_tbl,CHANGE_MODE,1);
////    current_therapy_settings.treatment.therapy_mode = LO;
////    med_mode_button(id);
////    TEST_ASSERT_EQUAL(1,current_therapy_settings.gui.mode_key);
////    TEST_ASSERT_EQUAL(MED,current_therapy_settings.treatment.therapy_mode);
//// }
//
//// void test_med_mode_button_mode_key_1 (void)
//// {
////    GRAPHIC_ID id = NULL_GRAPHIC_ID;
////    current_therapy_settings.gui.mode_key = 1;
////    current_therapy_settings.treatment.therapy_mode = LO;
////    med_mode_button(id);
////    TEST_ASSERT_EQUAL(1,current_therapy_settings.gui.mode_key);
////    TEST_ASSERT_EQUAL(LO,current_therapy_settings.treatment.therapy_mode);
//// }
//
//// void test_lo_mode_button_mode_key_0 (void)
//// {
////    GRAPHIC_ID id = NULL_GRAPHIC_ID;
////    current_therapy_settings.gui.mode_key = 0;
////    put_event_ExpectAndReturn(&evt_tbl,CHANGE_MODE,1);
////    current_therapy_settings.treatment.therapy_mode = HI;
////    lo_mode_button(id);
////    TEST_ASSERT_EQUAL(1,current_therapy_settings.gui.mode_key);
////    TEST_ASSERT_EQUAL(LO,current_therapy_settings.treatment.therapy_mode);
//// }
//
//// void test_lo_mode_button_mode_key_1 (void)
//// {
////    GRAPHIC_ID id = NULL_GRAPHIC_ID;
////    current_therapy_settings.gui.mode_key = 1;
////    current_therapy_settings.treatment.therapy_mode = HI;
////    lo_mode_button(id);
////    TEST_ASSERT_EQUAL(1,current_therapy_settings.gui.mode_key);
////    TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
////}
//
void test_update_gui(void) {
    current_therapy_settings.gui.screenID = 22; //ERROR_SCREEN
    current_therapy_settings.gui.graphics.GraphicNumber = 75; //OK_BUTTON;
    uint8_t return_value = update_gui();
}

void test_update_screen_shutdown_imminent(void) {
    current_therapy_settings.sw.shutdown_imminent = TRUE;
    uint8_t return_value = update_screen();
    TEST_ASSERT_EQUAL(0, current_therapy_settings.gui.screenID);
}

void test_update_screen_splash_screen(void) {
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = 0;
    // Call the function 4 times to get get past the splash screen counter
    uint8_t return_value;
    for (uint8_t i = 0 ; i < 4; i++)
    {
       return_value = update_screen();
    }
    TEST_ASSERT_EQUAL(SPLASH_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_fault_screen(void){
	current_therapy_settings.sw.shutdown_imminent = FALSE;
	current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	current_therapy_settings.notify.status = 0x80000;
	uint8_t return_value = update_screen();
	TEST_ASSERT_EQUAL(FAULT_SCREEN, current_therapy_settings.gui.screenID);
	
	current_therapy_settings.sw.shutdown_imminent = FALSE;
	current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	current_therapy_settings.notify.status2 = 0x100;
	return_value = update_screen();
	TEST_ASSERT_EQUAL(FAULT_SCREEN, current_therapy_settings.gui.screenID);
	current_therapy_settings.notify.status2 ^= 0x100;
}

void test_update_screen_error_screen1(void) {
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
    current_therapy_settings.notify.status = 0xC;
	cancel_new_profile_IgnoreAndReturn(0);
    uint8_t return_value = update_screen();
    TEST_ASSERT_EQUAL(ERROR_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_error_screen2(void) {
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
    current_therapy_settings.notify.status = 0xB;
	cancel_new_profile_IgnoreAndReturn(0);
    uint8_t return_value = update_screen();
    TEST_ASSERT_EQUAL(ERROR_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_error_screen3(void){
	current_therapy_settings.sw.shutdown_imminent = FALSE;
	current_therapy_settings.gui.screenID = ERROR_SCREEN;
	uint8_t testvalue = current_therapy_settings.gui.depth_idx;
	uint8_t returnvalue = update_screen();
	TEST_ASSERT_EQUAL(testvalue, current_therapy_settings.gui.depth_idx);
}

void test_update_screen_error_screen4(void){
	current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = ERROR_SCREEN;
    current_therapy_settings.notify.status = 0x19;
	uint32_t testvalue = current_therapy_settings.notify.status2;
	uint8_t return_value;
	for (uint8_t i = 0 ; i < 52; i++){
		update_screen();
	}
	TEST_ASSERT_EQUAL(testvalue & ~(CHECK_ELECTRODE_NOTIFICATION_MASK), current_therapy_settings.notify.status2);
}

void test_update_screen_firmware_update (void){
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.notify.status2 = 0x00;
   current_therapy_settings.sdhc.card_ready = TRUE;
   current_therapy_settings.wand.unlocked = 0;
   cancel_new_profile_IgnoreAndReturn(0);
   uint8_t return_value = update_screen();
   TEST_ASSERT_EQUAL(FIRMWARE_UPDATE_SCREEN, current_therapy_settings.gui.screenID);
   
   current_therapy_settings.wand.unlocked = TRUE;
   cancel_new_profile_IgnoreAndReturn(0);
   return_value = update_screen();
   current_therapy_settings.sdhc.card_ready = FALSE;
   current_therapy_settings.wand.unlocked = FALSE;
}

void test_update_screen_wireless_footpedal (void){
   current_therapy_settings.gui.screenID = WIRELESS_FOOTPEDAL_HANDSHAKE_SCREEN;
   uint8_t return_value;
   current_therapy_settings.footpedal.wireless.married = 1;
   current_therapy_settings.gui.depth_idx = 2;
   
   for (uint8_t i=0; i < 46;i++)
   {
	  cancel_new_profile_IgnoreAndReturn(0);
      return_value = update_screen();
   }
   current_therapy_settings.footpedal.wireless.married = 0;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 1;
   current_therapy_settings.gui.screenID = WIRELESS_FOOTPEDAL_HANDSHAKE_SCREEN;
   cancel_new_profile_IgnoreAndReturn(0);
   return_value = update_screen();
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.marriage_err = 1;
   put_event_ExpectAndReturn(&evt_tbl,WIRELESS_HANDSHAKE,1);
   for (uint8_t i=0; i < 16;i++)
   {
	   cancel_new_profile_IgnoreAndReturn(0);
      return_value = update_screen();
   }
}

void test_update_screen_wand_detection_screen(void) {
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
    current_therapy_settings.notify.status = 0x0;
    current_therapy_settings.wand.unlocked = FALSE;
    uint8_t return_value = update_screen();
    TEST_ASSERT_EQUAL(WAND_DETECTION_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_wand_detection_screen_10(void) {
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
    current_therapy_settings.notify.status = 0x0;
    current_therapy_settings.wand.unlocked = FALSE;
    uint8_t return_value;
    for (uint8_t i = 0; i < 11; i++) {
        return_value = update_screen();
    }
    TEST_ASSERT_EQUAL(WAND_DETECTION_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_connect_footpedal_help1 (void){
	uint8_t return_value;
	current_therapy_settings.controller = SPORTS_MED;
	current_therapy_settings.active_wand.smart_wand = UNKNOWN_GEN;
	current_therapy_settings.treatment.finger_switch_en = 0;
	current_therapy_settings.footpedal.wireless.present = 0;
	current_therapy_settings.footpedal.wired.present = 0;
	legacy_wand_params_obj.wand_connected = 1;
	cancel_new_profile_IgnoreAndReturn(0);
	return_value = update_screen();
	TEST_ASSERT_EQUAL(CONNECT_FOOTPEDAL_HELP_SCREEN, current_therapy_settings.gui.screenID);
   
	for (uint8_t i = 0; i < 20 ; i++)
	{
		cancel_new_profile_IgnoreAndReturn(0);
		return_value = update_screen();
	}
	TEST_ASSERT_EQUAL(UNKNOWN_GEN,current_therapy_settings.active_wand.smart_wand);
	TEST_ASSERT_EQUAL(SPORTS_MED,current_therapy_settings.controller);
	TEST_ASSERT_EQUAL(SWITCHING_SETTINGS_SCREEN, current_therapy_settings.gui.screenID);
	legacy_wand_params_obj.wand_connected = 0;
}

void test_update_screen_connect_footpedal_help2 (void){
	uint8_t return_value;
	current_therapy_settings.treatment.finger_switch_en = 0;
	current_therapy_settings.footpedal.wireless.present = 1;
	current_therapy_settings.footpedal.wired.present = 0;
	legacy_wand_params_obj.wand_connected = 1;
	return_value = update_screen();
	
	current_therapy_settings.treatment.finger_switch_en = 0;
	current_therapy_settings.footpedal.wireless.present = 0;
	current_therapy_settings.footpedal.wired.present = 1;
	return_value = update_screen();
	legacy_wand_params_obj.wand_connected = 0;
	
}

void test_update_screen_connect_footpedal_screen(void){
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
    current_therapy_settings.notify.status = 0x0;
    uint8_t return_value;
	
	legacy_wand_params_obj.wand_connected = 0;
	current_therapy_settings.wand.unlocked = FALSE;
	legacy_wand_params_obj.wand_909 = 1;
	current_therapy_settings.footpedal.wired.present = 0;
    current_therapy_settings.footpedal.wireless.present = 0;
	cancel_new_profile_IgnoreAndReturn(0);
	return_value = update_screen();
	
	legacy_wand_params_obj.wand_connected = 1;
	current_therapy_settings.wand.unlocked = FALSE;
	legacy_wand_params_obj.wand_909 = 1;
	current_therapy_settings.footpedal.wired.present = 1;
    current_therapy_settings.footpedal.wireless.present = 0;
	cancel_new_profile_IgnoreAndReturn(0);
	return_value = update_screen();
	
	legacy_wand_params_obj.wand_connected = 1;
	current_therapy_settings.wand.unlocked = FALSE;
	legacy_wand_params_obj.wand_909 = 1;
	current_therapy_settings.footpedal.wired.present = 0;
    current_therapy_settings.footpedal.wireless.present = 1;
	cancel_new_profile_IgnoreAndReturn(0);
	return_value = update_screen();	
	
    current_therapy_settings.footpedal.wired.present = 0;
    current_therapy_settings.footpedal.wireless.present = 0;
	legacy_wand_params_obj.wand_connected = 1;
    current_therapy_settings.wand.unlocked = FALSE;
    legacy_wand_params_obj.wand_909 = 1;
    current_therapy_settings.treatment.finger_switch_en = 1;
	cancel_new_profile_IgnoreAndReturn(0);
    return_value = update_screen();
    TEST_ASSERT_EQUAL(CONNECT_FOOTPEDAL_HELP_SCREEN, current_therapy_settings.gui.screenID);
	for (uint8_t i = 0 ; i < 20; i++)
	{
		cancel_new_profile_IgnoreAndReturn(0);
		return_value = update_screen();
	}
	TEST_ASSERT_EQUAL(SWITCHING_SETTINGS_SCREEN, current_therapy_settings.gui.screenID);
}

void test_LEGACY_6_8_WAND_TREATMENT_SCREEN (void){ 
   legacy_wand_params_obj.wand_909 = 0;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.controller = SPORTS_MED;
   current_therapy_settings.gui.screenID = CONNECT_FOOTPEDAL_HELP_SCREEN;
   uint8_t return_value;
   cancel_new_profile_IgnoreAndReturn(0);
   return_value = update_screen();
   TEST_ASSERT_EQUAL(LEGACY_6_8_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.screenID);
   
   current_therapy_settings.gui.screenID = WAND_INFO_SCREEN;
   return_value = update_screen();
   TEST_ASSERT_EQUAL(LEGACY_6_8_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.screenID);
   
   current_therapy_settings.controller = ENT;
   return_value = update_screen();
   current_therapy_settings.active_wand.lw_active = 0;
}

void test_update_screen_tube_absent_screen(void) {
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
    current_therapy_settings.notify.status = 0x0;
    current_therapy_settings.wand.unlocked = TRUE;
    current_therapy_settings.pump.status |= TUBE_ABSENT;
    uint8_t return_value = update_screen();
    TEST_ASSERT_EQUAL(INSERT_TUBE_HELP_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_CLOSE_PUMP_DOOR_HELP_SCREEN(void) {
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
    current_therapy_settings.notify.status = 0x0;
    current_therapy_settings.wand.unlocked = TRUE;
    current_therapy_settings.pump.status &= ~TUBE_ABSENT;
    current_therapy_settings.pump.status |= DOOR_OPEN;
    uint8_t return_value = update_screen();
    TEST_ASSERT_EQUAL(CLOSE_PUMP_DOOR_HELP_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_OPEN_PUMP_DOOR_HELP_SCREEN(void) {
    current_therapy_settings.sw.shutdown_imminent = FALSE;
    current_therapy_settings.gui.screenID = SPLASH_SCREEN;
    current_therapy_settings.notify.status = 0x0;
    current_therapy_settings.wand.unlocked = TRUE;
    current_therapy_settings.pump.status &= ~TUBE_ABSENT;
    current_therapy_settings.pump.status &= ~DOOR_OPEN;
    current_therapy_settings.pump.status |= DOOR_CLOSE;
    uint8_t return_value = update_screen();
    TEST_ASSERT_EQUAL(OPEN_PUMP_DOOR_HELP_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_RF_2000_WAND_TREATMENT_SCREEN(void) {
   get_ww_temp_IgnoreAndReturn(1);
   thermometer_color_IgnoreAndReturn(1);
   current_therapy_settings.sw.shutdown_imminent = FALSE;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.notify.status = 0x0;
   current_therapy_settings.wand.unlocked = TRUE;
   current_therapy_settings.pump.status &= ~TUBE_ABSENT;
   current_therapy_settings.pump.status &= ~DOOR_OPEN;
   current_therapy_settings.pump.status &= ~DOOR_CLOSE;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.controller = SPORTS_MED;
   current_therapy_settings.active_wand.lw_active = 0;
   uint8_t return_value = update_screen();
   TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.screenID);
}

void test_update_screen_MANTA_SMART_WAND_TREATMENT_SCREEN(void) {
   get_ww_temp_IgnoreAndReturn(1);
   thermometer_color_IgnoreAndReturn(1);
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.controller = SPORTS_MED;
   smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
   current_therapy_settings.sw.shutdown_imminent = FALSE;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.notify.status = 0x0;
   current_therapy_settings.wand.unlocked = TRUE;
   current_therapy_settings.pump.status &= ~TUBE_ABSENT;
   current_therapy_settings.pump.status &= ~DOOR_OPEN;
   current_therapy_settings.pump.status &= ~DOOR_CLOSE;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.controller = SPORTS_MED;
   current_therapy_settings.active_wand.lw_active = 0;
   uint8_t return_value = update_screen();
   TEST_ASSERT_EQUAL(MANTA_SMART_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.screenID);
   
   current_therapy_settings.controller = ENT;
   smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_SPORTS_MED;
   return_value = update_screen();
   TEST_ASSERT_EQUAL(ENT_SMART_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.screenID);
   
   current_therapy_settings.controller = 3;
   return_value = update_screen();
}

void test_update_screen_legacy_active(void) 
{
	current_therapy_settings.active_wand.lw_active = TRUE;
	uint8_t return_value = update_screen();
	
	current_therapy_settings.gui.screenID = WAND_INFO_SCREEN;
	current_therapy_settings.controller = ENT;
	return_value = update_screen();
	TEST_ASSERT_EQUAL(LEGACY_ENT_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.screenID);
	
	current_therapy_settings.gui.screenID = SPLASH_SCREEN;
	return_value = update_screen();
	TEST_ASSERT_EQUAL(LEGACY_ENT_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.screenID);
	
	current_therapy_settings.active_wand.lw_active = FALSE;
}

void test_update_screen_legacy_inactive(void) 
{
	//db.mfg.data[45] = 0;
	cancel_new_profile_IgnoreAndReturn(0);
	legacy_wand_params_obj.wand_connected = 0;
	smart_wand_params.mfg.finger_switch_enable = 0;
	current_therapy_settings.controller = ENT;
	current_therapy_settings.footpedal.wired.present = 0;
	uint8_t return_value = update_screen();
	
	for (uint8_t i = 0 ; i < 20; i++)
	{
		cancel_new_profile_IgnoreAndReturn(0);
		return_value = update_screen();
	}
	
	current_therapy_settings.footpedal.wired.present = 1;
	
}

void test_load_screen01(void) {
    current_therapy_settings.gui.screenID = MAX_NUM_SCREENS;
    uint8_t return_value = load_screen01();
    TEST_ASSERT_EQUAL(SPLASH_SCREEN, current_therapy_settings.gui.screenID);
}

//void test_levels_access(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    levels_access(id);
//}
//
//void test_thermometer(void) {
//    GRAPHIC_ID id = NULL_GRAPHIC_ID;
//    put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
//    thermometer(id);
//}
//

void test_hn(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.hi = ABLATE_PLUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	hn(value);
	TEST_ASSERT_EQUAL(ABLATE_MINUS, current_therapy_settings.setpoint.hi);
}

void test_hd(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.hi = ABLATE_PLUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	hd(value);
	TEST_ASSERT_EQUAL(ABLATE_DEFAULT, current_therapy_settings.setpoint.hi);
}

void test_hp(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.hi = ABLATE_MINUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	hp(value);
	TEST_ASSERT_EQUAL(ABLATE_PLUS, current_therapy_settings.setpoint.hi);
}
//
void test_mn(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.med = ABLATE_PLUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	mn(value);
	TEST_ASSERT_EQUAL(ABLATE_MINUS, current_therapy_settings.setpoint.med);
}

void test_md(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.med = ABLATE_PLUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	md(value);
	TEST_ASSERT_EQUAL(ABLATE_DEFAULT, current_therapy_settings.setpoint.med);
}

void test_mp(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.med = ABLATE_MINUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	mp(value);
	TEST_ASSERT_EQUAL(ABLATE_PLUS, current_therapy_settings.setpoint.med);
}
//
void test_ln(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.lo = ABLATE_PLUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	ln(value);
	TEST_ASSERT_EQUAL(ABLATE_MINUS, current_therapy_settings.setpoint.lo);
}

void test_ld(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.lo = ABLATE_PLUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	ld(value);
	TEST_ASSERT_EQUAL(ABLATE_DEFAULT, current_therapy_settings.setpoint.lo);
}

void test_lp(void)
{
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.setpoint.lo = ABLATE_MINUS;
	switch_ablate_parameters_IgnoreAndReturn(1);
	lp(value);
	TEST_ASSERT_EQUAL(ABLATE_PLUS, current_therapy_settings.setpoint.lo);
}

void test_hi_mode_button(void) 
{
	uint8_t value = 0;
    current_therapy_settings.pump.status |= PUMP_READY;
    switch_ablate_parameters_IgnoreAndReturn(1);
    play_change_mode_audio_ExpectAndReturn(1);
    hm(value);
    TEST_ASSERT_EQUAL(IDLE_SPEED, pump.state);
}

void test_hi_mode_button_1(void)
 {
    uint8_t value = 0;
    pump.state = PROTOCOL;
    current_therapy_settings.pump.status &= ~PUMP_READY;
    switch_ablate_parameters_IgnoreAndReturn(1);
    play_change_mode_audio_ExpectAndReturn(1);
    hm(value);
    TEST_ASSERT_EQUAL(PROTOCOL, pump.state);
}

void test_lo_mode_button(void)
{
    uint8_t id = 0;
    current_therapy_settings.pump.status |= PUMP_READY;
    switch_ablate_parameters_IgnoreAndReturn(1);
    play_change_mode_audio_ExpectAndReturn(1);
    lm(id);
    TEST_ASSERT_EQUAL(IDLE_SPEED, pump.state);
}

void test_lo_mode_button_1(void)
{
    uint8_t id = 0;
    pump.state = PROTOCOL;
    current_therapy_settings.pump.status &= ~PUMP_READY;
    switch_ablate_parameters_IgnoreAndReturn(1);
    play_change_mode_audio_ExpectAndReturn(1);
    lm(id);
    TEST_ASSERT_EQUAL(PROTOCOL, pump.state);
}

void test_med_mode_button(void)
{
    uint8_t value = 0;
    current_therapy_settings.pump.status |= PUMP_READY;
    switch_ablate_parameters_IgnoreAndReturn(1);
    play_change_mode_audio_ExpectAndReturn(1);
    mm(value);
    TEST_ASSERT_EQUAL(IDLE_SPEED, pump.state);
}

void test_med_mode_button_1(void)
{
     uint8_t value = 0;
    pump.state = PROTOCOL;
    current_therapy_settings.pump.status &= ~PUMP_READY;
    switch_ablate_parameters_IgnoreAndReturn(1);
    play_change_mode_audio_ExpectAndReturn(1);
    mm(value);
    TEST_ASSERT_EQUAL(PROTOCOL, pump.state);
}

void test_ba (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.gui.depth_idx = 1;
   ba(0);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.gui.depth_idx);
}

void test_ba1 (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.gui.depth_idx = 1;
   current_therapy_settings.gui.previous_screenID[current_therapy_settings.gui.depth_idx-1] = PROFILE_SAVE_SCREEN;
   ba(0);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.gui.depth_idx);
}

void test_ba2 (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.gui.depth_idx = 2;
   current_therapy_settings.gui.previous_screenID[current_therapy_settings.gui.depth_idx-1] = 0;
   ba(0);
   TEST_ASSERT_EQUAL(current_therapy_settings.gui.screenID, WAND_DETECTION_SCREEN);
}

void test_ba3 (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.gui.depth_idx = 2;
   current_therapy_settings.gui.previous_screenID[current_therapy_settings.gui.depth_idx] = 0;
   ba(0);
   TEST_ASSERT_EQUAL(current_therapy_settings.gui.screenID, WAND_DETECTION_SCREEN);
}


void test_ba4 (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   
   //put_event_ExpectAndReturn(&evt_tbl,CANCEL_SW_UPDATE,1);
   current_therapy_settings.gui.depth_idx = 2;
   current_therapy_settings.gui.previous_screenID[current_therapy_settings.gui.depth_idx] = 0;
   current_therapy_settings.sdhc.card_ready = 1;
   ba(0);
   TEST_ASSERT_EQUAL(current_therapy_settings.gui.screenID, WAND_DETECTION_SCREEN);
}

void test_kb (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.user_data.profile[1].record_full = TRUE;
   kb(8);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
   TEST_IGNORE_MESSAGE("Loop starts at 1, is it okay?");
}

void test_rc (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.user_data.profile[200].record_full = TRUE;
   current_therapy_settings.sdhc.card_ready = 0;
   uint8_t value = 0;
   load_profile_ExpectAndReturn(value,0);
   rc(8);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
   //TEST_IGNORE_MESSAGE("Is calling rc going to sound the click 2 times");
}

void test_ap (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t value = 10;
   load_profile_ExpectAndReturn(value,1);
   ap(value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}


void test_ap1 (void)
{
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t value = 10;
   load_profile_ExpectAndReturn(value,0);
   ap(value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_df(void)
{
   uint8_t value = 10;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   legacy_wand_params_obj.wand_connected = 0;
   current_therapy_settings.wand.unlocked = 1;
   df(value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_df1(void)
{
   uint8_t value = 10;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 1;
   df(value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_dp (void)
{
   uint8_t value = 10;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   dp(0);
}

void test_dp1 (void)
{
   uint8_t value = 10;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   load_profile_ExpectAndReturn(0,1);
   dp(2);
}

void test_sv (void)
{
   uint8_t value = 10;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.user_data.settings.active_idx = 1;
   save_profile_ExpectAndReturn(1,1);
   sv (value);
}

void test_sv1 (void)
{
   uint8_t value = 10;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.user_data.settings.active_idx = 0;
   sv (value);
}

void test_leap (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   legacy_ablate_increment_ExpectAndReturn(1);
   leap (value);
}

void test_leam (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   legacy_ablate_decrement_ExpectAndReturn(1);
   leam (value);
}

void test_lecp (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   legacy_coag_increment_ExpectAndReturn(1);
   lecp (value);
}

void test_lecm (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   legacy_coag_decrement_ExpectAndReturn(1);
   lecm (value);
}

void test_LeR (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,RESET_ERROR,1);
   LeR (value);
}

void test_NA (void)
{
   uint8_t value = 0;
   NA (value);
}

void test_cd (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   cd( value);
}

void test_dl (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   dl (value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_ef (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   ef (value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_in (void)
{
   uint8_t value;
   in (value);
   // no test possible
}

void test_ls (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   ls (value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_of (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   put_event_ExpectAndReturn(&evt_tbl,COMMENCE_UIS_SW_UPDATE,1);
   of(value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_ps (void)
{
   uint8_t value = 0;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   ps (value);
   TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID,current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_sn_1 (void)
{
   uint8_t value = 0;
   //put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1); // one in ba
   current_therapy_settings.user_data.settings.active_idx = 0;
   current_therapy_settings.gui.depth_idx = 1;
   sn(1);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.gui.depth_idx);
}

void test_sn_2 (void)
{
   uint8_t value = 0;
   //put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1); // one in ba
   save_profile_IgnoreAndReturn(1);
   current_therapy_settings.user_data.settings.active_idx = 1;
   current_therapy_settings.gui.depth_idx = 1;
   sn(1);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.gui.depth_idx);
}

void test_cu1 (void)
{
	current_therapy_settings.sw.update_progress = 0;
	put_event_ExpectAndReturn(&evt_tbl,CANCEL_SW_UPDATE,1);
	cu(1);
	TEST_ASSERT_EQUAL(WAND_DETECTION_SCREEN, current_therapy_settings.gui.screenID);
	
}

void test_cu2 (void)
{
	current_therapy_settings.sw.update_progress = 1U;
	cu(1);
	TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID, current_therapy_settings.gui.graphics.GraphicNumber);
}
	
void test_ce1 (void)
{
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.ablate_enable = TRUE;
	ce(1);
	TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.ablate_enable);

	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.ablate_enable = FALSE;
	ce(1);
	TEST_ASSERT_EQUAL(TRUE, current_therapy_settings.treatment.ablate_enable);
}

void test_fdl (void)
{
	uint8_t testvalue1 = 10;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.flow_rate = testvalue1;
	fdl(1);
	TEST_ASSERT_EQUAL(testvalue1 - 1, current_therapy_settings.treatment.flow_rate);
	TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID, current_therapy_settings.gui.graphics.GraphicNumber);
	
	uint8_t testvalue2 = 1;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.flow_rate = testvalue2;
	fdl(1);
	TEST_ASSERT_EQUAL(testvalue2, current_therapy_settings.treatment.flow_rate);
	TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID, current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_fil(void)
{
	uint8_t testvalue1 = 5;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.flow_rate = testvalue1; // if statement will not be taken
	fil(1);
	TEST_ASSERT_EQUAL(testvalue1, current_therapy_settings.treatment.flow_rate);
	TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID, current_therapy_settings.gui.graphics.GraphicNumber);
	
	uint8_t testvalue2 = 1;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	current_therapy_settings.treatment.flow_rate = testvalue2; // if statement will be taken
	fil(1);
	TEST_ASSERT_EQUAL(testvalue2 + 1, current_therapy_settings.treatment.flow_rate);
	TEST_ASSERT_EQUAL(NULL_GRAPHIC_ID, current_therapy_settings.gui.graphics.GraphicNumber);
}

void test_pa(void){
	uint8_t value = 0;
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	//fs_any_button_pressed_ExpectAndReturn(1);
	put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON, 1);
	AutoPrimeId = 1U;
	pa(value);
	TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
	
	value = 1;
	//fs_any_button_pressed_ExpectAndReturn(0);
	gui_clear_auto_prime_event();
	put_event_ExpectAndReturn(&evt_tbl, THERAPY_ON, 1);
	pa(value);
	TEST_ASSERT_EQUAL(PRIME_AUTO, PrimeState);
}

void test_gui_stop_manual_prime (void){
	lw_get_pump_speed_IgnoreAndReturn(1);
	pump_set_speed_Expect(1);
	current_therapy_settings.gui.graphics.GraphicNumber = IV_BAG_NOT_AUTO;
	//prime.manual_mode = FALSE;
	//prime.auto_mode = FALSE;
	current_therapy_settings.treatment.pre_prime = TRUE;
	gui_stop_manual_prime();
	TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
	
	current_therapy_settings.gui.graphics.GraphicNumber = IV_BAG_AUTO;
	current_therapy_settings.pump.door_present = TRUE;
	lw_get_pump_speed_IgnoreAndReturn(1);
	pump_set_speed_Expect(1);
	gui_stop_manual_prime();
	
	current_therapy_settings.gui.graphics.GraphicNumber = IV_BAG_NOT_AUTO;
	//prime.suction_tube_err = TRUE;
	//prime.manual_mode = TRUE;
	lw_get_pump_speed_IgnoreAndReturn(1);
	pump_set_speed_Expect(1);
	gui_stop_manual_prime();
	
	//prime.suction_tube_err = FALSE;
	
} 


void test_get_priming_status(void){
	PrimeState = PRIME_OFF;
	TEST_ASSERT_EQUAL(PRIME_OFF, get_priming_status());
} 

void test_gui_save_flow_status(void){
	FlagStatus value = RESET;
	lw_get_pump_speed_IgnoreAndReturn(1);
	pump_set_speed_Expect(1);
	gui_save_flow_status(value);
	TEST_ASSERT_EQUAL(DEFAULT_FLOW_RATE, current_therapy_settings.treatment.flow_rate);
	
	value = SET;
	lw_get_pump_speed_IgnoreAndReturn(1);
	pump_set_speed_Expect(1);
	gui_save_flow_status(value);
	TEST_ASSERT_EQUAL(5, current_therapy_settings.treatment.flow_rate);
}

void test_m_prime(void){
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON, 1);
	float value = 0;
	PrimeId = 0U;
	AutoPrimeId = 0U;
	uint8_t return_value = m_prime(value);
	TEST_ASSERT_EQUAL(value, current_therapy_settings.pump.idle_rps);
	
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	PrimeId = 1U;
	return_value = m_prime(value);
}

void test_pm(void){
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON, 1);
	PrimeId = 0U;
	AutoPrimeId = 0U;
	pm(0);
	TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
}

void test_pml(void){
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON, 1);
	PrimeId = 0U;
	AutoPrimeId = 0U;
	lw_get_pump_speed_IgnoreAndReturn(0);
	pml(0);
	TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
}

void test_mute_timer(void){
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	legacy_wand_params_obj.timer.audio_muted = TRUE;
	mute_timer(0);
	TEST_ASSERT_EQUAL(FALSE, legacy_wand_params_obj.timer.audio_muted);
	
	put_event_ExpectAndReturn(&evt_tbl, CLICK_SOUND, 1);
	legacy_wand_params_obj.timer.audio_muted = FALSE;
	mute_timer(0);
	TEST_ASSERT_EQUAL(TRUE, legacy_wand_params_obj.timer.audio_muted);
}

void test_gui_stop_auto_prime(void)
{
	put_event_ExpectAndReturn(&evt_tbl, THERAPY_OFF, 1);
	gui_stop_auto_prime();
	TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
}

void test_rel(void)
{	
	current_therapy_settings.wand.valid = FALSE;
	rel();
}
void test_rel1(void)
{
	uint8_t Speed = 1;
	current_therapy_settings.wand.valid = TRUE;
	pump_set_speed_Expect(Speed);
	lw_get_pump_speed_IgnoreAndReturn(1); //called by pump_set_speed
	rel();
}
void test_rel2(void)
{
	PrimeId = 0;
	PrimeState = PRIME_AUTO;
	put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
	rel();
}
	void test_rel3(void)
{
	uint8_t Speed = 1;
	PrimeId = 0;
	PrimeState = PRIME_OFF;
	pump_set_speed_Expect(Speed);
	lw_get_pump_speed_IgnoreAndReturn(1); //called by pump_set_speed
	rel();
}
void test_rel4(void)
{
	PrimeState = PRIME_OFF;
	AutoPrimeId = 1;
	put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
	put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
	rel();
}

void test_mfd(void)
{
	put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND, 1);
	mfd();
}

void test_mfi(void)
{
	put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND, 1);
	mfi();
}

void test_fd_invalid(void)
{
	current_therapy_settings.wand.valid = TRUE;
	current_therapy_settings.active_wand.smart_wand = SPORTS_MED;
	fd(0);
}

void test_fi_invalid(void)
{
	fi(0);
	current_therapy_settings.active_wand.smart_wand = ARTHROPLASTY;
}

void test_scp(void)
{
	put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND, 1); //This is an issue
	
	put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND, 1);
	scp();
}

void test_scm (void)
{
	put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND, 1); //This is an issue
	
	put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND, 1);
	scm();
}

void test_autoprime_timeout (void)
{
	autoprime_timeout();
}

void test_autoprime_lock_timeout(void)
{
	autoprime_lock_timeout();
}

void test_change_flow_manta (void)
{
	uint8_t coag_level;
	uint8_t new_flow;
	
	change_flow_manta(coag_level, new_flow);
}

// void test_gui_stop_auto_prime_rf_detect(void)
// {
	// prime.ap_rf_detect = TRUE;
	// put_event_ExpectAndReturn(&evt_tbl, THERAPY_OFF, 1);
	// gui_stop_auto_prime();
	// TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
// }

// void test_gui_stop_auto_prime_suction_tube(void)
// {
	// prime.ap_rf_detect = FALSE;
	// prime.suction_tube_err = TRUE;
	// put_event_ExpectAndReturn(&evt_tbl, THERAPY_OFF, 1);
	// gui_stop_auto_prime();
	// TEST_ASSERT_EQUAL(FALSE, current_therapy_settings.treatment.flow_auto);
// }

void test_dummy(void) {
#if _BullseyeCoverage
    cov_dumpData();
#endif
}







































