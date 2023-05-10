#include "unity.h"
#include "MK26F18.h"
#include "audio.h"
#include "common.h"
#include "Mocksystem.h"
#include "Mockpit.h"
#include "cpu.h"
#include "../../48386/lib/lib.h"
#include "Mockevent_buffer.h"

const uint16_t lib_lo_sound[43946];
const uint16_t lib_med_sound[43946];
const uint16_t lib_hi_sound[43946];
const uint16_t lib_vac_sound[2994];
const uint16_t lib_coag_sound[85];
const uint16_t lib_click_sound[1022];
const uint16_t lib_ambient_alarm_sound[6643];
const uint16_t lib_error_sound[53655];
const uint16_t lib_splash_sound[38836];
//SOUND_LIB lib_sounds[13];

// typedef struct {
//   uint16_t *p;
//   uint16_t n;
//   uint32_t a;
// }SOUND_LIB;
uint16_t array[13];


GEN_SETTINGS current_therapy_settings;
//GEN_SETTINGS *const pCurrentTherapySettings = &CurrentTherapySettings;
event_table evt_tbl;

void setUp(void)
{
   I2C0_S |= I2C_S_IICIF_MASK;
   SOUND_LIB *sound_lib_ptr = (SOUND_LIB *)&lib_sounds[0];
   for (uint8_t i=0; i < 13; i++)
   {
      sound_lib_ptr->p = &array[i];
      sound_lib_ptr++;
   }
}

void tearDown(void)
{
   I2C0_S &= ~I2C_S_IICIF_MASK;

}

void test_AudioAmp_Init(void)
{
   uint8_t return_value = audio_amp_init();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_set_lm48100q_volume_0 (void)
{
   uint8_t reset;
   reset = 0;
   set_lm48100q_volume(reset);
   TEST_ASSERT_EQUAL(I2C0_D,((LM48100Q_I2C_ADDRESS<<1)|MWSR));
}

void test_set_lm48100q_volume_1 (void)
{
   uint8_t reset;
   reset = 1;
   set_lm48100q_volume(reset);
   TEST_ASSERT_EQUAL(I2C0_S&I2C_S_IICIF_MASK,I2C_S_IICIF_MASK);
}

void test_set_lm48100q_volume_2 (void)
{
   uint8_t reset = 0;
   set_lm48100q_volume(reset);
   reset = 1;
   for (uint8_t i =0; i < 3; i++)
   {
      set_lm48100q_volume(reset);
   }
   TEST_ASSERT_EQUAL(I2C0_S&I2C_S_IICIF_MASK,I2C_S_IICIF_MASK);
}

void test_LM48100Q_I2C_StartTransmission (void)
{
   uint8_t value = 0;
   LM48100Q_I2C_StartTransmission (value);
   TEST_ASSERT_EQUAL(I2C0_D,(LM48100Q_I2C_ADDRESS<<1)|value);
}

void test_AudioPWM_Init(void)
{
   uint8_t return_value = audio_pwm_init();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_Audio_PWM_DMA_Init (void)
{
   Audio_PWM_DMA_Init ();
   // Test one of the register setups
   TEST_ASSERT_EQUAL(0,DMA_TCD14_CSR);
}


void test_IRQ_Audio_Load_Next_Page(void)
{
   //IRQ_Audio_Load_Next_Page();
   TEST_IGNORE_MESSAGE("No definition for IRQ_Audio_Load_Next_Page()");
}


void test_Audio_Vol_Ctrl_Init(void)
{
   uint8_t return_value = audio_vol_ctrl_init();
   TEST_ASSERT_EQUAL(1,return_value);
}


void test_audible_click(void)
{
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.gui.alarm_test_sound = 0;
   uint8_t return_value = audible_click();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_audible_click_1(void)
{
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.gui.alarm_test_sound = 1;
   uint8_t return_value = audible_click();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_audible_click_3(void)
{
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   uint8_t return_value = audible_click();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_reset_alarm_audio(void)
{
   //current_therapy_settings.audio.p_sound_file = 1;
   //current_therapy_settings.gui.alarm_test_sound = 1;
   current_therapy_settings.audio.ambient_alarm_page = 5;
   current_therapy_settings.gui.alarm_test_sound = 1;
   put_event_ExpectAndReturn(&evt_tbl,VOLUME_CTRL,0);
   uint8_t return_value = reset_alarm_audio();
   TEST_ASSERT_EQUAL(0,current_therapy_settings.audio.p_sound_file);
   TEST_ASSERT_EQUAL(0,current_therapy_settings.gui.alarm_test_sound);
}

void test_reset_alarm_audio_1(void)
{
   current_therapy_settings.audio.p_sound_file = (uint16_t *)0xFFFFFFFF;

   current_therapy_settings.audio.ambient_alarm_page = 4;
   current_therapy_settings.gui.alarm_test_sound = 0;
   put_event_ExpectAndReturn(&evt_tbl,VOLUME_CTRL,0);
   uint8_t return_value = reset_alarm_audio();
   TEST_ASSERT_EQUAL(0xFFFFFFFF,current_therapy_settings.audio.p_sound_file);
}



void test_play_change_mode_audio(void)
{
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.treatment.therapy_mode = LO;
   uint8_t return_value = play_change_mode_audio();
}

void test_play_change_mode_audio_1(void)
{
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.ambient.alarm_active = 1;
   uint8_t return_value = play_change_mode_audio();
}

void test_play_change_mode_audio_2(void)
{
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   uint8_t return_value = play_change_mode_audio();
}

void test_play_change_mode_audio_3(void)
{
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.treatment.therapy_mode = MED;
   uint8_t return_value = play_change_mode_audio();
}

void test_play_change_mode_audio_4(void)
{
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.treatment.therapy_mode = HI;
   uint8_t return_value = play_change_mode_audio();
}

void test_play_change_mode_audio_5(void)
{
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.treatment.therapy_mode = NONE;
   uint8_t return_value = play_change_mode_audio();
}




// void test_poll_volume_ctrl(void)
// {
   // //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   // FTM1_QDCTRL = 0x01<<FTM_QDCTRL_QUADIR_SHIFT;
   // FTM1_CNT = 1;
   // uint8_t return_value = poll_volume_ctrl();
   // TEST_ASSERT_EQUAL(0,FTM1_CNT);
// }

void test_poll_volume_ctrl_1(void)
{
   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   FTM1_QDCTRL = 0x01<<FTM_QDCTRL_QUADIR_SHIFT;
   FTM1_CNT = 3;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
}

void test_poll_volume_ctrl_2(void)
{
   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   FTM1_QDCTRL = 0x01<<FTM_QDCTRL_QUADIR_SHIFT;
   FTM1_CNT = 3;
   current_therapy_settings.audio.volume = 7;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
   TEST_ASSERT_EQUAL(8,current_therapy_settings.audio.volume);
}

void test_poll_volume_ctrl_3(void)
{
   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   FTM1_QDCTRL = 0x01<<FTM_QDCTRL_QUADIR_SHIFT;
   FTM1_CNT = 3;
   current_therapy_settings.audio.volume = 11;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
   TEST_ASSERT_EQUAL(11,current_therapy_settings.audio.volume);
}

void test_poll_volume_ctrl_4(void)
{
   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   FTM1_QDCTRL = 0x01<<FTM_QDCTRL_QUADIR_SHIFT;
   FTM1_CNT = 3;
   current_therapy_settings.ambient.volume = 11;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
   TEST_ASSERT_EQUAL(11,current_therapy_settings.ambient.volume);
}

// void test_poll_volume_ctrl_5(void)
// {
//    //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
//    FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
//    put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
//    uint8_t return_value = poll_volume_ctrl();
//    FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
//    FTM1_CNT = 3;
//    FTM1_MOD = 0xFFF;
//    current_therapy_settings.audio.volume = 11;
//    put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
//    return_value = poll_volume_ctrl();
//    TEST_ASSERT_EQUAL(0,FTM1_CNT);
//    TEST_ASSERT_EQUAL(11,current_therapy_settings.audio.volume);
// }

void test_poll_volume_ctrl_6(void)
{
   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t return_value = poll_volume_ctrl();
   FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   FTM1_CNT = 7;
   FTM1_MOD = 0xFFF;
   current_therapy_settings.audio.volume = 1;
   //put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.audio.volume);
}

void test_poll_volume_ctrl_7(void)
{
   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t return_value = poll_volume_ctrl();
   FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   FTM1_CNT = 7;
   FTM1_MOD = 0xFFF;
   current_therapy_settings.audio.volume = 2;
   //put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.audio.volume);
}

void test_poll_volume_ctrl_8(void)
{
   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t return_value = poll_volume_ctrl();
   FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   FTM1_CNT = 7;
   FTM1_MOD = 0xFFF;
   current_therapy_settings.ambient.volume = 1;
   //put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.ambient.volume);
}

void test_poll_volume_ctrl_9(void)
{
   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   uint8_t return_value = poll_volume_ctrl();
   FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   FTM1_CNT = 7;
   FTM1_MOD = 0xFFF;
   current_therapy_settings.ambient.volume = 2;
   //put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.ambient.volume);
}

void test_poll_volume_ctrl_10(void)
{
   uint8_t return_value;

   //((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1
   // FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   // put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   // uint8_t return_value = poll_volume_ctrl();
   // FTM1_QDCTRL = 0x00<<FTM_QDCTRL_QUADIR_SHIFT;
   // FTM1_CNT = 7;
   // FTM1_MOD = 0xFFF;
   // current_therapy_settings.ambient.volume = 2;
   //put_event_ExpectAndReturn(&evt_tbl,CLICK_SOUND,1);
   current_therapy_settings.notify.status = 0x01;
   //current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.p_sound_file = lib_sounds[7].p;

   return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(0,FTM1_CNT);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.ambient.volume);
}

void test_poll_volume_ctrl_11(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   return_value = poll_volume_ctrl();

}

void test_poll_volume_ctrl_12(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.notify.status = 0x00;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_13(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_14(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = (uint16_t *) &lib_ambient_alarm_sound[0];
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_15(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 4;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_16(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_17(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 2;
   return_value = poll_volume_ctrl();
   current_therapy_settings.audio.ambient_alarm_page = 1;
   return_value = poll_volume_ctrl();
   current_therapy_settings.audio.ambient_alarm_page = 3;
   return_value = poll_volume_ctrl();
   current_therapy_settings.audio.ambient_alarm_page = 5;
   return_value = poll_volume_ctrl();
   current_therapy_settings.audio.ambient_alarm_page = 8;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_18(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.audio.p_sound_file = 0;

   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_18_VAC(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.audio.p_sound_file = 0;
   current_therapy_settings.treatment.therapy_mode = VAC;
   return_value = poll_volume_ctrl();
}


void test_poll_volume_ctrl_19(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.audio.p_sound_file = (uint16_t *)0xFFFFFFFF;

   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_19_250(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.audio.p_sound_file = (uint16_t *)0xFFFFFFFF;
   for (uint8_t i =0; i < 251; i++)
   {
      return_value = poll_volume_ctrl();
   }
}



void test_poll_volume_ctrl_19_coag_ablate_0(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.audio.p_sound_file = (uint16_t *)0xFFFFFFFF;
   current_therapy_settings.treatment.coag_on =0;
   current_therapy_settings.treatment.ablate_on = 0;
   return_value = poll_volume_ctrl();
   current_therapy_settings.treatment.coag_on =1;
   current_therapy_settings.treatment.ablate_on = 1;
}


void test_poll_volume_ctrl_20(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.p_sound_file = 0;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = 0;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_21(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.audio.ambient_alarm_page = 0;
   current_therapy_settings.audio.p_sound_file = NULL;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.p_sound_file = 0;
   current_therapy_settings.audio.ambient_alarm_page = 6;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = (uint16_t*)0xFFFFFFFF;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_22(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = (uint16_t *) &lib_ambient_alarm_sound[0];
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.gui.alarm_test_sound = 0;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_23(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = (uint16_t *) &lib_ambient_alarm_sound[0];
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.ambient.alarm_active = 3;
   current_therapy_settings.gui.alarm_test_sound = 0;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_24(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 1;
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = (uint16_t *) &lib_ambient_alarm_sound[0];
   current_therapy_settings.ambient.alarm_active = 0;
   current_therapy_settings.gui.alarm_test_sound = 1;
   return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(&lib_ambient_alarm_sound,current_therapy_settings.audio.p_sound_file);
}

void test_poll_volume_ctrl_25(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)0);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 1;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_26(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)0);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   current_therapy_settings.wand.tube_warn_pulse = TRUE;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_27(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)lib_sounds[12].p);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_28(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)lib_sounds[12].p);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.audio.splash_volume = 675;

   return_value = poll_volume_ctrl();
   TEST_ASSERT_EQUAL(677,current_therapy_settings.audio.splash_volume);
}

void test_poll_volume_ctrl_29(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)lib_sounds[12].p);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.audio.splash_volume = 1285;

   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_30(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 1;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)0);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.audio.splash_volume = 1285;

   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_31(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 0;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)lib_sounds[5].p);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.audio.splash_volume = 1285;

   return_value = poll_volume_ctrl();
}

void test_poll_volume_ctrl_31_VAC(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 1;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 0;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)lib_sounds[5].p);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.audio.splash_volume = 1285;
   current_therapy_settings.treatment.therapy_mode = NONE;

   return_value = poll_volume_ctrl();
}



void test_poll_volume_ctrl_32(void)
{
   uint8_t return_value;
   current_therapy_settings.ambient.audio_enabled = 0;
   current_therapy_settings.notify.status = 0x00;
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.audio.ambient_alarm_page = 7;
   current_therapy_settings.treatment.coag_on = 0;
   current_therapy_settings.audio.p_sound_file = ((uint16_t *)lib_sounds[0].p);
   current_therapy_settings.ambient.alarm_active = 1;
   current_therapy_settings.gui.alarm_test_sound = 0;
   current_therapy_settings.wand.tube_warn_pulse = FALSE;
   current_therapy_settings.audio.splash_volume = 1285;

   return_value = poll_volume_ctrl();
}

void test_continuous_audio_pwm_dma_init (void)
{
   uint16_t nsamples = 10;
   continuous_audio_pwm_dma_init (nsamples);
   DMA_TCD13_SLAST = -(nsamples * 2);
   TEST_ASSERT_EQUAL((-(nsamples * 2)),DMA_TCD13_SLAST);
}

void test_set_volume (void)
{
   uint8_t return_value = set_volume();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_load_default_volume (void)
{
   uint8_t mv = 8;
   uint8_t av = 7;
   load_default_volume(mv, av);
   TEST_ASSERT_EQUAL(mv,current_therapy_settings.audio.volume);
   TEST_ASSERT_EQUAL(av,current_therapy_settings.ambient.volume);
}

void test_load_default_volume_2 (void)
{
   uint8_t mv = 80;
   uint8_t av = 70;
   load_default_volume(mv, av);
   TEST_ASSERT_EQUAL(5,current_therapy_settings.audio.volume);
   TEST_ASSERT_EQUAL(5,current_therapy_settings.ambient.volume);
}

void test_load_default_volume_3 (void)
{
   uint8_t mv = 0;
   uint8_t av = 0;
   load_default_volume(mv, av);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.audio.volume);
   TEST_ASSERT_EQUAL(1,current_therapy_settings.ambient.volume);
}


void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}


// void test_coag_audio_pwm_dma_init(void)
// {
//    coag_audio_pwm_dma_init();
//    TEST_FAIL_MESSAGE("NEED to WRITE TEST");
// }
