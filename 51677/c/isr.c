/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2838                 $:
$Date:: 2022-07-24 23:42:33#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      isr.c
\brief     Interrupt service routines.
\details   This file contains the interrupt service handlers which are called
by the hardware when the corresponding interrupts occur.
*******************************************************************************/

/*******************************************************************************
 * Comments:
 * Configure interrupt vector table and run interrupt service routines.
*******************************************************************************/

#include "isr.h"
#include "MK26F18.h"
#include "pit.h"
#include "wand.h"
#include "footpedal.h"
#include "pump.h"
#include "rf.h"
#include "audio.h"
#include "system.h"
#include "adc.h"
#include "buck.h"
#include "rtc.h"
#include "can.h"
#include <stdio.h>
#include "led.h"
#include "zigbee.h"
#include "ad7265.h"
//#include "esdhc_def.h"
#include "legacy_wand.h"
#include "notifications.h"
#include "can_debug.h"
#include "state_machine.h"

#ifdef DEBUG  
#include "bdm_ww_main.h"
extern wcd_bdm_port_status_t            port_status_msg;
#endif

extern manta_prime prime;
extern legacy_wand_params legacy_wand_params_obj;
extern bool usb_connected;
extern uint8_t current_coag_setpoint;
extern void pm(uint8_t);
extern prime_mode_t PrimeState;
extern u8 AutoPrimeId;
extern u8 PrimeId;
extern state current_state;

#ifdef DEBUG_K26
extern uint32_t ipeak_threshold_print;
extern uint16_t irms2_print;
extern uint16_t dac_print;
#endif


/******************************************************************************
* Vector Table
******************************************************************************/

//#pragma location = 0x1FFF0000 
//#pragma data_alignment = 512
vector_entry  __vector_table2[] = //@ ".intvec" =
{
   VECTOR_000,           /* Initial SP           */
   VECTOR_001,           /* Initial PC           */
   VECTOR_002,
   VECTOR_003,
   VECTOR_004,
   VECTOR_005,
   VECTOR_006,
   VECTOR_007,
   VECTOR_008,
   VECTOR_009,
   VECTOR_010,
   VECTOR_011,
   VECTOR_012,
   VECTOR_013,
   VECTOR_014,
   VECTOR_015,
   VECTOR_016,
   VECTOR_017,
   VECTOR_018,
   VECTOR_019,
   VECTOR_020,
   VECTOR_021,
   VECTOR_022,
   VECTOR_023,
   VECTOR_024,
   VECTOR_025,
   VECTOR_026,
   VECTOR_027,
   VECTOR_028,
   VECTOR_029,
   VECTOR_030,
   VECTOR_031,
   VECTOR_032,
   VECTOR_033,
   VECTOR_034,
   VECTOR_035,
   VECTOR_036,
   VECTOR_037,
   VECTOR_038,
   VECTOR_039,
   VECTOR_040,
   VECTOR_041,
   VECTOR_042,
   VECTOR_043,
   VECTOR_044,
   VECTOR_045,
   VECTOR_046,
   VECTOR_047,
   VECTOR_048,
   VECTOR_049,
   VECTOR_050,
   VECTOR_051,
   VECTOR_052,
   VECTOR_053,
   VECTOR_054,
   VECTOR_055,
   VECTOR_056,
   VECTOR_057,
   VECTOR_058,
   VECTOR_059,
   VECTOR_060,
   VECTOR_061,
   VECTOR_062,
   VECTOR_063,
   VECTOR_064,
   VECTOR_065,
   VECTOR_066,
   VECTOR_067,
   VECTOR_068,
   VECTOR_069,
   VECTOR_070,
   VECTOR_071,
   VECTOR_072,
   VECTOR_073,
   VECTOR_074,
   VECTOR_075,
   VECTOR_076,
   VECTOR_077,
   VECTOR_078,
   VECTOR_079,
   VECTOR_080,
   VECTOR_081,
   VECTOR_082,
   VECTOR_083,
   VECTOR_084,
   VECTOR_085,
   VECTOR_086,
   VECTOR_087,
   VECTOR_088,
   VECTOR_089,
   VECTOR_090,
   VECTOR_091,
   VECTOR_092,
   VECTOR_093,
   VECTOR_094,
   VECTOR_095,
   VECTOR_096,
   VECTOR_097,
   VECTOR_098,
   VECTOR_099,
   VECTOR_100,
   VECTOR_101,
   VECTOR_102,
   VECTOR_103,
   VECTOR_104,
   VECTOR_105,
   VECTOR_106,
   VECTOR_107,
   VECTOR_108,
   VECTOR_109,
   VECTOR_110,
   VECTOR_111,
   VECTOR_112,
   VECTOR_113,
   VECTOR_114,
   VECTOR_115,
   VECTOR_116,
   VECTOR_117,
   VECTOR_118,
   VECTOR_119,
   VECTOR_120,
   VECTOR_121,
   VECTOR_122,
   VECTOR_123,
   VECTOR_124,
   VECTOR_125,
   VECTOR_126,
   VECTOR_127,
   VECTOR_128,
   VECTOR_129,
   VECTOR_130,
   VECTOR_131,
   VECTOR_132,
   VECTOR_133,
   VECTOR_134,
   VECTOR_135,
   VECTOR_136,
   VECTOR_137,
   VECTOR_138,
   VECTOR_139,
   VECTOR_140,
   VECTOR_141,
   VECTOR_142,
   VECTOR_143,
   VECTOR_144,
   VECTOR_145,
   VECTOR_146,
   VECTOR_147,
   VECTOR_148,
   VECTOR_149,
   VECTOR_150,
   VECTOR_151,
   VECTOR_152,
   VECTOR_153,
   VECTOR_154,
   VECTOR_155,
   VECTOR_156,
   VECTOR_157,
   VECTOR_158,
   VECTOR_159,
   VECTOR_160,
   VECTOR_161,
   VECTOR_162,
   VECTOR_163,
   VECTOR_164,
   VECTOR_165,
   VECTOR_166,
   VECTOR_167,
   VECTOR_168,
   VECTOR_169,
   VECTOR_170,
   VECTOR_171,
   VECTOR_172,
   VECTOR_173,
   VECTOR_174,
   VECTOR_175,
   VECTOR_176,
   VECTOR_177,
   VECTOR_178,
   VECTOR_179,
   VECTOR_180,
   VECTOR_181,
   VECTOR_182,
   VECTOR_183,
   VECTOR_184,
   VECTOR_185,
   VECTOR_186,
   VECTOR_187,
   VECTOR_188,
   VECTOR_189,
   VECTOR_190,
   VECTOR_191,
   VECTOR_192,
   VECTOR_193,
   VECTOR_194,
   VECTOR_195,
   VECTOR_196,
   VECTOR_197,
   VECTOR_198,
   VECTOR_199,
   VECTOR_200,
   VECTOR_201,
   VECTOR_202,
   VECTOR_203,
   VECTOR_204,
   VECTOR_205,
   VECTOR_206,
   VECTOR_207,
   VECTOR_208,
   VECTOR_209,
   VECTOR_210,
   VECTOR_211,
   VECTOR_212,
   VECTOR_213,
   VECTOR_214,
   VECTOR_215,
   VECTOR_216,
   VECTOR_217,
   VECTOR_218,
   VECTOR_219,
   VECTOR_220,
   VECTOR_221,
   VECTOR_222,
   VECTOR_223,
   VECTOR_224,
   VECTOR_225,
   VECTOR_226,
   VECTOR_227,
   VECTOR_228,
   VECTOR_229,
   VECTOR_230,
   VECTOR_231,
   VECTOR_232,
   VECTOR_233,
   VECTOR_234,
   VECTOR_235,
   VECTOR_236,
   VECTOR_237,
   VECTOR_238,
   VECTOR_239,
   VECTOR_240,
   VECTOR_241,
   VECTOR_242,
   VECTOR_243,
   VECTOR_244,
   VECTOR_245,
   VECTOR_246,
   VECTOR_247,
   VECTOR_248,
   VECTOR_249,
   VECTOR_250,
   VECTOR_251,
   VECTOR_252,
   VECTOR_253,
   VECTOR_254,
   VECTOR_255,
   CONFIG_1,        /* Flash configuration field values */
   CONFIG_2,        /* Flash configuration field values */
   CONFIG_3,        /* Flash configuration field values */
   CONFIG_4,        /* Flash configuration field values */

}; /* VECTOR_TABLE end */

/***************************************************************************//**
default_isr(void) Non-maskable interrupt.
@todo Is this context still valid
@param void
@return 0
@outputs
@context
Note: Current hardware does not support this function so switch pin function
to avoid getting this interrupt.
*******************************************************************************/
void nmi_isr(void)
{
  /* Turn on gate clock */
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
  
  /* Switch pin function to GPIO */
  PORTA_PCR4 = PORT_PCR_MUX(1);  
  
//  /* Alert that unhandled interrupt occured */
//  GPIOA_PCOR = 0x4000000;
  
}

/***************************************************************************//**
default_isr(void)
@param void
@return 0
@outputs
@context Default ISR definition.
*******************************************************************************/
void default_isr(void)
{
   #define VECTORNUM (*(volatile uint8_t*)(0xE000ED04))

  //printf("\n****default_isr entered on vector %d*****\r\n\n",VECTORNUM);
  
  /* Alert that unhandled interrupt occured */
  //GPIOA_PCOR = 0x4000000;

  for(short n=0;n<200;n++)
  {
    asm("nop");
  } 
    
  return;
  
}

/***************************************************************************//**
pit0_isr(void)
@todo write a description for this isr.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void pit0_isr(void)
{
  
  //DISABLE_PIT(0);
  
  RESET_PIT(0);
        
  put_event(&evt_tbl,PUMP_COM_FAULT);
}

/***************************************************************************//**
pit1_isr(void)
@param void
@return 0
@outputs
@context ISR definition for Programmable Interrupt Timer 1 running @ 100 ms.
*******************************************************************************/
void pit1_isr(void)
{
 
  RESET_PIT(1);
  
  put_event(&evt_tbl,CAN_TIMEOUT);
}

/***************************************************************************//**
pit2_isr(void)
@param void
@return 0
@outputs
@context ISR definition for Programmable Interrupt Timer 2. Strobed @ 20ms.
*******************************************************************************/
void pit2_isr(void)
{

  /* Start timeout */
  RESET_PIT(2);
  
  /* Force PWM outputs low */
  RF_OFF(pSmartWandFTM);
   
  put_event(&evt_tbl,WAND_TIMEOUT);
  
}

/***************************************************************************//**
pit3_isr(void)
@todo Fix the context content. Does not seem correct
@param void
@return 0
@outputs
@context ISR definition for Programmable Interrupt Timer 3 running @ 10 ms.
 Timer also drives pdb & adc so it must run at 10ms.
*******************************************************************************/
void pit3_isr(void)
{
  
  static uint8_t count = 1;
    
  RESET_PIT(3);
  
  //LPTMR0_CNR = 0x1;
      
  put_event(&evt_tbl,POLL_FOOTPEDAL);
  put_event(&evt_tbl,VOLUME_CTRL);
  
  /* 200 ms */
  if ((count % 20) == 0)
  {
    
    put_event(&evt_tbl,UPDATE_SCREEN);
    put_event(&evt_tbl,CAN_TX);
    /* case: periodic data is rx'd but you're stuck in idle */    
    //put_event(&evt_tbl,WAND_TX);
    
    /* simulate wdog fault by uncommenting this if statement */
    //if ((RCM_SRS0 & RCM_SRS0_WDOG_MASK) == RCM_SRS0_WDOG_MASK)
    //{

      put_event(&evt_tbl,REFRESH_WDOG);

    //}    

    
  }
  else if ((count == 90) || (count == 190))
  {
    
    put_event(&evt_tbl,SLOW_CAN_TX);
    //current_therapy_settings.footpedal.wireless.handshake_phase = ((current_therapy_settings.footpedal.wireless.handshake_phase+1) % 4);

    
  }
  #ifdef DEBUG_CAN
  else if ((count % 2) == 0)
  {
    
    put_event(&evt_tbl,WAND_CAN_TX_DEBUG);
    
  }
  
  /* 20 ms */
  if ((count % 2) == 1)
  {

    put_event(&evt_tbl,PUMP_CAN_TX_DEBUG);
    
  }
  
  if ((count % 199) == 0)
  {
    
    put_event(&evt_tbl,WAND_WEAR_CAN_DEBUG);
    
  }
  #endif
  
  /* 20 ms */
  if ((count % 2) == 1)
  {

    put_event(&evt_tbl,POLL_PWR_SWITCH);
    //if (legacy_wand_params_obj.wand_connected == 1)
    //{
      put_event(&evt_tbl,LW_READ);
    //}
      put_event(&evt_tbl,SDHC_LOG_DATA);
      put_event(&evt_tbl,POLL_WIRELESS_STATUS);
      if(usb_connected)
      {
        usb_connected = FALSE; 
        put_event(&evt_tbl,POLL_USB);
      }
      if((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA))
      {
        if(current_therapy_settings.treatment.ent_turbo_mode == TRUE)
        {
          current_therapy_settings.setpoint.coag = COAG_10;
        }
        else
        {
          current_therapy_settings.setpoint.coag = current_coag_setpoint;
        }
      }
      if ((current_therapy_settings.pump.door_present == 0) && (current_therapy_settings.gui.graphics.GraphicNumber == IV_BAG_AUTO))
      {
        current_therapy_settings.gui.graphics.GraphicNumber = 0;
      }
      
      if((current_therapy_settings.wand.valid == TRUE) && 
         (!current_therapy_settings.active_wand.lw_active))
    {
      if(current_therapy_settings.controller == ENT)
      {
        smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_ENT;
      }
      else if((current_therapy_settings.controller == SPORTS_MED) && (current_therapy_settings.active_wand.smart_wand == ARTHROPLASTY))
      {
        smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_MANTA;
      }
      else 
      {
        smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_SPORTS_MED; //Default
      }   
    }
    
    if ((legacy_wand_params_obj.wand_connected) && (current_therapy_settings.active_wand.lw_active) ||
        (legacy_wand_params_obj.wand_connected == FALSE) && (current_therapy_settings.wand.valid == FALSE))
    {
      smart_wand_params.mfg.indication = SMART_WAND_MFG_INDICATION_SPORTS_MED; //Default
    }
  }
  
  /* 100 ms & staggered from 20 ms count above */
  if (((count + 5) % 10) == 0)
  {
    /* case: periodic data is rx'd but you're stuck in idle */    
    put_event(&evt_tbl,WAND_TX);
    put_event(&evt_tbl,POLL_ADC_DATA);
    put_event(&evt_tbl,POLL_PUMP_SENSORS);
    //put_event(&evt_tbl,POLL_PWR_SWITCH);
    put_event(&evt_tbl,PUMP_TX);
#ifdef DEBUG_CAN
    put_event(&evt_tbl,WIRELESS_STATUS_CAN_TX);
    
#endif
    
    if(((PrimeState != PRIME_OFF) && (!current_therapy_settings.wand.valid) && (current_therapy_settings.pump.on)) ||
       ((PrimeState != PRIME_OFF) && (smart_wand_params.mfg.indication != SMART_WAND_MFG_INDICATION_MANTA) && (current_therapy_settings.pump.on)))
    {
      if (current_therapy_settings.controller != ENT)
      {
        pump_set_speed(lw_get_pump_speed(0));
        PrimeState = PRIME_OFF;
        PrimeId = 0U;
        AutoPrimeId = 0U;
      }
    }
    
    
  }
  
  /* 500 ms */
  if ((count % 50) == 0)
  {
    // When controller reboots or when Manta plugged in, toggle Prime glow.
    if(prime.blink)
    {
      current_therapy_settings.treatment.prime_glow =~ (bool)current_therapy_settings.treatment.prime_glow;
    }
    put_event(&evt_tbl,CTRL_LED_INPUTS);
    put_event(&evt_tbl,WAND_LOG_ERRORS);

#ifdef DEBUG
    printf("Device Mode:(%d)\n",port_status_msg.devMode);
    printf("Ipeak_threshol_calc:(%d), Dac:(%d), Irms2(%d), Ipeak_counts:(%d) \n",
           ipeak_threshold_print,dac_print,irms2_print,current_therapy_settings.setpoint.trip_load);
    printf("Profile ID: (%d)\n",current_therapy_settings.user_data.settings.active_idx);
    printf("Flow Control Box: %d\n",current_therapy_settings.treatment.flow_auto);
    printf("RAM COAG: %d\n",current_coag_setpoint);
#endif
  }
  
  if (count == 199)
  {
#ifdef DEBUG_K26
    //put_event(&evt_tbl,SAVE_PARAMS_TO_FLASH);
    //save_params2flash();
    //save_jump_address2();
//    if ((FTM3_FMS & FTM_FMS_FAULTF2_MASK) == FTM_FMS_FAULTF2_MASK)
//    {
//      FTM3_FMS &= ~FTM_FMS_FAULTF2_MASK;
//      printf("clear FLT2\n");
//      if((FTM3_FMS & FTM_FMS_FAULTIN_MASK) == FTM_FMS_FAULTIN_MASK)
//      {
//        FTM3_FMS &= ~FTM_FMS_FAULTIN_MASK;
//        printf("clear FLTIN\n");
//      }
//    }
    //printf("%d %d %d %d \n",ipeak_threshold_print,dac_print,irms2_print,current_therapy_settings.setpoint.trip_load); 
    //put_event(&evt_tbl,PRINT_DEBUG);
    //printf("Device Mode:(%d)\n",port_status_msg.devMode);
//    printf("Ipeak_threshol_calc:(%d), Dac:(%d), Irms2(%d), Ipeak_counts:(%d) \n",
//           ipeak_threshold_print,dac_print,irms2_print,current_therapy_settings.setpoint.trip_load);
    //    printf("Profile ID: (%d)\n",current_therapy_settings.user_data.settings.active_idx);
    //    printf("Flow Control Box: %d\n",current_therapy_settings.treatment.flow_auto);
    //    printf("RAM COAG: %d\n",current_coag_setpoint);
#endif
    
  }

  /* reset count */
  if (count == 200)
  { 
    count = 0;
  }
  
  count++;
        
}

/***************************************************************************//**
ftm0_isr(void)
@param void
@return 0
@outputs
@context Legacy wand fault controller ISR
*******************************************************************************/
void ftm0_isr(void)
{
   legacy_wand_IRQ_PWM_fault(pLegacyWandFTM, FTM_FMS_FAULTF1_MASK, FTM_FMS_FAULTF3_MASK);
}
/***************************************************************************//**
ftm3_isr(void)
@param void
@return 0
@outputs
@context Smart wand fault controller ISR
*******************************************************************************/
void ftm3_isr(void)
{
   IRQ_PWM_Fault(pSmartWandFTM, FTM_FMS_FAULTF2_MASK, FTM_FMS_FAULTF0_MASK);
}
/******************************************************************************/


/***************************************************************************//**
dma13_isr(void)
@param void
@return 0
@outputs
@context play pcm audio
*******************************************************************************/
void dma13_isr(void)
{
     
  const uint32_t vac_last_full_audio_pg = (uint32_t) (lib_sounds[4].p + (lib_sounds[4].n / 511) * 511);
  
  
  DMA_CINT |= DMA_CINT_CINT(13);
    
  //if (DMA_TCD13_SADDR == (uint32_t) (&lib_vac_sound[2555]))
  if (DMA_TCD13_SADDR == vac_last_full_audio_pg)
  {
    
    /* No link channel to channel, 1 transaction */
    DMA_TCD13_CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK |
                               DMA_CITER_ELINKYES_LINKCH(14) |
                               DMA_CITER_ELINKYES_CITER(439);
      
    DMA_TCD13_BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK |
                               DMA_BITER_ELINKYES_LINKCH(14) |
                               DMA_BITER_ELINKYES_BITER(439);
    
    DMA_SERQ = DMA_SERQ_SERQ(13);
    
  }
  
//  if ((DMA_TCD13_SADDR != (uint32_t) (&lib_click_sound[1021] + 1)) &&
//      (DMA_TCD13_SADDR != (uint32_t) (&lib_ambient_alarm_sound[14818] + 1)) &&
//      (DMA_TCD13_SADDR != (uint32_t) (&lib_splash_sound[182426] + 1)))
  else if ((DMA_TCD13_SADDR != lib_sounds[6].a) &&
      (DMA_TCD13_SADDR != lib_sounds[7].a) &&
      //(DMA_TCD13_SADDR != (uint32_t) (&lib_splash_sound[(sizeof(lib_splash_sound)/2)-1] + 1)) &&
      //(DMA_TCD13_SADDR != (uint32_t) (&lib_vac_sound[(sizeof(lib_vac_sound)/2)-1] + 1)) &&
      //(DMA_TCD13_SADDR != (uint32_t) (&lib_hi_sound[(sizeof(lib_hi_sound)/2)-1] + 1)) &&
      //(DMA_TCD13_SADDR != (uint32_t) (&lib_med_sound[(sizeof(lib_med_sound)/2)-1] + 1)) &&
      (DMA_TCD13_SADDR != lib_sounds[4].a) &&
      (DMA_TCD13_SADDR != lib_sounds[1].a) &&
      (DMA_TCD13_SADDR != lib_sounds[2].a) &&
      (DMA_TCD13_SADDR != lib_sounds[3].a) &&
      (DMA_TCD13_SADDR != lib_sounds[8].a) &&
      (DMA_TCD13_SADDR != lib_sounds[11].a) &&
      (DMA_TCD13_SADDR != lib_sounds[10].a) &&
      (DMA_TCD13_SADDR != lib_sounds[9].a) &&
      //(DMA_TCD13_SADDR != lib_sounds[5].a) &&
      (DMA_TCD13_SADDR != lib_sounds[12].a))
  {

    DMA_SERQ = DMA_SERQ_SERQ(13);
    
  }
  else if (DMA_TCD13_SADDR == lib_sounds[12].a)
  {
    
    //DMA_TCD13_SADDR = (uint32_t) &lib_splash_sound[0];
    //DMA_TCD13_SADDR = (uint32_t) lib_sounds[12].p;
        
    //DMA_SERQ = DMA_SERQ_SERQ(13);
    
  }
  else if (DMA_TCD13_SADDR == lib_sounds[7].a)
  {
    
    //DMA_TCD13_SADDR = (uint32_t) &lib_ambient_alarm_sound[0];
    DMA_TCD13_SADDR = (uint32_t) lib_sounds[7].p;
        
    DMA_SERQ = DMA_SERQ_SERQ(13);
    
    put_event(&evt_tbl,RESET_ALARM_AUDIO);
            
  }
//  else if (DMA_TCD13_SADDR == (uint32_t)(&lib_vac_sound[(sizeof(lib_vac_sound)/2)-1] + 1))
//  {
//    
//    DMA_TCD13_SADDR = (uint32_t) &lib_vac_sound[0];
//    DMA_SERQ = DMA_SERQ_SERQ(13);
//    
//  }
//  else if (DMA_TCD13_SADDR == (uint32_t)(&lib_hi_sound[(sizeof(lib_hi_sound)/2)-1] + 1))
//  {
//    
//    DMA_TCD13_SADDR = (uint32_t) &lib_hi_sound[0];
//    DMA_SERQ = DMA_SERQ_SERQ(13);
//    
//  }
//  else if (DMA_TCD13_SADDR == (uint32_t)(&lib_med_sound[(sizeof(lib_med_sound)/2)-1] + 1))
//  {
//    
//    DMA_TCD13_SADDR = (uint32_t) &lib_med_sound[0];
//    DMA_SERQ = DMA_SERQ_SERQ(13);
//    
//  }
  else if (DMA_TCD13_SADDR == lib_sounds[4].a)
  {
    
    DMA_TCD13_SADDR = (uint32_t)lib_sounds[4].p;
    
    /* No link channel to channel, 1 transaction */
    DMA_TCD13_CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK |
                               DMA_CITER_ELINKYES_LINKCH(14) |
                               DMA_CITER_ELINKYES_CITER(511);
      
    DMA_TCD13_BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK |
                               DMA_BITER_ELINKYES_LINKCH(14) |
                               DMA_BITER_ELINKYES_BITER(511);
        
    DMA_SERQ = DMA_SERQ_SERQ(13);
    
  }
  else if (DMA_TCD13_SADDR == lib_sounds[8].a)
  {
    
    if (((current_therapy_settings.notify.status & FAULT_MASK_1) > 0) ||
        ((current_therapy_settings.notify.status2 & FAULT_MASK_2) > 0))
    {
      
      //DMA_TCD13_SADDR = (uint32_t) &lib_error_sound[0];
      DMA_TCD13_SADDR = (uint32_t) lib_sounds[8].p;
      DMA_SERQ = DMA_SERQ_SERQ(13);
      
    }
    else if (current_therapy_settings.audio.error_burst_length < 2)
    {
      
      current_therapy_settings.audio.error_burst_length++;
      //DMA_TCD13_SADDR = (uint32_t) &lib_error_sound[0];
      DMA_TCD13_SADDR = (uint32_t) lib_sounds[8].p;
      DMA_SERQ = DMA_SERQ_SERQ(13);
      
    }
    
  }
  else if (DMA_TCD13_SADDR == lib_sounds[1].a)
  {
    
    DMA_TCD13_SADDR = (uint32_t) lib_sounds[1].p;
        
    DMA_SERQ = DMA_SERQ_SERQ(13);
    
  }
  else if (DMA_TCD13_SADDR == lib_sounds[2].a)
  {
    
    DMA_TCD13_SADDR = (uint32_t) lib_sounds[2].p;
        
    DMA_SERQ = DMA_SERQ_SERQ(13);
    
  }
  else if (DMA_TCD13_SADDR == lib_sounds[3].a)
  {
    
    DMA_TCD13_SADDR = (uint32_t) lib_sounds[3].p;
        
    DMA_SERQ = DMA_SERQ_SERQ(13);
    
  }
  
//  else if ((DMA_TCD13_SADDR == lib_sounds[11].a) ||
//           (DMA_TCD13_SADDR == lib_sounds[10].a) ||
//           (DMA_TCD13_SADDR == lib_sounds[9].a))
//  {
//
//    DISABLE_AUDIO;
//    
//  }
//  else if (DMA_TCD13_SADDR == lib_sounds[5].a)
//  {
//    
//    DMA_TCD13_SADDR = (uint32_t) &lib_coag_sound[0];
//    DMA_SERQ = DMA_SERQ_SERQ(13);
//    
//  }
  

  
}
/******************************************************************************/


/***************************************************************************//**
can0_isr(void)
@param void
@return 0
@outputs
@context CAN bus ISR used to interact with the GUI processor.
*******************************************************************************/
void can0_mb_isr(void)
{

  put_event(&evt_tbl,CAN_RX);
  
  flexCAN_readMB();

}


/***************************************************************************//**
uart0_isr(void)
@param void
@return 0
@outputs
@context Interfaces with the smartwand
*******************************************************************************/
void uart0_isr(void)
{
  
  if ((UART0_C2 & UART_C2_TIE_MASK) == UART_C2_TIE_MASK)
  {
  
      smartwand_send_packet();
  
  }
   
  if ((UART0_S1 & UART_S1_RDRF_MASK)==UART_S1_RDRF_MASK) 
  {
  
    smartwand_periodic_packet_handler();
    
  }
  
}

/***************************************************************************//**
uart5_isr(void)
@param void
@return 0
@outputs
@context Interfaces with the zigbee wireless footpedal
*******************************************************************************/
void uart5_isr(void)
{
     
  
  if ((LPUART0_CTRL & LPUART_CTRL_TCIE_MASK) == LPUART_CTRL_TCIE_MASK)
  {
  
    send_wireless_cmd();
  
  }
  if ((LPUART0_STAT & LPUART_STAT_RDRF_MASK)==LPUART_STAT_RDRF_MASK) 
  {
  
    receive_wireless_cmd();
    
  }

}
  
  
/***************************************************************************//**
adc0_isr(void)
@param void
@return 0
@outputs
@context legacy wand adc channel isr. trigger set @ 100us
*******************************************************************************/

void adc0_isr(void)
{
  volatile uint16_t result;
  result = (unsigned short) ADC0_RA;  
  put_event(&evt_tbl,ADC0_COCO);
  
}

/***************************************************************************//**
adc1_isr(void)
@todo What does this isr do?
@param void
@return 0
@outputs
@context trigger set @ 100us for irms2 & stator
*******************************************************************************/
void adc1_isr(void)
{   
  volatile uint16_t adc_result0, adc_result1;
  //static uint32_t stator_accum = 0;
  const uint32_t adc_cmd_word_mod2 = 0x0000004E; 
  const uint32_t adc_cmd_word_mod1 = 0x0000004F;
  static uint8_t vmod2_sa_cnt = 0,
                 vmod1_sa_cnt = 0;
  static bool temp_flag = TRUE;
           
  if (( ADC1_SC1A & ADC_SC1_COCO_MASK ) == ADC_SC1_COCO_MASK)
  {
    
    if ((uint16_t)ADC1_RA > 56301)
    {
    
      adc_result0 = 0xFFFFu;
      
    }
    else
    {
      
      adc_result0 = (((uint16_t)ADC1_RA * 1164u) / 1000u) & 0xFFFFu;
      
    }
    if((vmod.vmod1_test_status != VMOD_TEST_DONE) || (vmod.vmod2_test_status != VMOD_TEST_DONE))
    {
      
    if (ADC1_SC1A == (uint32_t) adc_cmd_word_mod1)
    {
      //printf("VM1\n");
      ADC1_SC1A = (uint32_t) adc_cmd_word_mod2;
      
      #ifdef DEBUG_CAN

      CANdebug.adc.vmod1 = adc_result0;
      
      #endif
      
      /* weighted filter */
      vmod.mod1_meas =
      (adc_result0 >> 1) + (vmod.mod1_meas >> 1);

//      if (vmod.mod1_meas > 59120)
//      {
//
//        put_event(&evt_tbl,VMOD_FAULT);
//
//      }
      
      if (vmod1_sa_cnt == 0)
      {

        put_event(&evt_tbl,VMOD1_TEST);

      }
      
      vmod1_sa_cnt = (vmod1_sa_cnt + 1) % 20;
      
    }
    else
    {
      //printf("VM2\n");
      ADC1_SC1A = (uint32_t) adc_cmd_word_mod1;
      
      #ifdef DEBUG_CAN

      CANdebug.adc.vmod2 = adc_result0;
      
      #endif
      /* weighted filter */
      vmod.mod2_meas =
      (adc_result0 >> 1) + (vmod.mod2_meas >> 1);

//      if (vmod.mod2_meas > 59120)
//      {
//
//        put_event(&evt_tbl,VMOD_FAULT);
//
//      }
      
      if (vmod2_sa_cnt == 0)
      {

        put_event(&evt_tbl,VMOD2_TEST);

      }
      
      vmod2_sa_cnt = (vmod2_sa_cnt + 1) % 20;
    
    }
    }
    //stator_accum += adc_result0;
    //GPIOA_PTOR = YELLOW_LED;
      if((vmod.vmod1_test_status == VMOD_TEST_DONE) && 
         (vmod.vmod2_test_status == VMOD_TEST_DONE) &&
         temp_flag)
      {
        temp_flag = FALSE;
        PDB0_SC &= ~PDB_SC_PDBEN_MASK;
        ADC1_SC1B = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(19); //irms2
        ADC1_SC1A = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(0); //irms1
        PDB0_SC |= PDB_SC_PDBEN_MASK;
        
      }
    
     if (legacy_wand_params_obj.wand_connected && (ADC1_SC1A == (ADC_SC1_AIEN(1) | ADC_SC1_ADCH(0))))
    {
      put_event(&evt_tbl,IRMS1_ADC_RESULT);
      adc_result0 = (uint16_t) ADC1_RA;
    }
       
  }

  if (( ADC1_SC1B & ADC_SC1_COCO_MASK ) == ADC_SC1_COCO_MASK)
  {
    //printf("IRMS2\n");
    
      put_event(&evt_tbl,IRMS2_ADC_RESULT);
      
      adc_result1 = (uint16_t) ADC1_RB;
      
#ifdef DEBUG_CAN
      CANdebug.adc.irms2 = adc_result1;
#endif

  }
  
  
}

/***************************************************************************//**
hard_fault_handler_asm
@param pointer to hardfault arguments
@return 0
@outputs
@context This is the hard fault asm handler
*******************************************************************************/
void hard_fault_handler_asm(void)
{
  /* slight change here from EWARM 6.5 - see IAR Tech Note 31138 */
  asm("TST LR, #4 \n"
  "ITE EQ \n"
  "MRSEQ R0, MSP \n"
  "MRSNE R0, PSP \n"
  "B hard_fault_handler_c \n");
}

/***************************************************************************//**
hard_fault_handler_c
@param pointer to hardfault arguments
@return 0
@outputs
@context This is the hard fault C handler
*******************************************************************************/
void hard_fault_handler_c (unsigned int * hardfault_args)
{
  
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;
 
  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);
 
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr = ((unsigned long) hardfault_args[5]);
  stacked_pc = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);
 
  printf ("\n\n[Hard fault handler - all numbers in hex]\n");
  printf ("R0 = %x\n", stacked_r0);
  printf ("R1 = %x\n", stacked_r1);
  printf ("R2 = %x\n", stacked_r2);
  printf ("R3 = %x\n", stacked_r3);
  printf ("R12 = %x\n", stacked_r12);
  printf ("LR [R14] = %x  subroutine call return address\n", stacked_lr);
  printf ("PC [R15] = %x  program counter\n", stacked_pc);
  printf ("PSR = %x\n", stacked_psr);
  printf ("BFAR = %x\n", (*((volatile unsigned long *)(0xE000ED38))));
  printf ("CFSR = %x\n", (*((volatile unsigned long *)(0xE000ED28))));
  printf ("HFSR = %x\n", (*((volatile unsigned long *)(0xE000ED2C))));
  printf ("DFSR = %x\n", (*((volatile unsigned long *)(0xE000ED30))));
  printf ("AFSR = %x\n", (*((volatile unsigned long *)(0xE000ED3C))));
  //printf ("SCB_SHCSR = %x\n", SCB->SHCSR);
 
  while(1);
  
}

/***************************************************************************//**
pdb_isr(void)
@todo What does this isr do?
@param void
@return 0
@outputs
@context
*******************************************************************************/
void pdb_isr(void)
{
  
  if (((PDB0_CH0S & PDB_S_ERR_MASK) > 0) || ((PDB0_CH1S & PDB_S_ERR_MASK) > 0))
  {
    
    PDB0_CH0S &= ~PDB_S_ERR_MASK;
    PDB0_CH1S &= ~PDB_S_ERR_MASK;
    
  }
  
  /* comment if to trap */
  PDB0_SC |= PDB_SC_PDBIF_MASK;
  
}


/***************************************************************************//**
uart4_isr(void)
@param void
@return 0
@outputs
@context Interfaces with the pump
*******************************************************************************/
void uart4_isr(void)
{
     
  
  if ((UART4_C2 & UART_C2_TCIE_MASK) == UART_C2_TCIE_MASK)
  {
  
    send_pump_cmd();
  
  }
  if ((UART4_S1 & UART_S1_RDRF_MASK)==UART_S1_RDRF_MASK) 
  {
  
    receive_pump_cmd();
    
  }

}

/***************************************************************************//**
rtc_isr(void)
@todo write what this isr does
@param void
@return 0
@outputs
@context 
*******************************************************************************/
void rtc_isr(void)
{
    
  /* alarm indicates a firmware updated completed */
  if((RTC_SR & RTC_SR_TAF_MASK) == 0x04)
  {
  
    if (current_therapy_settings.sw.update_progress == 10)
    {
      
      current_therapy_settings.sw.update_complete = TRUE;
      
    }
    else
    {

      /* firmware update timeout */      
      current_therapy_settings.sdhc.card_ready = 0;
      current_therapy_settings.sdhc.update_uis = FALSE;
      current_therapy_settings.sdhc.gsfw_update_avaliable = 0;
      current_therapy_settings.sdhc.uifw_update_avaliable = 0; 
      current_therapy_settings.notify.status2 |= GENERIC_ERROR_1_STATUS_2_MASK;
      put_event(&evt_tbl,COMMENCE_GS_SW_UPDATE);
      current_therapy_settings.sdhc.card_detected = 0;
      
    }
      
    
  }
  
  rtc_check_errors();
  
}


/***************************************************************************//**
rtc_sec_isr(void)
@param void
@return 0
@outputs
@context This is the ISR by the real time counter. It gives an interrupt once
every second.
*******************************************************************************/
void rtc_sec_isr(void)
{
      
  put_event(&evt_tbl,WAND_USE_LIMIT_CHECK);
  if ((SIM_SCGC5 & SIM_SCGC5_LPTMR_MASK) == SIM_SCGC5_LPTMR_MASK)
    put_event(&evt_tbl,RESET_ROTOR_PULSE_CNT);
    
}

/***************************************************************************//**
lpt_isr(void)
@todo write what device this interfaces to
@param void
@return 0
@outputs
@context Low power timer ISR
*******************************************************************************/
void lpt_isr(void)
{

  /* motor exceed 500 rpm (9 counts in 1 sec) */
  put_event(&evt_tbl,PUMP_COM_FAULT);
    
  /* reset timer */
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
  LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
  
}


/***************************************************************************//**
i2c0_isr(void)
@param void
@return 0
@outputs
@context interfaces with the volume control chip to control the speaker output
*******************************************************************************/
void i2c0_isr(void)
{
  
  uint8_t reset = 1;

  /* reset interrupt */
  I2C1_S |= I2C_S_IICIF_MASK;

  set_lm48100q_volume(reset);

  //audio_i2c_state = (audio_i2c_state + 1) % 3;
  
  
}

/***************************************************************************//**
i2c1_isr(void)
@todo write what device this interfaces to
@param void
@return 0
@outputs
@context 
*******************************************************************************/
void i2c1_isr(void)
{
  
  static uint8_t i2c_state = 0;
  
  /* reset interrupt */
  I2C1_S |= I2C_S_IICIF_MASK;
  
  pcf8575_iic_write(i2c_state);
  
  i2c_state = (i2c_state + 1) % 3;
  
  
}

/***************************************************************************//**
systick_isr(void)
@todo add more description
@param void
@return 0
@outputs
@context System tick ISR.
*******************************************************************************/
void systick_isr(void)
{
  
  put_event(&evt_tbl,SYSTICK);

  /* disable systick */
  SYST_CSR &= ~SysTick_CSR_ENABLE_MASK;
  
}

/***************************************************************************//**
sdhc_isr(void)
@param void
@return 0
@outputs
@context SD card ISR
*******************************************************************************/
void sdhc_isr(void)
{
  
  
  //SDHC_IRQSIGEN = 0x00000000; // Disable Interrupt 

   
//printf("\n\n\t****ESDHC ISR entered*****\n");
   //printf("\tSDHC_IRQSTAT = %x \n",SDHC_IRQSTAT);

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_DMAE_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_DMAE is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_DMAE_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_AC12E_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_AC12E is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_AC12E_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_DEBE_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_DEBE is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_DEBE_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_DCE_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_DCE is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_DCE_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_DTOE_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_DTOE is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_DTOE_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_CIE_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_CIE is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_CIE_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_CEBE_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_CEBE is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_CEBE_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_CCE_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_CCE is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_CCE_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_CTOE is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_CTOE_MASK;
       
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_CINT_MASK)  
   {
   	//printf("\t\tESDHCV2_IRQSTAT_CINT is set \n");
	SDHC_IRQSTAT = SDHC_IRQSTAT_CINT_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK)  
   {
      //printf("\t\tESDHCV2_IRQSTAT_CRM is set \n");
      SDHC_IRQSTAT = SDHC_IRQSTAT_CRM_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_CINS_MASK)  
   {
      //printf("\t\tESDHCV2_IRQSTAT_CINS is set \n");
      SDHC_IRQSTAT = SDHC_IRQSTAT_CINS_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_BRR_MASK)  
   {
      //printf("\t\tESDHCV2_IRQSTAT_BRR is set \n");
      SDHC_IRQSTAT = SDHC_IRQSTAT_BRR_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_BWR_MASK)  
   {
      //printf("\t\tESDHCV2_IRQSTAT_BWR is set \n");
      SDHC_IRQSTAT = SDHC_IRQSTAT_BWR_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_DINT_MASK)  
   {
      //printf("\t\tESDHCV2_IRQSTAT_DINT is set \n");
      SDHC_IRQSTAT = SDHC_IRQSTAT_DINT_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_BGE_MASK)  
   {
      //printf("\t\tESDHCV2_IRQSTAT_BGE is set \n");
      SDHC_IRQSTAT = SDHC_IRQSTAT_BGE_MASK;
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_TC_MASK)  
   {
      //printf("\t\tESDHCV2_IRQSTAT_TC is set \n");
      SDHC_IRQSTAT = SDHC_IRQSTAT_TC_MASK;
     // Rd_Response48();
      put_event(&evt_tbl,SDHC_DOWNLOAD);
   }

   if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)  
   {
      //printf("\t\tESDHCV2_IRQSTAT_CC is set \n");
      SDHC_IRQSTAT = SDHC_IRQSTAT_CC_MASK;
      put_event(&evt_tbl,SDHC_DOWNLOAD);
   }

  //SDHC_IRQSTATEN = 0x00000000;// Disable Set Interrupt flag
   
  //SDHC_IRQSTATEN |= SDHC_IRQSTATEN_CCSEN_MASK;
  // Clear Command Complete bit
  //SDHC_IRQSTAT = 0xFFFFFFFF;
  //SDHC_IRQSIGEN = SDHC_IRQSIGEN_CCIEN_MASK;
  
       
}

/***************************************************************************//**
spi0_isr(void)
@param void
@return 0
@outputs
@context SPI0 ISR. This ISR interfaces with AD7265 which read the legacy wand
parameters and events
*******************************************************************************/
void spi0_isr(void) 
{
   __istate_t s = __get_interrupt_state();
  __disable_interrupt();
  // read all 12 channels at once.
  static uint8_t channel = 0;
  channel = (channel + 1) % 6;
  IRQ_AD7265_Read(channel);
  if (channel != 5)
  {
    DMA_SERQ |= DMA_SERQ_SERQ(4);
    DMA_SERQ |= DMA_SERQ_SERQ(5);
  }
  __set_interrupt_state(s);
//   IRQ_AD7265_Read(1);
//   DMA_SERQ |= DMA_SERQ_SERQ(4);
//   DMA_SERQ |= DMA_SERQ_SERQ(5);
//   IRQ_AD7265_Read(2);
//   DMA_SERQ |= DMA_SERQ_SERQ(4);
//   DMA_SERQ |= DMA_SERQ_SERQ(5);
//   IRQ_AD7265_Read(3);
//   DMA_SERQ |= DMA_SERQ_SERQ(4);
//   DMA_SERQ |= DMA_SERQ_SERQ(5);
//   IRQ_AD7265_Read(4);
//   DMA_SERQ |= DMA_SERQ_SERQ(4);
//   DMA_SERQ |= DMA_SERQ_SERQ(5);
//   IRQ_AD7265_Read(5);
//   DMA_SERQ = DMA_SERQ_SERQ(4);
//   DMA_SERQ = DMA_SERQ_SERQ(5);
}
