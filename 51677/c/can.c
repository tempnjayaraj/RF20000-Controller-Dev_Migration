/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************
$Author:: Harshil.Shah    $: Author
$Rev:: 2798               $: Revision of last commit
$Date:: 2021-11-17 02:10:#$: Date of last commit

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.30.4.3295
*******************************************************************************/

/***************************************************************************//**
@file      can.c
@brief     FlexCAN driver.
@details   Controls the CAN bus which is used to interact with the GUI section
of the board.
NOTE: Baud rate set to 1Mbps.
*******************************************************************************/

#include "can.h"
#include "cpu.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rf.h"
#include "wand.h"
#include "sw_manager.h"

const static CAN_MemMapPtr pFlexCAN = CAN0_BASE_PTR;
extern legacy_wand_params legacy_wand_params_obj;
CAN_MB mb_buffer;
extern uint8_t current_coag_setpoint;

/***************************************************************************//**
Initialize FlexCAN.
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t flexcan_init(void)
{

  uint8_t i;

  /* Enable gate clock for FlexCAN0 */
  SIM_SCGC6 |= SIM_SCGC6_FLEXCAN0_MASK;

  /* Enable external reference clock for FlexCAN*/
  OSC_CR |= OSC_CR_ERCLKEN_MASK;

  /* Configure Port Pin Control for CAN */
  PORTB_PCR18 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK; // CAN0_TX
  PORTB_PCR19 = PORT_PCR_MUX(2); // CAN0_RX

  /* Use 32MHz oscillator clock source */
  pFlexCAN->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;

  /* Permit entry to freeze mode */
  pFlexCAN->MCR |= CAN_MCR_FRZ_MASK;

  /* Enable FlexCAN */
  pFlexCAN->MCR &= ~CAN_MCR_MDIS_MASK;

#ifndef TEST 
  /* Wait for low power mode acknowledge */
  while(pFlexCAN->MCR & CAN_MCR_LPMACK_MASK);
#endif
  /* Soft Reset */
  pFlexCAN->MCR ^= CAN_MCR_SOFTRST_MASK;
  
#ifndef TEST   
  while(pFlexCAN->MCR & CAN_MCR_SOFTRST_MASK);
#endif
  /* Enter freeze mode */
  //pFlexCAN->MCR |= CAN_MCR_HALT_MASK;

#ifndef TEST 
  /* Check its in Freeze Mode */
  while(!(CAN_MCR_FRZACK_MASK & pFlexCAN->MCR));
#endif
  /* Disable Self-Reception */
  pFlexCAN->MCR |= CAN_MCR_SRXDIS_MASK;

  /* Enable Individual Rx Masking */
  pFlexCAN->MCR |= CAN_MCR_IRMQ_MASK;

  /* Initialize all 16 MBs */
  for(i=0;i<NUMBER_OF_CAN_MB;i++)
  {
      pFlexCAN->MB[i].CS = 0;
      pFlexCAN->MB[i].ID = 0;
      pFlexCAN->MB[i].WORD0 = 0;
      pFlexCAN->MB[i].WORD1 = 0;
      pFlexCAN->RXIMR[i] = 0;
  }


  /* Clear all interrupt bits */
  pFlexCAN->IFLAG1 = CAN_IFLAG1_BUF0I_MASK |
                     CAN_IFLAG1_BUF4TO1I_MASK |
                     CAN_IFLAG1_BUF5I_MASK |
                     CAN_IFLAG1_BUF6I_MASK |
                     CAN_IFLAG1_BUF7I_MASK |
                     CAN_IFLAG1_BUF31TO8I_MASK;

  /* Enable Rx FIFO structure */
  pFlexCAN->MCR |= CAN_MCR_RFEN_MASK;

  /* Set Baud Rate */
  pFlexCAN->CTRL1 |= (CAN_CTRL1_PROPSEG(0) | CAN_CTRL1_RJW(1)|
                         CAN_CTRL1_PSEG1(7) | CAN_CTRL1_PSEG2(5) |
                         CAN_CTRL1_PRESDIV(3));

  //pFlexCAN->CTRL2 |= RFFN;

  /* Initialize mask registers */
  CAN0_RXIMR8 = 0x1FFFFFFF; // Check all 29-bits.

  /* De-assert Freeze Mode */
  pFlexCAN->MCR &= ~CAN_MCR_HALT_MASK;

#ifndef TEST
  /* Wait till exit of freeze mode */
  while(pFlexCAN->MCR & CAN_MCR_FRZACK_MASK);
#endif  

#ifndef TEST
  /* Wait till ready */
  while(pFlexCAN->MCR & CAN_MCR_NOTRDY_MASK);
#endif

  pFlexCAN->IMASK1 = 0x20; // rx interrupt

  /* Enable interrupt */
  NVIC_EnableIRQ(CAN0_ORed_Message_buffer_IRQn);

  return 1;

}


/***************************************************************************//**
Write to FlexCAN.
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t flexCAN_write_fast_msgs(void) {

  uint32_t word0, word1;
  uint8_t *pword0 = (uint8_t*) &word0;
  uint8_t *pword1 = (uint8_t*) &word1;

  /* pointer to rtc_tsr register */
  uint32_t *p_rtc = (uint32_t*) 0x4003D000;

  /* !!! DEBUG ONLY PURPOSES !!! */
//  current_therapy_settings.wand_name.char0 = 0x53; /* S */
//  current_therapy_settings.wand_name.char1 = 0x68; /* h */
//  current_therapy_settings.wand_name.char2 = 0x6F; /* o */
//  current_therapy_settings.wand_name.char3 = 0x75; /* u */
//  current_therapy_settings.wand_name.char4 = 0x6C; /* l */
//  current_therapy_settings.wand_name.char5 = 0x64; /* d */
//  current_therapy_settings.wand_name.char6 = 0x65; /* e */
//  current_therapy_settings.wand_name.char7 = 0x72; /* r */
//  current_therapy_settings.wand_name.char8 = 0x20; /*   */
//  current_therapy_settings.wand_name.char9 = 0x57; /* W */
//  current_therapy_settings.wand_name.char10 = 0x61; /* a */
//  current_therapy_settings.wand_name.char11 = 0x6E; /* n */
//  current_therapy_settings.wand_name.char12 = 0x64; /* d */
//  current_therapy_settings.wand_name.char13 = 0x20; /*   */
//  current_therapy_settings.wand_name.char14 = 0x20; /*   */
//  current_therapy_settings.wand_name.char15 = 0x20; /*   */

  uint8_t disable_can_tx_jmpr = ~(GPIOB_PDIR >> 9) & 1;

  if (disable_can_tx_jmpr == 0)
  {
    uint8_t therapy_mode = 0;
      
    //current_therapy_settings.sw.debug_mode = TRUE;
    if (current_therapy_settings.treatment.ablate_enable == TRUE)
    {
      therapy_mode = current_therapy_settings.treatment.therapy_mode;
    }
    
  PWORD_CLEAR(pword0, pword1);

/******************************************************************************
* Message Name: GEN Watchdog Message
* Arbitration ID: 0x80
******************************************************************************/

  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(0)) == CAN_IFLAG1_BUF31TO8I(0))
  {

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(0);

  }

  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_WDOG_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {

    pFlexCAN->MB[GEN_WDOG_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

  }

  /* Step 3: Verify transmissin complete or aborted */
  if (((pFlexCAN->MB[GEN_WDOG_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_WDOG_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_WDOG_MSG_MB].CS) == CAN_CS_CODE(0)))
  {

    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(0);

    (*(uint32_t *)pword0) =
      ((current_therapy_settings.gui.tx_count << CAN_GEN_TXCOUNT_SHIFT) |
       (current_therapy_settings.gui.tx_wdog_err << CAN_GEN_WDOG_ERROR_SHIFT));
    swap_4bytes(pword0);

    (*(uint32_t *)pword1) = *p_rtc;
    swap_4bytes(pword1);

    pFlexCAN->MB[GEN_WDOG_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
    pFlexCAN->MB[GEN_WDOG_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(0x80);
    pFlexCAN->MB[GEN_WDOG_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_WDOG_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Start transmission */
    pFlexCAN->MB[GEN_WDOG_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

    PWORD_CLEAR(pword0, pword1);

    /* Increment transmit count */
    current_therapy_settings.gui.tx_count++;

  }


/******************************************************************************
* Message Name: GEN Therapy Message
* Arbitration ID: 0x81
******************************************************************************/

  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(1)) == CAN_IFLAG1_BUF31TO8I(1))
  {

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(1);

  }

  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_THERAPY_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {

    pFlexCAN->MB[GEN_THERAPY_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

  }

  /* Step 3: Verify transmissin complete or aborted */
  if (((pFlexCAN->MB[GEN_THERAPY_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_THERAPY_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_THERAPY_MSG_MB].CS) == CAN_CS_CODE(0)))
  {

    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(1);

    (*(uint32_t *)pword0) =
      (current_therapy_settings.setpoint.lo << CAN_GEN_CUT_LO_SP_SHIFT) |
      (current_therapy_settings.setpoint.med << CAN_GEN_CUT_MED_SP_SHIFT) |
      (current_therapy_settings.setpoint.hi << CAN_GEN_CUT_HI_SP_SHIFT) |
      (current_therapy_settings.setpoint.coag << CAN_GEN_COAG_SP_SHIFT) |
      (therapy_mode << CAN_GEN_THERAPY_MODE_SHIFT) |      
      ((legacy_wand_params_obj.timer.current/1000) << CAN_GEN_REFLEX_TIME_SHIFT) |
      (smart_wand_params.mfg.coag_enable << CAN_GEN_COAG_EN_SHIFT) |
      (current_therapy_settings.legacy_wand_levels.legacy_wand_coag_level << CAN_GEN_COAG18_SP) |
      (current_therapy_settings.legacy_wand_levels.legacy_wand_ablate_level << CAN_GEN_CUT18_SP) |
      (current_therapy_settings.treatment.wireless_enable << CAN_GEN_WIRELESS_FOOTPEDAL_EN) |
      (current_therapy_settings.treatment.finger_switch_en << CAN_GEN_FINGER_SWITCH_EN) |
      (current_therapy_settings.treatment.ablate_on << CAN_GEN_CUT_ON_SHIFT) |
      (current_therapy_settings.treatment.coag_on << CAN_GEN_COAG_ON_SHIFT) |
      (current_therapy_settings.legacy_wand_levels.topaz_timer << CAN_GEN_TIMER_EN_SHIFT) |
      (current_therapy_settings.ambient.enable << CAN_GEN_AMBIENT_EN_SHIFT) |
      (current_therapy_settings.ambient.thermometer_color  << CAN_GEN_AMBIENT_COLOR_SHIFT);
    swap_4bytes(pword0);
    (*(uint32_t *)pword1) =
      (current_therapy_settings.ambient.temperature << CAN_GEN_AMBIENT_TEMP_SHIFT) |
      (current_therapy_settings.ambient.capable << CAN_GEN_AMBIENT_CAPABLE_SHIFT) |
      (current_therapy_settings.treatment.flow_rate << CAN_GEN_FLOW_RATE_SHIFT) |
      (current_therapy_settings.treatment.flow_auto << CAN_GEN_FLOW_RATE_AUTO_SHIFT) |
      (current_therapy_settings.footpedal.present << CAN_GEN_FOOTPEDAL_CONNECTED_SHIFT) |
      (current_therapy_settings.footpedal.wireless.present << CAN_GEN_WIRELESS_FOOTPEDAL_CONNECTED_SHIFT) |
      (current_therapy_settings.treatment.ent_turbo_mode << CAN_GEN_ENT_TURBO_MODE) |
      (current_therapy_settings.ambient.alarm_threshold << CAN_GEN_AMIBENT_NOTIFICATION_THRESHOLD_SHIFT) | 
      ((current_therapy_settings.setpoint.coag >> 2) << CAN_GEN_COAG_SP_EXTENDED_SHIFT) |
        (current_therapy_settings.treatment.pre_prime << CAN_GEN_PRE_PRIME_SHIFT) |
          (current_therapy_settings.treatment.prime_glow << CAN_GEN_GLOW_PRIME_SHIFT); 
     swap_4bytes(pword1);


    pFlexCAN->MB[GEN_THERAPY_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
    pFlexCAN->MB[GEN_THERAPY_MSG_MB].ID = CAN_ID_PRIO(1) | CAN_ID_STD(0x81);
    pFlexCAN->MB[GEN_THERAPY_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_THERAPY_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Start transmission */
    pFlexCAN->MB[GEN_THERAPY_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

    PWORD_CLEAR(pword0, pword1);

  }


/******************************************************************************
* Message Name: GEN Graphical Managment Message
* Arbitration ID: 0x82
******************************************************************************/

  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(2)) == CAN_IFLAG1_BUF31TO8I(2))
  {

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(2);

  }

  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {

    pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

  }

  /* Step 3: Verify transmissin complete or aborted */
  if (((pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
        ((pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(2);   
    
    (*(uint32_t *)pword0) =
      ((current_therapy_settings.gui.screenID << CAN_GEN_SCREEN_ID_SHIFT) |
       (current_therapy_settings.gui.languageID << CAN_GEN_LANGUAGE_ID_SHIFT) |
         (current_therapy_settings.gui.save_button_en << CAN_GEN_SAVE_EN_SHIFT) |
           (current_therapy_settings.user_data.settings.new_en << CAN_GEN_NEW_EN_SHIFT) |
             (current_therapy_settings.footpedal.wireless.handshake_phase << CAN_GEN_WIRELESS_HANDSHAKE_PHASE_SHIFT) |
               (current_therapy_settings.user_data.settings.active_idx << CAN_GEN_PROFILE_ID_SHIFT));
     swap_4bytes(pword0);
     
     (*(uint32_t *)pword1) = (smart_wand_params.mfg.indication << CAN_GEN_WAND_INDICATION_SHIFT);
     swap_4bytes(pword1);  
     
     pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
     pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].ID = CAN_ID_PRIO(2) | CAN_ID_STD(0x82);
     pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].WORD0 = (*(uint32_t *)pword0);
     pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].WORD1 = (*(uint32_t *)pword1);
     
     pFlexCAN->MB[GEN_GRAPHIC_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
     
     PWORD_CLEAR(pword0, pword1);
     
  }


/******************************************************************************
* Message Name: GEN Software Status Message
* Arbitration ID: 0x83
******************************************************************************/

  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(3)) == CAN_IFLAG1_BUF31TO8I(3))
  {

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);

  }

  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_SS_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {

    pFlexCAN->MB[GEN_SS_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

  }

  /* Step 3: Verify transmissin complete or aborted */
  if (((pFlexCAN->MB[GEN_SS_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_SS_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_SS_MSG_MB].CS) == CAN_CS_CODE(0)))
  {

    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);

    (*(uint32_t *)pword0) =
       ((pheader->meta.app_version_major << CAN_GEN_FIRMWARE_VERSION_MAJOR_SHIFT) |
        (pheader->meta.app_version_minor << CAN_GEN_FIRMWARE_VERSION_MINOR_SHIFT) |
        (current_therapy_settings.sdhc.card_loaded  << CAN_GEN_FW_UPDATE_CARD_LOADED_SHIFT) |
        (current_therapy_settings.sdhc.update_uis << CAN_GEN_FW_UPDATE_UIS_EN_SHIFT) |
        (current_therapy_settings.sw.debug_mode << CAN_GEN_DEBUG_MODE_FLAG_SHIFT) |
        (current_therapy_settings.sw.shutdown_imminent << CAN_GEN_SHUTDOWN_SHIFT) |
        (current_therapy_settings.sw.update_progress << CAN_GEN_FW_UPDATE_GS_PROGRESS_SHIFT));
    swap_4bytes(pword0);

    pFlexCAN->MB[GEN_SS_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
    pFlexCAN->MB[GEN_SS_MSG_MB].ID = CAN_ID_PRIO(3) | CAN_ID_STD(0x83);
    pFlexCAN->MB[GEN_SS_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_SS_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    pFlexCAN->MB[GEN_SS_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

    PWORD_CLEAR(pword0, pword1);

  }

  /******************************************************************************
* Message Name: GEN Alarm Status Message
* Arbitration ID: 0x84
******************************************************************************/

  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(4)) == CAN_IFLAG1_BUF31TO8I(4))
  {

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(4);

  }

  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_ALARM_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {

    pFlexCAN->MB[GEN_ALARM_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

  }

  /* Step 3: Verify transmissin complete or aborted */
  if (((pFlexCAN->MB[GEN_ALARM_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_ALARM_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_ALARM_MB].CS) == CAN_CS_CODE(0)))
  {

    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(4);

    (*(uint32_t *)pword0) = current_therapy_settings.notify.status;

    swap_4bytes(pword0);

    (*(uint32_t *)pword1) = current_therapy_settings.notify.status2;

    swap_4bytes(pword1);

  pFlexCAN->MB[GEN_ALARM_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
  pFlexCAN->MB[GEN_ALARM_MB].ID = CAN_ID_PRIO(4) | CAN_ID_STD(0x84);
  pFlexCAN->MB[GEN_ALARM_MB].WORD0 = (*(uint32_t *)pword0);
  pFlexCAN->MB[GEN_ALARM_MB].WORD1 = (*(uint32_t *)pword1);

  pFlexCAN->MB[GEN_ALARM_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

  PWORD_CLEAR(pword0, pword1);
  }


/******************************************************************************
* Message Name: GEN Notification Setting Message
* Arbitration ID: 0x85
******************************************************************************/

  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(5)) == CAN_IFLAG1_BUF31TO8I(5))
  {

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(5);

  }

  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {

    pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

  }

  /* Step 3: Verify transmissin complete or aborted */
  if (((pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].CS) == CAN_CS_CODE(0)))
  {

    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(5);

    (*(uint32_t *)pword0) =
      ((current_therapy_settings.ambient.volume << CAN_GEN_AMBIENT_NOTIFICATION_VOL_SHIFT) |
       (current_therapy_settings.ambient.audio_enabled << CAN_GEN_AMBIENT_NOTIFICATION_EN_SHIFT) |
       (current_therapy_settings.audio.volume << CAN_GEN_GENERAL_NOTIFICATION_VOL_SHIFT) |
       (legacy_wand_params_obj.timer.audio_muted << CAN_GEN_REFLEX_TIMER_NOTIFICATION_EN_SHIFT));
    swap_4bytes(pword0);

    pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
    pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].ID = CAN_ID_PRIO(5) | CAN_ID_STD(0x85);
    pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].WORD1 = 0;


  pFlexCAN->MB[GEN_NOTIFICATION_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

  PWORD_CLEAR(pword0, pword1);

  }

  /******************************************************************************
* Message Name: GEN Wand Trade Name Message 1
* Arbitration ID: 0x86
******************************************************************************/

  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(6)) == CAN_IFLAG1_BUF31TO8I(6))
  {

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(6);

  }

  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {

    pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

  }

  /* Step 3: Verify transmissin complete or aborted */
  if (((pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].CS) == CAN_CS_CODE(0)))
  {

    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(6);

    (*(uint32_t *)pword0) =
      ((smart_wand_params.mfg.name[0] << CAN_GEN_TRADE_NAME_CHAR_0_SHIFT) |
       (smart_wand_params.mfg.name[1] << CAN_GEN_TRADE_NAME_CHAR_1_SHIFT) |
       (smart_wand_params.mfg.name[2] << CAN_GEN_TRADE_NAME_CHAR_2_SHIFT) |
       (smart_wand_params.mfg.name[3] << CAN_GEN_TRADE_NAME_CHAR_3_SHIFT));
    swap_4bytes(pword0);

    (*(uint32_t *)pword1) =
      ((smart_wand_params.mfg.name[4] << CAN_GEN_TRADE_NAME_CHAR_4_SHIFT) |
       (smart_wand_params.mfg.name[5] << CAN_GEN_TRADE_NAME_CHAR_5_SHIFT) |
       (smart_wand_params.mfg.name[6] << CAN_GEN_TRADE_NAME_CHAR_6_SHIFT) |
       (smart_wand_params.mfg.name[7] << CAN_GEN_TRADE_NAME_CHAR_7_SHIFT));
    swap_4bytes(pword1);

    pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
    pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].ID = CAN_ID_PRIO(6) | CAN_ID_STD(0x86);
    pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    pFlexCAN->MB[GEN_WAND_NAME_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

    PWORD_CLEAR(pword0, pword1);

  }

  /******************************************************************************
* Message Name: GEN Wand Trade Name Message 2
* Arbitration ID: 0x87
******************************************************************************/

  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(7)) == CAN_IFLAG1_BUF31TO8I(7))
  {

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(7);

  }

  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {

    pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

  }

  /* Step 3: Verify transmissin complete or aborted */
  if (((pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].CS) == CAN_CS_CODE(0)))
  {

    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(7);

  (*(uint32_t *)pword0) =
    ((smart_wand_params.mfg.name[8] << CAN_GEN_TRADE_NAME_CHAR_8_SHIFT) |
     (smart_wand_params.mfg.name[9] << CAN_GEN_TRADE_NAME_CHAR_9_SHIFT) |
     (smart_wand_params.mfg.name[10] << CAN_GEN_TRADE_NAME_CHAR_10_SHIFT) |
     (smart_wand_params.mfg.name[11] << CAN_GEN_TRADE_NAME_CHAR_11_SHIFT));
  swap_4bytes(pword0);
  (*(uint32_t *)pword1) =
    ((smart_wand_params.mfg.low_enable << CAN_GEN_WAND_LO_CAPABLE) |
     (smart_wand_params.mfg.med_enable << CAN_GEN_WAND_MED_CAPABLE) |
     (smart_wand_params.mfg.hi_enable << CAN_GEN_WAND_HI_CAPABLE) |
     (smart_wand_params.mfg.vac_enable << CAN_GEN_WAND_VAC_CAPABLE));
  swap_4bytes(pword1);

  pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
  pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].ID = CAN_ID_PRIO(7) | CAN_ID_STD(0x87);
  pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].WORD0 = (*(uint32_t *)pword0);
  pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].WORD1 = (*(uint32_t *)pword1);

  pFlexCAN->MB[GEN_WAND_NAME_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

  PWORD_CLEAR(pword0, pword1);

  }

  }
  return 0;

}

/***************************************************************************//**
Write to FlexCAN.
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t flexCAN_write_slow_msgs(void) {

  uint32_t word0, word1;
  uint8_t *pword0 = (uint8_t*) &word0;
  uint8_t *pword1 = (uint8_t*) &word1;

  uint8_t disable_can_tx_jmpr = ~(GPIOB_PDIR >> 9) & 1;

  if (disable_can_tx_jmpr == 0)
  {

  
    PWORD_CLEAR(pword0, pword1);

  /******************************************************************************
  * Message Name: GEN Wand Active Life Message
  * Arbitration ID: 0x8A
  ******************************************************************************/

    /* Step 1: Check & clear interrupt */
    if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(0)) == CAN_IFLAG1_BUF31TO8I(0))
    {

      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(0);

    }

    /* Step 2: Abort pending transmission (if applicable) */
    if (pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
    {

      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

    }

    /* Step 3: Verify transmissin complete or aborted */
    if (((pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
        ((pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
        ((pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS) == CAN_CS_CODE(0)))
    {

      /* Step 3a: Clear interrupt */
      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(0);

      (*(uint32_t *)pword0) =
        ((smart_wand_params.life.lo << CAN_GEN_WAND_ACTIVE_LIFE_LO_SHIFT) |
         (smart_wand_params.life.med << CAN_GEN_WAND_ACTIVE_LIFE_MED_SHIFT));
      swap_4bytes(pword0);

      (*(uint32_t *)pword1) =
        ((smart_wand_params.life.hi << CAN_GEN_WAND_ACTIVE_LIFE_HI_SHIFT) |
         (smart_wand_params.life.vac << CAN_GEN_WAND_ACTIVE_LIFE_VAC_SHIFT));
      swap_4bytes(pword1);

      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(0x8A);
      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].WORD0 = (*(uint32_t *)pword0);
      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].WORD1 = (*(uint32_t *)pword1);

      /* Start transmission */
      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

      PWORD_CLEAR(pword0, pword1);

    }
    
    
    /******************************************************************************
  * Message Name: GEN Wand Active Life Message
  * Arbitration ID: 0x8C
  ******************************************************************************/

    /* Step 1: Check & clear interrupt */
    if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(0)) == CAN_IFLAG1_BUF31TO8I(0))
    {

      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(0);

    }

    /* Step 2: Abort pending transmission (if applicable) */
    if (pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
    {

      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

    }

    /* Step 3: Verify transmissin complete or aborted */
    if (((pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
        ((pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
        ((pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS) == CAN_CS_CODE(0)))
    {

      /* Step 3a: Clear interrupt */
      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(0);

      (*(uint32_t *)pword0) =
        (smart_wand_params.coag_life << CAN_GEN_WAND_ACTIVE_LIFE_COAG_SHIFT);
      swap_4bytes(pword0);

      (*(uint32_t *)pword1) = 0;
      swap_4bytes(pword1);

      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(0x8C);
      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].WORD0 = (*(uint32_t *)pword0);
      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].WORD1 = (*(uint32_t *)pword1);

      /* Start transmission */
      pFlexCAN->MB[GEN_ACTIVE_LIFE_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

      PWORD_CLEAR(pword0, pword1);

    }


  /******************************************************************************
  * Message Name: GEN Serial Number Message
  * Arbitration ID: 0x8B
  ******************************************************************************/

    /* Step 1: Check & clear interrupt */
    if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(1)) == CAN_IFLAG1_BUF31TO8I(1))
    {

      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(1);

    }

    /* Step 2: Abort pending transmission (if applicable) */
    if (pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
    {

      pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

    }

    /* Step 3: Verify transmissin complete or aborted */
    if (((pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
        ((pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
        ((pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].CS) == CAN_CS_CODE(0)))
    {

      /* Step 3a: Clear interrupt */
      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(1);

      (*(uint32_t *)pword0) =
         ((bcd_serial_number[0] << CAN_GEN_SERIAL_NUM_CHAR_0_SHIFT) |
          (bcd_serial_number[1] << CAN_GEN_SERIAL_NUM_CHAR_1_SHIFT) |
          (bcd_serial_number[2] << CAN_GEN_SERIAL_NUM_CHAR_2_SHIFT) |
          (bcd_serial_number[3] << CAN_GEN_SERIAL_NUM_CHAR_3_SHIFT) |
          (bcd_serial_number[4] << CAN_GEN_SERIAL_NUM_CHAR_4_SHIFT) |
         ((bcd_serial_number[5] & 0x3) << CAN_GEN_SERIAL_NUM_CHAR_5A_SHIFT));
      swap_4bytes(pword0);

      (*(uint32_t *)pword1) =
          (((bcd_serial_number[5] & 0x3C) >> 2) |
            (bcd_serial_number[6] << CAN_GEN_SERIAL_NUM_CHAR_6_SHIFT) |
            (bcd_serial_number[7] << CAN_GEN_SERIAL_NUM_CHAR_7_SHIFT) |
            (bcd_serial_number[8] << CAN_GEN_SERIAL_NUM_CHAR_8_SHIFT) |
            (bcd_serial_number[9] << CAN_GEN_SERIAL_NUM_CHAR_9_SHIFT));
      swap_4bytes(pword1);

      pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
      pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(0x8B);
      pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].WORD0 = (*(uint32_t *)pword0);
      pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].WORD1 = (*(uint32_t *)pword1);

      /* Start transmission */
      pFlexCAN->MB[GEN_SERIAL_NUM_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

      PWORD_CLEAR(pword0, pword1);

    }

  /******************************************************************************
  * Message Name: GEN Serial Number Message
  * Arbitration ID: 0x88
  ******************************************************************************/

    /* Step 1: Check & clear interrupt */
    if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(2)) == CAN_IFLAG1_BUF31TO8I(2))
    {

      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(2);

    }

    /* Step 2: Abort pending transmission (if applicable) */
    if (pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
    {

      pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

    }

    /* Step 3: Verify transmissin complete or aborted */
    if (((pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
        ((pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
        ((pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].CS) == CAN_CS_CODE(0)))
    {

      /* Step 3a: Clear interrupt */
      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(2);

      (*(uint32_t *)pword0) =
         ((smart_wand_params.mfg.catalog[0] << CAN_GEN_CATALOG_NUM_CHAR_0_SHIFT) |
          (smart_wand_params.mfg.catalog[1] << CAN_GEN_CATALOG_NUM_CHAR_1_SHIFT) |
          (smart_wand_params.mfg.catalog[2] << CAN_GEN_CATALOG_NUM_CHAR_2_SHIFT) |
          (smart_wand_params.mfg.catalog[3] << CAN_GEN_CATALOG_NUM_CHAR_3_SHIFT));
      swap_4bytes(pword0);

      (*(uint32_t *)pword1) =
         ((smart_wand_params.mfg.catalog[4] << CAN_GEN_CATALOG_NUM_CHAR_4_SHIFT) |
          (smart_wand_params.mfg.catalog[5] << CAN_GEN_CATALOG_NUM_CHAR_5_SHIFT) |
          (smart_wand_params.mfg.catalog[6] << CAN_GEN_CATALOG_NUM_CHAR_6_SHIFT) |
          (smart_wand_params.mfg.catalog[7] << CAN_GEN_CATALOG_NUM_CHAR_7_SHIFT));
      swap_4bytes(pword1);

      pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
      pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(0x88);
      pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].WORD0 = (*(uint32_t *)pword0);
      pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].WORD1 = (*(uint32_t *)pword1);

      /* Start transmission */
      pFlexCAN->MB[GEN_CATALOG_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

      PWORD_CLEAR(pword0, pword1);

    }

  /******************************************************************************
  * Message Name: GEN Serial Number Message
  * Arbitration ID: 0x89
  ******************************************************************************/

    /* Step 1: Check & clear interrupt */
    if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(3)) == CAN_IFLAG1_BUF31TO8I(3))
    {

      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);

    }

    /* Step 2: Abort pending transmission (if applicable) */
    if (pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
    {

      pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);

    }

    /* Step 3: Verify transmissin complete or aborted */
    if (((pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
        ((pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
        ((pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].CS) == CAN_CS_CODE(0)))
    {

      /* Step 3a: Clear interrupt */
      pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);

      (*(uint32_t *)pword0) =
         ((smart_wand_params.mfg.catalog[8] << CAN_GEN_CATALOG_NUM_CHAR_8_SHIFT) |
          (smart_wand_params.mfg.catalog[9] << CAN_GEN_CATALOG_NUM_CHAR_9_SHIFT) |
          (smart_wand_params.mfg.catalog[10] << CAN_GEN_CATALOG_NUM_CHAR_10_SHIFT) |
          (smart_wand_params.mfg.catalog[11] << CAN_GEN_CATALOG_NUM_CHAR_11_SHIFT));
      swap_4bytes(pword0);

      (*(uint32_t *)pword1) =
         ((smart_wand_params.mfg.work_order[0] << CAN_GEN_WO_NUM_0_SHIFT) |
          (smart_wand_params.mfg.work_order[1] << CAN_GEN_WO_NUM_1_SHIFT) |
          (smart_wand_params.mfg.work_order[2] << CAN_GEN_WO_NUM_2_SHIFT) |
          (smart_wand_params.mfg.work_order[3] << CAN_GEN_WO_NUM_3_SHIFT));
      swap_4bytes(pword1);

      pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE);
      pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(0x89);
      pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].WORD0 = (*(uint32_t *)pword0);
      pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].WORD1 = (*(uint32_t *)pword1);

      /* Start transmission */
      pFlexCAN->MB[GEN_CATALOG_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);

      PWORD_CLEAR(pword0, pword1);

    }
    
  }

  return 0;

}


/***************************************************************************//**
Read Flex Can
@param void
@return void
@outputs
@context
*******************************************************************************/
void flexCAN_readMB(void)
{

  volatile uint8_t code;
  uint8_t *p = (uint8_t *)&pFlexCAN->MB[0].CS;

  while (pFlexCAN->IFLAG1)
  {

    memcpy(&mb_buffer.data[mb_buffer.idx],p,16);

    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF5I_MASK;

    if (mb_buffer.idx < 9)
    {

      mb_buffer.idx++;

    }



  }

  /* Unlock MBs */
  code = pFlexCAN->TIMER;

}


/***************************************************************************//**
Parse & unpack FlexCAN data.
@param void
@return uint8_t
@outputs
@context
*******************************************************************************/
uint8_t read_flexcan(void) {

  uint8_t iMB;
  uint32_t pword0, pword1;
  uint8_t id;

   __istate_t s = __get_interrupt_state();
   __disable_interrupt();

  for (iMB = 0; iMB < mb_buffer.idx; iMB++)
  {

    /* extract Frame Identifier */
    id = (mb_buffer.data[0].id & CAN_ID_STD_MASK) >> CAN_ID_STD_SHIFT;

    switch (id)
    {
      /* Case 0: UI Watchdog Message 0x90 */
      case (0x90):
      {

        PWORD_GET(&pword0,&pword1);
        current_therapy_settings.gui.rx_wdog.tx_cntr_idx = pword0 >> CAN_UI_TXCOUNT_SHIFT;
        current_therapy_settings.gui.rx_wdog.err = pword0 >> CAN_UI_TXFLAG_SHIFT;
        current_therapy_settings.gui.rx_wdog.time = (mb_buffer.data[0].cs & CAN_CS_TIME_STAMP_MASK);
        if (current_therapy_settings.gui.rx_wdog.err == 1)
        {

          put_event(&evt_tbl,GEN_CAN_TIMEOUT);
    
        }
        put_event(&evt_tbl,CAN_WDOG_RX);
        break;

      }

      /* Case 1: UI Notification Message 0x91 */
      case (0x91):
      {
        uint8_t faults = 0;
        PWORD_GET(&pword0,&pword1);
        current_therapy_settings.gui.sw.update_status = pword0 >> CAN_UI_FW_UPDATE_STATUS_SHIFT;
        current_therapy_settings.gui.errors.can = pword0 >> CAN_UI_CAN_SW_ERROR_SHIFT;
        current_therapy_settings.gui.errors.transport =  pword0 >> CAN_UI_CAN_TRANSPORT_SW_ERROR_SHIFT;
        current_therapy_settings.gui.errors.touch = pword0 >> CAN_UI_TOUCH_SW_ERROR_SHIFT;
        current_therapy_settings.gui.faults.sw = pword0 >> CAN_UI_CAN_SW_FAULT_SHIFT;
        current_therapy_settings.gui.faults.transport = pword0 >> CAN_UI_CAN_TRANSPORT_SW_FAULT_SHIFT;
        current_therapy_settings.gui.faults.touch = pword0 >> CAN_UI_TOUCH_SW_FAULT_SHIFT;
        current_therapy_settings.gui.faults.render = pword0 >> CAN_UI_RENDER_SW_FAULT_SHIFT;
        faults = current_therapy_settings.gui.faults.render |
                 current_therapy_settings.gui.faults.touch |
                 current_therapy_settings.gui.faults.transport |
                 current_therapy_settings.gui.faults.sw;
        if (faults != 0)
        {
          
          put_event(&evt_tbl,UI_FAULT);
          
        }
        
        if (current_therapy_settings.gui.sw.update_status > 0)
        {
          
          put_event(&evt_tbl,UI_UPDATE_STATUS);
          
        }
        break;

      }
      /* Case 2: UI Status Message 0x92 */
      case (0x92):
      {

        PWORD_GET(&pword0,&pword1);
        current_therapy_settings.gui.graphics.GraphicNumber = (GRAPHIC_ID)(pword0 >> CAN_UI_GRAPHIC_NUMBER_SHIFT);
        current_therapy_settings.gui.graphics.GraphicValue = pword0 >> CAN_UI_GRAPHIC_VALUE_SHIFT;
        put_event(&evt_tbl,GUI_GRAPHIC);
        break;

      }
      /* Case 3: UI Software Status Message 0x93 */
      case (0x93):
      {

        uint8_t firmware_update = 0,
//                KernelVerMajor = 0,
//                KernelVerMinor = 0,
//                BSPVerMajor = 0,
//                BSPVerMinor = 0,
                AppVerMajor = 0,
                AppVerMinor = 0;
//                GraphicsVerMajor = 0,
//                GraphicsVerMinor = 0;

         PWORD_GET(&pword0,&pword1);

         //KernelVerMajor = pword0 >> CAN_UI_KERNEL_VERSION_MAJOR_SHIFT;
         //KernelVerMinor = pword0 >> CAN_UI_KERNEL_VERSION_MINOR_SHIFT;
         //BSPVerMajor = pword0 >> CAN_UI_BSP_VERSION_MAJOR_SHIFT;
         //BSPVerMinor = pword0 >> CAN_UI_BSP_VERSION_MINOR_SHIFT;
         AppVerMajor = pword0 >> CAN_UI_APPLICATION_VERSION_MAJOR_SHIFT;
         AppVerMinor = pword0 >> CAN_UI_APPLICATION_VERSION_MINOR_SHIFT;
         current_therapy_settings.sw.app2_version_major = AppVerMajor;
         current_therapy_settings.sw.app2_version_minor = AppVerMinor;
         //GraphicsVerMajor = pword1 >> CAN_UI_GRAPHIC_VERSION_MAJOR_SHIFT;
         //GraphicsVerMinor = pword1 >> CAN_UI_GRAPHICS_VERSION_MINOR_SHIFT;

         firmware_update = check4uifw(AppVerMajor,AppVerMinor);

         if (firmware_update)
         {

           current_therapy_settings.sdhc.card_ready = 1;

         }

         current_therapy_settings.sdhc.uifw_update_avaliable = firmware_update;

        //current_therapy_settings.gui.sw.time = timeStamp[iMB];
        //pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF4TO0I(3);
         break;

      }
      /* Case 4: UI Debug Message 0x9F */
      case (0x94):
      {

        PWORD_GET(&pword0,&pword1);
        current_therapy_settings.gui.debug.echo_l = pword0 >> CAN_UI_DEBUG_MSG_ECHO_L_SHIFT;
        current_therapy_settings.gui.debug.echo_h = pword1 >> CAN_UI_DEBUG_MSG_ECHO_H_SHIFT;
        //current_therapy_settings.gui.debug.time = timeStamp[iMB];
        /* Clear BUF8I Flag */
        //pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF4TO0I(4);
        break;

      }

      #ifdef DEBUG_CAN
      case (0xB0):
      {
        PWORD_GET(&pword0,&pword1);
        pSmartWandFTM->CONTROLS[0].CnV = (pword0 >> 0) & 0xFFFF;
        pSmartWandFTM->CONTROLS[1].CnV = (pword0 >> 16) & 0xFFFF;
        pSmartWandFTM->CONTROLS[2].CnV = (pword1 >> 0) & 0xFFFF;
        pSmartWandFTM->CONTROLS[3].CnV = (pword1 >> 16) & 0xFFFF;
        break;
      }
      case (0xB1):
      {
        PWORD_GET(&pword0,&pword1);
        pSmartWandFTM->CONTROLS[4].CnV = (pword0 >> 0) & 0xFFFF;
        pSmartWandFTM->CONTROLS[5].CnV = (pword0 >> 16) & 0xFFFF;
        pSmartWandFTM->CONTROLS[6].CnV = (pword1 >> 0) & 0xFFFF;
        pSmartWandFTM->CONTROLS[7].CnV = (pword1 >> 16) & 0xFFFF;
        pSmartWandFTM->SYNC |= FTM_SYNC_SWSYNC_MASK;
        break;
      }
      case (0xB2):
      {
        PWORD_GET(&pword0,&pword1);
        break;
      }
      case (0xB3):
      {
        PWORD_GET(&pword0,&pword1);
        break;
      }
      #endif

      default:
        //printf("CAN Error: Message ID Not Recognized");
        break;
    }

    memcpy(&mb_buffer.data[0],&mb_buffer.data[1],mb_buffer.idx*16);

    mb_buffer.idx--;

  }

  __set_interrupt_state(s);

  return 0;
}


/***************************************************************************//**
Reset CAN wdog
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t can_wdog_rx(void)
{

  static uint32_t next_tx_cntr_idx = 0;
  volatile uint32_t temp;

  /* reset timeout timer */
  RESET_PIT(1);

  if (current_therapy_settings.gui.rx_wdog.tx_cntr_idx == 0)
  {

    next_tx_cntr_idx = 0;

  }

  if (((current_therapy_settings.gui.rx_wdog.tx_cntr_idx & next_tx_cntr_idx) != next_tx_cntr_idx) &&
       (current_therapy_settings.gui.tx_wdog_err == FALSE))
  {

    /* reset timeout timer */
    DISABLE_PIT(1);

    /* idx error */
    put_event(&evt_tbl,CAN_TIMEOUT);

  }

  next_tx_cntr_idx = (current_therapy_settings.gui.rx_wdog.tx_cntr_idx + 1) % 0xFFFFFFFF;


  return 0;

}
