#include "unity.h"
#include "unity_internals.h"
#include "Mockpit.h"
#include "MK26F18.h"
#include "Mocksystem.h"
#include "rf.h"
#include "cpu.h"
#include "Mockbuck.h"
#include "Mockevent_buffer.h"
#include "Mocklegacy_wand.h"
#include "Mockwand.h"

legacy_wand_params legacy_wand_params_obj;
GEN_SETTINGS current_therapy_settings;
//GEN_SETTINGS *const pCurrentTherapySettings = &current_therapy_settings;
SMART_WAND_PARAMETERS smart_wand_params;

event_table evt_tbl;

uint16_t manta_pwm_mod;
uint16_t manta_pwm_C0V;
uint16_t manta_pwm_C1V;
uint16_t manta_pwm_C2V;
uint16_t manta_pwm_C3V;
uint16_t manta_pwm_C4V;
uint16_t manta_pwm_C5V;
uint16_t manta_pwm_C6V;
uint16_t manta_pwm_C7V;
bool manta_pwm_values_valid;


/* buck 18-pin dac output */
const DAC_MemMapPtr pLegacyWandDAC = DAC0_BASE_PTR;
/* buck smart wand dac output */
const DAC_MemMapPtr pSmartWandDAC = DAC1_BASE_PTR;


void setUp(void)
{
}

void tearDown(void)
{
}

void test_SmartWand_PWMInit (void)
{
    
	smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
	manta_pwm_values_valid = TRUE;
	uint8_t return_value = smartwand_pwm_init();
    TEST_ASSERT_EQUAL(1,return_value);
	
	manta_pwm_values_valid = FALSE;
	return_value = smartwand_pwm_init();
	TEST_ASSERT_EQUAL(1,return_value);
}

void test_LegacyWand_PWMInit (void)
{
   LegacyWand_PWMInit();
   TEST_ASSERT_EQUAL((PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK), PORTD_PCR6);
}


void test_FTM_Init (void)
{
   FTM_MemMapPtr pFTMPTR = FTM0_BASE_PTR;
   FTM_Init(pFTMPTR);
   TEST_ASSERT_EQUAL(FTM_SYNC_SWSYNC_MASK,(pFTMPTR->SYNC&FTM_SYNC_SWSYNC_MASK));
}

void test_ScopeSaver_Init (void)
{
   CMP_MemMapPtr piPeak1CMP = CMP1_BASE_PTR;
   uint8_t x = 10;
   ScopeSaver_Init(piPeak1CMP,x);
   TEST_ASSERT_EQUAL((CMP_MUXCR_PSEL(7) | CMP_MUXCR_MSEL(x)),piPeak1CMP->MUXCR);
}

void test_IRQ_PWM_Fault_FTM_FMS_FAULTF2_MASK_false (void)
{
   FTM_MemMapPtr pFTMPTR = FTM3_BASE_PTR;
   uint8_t x1 = 10;
   uint8_t x2 = 10;
   current_therapy_settings.wand.tube_warn = 0;
   // Getting the correct events is the test
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   // put_event_ExpectAndReturn(&evt_tbl, RF_ON,1);
   // put_event_ExpectAndReturn(&evt_tbl,OVER_ENERGY,1);
   IRQ_PWM_Fault(pFTMPTR, x1, x2);
}

void test_IRQ_PWM_Fault_FTM_FMS_FAULTF2_MASK_false_1 (void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   FTM_MemMapPtr pFTMPTR = FTM3_BASE_PTR;
   uint8_t x1 = 10;
   uint8_t x2 = 10;
   current_therapy_settings.wand.tube_warn = 1;
   // Getting the correct events is the test
   IRQ_PWM_Fault(pFTMPTR, x1, x2);
}

void test_IRQ_PWM_Fault_FTM_FMS_FAULTF2_MASK_true (void)
{
   FTM_MemMapPtr pFTMPTR = FTM3_BASE_PTR;
   FTM3_FMS |= FTM_FMS_FAULTF2_MASK;
   uint8_t x1 = 10;
   uint8_t x2 = 10;
   // Getting the correct events is the test
   put_event_IgnoreAndReturn(1);
   put_event_IgnoreAndReturn(1);
   IRQ_PWM_Fault(pFTMPTR, x1, x2);
}

/* void test_lw_IRQ_PWM_Fault_FTM_FMS_FAULTF2_MASK_true (void)
{
   uint8_t x1 = 10;
   uint8_t x2 = 10;
   FTM_MemMapPtr pFTMPTR = FTM0_BASE_PTR;
   pFTMPTR->FMS |= x1;
   // Getting the correct events is the test
   put_event_ExpectAndReturn(&evt_tbl,WAND_SHORT,1);
   //put_event_ExpectAndReturn(&evt_tbl,OVER_CURRENT,1);
   legacy_wand_IRQ_PWM_fault(pFTMPTR, x1, x2);
   pFTMPTR->FMS = 0;
   legacy_wand_IRQ_PWM_fault(pFTMPTR, x1, x2);
} */

void test_PWM_SoftStart (void)
{
   FTM_MemMapPtr pFTMPTR = FTM0_BASE_PTR;
   uint8_t x1 = 10;
   //bool return_value = PWM_SoftStart(pFTMPTR,x1);
   TEST_IGNORE_MESSAGE("Remove PWM_SoftStart() declaration ");
}

void test_PWM_SoftStart_DMA_Init (void)
{
   FTM_MemMapPtr pFTMPTR = FTM0_BASE_PTR;
   uint16_t x1 = 10;
   uint16_t x2 = 20;
   //PWM_SoftStart_DMA_Init(x1, pFTMPTR,x2);
   TEST_IGNORE_MESSAGE("Remove PWM_SoftStart_DMA_Init() declaration");
}

void test_legacy_wand_IRQ_PWM_fault (void)
{	
	FTM_MemMapPtr pFTMPTR = FTM0_BASE_PTR;
	legacy_wand_IRQ_PWM_fault(pFTMPTR, FTM_FMS_FAULTF1_MASK, FTM_FMS_FAULTF3_MASK);
	
	pFTMPTR->FMS = 0xFFFF;
	put_event_IgnoreAndReturn(1);
	//put_event_ExpectAndReturn(&evt_tbl,WAND_SHORT,1);
	legacy_wand_IRQ_PWM_fault(pFTMPTR, FTM_FMS_FAULTF1_MASK, FTM_FMS_FAULTF3_MASK);
}

 void test_FTM_Init_MANTA(void)
 {
	FTM_Init_MANTA(pSmartWandFTM);
 }
 
 
void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
