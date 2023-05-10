#include "unity.h"
#include "unity_internals.h"
#include "MK26F18.h"
#include "systick.h"
#include "Mockgui.h"
#include "system.h"

#define MAX_NBR_OF_CALLBACKS    ((u8)16)
//typedef enum {FALSE, TRUE} bool;
typedef struct _cbtmr_t
{
    bool          in_use;         ///<flag to trak availablility
    u8            id;             ///<callback identifier
    u32           trig_lvl;       ///<given time to "do something"
    u32           count;          ///<current tick value
    cbtmr_mode_t  cbtmr_mode;     ///<single or continuous?
    FlagStatus    *cbtmr_flag;    ///<returns flag to indicate "time to do something"
    cbtmr_func_t  cbtmr_func;     ///<callback function
} cbtmr_t;

extern volatile cbtmr_t cbtmr_register[MAX_NBR_OF_CALLBACKS];
FlagStatus StopAutoPrime = RESET;
uint8_t AutoPrimeId = 0U;
uint8_t AutoPrimeTimeoutId = 0U;
SMART_WAND_PARAMETERS SmartWandParams;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_systick_init (void)
{
   uint8_t return_value = systick_init();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_cbtmr_isr (void)
{
	uint8_t return_value;
	FlagStatus temp = RESET;

	cbtmr_register[0].cbtmr_flag = &temp;
	cbtmr_register[0].cbtmr_func = NULL;
	cbtmr_register[0].in_use = TRUE;
	cbtmr_register[0].trig_lvl = 1;
	cbtmr_register[0].count = 0;
	return_value = cbtmr_isr();
	
	cbtmr_register[2].in_use = TRUE;
	cbtmr_register[0].cbtmr_mode = CONTINUOUS_CBTMR;
	cbtmr_register[0].trig_lvl = 2;
	cbtmr_register[0].count = 1;
	return_value = cbtmr_isr();
	
	cbtmr_register[0].in_use = FALSE;
	cbtmr_register[2].in_use = TRUE;
    cbtmr_register[2].cbtmr_mode = CONTINUOUS_CBTMR;
	cbtmr_register[2].trig_lvl = 2;
	cbtmr_register[2].count = 1;
	return_value = cbtmr_isr();
	
	cbtmr_register[15].in_use = TRUE;
	cbtmr_register[0].cbtmr_func = &test_systick_init;
	cbtmr_register[0].cbtmr_flag = &temp;
	cbtmr_register[0].trig_lvl = 2;
	cbtmr_register[0].count = 1;
	return_value = cbtmr_isr();
	
}

void test_cbtmr_init (void)
{
	cbtmr_init();
	
}

//ErrorStatus cbtmr_reg_callback(cbtmr_func_t cbtmr_func,FlagStatus *cbtmr_flag, const u32 cbtmr_time_ms,const cbtmr_mode_t cbtmr_mode, u8 *cbtmr_id);

void test_cbtmr_reset_timer (void)
{
	const u8 ResetTimerID = 0;
	cbtmr_register[0].in_use = TRUE;
	cbtmr_register[0].id = ResetTimerID;
	cbtmr_reset_timer(ResetTimerID);
	
	cbtmr_reset_timer(2);
}

void test_cbtmr_unreg_callback (void)
{
	const u8 UnregID = 0;
	ErrorStatus return_value = cbtmr_unreg_callback(UnregID);
}

void test_cbtmr_reg_callback (void)
{
		FlagStatus clearErrorFlag = RESET;
		ErrorStatus return_value = cbtmr_reg_callback(NULL,&clearErrorFlag,1,SINGLE_CBTMR,NULL);
		
		return_value = cbtmr_reg_callback(gui_stop_auto_prime,
                             &StopAutoPrime,
                             smart_wand_params.manta.prime.autom.time,
                             SINGLE_CBTMR,
                             &AutoPrimeTimeoutId);
							 
}


	  
void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
