#include "unity.h"
#include "MK26F18.h"
#include "common.h"
#include "pump.h"
#include <stdio.h>
#include "system.h"
#include "Mockevent_buffer.h"
#include "Mockpit.h"
#include "cpu.h"
#include "Mockbuck.h"
#include "Mockrf.h"
#include "Mockartc_arm_math.h"
#include "Mocksensors.h"
#include "Mocknotifications.h"
#include "Mockgui.h"

// #ifdef DEBUG_CAN
// #include "Mockcan_debug.h"
// #endif
#include <string.h>
// GEN_SETTINGS CurrentTherapySettings;
// GEN_SETTINGS *const pCurrentTherapySettings = &CurrentTherapySettings;
// event_table evt_tbl;
// SMART_WAND_PARAMETERS SmartWandParams;
// SMART_WAND_PARAMETERS *const smart_wand_params. = &SmartWandParams;
const static CAN_MemMapPtr pFlexCAN = CAN0_BASE_PTR;
// DEBUG CANdebug;
// volatile DEBUG *pCANdebug = &CANdebug;
extern PUMP_CMD pump;
extern SPEED_CTRL_STATES spd_ctrl_state;

#define TEST

void setUp(void)
{
}

void tearDown(void)
{
}

void test_PumpSendCMD (void)
{
   //TEST_FAIL_MESSAGE("dummy test");
}



void test_PumpFIR_Init (void)
{
  q15_t init_value = 0;
  uint16_t numTaps = 8;
  int8_t postShift = 0;
  static q15_t state[8];
  uint32_t blockSize = 1;
  arm_status status;
  arm_fir_instance_q15 fir;
  arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
  PumpFIR_Init(init_value);
  arm_fir_init_q15_IgnoreAndReturn(10);
  PumpFIR_Init(init_value);

}

void arm_fir_q15_callback (const arm_fir_instance_q15 *a, q15_t *b,q15_t *c, uint32_t d, int Numcalls)
{
   switch(Numcalls)
   {
   case 0:
      {
         *c = 1;
         break;
      }
   case 1:
      {
         *c = 2830;
         break;
      }
   case 2:
      {
         *c = 5213;
         break;
      }
   default:
      {
         *c = 5213;
         break;
      }
   }
}



void test_PumpPID1(void)
{
   q15_t pv = 32767;
   bool x = TRUE ;
   q15_t filt_pv_q15 = 0;
   float32_t filt_pv_f32 = 0;
   uint32_t blockSize = 1;
   arm_fir_instance_q15 fir;
   arm_fir_q15_StubWithCallback(arm_fir_q15_callback);
   current_therapy_settings.pump.pid.e1 = 1; // setting it to some arbitary value
   current_therapy_settings.pump.pid.e2 = 1.5;
   current_therapy_settings.pump.pid.sp_amps = 2; // setting it to some arbitary value
   
   float32_t return_value = PumpPID(pv,x);
   return_value = PumpPID(pv,x);
   return_value = PumpPID(pv,x);
   //current_therapy_settings.pump.pid.sp_amps = 10; // setting it to some arbitary value
   current_therapy_settings.pump.pid.kp3 = 0x10000;
   current_therapy_settings.pump.pid.ki3 = 0x00;
   
   return_value = PumpPID(pv,x);
   current_therapy_settings.pump.pid.kp1 = 0x10000;
   current_therapy_settings.pump.pid.ki1 = 0x0;
   current_therapy_settings.pump.pid.max_rps = 0x64;
   current_therapy_settings.pump.pid.min_rps = 0x10;
   return_value = PumpPID(pv,x);
   current_therapy_settings.pump.pid.kp1 = 0x1;
   current_therapy_settings.pump.pid.ki1 = 0x0;
   current_therapy_settings.pump.pid.max_rps = 0x64;
   current_therapy_settings.pump.pid.min_rps = 0x10;

   current_therapy_settings.pump.pid.kp1 = 0x1;
   current_therapy_settings.pump.pid.ki1 = 0x1;
   return_value = PumpPID(pv,FALSE);
   TEST_ASSERT_EQUAL(0x10,current_therapy_settings.pump.pid.min_rps);
   current_therapy_settings.pump.pid.kp1 = 0x1;
   current_therapy_settings.pump.pid.ki1 = 0x0;
   return_value = PumpPID(pv,FALSE);
   current_therapy_settings.pump.pid.kp1 = 0x1;
   current_therapy_settings.pump.pid.ki1 = 0x1;
   return_value = PumpPID(pv,FALSE);
   current_therapy_settings.pump.pid.kp1 = 0x0;
   current_therapy_settings.pump.pid.ki1 = 0x1;
   return_value = PumpPID(pv,FALSE);
   pump.idx = 0;
   pump.cmd[pump.idx].data[0] = '\0';
   return_value = PumpPID(pv,FALSE);

   // current_therapy_settings.pump.pid.kp2 = 0x0;
   // current_therapy_settings.pump.pid.ki2 = 0x10;
   // current_therapy_settings.pump.pid.kp1 = 0x0;
   // current_therapy_settings.pump.pid.ki2 = 0x10;
   // return_value = PumpPID(pv,x);
}

// void test_PumpPID2(void)
// {
//    q15_t pv = 32767;
//    bool x = TRUE ;
//    q15_t filt_pv_q15 = 0;
//    float32_t filt_pv_f32 = 0;
//    uint32_t blockSize = 1;
//    arm_fir_instance_q15 fir;
//    arm_fir_q15_StubWithCallback(arm_fir_q15_callback);
//    current_therapy_settings.pump.pid.e1 = 1; // setting it to some arbitary value
//    current_therapy_settings.pump.pid.e2 = 1.5;
//    current_therapy_settings.pump.pid.sp_amps = 2; // setting it to some arbitary value
   
//    float32_t return_value = PumpPID(pv,x);
// }

// void test_PumpPID3(void)
// {
//    q15_t pv = 32767;
//    bool x = TRUE ;
//    q15_t filt_pv_q15 = 0;
//    float32_t filt_pv_f32 = 0;
//    uint32_t blockSize = 1;
//    arm_fir_instance_q15 fir;
//    arm_fir_q15_StubWithCallback(arm_fir_q15_callback);
//    current_therapy_settings.pump.pid.e1 = 1; // setting it to some arbitary value
//    current_therapy_settings.pump.pid.e2 = 1.5;
//    current_therapy_settings.pump.pid.sp_amps = 2; // setting it to some arbitary value

//    float32_t return_value = PumpPID(pv,x);
// }

void test_pump_inactivity_monitor (void)
{
   uint8_t return_value = pump_inactivity_monitor();
}

void test_pump_speed_ctrl (void){
	GenTypeDef Temp1 = current_therapy_settings.controller;
	GenTypeDef Temp2 = current_therapy_settings.active_wand.smart_wand;
	current_therapy_settings.controller = ENT;
	current_therapy_settings.active_wand.smart_wand = ENT;
	uint8_t return_value = pump_speed_ctrl();
	current_therapy_settings.controller = Temp1;
	current_therapy_settings.active_wand.smart_wand = Temp2;
	
}

void test_pump_speed_ctrl_VAC (void)
{
   current_therapy_settings.treatment.therapy_mode = VAC;
   uint8_t return_value =  pump_speed_ctrl();
}

void test_pump_speed_ctrl_ablate_on (void)
{
   current_therapy_settings.treatment.ablate_on = 1;
   uint8_t return_value =  pump_speed_ctrl();
}

void test_pump_speed_ctrl_QUIESCENT(void)
{
   // 2, 1, 0
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = OFF;
   current_therapy_settings.pump.quiescent_delay = 0x10;
   uint8_t return_value = pump_speed_ctrl();
}

void test_pump_speed_ctrl_QUIESCENT_1(void)
{
   // 2, 1, 0
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = OFF;
   current_therapy_settings.pump.quiescent_delay = 0x00;
   put_event_ExpectAndReturn(&evt_tbl, MEASURE_IMPEDANCE,1);
   uint8_t return_value = pump_speed_ctrl();
}


void test_pump_speed_ctrl_OFF(void)
{
   // 0,0,0
   current_therapy_settings.treatment.therapy_mode = NONE;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.pump.ctrl_en = 0;
   spd_ctrl_state = OFF;
   current_therapy_settings.pump.quiescent_delay = 0x10;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_STARTING(void)
{
   // 1,1,1 in the state table
   current_therapy_settings.treatment.therapy_mode = NONE;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = QUIESCENT;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   arm_fir_q15_Ignore();
   pump.state = CONTROL;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_STARTING_1(void)
{
   // 1,1,1 in the state table
   current_therapy_settings.treatment.therapy_mode = NONE;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = QUIESCENT;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = IDLE_SPEED;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_ON(void)
{
   spd_ctrl_state = STARTING;
   pump.state = CONTROL;
   arm_fir_q15_Ignore();
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_ON_1(void)
{
   spd_ctrl_state = STARTING;
   pump.state = IDLE_SPEED;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_VAC_START_1(void)
{
   // 2,1,1 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = QUIESCENT;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = IDLE_SPEED;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_PULSING_DELAY(void)
{
   // 2,1,5 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = VAC_START;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = IDLE_SPEED;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_PULSING_DELAY_1(void)
{
   // 2,1,5 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = VAC_START;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = IDLE_SPEED;
   current_therapy_settings.pump.vac.pulse_delay = 2;
   uint8_t return_value = pump_speed_ctrl();
   TEST_ASSERT_EQUAL(PEAK_START,spd_ctrl_state);
}

void test_pump_speed_ctrl_PULSING_DELAY_2(void)
{
   // 2,1,5 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = VAC_START;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = CONTROL;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_PEAK_DELAY_0(void)
{
   // 2,1,8 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = PEAK_START;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = CONTROL;
   arm_fir_q15_Ignore();
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_PEAK_DELAY_1(void)
{
   //Call VAC_START to reset the counter
   // 2,1,1 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = QUIESCENT;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = IDLE_SPEED;
   uint8_t return_value = pump_speed_ctrl();

   // 2,1,8 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   spd_ctrl_state = PEAK_START;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   current_therapy_settings.pump.vac.peak_time = 0x01;
   pump.state = IDLE_SPEED;
   return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_PEAK_DELAY_2(void)
{
   // 2,1,8 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   current_therapy_settings.pump.vac.peak_time = 0x0200;
   spd_ctrl_state = PEAK_START;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = CONTROL;
   arm_fir_q15_Ignore();
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_VAC_CTRL(void)
{
   // 2,1,9 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   current_therapy_settings.pump.vac.peak_time = 0x0200;
   spd_ctrl_state = VAC_CTRL;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = CONTROL;
   arm_fir_q15_Ignore();
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_VAC_CTRL_2(void)
{
   // 2,1,9 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   smart_wand_params.vac.max_time = 0x02;
   spd_ctrl_state = VAC_CTRL;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = IDLE_SPEED;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_VAC_CTRL_3(void)
{
   // 2,1,9 in the state table
   current_therapy_settings.treatment.therapy_mode = VAC;
   current_therapy_settings.pump.ctrl_en = 1;
   smart_wand_params.vac.max_time = 0x0200;
   spd_ctrl_state = VAC_CTRL;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = IDLE_SPEED;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_COAG_MODE(void)
{
   // 0,1,1 in the state table
   current_therapy_settings.treatment.therapy_mode = NONE;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.pump.ctrl_en = 1;
   smart_wand_params.vac.max_time = 0x0200;
   spd_ctrl_state = QUIESCENT;
   arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
   pump.state = IDLE_SPEED;
   uint8_t return_value = pump_speed_ctrl();

}

void test_pump_speed_ctrl_MANTA0(void)
{
    GenTypeDef Temp1 = current_therapy_settings.controller;
	GenTypeDef Temp2 = current_therapy_settings.active_wand.smart_wand;
	
	current_therapy_settings.pump.ctrl_en = 0;
	current_therapy_settings.active_wand.legacy_wand = ENT;
	current_therapy_settings.active_wand.smart_wand = 0;
	arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
	uint8_t return_value = pump_speed_ctrl();
	current_therapy_settings.active_wand.legacy_wand = 0;
	current_therapy_settings.controller = Temp1;
	current_therapy_settings.active_wand.smart_wand = Temp2;
}

void test_pump_speed_ctrl_MANTA1(void)
{
	GenTypeDef Temp1 = current_therapy_settings.controller;
	GenTypeDef Temp2 = current_therapy_settings.active_wand.smart_wand;
	
	current_therapy_settings.pump.ctrl_en = 0;
	current_therapy_settings.active_wand.smart_wand = ARTHROPLASTY;
	arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
	uint8_t return_value = pump_speed_ctrl();
	current_therapy_settings.controller = Temp1;
	current_therapy_settings.active_wand.smart_wand = Temp2;
	
}

void test_pump_speed_ctrl_MANTA2(void)
{
	GenTypeDef Temp1 = current_therapy_settings.controller;
	GenTypeDef Temp2 = current_therapy_settings.active_wand.smart_wand;
	
	current_therapy_settings.pump.ctrl_en = 0;
	current_therapy_settings.controller = SPORTS_MED;
	current_therapy_settings.active_wand.smart_wand = ENT;
	arm_fir_init_q15_IgnoreAndReturn(ARM_MATH_SUCCESS);
	uint8_t return_value = pump_speed_ctrl();
	current_therapy_settings.controller = Temp1;
	current_therapy_settings.active_wand.smart_wand = Temp2;
}

void test_pump_speed_ctrl_MANTA3(void)
{
	GenTypeDef Temp1 = current_therapy_settings.controller;
	GenTypeDef Temp2 = current_therapy_settings.active_wand.smart_wand;
	
	current_therapy_settings.controller = SPORTS_MED;
	current_therapy_settings.active_wand.smart_wand = ARTHROPLASTY;
	current_therapy_settings.pump.ctrl_en = 1;
    spd_ctrl_state = OFF;
	uint8_t return_value = pump_speed_ctrl();

	current_therapy_settings.controller = Temp1;
	current_therapy_settings.active_wand.smart_wand = Temp2;

}


void test_PumpUART4_Init (void)
{
   BAUD_RATE bps = BAUD_9600;
   BAUD_RATE bps1 = BAUD_38400;
   BAUD_RATE bps2 = MAX_BAUD_SETTINGS;
   PumpUART4_Init(bps);
   PumpUART4_Init(bps1);
   PumpUART4_Init(bps2);
}

// void test_pump_inactivity_monitor (void)
// {
//    uint8_t return_value = pump_inactivity_monitor();
// }


void test_send_pump_cmd(void)
{
   send_pump_cmd();
}

void test_send_pump_cmd_1(void)
{
   //UART4_S1 |= UART_S1_TC_MASK;
   pump.idx = 0;
   pump.cmd[0].size = 12;
   for (uint8_t i; i < 12; i++)
   {
      send_pump_cmd();
      pump.idx = 1;
   }
   pump.cmd[0].size = 9;
   send_pump_cmd();
}

// void test_receive_pump_cmd (void)
// {
//    uint8_t return_value =  receive_pump_cmd();
// }

void test_pump_packetizer (void)
{
   pump.idx = 14;
   pump.attempts = 2;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(0,pump.state);
}


void test_pump_packetizer_CHANGE_BAUD(void)
{
   pump.idx =12;
   pump.state = CHANGE_BAUD;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(CMD_MODE,pump.state);
}

void test_pump_packetizer_PROTOCOL_16(void)
{
   pump.idx =12;
   pump.state = PROTOCOL;
   pump.attempts = 16;
   uint8_t return_value = pump_packetizer();
}

void test_pump_packetizer_PROTOCOL_7(void)
{
   pump.idx =12;
   pump.state = PROTOCOL;
   pump.attempts = 7;
   uint8_t return_value = pump_packetizer();
}

void test_pump_packetizer_MOTOR_DISABLE(void)
{
   pump.idx =12;
   pump.state = MOTOR_DISABLE;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_MOTOR_DISABLE_initial_attempts(void)
{
   pump.idx =12;
   pump.state = MOTOR_DISABLE;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(MOTOR_DISABLE,pump.state);
}

void test_pump_packetizer_MAX_CURRENT(void)
{
   pump.idx =12;
   pump.state = MAX_CURRENT;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_MAX_CURRENT_initial_attempts(void)
{
   pump.idx =12;
   pump.state = MAX_CURRENT;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(MAX_CURRENT,pump.state);
}

void test_pump_packetizer_CONT_CURRENT(void)
{
   pump.idx =12;
   pump.state = CONT_CURRENT;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_CONT_CURRENT_initial_attempts(void)
{
   pump.idx =12;
   pump.state = CONT_CURRENT;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(CONT_CURRENT,pump.state);
}

void test_pump_packetizer_REQUEST_PWR_MODE(void)
{
   pump.idx =12;
   pump.state = REQUEST_PWR_MODE;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(PROTOCOL,pump.state);
   //TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_REQUEST_PWR_MODE_2(void)
{
   pump.idx =12;
   pump.state = REQUEST_PWR_MODE;
   pump.attempts = 4;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(REQUEST_PWR_MODE,pump.state);
}


void test_pump_packetizer_CMD_MODE(void)
{
   pump.idx =12;
   pump.state = CMD_MODE;
   pump.attempts = 16;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_CMD_MODE_2(void)
{
   pump.idx =12;
   pump.state = CMD_MODE;
   pump.attempts = 4;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(CMD_MODE,pump.state);
}


void test_pump_packetizer_AR_FILT_FREQ(void)
{
   pump.idx =12;
   pump.state = AR_FILT_FREQ;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_AR_FILT_FREQ_2(void)
{
   pump.idx =12;
   pump.state = AR_FILT_FREQ;
   pump.attempts = 4;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(AR_FILT_FREQ,pump.state);
}

void test_pump_packetizer_AR_FILT_GAIN(void)
{
   pump.idx =12;
   pump.state = AR_FILT_GAIN;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_AR_FILT_GAIN_2(void)
{
   pump.idx =12;
   pump.state = AR_FILT_GAIN;
   pump.attempts = 4;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(AR_FILT_GAIN,pump.state);
}

void test_pump_packetizer_PROBE_MOTOR(void)
{
   pump.idx =12;
   pump.state = PROBE_MOTOR;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_PROBE_MOTOR_2(void)
{
   pump.idx =12;
   pump.state = PROBE_MOTOR;
   pump.attempts = 4;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(PROBE_MOTOR,pump.state);
}

void test_pump_packetizer_DELAY_TIME(void)
{
   pump.idx =12;
   pump.state = DELAY_TIME;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_DELAY_TIME_initial_attempts(void)
{
   pump.idx =12;
   pump.state = DELAY_TIME;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(DELAY_TIME,pump.state);
}

void test_pump_packetizer_IDLE_CURRENT(void)
{
   pump.idx =12;
   pump.state = IDLE_CURRENT;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_IDLE_CURRENT_initial_attempts(void)
{
   pump.idx =12;
   pump.state = IDLE_CURRENT;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(IDLE_CURRENT,pump.state);
}

// void test_pump_packetizer_ANTI_RES_FILT_FREQ(void)
// {
//    pump.idx =12;
//    pump.state = ANTI_RES_FILT_FREQ;
//    pump.attempts = 6;
//    uint8_t return_value = pump_packetizer();
//    TEST_ASSERT_EQUAL(CHANGE_BAUD,pump.state);
// }

// void test_pump_packetizer_ANTI_RES_FILT_FREQ_initial_attempts(void)
// {
//    pump.idx =12;
//    pump.state = ANTI_RES_FILT_FREQ;
//    pump.attempts = 0;
//    uint8_t return_value = pump_packetizer();
//    TEST_ASSERT_EQUAL(ANTI_RES_FILT_FREQ,pump.state);
// }

// void test_pump_packetizer_ANTI_RES_FILT_GAIN(void)
// {
//    pump.idx =12;
//    pump.state = ANTI_RES_FILT_GAIN;
//    pump.attempts = 6;
//    uint8_t return_value = pump_packetizer();
//    TEST_ASSERT_EQUAL(CHANGE_BAUD,pump.state);
// }

// void test_pump_packetizer_ANTI_RES_FILT_GAIN_initial_attempts(void)
// {
//    pump.idx =12;
//    pump.state = ANTI_RES_FILT_GAIN;
//    pump.attempts = 0;
//    uint8_t return_value = pump_packetizer();
//    TEST_ASSERT_EQUAL(ANTI_RES_FILT_GAIN,pump.state);
// }

// void test_pump_packetizer_LEAD_ANGLE(void)
// {
//    pump.idx =12;
//    pump.state = LEAD_ANGLE;
//    pump.attempts = 6;
//    uint8_t return_value = pump_packetizer();
//    TEST_ASSERT_EQUAL(DRIVE_STATUS,pump.state);
// }

// void test_pump_packetizer_LEAD_ANGLE_initial_attempts(void)
// {
//    pump.idx =12;
//    pump.state = LEAD_ANGLE;
//    pump.attempts = 0;
//    uint8_t return_value = pump_packetizer();
//    TEST_ASSERT_EQUAL(LEAD_ANGLE,pump.state);
// }

void test_pump_packetizer_JOG_DECEL(void)
{
   pump.idx =12;
   pump.state = JOG_DECEL;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_JOG_DECEL_initial_attempts(void)
{
   pump.idx =12;
   pump.state = JOG_DECEL;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(JOG_DECEL,pump.state);
}

void test_pump_packetizer_JOG_SPD(void)
{
   pump.idx =12;
   pump.state = JOG_SPD;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_JOG_SPD_initial_attempts(void)
{
   pump.idx =12;
   pump.state = JOG_SPD;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(JOG_SPD,pump.state);
}

void test_pump_packetizer_DRIVE_STATUS(void)
{
   pump.idx =12;
   pump.state = DRIVE_STATUS;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}

void test_pump_packetizer_DRIVE_STATUS_inital_attempts(void)
{
   pump.idx =12;
   pump.state = DRIVE_STATUS;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(DRIVE_STATUS,pump.state);
}

void test_pump_packetizer_MOTOR_ENABLE(void)
{
   pump.idx =12;
   pump.state = MOTOR_ENABLE;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
   TEST_ASSERT_EQUAL(13,pump.idx);
}
void test_pump_packetizer_MOTOR_ENABLE_initial_attempts(void)
{
   pump.idx =12;
   pump.state = MOTOR_ENABLE;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(MOTOR_ENABLE,pump.state);
}

void test_pump_packetizer_COMMENCE_JOG(void)
{
   pump.idx =12;
   pump.state = COMMENCE_JOG;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);//TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}
void test_pump_packetizer_COMMENCE_JOG_initial_attempts(void)
{
   pump.idx =12;
   pump.state = COMMENCE_JOG;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(COMMENCE_JOG,pump.state);
}

void test_pump_packetizer_IDLE_SPEED(void)
{
   pump.idx =12;
   pump.state = IDLE_SPEED;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);//TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}

void test_pump_packetizer_IDLE_SPEED_initial_attempts(void)
{
   pump.idx =12;
   pump.state = IDLE_SPEED;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(IDLE_SPEED,pump.state);
}

void test_pump_packetizer_STOP_KILL(void)
{
   pump.idx =12;
   pump.state = STOP_KILL;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);//TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}

void test_pump_packetizer_STOP_KILL_initial_attempts(void)
{
   pump.idx =12;
   pump.state = STOP_KILL;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(STOP_KILL,pump.state);
}

void test_pump_packetizer_ZERO_SPEED(void)
{
   pump.idx =12;
   pump.state = ZERO_SPEED;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);//TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}

void test_pump_packetizer_ZERO_SPEED_initial_attempts(void)
{
   pump.idx =12;
   pump.state = ZERO_SPEED;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(ZERO_SPEED,pump.state);
}

void test_pump_packetizer_VAC_PULSE_SPEED(void)
{
   pump.idx =12;
   pump.state = VAC_PULSE_SPEED;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   //TEST_ASSERT_EQUAL(13,pump.idx);
   TEST_ASSERT_EQUAL(VAC_PULSE_SPEED,pump.state);
}

void test_pump_packetizer_VAC_PULSE_SPEED_initial_attempts(void)
{
   pump.idx =12;
   pump.state = VAC_PULSE_SPEED;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(VAC_PULSE_SPEED,pump.state);
}

void test_pump_packetizer_SOFT_RESET_24(void)
{
   pump.idx =12;
   pump.state = SOFT_RESET;
   pump.attempts = 24;
   uint8_t return_value = pump_packetizer();
}

void test_pump_packetizer_SOFT_RESET_initial_attempts(void)
{
   pump.idx =12;
   pump.state = SOFT_RESET;
   pump.attempts = 125;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}

void test_pump_packetizer_HARMONIC_FILT_PHASE(void)
{
   pump.idx =12;
   pump.state = HARMONIC_FILT_PHASE;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);//TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}

void test_pump_packetizer_HARMONIC_FILT_PHASE_2(void)
{
   pump.idx =12;
   pump.state = HARMONIC_FILT_PHASE;
   pump.attempts = 4;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(HARMONIC_FILT_PHASE,pump.state);
}


void test_pump_packetizer_HARMONIC_FILT_GAIN(void)
{
   pump.idx =12;
   pump.state = HARMONIC_FILT_GAIN;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);//TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}

void test_pump_packetizer_HARMONIC_FILT_GAIN_2(void)
{
   pump.idx =12;
   pump.state = HARMONIC_FILT_GAIN;
   pump.attempts = 4;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(HARMONIC_FILT_GAIN,pump.state);
}



void test_pump_packetizer_JOG_ACCEL(void)
{
   pump.idx =12;
   pump.state = JOG_ACCEL;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);//TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}

void test_pump_packetizer_JOG_ACCEL_initial_attempts(void)
{
   pump.idx =12;
   pump.state = JOG_ACCEL;
   pump.attempts = 0;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(JOG_ACCEL,pump.state);
}

void test_pump_packetizer_PWR_MODE_SCL_6(void)
{
   pump.idx =12;
   pump.state = PWR_MODE_SCL;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
   TEST_ASSERT_EQUAL(13,pump.idx);//TEST_ASSERT_EQUAL(SOFT_RESET,pump.state);
}

void test_pump_packetizer_PWR_MODE_SCL_4(void)
{
   pump.idx =12;
   pump.state = PWR_MODE_SCL;
   pump.attempts = 4;
   uint8_t return_value = pump_packetizer();
}




void test_pump_packetizer_CONTROL(void)
{
   pump.idx =12;
   pump.state = CONTROL;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
}

void test_pump_packetizer_WAIT_PROBE_TEST_COMPLETE(void)
{
   pump.idx =12;
   pump.state = WAIT_PROBE_TEST_COMPLETE;
   pump.attempts = 6;
   uint8_t return_value = pump_packetizer();
}

void test_receive_pump_cmd_ack (void)
{
   uint8_t return_value;
   pump.state = MAX_CURRENT;
   //UART4_S1 |= UART_S1_RDRF_MASK;
   UART4_D = '?';
   put_event_ExpectAndReturn(&evt_tbl,PUMP_CMD_ACK,1);
   return_value = receive_pump_cmd();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_receive_pump_cmd_nack (void)
{
   uint8_t return_value;
   pump.state = IDLE_SPEED;
   //UART4_S1 |= UART_S1_RDRF_MASK;
   UART4_D = '?';
   put_event_ExpectAndReturn(&evt_tbl,PUMP_CMD_NACK,1);
   return_value = receive_pump_cmd();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_receive_pump_cmd_eack (void)
{
   uint8_t return_value;
   UART4_D = '*';
   //UART4_S1 |= UART_S1_RDRF_MASK;
   put_event_ExpectAndReturn(&evt_tbl,PUMP_CMD_EACK,1);
   return_value = receive_pump_cmd();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_receive_pump_cmd_drive_status_rx (void)
{
   uint8_t return_value;
   //uint8_t pump_packet[MAX_NUM_CHARS];
   //char pump_packet[] = "RS=UnitTest";
   UART4_D = '\r';
   //UART4_S1 |= UART_S1_RDRF_MASK;
   //put_event_ExpectAndReturn(&evt_tbl,PUMP_CMD_EACK,1);
   return_value = receive_pump_cmd();
   TEST_ASSERT_EQUAL(0,return_value);
}
   
/* void test_receive_pump_cmd_drive_status_rx_RS (void)
{
   uint8_t return_value;
   //uint8_t pump_packet[MAX_NUM_CHARS];
   //char pump_packet[] = "RS=UnitTest";
   UART4_D = "RS=XX";
   UART4_S1 |= UART_S1_RDRF_MASK;
   //put_event_ExpectAndReturn(&evt_tbl,PUMP_CMD_EACK,1);
   return_value = receive_pump_cmd();
   TEST_ASSERT_EQUAL(0,return_value);
} */



//HMS temporary block
/* void test_receive_pump_cmd_1 (void)
{
   uint8_t return_value;
   UART4_S1 |= UART_S1_RDRF_MASK;
   UART4_D = '*';
   pump.state = MAX_CURRENT;
   put_event_ExpectAndReturn(&evt_tbl,PUMP_CMD_ACK,1);
   // An error of put_event more times than expected occurs, because
   // the code sits in a while loop. The code needs to be modified to
   // contain #ifndef TEST
   //return_value = receive_pump_cmd();
   TEST_IGNORE_MESSAGE("while loop in receive_pump_cmd()");
}

void test_receive_pump_cmd_2 (void)
{
   uint8_t return_value;
   UART4_S1 |= UART_S1_RDRF_MASK;
   UART4_D = '?';
   pump.state = MAX_CURRENT-1;
   put_event_ExpectAndReturn(&evt_tbl,PUMP_CMD_NACK,1);
   // An error of put_event more times than expected occurs, because
   // the code sits in a while loop. The code needs to be modified to
   // contain #ifndef TEST
   //return_value = receive_pump_cmd();
   TEST_IGNORE_MESSAGE("while loop in receive_pump_cmd()");
}

void test_receive_pump_cmd_3 (void)
{
   uint8_t return_value;
   UART4_S1 |= UART_S1_RDRF_MASK;
   UART4_D = '\r';
   put_event_ExpectAndReturn(&evt_tbl,DRIVE_STATUS_RX,1);
   // An error of put_event more times than expected occurs, because
   // the code sits in a while loop. The code needs to be modified to
   // contain #ifndef TEST
   //return_value = receive_pump_cmd();
   TEST_IGNORE_MESSAGE("while loop in receive_pump_cmd()");
}
 */
void test_drive_power_mode(void)
{
   pump.status_data[0] = 'R';
   pump.status_data[1] = 'R';
   pump.status_data[2] = '=';
   pump.status_data[3] = '3';

   uint8_t return_value = drive_power_mode();
   TEST_ASSERT_EQUAL(PWR_MODE_SCL,pump.state);
   pump.status_data[0] = 'R';
   pump.status_data[1] = 'R';
   pump.status_data[2] = '=';
   pump.status_data[3] = '2';
   return_value = drive_power_mode();
   TEST_ASSERT_EQUAL(CHANGE_BAUD,pump.state);
}

void test_over_current (void)
{
   uint8_t return_value;
   current_therapy_settings.treatment.therapy_mode = NONE;
   return_value = over_current();
   TEST_ASSERT_EQUAL(STOPPING,spd_ctrl_state);
   current_therapy_settings.treatment.therapy_mode = VAC;
   return_value = over_current();
   TEST_ASSERT_EQUAL(OFF,spd_ctrl_state);
}

void test_over_energy (void)
{
   uint8_t return_value;
   spd_ctrl_state = OFF;
   current_therapy_settings.treatment.therapy_mode = NONE;
   return_value = over_energy();
   TEST_ASSERT_EQUAL(OFF,spd_ctrl_state);
   current_therapy_settings.treatment.therapy_mode = VAC;
   return_value = over_energy();
   TEST_ASSERT_EQUAL(QUIESCENT,spd_ctrl_state);
}

void test_probe_test_complete(void)
{
   pump.state = DRIVE_STATUS;
   uint8_t return_value = probe_test_complete();
   TEST_ASSERT_EQUAL(DELAY_TIME,pump.state);
}

void test_drive_status_pump_fault(void)
{
   uint8_t *status_data = "RS=Jxxxxxxxxxxxxxxxx";
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status &= ~PUMP_ON;
   current_therapy_settings.pump.status &= ~PUMP_DISABLED;
   current_therapy_settings.pump.status |= PUMP_FAULT;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_pump_running(void)
{
   uint8_t *status_data = "RS=Rxxxxxxxxxxxxxxxx";
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.pump.status &= ~PUMP_ON;
   current_therapy_settings.pump.status &= ~PUMP_DISABLED;
   current_therapy_settings.pump.status &= ~PUMP_RUNNING;
   //current_therapy_settings.pump.sensor_check = FALSE;
   //current_therapy_settings.pump.status &= ~PUMP_FAULT;

   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_pump_cmd_nack (void)
{
   uint8_t return_value = pump_cmd_nack();
}



void test_pump_reset (void)
{
   uint8_t return_value =  pump_reset();
}

void test_pump_cmd_ack_COMMENCE_JOG (void)
{
   pump.state = COMMENCE_JOG;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(DRIVE_STATUS,pump.state);
}

void test_pump_cmd_ack_IDLE_SPEED_1 (void)
{
   pump.state = IDLE_SPEED;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(DRIVE_STATUS,pump.state);
}

void test_pump_cmd_ack_STOP_KILL (void)
{
   pump.state = STOP_KILL;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(DRIVE_STATUS,pump.state);
}

void test_pump_cmd_ack_PROBE_MOTOR (void)
{
   pump.state = PROBE_MOTOR;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_pump_cmd_ack_VAC_PULSE_SPEED (void)
{
   pump.state = VAC_PULSE_SPEED;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_pump_cmd_ack_CONTROL_1 (void)
{
   pump.state = CONTROL;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_pump_cmd_ack_PWR_MODE_SCL (void)
{
   pump.state = PWR_MODE_SCL;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(0,return_value);
}



void test_pump_cmd_ack_ZERO_SPEED (void)
{
   pump.state = ZERO_SPEED;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(CONTROL,pump.state);
}

// void test_pump_cmd_ack_SOFT_RESET (void)
// {
//    pump.state = SOFT_RESET;
//    uint8_t return_value =  pump_cmd_ack();
//    TEST_ASSERT_EQUAL(CHANGE_BAUD,pump.state);
// }

void test_pump_cmd_ack_JOG_SPEED (void)
{
   pump.state = JOG_SPD;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(DRIVE_STATUS,pump.state);
}

void test_pump_cmd_ack_MOTOR_ENABLE (void)
{
   pump.state = MOTOR_ENABLE;
   uint8_t return_value =  pump_cmd_ack();
   TEST_ASSERT_EQUAL(COMMENCE_JOG,pump.state);
}


void test_drive_status_A(void)
{
   uint8_t *status_data = "RS=Axxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   uint8_t return_value = drive_status();
}

void test_drive_status_D(void)
{
   uint8_t *status_data = "RS=Dxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_E(void)
{
   uint8_t *status_data = "RS=Exxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_F(void)
{
   uint8_t *status_data = "RS=Fxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_H(void)
{
   uint8_t *status_data = "RS=Hxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_J(void)
{
   uint8_t *status_data = "RS=Jxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value;
   pump.inactive_count = 1799;
   put_event_ExpectAndReturn(&evt_tbl,BUCK_INACTIVE_SHUTDOWN,1);
   get_priming_status_ExpectAndReturn(PRIME_OFF); 
   return_value  = drive_status();
}

void test_drive_status_M(void)
{
   uint8_t *status_data = "RS=Mxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_P(void)
{
   uint8_t *status_data = "RS=Pxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_R(void)
{
   uint8_t *status_data = "RS=Rxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}


void test_drive_status_S(void)
{
   uint8_t *status_data = "RS=Sxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_T(void)
{
   uint8_t *status_data = "RS=Txxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_W(void)
{
   uint8_t *status_data = "RS=Wxxxxxxxxxxxxxxxx";
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_pump_on(void)
{
   uint8_t *status_data = "RS=Jxxxxxxxxxxxxxxxx";
   current_therapy_settings.pump.on = 0;
   current_therapy_settings.pump.status = PUMP_ON;
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_pump_ON(void)
{
   uint8_t *status_data = "RS=JFRxxxxxxxxxxxxxx";
   current_therapy_settings.pump.on = 0;
   current_therapy_settings.notify.status = 0x0;
   memcpy(&pump.status_data[0],&status_data[0],20);
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_ON,1);
   uint8_t return_value = drive_status();
}

void test_drive_status_pump_pump_on_0(void)
{
   uint8_t *status_data = "RS=JMFDxxxxxxxxxxxxx";
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.notify.status = 0x0;
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_drive_status_pump_stop_kill(void)
{
   uint8_t *status_data = "RS=JMFExxxxxxxxxxxxx";
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.notify.status = 0x0;
   memcpy(&pump.status_data[0],&status_data[0],20);
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   //get_priming_status_ExpectAndReturn(PRIME_AUTO); 
   uint8_t return_value = drive_status();
}

void test_drive_status_pump_stop_kill_1(void)
{
   uint8_t *status_data = "RS=JMFxxxxxxxxxxxxxx";
   current_therapy_settings.pump.sensor_status |=  DOOR_SENSOR_ON;
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.notify.status = 0x0;
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}


void test_drive_status_pump_stop_kill_2(void)
{
   uint8_t *status_data = "RS=JMFxxxxxxxxxxxxxx";
   current_therapy_settings.pump.sensor_status |=  DOOR_SENSOR_ON;
   current_therapy_settings.pump.on = 1;
   current_therapy_settings.notify.status = 0x0;
   memcpy(&pump.status_data[0],&status_data[0],20);
   current_therapy_settings.pump.status |= ERROR;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   //get_priming_status_ExpectAndReturn(PRIME_AUTO); 
   uint8_t return_value = drive_status();
}



// (((current_therapy_settings.pump.on == 1) && 
//             (current_therapy_settings.pump.status & PUMP_DISABLED) != PUMP_DISABLED) &&
//           (((current_therapy_settings.pump.status & PUMP_FAULT) == PUMP_FAULT) ||
//            ((current_therapy_settings.pump.status & PUMP_ALARM) == PUMP_ALARM) ||
//            ((current_therapy_settings.pump.sensor_status & DOOR_SENSOR_ON) != DOOR_SENSOR_ON)||
//            ((current_therapy_settings.pump.status & ERROR) == ERROR)))



void test_drive_status_pump_notify(void)
{
   uint8_t *status_data = "RS=Rxxxxxxxxxxxxxxxx";
   current_therapy_settings.notify.status |= PUMP_FAULT_MASK;
   memcpy(&pump.status_data[0],&status_data[0],20);
   uint8_t return_value = drive_status();
}

void test_pump_uart_init(void)
{
   uint8_t return_value = pump_uart_init();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_pump_cmd_eack (void)
{
   pump.state = COMMENCE_JOG;
   uint8_t return_value =  pump_cmd_eack ();
   TEST_ASSERT_EQUAL(DRIVE_STATUS,pump.state);
   pump.state = MOTOR_ENABLE;
   return_value =  pump_cmd_eack ();
   TEST_ASSERT_EQUAL(COMMENCE_JOG,pump.state);
}

void test_pump_startup_delay (void)
{
   pump.state = MOTOR_ENABLE;
   put_event_ExpectAndReturn(&evt_tbl,PUMP_RESET,1);
   uint8_t return_value =  pump_startup_delay ();
}

void test_pump_startup_delay_2 (void)
{
   pump.state = WAIT_PROBE_TEST_COMPLETE;
   uint8_t return_value =  pump_startup_delay ();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_pump_set_speed (void)
{
	float Speed = 0;
	pump_set_speed (Speed);
	TEST_ASSERT_EQUAL(Speed,current_therapy_settings.pump.idle_rps);
	
	Speed = 1.0;
	pump_set_speed (Speed);
	TEST_ASSERT_EQUAL(Speed,current_therapy_settings.pump.idle_rps);
}


void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}

