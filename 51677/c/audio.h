/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************
$Author:: cwalli00        $: Author
$Rev:: 1434               $: Revision of last commit
$Date:: 2014-09-24 14:09:#$: Date of last commit

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.30.4.3295
*******************************************************************************/

/***************************************************************************//**
@file       audio.h
@brief      Audio power amplifier driver.
@details    This driver controls the audio portion of RF20000. It controls the
sound files using a PWM and a volume using a I2C driver.
*******************************************************************************/

#ifndef AUDIO_H
#define AUDIO_H

#include "../../48386/lib/lib.h"
#include "common.h"

#define MWSR                   0x00  /* Master write  */
#define MRSW                   0x01  /* Master read */

#define I2C_START()            I2C0_C1 |= I2C_C1_TX_MASK;\
                               I2C0_C1 |= I2C_C1_MST_MASK

#define I2C_WAIT()             while((I2C0_S & I2C_S_IICIF_MASK)==0) {} \
                               I2C0_S |= I2C_S_IICIF_MASK;

#define I2C_STOP()             I2C0_C1 &= ~I2C_C1_MST_MASK;\
                               I2C0_C1 &= ~I2C_C1_TX_MASK

#define VOLUME_CTRL(x)         ((x) & 0x1F)
#define MODE_POWER_ON_MASK     0x10
#define MODE_INPUT_2_MASK      0x08
#define MODE_INPUT_1_MASK      0x04

#define LM48100Q_I2C_ADDRESS   0x7C
#define MODE_CRTL_REG_ADDRESS  0x0
#define DIAG_CRTL_REG_ADDRESS  0x20
#define FAULT_CRTL_REG_ADDRESS 0x40
#define VOL_1_CRTL_REG_ADDRESS 0x60
#define VOL_2_CRTL_REG_ADDRESS 0x80

//#define AUDIO_PWM_MODULO       0x1770
//#define AUDIO_PWM_MODULO        0x16e4
//#define AUDIO_PWM_MODULO        1678
#define AUDIO_PWM_MODULO          1678

#define DISABLE_AUDIO   DMA_CERQ = DMA_CERQ_CERQ(13); \
                        DMA_CERQ = DMA_CERQ_CERQ(14); \
                        DMA_CDNE |= DMA_CDNE_CDNE(13); \
                        DMA_CDNE |= DMA_CDNE_CDNE(14);

/* LM48100Q Registers */
typedef enum {
  AUDIO_INIT = 0,
  AUDIO_DIAG = 1,
  AUDIO_FAULT = 2,
  AUDIO_VOL1 = 3,
  AUDIO_VOL2 = 4
}LM48100Q_REGS;

/* Function Prototypes */
uint8_t audio_amp_init(void);
void LM48100Q_I2C_StartTransmission (uint8_t);
//uint8_t set_lm48100q_volume(void);
uint8_t audio_pwm_init(void);
//void Audio_PWM_Start(void);
void Audio_PWM_DMA_Init (void);
uint8_t poll_volume_ctrl(void);
//void Master_Audio_Vol_Ctrl(void);
uint8_t audio_vol_ctrl_init(void);
uint8_t audible_click(void);
void audio_alarmsound(void);
//void Audio_SplashSound(void);
//uint8_t audible_click_volume(void);
uint8_t reset_alarm_audio(void);
uint8_t play_change_mode_audio(void);
//void coag_audio_pwm_dma_init(void);
void continuous_audio_pwm_dma_init(uint16_t);
uint8_t set_volume(void);
void set_lm48100q_volume(uint8_t);
void load_default_volume(uint8_t, uint8_t);

extern SOUND_LIB lib_sounds[13];

#endif
