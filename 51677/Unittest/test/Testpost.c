#include "unity.h"
#include "unity_internals.h"
#include "MK26F18.h"
#include "post.h"
#include "Mockpit.h"
#include "common.h"
#include "Mockpump.h"
#include "Mocksystem.h"
#include "Mockevent_buffer.h"
#include "Mockbuck.h"
#include "Mockrcm.h"
#include "Mockaudio.h"
#include "Mockled.h"
#include "Mocklpt.h"
#include "Mocksensors.h"
#include "Mockftfe.h"
#include <stdint.h>


GEN_SETTINGS current_therapy_settings;
GEN_SETTINGS *const pCurrentTherapySettings = &current_therapy_settings;
event_table evt_tbl;
VMOD_COMPENSATION vmod;
extern uint8_t pump_test_latch;

/* buck 18-pin dac output */
const DAC_MemMapPtr pLegacyWandDAC = DAC0_BASE_PTR;

/* buck smart wand dac output */
const DAC_MemMapPtr pSmartWandDAC = DAC1_BASE_PTR;

// uint8_t __MFG_DATE_TIME[];
// uint8_t __SERIAL_NUMBER[];

// __root uint8_t mfg_date_time[MFG_TIME_NUM_CHARS];
// __root uint8_t serial_number[MFG_SERIAL_NUM_CHARS];
// __root uint8_t bcd_serial_number[MFG_SERIAL_NUM_CHARS];

uint8_t __MFG_DATE_TIME[];
uint8_t __SERIAL_NUMBER[];
uint8_t mfg_date_time[MFG_TIME_NUM_CHARS];
uint8_t serial_number[MFG_SERIAL_NUM_CHARS];
uint8_t bcd_serial_number[MFG_SERIAL_NUM_CHARS];

SOUND_LIB lib_sounds[13];
PUMP_CMD pump;
extern VMOD_COMPENSATION vmod;

void setUp(void)
{
}

void tearDown(void)
{
}


void test_post_startup (void)
{
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_ON;
   put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   put_event_ExpectAndReturn(&evt_tbl,POST,1);
   put_event_IgnoreAndReturn(1);
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(IDLE_SPEED,pump.state);
}

void test_post_startup_2 (void)
{
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status &= ~PUMP_ON;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   //put_event_ExpectAndReturn(&evt_tbl,POST,1);
   vmod.vmod2_test_status = VMOD_TEST_DONE;
   current_therapy_settings.leds.startup = FALSE;
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_3 (void)
{
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   pump.state = DRIVE_STATUS;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   //put_event_ExpectAndReturn(&evt_tbl,POST,1);
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_4 (void)
{
   put_event_IgnoreAndReturn(1);
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   pump.state = JOG_SPD;
   uint8_t return_value = post();
}

void test_post_startup_5 (void)
{
   current_therapy_settings.pump.sensor_status &= ~DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   pump.state = JOG_SPD;
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_6 (void)
{
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   pump.state = JOG_SPD;
   LPTMR0_CNR = 2;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_7 (void)
{
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   pump.state = JOG_SPD;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_8 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x0 << DISCHARGE_TUBE_SENSOR_SHIFT;
   pump.state = JOG_SPD;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_9 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x0 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   pump.state = JOG_SPD;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_10 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status &= ~DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859-1;
   pump.state = JOG_SPD;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_11 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status &= ~DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859+1;
   pump.state = DRIVE_STATUS;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_12 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859+1;
   pump.state = JOG_SPD;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_13 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859+1;
   pump.state = DRIVE_STATUS;
   LPTMR0_CNR = 2;
   pump_test_latch = 0;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   put_event_ExpectAndReturn(&evt_tbl,PUMP_COM_FAULT,1);
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_13a (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859+1;
   pump.state = DRIVE_STATUS;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = TRUE;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   //put_event_ExpectAndReturn(&evt_tbl,PUMP_COM_FAULT,1);
   put_event_ExpectAndReturn(&evt_tbl,POST,1);
   uint8_t return_value = post();
   current_therapy_settings.pump.sensor_status &= ~DOOR_SENSOR_ON;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   //put_event_ExpectAndReturn(&evt_tbl,PUMP_COM_FAULT,1);
   return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}


void test_post_startup_14 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859+1;
   pump.state = DRIVE_STATUS;
   LPTMR0_CNR = 2;
   pump_test_latch = 0;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   put_event_ExpectAndReturn(&evt_tbl,PUMP_COM_FAULT,1);
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_16 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status &= ~DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859+1;
   pump.state = DRIVE_STATUS;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,PUMP_COM_FAULT,1); //test
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_17 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status &= ~DOOR_SENSOR_ON;
   current_therapy_settings.pump.status &= ~PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859+1;
   pump.state = DRIVE_STATUS;
   LPTMR0_CNR = 2;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1); //test
   //put_event_ExpectAndReturn(&evt_tbl,PUMP_COM_FAULT,1); //test
   //current_therapy_settings.setpoint.mod1_meas = 38550-3;
   //current_therapy_settings.setpoint.mod2_meas = 38550-3;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
}

void test_post_startup_18 (void)
{
   GPIOC_PDIR = 0x0;
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   GPIOC_PDIR |= 0x1 << SUCTION_TUBE_SENSOR_SHIFT;
   GPIOC_PDIR |= 0x1 << DISCHARGE_TUBE_SENSOR_SHIFT;
   current_therapy_settings.pump.stator = 19859+1;
   pump.state = DRIVE_STATUS;
   LPTMR0_CNR = 2;
   //current_therapy_settings.setpoint.mod1_meas = 38550-3;
   //current_therapy_settings.setpoint.mod2_meas = 38550-3;
   current_therapy_settings.leds.startup = TRUE;
   // The first call will set pump_test_active = 1;
   put_event_ExpectAndReturn(&evt_tbl,POST,1);
   uint8_t return_value = post();
   current_therapy_settings.pump.status = PUMP_ON;
   put_event_ExpectAndReturn(&evt_tbl,POST,1);
   return_value = post();
}

void test_post_startup_15 (void)
{
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   pump.state = JOG_SPD;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   //put_event_ExpectAndReturn(&evt_tbl,PUMP_COM_FAULT,1);
   //   current_therapy_settings.setpoint.mod1_meas = 38550-3;

   //   current_therapy_settings.setpoint.mod2_meas = 38550+1;
   uint8_t return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //ut_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   //   current_therapy_settings.setpoint.mod1_meas = 38550-3;

   //   current_therapy_settings.setpoint.mod2_meas = 23130-1;
   return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   //   current_therapy_settings.setpoint.mod1_meas = 38550+1;
   return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
   current_therapy_settings.leds.startup = FALSE;
   current_therapy_settings.gui.rx_wdog.tx_cntr_idx = 5;
   //put_event_ExpectAndReturn(&evt_tbl,VMOD_FAULT,1);
   //   current_therapy_settings.setpoint.mod1_meas = 23130-1;
   return_value = post();
   TEST_ASSERT_EQUAL(1, return_value);
   //   current_therapy_settings.setpoint.mod2_meas = 23130+2;
   // current_therapy_settings.setpoint.mod1_meas = 23130+2;
   //return_value = post();
}

void test_post_startup_9a (void)
{
   current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
   current_therapy_settings.pump.status |= PUMP_DISABLED;
   pump.state = JOG_SPD;
   LPTMR0_CNR = 2;
   current_therapy_settings.leds.startup = TRUE;
   put_event_ExpectAndReturn(&evt_tbl,POST,1);
   uint8_t return_value = post();
}


void test_get_mfg_date (void)
{
   read_ftfe_once_IgnoreAndReturn(1);
   program_ftfe_once_IgnoreAndReturn(1);
   uint8_t return_value = get_mfg_date();
   TEST_ASSERT_EQUAL(0,return_value);
}


// void test_get_mfg_date_1 (void)
// {
   // uint8_t * ptr = (uint8_t *)&__MFG_DATE_TIME[0];
   // *ptr++ =  0;
   // *ptr++ =  0;//__MFG_DATE_TIME[1] = 0;
   // *ptr++ =  0;//__MFG_DATE_TIME[2] = 0;
   // *ptr++ =  0x01;//__MFG_DATE_TIME[3] = 0x1;
   // *ptr++ =  0xFF;//__MFG_DATE_TIME[4] = 0xFF;
   // program_ftfe_once_IgnoreAndReturn(1);
   // read_ftfe_once_IgnoreAndReturn(1);
   // uint8_t return_value = get_mfg_date();
   // TEST_ASSERT_EQUAL(0,return_value);
// }


void test_get_serial_num (void)
{
   uint8_t * ptr = (uint8_t *)&__SERIAL_NUMBER[0];
   uint8_t chksum =0;
   for (uint8_t i = 0; i < sizeof(serial_number)-1; i++)
    {
       *ptr = 0x0;
       chksum = *ptr;
       ptr++;
    }
   *ptr = ~chksum+1;
   program_ftfe_once_IgnoreAndReturn(1);
   program_ftfe_once_IgnoreAndReturn(1);
   read_ftfe_once_IgnoreAndReturn(1);
   read_ftfe_once_IgnoreAndReturn(1);
   uint8_t return_value = get_serial_num();
}

// void test_get_serial_num_1 (void)
// {
   // uint8_t * ptr = (uint8_t *)&__SERIAL_NUMBER[0];
   // uint8_t chksum =0;
   // for (uint8_t i = 0; i < sizeof(serial_number)-1; i++)
    // {
       // *ptr = 0x0;
       // chksum = *ptr;
       // ptr++;
    // }
   // *ptr = ~chksum;
   // program_ftfe_once_IgnoreAndReturn(1);
   // program_ftfe_once_IgnoreAndReturn(1);
   // read_ftfe_once_IgnoreAndReturn(1);
   // read_ftfe_once_IgnoreAndReturn(1);
   // uint8_t return_value = get_serial_num();
// }


// void test_post_no_startup (void)
// {
   // put_event_ExpectAndReturn(&evt_tbl,POST,1);
   // uint8_t return_value = post();
   // TEST_ASSERT_EQUAL(0,return_value);
// }

void test_post_init (void)
{
   //put_event_Expect((&evt_tbl,MFG_SN_FAULT,1);
   // Tests
   startup_led_init_ExpectAndReturn(1);
   reset_pulse_cnt_IgnoreAndReturn(1);
   Audio_PWM_DMA_Init_Expect();
   reset_monitor_ExpectAndReturn(1);
   vmod2_ramp_test_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,GET_MFG_DATE_TIME,1);
   put_event_ExpectAndReturn(&evt_tbl,GET_SERIAL_NUM,1);
   put_event_ExpectAndReturn(&evt_tbl,POST,1);
   vmod2_ramp_test_IgnoreAndReturn (1);
   vmod1_ramp_test_IgnoreAndReturn (1);
   uint8_t return_value = post_init();
}

// void test_post_exit (void)
// {
	// put_event_IgnoreAndReturn(0);
	// put_event_IgnoreAndReturn(0);
	
   // uint8_t return_value = post_exit();
// }

void test_post_exit_2 (void)
{
	put_event_ExpectAndReturn(&evt_tbl,MFG_SN_FAULT,1);
   put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   
    uint8_t * ptr = (uint8_t *)&__MFG_DATE_TIME[0];
   *ptr++ =  0;
   *ptr++ =  0;//__MFG_DATE_TIME[1] = 0;
   *ptr++ =  0;//__MFG_DATE_TIME[2] = 0;
   *ptr++ =  0x01;//__MFG_DATE_TIME[3] = 0x1;
   *ptr++ =  0xFE;//__MFG_DATE_TIME[4] = 0xFF;
   // Writing the wrong checksum value to make it fail
   mfg_date_time[MFG_TIME_NUM_CHARS-1] = 0x2;
   program_ftfe_once_IgnoreAndReturn (1);
   read_ftfe_once_IgnoreAndReturn (1);
   
   get_mfg_date();
   uint8_t return_value = post_exit();
}

// void test_post_exit_3 (void)
// {
    // uint8_t * ptr = (uint8_t *)&__MFG_DATE_TIME[0];
   // *ptr++ =  0;
   // *ptr++ =  0;//__MFG_DATE_TIME[1] = 0;
   // *ptr++ =  0;//__MFG_DATE_TIME[2] = 0;
   // *ptr++ =  0x01;//__MFG_DATE_TIME[3] = 0x1;
   // *ptr++ =  0xFF;//__MFG_DATE_TIME[4] = 0xFF;

   // program_ftfe_once_IgnoreAndReturn (1);
   // read_ftfe_once_IgnoreAndReturn (1);
   // get_mfg_date();

   // uint8_t return_value = post_exit();
// }

// void test_post_exit_4 (void)
// {
    // uint8_t * ptr = (uint8_t *)&__SERIAL_NUMBER[0];
    // for (uint8_t i = 0; i < 15;i++)
    // {
       // *ptr++ =  1;
    // }
    // *ptr++ =  0x1;//__SERIAL_NUMBER[4] = 0xFF;

   // program_ftfe_once_IgnoreAndReturn (1);
   // program_ftfe_once_IgnoreAndReturn (1);
   // read_ftfe_once_IgnoreAndReturn (1);
   // read_ftfe_once_IgnoreAndReturn (1);
   // //put_event_ExpectAndReturn(&evt_tbl,MFG_SN_FAULT,1);
   // get_serial_num();
   // uint8_t return_value = post_exit();
// }


// void test_post_exit_serial_num_checksum_error (void)
// {
   // //uint8_t * ptr = (uint8_t *)&__SERIAL_NUMBER[0];
   // uint8_t chksum =0;
   // for (uint8_t i = 0; i < sizeof(serial_number)-1; i++)
    // {
       // //  *ptr = i;
       // serial_number[i] = i;
       // chksum += i;
    // }
   // //*ptr = ~chksum+1;
   // // putting a wrong checksum in there to induce error
   // serial_number[MFG_SERIAL_NUM_CHARS-1] = 0;
   // put_event_ExpectAndReturn(&evt_tbl,MFG_SN_FAULT,1);
   // uint8_t return_value =post_exit();
   // // Write the proper checksum
   // serial_number[MFG_SERIAL_NUM_CHARS-1] = ~chksum+1;
// }

// void test_post_exit_mfg_data_checksum_error (void)
// {
   // //uint8_t * ptr = (uint8_t *)&__SERIAL_NUMBER[0];
   // uint8_t chksum =0;
   // for (uint8_t i = 0; i < sizeof(mfg_date_time)-1; i++)
    // {
       // //  *ptr = i;
       // mfg_date_time[i] = i+10;
       // chksum += i+10;
    // }
   // //*ptr = ~chksum+1;
   // // putting a wrong checksum in there to induce error
   // mfg_date_time[MFG_TIME_NUM_CHARS-1] = 0;
   // put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   // uint8_t return_value =post_exit();
   // mfg_date_time[MFG_TIME_NUM_CHARS-1] = ~chksum+1;
// }

// void test_post_exit_clock_error (void)
// {
   // uint8_t return_value =post_exit();
   // TEST_ASSERT_EQUAL(RTC_SR_TCE_MASK,(RTC_SR&RTC_SR_TCE_MASK));
// }

// void test_post_exit_tamper_detect (void)
// {
   // current_therapy_settings.rtc.tamper_detected = 1;
   // //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   // uint8_t return_value =post_exit();
   // TEST_ASSERT_EQUAL(0x8,current_therapy_settings.notify.status2);
// }

// void test_post_exit_reset_jumper (void)
// {
   // //uint8_t reset_vbatt_tamper_jmpr = ~(GPIOB_PDIR >> 9) & 1;
   // GPIOB_PDIR &= 0x0<<9;
   // uint8_t return_value =post_exit();
   // TEST_ASSERT_EQUAL(0,current_therapy_settings.rtc.tamper_detected);
// }



void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
