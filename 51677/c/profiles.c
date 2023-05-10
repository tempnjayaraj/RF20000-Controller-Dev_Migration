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
\file      profiles.c
\brief     Profile Manager
\details   NA
*******************************************************************************/

#include "MK26F18.h"
#include "system.h"
//#include "profiles.h"
#include "wand.h"

static const profile_record empty_record;
extern uint8_t current_profile_active;

/***************************************************************************//**
Profile manager
@todo More description
@param void
@return 0
@outputs
@context
*******************************************************************************/
void profile_manager(void)
{
  
  current_therapy_settings.user_data.settings.new_en = 1;
    
}

/***************************************************************************//**
save profile
@todo More description
@param value
@return 0
@outputs
@context
*******************************************************************************/
uint8_t save_profile(uint8_t value)
{
  
  bool profile_saved = TRUE;
  
  profile_record profile = empty_record;
  
  profile.ambient.alarm_threshold = current_therapy_settings.ambient.alarm_threshold;
  profile.ambient.audio_enabled = current_therapy_settings.ambient.audio_enabled;
  profile.ambient.enable = current_therapy_settings.ambient.enable;
  profile.ambient.enable_user_pref = current_therapy_settings.ambient.enable_user_pref;
  profile.finger_switch_en = current_therapy_settings.treatment.finger_switch_en;
  //profile.wireless_enable = current_therapy_settings.treatment.wireless_enable;
  profile.languageID = current_therapy_settings.gui.languageID;
  
  if (smart_wand_params.mfg.low_enable == 1)
  {
    
    profile.setpoint.lo = current_therapy_settings.setpoint.lo;
    
  }
  else
  {
    
    profile.setpoint.lo = ABLATE_DEFAULT;
    
  }
  
  if (smart_wand_params.mfg.med_enable == 1)
  {
    
    profile.setpoint.med = current_therapy_settings.setpoint.med;
    
  }
  else
  {
    
    profile.setpoint.med = ABLATE_DEFAULT;
    
  }
  
  if (smart_wand_params.mfg.hi_enable == 1)
  {
    
    profile.setpoint.hi = current_therapy_settings.setpoint.hi;
    
  }
  else
  {
    
    profile.setpoint.hi = ABLATE_DEFAULT;
    
  }
  
  if (smart_wand_params.mfg.coag_enable == 1)
  {
    
    profile.setpoint.coag = current_therapy_settings.setpoint.coag;
    
  }
  else
  {
    
    profile.setpoint.coag = COAG_1;
    
  }
  
  if ((current_therapy_settings.active_wand.smart_wand == ARTHROPLASTY) ||
      (current_therapy_settings.previous_wand_indication == SMART_WAND_MFG_INDICATION_MANTA))
  {
    /* SPORTS MED DEFAULTS */
    profile.setpoint.lo = ABLATE_DEFAULT;
    profile.setpoint.med = ABLATE_DEFAULT;
    profile.setpoint.hi = ABLATE_DEFAULT;
    profile.setpoint.coag = COAG_1;   
  }
  
  current_therapy_settings.user_data.profile[value] = profile;  
  
  return profile_saved;
  
}

/***************************************************************************//**
Load profile
@todo More description
@param value
@return 0
@outputs
@context
*******************************************************************************/
uint8_t load_profile(uint8_t value)
{
  
  uint8_t record_invalid = 0;
  profile_record *profile = &current_therapy_settings.user_data.profile[value];
  
  /* go through boundary checks ensuring record valid */
  if (profile->ambient.alarm_threshold > 60)
  {
    
    record_invalid = 1;
    
  }
  
  if (profile->languageID >= MAX_NUM_LANGUAGES)
  {
    
    record_invalid = 1;
    
  }
  
  if ((profile->setpoint.hi >= NUM_CUT_LEVELS) ||
      (profile->setpoint.hi <= ABLATE_ZILCH))
  {
    
    record_invalid = 1;
    
  }
  
  if ((profile->setpoint.med >= NUM_CUT_LEVELS) ||
      (profile->setpoint.med <= ABLATE_ZILCH))
  {
    
    record_invalid = 1;
    
  }
  
  if ((profile->setpoint.lo >= NUM_CUT_LEVELS) ||
      (profile->setpoint.lo <= ABLATE_ZILCH))
  {
    
    record_invalid = 1;
    
  }
  
  if ((profile->setpoint.coag > MAX_NUM_OF_ENT_COAG_LEVELS) ||
      (profile->setpoint.coag <= COAG_0))
  {
    record_invalid = 1;    
  }
  
  if (record_invalid == 0)
  {
    
    current_therapy_settings.ambient.audio_enabled = profile->ambient.audio_enabled;
    current_therapy_settings.ambient.alarm_threshold = profile->ambient.alarm_threshold;
    current_therapy_settings.ambient.enable = profile->ambient.enable;
    current_therapy_settings.ambient.enable_user_pref = profile->ambient.enable_user_pref;   
    current_therapy_settings.ambient.enable = (current_therapy_settings.ambient.capable &
                                               current_therapy_settings.ambient.enable_user_pref);
    current_therapy_settings.treatment.finger_switch_en = (bool)profile->finger_switch_en;      
    current_therapy_settings.setpoint.lo = profile->setpoint.lo;
    current_therapy_settings.setpoint.med = profile->setpoint.med;
    current_therapy_settings.setpoint.hi = profile->setpoint.hi;
    if ((current_therapy_settings.active_wand.smart_wand == SPORTS_MED) &&
        (profile->setpoint.coag > COAG_2))
    {
      current_therapy_settings.setpoint.coag = COAG_1;
    }
    else
    {
      current_therapy_settings.setpoint.coag = profile->setpoint.coag;
    }
    
  }
  
  return record_invalid;
  
}

/***************************************************************************//**
return to profile list screen, cancel new profile add and point to previous 
profile selected
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t cancel_new_profile(void)
{
  
  uint8_t last_record = 0;
  
  if (current_therapy_settings.gui.screenID == ENTER_NEW_USER_PROFILE_NAME_SCREEN)
  {
    /* skip save profile screen b/c save just occured */
    if (current_therapy_settings.gui.depth_idx > 0)
    {
      current_therapy_settings.gui.depth_idx--;
    }
  
    if (current_therapy_settings.gui.previous_screenID[current_therapy_settings.gui.depth_idx] == 0)
    {
      current_therapy_settings.gui.screenID = WAND_DETECTION_SCREEN;
    }
    else
    {
      current_therapy_settings.gui.screenID = 
        current_therapy_settings.gui.previous_screenID[current_therapy_settings.gui.depth_idx];
    }
        
    if (load_profile(current_profile_active) == 0)
    {
      current_therapy_settings.user_data.settings.active_idx = current_profile_active;        
    }
  }
  

  return last_record;  
}  

