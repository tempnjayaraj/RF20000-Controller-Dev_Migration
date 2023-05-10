/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************
$Author:: Harshil.Shah    $: Author
$Rev:: 2810               $: Revision of last commit
$Date:: 2022-02-08 15:46:#$: Date of last commit

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.30.4.3295
*******************************************************************************/

/***************************************************************************//**
@file     audio.c
@brief     Audio power amplifier driver.
@details   This driver controls the audio portion of RF20000. It controls the
sound files using a PWM and a volume using a I2C driver.
*******************************************************************************/

#include "MK26F18.h"
#include "audio.h"
#include "common.h"
#include "system.h"
#include "pit.h"
#include "cpu.h"

const uint32_t ftm2_sync_cmd_word = FTM_SYNC_REINIT_MASK |
                                           FTM_SYNC_CNTMAX_MASK |
                                           FTM_SYNC_SWSYNC_MASK;

__no_init SOUND_LIB lib_sounds[13] @ 0x70000;

__root const uint16_t lib_coag_max_sound[85] =
{809, 814, 817, 823, 827, 832, 837, 841, 847, 849,
855, 858, 863, 866, 870, 873, 876, 879, 879, 881,
879, 879, 878, 875, 875, 869, 868, 861, 859, 852,
849, 844, 839, 836, 830, 828, 823, 821, 817, 814,
811, 807, 802, 797, 792, 787, 783, 779, 774, 772,
767, 765, 760, 757, 755, 751, 750, 745, 745, 741,
740, 741, 739, 744, 742, 746, 749, 751, 755, 758,
762, 765, 768, 772, 776, 778, 782, 785, 787, 792,
793, 798, 799, 805, 807};

//__root const SOUND_LIB coag_max_sound;
//coag_max_sound.p = (uint16_t *)&lib_coag_max_sound[0];
//coag_max_sound.n = (sizeof(lib_coag_max_sound) / sizeof(uint16_t));
//coag_max_sound.a = (uint32_t) (&lib_coag_max_sound[84] + 1);

/***************************************************************************//**
Inits the audio amplifier part of the circuit.
@param void
@return uint8_t
@outputs
@context
*******************************************************************************/
uint8_t audio_amp_init(void)
{

  /* disable irq in case entering after brown-out */
  NVIC_DisableIRQ(I2C0_IRQn);

  /* enable i2c0 gate clock */
  SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;

  /* disable i2c before mucking w/ registers */
  I2C0_C1 &= ~(I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK);

  /* set i2c0 pins to control LM48100Q audio amplifier */
  PORTB_PCR0 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK; // I2C0_SCL
  PORTB_PCR1 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK; // I2C0_SDA

  /* set input for LM48100Q amplifier fault pin */
  PORTB_PCR5 = PORT_PCR_MUX(1); // GPIO

  /* set baud rate */
  I2C0_F = I2C_F_MULT(2) | I2C_F_ICR(128); // 289.0625kHz

  /* enable i2c */
  I2C0_C1 |= I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK;

  /* send address */
  LM48100Q_I2C_StartTransmission(MWSR);

  /* wait for ack */
  I2C_WAIT();

  /* send init packet */
  I2C0_D = MODE_CRTL_REG_ADDRESS | MODE_POWER_ON_MASK | MODE_INPUT_2_MASK;

  /* wait for ack */
  I2C_WAIT();

  /* send stop signal */
  I2C_STOP();

  NVIC_EnableIRQ(I2C0_IRQn); // i2c0

  return 1;

}

/***************************************************************************//**
Start I2C transmission to LM48100Q amplifier.
@param Mode (read of write operation)
@return
@outputs
@context
*******************************************************************************/
void LM48100Q_I2C_StartTransmission (uint8_t Mode)
{
  uint8_t SlaveID;

  /* Shift ID in right possition */
  SlaveID = (uint8_t) LM48100Q_I2C_ADDRESS << 1;

  /* Set R/W bit at end of Slave Address */
  SlaveID |= (uint8_t)Mode;

  /* Generate start signal */
  I2C_START();

  /* Send ID with W/R bit */
  I2C0_D = SlaveID;

}

/***************************************************************************//**
Sets the volume to the reset level
@param Mode
@return
@outputs
@context
*******************************************************************************/
uint8_t set_volume(void)
{

  uint8_t reset = 0;

  set_lm48100q_volume(reset);

  return 0;

}

/***************************************************************************//**
Start I2C transmission.
@param uint8_t reset (used to determine the packet byte order)
@return
@outputs
@context
*******************************************************************************/
void set_lm48100q_volume(uint8_t reset)
{

  static uint8_t audio_i2c_state = 0;
  uint8_t volume_step = 0;

  if (reset == 1)
  {

    audio_i2c_state = (audio_i2c_state + 1) % 3;

  }
  else
  {

    audio_i2c_state = 0;

  }

  /* reset interrupt */
  I2C0_S |= I2C_S_IICIF_MASK;

  switch(audio_i2c_state)
  {

    case(0):
    {

      /* Send address */
      LM48100Q_I2C_StartTransmission(MWSR);
      break;

    }
    case(1):
    {

      volume_step = poll_volume_ctrl();
      I2C0_D = VOL_2_CRTL_REG_ADDRESS | VOLUME_CTRL(volume_step);
      break;

    }
    case(2):
    {

      /* Generate stop signal */
      I2C_STOP();
      break;

    }

  }

}

/***************************************************************************//**
Configure audio output using PWM timer.
@param
@return
@outputs
@context
*******************************************************************************/
uint8_t audio_pwm_init(void)
{

  /* Enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;

  /* Set Sound PWM Output for LM48100Q */
  PORTA_PCR11 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK; // FTM2_CH1

  /* Disable write protection */
  FTM2_MODE |= FTM_MODE_WPDIS_MASK;

  FTM2_CONF |= FTM_CONF_BDMMODE(3);

  /* Set Period */
  FTM2_MOD = AUDIO_PWM_MODULO-1;

  /* Edge-aligned, low-true pulses */
  FTM2_C1SC |= FTM_CnSC_ELSB_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_DMA_MASK | FTM_CnSC_CHIE_MASK;

  /* Set Dummy Duty Cycle For Now*/
  FTM2_C1V = 0x1;

  /* Initialize channels */
  FTM2_MODE |= FTM_MODE_INIT_MASK;

  /* Configure PWM synchronization by software trigger*/
  FTM2_SYNCONF |= FTM_SYNCONF_SYNCMODE_MASK | //FTM_SYNCONF_SWRSTCNT_MASK |
                  FTM_SYNCONF_SWWRBUF_MASK | FTM_SYNCONF_SWOM_MASK |
                  FTM_SYNCONF_SWSOC_MASK | FTM_SYNCONF_SWOC_MASK;

  FTM2_SYNC |= FTM_SYNC_REINIT_MASK | FTM_SYNC_CNTMAX_MASK;

  /* Trigger PWM synchronization */
  FTM2_SYNC |= FTM_SYNC_SWSYNC_MASK;

  /* Select system clock */
  FTM2_SC = FTM_SC_PS(0) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

  return 1;

}


///******************************************************************************
//* Function: PWM_SoftStart_Init
//*
//* Purpose: Ramp up duty cycle of quadrature PWM channels.
//******************************************************************************/
//
//void Audio_PWM_Start (void) {
//
//  uint16_t *ptr, *foo;
//  uint32_t *boo;
//  bool error;
//
//  /* Initialize DMA that will load soft-start values into PWM timer*/
//  Audio_PWM_DMA_Init(ptr);
//
//  /* Clear done bits */
//  DMA_CDNE |= DMA_CDNE_CDNE(13);
//  DMA_CDNE |= DMA_CDNE_CDNE(14);
//  //DMA_CDNE |= DMA_CDNE_CDNE(12);
//
//  /* Turn RF ON */
//  //RF_ON(PWM_SMART_WAND);
//
//  /* Set enable request bits (two for FTM channels & one for sync) */
//  DMA_SERQ = DMA_SERQ_SERQ(13); // FTM1_C0V
//  DMA_SERQ = DMA_SERQ_SERQ(14); // FTM1_SYNC
//  //DMA_SERQ = DMA_SERQ_SERQ(12); // FTM0_SYNC
//
//  /* Wait for soft-start to complete or timeout */
//  //while((PIT_TFLG0 & PIT_TFLG_TIF_MASK != PIT_TFLG_TIF_MASK) ||
//  //      (DMA_INT & DMA_INT_INT12_MASK != DMA_INT_INT12_MASK));
//
//
//}


/***************************************************************************//**
DMA driver for audio PWM data
@param
@return
@outputs
@context
*******************************************************************************/
void Audio_PWM_DMA_Init (void) {

  DISABLE_AUDIO;

  /* FTM2, Channel 1 */
  DMAMUX_CHCFG13 = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(31);

    /* Source address */
  DMA_TCD13_SADDR = (uint32_t) current_therapy_settings.audio.p_sound_file;

  DMA_TCD14_SADDR = (uint32_t) &ftm2_sync_cmd_word;

  /* Destination address */
  //DMA_TCD13_DADDR = 0x1FFF9000; // SRAM for testing
  //DMA_TCD14_DADDR = 0x1FFF9004; // SRAM for testing
  DMA_TCD13_DADDR = 0x4003A018;//0x400B8018; // FTM2_C1V
  DMA_TCD14_DADDR = 0x4003A058;//0x400B8058; // FTM2_SYNC

  /* Source offset disabled */
  DMA_TCD13_SOFF = 2;
  DMA_TCD14_SOFF = 0;

  /* Source and Destination Modulo off, source and destination size 0 = 8 bits */
  DMA_TCD13_ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);
  DMA_TCD14_ATTR = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);

  /* Transfer 2 bytes per transaction */
  DMA_TCD13_NBYTES_MLNO = 2;
  DMA_TCD14_NBYTES_MLNO = 4;

  /* No adjust needed */
  DMA_TCD13_SLAST = 0;
  DMA_TCD14_SLAST = 0;

  /* Destination offset -1 byte */
  DMA_TCD13_DOFF = 0;
  DMA_TCD14_DOFF = 0;

//  /* No link channel to channel, 1 transaction */
//  DMA_TCD13_CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK |
//                             DMA_CITER_ELINKYES_LINKCH(14) |
//                             DMA_CITER_ELINKYES_CITER(current_therapy_settings.audio.nsamples);
//  DMA_TCD14_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(1);
//
//  /* Adjustment destination address 4 bytes after major loop complete */
//  DMA_TCD13_DLASTSGA = 0;
//  DMA_TCD14_DLASTSGA = 0;
//
//  DMA_TCD13_BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK |
//                             DMA_BITER_ELINKYES_LINKCH(14) |
//                             DMA_BITER_ELINKYES_BITER(current_therapy_settings.audio.nsamples);
//  DMA_TCD14_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1);
//
//  DMA_TCD14_CSR = 0;//DMA_CSR_INTMAJOR_MASK;// | DMA_CSR_DREQ_MASK;
//
////  if (!loop)
////  {
////
////    DMA_TCD13_CSR |= DMA_CSR_DREQ_MASK;
////
////  }
//
//  DMA_CINT |= DMA_CINT_CINT(13);
//  DMA_CINT |= DMA_CINT_CINT(14);
//
//  DMA_TCD13_CSR = DMA_CSR_MAJORELINK_MASK |
//              DMA_CSR_MAJORLINKCH(14);
//
//  DMA_TCD13_SLAST = -(current_therapy_settings.audio.nsamples * 2);
//
//  Disable_IRQ(13);
//
//
//  DMA_SERQ = DMA_SERQ_SERQ(13); // FTM1_C0V
//  DMA_SERQ = DMA_SERQ_SERQ(14); // FTM1_SYNC

  //-------------
      /* No link channel to channel, 1 transaction */
  DMA_TCD13_CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK |
                             DMA_CITER_ELINKYES_LINKCH(14) |
                             DMA_CITER_ELINKYES_CITER(511);
  DMA_TCD14_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(1);

  /* Adjustment destination address 4 bytes after major loop complete */
  DMA_TCD13_DLASTSGA = 0;
  DMA_TCD14_DLASTSGA = 0;

  DMA_TCD13_BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK |
                             DMA_BITER_ELINKYES_LINKCH(14) |
                             DMA_BITER_ELINKYES_BITER(511);
  DMA_TCD14_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1);

  DMA_TCD14_CSR = 0;//DMA_CSR_INTMAJOR_MASK;// | DMA_CSR_DREQ_MASK;

//  if (!loop)
//  {
//
//    DMA_TCD13_CSR |= DMA_CSR_DREQ_MASK;
//
//  }
  //DMA_TCD13_SLAST = -(511 * 2);
  DMA_CINT |= DMA_CINT_CINT(13);
  DMA_CINT |= DMA_CINT_CINT(14);

  DMA_TCD13_CSR = DMA_CSR_MAJORELINK_MASK |
                  DMA_CSR_MAJORLINKCH(14)|
                  DMA_CSR_INTMAJOR_MASK |
                  DMA_CSR_DREQ_MASK;

  NVIC_EnableIRQ(DMA13_DMA29_IRQn);

  DMA_SERQ = DMA_SERQ_SERQ(14); // FTM1_SYNC
  DMA_SERQ = DMA_SERQ_SERQ(13); // FTM1_C0V

  /* Enable irq's */
  //  NVIC_EnableIRQ(14);
//  NVIC_EnableIRQ(12);

}

/***************************************************************************//**
Init DMA PWM for continuous audio output
@param number of samples
@return
@outputs
@context
*******************************************************************************/
void continuous_audio_pwm_dma_init (uint16_t nsamples)
{

  DISABLE_AUDIO;

  /* FTM2, Channel 1 */
  DMAMUX_CHCFG13 = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(31);

    /* Source address */
  DMA_TCD13_SADDR = (uint32_t) current_therapy_settings.audio.p_sound_file;

  DMA_TCD14_SADDR = (uint32_t) &ftm2_sync_cmd_word;

  /* Destination address */
  //DMA_TCD13_DADDR = 0x1FFF9000; // SRAM for testing
  //DMA_TCD14_DADDR = 0x1FFF9004; // SRAM for testing
  DMA_TCD13_DADDR = 0x4003A018;//0x400B8018; // FTM2_C1V
    DMA_TCD14_DADDR = 0x4003A058;//0x400B8058; // FTM2_SYNC

  /* Source offset disabled */
  DMA_TCD13_SOFF = 2;
  DMA_TCD14_SOFF = 0;

  /* Source and Destination Modulo off, source and destination size 0 = 8 bits */
  DMA_TCD13_ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);
  DMA_TCD14_ATTR = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);

  /* Transfer 2 bytes per transaction */
  DMA_TCD13_NBYTES_MLNO = 2;
  DMA_TCD14_NBYTES_MLNO = 4;

  /* No adjust needed */
  DMA_TCD13_SLAST = 0;
  DMA_TCD14_SLAST = 0;

  /* Destination offset -1 byte */
  DMA_TCD13_DOFF = 0;
  DMA_TCD14_DOFF = 0;

  /* No link channel to channel, 1 transaction */
  DMA_TCD13_CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK |
                             DMA_CITER_ELINKYES_LINKCH(14) |
                             DMA_CITER_ELINKYES_CITER(nsamples);
  DMA_TCD14_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(1);

  /* Adjustment destination address 4 bytes after major loop complete */
  DMA_TCD13_DLASTSGA = 0;
  DMA_TCD14_DLASTSGA = 0;

  DMA_TCD13_BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK |
                             DMA_BITER_ELINKYES_LINKCH(14) |
                             DMA_BITER_ELINKYES_BITER(nsamples);
  DMA_TCD14_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1);

  DMA_TCD14_CSR = 0;//DMA_CSR_INTMAJOR_MASK;// | DMA_CSR_DREQ_MASK;

//  if (!loop)
//  {
//
//    DMA_TCD13_CSR |= DMA_CSR_DREQ_MASK;
//
//  }

  DMA_CINT |= DMA_CINT_CINT(13);
  DMA_CINT |= DMA_CINT_CINT(14);

  DMA_TCD13_CSR = DMA_CSR_MAJORELINK_MASK |
              DMA_CSR_MAJORLINKCH(14);// |
              //DMA_CSR_INTMAJOR_MASK |
              //  DMA_CSR_DREQ_MASK;

  DMA_TCD13_SLAST = -(nsamples * 2);

  NVIC_DisableIRQ(DMA13_DMA29_IRQn);
  //NVIC_EnableIRQ(13);


  DMA_SERQ = DMA_SERQ_SERQ(14); // FTM1_SYNC
  DMA_SERQ = DMA_SERQ_SERQ(13); // FTM1_C0V


}


/***************************************************************************//**
Reset audio alarm
@param
@return
@outputs
@context
*******************************************************************************/
uint8_t reset_alarm_audio(void)
{

  current_therapy_settings.audio.ambient_alarm_page++;
  put_event(&evt_tbl,VOLUME_CTRL);

  /* reset index to track eof--dma x-fer limit is 512 samples / pg */
  if (current_therapy_settings.audio.ambient_alarm_page == 6)
  {
    //current_therapy_settings.audio.ambient_alarm_page = 0;
    current_therapy_settings.audio.p_sound_file = 0;
    DISABLE_AUDIO
  }

  /* test audio snippet stops here, otherwise reset dma */
  if (current_therapy_settings.gui.alarm_test_sound == 1)
  {

    current_therapy_settings.gui.alarm_test_sound = 0;
    current_therapy_settings.audio.ambient_alarm_page = 0;
    DISABLE_AUDIO

  }

  return 0;

}

/***************************************************************************//**
Setup audio volume control driver
@param
@return
@outputs
@context
*******************************************************************************/
uint8_t audio_vol_ctrl_init(void)
{

  volatile uint32_t temp;

  /* Enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;

  /* Disable write protection */
  FTM1_MODE |= FTM_MODE_WPDIS_MASK;

  FTM1_MODE |= FTM_MODE_FTMEN_MASK;

  FTM1_CNT = 0;

  FTM1_MOD = 0xFFF;

  FTM1_QDCTRL |= FTM_QDCTRL_QUADEN_MASK | FTM_QDCTRL_PHAFLTREN_MASK | FTM_QDCTRL_PHBFLTREN_MASK;

  FTM1_SC = FTM_SC_PS(0) | FTM_SC_CLKS(7);

  FTM1_FILTER = FTM_FILTER_CH0FVAL(15) | FTM_FILTER_CH1FVAL(15);

  /* Set Sound PWM Output for LM48100Q */
  PORTA_PCR12 = PORT_PCR_MUX(7); // FTM1_QD_PHA
  PORTA_PCR13 = PORT_PCR_MUX(7); // FTM1_QD_PHB

  return 1;

}

/***************************************************************************//**
Play Click
@param
@return
@outputs
@context
*******************************************************************************/
uint8_t audible_click(void)
{

  if (((current_therapy_settings.ambient.audio_enabled == 0) ||
       (current_therapy_settings.ambient.alarm_active == 0)) &&
      (current_therapy_settings.gui.alarm_test_sound == 0) &&
      ((current_therapy_settings.notify.status & 0xFF) == 0))
  {

    //uint32_t prebutton_click_audio;
    //uint8_t prebutton_click_nsamples;

    /* Stop playing currently loaded audio file */
    DISABLE_AUDIO;

    /* Turn volume up */
    //LM48100Q_I2C_Write(AUDIO_VOL2, 25);

  //  DISABLE_PIT(0);
  //
  //  /* Set 22.67 ms timeout value */
  //  PIT_LDVAL0 =  740000;
  //
  //  RESET_PIT(0);
  //
  //  while((PIT_TFLG0 & PIT_TFLG_TIF_MASK) != PIT_TFLG_TIF_MASK);

    /* Save pointer to currently loaded audio file to revert back after button click */
    //prebutton_click_audio = (uint32_t) &lib_lo_sound[0];

    /* Save number of pages for currently loaded audio file to revert back */
    //prebutton_click_nsamples = 337;


    /* Load pointer to button click audio file */
    //current_therapy_settings.audio.p_sound_file = (uint16_t *) &lib_click_sound;

    /* Calculate qty of pages in audio file */
    //current_therapy_settings.audio.num_pages = (sizeof(lib_click_sound) >> 1) / 500;
    current_therapy_settings.audio.p_sound_file = lib_sounds[6].p;
  //  current_therapy_settings.audio.p_end_sound_file = (uint16_t *) &lib_click_sound[1021];
  //  current_therapy_settings.audio.nsamples = 511;

    /* Set up DMA and kick-off playing file */
    //Audio_PWM_DMA_Init();


    /* FTM2, Channel 1 */
    DMAMUX_CHCFG13 = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(31);

      /* Source address */
    //DMA_TCD13_SADDR = (uint32_t) &lib_click_sound[0];
    DMA_TCD13_SADDR = (uint32_t) lib_sounds[6].p;

    DMA_TCD14_SADDR = (uint32_t) &ftm2_sync_cmd_word;

    /* Destination address */
    //DMA_TCD13_DADDR = 0x1FFF9000; // SRAM for testing
    //DMA_TCD14_DADDR = 0x1FFF9004; // SRAM for testing
    DMA_TCD13_DADDR = 0x4003A018;//0x400B8018; // FTM2_C1V
    DMA_TCD14_DADDR = 0x4003A058;//0x400B8058; // FTM2_SYNC

    /* Source offset disabled */
    DMA_TCD13_SOFF = 2;
    DMA_TCD14_SOFF = 0;

    /* Source and Destination Modulo off, source and destination size 0 = 8 bits */
    DMA_TCD13_ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);
    DMA_TCD14_ATTR = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);

    /* Transfer 2 bytes per transaction */
    DMA_TCD13_NBYTES_MLNO = 2;
    DMA_TCD14_NBYTES_MLNO = 4;

    /* No adjust needed */
    DMA_TCD13_SLAST = 0;
    DMA_TCD14_SLAST = 0;

    /* Destination offset -1 byte */
    DMA_TCD13_DOFF = 0;
    DMA_TCD14_DOFF = 0;

    /* No link channel to channel, 1 transaction */
    DMA_TCD13_CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK |
                               DMA_CITER_ELINKYES_LINKCH(14) |
                               DMA_CITER_ELINKYES_CITER(511);
    DMA_TCD14_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(1);

    /* Adjustment destination address 4 bytes after major loop complete */
    DMA_TCD13_DLASTSGA = 0;
    DMA_TCD14_DLASTSGA = 0;

    DMA_TCD13_BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK |
                               DMA_BITER_ELINKYES_LINKCH(14) |
                               DMA_BITER_ELINKYES_BITER(511);
    DMA_TCD14_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1);

    DMA_TCD14_CSR = 0;//DMA_CSR_INTMAJOR_MASK;// | DMA_CSR_DREQ_MASK;

    DMA_CINT |= DMA_CINT_CINT(13);
    DMA_CINT |= DMA_CINT_CINT(14);


    DMA_TCD13_CSR = DMA_CSR_MAJORELINK_MASK |
                    DMA_CSR_MAJORLINKCH(14) |
                    DMA_CSR_INTMAJOR_MASK |
                    DMA_CSR_DREQ_MASK;

    NVIC_EnableIRQ(DMA13_DMA29_IRQn);

    DMA_SERQ = DMA_SERQ_SERQ(14); // FTM1_SYNC
    DMA_SERQ = DMA_SERQ_SERQ(13); // FTM1_C0V
  }

  return 0;

}

/***************************************************************************//**
Plays the audio samples for the alarm sound. It uses the DMA to send the
sample data via the PWM to the speakers.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void audio_alarmsound(void)
{

  //uint32_t prebutton_click_audio;
  //uint8_t prebutton_click_nsamples;

  /* Stop playing currently loaded audio file */
  DISABLE_AUDIO;

  /* Turn volume up */
  //LM48100Q_I2C_Write(AUDIO_VOL2, 25);

//  DISABLE_PIT(0);
//
//  /* Set 22.67 ms timeout value */
//  PIT_LDVAL0 =  740000;
//
//  RESET_PIT(0);
//
//  while((PIT_TFLG0 & PIT_TFLG_TIF_MASK) != PIT_TFLG_TIF_MASK);

  /* Save pointer to currently loaded audio file to revert back after button click */
  //prebutton_click_audio = (uint32_t) &lib_lo_sound[0];

  /* Save number of pages for currently loaded audio file to revert back */
  //prebutton_click_nsamples = 337;


  /* Load pointer to button click audio file */
  //current_therapy_settings.audio.p_sound_file = (uint16_t *) &lib_click_sound;

  /* Calculate qty of pages in audio file */
  //current_therapy_settings.audio.num_pages = (sizeof(lib_click_sound) >> 1) / 500;
//  current_therapy_settings.audio.p_sound_file = (uint16_t *) &lib_click_sound[0];
//  current_therapy_settings.audio.p_end_sound_file = (uint16_t *) &lib_click_sound[1021];
//  current_therapy_settings.audio.nsamples = 511;

  /* Set up DMA and kick-off playing file */
  //Audio_PWM_DMA_Init();


  /* FTM2, Channel 1 */
  DMAMUX_CHCFG13 = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(31);

    /* Source address */
  //DMA_TCD13_SADDR = (uint32_t) &lib_ambient_alarm_sound[0];
  DMA_TCD13_SADDR = (uint32_t) lib_sounds[7].p;

  DMA_TCD14_SADDR = (uint32_t) &ftm2_sync_cmd_word;

  /* Destination address */
  //DMA_TCD13_DADDR = 0x1FFF9000; // SRAM for testing
  //DMA_TCD14_DADDR = 0x1FFF9004; // SRAM for testing
  DMA_TCD13_DADDR = 0x4003A018;//0x400B8018; // FTM2_C1V
    DMA_TCD14_DADDR = 0x4003A058;//0x400B8058; // FTM2_SYNC

  /* Source offset disabled */
  DMA_TCD13_SOFF = 2;
  DMA_TCD14_SOFF = 0;

  /* Source and Destination Modulo off, source and destination size 0 = 8 bits */
  DMA_TCD13_ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);
  DMA_TCD14_ATTR = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);

  /* Transfer 2 bytes per transaction */
  DMA_TCD13_NBYTES_MLNO = 2;
  DMA_TCD14_NBYTES_MLNO = 4;

  /* No adjust needed */
  DMA_TCD13_SLAST = 0;
  DMA_TCD14_SLAST = 0;

  /* Destination offset -1 byte */
  DMA_TCD13_DOFF = 0;
  DMA_TCD14_DOFF = 0;

  /* No link channel to channel, 1 transaction */
  DMA_TCD13_CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK |
                             DMA_CITER_ELINKYES_LINKCH(14) |
                             DMA_CITER_ELINKYES_CITER(511);
  DMA_TCD14_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(1);

  /* Adjustment destination address 4 bytes after major loop complete */
  DMA_TCD13_DLASTSGA = 0;
  DMA_TCD14_DLASTSGA = 0;

  DMA_TCD13_BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK |
                             DMA_BITER_ELINKYES_LINKCH(14) |
                             DMA_BITER_ELINKYES_BITER(511);
  DMA_TCD14_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1);

  DMA_TCD14_CSR = 0;//DMA_CSR_INTMAJOR_MASK;// | DMA_CSR_DREQ_MASK;

//  if (!loop)
//  {
//
//    DMA_TCD13_CSR |= DMA_CSR_DREQ_MASK;
//
//  }
  //DMA_TCD13_SLAST = -(511 * 2);
  DMA_CINT |= DMA_CINT_CINT(13);
  DMA_CINT |= DMA_CINT_CINT(14);

  DMA_TCD13_CSR = DMA_CSR_MAJORELINK_MASK |
                  DMA_CSR_MAJORLINKCH(14)|
                  DMA_CSR_INTMAJOR_MASK |
                  DMA_CSR_DREQ_MASK;

  NVIC_EnableIRQ(DMA13_DMA29_IRQn);

  DMA_SERQ = DMA_SERQ_SERQ(14); // FTM1_SYNC
  DMA_SERQ = DMA_SERQ_SERQ(13); // FTM1_C0V





  /* Wait for click audio file to play completely */
  //while((DMA_ERQ & DMA_ERQ_ERQ13_MASK) == DMA_ERQ_ERQ13_MASK);

//  DISABLE_PIT(0);
//
//  /* Set 22.67 ms timeout value */
//  PIT_LDVAL0 =  1678005;
//
//  RESET_PIT(0);
//
//  while((PIT_TFLG0 & PIT_TFLG_TIF_MASK) != PIT_TFLG_TIF_MASK);

  /* Update setting so you know file has finished */
  //current_therapy_settings.gui.button_click_sync = SYNCD;

  /* Reload pointer to original audio file */
  //current_therapy_settings.audio.p_sound_file = (uint16_t *) prebutton_click_audio;

  /* Reload number of pages in original audio file */
  //current_therapy_settings.audio.nsamples = prebutton_click_nsamples;

}


//void Audio_SplashSound(void)
//{
//
//  /* Stop playing currently loaded audio file */
//  DISABLE_AUDIO;
//
//  /* FTM2, Channel 1 */
//  DMAMUX0_CHCFG13 = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(31);
//
//    /* Source address */
//  DMA_TCD13_SADDR = (uint32_t) &lib_splash_sound[0];
//
//  DMA_TCD14_SADDR = (uint32_t) &ftm2_sync_cmd_word;
//
//  /* Destination address */
//  //DMA_TCD13_DADDR = 0x1FFF9000; // SRAM for testing
//  //DMA_TCD14_DADDR = 0x1FFF9004; // SRAM for testing
//    DMA_TCD13_DADDR = 0x4003A018;//0x400B8018; // FTM2_C1V
//    DMA_TCD14_DADDR = 0x4003A058;//0x400B8058; // FTM2_SYNC
//
//  /* Source offset disabled */
//  DMA_TCD13_SOFF = 2;
//  DMA_TCD14_SOFF = 0;
//
//  /* Source and Destination Modulo off, source and destination size 0 = 8 bits */
//  DMA_TCD13_ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);
//  DMA_TCD14_ATTR = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);
//
//  /* Transfer 2 bytes per transaction */
//  DMA_TCD13_NBYTES_MLNO = 2;
//  DMA_TCD14_NBYTES_MLNO = 4;
//
//  /* No adjust needed */
//  DMA_TCD13_SLAST = 0;
//  DMA_TCD14_SLAST = 0;
//
//  /* Destination offset -1 byte */
//  DMA_TCD13_DOFF = 0;
//  DMA_TCD14_DOFF = 0;
//
//  /* No link channel to channel, 1 transaction */
//  DMA_TCD13_CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK |
//                             DMA_CITER_ELINKYES_LINKCH(14) |
//                             DMA_CITER_ELINKYES_CITER(511);
//  DMA_TCD14_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(1);
//
//  /* Adjustment destination address 4 bytes after major loop complete */
//  DMA_TCD13_DLASTSGA = 0;
//  DMA_TCD14_DLASTSGA = 0;
//
//  DMA_TCD13_BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK |
//                             DMA_BITER_ELINKYES_LINKCH(14) |
//                             DMA_BITER_ELINKYES_BITER(511);
//  DMA_TCD14_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1);
//
//  DMA_TCD14_CSR = 0;//DMA_CSR_INTMAJOR_MASK;// | DMA_CSR_DREQ_MASK;
//
////  if (!loop)
////  {
////
////    DMA_TCD13_CSR |= DMA_CSR_DREQ_MASK;
////
////  }
//  //DMA_TCD13_SLAST = -(511 * 2);
//  DMA_CINT |= DMA_CINT_CINT(13);
//  DMA_CINT |= DMA_CINT_CINT(14);
//
//  DMA_TCD13_CSR = DMA_CSR_MAJORELINK_MASK |
//                  DMA_CSR_MAJORLINKCH(14)|
//                  DMA_CSR_INTMAJOR_MASK |
//                  DMA_CSR_DREQ_MASK;
//
//  NVIC_EnableIRQ(13);
//
//  DMA_SERQ = DMA_SERQ_SERQ(14); // FTM1_SYNC
//  DMA_SERQ = DMA_SERQ_SERQ(13); // FTM1_C0V
//
//
//
//}

/***************************************************************************//**
polls volume control
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t poll_volume_ctrl(void)
{
//
  uint8_t volume = 0;
  //static uint8_t last_volume;
  //static bool alarm_dB_write_success;
  uint8_t skew = 10;

  uint16_t audio_dB;
  uint8_t direction,highest_priority_error;
  static uint8_t previous_direction,inter_burst_count,previous_error=0;

  /* master volume controller */
  direction = ((FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK) >> FTM_QDCTRL_QUADIR_SHIFT) & 1;

  if (direction != previous_direction)
  {

    FTM1_CNT = 0;
    audio_dB = 0;

  }
  else
  {

    audio_dB = FTM1_CNT;

  }
  previous_direction = direction;
//  FTM1_CNT = 0;

  if ((direction == 1) && (audio_dB >= 2))
  {
    FTM1_CNT = 0;
    if (current_therapy_settings.audio.volume < 10)
    {

      current_therapy_settings.audio.volume++;


    }
    if (current_therapy_settings.ambient.volume < 10)
    {

      current_therapy_settings.ambient.volume++;

    }
    //current_therapy_settings.gui.button_click_sync = PENDING;
    put_event(&evt_tbl,CLICK_SOUND);

  }
  else if ((direction == 0) && (audio_dB <= (FTM1_MOD - 1)) && (audio_dB != 0))
  {

    FTM1_CNT = 0;

    if (current_therapy_settings.audio.volume > 1)
    {

      current_therapy_settings.audio.volume--;


    }
    if (current_therapy_settings.ambient.volume > 1)
    {

      current_therapy_settings.ambient.volume--;

    }
    //current_therapy_settings.gui.button_click_sync = PENDING;
    put_event(&evt_tbl,CLICK_SOUND);

  }

  volume = (current_therapy_settings.audio.volume << 1) + skew;

  highest_priority_error = current_therapy_settings.notify.status & 0xFF;
  
  if (current_therapy_settings.sw.shutdown_imminent == TRUE)
  {

    volume = 0;

  }
  else if (highest_priority_error != 0)
  {

    if ((current_therapy_settings.audio.p_sound_file != lib_sounds[8].p) ||
        (highest_priority_error != previous_error))
    {

      current_therapy_settings.audio.error_burst_length = 0;
      current_therapy_settings.audio.p_sound_file = lib_sounds[8].p;
      //current_therapy_settings.audio.nsamples = lib_sounds[8].n;
      current_therapy_settings.audio.ambient_alarm_page = 0;

      DISABLE_AUDIO;
      FTM2_MOD = AUDIO_PWM_MODULO-1;
      Audio_PWM_DMA_Init();

    }

  }
  else if (current_therapy_settings.audio.p_sound_file == lib_sounds[12].p)
  {


    if (current_therapy_settings.audio.splash_volume < 125)
    {

      current_therapy_settings.audio.splash_volume++;
      //volume = current_therapy_settings.audio.splash_volume / 5;
      volume = (current_therapy_settings.audio.volume << 1) + skew;
      
    }
    else if (current_therapy_settings.audio.splash_volume >= 825)
    {

      volume = 0;
      DISABLE_AUDIO;
      current_therapy_settings.audio.p_sound_file = 0;
      /* Set Period */
      FTM2_MOD = AUDIO_PWM_MODULO-1;

    }
    else if (current_therapy_settings.audio.splash_volume >= 125)
    {

      current_therapy_settings.audio.splash_volume += 2;
      //volume = 25 - ((current_therapy_settings.audio.splash_volume - 625) / 25);
      //volume = 25;
       volume = (current_therapy_settings.audio.volume << 1) + skew;
    }

  }
  /* alarm volume controller */
  else if ((current_therapy_settings.ambient.audio_enabled == 1) &&
      //(current_therapy_settings.ambient.enable == 1) &&
      (current_therapy_settings.ambient.alarm_active == 1))
  {

    //if ((current_therapy_settings.audio.ambient_alarm_page == 0) &&
    //   (current_therapy_settings.audio.p_sound_file != (uint16_t *) &lib_ambient_alarm_sound[0]))
    if ((current_therapy_settings.audio.ambient_alarm_page == 0) &&
        (current_therapy_settings.audio.p_sound_file != lib_sounds[7].p))
    {

      //current_therapy_settings.audio.p_sound_file = (uint16_t *) &lib_ambient_alarm_sound[0];
      current_therapy_settings.audio.p_sound_file = lib_sounds[7].p;
      //current_therapy_settings.audio.nsamples = lib_sounds[7].n;

      DISABLE_AUDIO;

      audio_alarmsound();

      inter_burst_count = 0;

    }

    if ((current_therapy_settings.audio.ambient_alarm_page == 0) ||
        (current_therapy_settings.audio.ambient_alarm_page == 2) ||
        (current_therapy_settings.audio.ambient_alarm_page == 4))
    {

      volume = (current_therapy_settings.ambient.volume << 1);
      volume += skew;
      //volume = 25;

    }

    else if ((current_therapy_settings.audio.ambient_alarm_page == 1) ||
             (current_therapy_settings.audio.ambient_alarm_page == 3) ||
             (current_therapy_settings.audio.ambient_alarm_page == 5))
    {

      volume = 0;

    }

    else if ((current_therapy_settings.audio.ambient_alarm_page == 6) &&
             (current_therapy_settings.treatment.ablate_on == 1) &&
             (current_therapy_settings.audio.p_sound_file == 0))
    {
      /* Ablate */
      if(current_therapy_settings.active_wand.lw_active == 1) /* Legacy wands */
      {
        current_therapy_settings.audio.p_sound_file = lib_sounds[MED].p;
      }
      else /* FLOW wands */
      {
        current_therapy_settings.audio.p_sound_file = lib_sounds[current_therapy_settings.treatment.therapy_mode].p; 
      }
      DISABLE_AUDIO;
      Audio_PWM_DMA_Init();
      volume = (current_therapy_settings.audio.volume << 1) + skew;
    }

    else if ((current_therapy_settings.audio.ambient_alarm_page == 6) &&
             (current_therapy_settings.treatment.coag_on == 1) &&
             (current_therapy_settings.audio.p_sound_file == 0))
    {

      /* coag */
      current_therapy_settings.audio.p_sound_file = lib_sounds[5].p;
      continuous_audio_pwm_dma_init(lib_sounds[5].n);

      volume = (current_therapy_settings.audio.volume << 1) + skew;

    }

    else if (current_therapy_settings.audio.ambient_alarm_page == 6)
    {

      inter_burst_count++;

      if (current_therapy_settings.wand.tube_warn == TRUE)
      {

      volume = 0;

      }
      else if ((current_therapy_settings.treatment.coag_on == 1) ||
          (current_therapy_settings.treatment.ablate_on == 1))
      {

        volume = (current_therapy_settings.audio.volume << 1) + skew;

      }
      else
      {

        volume = 0;

      }

    }

    if (inter_burst_count >= 250)
    {

      current_therapy_settings.audio.ambient_alarm_page = 0;

    }

  }
//  else if ((current_therapy_settings.audio.p_sound_file == (uint16_t *) &lib_ambient_alarm_sound[0]) &&
//          (current_therapy_settings.ambient.alarm_active == 0) &&
//          (current_therapy_settings.gui.alarm_test_sound == 0))
  else if ((current_therapy_settings.audio.p_sound_file == lib_sounds[7].p) &&
           (current_therapy_settings.ambient.alarm_active == 0) &&
           (current_therapy_settings.gui.alarm_test_sound == 0))
  {

    DISABLE_AUDIO;
    current_therapy_settings.audio.p_sound_file = 0;

  }
//  else if ((current_therapy_settings.ambient.audio_enabled == 0) &&
//           (current_therapy_settings.ambient.alarm_active == 1) &&
//           (current_therapy_settings.audio.p_sound_file == (uint16_t *) &lib_ambient_alarm_sound[0]))
  else if ((current_therapy_settings.ambient.audio_enabled == 0) &&
           (current_therapy_settings.ambient.alarm_active == 1) &&
           (current_therapy_settings.audio.p_sound_file == lib_sounds[7].p))
  {

//    volume = 0;
    current_therapy_settings.audio.ambient_alarm_page = 0;
    current_therapy_settings.audio.p_sound_file = 0;

  }
  else if (current_therapy_settings.gui.alarm_test_sound == 1)
  {

    volume = (current_therapy_settings.ambient.volume << 1) + skew;

  }
  else if (current_therapy_settings.wand.tube_warn == TRUE)
  {

    volume = 0;

  }
  else
  {
    if ((current_therapy_settings.treatment.coag_on == 1) && (current_therapy_settings.treatment.ent_turbo_mode == TRUE))
    {
      if(current_therapy_settings.audio.p_sound_file != lib_sounds[5].p)
      {
        //AUDIO_PWM_MODULO = 1088; //68kHz tone
        DISABLE_AUDIO;
        FTM2_MOD = 1088-1;
        current_therapy_settings.audio.p_sound_file = lib_sounds[5].p;
        continuous_audio_pwm_dma_init(lib_sounds[5].n);
        //DISABLE_AUDIO;
        //Audio_PWM_DMA_Init();
      }
    }
    else if (current_therapy_settings.treatment.coag_on == 1)
    {
      
      /* trump mode change burst and silence by setting count down to zero */
      current_therapy_settings.audio.change_mode_burst_length = 0;
      
      if (current_therapy_settings.audio.p_sound_file != lib_sounds[5].p)
      {
        current_therapy_settings.audio.p_sound_file = lib_sounds[5].p;
        continuous_audio_pwm_dma_init(lib_sounds[5].n);
        
      }

    }
    else if (current_therapy_settings.treatment.ablate_on == 1)
    {

      /* trump mode change burst and silence by setting count down to zero */
      current_therapy_settings.audio.change_mode_burst_length = 0;

      if (current_therapy_settings.active_wand.lw_active == 1)
      {
      
        if (current_therapy_settings.audio.p_sound_file != lib_sounds[MED].p)
        {
          current_therapy_settings.audio.p_sound_file = lib_sounds[MED].p;
          DISABLE_AUDIO;
          Audio_PWM_DMA_Init();
          
        }
        
      }
      else if (current_therapy_settings.audio.p_sound_file != lib_sounds[current_therapy_settings.treatment.therapy_mode].p)
      {
        current_therapy_settings.audio.p_sound_file = lib_sounds[current_therapy_settings.treatment.therapy_mode].p;
        //current_therapy_settings.audio.nsamples = lib_sounds[current_therapy_settings.treatment.therapy_mode].n;
        //if (current_therapy_settings.treatment.therapy_mode == VAC)
        //{

          DISABLE_AUDIO;
          Audio_PWM_DMA_Init();

//        }
//        else
//        {
//
//          continuous_audio_pwm_dma_init(lib_sounds[current_therapy_settings.treatment.therapy_mode].n);
//
//        }

        

      }

    }
    else if ((current_therapy_settings.audio.p_sound_file == lib_sounds[11].p) ||
             (current_therapy_settings.audio.p_sound_file == lib_sounds[10].p) ||
             (current_therapy_settings.audio.p_sound_file == lib_sounds[9].p) ||
             (current_therapy_settings.audio.p_sound_file == lib_sounds[6].p))

    {
      
      volume = (current_therapy_settings.audio.volume << 1) + skew;
      return volume;
      
    }
    else if ((current_therapy_settings.audio.p_sound_file != lib_sounds[1].p) &&
             (current_therapy_settings.audio.p_sound_file != lib_sounds[2].p) &&
             (current_therapy_settings.audio.p_sound_file != lib_sounds[3].p))

    {

      /* turn audio off */
      current_therapy_settings.audio.p_sound_file = 0;
      DISABLE_AUDIO;

    }
    else if (current_therapy_settings.audio.change_mode_burst_length > 0)
    {
      
      current_therapy_settings.audio.change_mode_burst_length--;
      
      if (current_therapy_settings.treatment.therapy_mode == MED)
      {

        if ((current_therapy_settings.audio.change_mode_burst_length < 32) &&
            (current_therapy_settings.audio.change_mode_burst_length >= 16))
        {
          
          volume = 0;
          return volume;
          
        }

      }
      else if (current_therapy_settings.treatment.therapy_mode == HI)
      {

        if ((current_therapy_settings.audio.change_mode_burst_length < 40) &&
            (current_therapy_settings.audio.change_mode_burst_length >= 30))
        {
          
          volume = 0;
          return volume;
          
        }
        else if ((current_therapy_settings.audio.change_mode_burst_length < 20) &&
                 (current_therapy_settings.audio.change_mode_burst_length >= 10))
        {
          
          volume = 0;
          return volume;
          
        }

      }
      
      if (current_therapy_settings.audio.change_mode_burst_length == 1)
      {
      
        /* turn audio off */
        current_therapy_settings.audio.p_sound_file = 0;
        current_therapy_settings.audio.change_mode_burst_length = 0;
        DISABLE_AUDIO;
        
      }
      
    }
    else 
    {

      /* turn audio off */
      current_therapy_settings.audio.p_sound_file = 0;
      DISABLE_AUDIO;

    }

//    if ((current_therapy_settings.ambient.thermometer_color == 1) &&
//        ((current_therapy_settings.treatment.ablate_on == 1) ||
//         (current_therapy_settings.treatment.coag_on == 1)))
//    {
//
//      if (burst_length >= 10)
//      {
//
//        skew += 4;
//
//      }
//
//      if (burst_length > 20)
//      {
//
//        burst_length = 0;
//
//      }
//      else
//      {
//
//        burst_length++;
//
//      }
//
//    }

    volume = (current_therapy_settings.audio.volume << 1) + skew;

  }

  previous_error = highest_priority_error;
  
  //LM48100Q_I2C_Write(AUDIO_VOL2, volume);

  return volume;

}


//uint8_t audible_click_volume(void)
//{
//
//  LM48100Q_I2C_Write(AUDIO_VOL2, 25);
//
//}

/***************************************************************************//**
Plays the change mode sound file
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t play_change_mode_audio(void)
{

  /* lookup table of just settings type screens */
  const uint8_t settings_screens[MAX_NUM_SCREENS] =
  {
    /* 0-6   */  0,0,0,0,0,0,0,
    /* 7     */  DELETE_INDIVIDUAL_USER_PROFILE_CHANCE_TO_BACK_OUT_SCREEN,
    /* 8     */  SET_LEVEL_SCREEN,
    /* 9     */  0,
    /* 10    */  MAIN_SETTINGS_SCREEN,
    /* 11    */  PROFILE_SAVE_SCREEN,
    /* 12    */  USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN,
    /* 13    */  USER_PROFILES_SCREEN,
    /* 14    */  ENTER_NEW_USER_PROFILE_NAME_SCREEN,
    /* 15-17 */  0,0,0,
    /* 18    */  SWITCHING_SETTINGS_SCREEN,
    /* 19    */  LANGUAGES_SCREEN,
    /* 20    */  WAND_INFO_SCREEN,
    /* 21-39 */  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 40    */  RF_2000_WAND_TREATMENT_SCREEN,
    /* 41-51 */  0,0,0,0,0,0,0,0,0,0,0,
    /* 52    */  ENT_SMART_WAND_TREATMENT_SCREEN,
    /* 53    */  MANTA_SMART_WAND_TREATMENT_SCREEN,
  };
  
  if (settings_screens[current_therapy_settings.gui.screenID] == current_therapy_settings.gui.screenID)
  {

  
    if (((current_therapy_settings.ambient.audio_enabled == 0) ||
         (current_therapy_settings.ambient.alarm_active == 0)) &&
         (current_therapy_settings.notify.status == 0) &&
         (current_therapy_settings.notify.status2 == 0))
    {


      if (current_therapy_settings.treatment.therapy_mode == LO)
      {

        if (current_therapy_settings.gui.languageID == ENGLISH)
        {
         
          current_therapy_settings.audio.p_sound_file = lib_sounds[11].p;
          
        }
        else
        {
          
          current_therapy_settings.audio.p_sound_file = lib_sounds[1].p;
          current_therapy_settings.audio.change_mode_burst_length = 50;
          
        }

      }
      else if (current_therapy_settings.treatment.therapy_mode == MED)
      {

        if (current_therapy_settings.gui.languageID == ENGLISH)
        {
         
          current_therapy_settings.audio.p_sound_file = lib_sounds[10].p;
          
        }
        else
        {
          
          current_therapy_settings.audio.p_sound_file = lib_sounds[2].p;
          current_therapy_settings.audio.change_mode_burst_length = 50;
          
        }

      }
      else if (current_therapy_settings.treatment.therapy_mode == HI)
      {

        if (current_therapy_settings.gui.languageID == ENGLISH)
        {
         
          current_therapy_settings.audio.p_sound_file = lib_sounds[9].p;
          
        }
        else
        {
          
          current_therapy_settings.audio.p_sound_file = lib_sounds[3].p;
          current_therapy_settings.audio.change_mode_burst_length = 50;
          
        }

      }

      DISABLE_AUDIO;
      Audio_PWM_DMA_Init();
  //    current_therapy_settings.audio.change_mode_burst_length = 50;

    }
    
  }

  return 0;

}


/***************************************************************************//**
Loads default volume
@param
@return
@outputs
@context
*******************************************************************************/
void load_default_volume(uint8_t master_value, uint8_t ambient_value)
{

  if (master_value > 10)
  {

    current_therapy_settings.audio.volume = 5;

  }
  else if (master_value > 1)
  {

    current_therapy_settings.audio.volume = master_value;

  }
  else
  {

    current_therapy_settings.audio.volume = 1;

  }

  if (ambient_value > 10)
  {

    current_therapy_settings.ambient.volume = 5;

  }
  else if (ambient_value > 1)
  {

    current_therapy_settings.ambient.volume = ambient_value;

  }
  else
  {

   current_therapy_settings.ambient.volume = 1;

  }

}
