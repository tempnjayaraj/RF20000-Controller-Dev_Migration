#include "unity.h"
#include "unity_internals.h"
#include "Mocksystem.h"
#include "switches.h"
#include "MK26F18.h"
#include "Mockfootpedal.h"
#include "Mockrf.h"
#include "Mockaudio.h"
#include "Mockbuck.h"
#include "Mockpit.h"
#include "Mockwand.h"
#include "Mockadc.h"
#include "Mockgui.h"
#include "Mockinit.h"
#include "Mockcpu.h"
#include "Mockpump.h"
#include "Mockevent_buffer.h"
#include "Mocklegacy_wand.h"
#include "Mockprofiles.h"

extern uint32_t therapy_start_time;
/** DAC values for different ablate levels */
const uint16_t legacy_wand_ablate_dac_values [11] = 
{
 0,
1171,
1473,
1797,
2109,
2432,
2756,
3063,
3379,
3694,
3756
};

/** DAC values for different coag levels */
const uint16_t legacy_wand_coag_dac_values[3] = 
{
  0,
757,
1171
};

/*
// These are current limits for respective setpoints from 1 to 10.
// The limits are given below in Amps
 
1 2.6
2 2.27
3 2.08
4 1.9
5 1.74
6 1.61
7 1.54
8 1.44
9 1.39
10 1.33

*/
const uint16_t legacy_over_energy_current_limits_for_ablate[11] = 
{
0,      // For level 0
25817,
22540,
20653,
18866,
17277,
15987,
15292,
14299,
13802,
13206
};

/* 
C1 3.19A
C2 2.6A
*/
const uint16_t legacy_over_energy_current_limits_for_coag[3] = 
{
  0,   // For level 0
31675,
25817
};

////////////////////////////////Special Limits /////////////////////////////////

/*
// These are current limits for respective setpoints from 1 to 10.
// The limits are given below in Amps for R1= 324, 649, 1210
 
1 2.0
2 1.9
3 1.69
4 1.39
5 1.3
6 1.21
7 1.18
8 1.18
9 1.16
10 0.5 (Level is not reached by any of these wands, so lvl 10 is set very low
        0 for safety reasons)

*/
const uint16_t legacy_over_energy_current_limits_for_ablate_27pin[11] = 
{
0,      // For level 0
19879,
18975,
16807,
13825,
12908,
12108,
11717,
11717,
11518,
4965
};


/* This is for R1 = 324, 649, 1210
C1 2.70A
C2 2.0A
*/
const uint16_t legacy_over_energy_current_limits_for_coag_27pin[3] = 
{
  0,   // For level 0
26884,
19879 
};

/*
  @ENT
  wave form characterizations are different between SportsMed and ENT
*/
//DAC values for different ENT ablate levels
const uint16_t ent_legacy_wand_ablate_dac_values [10] =
{
  0,
  1204, // 100V
  1536, // 127.5V
  1867, // 155V
  2198, // 182.5V
  2529, // 210V
  2861, // 237.5V
  3192, // 265V
  3493, // 290V
  3614, // 300V
};

//DAC values for different ENT coag levels
const uint16_t ent_legacy_wand_coag_dac_values[10] =
{
  0,
  783,  // 65V
  843,  // 70V
  903,  // 75V
  975,  // 81V
  1048, // 87V
  1120, // 93V
  1204, // 100V
  1301, // 108V
  1385, // 115V
};

/*
// These are current limits for respective setpoints from 1 to 10.
// The limits are given below in Amps

1 2.6
2 2.27
3 2.08
4 1.9
5 1.74
6 1.61
7 1.54
8 1.44
9 1.39
10 1.33

*/
const uint16_t ent_legacy_over_energy_current_limits_for_ablate[10] =
{
  0,      // For level 0
  25817,
  22540,
  20653,
  18866,
  17277,
  15987,
  15292,
  14299,
  13802
};

const uint16_t ent_legacy_over_energy_current_limits_for_coag[10] =
{
  0,   // For level 0
  17873, // C1 = 1.8 Amps
  18866, // C2 = 1.9 Amps
  18866, // C3 = 1.9 Amps
  19859, // C4 = 2.0 Amps
  22838, // C5 = 2.3 Amps
  24824, // C6 = 2.5 Amps
  25817, // C7 = 2.6 Amps
  26810, // C8 = 2.7 Amps
  26810, // C9 = 2.7 Amps
};

manta_prime prime;

typedef enum {
  COAG_ZILCH,
  COAG_MINUS,
  COAG_PLUS
};

GEN_SETTINGS current_therapy_settings;

event_table evt_tbl;
SMART_WAND_PARAMETERS smart_wand_params;
SMART_WAND_PARAMETERS *const pSmartWandParams = &smart_wand_params;
PUMP_CMD pump;
const FTM_MemMapPtr pSmartWandFTM = FTM3_BASE_PTR;
const FTM_MemMapPtr pLegacyWandFTM = FTM0_BASE_PTR;
/* buck 18-pin dac output */
const DAC_MemMapPtr pLegacyWandDAC = DAC0_BASE_PTR;

/* buck smart wand dac output */
const DAC_MemMapPtr pSmartWandDAC = DAC1_BASE_PTR;

legacy_wand_params legacy_wand_params_obj;
const uint16_t legacy_wand_ablate_dac_values[11];
const uint16_t legacy_wand_coag_dac_values[3];
const uint16_t legacy_over_energy_current_limits_for_ablate[11];
const uint16_t legacy_over_energy_current_limits_for_coag[3];
const uint16_t legacy_over_energy_current_limits_for_ablate_27pin[11];
const uint16_t legacy_over_energy_current_limits_for_coag_27pin[3];
uint16_t over_energy_current_limit;

void setUp(void)
{
   // Flush all the events everytime a new test is called
   // while (get_event(& evt_tbl) != NULL_EVENT)
   // {
   //    // Stay in this loop to flush all the events
   // }
 

}

void tearDown(void)
{
}


void test_change_mode(void)
{
   cancel_new_profile_IgnoreAndReturn(1);
   cancel_new_profile_IgnoreAndReturn(1);
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   //current_therapy_settings.gui.mode_key = 0;
   current_therapy_settings.pump.status  |= PUMP_READY;

   //Pump_Config_Expect(PUMP_IDLE);
   uint8_t return_value = change_mode();
   TEST_ASSERT_EQUAL(IDLE_SPEED,pump.state);

   current_therapy_settings.pump.status  &= ~PUMP_READY;
   pump.state  = PROTOCOL;

   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   //Pump_Config_Expect(PUMP_IDLE);
   return_value = change_mode();
   TEST_ASSERT_EQUAL(PROTOCOL,pump.state);


}

void test_change_mode_1(void)
{
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   //current_therapy_settings.gui.mode_key = 0;
   current_therapy_settings.pump.status  |= PUMP_READY;

   //Pump_Config_Expect(PUMP_IDLE);
   uint8_t return_value = change_mode();
   TEST_ASSERT_EQUAL(0,return_value);

}


void test_switch_ablate_parameters_LO_ABLATE_PLUS (void)
{
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // Set it up so we get in the LO mode, ABLATE_PLUS state
   float32_t current_sp_plus = 3.14;
   uint16_t voltage_sp_plus = 3140;
   pSmartWandParams->lo.voltage_sp_plus = voltage_sp_plus;
   pSmartWandParams->lo.current_sp_plus = current_sp_plus;
   current_therapy_settings.treatment.therapy_mode = LO;
   current_therapy_settings.setpoint.lo = ABLATE_PLUS;

   // Call the functions
   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_plus,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(current_sp_plus,current_therapy_settings.pump.pid.sp_amps);
   TEST_ASSERT_EQUAL(LO,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_PLUS,current_therapy_settings.setpoint.lo);


}


void test_switch_ablate_parameters_LO_ABLATE_MINUS (void)
{
   float32_t current_sp_minus = 3.141517;
   uint16_t voltage_sp_minus = 5235;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.treatment.therapy_mode = LO;
   current_therapy_settings.setpoint.lo = ABLATE_MINUS;
   pSmartWandParams->lo.voltage_sp_minus = voltage_sp_minus;
   pSmartWandParams->lo.current_sp_minus = current_sp_minus;
   current_therapy_settings.log.level = 3;

   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_minus,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(current_sp_minus,current_therapy_settings.pump.pid.sp_amps);
   TEST_ASSERT_EQUAL(LO,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_MINUS,current_therapy_settings.setpoint.lo);

}


void test_switch_ablate_parameters_LO_ABLATE_DEFAULT (void)
{
   float32_t current_sp_default = 3.141517;
   uint16_t voltage_sp_default = 5235;
   current_therapy_settings.treatment.therapy_mode = LO;
   current_therapy_settings.setpoint.lo = ABLATE_DEFAULT;
   pSmartWandParams->lo.voltage_sp_default = voltage_sp_default;
   pSmartWandParams->lo.current_sp_default = current_sp_default;
   current_therapy_settings.log.level = 3;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_default,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(current_sp_default,current_therapy_settings.pump.pid.sp_amps);
   TEST_ASSERT_EQUAL(LO,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_DEFAULT,current_therapy_settings.setpoint.lo);

}

void test_switch_ablate_parameters_LO_ABLATE_ZILCH (void)
{
   float32_t current_sp_default = 3.141517;
   uint16_t voltage_sp_default = 5235;

   current_therapy_settings.treatment.therapy_mode = LO;
   current_therapy_settings.setpoint.lo = ABLATE_ZILCH;
   pSmartWandParams->lo.voltage_sp_default = voltage_sp_default;
   pSmartWandParams->lo.current_sp_default = current_sp_default;

   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.log.level = 0;
   // Call the functions
   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL(0,current_therapy_settings.log.level);
}



void test_switch_ablate_parameters_MED_ABLATE_PLUS (void)
{
   // Set it up so we get in the MED mode, ABLATE_PLUS state
   float32_t current_sp_plus = 3.14;
   uint16_t voltage_sp_plus = 3140;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   pSmartWandParams->med.voltage_sp_plus = voltage_sp_plus;
   pSmartWandParams->med.current_sp_plus = current_sp_plus;
   current_therapy_settings.treatment.therapy_mode = MED;
   current_therapy_settings.setpoint.med = ABLATE_PLUS;

   // Call the functions
   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_plus,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(current_sp_plus,current_therapy_settings.pump.pid.sp_amps);
   TEST_ASSERT_EQUAL(MED,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_PLUS,current_therapy_settings.setpoint.med);


}

void test_switch_ablate_parameters_MED_ZILCH (void)
{
   // Set it up so we get in the MED mode, ABLATE_PLUS state
   float32_t current_sp_plus = 3.14;
   uint16_t voltage_sp_plus = 3140;
   pSmartWandParams->med.voltage_sp_plus = voltage_sp_plus;
   pSmartWandParams->med.current_sp_plus = current_sp_plus;
   current_therapy_settings.treatment.therapy_mode = MED;
   current_therapy_settings.setpoint.med = ABLATE_ZILCH;

   current_therapy_settings.log.level = 0;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   // Call the functions
   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL(0,current_therapy_settings.log.level);
}




void test_switch_ablate_parameters_MED_ABLATE_MINUS (void)
{
   float32_t current_sp_minus = 3.141517;
   uint16_t voltage_sp_minus = 5235;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.treatment.therapy_mode = MED;
   current_therapy_settings.setpoint.med = ABLATE_MINUS;
   pSmartWandParams->med.voltage_sp_minus = voltage_sp_minus;
   pSmartWandParams->med.current_sp_minus = current_sp_minus;
   current_therapy_settings.log.level = 3;

   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_minus,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(current_sp_minus,current_therapy_settings.pump.pid.sp_amps);
   TEST_ASSERT_EQUAL(MED,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_MINUS,current_therapy_settings.setpoint.med);

}


void test_switch_ablate_parameters_MED_ABLATE_DEFAULT (void)
{
   float32_t current_sp_default = 3.141517;
   uint16_t voltage_sp_default = 5235;

   current_therapy_settings.treatment.therapy_mode = MED;
   current_therapy_settings.setpoint.med = ABLATE_DEFAULT;
   pSmartWandParams->med.voltage_sp_default = voltage_sp_default;
   pSmartWandParams->med.current_sp_default = current_sp_default;
   current_therapy_settings.log.level = 3;

   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_default,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(current_sp_default,current_therapy_settings.pump.pid.sp_amps);
   TEST_ASSERT_EQUAL(MED,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_DEFAULT,current_therapy_settings.setpoint.med);

}

void test_switch_ablate_parameters_HI_ABLATE_PLUS (void)
{
   // Set it up so we get in the HI mode, ABLATE_PLUS state
   float32_t max_rps_plus = 3.14;
   uint16_t voltage_sp_plus = 3140;
   pSmartWandParams->hi.voltage_sp_plus = voltage_sp_plus;
   pSmartWandParams->hi.max_rps_plus = max_rps_plus;
   current_therapy_settings.treatment.therapy_mode = HI;
   current_therapy_settings.setpoint.hi = ABLATE_PLUS;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // Call the functions
   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_plus,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(max_rps_plus,current_therapy_settings.pump.pid.max_rps);
   TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_PLUS,current_therapy_settings.setpoint.hi);


}


void test_switch_ablate_parameters_HI_ABLATE_MINUS (void)
{
   float32_t max_rps_minus = 3.141517;
   uint16_t voltage_sp_minus = 5235;

   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.treatment.therapy_mode = HI;
   current_therapy_settings.setpoint.hi = ABLATE_MINUS;
   pSmartWandParams->hi.voltage_sp_minus = voltage_sp_minus;
   pSmartWandParams->hi.max_rps_minus = max_rps_minus;
   current_therapy_settings.log.level = 3;

   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_minus,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(max_rps_minus,current_therapy_settings.pump.pid.max_rps);
   TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_MINUS,current_therapy_settings.setpoint.hi);

}


void test_switch_ablate_parameters_HI_ABLATE_DEFAULT (void)
{
   float32_t max_rps_default = 3.141517;
   uint16_t voltage_sp_default = 5235;

   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.treatment.therapy_mode = HI;
   current_therapy_settings.setpoint.hi = ABLATE_DEFAULT;
   pSmartWandParams->hi.voltage_sp_default = voltage_sp_default;
   pSmartWandParams->hi.max_rps_default = max_rps_default;
   current_therapy_settings.log.level = 3;

   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL_UINT16(voltage_sp_default,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL_FLOAT(max_rps_default,current_therapy_settings.pump.pid.max_rps);
   TEST_ASSERT_EQUAL(HI,current_therapy_settings.treatment.therapy_mode);
   TEST_ASSERT_EQUAL(ABLATE_DEFAULT,current_therapy_settings.setpoint.hi);

}

void test_switch_ablate_parameters_HI_ABLATE_ZILCH (void)
{
   float32_t max_rps_default = 3.141517;
   uint16_t voltage_sp_default = 5235;

   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.treatment.therapy_mode = HI;
   current_therapy_settings.setpoint.hi = ABLATE_ZILCH;
   pSmartWandParams->hi.voltage_sp_default = voltage_sp_default;
   pSmartWandParams->hi.max_rps_default = max_rps_default;
   current_therapy_settings.log.level = 0;
   // Call the functions
   switch_ablate_parameters();

   //Test if parameters are setup properly
   TEST_ASSERT_EQUAL(0,current_therapy_settings.log.level);
}



void test_switch_ablate_parameters_VAC (void)
{
   float32_t max_rps_default = 3.141517;
   uint16_t voltage_sp_default = 5235;

   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.log.level = 1;
   current_therapy_settings.treatment.therapy_mode = VAC;
   switch_ablate_parameters();
   //Test if parameters are setup properly
   //TEST_ASSERT_EQUAL(VAC,current_therapy_settings.log.mode);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.log.level);
}

void test_switch_ablate_parameters_DEFAULT_MODE (void)
{
   current_therapy_settings.treatment.therapy_mode = NONE;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   switch_ablate_parameters();
   //Test if parameters are setup properly
   //TEST_ASSERT_EQUAL(NONE,current_therapy_settings.log.mode);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.log.level);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.setpoint.level);

}

void test_switch_ablate_parameters_DEFAULT_MODE_notify_status (void)
{
   current_therapy_settings.audio.p_sound_file =(uint16_t *) 0x1234;
   current_therapy_settings.notify.status = 0xFF;
   current_therapy_settings.treatment.therapy_mode = NONE;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   switch_ablate_parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL((uint16_t *) 0x1234,current_therapy_settings.audio.p_sound_file);
}

void test_switch_ablate_parameters_DEFAULT_MODE_max_rps (void)
{
   //pSmartWandParams->vac.min_rps_default = -15.0;
   pSmartWandParams->vac.max_rps_default = 15.0;

   current_therapy_settings.treatment.therapy_mode = VAC;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   switch_ablate_parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL(10.0f,current_therapy_settings.pump.pid.max_rps);
}

void test_switch_ablate_parameters_DEFAULT_MODE_idle_rps (void)
{
   pSmartWandParams->hi.idle_rps = 15.0f;
   current_therapy_settings.treatment.therapy_mode = HI;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   switch_ablate_parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL(10.0f,current_therapy_settings.pump.idle_rps);
}

void test_switch_ablate_parameters_DEFAULT_MODE_idle_rps_clockwise (void)
{
   pSmartWandParams->hi.idle_rps = -15.0f;
   current_therapy_settings.treatment.therapy_mode = HI;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   switch_ablate_parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL(0.0f,current_therapy_settings.pump.idle_rps);
}

void test_switch_ablate_parameters_DEFAULT_MODE_min_rps (void)
{
   pSmartWandParams->vac.min_rps_default = -15.0;
   //pSmartWandParams->vac.max_rps_default = 15.0;
   current_therapy_settings.treatment.therapy_mode = VAC;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   switch_ablate_parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL(0.0f,current_therapy_settings.pump.pid.min_rps);
}

void test_change_setpoint_PENDING (void)
{
   //   current_therapy_settings.setpoint.cut_sync = PENDING;
   // uint8_t return_value =  change_setpoint();
   // TEST_ASSERT_EQUAL(SYNCD,current_therapy_settings.setpoint.cut_sync);
   // Need test
}

void test_change_setpoint_NOT_PENDING (void)
{
   //   current_therapy_settings.setpoint.cut_sync = 0;
   //  uint8_t return_value =  change_setpoint();
   //   TEST_ASSERT_EQUAL(0,current_therapy_settings.setpoint.cut_sync);
   // Need test
}



void test_therapy_off (void)
{
	get_priming_status_IgnoreAndReturn(0);
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.treatment.previous_therapy_mode = MED;
   
   //current_therapy_settings.treatment.preVAC_therapy_mode = MED;
   //Pump_Config_Expect(PUMP_IDLE);
   log_use_data_Expect(therapy_start_time);
   
   uint8_t return_value =  therapy_off();
   TEST_ASSERT_EQUAL(MED,current_therapy_settings.treatment.therapy_mode);
}


void test_therapy_off_user_screen (void)
{
	get_priming_status_IgnoreAndReturn(0);
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
      log_use_data_Expect(therapy_start_time);
   uint8_t return_value =  therapy_off();
   TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN,current_therapy_settings.gui.screenID);
}

void test_Switch_Coag_Mode_Parameters_COAG_MINUS (void)
{
   uint16_t coag1_voltage = 1234;
   current_therapy_settings.log.level = 2;
   pSmartWandParams->coag.coag1_voltage = coag1_voltage;
   current_therapy_settings.setpoint.coag = COAG_MINUS;
   current_therapy_settings.pump.on = 1;// Disable calling the pump
   Switch_Coag_Mode_Parameters();
   TEST_ASSERT_EQUAL(coag1_voltage,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.log.level);
}

void test_Switch_Coag_Mode_Parameters_COAG_PLUS (void)
{
   uint16_t coag2_voltage = 4321;
   current_therapy_settings.log.level = 0;
   pSmartWandParams->coag.coag2_voltage = coag2_voltage;
   current_therapy_settings.pump.on = 1;// Disable calling the pump
   current_therapy_settings.setpoint.coag = COAG_PLUS;
   Switch_Coag_Mode_Parameters();
   TEST_ASSERT_EQUAL(coag2_voltage,current_therapy_settings.setpoint.level);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.log.level);
}

void test_Switch_Coag_Mode_Parameters_COAG_ZILCH (void)
{
   current_therapy_settings.log.level = 2;
   current_therapy_settings.setpoint.coag = COAG_ZILCH;
   Switch_Coag_Mode_Parameters();
   // Make sure the log level is not changed
   TEST_ASSERT_EQUAL(2,current_therapy_settings.log.level);
}

void test_Switch_Coag_Mode_parameters_notify_status_1 (void)
{
   current_therapy_settings.audio.p_sound_file =(uint16_t *) 0x1234;
   current_therapy_settings.notify.status = 0xFF;
   current_therapy_settings.setpoint.coag = COAG_ZILCH;
   Switch_Coag_Mode_Parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL((uint16_t *) 0x1234,current_therapy_settings.audio.p_sound_file);
}

void test_Switch_Coag_Mode_parameters_notify_status_0 (void)
{
   current_therapy_settings.audio.p_sound_file =(uint16_t *) 0x1234;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.setpoint.coag = COAG_ZILCH;
   Switch_Coag_Mode_Parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL(0,current_therapy_settings.audio.p_sound_file);
}



void test_Switch_Coag_Mode_Parameters_idle_rps (void)
{
   current_therapy_settings.pump.idle_rps = 15.0f;
   current_therapy_settings.setpoint.coag = COAG_ZILCH;
   Switch_Coag_Mode_Parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL(10.0f,current_therapy_settings.pump.idle_rps);
}

void test_Switch_Coag_Mode_Parameters_DEFAULT_MODE_idle_rps (void)
{
   current_therapy_settings.pump.idle_rps = -15.0f;
   current_therapy_settings.setpoint.coag = COAG_ZILCH;
   Switch_Coag_Mode_Parameters();
   //Make sure audio.p_sound_file value is not changed
   TEST_ASSERT_EQUAL(0.0f,current_therapy_settings.pump.idle_rps);
}


void test_Update_Irms_Overcurrent_Thresholds (void)
{
   uint16_t counter_delay = 12;
   uint32_t med_threshold = 0;
   uint32_t iec_threshold = 0;
   current_therapy_settings.pulsing.pulse_window = counter_delay;
   
   Update_Irms_Overcurrent_Thresholds();
   //TEST_ASSERT_EQUAL(iec_threshold, current_therapy_settings.pulsing.iec_threshold);
   //TEST_ASSERT_EQUAL(med_threshold,current_therapy_settings.pulsing.med_threshold);
   TEST_ASSERT_EQUAL((counter_delay/2 + 1),current_therapy_settings.pulsing.counter_delay);
}


void test_rf_off (void)
{
   uint8_t return_value = rf_off();
   // Test and make sure the DACs are set to zero
   TEST_ASSERT_EQUAL(0,(pSmartWandDAC)->DAT[1].DATL);
   TEST_ASSERT_EQUAL(0,(pSmartWandDAC)->DAT[1].DATH);
   TEST_ASSERT_EQUAL(0,(pSmartWandDAC)->DAT[0].DATL);
   TEST_ASSERT_EQUAL(0,(pSmartWandDAC)->DAT[0].DATL);

   TEST_ASSERT_EQUAL(0,(pLegacyWandDAC)->DAT[1].DATL);
   TEST_ASSERT_EQUAL(0,(pLegacyWandDAC)->DAT[1].DATH);
   TEST_ASSERT_EQUAL(0,(pLegacyWandDAC)->DAT[0].DATL);
   TEST_ASSERT_EQUAL(0,(pLegacyWandDAC)->DAT[0].DATL);
   TEST_ASSERT_EQUAL(0, return_value);
}

void test_measure_z (void)
{
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   
   uint8_t return_value =  measure_z();
   TEST_ASSERT_EQUAL(TRUE, current_therapy_settings.wand.z_test);
   //   TEST_ASSERT_EQUAL(RF_ON, get_event(& evt_tbl));
   TEST_ASSERT_EQUAL(0, return_value);
}

// void test_therapy_on (void)
// {
	// cancel_new_profile_IgnoreAndReturn(1);
   // wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // current_therapy_settings.wand.cut_key = 1;
   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.wand.vac_key = 0;
   // current_therapy_settings.footpedal.mode = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.wand.valid = TRUE;
   // //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.pump.sensor_status = 15;
   // current_therapy_settings.pump.ctrl_en = FALSE;
   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   // current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // // Call the pump
   // current_therapy_settings.pump.on = 1;
   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status |= PUMP_ON;

   // // We are expecting to call the pump twice
   // //Pump_Config_Expect(ENABLE);
   // //Pump_Config_Expect(RUN);
   // smart_wand_params.mfg.low_enable = 1;
   // smart_wand_params.mfg.vac_enable = 1;
   // smart_wand_params.mfg.coag_enable =1;
 
   // uint8_t return_value = therapy_on();


   // TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
   // TEST_ASSERT_EQUAL(1,return_value);

// }

// void test_therapy_on_RF2000 (void)
// {
	// cancel_new_profile_IgnoreAndReturn(1);
   // //   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // current_therapy_settings.wand.cut_key = 1;
   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.wand.vac_key = 0;
   // current_therapy_settings.footpedal.mode = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.wand.valid = TRUE;
   // //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.pump.sensor_status = 15;
   // current_therapy_settings.pump.ctrl_en = FALSE;
   // current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // // Call the pump
   // current_therapy_settings.pump.on = 1;
   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status |= PUMP_ON;

   // // We are expecting to call the pump twice
   // //Pump_Config_Expect(ENABLE);
   // //Pump_Config_Expect(RUN);
   // wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   // uint8_t return_value = therapy_on();


   // TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
   // TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
   // TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.pump.ctrl_en);
   // TEST_ASSERT_EQUAL(1,return_value);

// }


// void test_therapy_on_1 (void)
// {
	// get_priming_status_IgnoreAndReturn(0);
	// cancel_new_profile_IgnoreAndReturn(1);
   // current_therapy_settings.wand.cut_key = 1;
   // current_therapy_settings.footpedal.ablate = 1;
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.wand.vac_key = 0;
   // current_therapy_settings.footpedal.mode = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.wand.valid = TRUE;
   // //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.pump.sensor_status = 15;
   // current_therapy_settings.pump.ctrl_en = FALSE;
   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   // current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // // Call the pump
   // current_therapy_settings.pump.on = 1;
   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status |= PUMP_ON;

   // uint8_t return_value = therapy_on();

   // TEST_ASSERT_EQUAL(0,return_value);
// }


// void test_therapy_on_3 (void)
// {
	// get_priming_status_IgnoreAndReturn(0);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
   // current_therapy_settings.wand.cut_key = 0;
   // current_therapy_settings.footpedal.ablate = 1;
   // current_therapy_settings.footpedal.coag = 1;
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.wand.vac_key = 0;
   // current_therapy_settings.footpedal.mode = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.wand.valid = TRUE;
   // //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.pump.sensor_status = 15;
   // current_therapy_settings.pump.ctrl_en = FALSE;
   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   // current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // // Call the pump
   // current_therapy_settings.pump.on = 1;
   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status |= PUMP_ON;

   // uint8_t return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.footpedal.vac_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.wand.coag_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.wand.vac_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.wand.vac_key = 0;
   // current_therapy_settings.footpedal.mode = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.footpedal.mode = 0;
   // current_therapy_settings.wand.mode_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.wand.valid = FALSE;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.wand.valid = TRUE;
   // current_therapy_settings.pump.on = 0;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.pump.on = 1;
   // current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   // current_therapy_settings.pump.status &= ~SENSORS_OK;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.pump.status &= ~SENSORS_OK;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status &= ~PUMP_ON;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
// }

// void test_therapy_on_vac (void)
// {
	// get_priming_status_IgnoreAndReturn(0);
	// cancel_new_profile_IgnoreAndReturn(1);
   // wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // current_therapy_settings.wand.cut_key = 0;
   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.wand.vac_key = 0;
   // current_therapy_settings.footpedal.mode = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.wand.valid = TRUE;
   // current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.wand.vac_key = 1;
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.pump.sensor_status = 15;

   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status |= PUMP_ON;

// //current_therapy_settings.gui.screenID == USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN
   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   // current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // // Call the pump
   // current_therapy_settings.pump.on = 1;
   // // We are expecting to call the pump twice
   // //Pump_Config_Expect(ENABLE);
   // //Pump_Config_Expect(RUN);

   // uint8_t return_value = therapy_on();




   // TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
   // TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.pump.ctrl_en);
   // TEST_ASSERT_EQUAL(1,return_value);
// }

// void test_therapy_on_vac_1 (void)
// {
	// get_priming_status_IgnoreAndReturn(0);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // current_therapy_settings.wand.cut_key = 0;
   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.wand.vac_key = 1;
   // current_therapy_settings.footpedal.mode = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.wand.valid = TRUE;
   // current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.wand.vac_key = 1;
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.pump.sensor_status = 15;

   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status |= PUMP_ON;

// //current_therapy_settings.gui.screenID == USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN
   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   // current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // // Call the pump
   // current_therapy_settings.pump.on = 1;
   // // We are expecting to call the pump twice
   // //Pump_Config_Expect(ENABLE);
   // //Pump_Config_Expect(RUN);
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   // current_therapy_settings.footpedal.vac_key = 1;
   // uint8_t return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.footpedal.mode = 1;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // current_therapy_settings.footpedal.vac_key = 1;
   // current_therapy_settings.wand.vac_key = 0;
   // current_therapy_settings.wand.mode_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.footpedal.mode = 0;
   // current_therapy_settings.footpedal.coag = 1;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.footpedal.ablate = 1;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.wand.coag_key = 1;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.wand.cut_key = 1;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.wand.cut_key = 0;
   // current_therapy_settings.wand.valid = FALSE;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.wand.valid = TRUE;
   // current_therapy_settings.pump.on = 0;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.pump.on = 1;
   // current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.pump.sensor_status = 15;
   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status |= PUMP_ON;
   // wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // //TEST_ASSERT_EQUAL(0,return_value);
   // //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // //return_value = therapy_on();
   // TEST_ASSERT_EQUAL(VAC,current_therapy_settings.treatment.therapy_mode); //current_therapy_settings.treatment.therapy_mode = VAC;
   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   
   // wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
   // current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);


   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   // current_therapy_settings.pump.status &= ~SENSORS_OK;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status &= ~PUMP_ON;
   // //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);
// }

// // void test_therapy_on_coag (void)
// // {
// //    current_therapy_settings.wand.cut_key = 0;
// //    current_therapy_settings.footpedal.ablate = 0;
// //    current_therapy_settings.wand.coag_key = 1;
// //    current_therapy_settings.footpedal.coag = 0;
// //    current_therapy_settings.wand.coag_key =0;
// //    current_therapy_settings.footpedal.coag = 1;
// //    current_therapy_settings.wand.valid = TRUE;
// //    current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
// //    current_therapy_settings.wand.vac_key = 0;
// //    //current_therapy_settings.wand.mode_key = 0;
// //    current_therapy_settings.pump.sensor_status = 15;

// //    current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
// //    current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
// //    // Call the pump
// //    current_therapy_settings.pump.on = 1;
// //    current_therapy_settings.pump.status |= SENSORS_OK;
// //    current_therapy_settings.pump.status |= PUMP_ON;
// //    // We are expecting to call the pump twice
// //    //Pump_Config_Expect(ENABLE);
// //    //Pump_Config_Expect(RUN);

// //    uint8_t return_value = therapy_on();
// //    TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
// //    TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
// //    TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.coag_on);
// //    TEST_ASSERT_EQUAL(1,return_value);
// // }

// void test_therapy_on_coag_1 (void)
// {
	// get_priming_status_IgnoreAndReturn(0);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
   // // Make sure calls from this function do not go into the previous if statements
   // current_therapy_settings.gui.screenID = SPLASH_SCREEN;


   // // ((((current_therapy_settings.wand.coag_key == 1) && (current_therapy_settings.footpedal.coag == 0)) ||
   // //       ((current_therapy_settings.wand.coag_key == 0) && (current_therapy_settings.footpedal.coag == 1))) &&
   // //        (current_therapy_settings.footpedal.ablate == 0) && (current_therapy_settings.footpedal.vac_key == 0) &&
   // //        (current_therapy_settings.wand.cut_key == 0) && (current_therapy_settings.wand.vac_key == 0) &&
   // //        (current_therapy_settings.footpedal.mode == 0) && (current_therapy_settings.wand.mode_key == 0) &&
   // //        (current_therapy_settings.wand.valid == TRUE) && (current_therapy_settings.pump.on == 1) &&
   // //       ((current_therapy_settings.gui.screenID == RF_2000_WAND_TREATMENT_SCREEN) ||
   // //        (current_therapy_settings.gui.screenID == USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN)) &&
   // //        ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK)  && ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON))

   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.footpedal.coag = 0;

   // uint8_t return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.wand.coag_key = 1;
   // current_therapy_settings.footpedal.coag = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.wand.coag_key = 0;
   // current_therapy_settings.footpedal.coag = 1;
   // current_therapy_settings.footpedal.ablate = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.footpedal.vac_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.footpedal.vac_key = 0;
   // current_therapy_settings.wand.cut_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.wand.cut_key = 0;
   // current_therapy_settings.wand.vac_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.wand.vac_key = 0;
   // current_therapy_settings.footpedal.mode = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.footpedal.mode = 0;
   // current_therapy_settings.wand.mode_key = 1;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.wand.valid = FALSE;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.wand.valid = TRUE;
   // current_therapy_settings.pump.on = 0;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.pump.on = 1;
   // current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.pump.status &= ~SENSORS_OK;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);


   // current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   // current_therapy_settings.pump.status &= ~SENSORS_OK;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.pump.status |= SENSORS_OK;
   // current_therapy_settings.pump.status &= ~PUMP_ON;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.pump.status |= PUMP_ON;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(1,return_value);
   // TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.coag_on);

   // current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   // current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.auto_revert_back_screenID);
   // TEST_ASSERT_EQUAL(1,return_value);
// }

// void test_therapy_on_none (void)
// {
	// get_priming_status_IgnoreAndReturn(0);
	// cancel_new_profile_IgnoreAndReturn(1);
   // current_therapy_settings.wand.cut_key = 0;
   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.wand.coag_key = 1;
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.wand.coag_key =0;
   // current_therapy_settings.footpedal.coag = 1;
   // current_therapy_settings.wand.valid = TRUE;
   // current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   // current_therapy_settings.wand.vac_key = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.pump.sensor_status = 15;

   // uint8_t return_value = therapy_on();

   // TEST_ASSERT_EQUAL(0,return_value);
// }

// void test_therapy_on_none_1 (void)
// {
	// get_priming_status_IgnoreAndReturn(0);
	// cancel_new_profile_IgnoreAndReturn(1);
   // current_therapy_settings.wand.cut_key = 0;
   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.wand.coag_key = 1;
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.wand.coag_key =0;
   // current_therapy_settings.footpedal.coag = 1;
   // current_therapy_settings.wand.valid = TRUE;
   // current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   // current_therapy_settings.wand.vac_key = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.pump.sensor_status = 15;

   // current_therapy_settings.pump.on = 0;
   // current_therapy_settings.pump.status |= PUMP_DISABLED;
   // pump.state = DRIVE_STATUS;

   // uint8_t return_value = therapy_on();

   // TEST_ASSERT_EQUAL(0,return_value);
// }


// void test_therapy_on_none_2 (void)
// {
	// get_priming_status_IgnoreAndReturn(0);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);
	// cancel_new_profile_IgnoreAndReturn(1);

   // current_therapy_settings.wand.cut_key = 0;
   // current_therapy_settings.footpedal.ablate = 0;
   // current_therapy_settings.wand.coag_key = 1;
   // current_therapy_settings.footpedal.coag = 0;
   // current_therapy_settings.wand.coag_key =0;
   // current_therapy_settings.footpedal.coag = 1;
   // current_therapy_settings.wand.valid = TRUE;
   // current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   // current_therapy_settings.wand.vac_key = 0;
   // //current_therapy_settings.wand.mode_key = 0;
   // current_therapy_settings.pump.sensor_status = 15;


   // pump.state = PROTOCOL;
   // current_therapy_settings.pump.on = 0;
   // current_therapy_settings.pump.status &= ~PUMP_DISABLED;
   // uint8_t return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.pump.on = 1;
   // current_therapy_settings.pump.status |= PUMP_DISABLED;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.pump.on = 0;
   // current_therapy_settings.pump.status |= PUMP_DISABLED;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(0,return_value);

   // current_therapy_settings.pump.on = 0;
   // current_therapy_settings.pump.status |= PUMP_DISABLED;
   // pump.state = DRIVE_STATUS;
   // return_value = therapy_on();
   // TEST_ASSERT_EQUAL(MOTOR_ENABLE,pump.state);
   
// }
// (((current_therapy_settings.pump.on == 0) ||
//           ((current_therapy_settings.pump.status & PUMP_DISABLED) == PUMP_DISABLED)) &&
//            (pump.state == DRIVE_STATUS))


// CANNOT CALL rf_on() because there is a while loop without a #ifndef clause
// IT WAITS FOR THE HW TO CLEAR WHICH DOES NOT HAPPEN IN THE SIMULATOR

// void test_rf_on_else_case(void)
// {
//    FTM_SWOCTRL_REG(pSmartWandFTM)  &= ~FTM_SWOCTRL_CH0OC_MASK;
//    current_therapy_settings.treatment.ablate_on = 0;
//    uint8_t return_value = rf_on();
//    // NO put event called
//    TEST_ASSERT_EQUAL(1,return_value);
//    current_therapy_settings.treatment.ablate_on = 0;
//    current_therapy_settings.treatment.coag_on = 0;
//    return_value = rf_on();
//    TEST_ASSERT_EQUAL(1,return_value);
//    current_therapy_settings.treatment.coag_on = 1;
//    return_value = rf_on();
//    TEST_ASSERT_EQUAL(1,return_value);
//       FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
//    current_therapy_settings.treatment.coag_on = 1;
//    put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
//    FTM_SWOCTRL_REG(pSmartWandFTM) |= FTM_SWOCTRL_CH0OC_MASK;
//    return_value = rf_on();
//    // Put event is the test for this case

// }

// void test_rf_on_put_event(void)
// {

//    FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
//    current_therapy_settings.treatment.ablate_on = 1;
//    FTM_SWOCTRL_REG(pSmartWandFTM) |= FTM_SWOCTRL_CH0OC_MASK;
//    put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
//    //TEST_ASSERT_EQUAL(RF_ON,get_event(&evt_tbl));
//    uint8_t return_value = rf_on();
//    TEST_ASSERT_EQUAL(1,return_value);
// }

// void test_rf_on(void)
// {

//    FTM_FMS_REG(pSmartWandFTM) &= ~FTM_FMS_FAULTIN_MASK;
//    current_therapy_settings.treatment.ablate_on = 0;
//    FTM_SWOCTRL_REG(pSmartWandFTM) |= FTM_SWOCTRL_CH0OC_MASK;
//    // NO put event called
//    //TEST_ASSERT_EQUAL(RF_ON,get_event(&evt_tbl));
//    uint8_t return_value = rf_on();
//    TEST_ASSERT_EQUAL(1,return_value);
// }

/**
 *void test_write_sdhc_eol(void)
 *{
 *   uint8_t return_value = write_sdhc_eol();
 *   TEST_ASSERT_EQUAL(0,return_value);
 *}
 */


void test_legacy_wand_therapy_on()
{
	cancel_new_profile_IgnoreAndReturn(1);
   legacy_wand_params_obj.ablate_flag = 1;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   legacy_wand_params_obj.coag_flag = 0;
   current_therapy_settings.footpedal.mode = 0;
   legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   uint8_t return_value = legacy_wand_therapy_on ();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_legacy_wand_therapy_on_coag()
{
	cancel_new_profile_IgnoreAndReturn(1);
   legacy_wand_params_obj.ablate_flag = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.footpedal.vac_key = 0;
   legacy_wand_params_obj.coag_flag = 0;
   current_therapy_settings.footpedal.mode = 0;
   legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 1;
   uint8_t return_value = legacy_wand_therapy_on ();
   TEST_ASSERT_EQUAL(1,return_value);
   legacy_wand_params_obj.coag_enable = 0;
}


void test_legacy_wand_therapy_none()
{
	cancel_new_profile_IgnoreAndReturn(1);
   legacy_wand_params_obj.ablate_flag = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   legacy_wand_params_obj.coag_flag = 0;
   current_therapy_settings.footpedal.mode = 0;
   legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
   uint8_t return_value = legacy_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);
}


void test_legacy_wand_therapy_off ()
{
   legacy_wand_therapy_off();
   current_therapy_settings.gui.auto_revert_back_screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   TEST_ASSERT_EQUAL(LEGACY_6_8_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
}

void test_legacy_wand_therapy_off_2 ()
{
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   legacy_wand_therapy_off();
   TEST_ASSERT_EQUAL(SPLASH_SCREEN,current_therapy_settings.gui.screenID);
}

void test_legacy_wand_therapy_off_3 ()
{
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   current_therapy_settings.gui.auto_revert_back_screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   legacy_wand_therapy_off();
   
   TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN,current_therapy_settings.gui.screenID);
}

void test_lw_rf_off (void)
{
   uint8_t return_value = lw_rf_off ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_lw_rf_on (void)
{
   FTM_SWOCTRL_REG(pLegacyWandFTM)  |= FTM_SWOCTRL_CH0OC_MASK;
   // Cannot test LW_RF_ON because of the while loop.
   // SO only the RF OFF portion is being tested.
   FTM_FMS_REG(pLegacyWandFTM) |= FTM_FMS_FAULTIN_MASK;
   //FTM_FMS_REG(pLegacyWandFTM) &= ~FTM_FMS_FAULTIN_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   uint8_t return_value = lw_rf_on();
}


void test_smartwand_rf_on (void)
{
   FTM_SWOCTRL_REG(pSmartWandFTM)  |= FTM_SWOCTRL_CH0OC_MASK;
   // Cannot test LW_RF_ON because of the while loop.
   // SO only the RF OFF portion is being tested.
   FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
   //FTM_FMS_REG(pSmartWandFTM) &= ~FTM_FMS_FAULTIN_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   uint8_t return_value = rf_on();
}

void test_load_ablate_params_ent (void)
{
	current_therapy_settings.notify.status = 0;
	
	MODES therapy_mode = LO;
	load_ablate_params_ent(therapy_mode);
	
	therapy_mode = MED;
	load_ablate_params_ent(therapy_mode);
	
	therapy_mode = HI;
	load_ablate_params_ent(therapy_mode);
	
	therapy_mode = VAC;
	load_ablate_params_ent(therapy_mode);
	
	current_therapy_settings.notify.status = 1;
	load_ablate_params_ent(therapy_mode);
}


void test_load_coag_params_ent (void)
{
	COAG_LEVELS coag_level = 1;
	current_therapy_settings.notify.status = 1;
	load_coag_params_ent(coag_level);
	
}

void test_legacy_ent_wand_therapy_on (void)
{
   
   current_therapy_settings.footpedal.ablate = 1; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 0; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 1; current_therapy_settings.pump.status = 0xFFFF;
   
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   lw_get_pump_speed_IgnoreAndReturn (1);
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(1,return_value);
   
}

void test_legacy_ent_wand_therapy_on_2 (void)
{   
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 1; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 1; current_therapy_settings.pump.status = 0xFFFF;
   
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   lw_get_pump_speed_IgnoreAndReturn (1);
   pump_set_speed_CMockIgnore (1);
   legacy_wand_params_obj.coag_enable = 1;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(1,return_value);
   
}

void test_legacy_ent_wand_therapy_on_3 (void)
{   
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 1; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 1;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_ent_wand_therapy_on_4 (void)
{   
   current_therapy_settings.footpedal.ablate = 1; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 0; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 1; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = FIRMWARE_UPDATE_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 1;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_ent_wand_therapy_on_5 (void)
{   
   current_therapy_settings.footpedal.ablate = 1; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 0; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 1; current_therapy_settings.pump.status = 0xFFFF;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = FIRMWARE_UPDATE_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 1;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_ent_wand_therapy_on_6 (void)
{   
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 1; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 1; current_therapy_settings.pump.status = 0xFFFF;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = FIRMWARE_UPDATE_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 1;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_ent_wand_therapy_on_7 (void)
{   
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 1; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 1; current_therapy_settings.pump.status = 0xFFFF;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 0;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_ent_wand_therapy_on_8 (void)
{   
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 0; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 1; current_therapy_settings.pump.status = 0xFFFF;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 0;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_ent_wand_therapy_on_9 (void)
{   
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 0; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 1; current_therapy_settings.pump.status = 0xFFFF;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = FIRMWARE_UPDATE_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 0;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_ent_wand_therapy_on_10 (void)
{   
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 0; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = FIRMWARE_UPDATE_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   legacy_wand_params_obj.coag_enable = 0;
   uint8_t return_value = legacy_ent_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_sm_wand_therapy_on_1 (void)
{   
   legacy_wand_params_obj.ablate_flag = 1; current_therapy_settings.footpedal.ablate = 0;
   
   current_therapy_settings.footpedal.coag = 0; current_therapy_settings.footpedal.vac_key = 0;
   
   legacy_wand_params_obj.coag_flag = 0; current_therapy_settings.footpedal.mode = 0;

   //current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   //pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   //legacy_wand_params_obj.coag_enable = 0;
   legacy_wand_params_obj.lower_current_limit = 0;
   uint8_t return_value = legacy_sm_wand_therapy_on ();
   TEST_ASSERT_EQUAL(1,return_value);   
}

void test_legacy_sm_wand_therapy_on_2 (void)
{   
   legacy_wand_params_obj.ablate_flag = 1; current_therapy_settings.footpedal.ablate = 0;
   
   current_therapy_settings.footpedal.coag = 0; current_therapy_settings.footpedal.vac_key = 0;
   
   legacy_wand_params_obj.coag_flag = 0; current_therapy_settings.footpedal.mode = 0;

   //current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   //pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = FIRMWARE_UPDATE_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   //legacy_wand_params_obj.coag_enable = 0;
   uint8_t return_value = legacy_sm_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_sm_wand_therapy_on_3 (void)
{   
   legacy_wand_params_obj.ablate_flag = 1; current_therapy_settings.footpedal.ablate = 0;
   
   current_therapy_settings.footpedal.coag = 0; current_therapy_settings.footpedal.vac_key = 0;
   
   legacy_wand_params_obj.coag_flag = 0; current_therapy_settings.footpedal.mode = 0;
   
   legacy_wand_params_obj.mode_flag = 1;

   //current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   //pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   //legacy_wand_params_obj.coag_enable = 0;
   legacy_wand_params_obj.lower_current_limit = 0;
   uint8_t return_value = legacy_sm_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_sm_wand_therapy_on_4 (void)
{   
   legacy_wand_params_obj.coag_flag = 1; current_therapy_settings.footpedal.coag = 0;
   
   current_therapy_settings.footpedal.ablate = 0; current_therapy_settings.footpedal.vac_key = 0;
   
   legacy_wand_params_obj.ablate_flag = 0; current_therapy_settings.footpedal.mode = 0;
   
   legacy_wand_params_obj.mode_flag = 0;

   //current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   //pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   //legacy_wand_params_obj.coag_enable = 0;
   legacy_wand_params_obj.coag_enable = 0;
   uint8_t return_value = legacy_sm_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_sm_wand_therapy_on_5 (void)
{   
   legacy_wand_params_obj.coag_flag = 1; current_therapy_settings.footpedal.coag = 0;
   
   current_therapy_settings.footpedal.ablate = 0; current_therapy_settings.footpedal.vac_key = 0;
   
   legacy_wand_params_obj.ablate_flag = 0; current_therapy_settings.footpedal.mode = 0;
   
   legacy_wand_params_obj.mode_flag = 0;

   //current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   //pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   //legacy_wand_params_obj.coag_enable = 0;
   legacy_wand_params_obj.coag_enable = 1;
   legacy_wand_params_obj.lower_current_limit = 0;
   uint8_t return_value = legacy_sm_wand_therapy_on ();
   TEST_ASSERT_EQUAL(1,return_value);   
}

void test_legacy_sm_wand_therapy_on_6 (void)
{   
   legacy_wand_params_obj.coag_flag = 1; current_therapy_settings.footpedal.coag = 0;
   
   current_therapy_settings.footpedal.ablate = 0; current_therapy_settings.footpedal.vac_key = 0;
   
   legacy_wand_params_obj.ablate_flag = 0; current_therapy_settings.footpedal.mode = 0;
   
   legacy_wand_params_obj.mode_flag = 1;

   //current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   //pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   //legacy_wand_params_obj.coag_enable = 0;
   legacy_wand_params_obj.coag_enable = 1;
   legacy_wand_params_obj.lower_current_limit = 0;
   uint8_t return_value = legacy_sm_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}

void test_legacy_sm_wand_therapy_on_7 (void)
{   
   legacy_wand_params_obj.coag_flag = 1; current_therapy_settings.footpedal.coag = 0;
   
   current_therapy_settings.footpedal.ablate = 0; current_therapy_settings.footpedal.vac_key = 0;
   
   legacy_wand_params_obj.ablate_flag = 0; current_therapy_settings.footpedal.mode = 1;
   
   legacy_wand_params_obj.mode_flag = 1;

   //current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   //pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   //legacy_wand_params_obj.coag_enable = 0;
   legacy_wand_params_obj.coag_enable = 1;
   legacy_wand_params_obj.lower_current_limit = 0;
   uint8_t return_value = legacy_sm_wand_therapy_on ();
   TEST_ASSERT_EQUAL(0,return_value);   
}



void test_legacy_ent_reflex_wand_therapy_on (void)
{
   current_therapy_settings.footpedal.ablate = 1; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 0; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0;
   
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   
   uint8_t return_value = legacy_ent_reflex_wand_therapy_on ();
}

void test_legacy_ent_reflex_wand_therapy_on_2 (void)
{
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 1; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0;
   
   legacy_wand_params_obj.coag_enable = 1;
   
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   
   uint8_t return_value = legacy_ent_reflex_wand_therapy_on ();
}

void test_legacy_ent_reflex_wand_therapy_on_3 (void)
{
   current_therapy_settings.footpedal.ablate = 0; legacy_wand_params_obj.ablate_flag = 0;
   
   current_therapy_settings.footpedal.coag = 1; legacy_wand_params_obj.coag_flag = 0;
    
   current_therapy_settings.footpedal.mode = 0; legacy_wand_params_obj.mode_flag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   
   current_therapy_settings.pump.on = 0; current_therapy_settings.pump.status = 0xFFFF;
   
   legacy_wand_params_obj.coag_enable = 1;
   
   current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
   
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   
   uint8_t return_value = legacy_ent_reflex_wand_therapy_on ();
}

void test_load_coag_params_manta(void)
{
	change_flow_manta_IgnoreAndReturn(1);
	load_coag_params_manta(current_therapy_settings.setpoint.coag);
}

void test_therapy_on (void)
{
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.wand.cut_key = 1;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;
   current_therapy_settings.treatment.ablate_enable = 0;
   
   // We are expecting to call the pump twice
   //Pump_Config_Expect(ENABLE);
   //Pump_Config_Expect(RUN);
   smart_wand_params.mfg.low_enable = 1;
   smart_wand_params.mfg.vac_enable = 1;
   smart_wand_params.mfg.coag_enable =1;
 get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();

   current_therapy_settings.treatment.ablate_enable = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();

   TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
   TEST_ASSERT_EQUAL(1,return_value);

   current_therapy_settings.treatment.ablate_enable = 1;
}

void test_therapy_on_1a (void)
{
   current_therapy_settings.controller = UNKNOWN_GEN;
   // wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.wand.cut_key = 1;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;
   current_therapy_settings.treatment.ablate_enable = 0;
   
   // We are expecting to call the pump twice
   //Pump_Config_Expect(ENABLE);
   //Pump_Config_Expect(RUN);
   smart_wand_params.mfg.low_enable = 1;
   smart_wand_params.mfg.vac_enable = 1;
   smart_wand_params.mfg.coag_enable =1;
 get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();

   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.treatment.ablate_enable = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();

   TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
   TEST_ASSERT_EQUAL(1,return_value);

   current_therapy_settings.treatment.ablate_enable = 1;
   current_therapy_settings.controller = SPORTS_MED;
}

void test_therapy_on_RF2000 (void)
{
   //   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.wand.cut_key = 1;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;

   // We are expecting to call the pump twice
   //Pump_Config_Expect(ENABLE);
   //Pump_Config_Expect(RUN);
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);

   uint8_t return_value = therapy_on();


   TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
   TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
   TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.pump.ctrl_en);
   TEST_ASSERT_EQUAL(1,return_value);
   
   current_therapy_settings.controller = SPORTS_MED;

}

void test_therapy_on_0 (void)
{
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;
   
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();
   current_therapy_settings.pump.status &= ~PUMP_ON;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   current_therapy_settings.gui.screenID = ERROR_SCREEN;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   current_therapy_settings.pump.on = 0;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   current_therapy_settings.wand.valid = FALSE;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   current_therapy_settings.wand.mode_key = 1;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   current_therapy_settings.footpedal.mode = 1;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   
   current_therapy_settings.controller = SPORTS_MED;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;
}

void test_therapy_on_1 (void)
{
   current_therapy_settings.wand.cut_key = 1;
   current_therapy_settings.footpedal.ablate = 1;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);

   uint8_t return_value = therapy_on();

   TEST_ASSERT_EQUAL(0,return_value);
}

void test_therapy_on_2 (void)
{
	current_therapy_settings.controller = UNKNOWN_GEN;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.wand.cut_key = 1;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;
   current_therapy_settings.treatment.ablate_enable = 0;
   
   // We are expecting to call the pump twice
   //Pump_Config_Expect(ENABLE);
   //Pump_Config_Expect(RUN);
   smart_wand_params.mfg.low_enable = 1;
   smart_wand_params.mfg.vac_enable = 1;
   smart_wand_params.mfg.coag_enable = 1;

   current_therapy_settings.treatment.ablate_enable = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();

   TEST_ASSERT_EQUAL(1, current_therapy_settings.gui.auto_revert_back_screenID);
   TEST_ASSERT_EQUAL(0, return_value);

   current_therapy_settings.treatment.ablate_enable = 1;
   current_therapy_settings.controller = SPORTS_MED;
}

void test_therapy_on_3 (void)
{
   //   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.wand.cut_key = 1;
   current_therapy_settings.footpedal.ablate = 1;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;

   // We are expecting to call the pump twice
   //Pump_Config_Expect(ENABLE);
   //Pump_Config_Expect(RUN);
   // wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   // wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();


   TEST_ASSERT_EQUAL(1, current_therapy_settings.gui.auto_revert_back_screenID);
   TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
   TEST_ASSERT_EQUAL(FALSE,current_therapy_settings.pump.ctrl_en);
   TEST_ASSERT_EQUAL(0,return_value);
   
   current_therapy_settings.controller = SPORTS_MED;

}

void test_therapy_on_4 (void)
{
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.footpedal.ablate = 1;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);

   uint8_t return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.footpedal.mode = 0;
   current_therapy_settings.wand.mode_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = FALSE;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.pump.on = 0;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status &= ~PUMP_ON;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_therapy_on_vac (void)
{
   current_therapy_settings.controller = SPORTS_MED;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.wand.vac_key = 1;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.pump.sensor_status = 15;

   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;

//current_therapy_settings.gui.screenID == USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   // We are expecting to call the pump twice
   //Pump_Config_Expect(ENABLE);
   //Pump_Config_Expect(RUN);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);

   uint8_t return_value = therapy_on();




   TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
   TEST_ASSERT_EQUAL(TRUE,current_therapy_settings.pump.ctrl_en);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_therapy_on_vac_1 (void)
{
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.controller = SPORTS_MED;
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.vac_key = 1;
   current_therapy_settings.footpedal.mode = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.wand.vac_key = 1;
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.pump.sensor_status = 15;

   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;

//current_therapy_settings.gui.screenID == USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   // Call the pump
   current_therapy_settings.pump.on = 1;
   // We are expecting to call the pump twice
   //Pump_Config_Expect(ENABLE);
   //Pump_Config_Expect(RUN);
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);

   current_therapy_settings.footpedal.vac_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.footpedal.mode = 1;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   current_therapy_settings.footpedal.vac_key = 1;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.wand.mode_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.footpedal.mode = 0;
   current_therapy_settings.footpedal.coag = 1;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.footpedal.ablate = 1;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.wand.coag_key = 1;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.wand.cut_key = 1;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.wand.valid = FALSE;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.pump.on = 0;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status |= PUMP_ON;
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   //TEST_ASSERT_EQUAL(0,return_value);
   //current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   //return_value = therapy_on();
   TEST_ASSERT_EQUAL(VAC,current_therapy_settings.treatment.therapy_mode); //current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   
   wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);


   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status &= ~PUMP_ON;
   //wand_thandle_wear_monitor_ExpectAndReturn (0,0,1,1);
   //wand_irms_wear_monitor_ExpectAndReturn (0,1,1);
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);
}

// void test_therapy_on_coag (void)
// {
//    current_therapy_settings.wand.cut_key = 0;
//    current_therapy_settings.footpedal.ablate = 0;
//    current_therapy_settings.wand.coag_key = 1;
//    current_therapy_settings.footpedal.coag = 0;
//    current_therapy_settings.wand.coag_key =0;
//    current_therapy_settings.footpedal.coag = 1;
//    current_therapy_settings.wand.valid = TRUE;
//    current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
//    current_therapy_settings.wand.vac_key = 0;
//    //current_therapy_settings.wand.mode_key = 0;
//    current_therapy_settings.pump.sensor_status = 15;

//    current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
//    current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
//    // Call the pump
//    current_therapy_settings.pump.on = 1;
//    current_therapy_settings.pump.status |= SENSORS_OK;
//    current_therapy_settings.pump.status |= PUMP_ON;
//    // We are expecting to call the pump twice
//    //Pump_Config_Expect(ENABLE);
//    //Pump_Config_Expect(RUN);

//    uint8_t return_value = therapy_on();
//    TEST_ASSERT_EQUAL(USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN, current_therapy_settings.gui.auto_revert_back_screenID);
//    TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.screenID);
//    TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.coag_on);
//    TEST_ASSERT_EQUAL(1,return_value);
// }

void test_therapy_on_coag_1 (void)
{
   // Make sure calls from this function do not go into the previous if statements
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;


   // ((((current_therapy_settings.wand.coag_key == 1) && (current_therapy_settings.footpedal.coag == 0)) ||
   //       ((current_therapy_settings.wand.coag_key == 0) && (current_therapy_settings.footpedal.coag == 1))) &&
   //        (current_therapy_settings.footpedal.ablate == 0) && (current_therapy_settings.footpedal.vac_key == 0) &&
   //        (current_therapy_settings.wand.cut_key == 0) && (current_therapy_settings.wand.vac_key == 0) &&
   //        (current_therapy_settings.footpedal.mode == 0) && (current_therapy_settings.wand.mode_key == 0) &&
   //        (current_therapy_settings.wand.valid == TRUE) && (current_therapy_settings.pump.on == 1) &&
   //       ((current_therapy_settings.gui.screenID == RF_2000_WAND_TREATMENT_SCREEN) ||
   //        (current_therapy_settings.gui.screenID == USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN)) &&
   //        ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK)  && ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON))

   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.controller = SPORTS_MED;
get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.coag_key = 1;
   current_therapy_settings.footpedal.coag = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.footpedal.ablate = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.vac_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.cut_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.wand.vac_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.footpedal.mode = 0;
   current_therapy_settings.wand.mode_key = 1;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = FALSE;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.pump.on = 0;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.on = 1;
   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);


   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status &= ~PUMP_ON;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.status |= PUMP_ON;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(1,return_value);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.coag_on);

   current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(RF_2000_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.auto_revert_back_screenID);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_therapy_on_coag_Manta (void)
{
   // Make sure calls from this function do not go into the previous if statements
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.controller = SPORTS_MED;
	get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.coag_key = 1;
   current_therapy_settings.footpedal.coag = 1;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.coag_key = 0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.footpedal.ablate = 1;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.footpedal.vac_key = 1;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.footpedal.vac_key = 0;
   current_therapy_settings.wand.cut_key = 1;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.wand.vac_key = 1;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.footpedal.mode = 1;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.footpedal.mode = 0;
   current_therapy_settings.wand.mode_key = 1;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.wand.valid = FALSE;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.pump.on = 0;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.on = 1;
   current_therapy_settings.gui.screenID = MANTA_SMART_WAND_TREATMENT_SCREEN;
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);


   current_therapy_settings.gui.screenID = USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN;
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.status |= SENSORS_OK;
   current_therapy_settings.pump.status &= ~PUMP_ON;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.status |= PUMP_ON;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(1,return_value);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.coag_on);

   current_therapy_settings.gui.screenID = MANTA_SMART_WAND_TREATMENT_SCREEN;
   current_therapy_settings.gui.auto_revert_back_screenID = SPLASH_SCREEN;
   current_therapy_settings.treatment.pre_prime = FALSE;
   get_priming_status_IgnoreAndReturn(0);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(MANTA_SMART_WAND_TREATMENT_SCREEN,current_therapy_settings.gui.auto_revert_back_screenID);
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_therapy_on_turbo_Manta (void)
{
	current_therapy_settings.wand.coag_key = 0;
	current_therapy_settings.footpedal.coag = 0;
	current_therapy_settings.footpedal.ablate = 0;
	current_therapy_settings.footpedal.vac_key = 0;
	current_therapy_settings.wand.vac_key = 0;
	current_therapy_settings.wand.cut_key = 0;
	current_therapy_settings.footpedal.mode = 0;
   current_therapy_settings.wand.mode_key = 1;
   current_therapy_settings.wand.mode_key = 1;
	gui_stop_auto_prime_Ignore();
	get_priming_status_IgnoreAndReturn(1);
	cancel_new_profile_IgnoreAndReturn(1);
	change_flow_manta_IgnoreAndReturn(1);
    uint8_t return_value = therapy_on();
}

void test_therapy_on_none (void)
{
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.wand.coag_key = 1;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.wand.coag_key =0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.wand.vac_key = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.pump.sensor_status = 15;
	get_priming_status_IgnoreAndReturn(2);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();

   TEST_ASSERT_EQUAL(0,return_value);
}

void test_therapy_on_none_1 (void)
{
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.wand.coag_key = 1;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.wand.coag_key =0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.wand.vac_key = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.pump.sensor_status = 15;

   current_therapy_settings.pump.on = 0;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   pump.state = DRIVE_STATUS;
	get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();

   TEST_ASSERT_EQUAL(0,return_value);
}

void test_therapy_on_none_1a (void)
{
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.wand.coag_key = 1;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.wand.coag_key =0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.wand.vac_key = 0;
   current_therapy_settings.pump.sensor_status = 15;
   current_therapy_settings.pump.on = 0;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   current_therapy_settings.pump.status &= ~SENSORS_OK;
   pump.state = DRIVE_STATUS;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();
}



void test_therapy_on_none_2 (void)
{
   current_therapy_settings.wand.cut_key = 0;
   current_therapy_settings.footpedal.ablate = 0;
   current_therapy_settings.wand.coag_key = 1;
   current_therapy_settings.footpedal.coag = 0;
   current_therapy_settings.wand.coag_key =0;
   current_therapy_settings.footpedal.coag = 1;
   current_therapy_settings.wand.valid = TRUE;
   current_therapy_settings.gui.screenID = SPLASH_SCREEN;
   current_therapy_settings.wand.vac_key = 0;
   //current_therapy_settings.wand.mode_key = 0;
   current_therapy_settings.pump.sensor_status = 15;


   pump.state = PROTOCOL;
   current_therapy_settings.pump.on = 0;
   current_therapy_settings.pump.status &= ~PUMP_DISABLED;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   uint8_t return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.on = 0;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(0,return_value);

   current_therapy_settings.pump.on = 0;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   pump.state = DRIVE_STATUS;
   get_priming_status_IgnoreAndReturn(0);
 cancel_new_profile_IgnoreAndReturn(1);
   return_value = therapy_on();
   TEST_ASSERT_EQUAL(17,pump.state);
   
}

void test_rf_on_elseif_loop (void)
{
   current_therapy_settings.setpoint.dac = 1;
   
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   FTM_SWOCTRL_REG(pSmartWandFTM)  |= FTM_SWOCTRL_CH0OC_MASK;
   FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
   current_therapy_settings.wand.tube_warn = 1;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   uint8_t return_value = rf_on(); 
   
   FTM_SWOCTRL_REG(pSmartWandFTM)  |= FTM_SWOCTRL_CH0OC_MASK;
   FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.wand.tube_warn = 0;
   return_value = rf_on(); 
   
   
   
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.treatment.coag_on = 0;
   FTM_SWOCTRL_REG(pSmartWandFTM)  &= ~FTM_SWOCTRL_CH0OC_MASK;
   FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
   return_value = rf_on(); 

   FTM_SWOCTRL_REG(pSmartWandFTM)  |= FTM_SWOCTRL_CH0OC_MASK;
   FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   return_value = rf_on();   
}

void test_rf_on_if_loop (void)
{
	current_therapy_settings.setpoint.dac = 0;
	FTM_SWOCTRL_REG(pSmartWandFTM)  |= FTM_SWOCTRL_CH0OC_MASK;
    FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
	pSmartWandFTM->FMS = 0;
    unitTest = TRUE;
	put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
	uint8_t return_value = rf_on();
	
	current_therapy_settings.setpoint.dac = 1;
	FTM_SWOCTRL_REG(pSmartWandFTM)  |= FTM_SWOCTRL_CH0OC_MASK;
    FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
	pSmartWandFTM->FMS = 0;
	return_value = rf_on();
	
	FTM_SWOCTRL_REG(pSmartWandFTM)  &= ~FTM_SWOCTRL_CH0OC_MASK;
    FTM_FMS_REG(pSmartWandFTM) |= FTM_FMS_FAULTIN_MASK;
	return_value = rf_on();
	
	FTM_FMS_REG(pSmartWandFTM) &= ~FTM_FMS_FAULTIN_MASK;
	return_value = rf_on();
	
	
}

//HMS
/* void test_lw_rf_on (void)
{
   FTM_SWOCTRL_REG(pLegacyWandFTM)  |= FTM_SWOCTRL_CH0OC_MASK;
   // Cannot test LW_RF_ON because of the while loop.
   // SO only the RF OFF portion is being tested.
   FTM_FMS_REG(pLegacyWandFTM) |= FTM_FMS_FAULTIN_MASK;
   //FTM_FMS_REG(pLegacyWandFTM) &= ~FTM_FMS_FAULTIN_MASK;
   pLegacyWandFTM->FMS = 0;
   pLegacyWandFTM->SYNC |= FTM_SYNC_SWSYNC_MASK;
   current_therapy_settings.treatment.ablate_on = 1;
   uint8_t return_value = lw_rf_on();
   
   current_therapy_settings.legacy_wand_levels.dac = 1;
   pLegacyWandFTM->FMS = 1;
   pLegacyWandFTM->SWOCTRL = 1;
   put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   return_value = lw_rf_on();
   
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 1;
   pLegacyWandFTM->SWOCTRL = 0;
   //put_event_ExpectAndReturn(&evt_tbl,RF_ON,1);
   return_value = lw_rf_on();
   
} */

void test_therapy_start_screen (void)
{
	current_therapy_settings.gui.screenID = DELETE_INDIVIDUAL_USER_PROFILE_CHANCE_TO_BACK_OUT_SCREEN;
	uint8_t return1 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return1);
	
	current_therapy_settings.gui.screenID = SET_LEVEL_SCREEN ;
	uint8_t return2 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return2);
	
	current_therapy_settings.gui.screenID = MAIN_SETTINGS_SCREEN ;
	uint8_t return3 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return3);
	
	current_therapy_settings.gui.screenID = PROFILE_SAVE_SCREEN ;
	uint8_t return4 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return4);
	
	current_therapy_settings.gui.screenID = USER_PROFILES_SCREEN ;
	uint8_t return5 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return5);
	
	current_therapy_settings.gui.screenID = ENTER_NEW_USER_PROFILE_NAME_SCREEN ;
	uint8_t return6 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return6);
	
	current_therapy_settings.gui.screenID = SWITCHING_SETTINGS_SCREEN  ;
	uint8_t return7 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return7);
	
	current_therapy_settings.gui.screenID = LANGUAGES_SCREEN  ;
	uint8_t return8 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return8);
	
	current_therapy_settings.gui.screenID = WAND_INFO_SCREEN  ;
	uint8_t return9 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return9);
	
	current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN ;
	uint8_t return10 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return10);
	
	current_therapy_settings.gui.screenID = ENT_SMART_WAND_TREATMENT_SCREEN  ;
	uint8_t return11 = therapy_start_screen (current_therapy_settings.gui.screenID);
	TEST_ASSERT_EQUAL(1,return11);
}



void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
