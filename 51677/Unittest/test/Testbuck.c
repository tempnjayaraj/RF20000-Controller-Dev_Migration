#include "unity.h"
#include <stdint.h>
#include "Types.h"
#include "MK26F18.h"
#include "common.h"
#include "buck.h"
#include "Mockpit.h"
#include "Mocksystem.h"
#include "Mockevent_buffer.h"
#include "Mocklegacy_wand.h"
#include "Mockartc_arm_math.h"

#define TEST 2
GEN_SETTINGS current_therapy_settings;
//const DAC_MemMapPtr pSmartWandDAC = DAC1_BASE_PTR;
extern const DAC_MemMapPtr pLegacyWandDAC;
extern const DAC_MemMapPtr pSmartWandDAC;
extern const uint16_t buck_log_sample_vector[512];
extern VMOD_COMPENSATION vmod;
event_table evt_tbl;
legacy_wand_params legacy_wand_params_obj;
SMART_WAND_PARAMETERS smart_wand_params;
bool unitTest;

const uint16_t buck_log_sample_vector[512] = 
  { 
    0, 100, 100, 100, 100, 100, 100, 100,
100, 100, 100, 100, 100, 100, 100, 100,
100, 100, 100, 100, 100, 100, 100, 100,
100, 101, 101, 101, 101, 101, 101, 101,
101, 101, 101, 101, 101, 101, 101, 101,
101, 101, 101, 101, 101, 101, 101, 101,
101, 101, 101, 101, 101, 101, 101, 101,
101, 102, 102, 102, 102, 102, 102, 102,
102, 102, 102, 102, 102, 102, 102, 102,
102, 102, 102, 102, 102, 102, 103, 103,
103, 103, 103, 103, 103, 103, 103, 103,
103, 103, 103, 103, 103, 104, 104, 104,
104, 104, 104, 104, 104, 104, 104, 104,
104, 104, 105, 105, 105, 105, 105, 105,
105, 105, 105, 105, 106, 106, 106, 106,
106, 106, 106, 106, 106, 107, 107, 107,
107, 107, 107, 107, 108, 108, 108, 108,
108, 108, 108, 109, 109, 109, 109, 109,
109, 110, 110, 110, 110, 110, 110, 111,
111, 111, 111, 111, 112, 112, 112, 112,
112, 113, 113, 113, 113, 114, 114, 114,
114, 115, 115, 115, 115, 116, 116, 116,
116, 117, 117, 117, 118, 118, 118, 118,
119, 119, 119, 120, 120, 120, 121, 121,
122, 122, 122, 123, 123, 123, 124, 124,
125, 125, 126, 126, 126, 127, 127, 128,
128, 129, 129, 130, 130, 131, 131, 132,
132, 133, 133, 134, 135, 135, 136, 136,
137, 138, 138, 139, 139, 140, 141, 141,
142, 143, 144, 144, 145, 146, 147, 147,
148, 149, 150, 151, 151, 152, 153, 154,
155, 156, 157, 158, 159, 160, 161, 162,
163, 164, 165, 166, 167, 168, 169, 170,
172, 173, 174, 175, 176, 178, 179, 180,
182, 183, 184, 186, 187, 189, 190, 192,
193, 195, 196, 198, 199, 201, 203, 204,
206, 208, 210, 211, 213, 215, 217, 219,
221, 223, 225, 227, 229, 231, 233, 236,
238, 240, 242, 245, 247, 250, 252, 255,
257, 260, 262, 265, 268, 271, 273, 276,
279, 282, 285, 288, 291, 294, 297, 301,
304, 307, 311, 314, 318, 321, 325, 329,
332, 336, 340, 344, 348, 352, 356, 361,
365, 369, 374, 378, 383, 387, 392, 397,
402, 407, 412, 417, 422, 427, 433, 438,
444, 449, 455, 461, 467, 473, 479, 485,
491, 498, 504, 511, 518, 525, 532, 539,
546, 553, 561, 568, 576, 584, 592, 600,
608, 616, 625, 633, 642, 651, 660, 669,
678, 688, 697, 707, 717, 727, 738, 748,
759, 769, 780, 792, 803, 814, 826, 838,
850, 862, 875, 888, 900, 914, 927, 940,
954, 968, 982, 997, 1012, 1026, 1042, 1057,
1073, 1089, 1105, 1121, 1138, 1155, 1172, 1190,
1208, 1226, 1244, 1263, 1282, 1301, 1321, 1341,
1361, 1382, 1403, 1424, 1446, 1468, 1491, 1513,
1536, 1560, 1584, 1608, 1633, 1658, 1683, 1709,
1736, 1763, 1790, 1817, 1845, 1874, 1903, 1933,
1963, 1993, 2024, 2056, 2088, 2120, 2153, 2187,
2221, 2256, 2291, 2327, 2363, 2400, 2438, 2476,
2515, 2555, 2595, 2636, 2677, 2719, 2762, 2806,
2850, 2895, 2941, 2987, 3035, 3083, 3132, 3181,
3232, 3283, 3335, 3388, 3442, 3496, 3552, 3608,
3666, 3724, 3784, 3844, 3905, 3967, 4031, 4095,

  };

void setUp(void)
{
  SIM_SCGC2 = 0;

}

void tearDown(void)
{
}


void test_buck_init(void)
{
   uint8_t return_value = buck_init();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_set_buck_voltage (void)
{
   uint16_t irms = 0;
   uint16_t current_dac = 0;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   //TEST_IGNORE_MESSAGE("Need test");
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_set_buck_voltage_adjustment (void)
{
   current_therapy_settings.pulsing.target_current = 3000;
   current_therapy_settings.setpoint.level = 10;
   uint16_t irms = 0;
   uint16_t current_dac = 9;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(9,return_value);
}

void test_set_buck_voltage_adjustment_2 (void)
{
   current_therapy_settings.pulsing.target_current = 3000;
   current_therapy_settings.setpoint.level = 80;
   uint16_t irms = 0;
   uint16_t current_dac = 40;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(40,return_value);
}

void test_set_buck_voltage_adjustment_3 (void)
{
   current_therapy_settings.pulsing.target_current = 3000;
   current_therapy_settings.setpoint.level = 150;
   uint16_t irms = 0;
   uint16_t current_dac = 1;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_set_buck_voltage_adjustment_4 (void)
{
   current_therapy_settings.pulsing.target_current = 1;
   current_therapy_settings.setpoint.level = 150;
   uint16_t irms = 6500;
   uint16_t current_dac = 250;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(250,return_value);
}

void test_set_buck_voltage_adjustment_5 (void)
{
   current_therapy_settings.pulsing.target_current = 1;
   current_therapy_settings.setpoint.level = 150;
   uint16_t irms = 6500;
   uint16_t current_dac = 230;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(230,return_value);
}

void test_set_buck_voltage_adjustment_6 (void)
{
   current_therapy_settings.pulsing.target_current = 1;
   current_therapy_settings.setpoint.level = 150;
   uint16_t irms = 2001;
   uint16_t current_dac = 240;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(240,return_value);
}

void test_set_buck_voltage_adjustment_7 (void)
{
   current_therapy_settings.pulsing.target_current = 1;
   current_therapy_settings.setpoint.level = 150;
   uint16_t irms = 1;
   uint16_t current_dac = 240;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(240,return_value);
}

void test_set_buck_voltage_adjustment_8 (void)
{
   current_therapy_settings.pulsing.target_current = 105;
   smart_wand_params.ramp_rate.decrease = 100;
   current_therapy_settings.setpoint.level = 150;
   uint16_t irms = 1;
   uint16_t current_dac = 240;
   uint16_t return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(150,return_value);
   
   current_dac = 160;
   current_therapy_settings.setpoint.level = 160;
   smart_wand_params.ramp_rate.decrease = 150;
   smart_wand_params.ramp_rate.increase = 150;
   return_value = set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(160,return_value);
   
}


void test_ramp_down_buck_voltage(void)
{
   uint16_t current_dac = 250;
   uint16_t return_value = ramp_down_buck_voltage(pSmartWandDAC,current_dac);
   TEST_ASSERT_EQUAL(10,return_value);
}

void test_ramp_down_buck_voltage_2(void)
{
   uint16_t current_dac = 230;
   uint16_t return_value = ramp_down_buck_voltage(pSmartWandDAC,current_dac);
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_buck_inactive_shutdown(void)
{
   uint8_t return_value = buck_inactive_shutdown();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_vmod2_test (void)
{
   uint8_t return_value = vmod2_test();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_legacy_ramp_down_test_buck_voltage (void)
{
   //LEGACY_RAMP_DOWN_CONST = 30;
   uint16_t  current_dac = 100;
   uint16_t return_value = legacy_ramp_down_buck_voltage(current_dac);
   TEST_ASSERT_EQUAL(return_value,70);
}


void test_legacy_ramp_down_test_buck_voltage_2 (void)
{
   //LEGACY_RAMP_DOWN_CONST = 30;
   uint16_t  current_dac = 20;
   uint16_t return_value = legacy_ramp_down_buck_voltage(current_dac);
   TEST_ASSERT_EQUAL(return_value,0);
}

void test_legacy_set_buck_voltage (void)
{
   uint16_t irms = 0;
   uint16_t current_dac = 100;
   legacy_wand_params_obj.level = 10;
   uint16_t return_value = legacy_set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(10,return_value);
}

void test_legacy_set_buck_voltage_2 (void)
{
   uint16_t irms = 0;
   uint16_t current_dac = 100;
   legacy_wand_params_obj.level = 110;
   uint16_t return_value = legacy_set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(110,return_value);
}

void test_legacy_set_buck_voltage_3 (void)
{
   // LEGACY_RAMPUP_CONSTANT = 15
   uint16_t irms = 0;
   uint16_t current_dac = 100;
   legacy_wand_params_obj.level = 160;
   uint16_t return_value = legacy_set_buck_voltage(irms,current_dac);
   TEST_ASSERT_EQUAL(115,return_value);
}


void test_set_dac (void)
{
   uint16_t command = 5;
   vmod.vmod2_lookup[0] = 10;
   uint16_t return_value = set_dac(command);
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_set_dac_1 (void)
{
   uint16_t command = 1005;
   vmod.vmod2_lookup[4095] = 1000;
   uint16_t return_value = set_dac(command);
   TEST_ASSERT_EQUAL(1000,return_value);
}

void test_set_dac_2 (void)
{
   uint16_t command = 100;
   vmod.vmod2_lookup[100] = 10;
   uint16_t return_value = set_dac(command);
   TEST_ASSERT_EQUAL(10,return_value);
}

void test_sort_vmeas (void)
{
   for (uint16_t i = 0; i < 4096; i++)
   {
      vmod.vmod2_meas[i]=i;
   }
   vmod.vmod2_meas[10]=11;
   vmod.vmod2_meas[11]=10;
   sort_vmeas(&vmod.vmod2_meas[0]);
   TEST_ASSERT_EQUAL(11,vmod.vmod2_lookup[11]);
   TEST_IGNORE_MESSAGE("need to change the header description block");
   TEST_IGNORE_MESSAGE("ARRAY length is 4096 instead of 4095, the loops limits should used the MAX_12BIT_NUM-1 ");
}

void test_interp2 ()
{
   // reset = 0;

   uint8_t return_value = interp2 (0);
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_interp2_complete ()
{
   uint8_t return_value;
   uint8_t reset = 1;
   //Call it once to reset it
   return_value = interp2 (1);
   for (uint16_t i = 0; i <= 4096; i++)
   {
      return_value = interp2 (0);
   }
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_interpt1 ()
{
   TEST_IGNORE_MESSAGE("this function is no longer used");
}

void test_vmod2_ramp_test ()
{
   uint8_t reset = 1;
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (reset);
   TEST_ASSERT_EQUAL(RAMP_UP,  return_value);
}

void test_vmod2_ramp_test_ramp_up_1 ()
{
   uint8_t reset = 0;
   uint16_t dac_value = 495;
   //vmod.vmod2_meas[dac_value] = 100;
   vmod.mod2_meas = 0xFFF0000;
   vmod.vmod2_meas[dac_value-1] =  90;
   SET_DAC(pSmartWandDAC,dac_value);
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (reset);
   TEST_ASSERT_EQUAL(RAMP_UP,  return_value);
}


void test_vmod2_ramp_test_ramp_up ()
{
   uint8_t reset = 0;
   uint16_t dac_value = 4095;
   SET_DAC(pSmartWandDAC,dac_value);
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (reset);
   TEST_ASSERT_EQUAL(VMOD_INTERP,  return_value);
}




void test_vmod2_ramp_test_vmod_interp ()
{
   uint8_t reset = 0;
   uint16_t dac_value = 4095;
   uint16_t return_value0;
   return_value0 = interp2 (1);
   for (uint16_t i = 0; i < 4096; i++)
   {
      return_value0 = interp2 (0);
   }
   //put_event_ExpectAndReturn(&evt_tbl,VMOD2_TEST,0);
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (reset);
}


void test_vmod2_vmod_test_hold ()
{
   uint8_t reset = 0;
   uint16_t dac_value = 405;
   SET_DAC(pSmartWandDAC,dac_value);
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (reset);
   TEST_ASSERT_EQUAL(RAMP_UP,  return_value);
}

void test_vmod2_vmod_ramp_down ()
{
   uint8_t reset = 0;
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (reset);
   TEST_ASSERT_EQUAL(RAMP_UP, return_value);
}

void test_vmod2_vmod_test_done ()
{
   uint8_t reset = 0;
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (reset);
   TEST_ASSERT_EQUAL(RAMP_UP, return_value);
}

void test_ramp_down_buck_test_voltage (void)
{
   uint16_t dac_value = 405;
   SET_DAC(pSmartWandDAC,dac_value);
   uint16_t dac_cmd = 3855+240;
   uint8_t reset = 0;
   uint16_t return_value;
   for (uint8_t i = 0; i < 10; i++)
   {
      return_value  = ramp_down_buck_test_voltage (pSmartWandDAC,dac_cmd,reset);
   }
   
   TEST_ASSERT_EQUAL(3855, return_value);
}

void test_ramp_down_buck_test_voltage_2 (void)
{
   uint16_t dac_value = 405;
   SET_DAC(pSmartWandDAC,dac_value);
   uint16_t dac_cmd = 3855+260;
   uint8_t reset = 0;
   uint16_t return_value;
   return_value  = ramp_down_buck_test_voltage (pSmartWandDAC,dac_cmd,reset);
   TEST_ASSERT_EQUAL(3875, return_value);
}


void test_vmod2_vmod_test_hold_1 ()
{
   uint8_t reset = 0;
   uint16_t dac_value = 4095;
   SET_DAC(pSmartWandDAC,dac_value);
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (1);
   vmod.mod2_meas = 0xFFF0;
   vmod.vmod2_meas[dac_value-1] =  100;
   vmod.vmod2_meas[dac_value] =  150;


      return_value = vmod2_ramp_test (reset);
 
   TEST_IGNORE_MESSAGE(" if (ulimit_dac > 3412) cannot be made true");
}


void test_vmod_interp (void)
{
   //interpt2_IgnoreAndReturn(0);
   uint8_t return_value = vmod_interp ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_vmod1_test (void)
{
   uint8_t return_value = vmod1_test ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_vmod1_ramp_test1 (void)
{
   VMOD_TEST_STATUS return_value = vmod1_ramp_test (1);
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_vmod1_ramp_test2 (void)
{
   const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   uint16_t vmod1_sample_idx = 512;
   //VMOD_TEST_STATUS status = RAMP_UP;
   vmod.vmod1_test_status = RAMP_UP;
   //uint16_t dac = 2056;
   //set_dac(command);
   //GET_DAC_Expect(2056);
   VMOD_TEST_STATUS return_value = vmod1_ramp_test (0);
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_vmod1_ramp_test3 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod1_test_status = VMOD_TEST_HOLD; 
   VMOD_TEST_STATUS return_value = vmod1_ramp_test (0);
   TEST_ASSERT_EQUAL(RAMP_DOWN,return_value);
}

void test_vmod1_ramp_test4 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod1_test_status = RAMP_DOWN; 
   VMOD_TEST_STATUS return_value = vmod1_ramp_test (0);
   TEST_ASSERT_EQUAL(VMOD_TEST_DONE,return_value);
}

void test_vmod1_ramp_test5 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod1_test_status = VMOD_TEST_DONE; 
   VMOD_TEST_STATUS return_value = vmod1_ramp_test (0);
   TEST_ASSERT_EQUAL(VMOD_TEST_DONE,return_value);
}

void test_vmod1_ramp_test6 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod1_test_status = 15; //default
   VMOD_TEST_STATUS return_value = vmod1_ramp_test (0);
   TEST_ASSERT_EQUAL(VMOD_TEST_RESET,return_value);
}

/*   vmod2     */

void test_vmod2_ramp_test3 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod2_test_status = VMOD_INTERP; 
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (0);
   TEST_ASSERT_EQUAL(VMOD_INTERP,return_value);
}

void test_vmod2_ramp_test4 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod2_test_status = VMOD_TEST_HOLD; 
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (0);
   TEST_ASSERT_EQUAL(RAMP_DOWN,return_value);
}

void test_vmod2_ramp_test5 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod2_test_status = RAMP_DOWN; 
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (0);
   TEST_ASSERT_EQUAL(RAMP_DOWN,return_value);
}

void test_vmod2_ramp_test6 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod2_test_status = VMOD_TEST_DONE; //default
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (0);
   TEST_ASSERT_EQUAL(VMOD_TEST_DONE,return_value);
}

void test_vmod2_ramp_test7 (void)
{
   //const uint16_t vmod_ulimit = 2560, vmod_llimit = 1536;
   vmod.vmod2_test_status = 15; //default
   VMOD_TEST_STATUS return_value = vmod2_ramp_test (0);
   TEST_ASSERT_EQUAL(VMOD_TEST_RESET,return_value);
}

/*   vmod2     */

void test_set_buck_constant_voltage_constant_power_1 (void)
{
	uint16_t return_value, irms, dac, vlimit = 0;
	float power_target;
	unitTest = FALSE;
	arm_sqrt_f32_IgnoreAndReturn(ARM_MATH_SUCCESS);
	return_value = set_buck_voltage_constant_power(irms, dac, 1, 1);
	TEST_ASSERT_EQUAL(0,return_value);
}

void test_set_buck_constant_voltage_constant_power_2 (void)
{
	uint16_t return_value, irms = 1, dac = 0, vlimit = 0;
	float power_target;
	unitTest = TRUE;
	arm_sqrt_f32_IgnoreAndReturn(ARM_MATH_SUCCESS);
	return_value = set_buck_voltage_constant_power(irms, dac, 1, 1);
	TEST_ASSERT_EQUAL(1,return_value);
}

void test_set_buck_constant_voltage_constant_power_3 (void)
{
	uint16_t return_value, irms = 0, dac = 1, vlimit = 0;
	float power_target = 16.0f;
	//float32_t power_target_result;
	arm_sqrt_f32_IgnoreAndReturn(ARM_MATH_SUCCESS);
	//arm_sqrt_f32_ExpectAndReturn(power_target, power_target_result, ARM_MATH_SUCCESS);
	return_value = set_buck_voltage_constant_power(irms, dac, power_target, 1);
	TEST_ASSERT_EQUAL(1,return_value);
}

void test_set_buck_constant_voltage_constant_power_4 (void)
{
	uint16_t return_value, irms = 5, dac = 4095, vlimit = 1000;
	float power_target = 16.0f;
	//float32_t power_target_result;
	arm_sqrt_f32_IgnoreAndReturn(ARM_MATH_SUCCESS);
	//arm_sqrt_f32_ExpectAndReturn(power_target, power_target_result, ARM_MATH_SUCCESS);
	return_value = set_buck_voltage_constant_power(irms, dac, power_target, vlimit);
	TEST_ASSERT_EQUAL(3855,return_value);
}

void test_set_buck_constant_voltage_constant_power_5 (void)
{
	uint16_t return_value, irms = 5, dac = 2, vlimit = 2;
	float power_target = 1024.0f;
	unitTest = TRUE;
	//float32_t power_target_result;
	arm_sqrt_f32_IgnoreAndReturn(ARM_MATH_SUCCESS);
	//arm_sqrt_f32_ExpectAndReturn(power_target, power_target_result, ARM_MATH_SUCCESS);
	return_value = set_buck_voltage_constant_power(irms, dac, power_target, vlimit);
	TEST_ASSERT_EQUAL(2,return_value);
}

void test_set_buck_constant_voltage_constant_power_6 (void)
{
	uint16_t return_value, irms = 5, dac = 1024, vlimit = 1030;
	float power_target = 1024.0f;
	unitTest = TRUE;
	//float32_t power_target_result;
	arm_sqrt_f32_IgnoreAndReturn(ARM_MATH_SUCCESS);
	//arm_sqrt_f32_ExpectAndReturn(power_target, power_target_result, ARM_MATH_SUCCESS);
	return_value = set_buck_voltage_constant_power(irms, dac, power_target, vlimit);
	TEST_ASSERT_EQUAL(1030,return_value);
}

void test_set_buck_constant_voltage_constant_power_7 (void)
{
	uint16_t return_value, irms = 5, dac = 1024, vlimit = 1023;
	float power_target = 1024.0f;
	unitTest = TRUE;
	//float32_t power_target_result;
	arm_sqrt_f32_IgnoreAndReturn(ARM_MATH_SUCCESS);
	//arm_sqrt_f32_ExpectAndReturn(power_target, power_target_result, ARM_MATH_SUCCESS);
	return_value = set_buck_voltage_constant_power(irms, dac, power_target, vlimit);
	TEST_ASSERT_EQUAL(1023,return_value);
}

void test_set_buck_constant_voltage_constant_power_8 (void)
{
	uint16_t return_value, irms = 5, dac = 512, vlimit = 500;
	float power_target = 1024.0f;
	unitTest = FALSE;
	//float32_t power_target_result;
	arm_sqrt_f32_IgnoreAndReturn(ARM_MATH_SUCCESS);
	//arm_sqrt_f32_ExpectAndReturn(power_target, power_target_result, ARM_MATH_SUCCESS);
	return_value = set_buck_voltage_constant_power(irms, dac, power_target, vlimit);
	TEST_ASSERT_EQUAL(272,return_value);
}

void test_vmod1_adc_coco (void)
{
	put_event_IgnoreAndReturn(1);
	uint8_t return_value = 0;
	return_value = vmod1_adc_coco();
	
}

void test_vmod2_adc_coco (void)
{
	put_event_IgnoreAndReturn(1);
	uint8_t return_value = 0;
	return_value = vmod2_adc_coco();
	
	
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
