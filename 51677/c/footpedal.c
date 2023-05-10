/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2798                 $:
$Date:: 2021-11-17 02:10:26#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/

/***************************************************************************//**
\file:     footpedal.c
\brief     Foot pedal driver.
\details   N/A
*******************************************************************************/
#include "MK26F18.h"
#include "system.h"
#include "footpedal.h"
#include "cpu.h"
#include "event_buffer.h"
#include "legacy_wand.h"
#include "wand.h"

extern legacy_wand_params legacy_wand_params_obj;
extern manta_prime prime;
static uint8_t mode_button_pressed = 0;
/****************************************************************************//**
Initialize foot pedal interface.
@param void
@return void
@outputs
@context
********************************************************************************/
uint8_t foot_pedal_init(void)
{
  
  /* Enable gate clock */
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
  
  /* Configure port pin control registers */
  PORTB_PCR23 = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK;
  PORTB_PCR22 = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK;
  PORTB_PCR20 = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK;
  
  /* Configure port pins as inputs */
  GPIOB_PDDR &= ~GPIO_PDDR_PDD(FOOTPEDAL_MASK);
  
  return 1;
  
}


/****************************************************************************//**
Poll status of foot pedals.
@param void
@return void
@outputs
@context
********************************************************************************/
uint8_t poll_footpedal(void)
{
  
  uint8_t mode = 0,
  ablate = 0,
  coag = 0,
  vac = 0;
  
  uint8_t cut_enable = smart_wand_params.mfg.low_enable |
    smart_wand_params.mfg.med_enable |
      smart_wand_params.mfg.hi_enable;
  uint8_t vac_enable = smart_wand_params.mfg.vac_enable;
  uint8_t coag_enable = smart_wand_params.mfg.coag_enable;
  uint8_t PrimeRunning = get_priming_status();
  
  static uint8_t mode_cnt, wireless_blink_duty = 0, throttle_therapy_on = 0, mode_lock = 0;
  
  if ((legacy_wand_params_obj.wand_connected == 1) &&
      (current_therapy_settings.wand.unlocked == 1) &&
        ((current_therapy_settings.footpedal.present == 0) ||
         ((current_therapy_settings.treatment.ablate_on == 0) &&
          (current_therapy_settings.treatment.coag_on == 0))))
  {
    
    current_therapy_settings.footpedal.present = 0;
    current_therapy_settings.footpedal.wireless.present = 0;
    mode_cnt = 0;
    wireless_blink_duty = 0;
    
  }
  else if (current_therapy_settings.footpedal.wired.present == 1)
  {
    
    /* Set footpedal present status */
    ablate = ~(GPIOB_PDIR >> ABLATE_SHIFT) & 1;
    coag = ~(GPIOB_PDIR >> COAG_SHIFT) & 1;
    mode = ~(GPIOB_PDIR >> MODE_SHIFT) & 1;
    current_therapy_settings.footpedal.present = 1;
    
    current_therapy_settings.footpedal.wireless.present = 0;
    wireless_blink_duty = 0;
    
  }
  else if ((current_therapy_settings.footpedal.wireless.married2station1 == 1) &&
           (current_therapy_settings.footpedal.wireless.marry_in_progress == 0) &&
             (current_therapy_settings.footpedal.wireless.married_latch == 1) &&
               (current_therapy_settings.treatment.wireless_enable == 1))
  {
    
    ablate = current_therapy_settings.footpedal.wireless.ablate;
    coag = current_therapy_settings.footpedal.wireless.coag;
    mode = current_therapy_settings.footpedal.wireless.mode;
    if (current_therapy_settings.footpedal.wireless.batt_low_count>= 50)
    {
      
      if (wireless_blink_duty < 100)
      {
        
        current_therapy_settings.footpedal.wireless.present = 2;
        wireless_blink_duty++;
        
      }
      else if (wireless_blink_duty < 199)
      {
        
        current_therapy_settings.footpedal.wireless.present = 1;
        wireless_blink_duty++;
        
      }
      else
      {
        
        current_therapy_settings.footpedal.wireless.present = 1;
        wireless_blink_duty = 0;
        
      }
      
    }
    else
    {
      /* finish out last blink */
      if ((wireless_blink_duty < 100) && (wireless_blink_duty > 0))
      {
        
        current_therapy_settings.footpedal.wireless.present = 2;
        wireless_blink_duty++;
        
      }
      else
      {
        
        wireless_blink_duty = 0;
        current_therapy_settings.footpedal.wireless.present = 1;
        
      }      
      
    }
    current_therapy_settings.footpedal.present = 1;
    current_therapy_settings.treatment.vac_enabled = 1;
    
  }
  else
  {
    
    current_therapy_settings.footpedal.present = 0;
    current_therapy_settings.footpedal.wireless.present = 0;
    current_therapy_settings.treatment.vac_enabled = 0;
    wireless_blink_duty = 0;
    
  }
  
  /* Increment mode key counter if pressed */
  if (mode)
  {
    
    current_therapy_settings.footpedal.mode = 1;
    if (mode_cnt < 255)
    {
      mode_cnt++;
    }
    
  }
  else if ((mode_cnt > 0) && current_therapy_settings.active_wand.lw_active &&
           (mode_button_pressed == 0) && 
             !(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA))
  {
    put_event(&evt_tbl,CLICK_SOUND);
    put_event(&evt_tbl,LW_MODE_ON_EVENT);
    mode_button_pressed = 1;
  }
  
  /* When mode key not pressed & count isn't > delay mode key is intended */
  else if ((mode_cnt > 0) && (mode_cnt <= 75) &&
           (current_therapy_settings.active_wand.lw_active == 0) &&
             (mode_lock == 0) &&
               (cut_enable == 1) &&
                 !(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA))
  {
    
    current_therapy_settings.footpedal.mode = 0;   
    put_event(&evt_tbl,CHANGE_MODE);
    mode_cnt = 0;
    
  }
  else if (mode_cnt > 50)
  {
    
    current_therapy_settings.footpedal.mode = 0;
    mode_cnt--;
    
  }
  else
  {
    
    current_therapy_settings.footpedal.mode = 0;
    mode_cnt = 0;
    mode_button_pressed = 0;
    mode_lock = 0;
    
  }
  
  /* So long as mode key is held pressed longer than delay, stay in VAC mode */
  if ((mode_cnt > 75) ||
      ((mode_lock == 1) && (current_therapy_settings.footpedal.mode == 1)))
  {
    
    /* limit counter value */
    mode_cnt = 76;
    vac = 1;
    mode_lock = 1;
    
  }
  else
  {
    
    vac = 0;
    
  }
  
  if (ablate != current_therapy_settings.footpedal.ablate)
  {
    if (ablate == 1)
    {
      if ((current_therapy_settings.wand.unlocked == FALSE)  &&
          (legacy_wand_params_obj.wand_connected == 0))
      {
        
        put_event(&evt_tbl,BOTH_WANDS_DISCONNECT_ERROR);
        
      }
      else if ((current_therapy_settings.active_wand.lw_active == 0) && 
               !(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
                 (throttle_therapy_on == 0) &&
                   (cut_enable == 1))
      {
        put_event(&evt_tbl,THERAPY_ON);
        /* 150ms to throttle how often foot turns therapy b/c pump */
        throttle_therapy_on = 30;
      }
      else if ((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
                   (PrimeRunning == PRIME_AUTO))
      {
        gui_stop_auto_prime();
        current_therapy_settings.treatment.pre_prime = FALSE;
        prime.blink = FALSE;
      }
      
      else if (current_therapy_settings.active_wand.lw_active == 1)
      {
        put_event(&evt_tbl,LW_ABLATE_ON_EVENT);
      }
      
    }
    //else if (smart_wand_params.mfg.indication != SMART_WAND_MFG_INDICATION_MANTA)
    else
    {
      if (current_therapy_settings.active_wand.lw_active == 0)
      {
        put_event(&evt_tbl,THERAPY_OFF);
      }
      else
      {
        put_event(&evt_tbl,LW_ABLATE_OFF_EVENT);
      }
    }
    
  }
  
  if (coag != current_therapy_settings.footpedal.coag)
  {
    
    if (coag == 1)
    {
      if ((current_therapy_settings.wand.unlocked == FALSE)  &&
          (legacy_wand_params_obj.wand_connected == 0))
      {
        
        put_event(&evt_tbl,BOTH_WANDS_DISCONNECT_ERROR);
        
      }
      else if ((current_therapy_settings.active_wand.lw_active == 0) &&
               !(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
                 (throttle_therapy_on == 0) &&
                   (coag_enable == 1))
      {
        put_event(&evt_tbl,THERAPY_ON);
        /* 150ms to throttle how often foot turns therapy b/c pump */
        throttle_therapy_on = 30;
      }
      else if ((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
               (current_therapy_settings.treatment.pre_prime == FALSE))
      {
        put_event(&evt_tbl,THERAPY_ON);
        /* 150ms to throttle how often foot turns therapy b/c pump */
        throttle_therapy_on = 30;
      }
      else if ((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
                   (PrimeRunning == PRIME_AUTO))
      {
        gui_stop_auto_prime();
        current_therapy_settings.treatment.pre_prime = FALSE;
        prime.blink = FALSE;
      }
      else if (current_therapy_settings.active_wand.lw_active == 1)
      {
        put_event(&evt_tbl,LW_COAG_ON_EVENT);
      }
    }
    else
    {
      if (current_therapy_settings.active_wand.lw_active == 0)
      {
        put_event(&evt_tbl,THERAPY_OFF);
      }
      else
      {
        put_event(&evt_tbl,LW_COAG_OFF_EVENT);
      }
    }
    
  }
  
  if (current_therapy_settings.active_wand.lw_active == 0)
  {
    if ((vac != current_therapy_settings.footpedal.vac_key) &&
        (current_therapy_settings.treatment.vac_enabled == 1))
    {
      
      if ((current_therapy_settings.wand.unlocked == FALSE)  &&
          (legacy_wand_params_obj.wand_connected == 0))
      {
        
        put_event(&evt_tbl,BOTH_WANDS_DISCONNECT_ERROR);
        
      }
      else if ((vac == 1) &&
               (vac_enable == 1))
      {
        
        put_event(&evt_tbl,THERAPY_ON);
        
      }
      else
      {
        
        put_event(&evt_tbl,THERAPY_OFF);
        
      }
      
    }
  }
  
  if ( (current_therapy_settings.active_wand.lw_active == 0) && (current_therapy_settings.active_wand.smart_wand == ARTHROPLASTY) )
  {
    if (mode != current_therapy_settings.footpedal.mode_key)
    {
      if((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) && (current_therapy_settings.treatment.pre_prime == FALSE) && (PrimeRunning == 0))
      {
        if(current_therapy_settings.gui.screenID == MANTA_SMART_WAND_TREATMENT_SCREEN && (mode == 1))
        {
          manta_coag_mode_increment();
        }
        else if (current_therapy_settings.footpedal.wireless.marry_in_progress == 0)
        {
          current_therapy_settings.gui.screenID = MANTA_SMART_WAND_TREATMENT_SCREEN;
          current_therapy_settings.gui.auto_revert_back_screenID = 0;
        } 
      }
      else if ((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) && (PrimeRunning == PRIME_AUTO))
      {
        gui_stop_auto_prime();
        current_therapy_settings.treatment.pre_prime = FALSE;
        prime.blink = FALSE; 
      }
      else
      {
        
      }
    }
  }
  current_therapy_settings.footpedal.ablate = ablate;
  current_therapy_settings.footpedal.coag = coag;
  current_therapy_settings.footpedal.vac_key = vac;
  current_therapy_settings.footpedal.mode_key = mode;
  
  if (throttle_therapy_on > 0)
  {
    
    throttle_therapy_on--;
    
  }
  
  return 0;
  
}

/***************************************************************************//**
This function is called to set the footpedal switch stuck fault mask
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t check_fs_stuck(void)
{
  
  uint8_t ablate = 0,
  coag = 0,
  mode = 0;
  
  /* Set footpedal present status */
  ablate = ~(GPIOB_PDIR >> ABLATE_SHIFT) & 1;
  coag = ~(GPIOB_PDIR >> COAG_SHIFT) & 1;
  mode = ~(GPIOB_PDIR >> MODE_SHIFT) & 1;
  
  if (ablate | coag | mode)
  {
    
    put_event(&evt_tbl,FOOT_SWITCH_STUCK);
    
  }
  
  return 0;
  
}

/***************************************************************************//**
This function is called to check if any button is pressed on the footswitch
@param void
@return TRUE | FALSE
@outputs
@context
*******************************************************************************/
bool fs_any_button_pressed(void)
{
  bool AnyButtonPressed = TRUE;
  
  return(AnyButtonPressed);
}
