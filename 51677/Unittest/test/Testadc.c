#include "unity.h"
#include "MK26F18.h"
#include "common.h"
#include "adc.h"
#include "system.h"
#include <string.h>
#include "cpu.h"
#include "Mockevent_buffer.h"
#include "Mockbuck.h"
#include "Mockartc_arm_math.h"
#include "Mocklegacy_wand.h"
#include "audio.h"
#include "Mockgui.h"
// #include "../../48386/lib/lib.h"

/** buck smart wand dac output */
const DAC_MemMapPtr pSmartWandDAC = DAC1_BASE_PTR;
const FTM_MemMapPtr pSmartWandFTM = FTM3_BASE_PTR;


const DAC_MemMapPtr pLegacyWandDAC = DAC0_BASE_PTR;
const FTM_MemMapPtr pLegacyWandFTM = FTM0_BASE_PTR;

legacy_wand_params legacy_wand_params_obj;
VMOD_COMPENSATION vmod;



uint16_t over_energy_current_limit;


void setUp(void)
{
	FTM3_SWOCTRL |=  FTM_SWOCTRL_CH0OC_MASK;
}

void tearDown(void)
{
	ADC0_SC1A =0x0;
}

// void test_ADC_DMA_Init (void)
// {
//    ADC_DMA_Init();
// }
//HANGS
void test_adc_init2 (void)
{
   uint8_t return_value = adc_init_2();
}
void test_get_temp (void)
{
   uint16_t adc_value;
   get_temp(adc_value);
}

void test_irms2_adc_result (void)
{

	ramp_down_buck_voltage_IgnoreAndReturn(1);
   // Call to the set_buck_voltage() is the test for this
   set_buck_voltage_IgnoreAndReturn(0);
   put_event_ExpectAndReturn(&evt_tbl,RUN_IEC_OVERCURRENT,0);
   irms2_adc_result();
}

void test_irms2_adc_result_2 (void)
{
	put_event_IgnoreAndReturn(1);
   current_therapy_settings.wand.z_test =TRUE;
   current_therapy_settings.setpoint.dac = 100;
   smart_wand_params.impedance.z_level = 99;
   // Make sure there is no call to set_buck_voltage(), and there is a
   // call to ramp_down_buck_voltage()
   ramp_down_buck_voltage_ExpectAndReturn(pSmartWandDAC,current_therapy_settings.setpoint.dac,0);
   irms2_adc_result();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.setpoint.dac);
}

void test_irms2_adc_result_3 (void)
{
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   current_therapy_settings.wand.z_test =TRUE;
   current_therapy_settings.setpoint.dac = 100;
   smart_wand_params.impedance.z_level = 99;
   // Make sure there is no call to set_buck_voltage(), and there is a
   // call to ramp_down_buck_voltage()
   ramp_down_buck_voltage_ExpectAndReturn(pSmartWandDAC,current_therapy_settings.setpoint.dac,0);
   irms2_adc_result();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.setpoint.dac);
}

void test_irms2_adc_result_4 (void)
{
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   current_therapy_settings.wand.z_test =TRUE;
   current_therapy_settings.setpoint.dac = 50;
   smart_wand_params.impedance.z_level = 99;
   smart_wand_params.impedance.z_time = 1;
   smart_wand_params.impedance.z_level= 87;
   // Make sure there is no call to set_buck_voltage(), and there is a
   // call to ramp_down_buck_voltage()
   set_dac_IgnoreAndReturn(1);
   irms2_adc_result();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.setpoint.dac);
}

void test_irms2_adc_result_5 (void)
{
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   current_therapy_settings.wand.z_test =TRUE;
   current_therapy_settings.setpoint.dac = 50;
   smart_wand_params.impedance.z_level = 99;
   smart_wand_params.impedance.z_time = 1;
   smart_wand_params.impedance.z_level= 87;
   smart_wand_params.impedance.z_high = 100;
   smart_wand_params.impedance.z_low = 10;
  ADC1_RB = (uint16_t)(smart_wand_params.impedance.z_low+1);
   current_therapy_settings.wand.z_test = TRUE;
   set_dac_IgnoreAndReturn(1);
   // Make sure there is no call to set_buck_voltage(), and there is a
   // call to ramp_down_buck_voltage()
   irms2_adc_result();
   TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.wand.z_test);
}

void test_irms2_adc_result_6 (void)
{
	put_event_IgnoreAndReturn(1);
   FTM3_SWOCTRL |=  FTM_SWOCTRL_CH0OC_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 1;
   ramp_down_buck_voltage_ExpectAndReturn(pSmartWandDAC,current_therapy_settings.setpoint.dac,0);
   irms2_adc_result();
   // No specific test is possible for this else case.
   // Not calling ramp_down_buck_voltage and set_buck_voltage are
   // the actual tests

}

void test_irms2_adc_result_7 (void)
{
	put_event_IgnoreAndReturn(1);
   FTM3_SWOCTRL |=  FTM_SWOCTRL_CH0OC_MASK;
   current_therapy_settings.treatment.ablate_on = 0;
   ramp_down_buck_voltage_ExpectAndReturn(pSmartWandDAC,current_therapy_settings.setpoint.dac,0);
   irms2_adc_result();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.setpoint.dac);
}


void test_irms2_adc_result_8 (void)
{
   //put_event_ExpectAndReturn(&evt_tbl,RUN_IEC_OVERCURRENT,0);
   put_event_IgnoreAndReturn(1);
   FTM3_SWOCTRL |=  FTM_SWOCTRL_CH0OC_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.pulsing.mean_irms = 0;
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   // Calling it a specific number of times to increment the fast_idx
   irms2_adc_result();
   irms2_adc_result();
   irms2_adc_result();
   current_therapy_settings.setpoint.level = 1;
   current_therapy_settings.setpoint.dac = 1;
   ADC1_RB = (uint16_t)(10);
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   irms2_adc_result();
   // a zero is returned because the Vmod table is all zeros.
   // This is a phase 2 change.
   TEST_ASSERT_EQUAL(0,current_therapy_settings.pulsing.mean_irms);
}

void test_irms2_adc_result_9 (void)
{
	put_event_IgnoreAndReturn(1);
   FTM3_SWOCTRL |=  FTM_SWOCTRL_CH0OC_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.setpoint.dac= 1446;
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   irms2_adc_result();
   // No specific test possible for this case
}

// so far we have called irms2_adc_result()12 times. We need to call it
//200 times to get into the fast_idx >200 part of the code
void test_irms2_adc_result_fast_idx (void)
{
	put_event_IgnoreAndReturn(1);
   FTM3_SWOCTRL &=  ~FTM_SWOCTRL_CH0OC_MASK;
   current_therapy_settings.wand.z_test = TRUE;
   current_therapy_settings.setpoint.dac = 100;
   smart_wand_params.impedance.z_level = 150;
   smart_wand_params.impedance.z_time = 1000;
   ADC1_RB = (uint16_t)(10);
   smart_wand_params.impedance.z_high = 100;
   smart_wand_params.impedance.z_low = 5;

   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.setpoint.dac= 1446;
   uint8_t i;
   for(i = 12; i< 201;i++)
   {
      if (i == 200)
      {
         arm_mean_q15_Ignore();
         put_event_ExpectAndReturn(&evt_tbl,RUN_PUMP_SPEED_CTRL,0);
         put_event_ExpectAndReturn(&evt_tbl,WAND_IRMS_WEAR_MONITOR,0);
      }
      if ((i)%10 == 0)
      {
         current_therapy_settings.treatment.therapy_mode = VAC;
         put_event_ExpectAndReturn(&evt_tbl,RUN_IEC_OVERCURRENT,0);
      }
      ramp_down_buck_voltage_IgnoreAndReturn(1);
      irms2_adc_result();
   }
}

void test_irms2_adc_result_10 (void)
{
	put_event_IgnoreAndReturn(1);
   current_therapy_settings.wand.z_test =TRUE;
   current_therapy_settings.setpoint.dac = 50;
   smart_wand_params.impedance.z_level = 99;
   smart_wand_params.impedance.z_time = 10;
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   irms2_adc_result();
   // No specific test for this case,it just should not call
}

void test_irms2_adc_result_11 (void)
{
	put_event_IgnoreAndReturn(1);
	arm_mean_q15_Ignore();
   current_therapy_settings.wand.z_test =TRUE;
   current_therapy_settings.setpoint.dac = 50;
   smart_wand_params.impedance.z_level = 99;
   smart_wand_params.impedance.z_time = 1000;
   ramp_down_buck_voltage_IgnoreAndReturn(1);
   ADC1_RB = (uint16_t)(3);
   smart_wand_params.impedance.z_high = 100;
   smart_wand_params.impedance.z_low = 5;
   
   irms2_adc_result();
   // No specific test for this case,it just should not call
}

void test_irms2_adc_result_12 (void)
{
	put_event_IgnoreAndReturn(1);
	arm_mean_q15_Ignore();
   FTM3_SWOCTRL |=  FTM_SWOCTRL_CH0OC_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 1;
   // The setpoint.dac value is set by the
   // ramp_down_buck_voltage function, so return a value higher
   // than 2264
   ramp_down_buck_voltage_IgnoreAndReturn(2300);
   irms2_adc_result();
   // No specific test possible for this case
}

void test_irms2_adc_result_13 (void)
{
	put_event_IgnoreAndReturn(1);
	arm_mean_q15_Ignore();
   FTM3_SWOCTRL |=  FTM_SWOCTRL_CH0OC_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 1;
   // The setpoint.dac value is set by the
   // ramp_down_buck_voltage function, so set it less than
   //2264, so ipeak threshold is set by the formula
   //ipeak_threshold = current_therapy_settings.setpoint.dac / current_therapy_settings.setpoint.trip_load
   current_therapy_settings.setpoint.trip_load = 50;
   
   ramp_down_buck_voltage_IgnoreAndReturn(2200);
   irms2_adc_result();
   // No specific test possible for this case
}

void test_irms2_adc_result_14 (void)
{
	put_event_IgnoreAndReturn(1);
	arm_mean_q15_Ignore();
   current_therapy_settings.setpoint.dac = 0;

   smart_wand_params.ipeak.voltage = 100;
   current_therapy_settings.setpoint.trip_load = 50;
   vmod.vmod2_lookup[current_therapy_settings.setpoint.dac] = 0;
   ramp_down_buck_voltage_IgnoreAndReturn(0);
   irms2_adc_result();
}

void test_irms2_adc_result_15 (void)
{
	put_event_IgnoreAndReturn(1);
	arm_mean_q15_Ignore();
   current_therapy_settings.setpoint.dac = 0;

   smart_wand_params.ipeak.voltage = 100;
   current_therapy_settings.setpoint.trip_load = 0;
   vmod.vmod2_lookup[current_therapy_settings.setpoint.dac] = 0;
   ramp_down_buck_voltage_IgnoreAndReturn(0);
   irms2_adc_result();
}

void test_irms2_adc_result_16 (void)
{
	put_event_IgnoreAndReturn(1);
	arm_mean_q15_Ignore();
   current_therapy_settings.setpoint.dac = 0;

   smart_wand_params.ipeak.voltage = 101;
   current_therapy_settings.setpoint.trip_load = 1;
   vmod.vmod2_lookup[current_therapy_settings.setpoint.dac] = 100;
   ramp_down_buck_voltage_IgnoreAndReturn(0);
   irms2_adc_result();
}

void test_irms_overcurrent_2(void)
{
   FTM3_SWOCTRL &=  ~FTM_SWOCTRL_CH0OC_MASK;
   current_therapy_settings.pulsing.iec_threshold = 600;
   current_therapy_settings.pulsing.med_threshold = 600;
   current_therapy_settings.pulsing.counter_delay = 10;
   current_therapy_settings.pulsing.pulse_window  = 50;
   //current_therapy_settings.pulsing.pulse_window
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   uint16_t sample = 0;
   uint8_t return_var;
   for (sample =0; sample < 1002; sample++)
   {
      current_therapy_settings.pulsing.mean_irms = 10;
      return_var =  irms_overcurrent();
      if (current_therapy_settings.treatment.ablate_on == 0)
      {
         current_therapy_settings.treatment.ablate_on = 1;
      }

   }

}


void test_irms_overcurrent_1(void)
{
   current_therapy_settings.pulsing.iec_threshold = 250;
   current_therapy_settings.pulsing.med_threshold = 250;
   current_therapy_settings.pulsing.counter_delay = 10;
   current_therapy_settings.pulsing.pulse_window  = 50;

   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   uint16_t sample = 0;
   uint8_t return_var;
   for (sample =0; sample < 100; sample++)
   {
      current_therapy_settings.pulsing.mean_irms = 10;
      return_var =  irms_overcurrent();
      if (current_therapy_settings.treatment.ablate_on == 0)
      {
         current_therapy_settings.treatment.ablate_on = 1;
      }

   }

}

void test_irms_overcurrent_3(void)
{
   current_therapy_settings.pulsing.iec_threshold = 600;
   current_therapy_settings.pulsing.med_threshold = 600;

   current_therapy_settings.pulsing.pulse_window  = 1500;
   current_therapy_settings.pulsing.counter_delay = 0;

   //current_therapy_settings.pulsing.pulse_window
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   uint16_t sample = 0;
   uint8_t return_var;
   for (sample =0; sample < 1200; sample++)
   {
      current_therapy_settings.pulsing.mean_irms = 10;
      return_var =  irms_overcurrent();
      if (current_therapy_settings.treatment.ablate_on == 0)
      {
         current_therapy_settings.treatment.ablate_on = 1;
      }

   }

}

void test_irms_overcurrent_4(void)
{
   current_therapy_settings.pulsing.iec_threshold = 600;
   current_therapy_settings.pulsing.med_threshold = (2^30) ;
   current_therapy_settings.pulsing.pulse_window  = 5000;
   current_therapy_settings.pulsing.counter_delay = 0;
   //current_therapy_settings.pulsing.pulse_window
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   uint16_t sample = 0;
   uint8_t return_var;
   for (sample =0; sample < 1100; sample++)
   {
      current_therapy_settings.pulsing.mean_irms = 10;
      current_therapy_settings.pulsing.counter_delay = 0;
      return_var =  irms_overcurrent();

      if (current_therapy_settings.treatment.ablate_on == 0)
      {
         current_therapy_settings.treatment.ablate_on = 1;
      }
   }
}

void test_irms_overcurrent_coag_on(void)
{
   current_therapy_settings.pulsing.iec_threshold = 250;
   current_therapy_settings.pulsing.med_threshold = 250;
   current_therapy_settings.pulsing.counter_delay = 10;
   current_therapy_settings.pulsing.pulse_window  = 50;

   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   uint8_t return_var =  irms_overcurrent();
}

void test_irms_overcurrent_pulse_window(void)
{
   uint16_t pulse_window = 10;
   current_therapy_settings.setpoint.dac = 0;
   current_therapy_settings.pulsing.pulse_window = pulse_window;
   uint8_t return_var =  irms_overcurrent();

}


void test_ADC_Cal_calfail (void)
{
   ADC_MemMapPtr pADC = ADC0_BASE_PTR;
   pADC->SC1[0]  = ~COCO_NOT;
   pADC->SC3 = ADC_SC3_CALF_MASK;
   uint8_t return_value = ADC_Cal(pADC);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_ADC_Cal_calpass (void)
{
   ADC_MemMapPtr pADC = ADC0_BASE_PTR;
   pADC->SC1[0]  = ~COCO_NOT;
   pADC->SC3 = ~ADC_SC3_CALF_MASK;
   uint8_t return_value = ADC_Cal(pADC);
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_legacy_irms_adc_result (void)
{
	legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
   legacy_set_buck_voltage_IgnoreAndReturn(0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,1);
   uint8_t return_value = legacy_irms_adc_result();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_legacy_irms_adc_result_2 (void)
{	
   legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
    put_event_IgnoreAndReturn(1);
   legacy_wand_params_obj.z_test = TRUE;
   uint8_t return_value = legacy_irms_adc_result();
   TEST_ASSERT_EQUAL(0,return_value);
   legacy_wand_params_obj.z_test = FALSE;
}


void test_legacy_irms_adc_result_3 (void)
{
	legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
	put_event_IgnoreAndReturn(1);
   legacy_wand_params_obj.z_test = TRUE;
   legacy_set_buck_voltage_IgnoreAndReturn(0);
   current_therapy_settings.legacy_wand_levels.dac = 10;
   legacy_wand_params_obj.impedance.z_level = 5;
   legacy_ramp_down_buck_voltage_ExpectAndReturn(current_therapy_settings.legacy_wand_levels.dac,1);
   legacy_ramp_down_buck_voltage_IgnoreAndReturn(0);
   uint8_t return_value = legacy_irms_adc_result();
   TEST_ASSERT_EQUAL(0,return_value);
   legacy_wand_params_obj.z_test = FALSE;
}

void test_legacy_irms_adc_result_4 (void)
{
   	legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
    legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
	legacy_set_buck_voltage_IgnoreAndReturn(0);
   // Reset Z time
   legacy_wand_params_obj.z_test = FALSE;
   uint8_t return_value = legacy_irms_adc_result();
   // Call the function again to pass the z time test
   legacy_wand_params_obj.z_test = TRUE;
   current_therapy_settings.legacy_wand_levels.dac = 10;
   legacy_wand_params_obj.impedance.z_level = 15;
   legacy_wand_params_obj.impedance.z_time = 0;
   return_value = legacy_irms_adc_result();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_legacy_irms_adc_result_4_MAX_DAC (void)
{
   	legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
	legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
	legacy_set_buck_voltage_IgnoreAndReturn(0);
   // Reset Z time
   legacy_wand_params_obj.z_test = FALSE;
   uint8_t return_value = legacy_irms_adc_result();
   // Call the function again to pass the z time test
   legacy_wand_params_obj.z_test = TRUE;
   current_therapy_settings.legacy_wand_levels.dac = 0x10;
   legacy_wand_params_obj.impedance.z_level = 15;
   legacy_wand_params_obj.impedance.z_time = 0;
   // Expect 10 and return 0xFFFF
   legacy_ramp_down_buck_voltage_ExpectAndReturn(0x10, 0xFFFF);
   return_value = legacy_irms_adc_result();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_legacy_irms_adc_result_4a (void)
{
	legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
	legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
   legacy_set_buck_voltage_IgnoreAndReturn(0);
   // Reset Z time
   legacy_wand_params_obj.z_test = FALSE;
   uint8_t return_value = legacy_irms_adc_result();
   // Call the function again to pass the z time test
   legacy_wand_params_obj.z_test = TRUE;
   current_therapy_settings.legacy_wand_levels.dac = 10;
   legacy_wand_params_obj.impedance.z_level = 15;
   legacy_wand_params_obj.impedance.z_time = 0;

   legacy_wand_params_obj.impedance.z_high = 100;
   legacy_wand_params_obj.impedance.z_low = 1;
   ADC0_RA= (uint16_t)(10);
   return_value = legacy_irms_adc_result();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_legacy_irms_adc_result_5 (void)
{
	
   legacy_set_buck_voltage_IgnoreAndReturn(0);
   uint8_t return_value;
   // just to get into the over energy function which is
   // called once everyt 10 times.
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,1);
   current_therapy_settings.legacy_wand_levels.topaz_timer = 1;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   
   for (uint8_t i =0; i < 11; i++)
   {
	   legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
      return_value = legacy_irms_adc_result();
   }
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   
}

void test_legacy_irms_over_energy (void)
{
   put_event_ExpectAndReturn(&evt_tbl,RF_OFF,1);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = legacy_irms_over_energy();
   over_energy_current_limit = 100;
   for (uint8_t i = 30; i > 1 ; i--)
   {
	   put_event_IgnoreAndReturn(1);
      over_energy_current_limit = 100;
      return_value = legacy_irms_over_energy();
   }
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   return_value = legacy_irms_over_energy();
}

void test_legacy_irms_over_energy_peak_detect (void)
{
   over_energy_current_limit = 0xFFFF;
   put_event_IgnoreAndReturn(1);
      //put_event_ExpectAndReturn(&evt_tbl,RF_OFF,1);
   uint8_t return_value = legacy_irms_over_energy();
   over_energy_current_limit = 0xFFFF;
   legacy_wand_params_obj.peak_detect_flag = 1;
   
   for (uint8_t i = 30; i > 1 ; i--)
   {

      put_event_ExpectAndReturn(&evt_tbl,RF_OFF,1);
      put_event_ExpectAndReturn(&evt_tbl, RF_ON,1);
      return_value = legacy_irms_over_energy();
      legacy_wand_params_obj.peak_detect_flag = 0;
   }
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);

   return_value = legacy_irms_over_energy();
}


void test_legacy_timer_wands (void)
{
    put_event_IgnoreAndReturn(1);
   //put_event_ExpectAndReturn(&evt_tbl, LW_ABLATE_OFF_EVENT,1);
   legacy_timer_wands ();
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   legacy_timer_wands ();
}

//////////////////////////////////////////////////////////////////
void test_adc0_coco_stator(void)
{
	uint8_t return_value = 0;
	return_value = adc0_coco_stator();
	
}

//HANGS-Fixed
void test_poll_adc_data (void)
{
   // Not setting the ADC_SC1_COCO_MASK
   uint32_t expected_data = 86;//0x40;
   ADC0_SC1A = 0x00;
   uint8_t return_var = poll_adc_data();
   // If conversion is not complete, ADC0_SC1A must remain the same
   TEST_ASSERT_EQUAL(expected_data,ADC0_SC1A);
}


void test_adc3_coco (void)
{
   ADC0_SC1A = 0x0000005A;
   ADC0_RA= (unsigned short)0;
   uint8_t return_value;
   //put_event_ExpectAndReturn(&evt_tbl,INTERNAL_TEMP_ERROR,0);
   for (uint8_t i = 0; i < 33; i++)
   {
      ADC0_SC1A = 0x0000005A;
      return_value = adc3_coco();
   }
   // Checking to see if the right event is posted is the test
   // for this case
}


void test_adc3_coco_1 (void)
{
   ADC0_SC1A = 0x0000005A;
   ADC0_RA= (unsigned short)20000;
   //put_event_ExpectAndReturn(&evt_tbl,CLEAR_INTERNAL_TEMP_ERROR,0);
   uint8_t return_value;
   for (uint8_t i = 0; i < 12; i++)
   {
      ADC0_SC1A = 0x0000005A;
      ADC0_RA= (unsigned short)20000;
      return_value = adc3_coco();
      TEST_ASSERT_EQUAL(0,return_value);
   }
   // Checking to see if the right event is posted is the test
   // for this case
}

void test_adc3_coco_2 (void)
{
   ADC0_SC1A =0x0000005B;
   ADC0_RA= (unsigned short)(MIN_BANDGAP_SPEC-1);
   put_event_ExpectAndReturn(&evt_tbl,ADC_BOUNDARY_FAULT,0);
   uint8_t return_value;
   return_value = adc3_coco();
   TEST_ASSERT_EQUAL(ADC0_SC1A,0x00000053);

}

void test_adc3_coco_8 (void)
{
   put_event_ExpectAndReturn(&evt_tbl,ADC_BOUNDARY_FAULT,0);
   ADC0_SC1A =0x0000005B;
   ADC0_RA= (unsigned short)(MAX_BANDGAP_SPEC-1);
   //not calling the put_event function with ADC_BOUNDARY_FAULT
   uint8_t return_value;
   return_value = adc3_coco();
   TEST_ASSERT_EQUAL(ADC0_SC1A,0x00000050);

}


void test_adc3_coco_3 (void)
{
   ADC0_SC1A =0x0000005B;
   ADC0_RA= (unsigned short)(MAX_BANDGAP_SPEC+1);
   put_event_ExpectAndReturn(&evt_tbl,ADC_BOUNDARY_FAULT,0);
   uint8_t return_value;
   return_value = adc3_coco();
   TEST_ASSERT_EQUAL(ADC0_SC1A,0x00000053);

}

void test_adc3_coco_4 (void)
{
   ADC0_SC1A =0x00000053;
   ADC0_RA= (unsigned short)64;
   current_therapy_settings.pump.stator = 128;
   uint8_t return_value;
   return_value = adc3_coco();
   TEST_ASSERT_EQUAL(64,current_therapy_settings.pump.stator);

}

void test_adc3_coco_5 (void)
{
   ADC0_SC1A =0x0;
   current_therapy_settings.footpedal.present = 0;
   ADC0_RA= (unsigned short)24433 -1;
   current_therapy_settings.treatment.vac_enabled = 1;
   current_therapy_settings.pump.stator = 8500;
   uint8_t return_value;
   return_value = adc3_coco();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.pump.stator = 0000;
}

void test_adc3_coco_9 (void)
{
   ADC0_SC1A =0x0;
   current_therapy_settings.footpedal.present = 0;
   ADC0_RA= (unsigned short)32063+1;
   current_therapy_settings.treatment.vac_enabled = 1;
   uint8_t return_value;
   return_value = adc3_coco();
   // IN this footpedal present status is not updated
   TEST_ASSERT_EQUAL(0,current_therapy_settings.footpedal.present);
}

void test_adc3_coco_10 (void)
{
   ADC0_SC1A =0x0;
   current_therapy_settings.footpedal.present = 1;
   ADC0_RA= (unsigned short)24433;
   current_therapy_settings.treatment.vac_enabled = 1;
   uint8_t return_value;
   return_value = adc3_coco();
   // If footpedal is already present, VAC status is not updated
   TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.vac_enabled);
}


void test_adc3_coco_6 (void)
{
   ADC0_SC1A =0x0;
   current_therapy_settings.footpedal.present = 0;
   ADC0_RA= (unsigned short)24433;
   current_therapy_settings.treatment.vac_enabled = 0;
   uint8_t return_value;
   return_value = adc3_coco();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_adc3_coco_7 (void)
{
   ADC0_SC1A =0x0;
   current_therapy_settings.footpedal.present = 1;
   ADC0_RA= (32063+1);
   current_therapy_settings.treatment.vac_enabled = 1;
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,0);
   uint8_t return_value;
   return_value = adc3_coco();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_ent_blendizer_1(void)
{
	bool return_val;
	return_val = ent_blendizer(0, 0, FALSE);
}

void test_legacy_ent_timer_wands_1(void)
{
	current_therapy_settings.treatment.ablate_on = TRUE;
	legacy_ent_timer_wands();
}

void test_legacy_ent_timer_wands_2(void)
{
	legacy_wand_params_obj.timer.current = 1001;
	legacy_wand_params_obj.timer.audio_muted = FALSE;
	current_therapy_settings.treatment.ablate_on = TRUE;
	legacy_ent_timer_wands();
}

void test_legacy_irms_adc_result_6 (void)
{
   FTM0_SWOCTRL = 0;//FTM_SWOCTRL_CH0OC_MASK;
   //put_event_ExpectAndReturn(&evt_tbl,RUN_PUMP_SPEED_CTRL,1);
   
   
   legacy_set_buck_voltage_IgnoreAndReturn(1000);
   // legacy_set_buck_voltage_IgnoreAndReturn(0);
   uint8_t return_value;
   // just to get into the over energy function which is
   // called once everyt 10 times.
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);	
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);	
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,RUN_PUMP_SPEED_CTRL,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   put_event_ExpectAndReturn(&evt_tbl,LW_RUN_IEC_OVERCURRENT,0);
   current_therapy_settings.legacy_wand_levels.topaz_timer = 0;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   
   for (uint8_t i =0; i < 195; i++)
   {
	   //put_event_ExpectAndReturn(&evt_tbl,RUN_PUMP_SPEED_CTRL,0);
	   //legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
	   legacy_set_buck_voltage_IgnoreAndReturn(1000);
      return_value = legacy_irms_adc_result();
   }
   TEST_ASSERT_EQUAL(0,return_value);
   // put_event_ExpectAndReturn(&evt_tbl,RUN_PUMP_SPEED_CTRL,0);
   // legacy_ramp_down_buck_voltage_IgnoreAndReturn(1);
   // legacy_irms_adc_result();
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1; 
}

void test_irms_overcurrent_5(void)
{
	current_therapy_settings.pulsing.half_iec_threshold = 300;
   current_therapy_settings.pulsing.iec_threshold = 2;
   current_therapy_settings.pulsing.med_threshold = 0 ;
   current_therapy_settings.pulsing.pulse_window  = 0;
   current_therapy_settings.pulsing.counter_delay = 0;
   //current_therapy_settings.pulsing.pulse_window
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   uint16_t sample = 0;
   uint8_t return_var;
   for (sample =0; sample < 1100; sample++)
   {
      current_therapy_settings.pulsing.mean_irms = 10;
      current_therapy_settings.pulsing.counter_delay = 0;
      return_var =  irms_overcurrent();

      if (current_therapy_settings.treatment.ablate_on == 0)
      {
         current_therapy_settings.treatment.ablate_on = 1;
      }
   }
}

void test_irms2_adc_result_17 (void)
{
	set_dac_IgnoreAndReturn(0);
	set_dac_IgnoreAndReturn(0);
	get_priming_status_IgnoreAndReturn(0);
	put_event_IgnoreAndReturn(1);
	FTM3_SWOCTRL &=  ~FTM_SWOCTRL_CH0OC_MASK;
	current_therapy_settings.setpoint.trip_load  = 0;
	put_event_IgnoreAndReturn(1);
	arm_mean_q15_Ignore();
   current_therapy_settings.setpoint.dac = 0;
   current_therapy_settings.setpoint.ipeak_voltage = 4000;
   smart_wand_params.ipeak.voltage = 101;
   vmod.vmod2_lookup[current_therapy_settings.setpoint.dac] = 100;
   ramp_down_buck_voltage_IgnoreAndReturn(0);
   irms2_adc_result();
}


void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}

