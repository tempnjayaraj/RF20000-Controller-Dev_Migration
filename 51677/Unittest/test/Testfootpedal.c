#include "unity.h"
#include "common.h"
#include "MK26F18.h"
#include "Mocksystem.h"
#include "footpedal.h"
#include "cpu.h"
#include "Mockevent_buffer.h"
#include "MockLegacy_wand.h"
#include "Mockwand.h"
#include "Mockgui.h"

GEN_SETTINGS current_therapy_settings;
SMART_WAND_PARAMETERS smart_wand_params;
GEN_SETTINGS *const pcurrent_therapy_settings = &current_therapy_settings;
//SMART_WAND_PARAMETERS *const pSmartWandParams = &smart_wand_params;
event_table evt_tbl;
legacy_wand_params legacy_wand_params_obj;
manta_prime prime;
prime_mode_t PrimeState;
SMART_WAND_DATABASE db;

void setUp(void)
{
   GPIOB_PDIR = 0x00;
   
}

void tearDown(void)
{
}


void test_FootPedal_Init()
{
   uint8_t return_value = foot_pedal_init();
   SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
   TEST_ASSERT_EQUAL(SIM_SCGC5_PORTB_MASK,(SIM_SCGC5&SIM_SCGC5_PORTB_MASK));
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_poll_footpedal_not_present(void)
{
   current_therapy_settings.footpedal.present = 0;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t  return_value = poll_footpedal();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_footpedal_case1 (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t  return_value = poll_footpedal();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.footpedal.present);
   current_therapy_settings.footpedal.wired.present =1;
   current_therapy_settings.treatment.coag_on = 1;
}

void test_footpedal_case_2 (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t  return_value = poll_footpedal();
}

void test_footpedal_case_3 (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t  return_value = poll_footpedal();
}

void test_footpedal_case_4 (void)
{
   legacy_wand_params_obj.wand_connected = 1;
   current_therapy_settings.wand.unlocked = 1;
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 1;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t  return_value = poll_footpedal();
}

void test_footpedal_case_5 (void)
{
	smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
	current_therapy_settings.active_wand.smart_wand = ARTHROPLASTY;
	current_therapy_settings.gui.screenID = MANTA_SMART_WAND_TREATMENT_SCREEN;
   legacy_wand_params_obj.wand_connected = 0;
   current_therapy_settings.footpedal.present = 0;
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.married_latch = 1;
   current_therapy_settings.treatment.wireless_enable = 1;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t  return_value = poll_footpedal();
}

void test_footpedal_case_6 (void)
{
   legacy_wand_params_obj.wand_connected = 0;
   current_therapy_settings.footpedal.present = 0;
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.married_latch = 1;
   current_therapy_settings.treatment.wireless_enable = 0;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t  return_value = poll_footpedal();
}

void test_footpedal_case_7 (void)
{
   legacy_wand_params_obj.wand_connected = 0;
   current_therapy_settings.footpedal.present = 0;
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 1;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t  return_value = poll_footpedal();
}


void test_poll_footpedal_present(void)
{
   current_therapy_settings.treatment.vac_enabled = 0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.footpedal.ablate = 1;
   put_event_ExpectAndReturn (&evt_tbl,FOOT_SWITCH_STUCK,1);

   current_therapy_settings.footpedal.present = 1;
   uint8_t  return_value = check_fs_stuck();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_poll_footpedal_mode_zero (void)
{
	smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_ENT;
   GPIOB_PDIR &= ~(1 << MODE_SHIFT);
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.footpedal.mode = 0;
   legacy_wand_params_obj.wand_connected = 0;
   smart_wand_params.mfg.low_enable = 1;
   
   // Set this to zero, so the control wont get into the
   // Vac!=current_therapy_settings.footpedal.vac_key control block

   for (uint8_t i = 0; i < 3; i++)
   {
	   get_priming_status_IgnoreAndReturn(0);
      uint8_t  return_value = poll_footpedal();
   }
   GPIOB_PDIR |= 1 << MODE_SHIFT;
   current_therapy_settings.footpedal.vac_key = 1;

   put_event_ExpectAndReturn (&evt_tbl,CHANGE_MODE,1);
   get_priming_status_IgnoreAndReturn(0);
   uint8_t return_value = poll_footpedal();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.footpedal.mode);
}

void test_poll_footpedal_mode_cnt_greater_than_75_vac_on(void)
{
   GPIOB_PDIR &= ~(1 << MODE_SHIFT);
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.treatment.vac_enabled = 0;
   for (uint8_t i = 0; i < 77; i++)
   {
	  get_priming_status_IgnoreAndReturn(0);
      uint8_t  return_value = poll_footpedal();
   }
   TEST_ASSERT_EQUAL(1,current_therapy_settings.footpedal.mode);
}

void test_poll_footpedal_mode_cnt_less_than_75_on(void)
{
   GPIOB_PDIR &= ~(1 << MODE_SHIFT);
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.treatment.vac_enabled = 0;
   for (uint8_t i = 0; i < 70; i++)
   {
	  get_priming_status_IgnoreAndReturn(0);
      uint8_t  return_value = poll_footpedal();
   }
   TEST_ASSERT_EQUAL(1,current_therapy_settings.footpedal.mode);
   
   GPIOB_PDIR = 0;
   get_priming_status_IgnoreAndReturn(0);
   uint8_t return_value = poll_footpedal();
   
}


void test_poll_footpedal_ablate_on (void)
{
	
   GPIOB_PDIR &= ~(1 << ABLATE_SHIFT);
   current_therapy_settings.footpedal.ablate = 0;
   put_event_ExpectAndReturn (&evt_tbl,THERAPY_ON,1);
   get_priming_status_IgnoreAndReturn(0);
   uint8_t return_value = poll_footpedal();
}

void test_poll_footpedal_ablate_off (void)
{
   current_therapy_settings.footpedal.present = 1;
   GPIOB_PDIR |= (1 << ABLATE_SHIFT);
   current_therapy_settings.footpedal.ablate = 1;
   put_event_ExpectAndReturn (&evt_tbl,THERAPY_OFF,1);
   get_priming_status_IgnoreAndReturn(0);
   uint8_t return_value = poll_footpedal();
}

void test_poll_footpedal_coag_on (void)
{
   current_therapy_settings.footpedal.ablate = 1;
   current_therapy_settings.footpedal.present = 1;
   GPIOB_PDIR &= ~(1 << COAG_SHIFT);
   current_therapy_settings.footpedal.coag = 0;
   smart_wand_params.mfg.coag_enable = 1;
   
   uint8_t return_value;
   for (uint8_t i = 0; i < 28; i++)
   {
	   get_priming_status_IgnoreAndReturn(0);
      return_value = poll_footpedal();
   }
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   current_therapy_settings.footpedal.coag = 0;
   get_priming_status_IgnoreAndReturn(0);
   return_value = poll_footpedal();
}


void test_poll_footpedal_coag_on_manta_preprime (void)
{
   GPIOB_PDIR = 0;
   smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
   //db.mfg.data[46] = 2;
   current_therapy_settings.treatment.pre_prime = TRUE;
   PrimeState = PRIME_AUTO;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.present = 1;
   GPIOB_PDIR &= ~(1 << COAG_SHIFT);
   current_therapy_settings.footpedal.coag = 1;
   smart_wand_params.mfg.coag_enable = 1;
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   uint8_t return_value;
   for (uint8_t i = 0; i < 1; i++)
   {
	  get_priming_status_IgnoreAndReturn(0);
	  
      return_value = poll_footpedal();
   }
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   current_therapy_settings.footpedal.coag = 0;
   get_priming_status_ExpectAndReturn(1);
   gui_stop_auto_prime_Ignore();
   return_value = poll_footpedal();
}

void test_poll_footpedal_coag_on_manta_postprime (void)
{
   //smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
   current_therapy_settings.treatment.pre_prime = FALSE;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.present = 1;
   GPIOB_PDIR &= ~(1 << COAG_SHIFT);
   current_therapy_settings.footpedal.coag = 1;
   smart_wand_params.mfg.coag_enable = 1;
   
   uint8_t return_value;
   // for (uint8_t i = 0; i < 28; i++)
   // {
	  // get_priming_status_IgnoreAndReturn(0);
      // return_value = poll_footpedal();
   // }
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   current_therapy_settings.footpedal.coag = 0;
   get_priming_status_ExpectAndReturn(0);
   //gui_stop_auto_prime_Ignore();
   return_value = poll_footpedal();
}

void test_poll_footpedal_coag_off (void)
{
   current_therapy_settings.footpedal.ablate = 1;
   current_therapy_settings.footpedal.present = 1;
   GPIOB_PDIR |= (1 << COAG_SHIFT);
   current_therapy_settings.footpedal.coag = 1;
   put_event_ExpectAndReturn (&evt_tbl,THERAPY_OFF,1);
   get_priming_status_IgnoreAndReturn(0);
   uint8_t return_value = poll_footpedal();
}

void test_poll_footpedal_vac_off (void)
{
   current_therapy_settings.footpedal.ablate = 1;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.footpedal.present = 1;
   // Mode must be zero
   GPIOB_PDIR |= 1 << MODE_SHIFT;
   // Mode count must be less than 50
   // Just calling the function with mode = 0, makes mode count = 0
   current_therapy_settings.footpedal.vac_key = 1;
   current_therapy_settings.treatment.vac_enabled = 1;
   put_event_ExpectAndReturn (&evt_tbl,THERAPY_OFF,1);
   get_priming_status_IgnoreAndReturn(0);
   uint8_t return_value = poll_footpedal();
}

void test_poll_footpedal_mode_cnt_greater_than_75_vac_on_2(void)
{
	smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_ENT;
   GPIOB_PDIR &= ~(1 << MODE_SHIFT);
   current_therapy_settings.footpedal.present = 1;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.treatment.vac_enabled = 1;
   put_event_ExpectAndReturn (&evt_tbl,THERAPY_ON,1);
   smart_wand_params.mfg.vac_enable = 1;
   
   for (uint8_t i = 0; i < 77; i++)
   {
	   get_priming_status_IgnoreAndReturn(0);
      uint8_t  return_value = poll_footpedal();
   }
   //TEST_ASSERT_EQUAL(0,current_therapy_settings.footpedal.mode);

   //TEST_ASSERT_EQUAL(THERAPY_ON,get_event(&evt_tbl));
   
}

void test_poll_footpedal_wireless_1 (void)
{

   current_therapy_settings.footpedal.wired.present = 0;
   legacy_wand_params_obj.wand_connected = 0;
   
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marriage_err = 0;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.married = 1;
   current_therapy_settings.treatment.wireless_enable = 1;
   current_therapy_settings.footpedal.wireless.batt_low_count = 55;
   uint8_t  return_value;
   for (uint8_t i = 0 ; i < 95; i++)
   {
      current_therapy_settings.footpedal.ablate = 0;
      current_therapy_settings.footpedal.coag = 0;
      current_therapy_settings.footpedal.vac_key = 0;
      current_therapy_settings.active_wand.lw_active = 1;
      current_therapy_settings.footpedal.wired.present = 0;
      put_event_IgnoreAndReturn(1);
      put_event_IgnoreAndReturn(1);
	  get_priming_status_IgnoreAndReturn(0);
      return_value = poll_footpedal();
      current_therapy_settings.footpedal.wireless.present = 0;
   }
   current_therapy_settings.footpedal.wireless.married2station1 = 0;
   smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
}


void test_poll_footpedal_wireless_2 (void)
{
   current_therapy_settings.footpedal.wired.present = 0;
   legacy_wand_params_obj.wand_connected = 0;
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marriage_err = 0;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.married = 1;
   current_therapy_settings.treatment.wireless_enable = 1;
   current_therapy_settings.footpedal.wireless.batt_low_count = 35;
   uint8_t  return_value;
   for (uint8_t i = 0 ; i < 255; i++)
   {
      put_event_IgnoreAndReturn(1);
	  get_priming_status_IgnoreAndReturn(0);
      return_value = poll_footpedal();
      current_therapy_settings.footpedal.wireless.present = 0;
   }
   current_therapy_settings.footpedal.wireless.married2station1 = 0;
   current_therapy_settings.active_wand.lw_active = 0;
}

void test_poll_footpedal_wireless_3 (void)
{

   current_therapy_settings.footpedal.wired.present = 0;
   legacy_wand_params_obj.wand_connected = 0;
   
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marriage_err = 0;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.married = 1;
   current_therapy_settings.treatment.wireless_enable = 1;
   current_therapy_settings.footpedal.wireless.batt_low_count = 55;
   uint8_t  return_value;
   for (uint8_t i = 0 ; i < 255; i++)
   {
      current_therapy_settings.footpedal.ablate = 0;
      current_therapy_settings.footpedal.coag = 0;
      current_therapy_settings.footpedal.vac_key = 0;
      current_therapy_settings.active_wand.lw_active = 1;
      current_therapy_settings.footpedal.wired.present = 0;
      put_event_IgnoreAndReturn(1);
      put_event_IgnoreAndReturn(1);
	  get_priming_status_IgnoreAndReturn(0);
      return_value = poll_footpedal();
      current_therapy_settings.footpedal.wireless.present = 0;
   }
}

void test_poll_footpedal_lw_ablate_OFF (void)
{

   current_therapy_settings.footpedal.wired.present = 0;
   legacy_wand_params_obj.wand_connected = 0;

   uint8_t  return_value;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.ablate = 0x01;
   current_therapy_settings.footpedal.coag = 0x00;
   current_therapy_settings.footpedal.vac_key = 0;
   GPIOB_PDIR &= ~(1 << MODE_SHIFT);
   GPIOB_PDIR &= ~(1 << COAG_SHIFT);
   //GPIOB_PDIR &= ~(1 << ABLATE_SHIFT);
   GPIOB_PDIR |= (1 << ABLATE_SHIFT);
   put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_OFF_EVENT,1);
   get_priming_status_IgnoreAndReturn(0);
   return_value = poll_footpedal();
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marriage_err = 0;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.married = 1;
   current_therapy_settings.treatment.wireless_enable = 1;
   current_therapy_settings.footpedal.wireless.batt_low_count = 55;
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   return_value = poll_footpedal();
   
}
void test_poll_footpedal_lw_ablate_ON (void)
{

   current_therapy_settings.footpedal.wired.present = 1;
   legacy_wand_params_obj.wand_connected = 1;

   current_therapy_settings.footpedal.wireless.married2station1 = 0;
   uint8_t  return_value;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.ablate = 0x00;
   current_therapy_settings.footpedal.coag = 0x00;
   current_therapy_settings.footpedal.vac_key = 0;
   GPIOB_PDIR |= (1 << COAG_SHIFT);
   GPIOB_PDIR |= (1 << MODE_SHIFT);
   GPIOB_PDIR &= ~(1 << ABLATE_SHIFT);
   //   GPIOB_PDIR |= (1 << ABLATE_SHIFT);
   put_event_ExpectAndReturn(&evt_tbl,LW_ABLATE_ON_EVENT,1);
   get_priming_status_IgnoreAndReturn(0);
   return_value = poll_footpedal();
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marriage_err = 0;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.married = 1;
   current_therapy_settings.treatment.wireless_enable = 1;
   current_therapy_settings.footpedal.wireless.batt_low_count = 55;
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   return_value = poll_footpedal();
}


void test_poll_footpedal_lw_coag_OFF (void)
{

   current_therapy_settings.footpedal.wired.present = 0;
   legacy_wand_params_obj.wand_connected = 0;

   uint8_t  return_value;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.coag = 0x01;
   current_therapy_settings.footpedal.ablate = 0x00;
   current_therapy_settings.footpedal.vac_key = 0;
   GPIOB_PDIR &= ~(1 << MODE_SHIFT);
   GPIOB_PDIR &= ~(1 << ABLATE_SHIFT);
   //GPIOB_PDIR &= ~(1 << COAG_SHIFT);
   GPIOB_PDIR |= (1 << COAG_SHIFT);
   put_event_ExpectAndReturn(&evt_tbl,LW_COAG_OFF_EVENT,1);
   get_priming_status_IgnoreAndReturn(0);
   return_value = poll_footpedal();
   current_therapy_settings.footpedal.wireless.married2station1 = 1;
   current_therapy_settings.footpedal.wireless.marriage_err = 0;
   current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   current_therapy_settings.footpedal.wireless.married = 1;
   current_therapy_settings.treatment.wireless_enable = 1;
   current_therapy_settings.footpedal.wireless.batt_low_count = 55;
   put_event_IgnoreAndReturn(1);
   get_priming_status_IgnoreAndReturn(0);
   return_value = poll_footpedal();
   
}
void test_poll_footpedal_lw_coag_ON (void)
{


   smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_ENT;
   current_therapy_settings.footpedal.wired.present = 1;
   legacy_wand_params_obj.wand_connected = 1;
   smart_wand_params.mfg.coag_enable = 1;
   smart_wand_params.mfg.vac_enable = 1;
   current_therapy_settings.wand.unlocked = 0;
   
   current_therapy_settings.footpedal.wireless.married2station1 = 0;
   uint8_t  return_value;
   current_therapy_settings.active_wand.lw_active = 1;
   current_therapy_settings.footpedal.coag = 0x00;
   current_therapy_settings.footpedal.ablate = 0x00;
   current_therapy_settings.footpedal.vac_key = 0;
   GPIOB_PDIR |= (1 << ABLATE_SHIFT);
   GPIOB_PDIR |= (1 << MODE_SHIFT);
   GPIOB_PDIR &= ~(1 << COAG_SHIFT);
   //   GPIOB_PDIR |= (1 << COAG_SHIFT);
   put_event_ExpectAndReturn(&evt_tbl,LW_COAG_ON_EVENT,1);
   get_priming_status_IgnoreAndReturn(0);
   return_value = poll_footpedal();
   // current_therapy_settings.footpedal.wireless.married2station1 = 1;
   // current_therapy_settings.footpedal.wireless.marriage_err = 0;
   // current_therapy_settings.footpedal.wireless.marry_in_progress = 0;
   // current_therapy_settings.footpedal.wireless.married = 1;
   // current_therapy_settings.treatment.wireless_enable = 1;
   // current_therapy_settings.footpedal.wireless.batt_low_count = 55;
   // put_event_IgnoreAndReturn(1);
   // return_value = poll_footpedal();
}

void test_fs_any_button_pressed (void)
{
	bool return_value = fs_any_button_pressed();
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
