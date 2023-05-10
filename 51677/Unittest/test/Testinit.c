#include "unity.h"
#include "MK26F18.h"
#include "init.h"
#include "common.h"
#include "cpu.h"
#include "fpu.h"

#include "Mockpit.h"
#include "Mockpump.h"
#include "Mockaudio.h"
#include "Mockrf.h"
//#include "mcg.h"
#include "Mockwand.h"
#include "Mockbuck.h"
#include "Mockfootpedal.h"
#include "Mockdma.h"

//#include "ad7265.h"
#include "Mockcan.h"
#include "Mockadc.h"
#include "Mockpsu.h"
#include "Mocklpt.h"
//#include "esdhc.h"
#include "Mockled.h"
#include "Mocksensors.h"
#include "Mockevent_buffer.h"
#include "Mockrtc.h"
#include "Mocksystick.h"
#include "../../48386/lib/lib.h"
#include "Mocksystem.h"
#include "Mockcrc.h"
#include "Mockprofiles.h"
#include "nv_data.h"
#include "Mockgui.h"

//unsigned short const lib_splash_sound[100668];
//const uint16_t lib_lo_sound[134];

GEN_SETTINGS current_therapy_settings;
event_table evt_tbl;
SMART_WAND_PARAMETERS smart_wand_params;
previous_wands previous_wands_database[LW_DB_SZ];
__root program_header *pheader;
manta_prime prime;

SOUND_LIB lib_sounds[13];
uint8_t __MASTER_VOLUME_SETTING[1];
uint8_t __AMBIENT_VOLUME_SETTING[1];
uint8_t current_coag_setpoint;

event_table evt_tbl;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_sys_init_0(void)
{
   __MASTER_VOLUME_SETTING[0] =  100;
   current_therapy_settings.audio.volume = 0;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   uint8_t return_value = sys_init();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_sys_init_1(void)
{
   __MASTER_VOLUME_SETTING[0] =  0;
   current_therapy_settings.audio.volume = 0;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   uint8_t return_value = sys_init();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_sys_init_2(void)
{
   // This is setup for all function from here on out.
   nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
   //The checksum is set to in, the fastcrc function is mocked to return 1;
   p_data->checksum = 1;
   p_data->active_app_ptr = 0x30004;


   __MASTER_VOLUME_SETTING[0] =  7;
   current_therapy_settings.audio.volume = 0;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   uint8_t return_value = sys_init();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_sys_init_3(void)
{
   // This is setup for all function from here on out.
   nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
   //The checksum is set to in, the fastcrc function is mocked to return 1;
   p_data->checksum = 1;
   p_data->active_app_ptr = 0x30004;
   p_data->nwords = 256;


   __MASTER_VOLUME_SETTING[0] =  7;
   current_therapy_settings.audio.volume = 0;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   uint8_t return_value = sys_init();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_sys_init_4(void)
{
   // This is setup for all function from here on out.
   nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
   //The checksum is set to in, the fastcrc function is mocked to return 1;
   p_data->checksum = 1;
	p_data->active_app_ptr = 0x40004;
	p_data->nwords = 254;

   __MASTER_VOLUME_SETTING[0] =  7;
   current_therapy_settings.audio.volume = 0;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   uint8_t return_value = sys_init();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_sys_init_5(void)
{
   // This is setup for all function from here on out.
   nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
   //The checksum is set to in, the fastcrc function is mocked to return 1;
   p_data->checksum = 0;
	p_data->nwords = 0;

   __MASTER_VOLUME_SETTING[0] =  7;
   current_therapy_settings.audio.volume = 0;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   uint8_t return_value = sys_init();
   TEST_ASSERT_EQUAL(0,return_value);
   
}

void test_sys_init_6(void)
{
   // This is setup for all function from here on out.
   nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
   uint16_t nwords = (sizeof(nonvolatile_data_layout) + 15) >> 4;
   uint16_t nbytes = 0; 
   nbytes = p_data->nwords << 4;
   //The checksum is set to in, the fastcrc function is mocked to return 1;


   p_data->wireless_married_latch = 1;
   p_data->checksum = 1;
   p_data->wireless_en = 1;
   p_data->nwords = nwords;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   sys_init();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.wireless_enable);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.footpedal.wireless.married_latch);
   
   
   // __MASTER_VOLUME_SETTING[0] =  7;
   // current_therapy_settings.audio.volume = 0;
   // rtc_init_ExpectAndReturn(0);
   // fast_crc16_IgnoreAndReturn(1);
   // load_default_volume_Ignore();
   // save_profile_IgnoreAndReturn(1);
   // put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   // uint8_t return_value = sys_init();
   // TEST_ASSERT_EQUAL(0,return_value);
   
}

void test_sys_init_7(void)
{
   // This is setup for all function from here on out.
   nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
   uint16_t nwords = (sizeof(nonvolatile_data_layout) + 15) >> 4;
   uint16_t nbytes = 0; 
   nbytes = p_data->nwords << 4;
   //The checksum is set to in, the fastcrc function is mocked to return 1;

   p_data->wireless_married_latch = 0;
   p_data->checksum = 1;
   p_data->wireless_en = 1;
   p_data->nwords = nwords;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   sys_init();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.wireless_enable);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.footpedal.wireless.married_latch);
   
}

void test_sys_init_8(void)
{
   // This is setup for all function from here on out.
   nonvolatile_data_layout *p_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
   uint16_t nwords = (sizeof(nonvolatile_data_layout) + 15) >> 4;
   uint16_t nbytes = 0; 
   nbytes = p_data->nwords << 4;
   pheader->params.wireless_pair_timeout = 0xFFFFFFFF;
   //The checksum is set to in, the fastcrc function is mocked to return 1;

   p_data->wireless_married_latch = 1;
   p_data->checksum = 1;
   p_data->wireless_en = 0;
   p_data->nwords = nwords;
   rtc_init_ExpectAndReturn(0);
   fast_crc16_IgnoreAndReturn(1);
   load_default_volume_Ignore();
   save_profile_IgnoreAndReturn(1);
   put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   sys_init();
   TEST_ASSERT_EQUAL(1,current_therapy_settings.treatment.wireless_enable);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.footpedal.wireless.married_latch);
   
}

// void test_sys_init_0_ambient_volume(void)
// {
   // __AMBIENT_VOLUME_SETTING[0] =  100;
   // current_therapy_settings.ambient.volume = 0;
   // rtc_init_ExpectAndReturn(0);
   // fast_crc16_IgnoreAndReturn(1);
   // load_default_volume_Ignore();
   // save_profile_IgnoreAndReturn(1);
   // put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   // uint8_t return_value = sys_init();
   // TEST_ASSERT_EQUAL(0,return_value);
// }

// void test_sys_init_1_ambient_volume(void)
// {
   // __AMBIENT_VOLUME_SETTING[0] =  0;
   // current_therapy_settings.ambient.volume = 0;
   // rtc_init_ExpectAndReturn(0);
   // fast_crc16_IgnoreAndReturn(1);
   // load_default_volume_Ignore();
   // save_profile_IgnoreAndReturn(1);
   // put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   // uint8_t return_value = sys_init();
   // TEST_ASSERT_EQUAL(0,return_value);
// }

// void test_sys_init_2_ambient_volume(void)
// {
   // __AMBIENT_VOLUME_SETTING[0] =  7;
   // current_therapy_settings.ambient.volume = 0;
   // rtc_init_ExpectAndReturn(0);
   // fast_crc16_IgnoreAndReturn(1);
   // load_default_volume_Ignore();
   // save_profile_IgnoreAndReturn(1);
   // put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
   // uint8_t return_value = sys_init();
   // TEST_ASSERT_EQUAL(0,return_value);
// }



// void test_init_manager_3(void)
// {
//    uint8_t return_value;
//    *(uint32_t*)0x4003D000 = 10;
//    current_therapy_settings.log.start_time = 8;
//    put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
//    return_value = init_manager();
//    *(uint32_t*)0x4003D000 = 12;
//   // INIT_LPT,                     /*  71  */
//   // INIT_DMA_MUX,                 /*  72  */
//   // INIT_SYSTICK,                 /*  73  */
//   // INIT_AUDIO_VOL_CTRL,          /*  74  */
//   // INIT_AUDIO_PWM,               /*  75  */
//   // INIT_FOOT_PEDAL,              /*  76  */
//   // INIT_BUCK,                    /*  77  */
//   // INIT_SMART_WAND_PWM,          /*  78  */
//   // INIT_ADC,                     /*  79  */
//   // INIT_FLEXCAN,                 /*  80  */
//   // INIT_SMARTWAND_UART,          /*  81  */
//   // INIT_DOOR_SENSOR,             /*  82  */
//   // INIT_TUBE_SENSOR,             /*  83  */
//   // INIT_LED,                     /*  84  */
//   // INIT_AUDIO_AMP,               /*  85  */
//   // INIT_MANAGER,                 /*  86  */
   
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_LPT,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_DMA_MUX,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_SYSTICK,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_AUDIO_VOL_CTRL,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_AUDIO_PWM,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_FOOT_PEDAL,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_BUCK,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_SMART_WAND_PWM,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_ADC,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_FLEXCAN,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_SMARTWAND_UART,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_DOOR_SENSOR,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_TUBE_SENSOR,0);
//   // return_value = init_manager();
//   // put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
//   // *(uint32_t*)0x4003D000 = 3;
//   // current_therapy_settings.log.start_time = 2;
//   // return_value = init_manager();
// }

/* void test_init_manager_1 (void)
{
	uint8_t return_value;
	for (uint8_t i = INIT_LPT; i <= INIT_AUDIO_AMP; i++){
		put_event_ExpectAndReturn(&evt_tbl,i,0);
		return_value = init_manager();
	}
} */

void test_init_manager_2(void)
{
   uint8_t return_value;

  // INIT_LPT,                     /*  71  */
  // INIT_PUMP_UART,               /*  72  */
  // INIT_DMA_MUX,                 /*  73  */
  // INIT_SYSTICK,                 /*  74  */
  // INIT_AUDIO_VOL_CTRL,          /*  75  */
  // INIT_AUDIO_PWM,               /*  76  */
  // INIT_FOOT_PEDAL,              /*  77  */
  // INIT_BUCK,                    /*  78  */
  // INIT_SMART_WAND_PWM,          /*  79  */
  // INIT_ADC,                     /*  80  */
  // INIT_FLEXCAN,                 /*  81  */
  // INIT_SMARTWAND_UART,          /*  82  */
  // INIT_DOOR_SENSOR,             /*  83  */
  // INIT_TUBE_SENSOR,             /*  84  */
  // INIT_LED,                     /*  85 */
  // INIT_LEGACY_WAND,             /*  86  */
  // INIT_FOOT_PEDAL_ZIGBEE,       /*  87  */
  // INIT_ESDHC,                   /*  88  */
  // INIT_AUDIO_AMP,               /*  89  */
  // INIT_MANAGER,                 /*  90  */

  put_event_ExpectAndReturn(&evt_tbl,INIT_LPT,0);
  return_value = init_manager();
  
  put_event_ExpectAndReturn(&evt_tbl,INIT_PUMP_UART,0);
  return_value = init_manager();
  *(uint32_t*)0x4003D000 = 13;
  put_event_ExpectAndReturn(&evt_tbl,INIT_DMA_MUX,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_SYSTICK,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_AUDIO_VOL_CTRL,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_AUDIO_PWM,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_FOOT_PEDAL,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_BUCK,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_SMART_WAND_PWM,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_ADC,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_FLEXCAN,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_SMARTWAND_UART,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_DOOR_SENSOR,0);
  return_value = init_manager();
  put_event_ExpectAndReturn(&evt_tbl,INIT_TUBE_SENSOR,0);
  return_value = init_manager();
  
  *(uint32_t*)0x4003D000 = 10;
  put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
  return_value = init_manager();

  *(uint32_t*)0x4003D000 = 13;
  put_event_ExpectAndReturn(&evt_tbl,INIT_LED,0);
  return_value = init_manager();

  put_event_ExpectAndReturn(&evt_tbl,INIT_LEGACY_WAND,0);
  return_value = init_manager();
  
  put_event_ExpectAndReturn(&evt_tbl,INIT_FOOT_PEDAL_ZIGBEE,0);
  return_value = init_manager();
  
  put_event_ExpectAndReturn(&evt_tbl,INIT_ESDHC,0);
  return_value = init_manager();
  
  put_event_ExpectAndReturn(&evt_tbl,INIT_AUDIO_AMP,0);
  // *(uint32_t*)0x4003D000 = 4;
  current_therapy_settings.log.start_time = 2;
  return_value = init_manager();

  put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
  // *(uint32_t*)0x4003D000 = 4;
  current_therapy_settings.log.start_time = 2;
  return_value = init_manager();
  // put_event_ExpectAndReturn(&evt_tbl,INIT_LED,0);
  // *(uint32_t*)0x4003D000 = 3;
  // current_therapy_settings.log.start_time = 2;
  // return_value = init_manager();

  // put_event_ExpectAndReturn(&evt_tbl,INIT_AUDIO_AMP,0);
  // return_value = init_manager();
  // put_event_ExpectAndReturn(&evt_tbl,INIT_MANAGER,0);
  // return_value = init_manager();

  TEST_ASSERT_EQUAL(0,return_value);

}

// void test_load_default_language (void)
// {
	// SUPPORTED_LANGUAGES return_value,return_value1;
	// return_value = load_default_language(MAX_NUM_LANGUAGES);
	// TEST_ASSERT_EQUAL(ENGLISH,return_value);
	
	// return_value1 = load_default_language(1);
	// TEST_ASSERT_EQUAL(CHINESE,return_value1);
// }

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
