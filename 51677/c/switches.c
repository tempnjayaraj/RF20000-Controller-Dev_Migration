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
\file      switches.c
\brief     Driver for switches module which controls the turning ON/OFF of
therapy.
\details   NA
*******************************************************************************/

#include "switches.h"
#include "MK26F18.h"
#include "system.h"
#include "rf.h"
#include "audio.h"
#include "pit.h"
#include "gui.h"
#include "buck.h"
#include "wand.h"
#include "legacy_wand.h"
#include "fuse_blow.h"

#ifdef DEBUG_CAN
#include "can_debug.h"
#endif

/* used for converting watts from wand parameters to adc counts
* Watts x (4096*65535)/(8*42.5*6.6)
*/
#define ADC_COUNTS_PER_WATT 119623.643493761f  

extern const uint16_t legacy_wand_ablate_dac_values[11]; // [11] = {1205,1518,1855,2168,2494,2819,3132,3458,3783,3855};
extern const uint16_t legacy_wand_coag_dac_values[3];
extern const uint16_t legacy_over_energy_current_limits_for_ablate[11];
extern const uint16_t legacy_over_energy_current_limits_for_coag[3];
extern const uint16_t legacy_over_energy_current_limits_for_ablate_27pin[11];
extern const uint16_t legacy_over_energy_current_limits_for_coag_27pin[3];
/*
@ENT
adding ENT limits
*/
extern const uint16_t ent_legacy_wand_ablate_dac_values[11];
extern const uint16_t ent_legacy_wand_coag_dac_values[11];
extern const uint16_t ent_legacy_over_energy_current_limits_for_ablate[11];
extern const uint16_t ent_legacy_over_energy_current_limits_for_coag[11];

extern uint16_t over_energy_current_limit;

extern const uint32_t LW_ACTIVATION_TIME_LIMIT; //40 mins
extern const uint32_t LW_OVERALL_TIME_LIMIT; //8 hours
extern legacy_wand_params legacy_wand_params_obj;
extern uint8_t new_fuse_flag;
uint32_t therapy_start_time;
uint32_t start_duration=0;
ENT_ABLATE_MODE_PARAMS_T* load_ablate_mode_params_ent(ENT_ABLATE_MODE_PARAMS_T *mode, CUT_LEVELS cut_level, ENT_FLOW_SP_T flow_sp);
void load_ablate_params_ent(MODES therapy_mode);
void load_ablate_params_sm(MODES therapy_mode);
void load_coag_params_sm(COAG_LEVELS coag_level);
void load_coag_params_ent(COAG_LEVELS coag_level);
uint8_t legacy_sm_wand_therapy_on(void);
uint8_t legacy_ent_wand_therapy_on(void);
uint8_t legacy_ent_reflex_wand_therapy_on (void);
void load_coag_params_manta(COAG_LEVELS coag_level);
uint8_t current_coag_setpoint;
bool unitTest = FALSE;
extern manta_prime prime;

/***************************************************************************//**
This function turns on the therapy for the legacy wand. This function is called
when ablate or coag key is pressed. This function puts out the corresponding
output voltages.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t legacy_wand_therapy_on (void)
{
    uint8_t result = 0;
	
      cancel_new_profile();
    
	if (current_therapy_settings.controller == ENT)
	{
		if (current_therapy_settings.legacy_wand_levels.topaz_timer == 1)
		{
			result = legacy_ent_reflex_wand_therapy_on();
		}
		else
		{
			result = legacy_ent_wand_therapy_on();
		}
	}
	else
	{
		result = legacy_sm_wand_therapy_on();
	}
	
	return result;
}

/***************************************************************************//**
This function turns on the therapy for the legacy wand. This function is called
when ablate or coag key is pressed. This function puts out the corresponding
output voltages.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t legacy_sm_wand_therapy_on ()
{
  uint8_t return_value;
  /* Test ablate input from wand or foot switches */
  if ((((legacy_wand_params_obj.ablate_flag == 1) && (current_therapy_settings.footpedal.ablate == 0)) ||
       ((legacy_wand_params_obj.ablate_flag == 0) && (current_therapy_settings.footpedal.ablate == 1))) &&
      (current_therapy_settings.footpedal.coag == 0) && (current_therapy_settings.footpedal.vac_key == 0) && 
        (legacy_wand_params_obj.coag_flag == 0) &&
          (current_therapy_settings.footpedal.mode == 0) && (legacy_wand_params_obj.mode_flag == 0) &&
            (therapy_start_screen(current_therapy_settings.gui.screenID)))
    
  {
    current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
    // moving to the legacy wand therapy screen if it is not that screen already
    current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
    
    return_value = 1;
    
    /* copy RTC_TSR to ram for logging */
    memcpy(&start_duration,(uint8_t*)0x4003D000,sizeof(uint32_t));
    
      legacy_wand_params_obj.level= legacy_wand_ablate_dac_values[legacy_wand_params_obj.ablate_level];
    
      if (legacy_wand_params_obj.lower_current_limit == 1)
      {
        // R1 = 324, 649, 1210
        over_energy_current_limit = legacy_over_energy_current_limits_for_ablate_27pin[legacy_wand_params_obj.ablate_level];
      }
      else
      {
        // All other R1 wands
        over_energy_current_limit = legacy_over_energy_current_limits_for_ablate[legacy_wand_params_obj.ablate_level];
      }
      
    current_therapy_settings.treatment.previous_therapy_mode =
      current_therapy_settings.treatment.therapy_mode;   
    
    
    current_therapy_settings.log.mode = current_therapy_settings.treatment.therapy_mode;
    
    current_therapy_settings.treatment.ablate_on = 1;    
    legacy_wand_params_obj.therapy_on = 1;
    // Turn ON RF is activation time less than activation time limit
    put_event(&evt_tbl,RF_ON);
    return_value = 1;
 
  }
  else if 
    ((((legacy_wand_params_obj.coag_flag == 1) && (current_therapy_settings.footpedal.coag == 0)) ||
      ((legacy_wand_params_obj.coag_flag == 0) && (current_therapy_settings.footpedal.coag == 1))) &&
     (current_therapy_settings.footpedal.ablate == 0) && (current_therapy_settings.footpedal.vac_key == 0) && 
       (legacy_wand_params_obj.ablate_flag== 0) && 
         (current_therapy_settings.footpedal.mode == 0) && (legacy_wand_params_obj.mode_flag == 0) &&
           (therapy_start_screen(current_therapy_settings.gui.screenID)))
    {
      if (legacy_wand_params_obj.coag_enable)
      {
        current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
        // moving to the legacy wand therapy screen if it is not that screen already
        current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
        
        // COAG is only enabled for certain wands
        return_value = 1;
        
        /* copy RTC_TSR to ram for logging */
        memcpy(&start_duration,(uint8_t*)0x4003D000,sizeof(uint32_t));
        
        
        /* auto-switch screens but save current screen to revert back later */
        current_therapy_settings.log.mode = COAG;
        
          legacy_wand_params_obj.level= legacy_wand_coag_dac_values[legacy_wand_params_obj.coag_level];
        
          if (legacy_wand_params_obj.lower_current_limit == 1)
          {
            // R1 = 324, 649, 1210
            over_energy_current_limit = legacy_over_energy_current_limits_for_coag_27pin[legacy_wand_params_obj.coag_level];
          }
          else
          {
            // All other R1 wands
            over_energy_current_limit = legacy_over_energy_current_limits_for_coag[legacy_wand_params_obj.coag_level];
           }
       
        
        current_therapy_settings.treatment.coag_on = 1;
        legacy_wand_params_obj.therapy_on = 1;
        // Turn ON RF is activation time less than activation time limit
        put_event(&evt_tbl,RF_ON);
        return_value = 1;
      }
     
    }
  return return_value;
}

/***************************************************************************//**
This function turns on the therapy for the legacy wand. This function is called
when ablate or coag key is pressed. This function puts out the corresponding
output voltages.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t legacy_ent_wand_therapy_on (void)
{
	uint8_t return_value = 0;
	
	uint8_t ablate_flags =
		current_therapy_settings.footpedal.ablate +
		legacy_wand_params_obj.ablate_flag;
	
	uint8_t coag_flags =
		legacy_wand_params_obj.coag_flag +
		current_therapy_settings.footpedal.coag;
  
	uint8_t mode_flags = 
		current_therapy_settings.footpedal.mode +
		legacy_wand_params_obj.mode_flag +
		current_therapy_settings.footpedal.vac_key;
	
	uint8_t pump_flags = 
		current_therapy_settings.pump.on  &
		((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK) &
		((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON);
	
	/* Test ablate input from wand or foot switches */
	if ((ablate_flags == 1) &&
		(coag_flags == 0) &&
		(mode_flags == 0) &&
		(pump_flags == 1) &&
		therapy_start_screen(current_therapy_settings.gui.screenID))
    
	{
		current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
		
		// moving to the legacy wand therapy screen if it is not that screen already
		current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
    
		/* copy RTC_TSR to ram for logging */
		memcpy(&start_duration,(uint8_t*)0x4003D000,sizeof(uint32_t));
    
		legacy_wand_params_obj.level= ent_legacy_wand_ablate_dac_values[legacy_wand_params_obj.ablate_level];
		over_energy_current_limit = ent_legacy_over_energy_current_limits_for_ablate[legacy_wand_params_obj.ablate_level];
		current_therapy_settings.treatment.previous_therapy_mode =
		current_therapy_settings.treatment.therapy_mode;   
		current_therapy_settings.log.mode = current_therapy_settings.treatment.therapy_mode;
		current_therapy_settings.treatment.ablate_on = 1;    
		legacy_wand_params_obj.therapy_on = 1;
                current_therapy_settings.pump.ctrl_en = TRUE;
		
		// Turn ON RF is activation time less than activation time limit
		put_event(&evt_tbl,RF_ON);
		return_value = 1;
		current_therapy_settings.pump.idle_rps = lw_get_pump_speed(current_therapy_settings.treatment.flow_rate);
		pump.state = IDLE_SPEED;
	}
	else if ((ablate_flags == 0) &&
			(coag_flags == 1) &&
			(mode_flags == 0) &&			
			(pump_flags == 1) &&
			therapy_start_screen(current_therapy_settings.gui.screenID))
	{
		if (legacy_wand_params_obj.coag_enable)
		{
			current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
			// moving to the legacy wand therapy screen if it is not that screen already
			current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
			
			/* copy RTC_TSR to ram for logging */
			memcpy(&start_duration,(uint8_t*)0x4003D000,sizeof(uint32_t));
			/* auto-switch screens but save current screen to revert back 
			later */
			current_therapy_settings.log.mode = COAG;
			legacy_wand_params_obj.level= ent_legacy_wand_coag_dac_values[legacy_wand_params_obj.coag_level];
			over_energy_current_limit = ent_legacy_over_energy_current_limits_for_coag[legacy_wand_params_obj.coag_level];
			current_therapy_settings.treatment.coag_on = 1;
			legacy_wand_params_obj.therapy_on = 1;
                        current_therapy_settings.pump.ctrl_en = TRUE;
			
			// Turn ON RF is activation time less than activation time limit
			put_event(&evt_tbl,RF_ON);
			return_value = 1;
			pump_set_speed(lw_get_pump_speed(current_therapy_settings.treatment.flow_rate));
			pump.state = IDLE_SPEED;
		}
	}
	else if ((current_therapy_settings.pump.on == 0) &&
			((current_therapy_settings.pump.status & PUMP_DISABLED) == PUMP_DISABLED) &&
			(pump.state == DRIVE_STATUS) &&
			((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK))
	
	{
		pump.state = MOTOR_ENABLE;
		current_therapy_settings.pump.status &= ~PUMP_DISABLED;
	}
	else if ((ablate_flags == 0) &&
			(coag_flags == 0) &&
			(mode_flags == 0) &&			
			(pump_flags == 1) &&
			therapy_start_screen(current_therapy_settings.gui.screenID))
	{
		pump.state = IDLE_SPEED;
	}
	
	return return_value;
}

/***************************************************************************//**
This function turns on the therapy for the legacy wand. This function is called
when ablate or coag key is pressed. This function puts out the corresponding
output voltages.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t legacy_ent_reflex_wand_therapy_on (void)
{
	uint8_t return_value = 0;
	
	uint8_t ablate_flags =
		current_therapy_settings.footpedal.ablate +
		legacy_wand_params_obj.ablate_flag;
	
	uint8_t coag_flags =
		legacy_wand_params_obj.coag_flag +
		current_therapy_settings.footpedal.coag;
  
	uint8_t mode_flags = 
		current_therapy_settings.footpedal.mode +
		legacy_wand_params_obj.mode_flag +
		current_therapy_settings.footpedal.vac_key;
	
	uint8_t pump_flags = 
		current_therapy_settings.pump.on  |
		((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK) |
		((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON);
	
	/* Test ablate input from wand or foot switches */
	if ((ablate_flags == 1) &&
		(coag_flags == 0) &&
		(mode_flags == 0) &&
		(pump_flags == 0) &&
		therapy_start_screen(current_therapy_settings.gui.screenID))
    
	{
		current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
		
		// moving to the legacy wand therapy screen if it is not that screen already
		current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
    
		/* copy RTC_TSR to ram for logging */
		memcpy(&start_duration,(uint8_t*)0x4003D000,sizeof(uint32_t));
    
		legacy_wand_params_obj.level= ent_legacy_wand_ablate_dac_values[legacy_wand_params_obj.ablate_level];
		over_energy_current_limit = ent_legacy_over_energy_current_limits_for_ablate[legacy_wand_params_obj.ablate_level];
		current_therapy_settings.treatment.previous_therapy_mode =
		current_therapy_settings.treatment.therapy_mode;   
		current_therapy_settings.log.mode = current_therapy_settings.treatment.therapy_mode;
		current_therapy_settings.treatment.ablate_on = 1;    
		legacy_wand_params_obj.therapy_on = 1;
		
		// Turn ON RF is activation time less than activation time limit
		put_event(&evt_tbl,RF_ON);
		return_value = 1;
	}
	else if ((ablate_flags == 0) &&
			(coag_flags == 1) &&
			(mode_flags == 0) &&			
			(pump_flags == 0) &&
			therapy_start_screen(current_therapy_settings.gui.screenID))
	{
		if (legacy_wand_params_obj.coag_enable)
		{
			current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
			// moving to the legacy wand therapy screen if it is not that screen already
			current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
			
			/* copy RTC_TSR to ram for logging */
			memcpy(&start_duration,(uint8_t*)0x4003D000,sizeof(uint32_t));
			/* auto-switch screens but save current screen to revert back 
			later */
			current_therapy_settings.log.mode = COAG;
			legacy_wand_params_obj.level= ent_legacy_wand_coag_dac_values[legacy_wand_params_obj.coag_level];
			over_energy_current_limit = ent_legacy_over_energy_current_limits_for_coag[legacy_wand_params_obj.coag_level];
			current_therapy_settings.treatment.coag_on = 1;
			legacy_wand_params_obj.therapy_on = 1;
			// Turn ON RF is activation time less than activation time limit
			put_event(&evt_tbl,RF_ON);
			return_value = 1;
		}
	}
	else if ((current_therapy_settings.pump.status & PUMP_DISABLED) != PUMP_DISABLED)
	
	{
		pump.state = STOP_KILL;
	}
	
	return return_value;
}

/***************************************************************************//**
This function turns off the therapy for the legacy wand. This function is called
when ablate or coag key is released. This function shuts off the buck and the
RF output.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t legacy_wand_therapy_off(void)
{
   /* Disable RF output */
   RF_OFF(pLegacyWandFTM);
   
   if (current_therapy_settings.treatment.ablate_on == 1)
   {
     current_therapy_settings.log.legacy_ablate_on = 1;
     current_therapy_settings.log.legacy_coag_on = 0;
   }
   else if (current_therapy_settings.treatment.coag_on == 1)
   {
     current_therapy_settings.log.legacy_coag_on = 1;
     current_therapy_settings.log.legacy_ablate_on = 0;
   }
   else
   {
     current_therapy_settings.log.legacy_ablate_on = 0;
     current_therapy_settings.log.legacy_coag_on = 0;
   }
   
   /* Update status field */  
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.z_test = FALSE;     
  
  
   /* revert back to treatment screen */
   current_therapy_settings.gui.screenID = current_therapy_settings.gui.auto_revert_back_screenID;
  
   current_therapy_settings.gui.auto_revert_back_screenID = 0;
   
  /* turn yellow LED OFF */
  //GPIOA_PSOR |= YELLOW_LED;
  GPIOE_PCOR |= (EXT_CTRL_2 | EXT_CTRL_3);   
  current_therapy_settings.treatment.therapy_mode = 
    current_therapy_settings.treatment.previous_therapy_mode;
  
  current_therapy_settings.log.mode =
    current_therapy_settings.treatment.therapy_mode;
  
  legacy_wand_params_obj.therapy_on = 0;
  
  /*
    @ENT
    turn the pump off when the RF is no longer needed
  */
  if((current_therapy_settings.controller == ENT)&&
      (current_therapy_settings.legacy_wand_levels.topaz_timer == 0))
  {
    pump_set_speed(lw_get_pump_speed(0));
  }
  else
  {
    //_DO_NOTHING;
  }
  
  return 1;
}

/***************************************************************************//**
This function turns on the therapy for the werewolf wand. This function is called
when ablate or coag key is pressed. This function puts out the corresponding
output voltages.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t therapy_on(void)
{
  
    cancel_new_profile();
  
    /* Test ablate input from wand or foot switches */
    if (!(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
        (((current_therapy_settings.wand.cut_key == 1) && (current_therapy_settings.footpedal.ablate == 0)) ||
         ((current_therapy_settings.wand.cut_key == 0) && (current_therapy_settings.footpedal.ablate == 1))) &&
         (current_therapy_settings.footpedal.coag == 0) && (current_therapy_settings.footpedal.vac_key == 0) && 
         (current_therapy_settings.wand.coag_key == 0) && (current_therapy_settings.wand.vac_key == 0) &&
         (current_therapy_settings.footpedal.mode == 0) && (current_therapy_settings.wand.mode_key == 0) &&
         (current_therapy_settings.wand.valid == TRUE) && (current_therapy_settings.pump.on == 1) &&
        (therapy_start_screen(current_therapy_settings.gui.screenID)) &&
         ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK) && ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON) &&
         (current_therapy_settings.treatment.ablate_enable))
    {
    
      current_therapy_settings.treatment.previous_therapy_mode =
        current_therapy_settings.treatment.therapy_mode;   
      
      /* auto-switch screens but save current screen to revert back later */
      current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
      /* check sports med or ent wand */
      if (current_therapy_settings.controller == SPORTS_MED)
      {
        current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;     
      }
      else
      {
        current_therapy_settings.gui.screenID = ENT_SMART_WAND_TREATMENT_SCREEN;
      }
      current_therapy_settings.log.mode = current_therapy_settings.treatment.therapy_mode;
      
      /* update parameters */
      switch_ablate_parameters();
            
      /* copy RTC_TSR to ram for logging */
      memcpy(&therapy_start_time,(uint8_t*)0x4003D000,sizeof(uint32_t));
                 
      current_therapy_settings.treatment.ablate_on = 1;     
      current_therapy_settings.pump.ctrl_en = TRUE;
      /* switch off buck */
      GPIOE_PCOR |= GPIO_PDOR_PDO(BUCK_WW_ON);
            
      /* Transition to THERAPY state */
      return 1;
    
    }
  /* Test Manta(COAG) input from wand or foot switches */
  else if ((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
           (((current_therapy_settings.wand.coag_key == 1) && (current_therapy_settings.footpedal.coag == 0)) ||
             ((current_therapy_settings.wand.coag_key == 0) && (current_therapy_settings.footpedal.coag == 1))) &&
                 (current_therapy_settings.wand.cut_key == 0) && (current_therapy_settings.wand.vac_key == 0) &&
                   (current_therapy_settings.footpedal.mode == 0) && (current_therapy_settings.wand.mode_key == 0) &&
                     (current_therapy_settings.wand.valid == TRUE) && (current_therapy_settings.pump.on == 1) &&
               (therapy_start_screen(current_therapy_settings.gui.screenID)) &&
         ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK) && 
           ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON) &&
                             (current_therapy_settings.treatment.ablate_enable))
    {

      uint8_t PrimeRunning = get_priming_status() != PRIME_OFF ? 1 : 0;
    
    if (PrimeRunning != 0)
    {
      gui_stop_auto_prime();
      return 0;
    }
      current_therapy_settings.treatment.therapy_mode = COAG;
      current_therapy_settings.treatment.previous_therapy_mode =
        current_therapy_settings.treatment.therapy_mode;
      /* auto-switch screens but save current screen to revert back later */      
    current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;  
        current_therapy_settings.gui.screenID = MANTA_SMART_WAND_TREATMENT_SCREEN;
    if(current_therapy_settings.treatment.pre_prime == FALSE)
      {
    current_therapy_settings.log.mode = COAG;
      /* Update parameters */
    Switch_Coag_Mode_Parameters();
    /* copy RTC_TSR to ram for logging */
    memcpy(&therapy_start_time,(uint8_t*)0x4003D000,sizeof(uint32_t));
    current_therapy_settings.treatment.coag_on = 1;
    current_therapy_settings.pump.ctrl_en = TRUE;
      GPIOE_PCOR |= GPIO_PDOR_PDO(BUCK_WW_ON); 
    }
    return 1;      
  }
  
  else if (!(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
           (((current_therapy_settings.wand.vac_key == 1) && (current_therapy_settings.footpedal.vac_key == 0) && (current_therapy_settings.footpedal.mode == 0)) ||
       ((current_therapy_settings.wand.vac_key == 0) && (current_therapy_settings.footpedal.vac_key == 1) && (current_therapy_settings.wand.mode_key == 0))) &&
        (current_therapy_settings.footpedal.coag == 0) && (current_therapy_settings.footpedal.ablate == 0) && 
        (current_therapy_settings.wand.coag_key == 0) && (current_therapy_settings.wand.cut_key == 0) && 
        (current_therapy_settings.wand.valid == TRUE) && (current_therapy_settings.pump.on == 1) &&
         therapy_start_screen(current_therapy_settings.gui.screenID) &&
                     ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK)  && 
                       ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON) &&
         (current_therapy_settings.controller == SPORTS_MED))
  {
    
      /* Save current mode to revert back once vac key is released */
      current_therapy_settings.treatment.previous_therapy_mode =
        current_therapy_settings.treatment.therapy_mode;   
                     
      /* Transition to vac mode */
      current_therapy_settings.treatment.therapy_mode = VAC;
      
      current_therapy_settings.log.mode = VAC;
      
      /* auto-switch screens but save current screen to revert back later */       
      current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;  
      current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;
        
      /* copy RTC_TSR to ram for logging */
      memcpy(&therapy_start_time,(uint8_t*)0x4003D000,sizeof(uint32_t));
      
      /* Update parameters */
      switch_ablate_parameters();
                               
      current_therapy_settings.treatment.ablate_on = 1;
      current_therapy_settings.pump.ctrl_en = TRUE;
      /* switch off buck */
      GPIOE_PCOR |= GPIO_PDOR_PDO(BUCK_WW_ON);
      
      /* Transition to THERAPY state */
      return 1;
                
    }     
    /* Test coagulation pedal from foot switch */
  else if (!(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
           (((current_therapy_settings.wand.coag_key == 1) && (current_therapy_settings.footpedal.coag == 0)) ||
         ((current_therapy_settings.wand.coag_key == 0) && (current_therapy_settings.footpedal.coag == 1))) &&
          (current_therapy_settings.footpedal.ablate == 0) && (current_therapy_settings.footpedal.vac_key == 0) && 
          (current_therapy_settings.wand.cut_key == 0) && (current_therapy_settings.wand.vac_key == 0) &&
          (current_therapy_settings.footpedal.mode == 0) && (current_therapy_settings.wand.mode_key == 0) &&
          (current_therapy_settings.wand.valid == TRUE) && (current_therapy_settings.pump.on == 1) &&
           therapy_start_screen(current_therapy_settings.gui.screenID) &&
                       ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK)  && 
                         ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON))
  {

      uint8_t PrimeRunning = get_priming_status() != PRIME_OFF ? 1 : 0;
    
    if (PrimeRunning != 0)
    {
      gui_stop_auto_prime();
      return 0;
    }
    
    /* auto-switch screens but save current screen to revert back later */      
    current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;  
    if (current_therapy_settings.controller == SPORTS_MED)
    {
      current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;     
      }
      else
      {
      current_therapy_settings.gui.screenID = ENT_SMART_WAND_TREATMENT_SCREEN;
    } 
    current_therapy_settings.log.mode = COAG;
    
        /* Update parameters */
        Switch_Coag_Mode_Parameters();
    
        /* copy RTC_TSR to ram for logging */
        memcpy(&therapy_start_time,(uint8_t*)0x4003D000,sizeof(uint32_t));
    
        current_therapy_settings.treatment.coag_on = 1;
        current_therapy_settings.pump.ctrl_en = TRUE;
    GPIOE_PCOR |= GPIO_PDOR_PDO(BUCK_WW_ON);    
    return 1;
    
    }
  
  /* Test Turbo Mode MANTA */
  else if ((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) &&
           ((current_therapy_settings.wand.coag_key == 0) && (current_therapy_settings.footpedal.coag == 0)) &&
             (current_therapy_settings.footpedal.ablate == 0) && (current_therapy_settings.footpedal.vac_key == 0) && 
               (current_therapy_settings.wand.cut_key == 0) && (current_therapy_settings.wand.vac_key == 0) &&
                 (current_therapy_settings.footpedal.mode == 0) && (current_therapy_settings.wand.mode_key == 1) &&
                   (current_therapy_settings.wand.valid == TRUE) && (current_therapy_settings.pump.on == 1) &&
                     therapy_start_screen(current_therapy_settings.gui.screenID) &&
                       ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK)  && 
                         ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON))
    {
      uint8_t PrimeRunning = get_priming_status() != PRIME_OFF ? 1 : 0;
    
    if (PrimeRunning != 0)
    {
      gui_stop_auto_prime();
      return 0;
    }
    
    current_therapy_settings.treatment.therapy_mode = COAG;
    current_therapy_settings.treatment.previous_therapy_mode = current_therapy_settings.treatment.therapy_mode;
    /* auto-switch screens but save current screen to revert back later */      
    current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
    current_therapy_settings.gui.screenID = MANTA_SMART_WAND_TREATMENT_SCREEN;
    if(current_therapy_settings.treatment.pre_prime == FALSE)
    {
      current_therapy_settings.treatment.ent_turbo_mode = TRUE;
      current_therapy_settings.setpoint.coag = COAG_10;      
      current_therapy_settings.log.mode = COAG;
      /* Update parameters */
      Switch_Coag_Mode_Parameters();
      /* copy RTC_TSR to ram for logging */
      memcpy(&therapy_start_time,(uint8_t*)0x4003D000,sizeof(uint32_t));
      current_therapy_settings.treatment.coag_on = 1;
      current_therapy_settings.pump.ctrl_en = TRUE;
      GPIOE_PCOR |= GPIO_PDOR_PDO(BUCK_WW_ON);
    }
    return 1; 
            
  }
  else if ((get_priming_status() == PRIME_AUTO) &&
      (current_therapy_settings.wand.valid == TRUE) &&
      (current_therapy_settings.pump.on == 1) &&
      therapy_start_screen(current_therapy_settings.gui.screenID) &&
     ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK) &&
     ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON))
  {
    current_therapy_settings.gui.auto_revert_back_screenID = current_therapy_settings.gui.screenID;
    current_therapy_settings.gui.screenID = MANTA_SMART_WAND_TREATMENT_SCREEN;
    //GPIOE_PCOR |= GPIO_PDOR_PDO(BUCK_WW_ON);        /* Keep RF off for Auto-prime since saline detection feature removed*/
    current_therapy_settings.pump.quiescent_delay = 0;
    current_therapy_settings.pump.pulse_delay = 0xFFFF;
    current_therapy_settings.pump.pulse_rps = smart_wand_params.manta.prime.autom.initial_spd;
    current_therapy_settings.pump.ent_flow = smart_wand_params.manta.prime.autom.final_spd;
    current_therapy_settings.pump.ctrl_en = TRUE;
    return 1;
  }       
  else if ((current_therapy_settings.pump.on == 0) &&
          ((current_therapy_settings.pump.status & PUMP_DISABLED) == PUMP_DISABLED) &&
           (pump.state == DRIVE_STATUS) &&
           ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK))
  {
    
    pump.state = MOTOR_ENABLE;
    current_therapy_settings.pump.status &= ~PUMP_DISABLED;
    return 0;
    
  }
  else if ((current_therapy_settings.wand.coag_key == 0) && (current_therapy_settings.footpedal.coag == 0) &&
           (current_therapy_settings.footpedal.ablate == 0) && (current_therapy_settings.footpedal.vac_key == 0) && 
           (current_therapy_settings.wand.cut_key == 0) && (current_therapy_settings.wand.vac_key == 0) &&
           (current_therapy_settings.footpedal.mode == 0) && (current_therapy_settings.wand.mode_key == 0) &&
           (current_therapy_settings.wand.valid == TRUE) && (current_therapy_settings.pump.on == 1) &&
            therapy_start_screen(current_therapy_settings.gui.screenID) &&
                     ((current_therapy_settings.pump.status & SENSORS_OK) == SENSORS_OK)  && 
                       ((current_therapy_settings.pump.status & PUMP_ON) == PUMP_ON))
  {
    
    pump.state = IDLE_SPEED;
    return 0;
    
  }
  else
  {
    
    return 0;
    
  }
    
}

/***************************************************************************//**
This function is called after measuring the load impedance. Once the 
impedance is within range, this function is called which sets the Z-test flag
to true and turns on RF.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t measure_z(void)
{
  current_therapy_settings.wand.z_test = TRUE;
  
  put_event(&evt_tbl,RF_ON);
  
  return 0;
  
}

/***************************************************************************//**
This function is used to change mode for the Werewolf wand. It is used to
switch between LO, MED and HI
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t change_mode(void)
{
  MODES increment_mode[8][4] = {
    LO,LO,LO,LO,
    HI,HI,HI,HI,
    MED,MED,MED,MED,
    MED,MED,HI,MED,
    LO,LO,LO,LO,
    LO,HI,LO,LO,
    LO,MED,LO,LO,
    LO,MED,HI,LO, 
  };
  
  uint8_t modes_en = 0;
  uint8_t mode_changed = 0;
  
  if (therapy_start_screen(current_therapy_settings.gui.screenID))
  {
    cancel_new_profile();
    
    current_therapy_settings.gui.depth_idx = 0;    
    /* check sports med or ent wand */
    if (current_therapy_settings.controller == SPORTS_MED)
    {
      current_therapy_settings.gui.screenID = RF_2000_WAND_TREATMENT_SCREEN;     
    }
    else
    {
      current_therapy_settings.gui.screenID = ENT_SMART_WAND_TREATMENT_SCREEN;
    }
    
  }
  
  if ((current_therapy_settings.gui.screenID == RF_2000_WAND_TREATMENT_SCREEN) ||
      (current_therapy_settings.gui.screenID == ENT_SMART_WAND_TREATMENT_SCREEN))
  {
  
    modes_en = ((smart_wand_params.mfg.low_enable << 2) |
                (smart_wand_params.mfg.med_enable << 1) |
                (smart_wand_params.mfg.hi_enable));
    modes_en &= 0x7;
    MODES new_mode = increment_mode[modes_en][current_therapy_settings.treatment.therapy_mode]; 
      
    if ((new_mode <= current_therapy_settings.treatment.therapy_mode) &&
        (current_therapy_settings.controller == ENT) &&
        (current_therapy_settings.treatment.ablate_enable == 1))
    {
      current_therapy_settings.treatment.ablate_enable = FALSE;
    }
    else 
    {
      current_therapy_settings.treatment.ablate_enable = TRUE;
      mode_changed = 1;
      current_therapy_settings.treatment.therapy_mode = increment_mode[modes_en][current_therapy_settings.treatment.therapy_mode]; 
    }
        
    /* Update parameters */
    switch_ablate_parameters();
    if ((current_therapy_settings.pump.status & PUMP_READY) == PUMP_READY)
    {
      
      pump.state = IDLE_SPEED;
      
    }
        
  }
  return mode_changed;   
}


/***************************************************************************//**
This function turns off the therapy for the werewolf wand. This function is called
when ablate or coag key is released. This function shuts off the buck and the
RF output.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t therapy_off(void)
{
  
   /* Disable RF output */
   RF_OFF(pSmartWandFTM);
   
   /* Update status field */  
   current_therapy_settings.treatment.ablate_on = 0;
   current_therapy_settings.treatment.coag_on = 0;
   current_therapy_settings.pump.ctrl_en = FALSE;
   current_therapy_settings.wand.z_test = FALSE;
  
   current_therapy_settings.gui.screenID = current_therapy_settings.gui.auto_revert_back_screenID;
   if(current_therapy_settings.treatment.ent_turbo_mode == TRUE)
   {
     DISABLE_AUDIO;
     FTM2_MOD = 1678-1;  
     current_therapy_settings.treatment.ent_turbo_mode = FALSE;
   }
   
  
   current_therapy_settings.gui.auto_revert_back_screenID = 0;
   
  /* turn yellow LED OFF */
  //GPIOE_PCOR |= (EXT_CTRL_3); 
   if(prime.pump_running == FALSE)
   {
     log_use_data(therapy_start_time);
   }
   
  current_therapy_settings.treatment.therapy_mode = 
    current_therapy_settings.treatment.previous_therapy_mode;
  current_therapy_settings.log.mode =
    current_therapy_settings.treatment.therapy_mode;
  
  uint8_t PrimeRunning = get_priming_status() != PRIME_OFF ? 1 : 0;

  if (PrimeRunning != 0)
  {
    gui_stop_auto_prime();
  }
  prime.pump_running = FALSE;

  return 1;
  
}



/***************************************************************************//**
This function turns ON the RF section of the board for werewolf wands
RF output.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t rf_on(void)
{
  
  /* Commence soft-start */
  if ((!FAULT_FTM(pSmartWandFTM)) && 
      (SWOC_FTM(pSmartWandFTM) == FTM_SWOCTRL_CH0OC_MASK) &&
      (current_therapy_settings.setpoint.dac == 0))
  {
      
      /* Reinstate fault controller */
      pSmartWandFTM->FMS &= ~FTM_FMS_FAULTF_MASK;
      
      RF_ON(pSmartWandFTM);
      if (!unitTest)
      /* Wait for sync to complete */
      while((FTM3_SYNC & FTM_SYNC_SWSYNC_MASK) == FTM_SYNC_SWSYNC_MASK);
      
      /* Re-enable interrupt for fault controller */
      pSmartWandFTM->MODE |= FTM_MODE_FAULTIE_MASK;
                 
      //GPIOE_PSOR = EXT_CTRL_3;
          
  }
  else if (((current_therapy_settings.treatment.ablate_on == 1) ||
            (current_therapy_settings.treatment.coag_on == 1)) &&
            (SWOC_FTM(pSmartWandFTM) == FTM_SWOCTRL_CH0OC_MASK) &&
            (current_therapy_settings.wand.tube_warn == 0))
  {
    
    put_event(&evt_tbl,RF_ON);
    
  }
  
  
  return 1;
          
}

/***************************************************************************//**
This function turns ON the RF section of the board for legacy wands
RF output.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t lw_rf_on(void)
{
  
  /* Commence soft-start */
  if ((!FAULT_FTM(pLegacyWandFTM)) && 
      (SWOC_FTM(pLegacyWandFTM) == FTM_SWOCTRL_CH0OC_MASK) &&
      (current_therapy_settings.legacy_wand_levels.dac == 0))
  {
      
      /* Reinstate fault controller */
      pLegacyWandFTM->FMS &= ~FTM_FMS_FAULTF_MASK;
      
      RF_ON(pLegacyWandFTM);
      
      if (!unitTest)
      /* Wait for sync to complete */
      while((FTM0_SYNC & FTM_SYNC_SWSYNC_MASK) == FTM_SYNC_SWSYNC_MASK);
  
      /* Re-enable interrupt for fault controller */
      pLegacyWandFTM->MODE |= FTM_MODE_FAULTIE_MASK;
      
      /* switch on buck */
      GPIOE_PCOR |= GPIO_PDOR_PDO(BUCK_18PIN_ON);
            
      //GPIOE_PSOR = (EXT_CTRL_2 | EXT_CTRL_3);
      
      legacy_wand_params_obj.z_test =  TRUE;
          
  }
  else if (((current_therapy_settings.treatment.ablate_on == 1) ||
            (current_therapy_settings.treatment.coag_on == 1)) &&
           (SWOC_FTM(pLegacyWandFTM) == FTM_SWOCTRL_CH0OC_MASK))
  {
    
    put_event(&evt_tbl,RF_ON);
    
  }
  return 0;
}

/***************************************************************************//**
This function turns OFF the RF section of the board for werewolf wands
RF output.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t rf_off(void)
{
  /* Force PWM outputs low */
  RF_OFF(pSmartWandFTM);
  
  return 0;
}

/***************************************************************************//**
This function turns OFF the RF section of the board for legacy wands
RF output.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t lw_rf_off(void)
{
  /* Force PWM outputs low */
  RF_OFF(pLegacyWandFTM);
 
  return 0;
}

/***************************************************************************//**
This function loads ablate parameters corresponding to the LO, MED and HI modes
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t switch_ablate_parameters(void)
{
  if (current_therapy_settings.controller != ENT)  /* For No wand, SM, APL wands */
  {
    load_ablate_params_sm(current_therapy_settings.treatment.therapy_mode);
    current_therapy_settings.treatment.ablate_enable = TRUE;
    if(current_therapy_settings.controller != SPORTS_MED) 
    {
      current_therapy_settings.treatment.flow_auto = TRUE; 
    }
  }
  else /* For ENT Smart wands Only */
  {
    if(current_therapy_settings.active_wand.legacy_wand != ENT)
    {
      load_ablate_params_ent(current_therapy_settings.treatment.therapy_mode);
      current_therapy_settings.treatment.flow_auto = FALSE;
    }
  }
  
  return 0;
}

/***************************************************************************//**
This function loads therapy values for user selected mode
@param void
@return 0
@outputs
@context
*******************************************************************************/
ENT_ABLATE_MODE_PARAMS_T* load_ablate_mode_params_ent(ENT_ABLATE_MODE_PARAMS_T *mode, CUT_LEVELS cut_level, ENT_FLOW_SP_T flow_sp)
{
  /* subtract 1 b/c wand params don't have values for zero level */
  if (cut_level > (CUT_LEVELS)0)
  {
    cut_level -= (CUT_LEVELS)1;
  }
  
  current_therapy_settings.setpoint.level = mode->level[cut_level].primary_voltage;
  current_therapy_settings.setpoint.ent_primary_level = mode->level[cut_level].primary_voltage;
  current_therapy_settings.setpoint.ent_secondary_level = mode->level[cut_level].secondary_voltage;
  current_therapy_settings.setpoint.ent_duty_on_time = mode->level[cut_level].blend_primary_voltage_time;
  current_therapy_settings.setpoint.ent_duty_total_time = mode->level[cut_level].blend_total_time;
  current_therapy_settings.log.level = cut_level;            
  current_therapy_settings.pulsing.energy_threshold = mode->pulse_energy;
  current_therapy_settings.pulsing.pulse_window = mode->pulse_window;
  current_therapy_settings.pulsing.target_current = mode->rms_current_limit;
  current_therapy_settings.pump.ent_flow = mode->level[cut_level].flow[flow_sp];
  return mode;
  
}

void load_ablate_params_ent(MODES therapy_mode)
{
  
  switch (therapy_mode)
  {
    
    case (LO):
    {  
      CUT_LEVELS level = current_therapy_settings.setpoint.lo;
      load_ablate_mode_params_ent(&smart_wand_params.ent.lo,
                                  level,
                                  current_therapy_settings.treatment.ent_flow_sp[LO][level]); 
      current_therapy_settings.treatment.flow_rate = 
        current_therapy_settings.treatment.ent_flow_sp[LO][level];
      
      break;
    }
    case (MED):
    {  
      CUT_LEVELS level = current_therapy_settings.setpoint.med;
      load_ablate_mode_params_ent(&smart_wand_params.ent.med,
                                  level,
                                  current_therapy_settings.treatment.ent_flow_sp[MED][level]); 
      current_therapy_settings.treatment.flow_rate = 
        current_therapy_settings.treatment.ent_flow_sp[MED][level];
      break;
    }
    case (HI):
    {  
      CUT_LEVELS level = current_therapy_settings.setpoint.hi;
      load_ablate_mode_params_ent(&smart_wand_params.ent.hi,
                                  level,
                                  current_therapy_settings.treatment.ent_flow_sp[HI][level]); 
      current_therapy_settings.treatment.flow_rate = 
        current_therapy_settings.treatment.ent_flow_sp[HI][level];
      break;
    }
    default:
    {
      current_therapy_settings.setpoint.level = 0;
      current_therapy_settings.setpoint.ent_primary_level = 0;
      current_therapy_settings.setpoint.ent_secondary_level = 0;
      current_therapy_settings.setpoint.ent_duty_on_time = 0;
      current_therapy_settings.setpoint.ent_duty_total_time = 0;
      current_therapy_settings.log.level = 0;            
      current_therapy_settings.pulsing.energy_threshold = 0;
      current_therapy_settings.pulsing.pulse_window = 0;
      current_therapy_settings.pump.idle_rps = 0;
      current_therapy_settings.pulsing.target_current = 0;
      current_therapy_settings.treatment.flow_rate = 0;
      break;
    }
  } /* end switch(therapy_mode) */
  current_therapy_settings.pump.quiescent_delay = smart_wand_params.ent.prime.pre.rf_delay;
  current_therapy_settings.pump.pulse_delay = smart_wand_params.ent.prime.pre.time;
  current_therapy_settings.pump.pulse_rps = smart_wand_params.ent.prime.pre.spd;
  current_therapy_settings.pump.idle_rps = 0;
  
  if ((current_therapy_settings.notify.status & 0xFF) == 0)
  {
    current_therapy_settings.audio.p_sound_file = 0;
  }
  
  current_therapy_settings.setpoint.ipeak_voltage = smart_wand_params.ent.ipeak.voltage;
  current_therapy_settings.setpoint.trip_load = smart_wand_params.ent.ipeak.trip_load;
  current_therapy_settings.treatment.previous_therapy_mode = therapy_mode;
  
  Update_Irms_Overcurrent_Thresholds();
  
}

void load_ablate_params_sm(MODES therapy_mode)
{
  switch (therapy_mode)
  {
     
    case (LO):
      
      /* Update dac output voltage level */
      switch(current_therapy_settings.setpoint.lo)
      {
               
        case (ABLATE_MINUS):
          current_therapy_settings.setpoint.level = smart_wand_params.lo.voltage_sp_minus;
          current_therapy_settings.pump.pid.sp_amps = smart_wand_params.lo.current_sp_minus;
          current_therapy_settings.log.level = 0;
          break;
          
        case (ABLATE_DEFAULT):
          current_therapy_settings.setpoint.level = smart_wand_params.lo.voltage_sp_default;
          current_therapy_settings.pump.pid.sp_amps = smart_wand_params.lo.current_sp_default;
          current_therapy_settings.log.level = 1;
          break;
          
        case (ABLATE_PLUS):
          current_therapy_settings.setpoint.level = smart_wand_params.lo.voltage_sp_plus;
          current_therapy_settings.pump.pid.sp_amps = smart_wand_params.lo.current_sp_plus;
          current_therapy_settings.log.level = 2;
          break;
          
      }
    
      current_therapy_settings.pulsing.energy_threshold = smart_wand_params.lo.pulse_energy;
      current_therapy_settings.pulsing.pulse_window = smart_wand_params.lo.pulse_window;
      current_therapy_settings.pump.idle_rps = smart_wand_params.lo.idle_rps;
      current_therapy_settings.pump.quiescent_delay = smart_wand_params.lo.quiescent_delay;
      current_therapy_settings.pump.pulse_delay = 0;
      current_therapy_settings.pump.pulse_rps = 0;
      current_therapy_settings.pump.pid.min_rps = smart_wand_params.lo.min_rps_default;
      current_therapy_settings.pump.pid.max_rps = smart_wand_params.lo.max_rps_default;
      current_therapy_settings.pump.pid.kp1 = smart_wand_params.lo.kp1;
      current_therapy_settings.pump.pid.kp2 = smart_wand_params.lo.kp2;
      current_therapy_settings.pump.pid.kp3 = smart_wand_params.lo.kp3;
      current_therapy_settings.pump.pid.ki1 = smart_wand_params.lo.ki1;
      current_therapy_settings.pump.pid.ki2 = smart_wand_params.lo.ki2;
      current_therapy_settings.pump.pid.ki3 = smart_wand_params.lo.ki3;
      current_therapy_settings.pump.pid.e1 = smart_wand_params.lo.e1;
      current_therapy_settings.pump.pid.e2 = smart_wand_params.lo.e2;
      current_therapy_settings.pump.vac.voltage_sp_default = 0;
      current_therapy_settings.pump.vac.pulse_delay = 0;
      current_therapy_settings.pump.vac.peak_voltage = 0;
      current_therapy_settings.pump.vac.peak_time = 0;
      current_therapy_settings.pump.vac.max_time = 0;
      current_therapy_settings.pulsing.target_current = smart_wand_params.lo.ss_time;
      /* Save current mode to revert back once vac key is released */
      current_therapy_settings.treatment.previous_therapy_mode = therapy_mode;
    
      break;
    
    case (MED):
      
      /* Update dac output voltage level */
      switch(current_therapy_settings.setpoint.med)
      {
                
        case (ABLATE_MINUS):
      
          current_therapy_settings.setpoint.level = smart_wand_params.med.voltage_sp_minus;
          current_therapy_settings.pump.pid.sp_amps = smart_wand_params.med.current_sp_minus;
          current_therapy_settings.log.level = 0;
          break;
          
        case (ABLATE_DEFAULT):
      
          current_therapy_settings.setpoint.level = smart_wand_params.med.voltage_sp_default;
          current_therapy_settings.pump.pid.sp_amps = smart_wand_params.med.current_sp_default;
          current_therapy_settings.log.level = 1;
          break;
          
        case (ABLATE_PLUS):
      
          current_therapy_settings.setpoint.level = smart_wand_params.med.voltage_sp_plus;
          current_therapy_settings.pump.pid.sp_amps = smart_wand_params.med.current_sp_plus;
          current_therapy_settings.log.level = 2;
          break;
          
      }
    
      current_therapy_settings.pulsing.energy_threshold = smart_wand_params.med.pulse_energy;
      current_therapy_settings.pulsing.pulse_window = smart_wand_params.med.pulse_window;
      current_therapy_settings.pump.idle_rps = smart_wand_params.med.idle_rps;
      current_therapy_settings.pump.quiescent_delay = 0;
      current_therapy_settings.pump.pulse_delay = 0;
      current_therapy_settings.pump.pulse_rps = 0;
      current_therapy_settings.pump.pid.min_rps = smart_wand_params.med.min_rps_default;
      current_therapy_settings.pump.pid.max_rps = smart_wand_params.med.max_rps_default;
      current_therapy_settings.pump.pid.kp1 = smart_wand_params.med.kp1;
      current_therapy_settings.pump.pid.kp2 = smart_wand_params.med.kp2;
      current_therapy_settings.pump.pid.kp3 = smart_wand_params.med.kp3;
      current_therapy_settings.pump.pid.ki1 = smart_wand_params.med.ki1;
      current_therapy_settings.pump.pid.ki2 = smart_wand_params.med.ki2;
      current_therapy_settings.pump.pid.ki3 = smart_wand_params.med.ki3;
      current_therapy_settings.pump.pid.e1 = smart_wand_params.med.e1;
      current_therapy_settings.pump.pid.e2 = smart_wand_params.med.e2;
      current_therapy_settings.pump.vac.voltage_sp_default = 0;
      current_therapy_settings.pump.vac.pulse_delay = 0;
      current_therapy_settings.pump.vac.peak_voltage = 0;
      current_therapy_settings.pump.vac.peak_time = 0;
      current_therapy_settings.pump.vac.max_time = 0;
      current_therapy_settings.pulsing.target_current = smart_wand_params.med.ss_time;
      /* Save current mode to revert back once vac key is released */
      current_therapy_settings.treatment.previous_therapy_mode = therapy_mode;
    
      break;
    
    case (HI):
      
      /* Update dac output voltage level */
      switch(current_therapy_settings.setpoint.hi)
      {
        
        case (ABLATE_MINUS):
      
          current_therapy_settings.setpoint.level = smart_wand_params.hi.voltage_sp_minus;
          current_therapy_settings.pump.pid.max_rps = smart_wand_params.hi.max_rps_minus;
          current_therapy_settings.log.level = 0;
          break;
          
        case (ABLATE_DEFAULT):
      
          current_therapy_settings.setpoint.level = smart_wand_params.hi.voltage_sp_default;
          current_therapy_settings.pump.pid.max_rps = smart_wand_params.hi.max_rps_default;
          current_therapy_settings.log.level = 1;
          break;
          
        case (ABLATE_PLUS):
      
          current_therapy_settings.setpoint.level = smart_wand_params.hi.voltage_sp_plus;
          current_therapy_settings.pump.pid.max_rps = smart_wand_params.hi.max_rps_plus;
          current_therapy_settings.log.level = 2;
          break;
          
      }
    
      current_therapy_settings.pulsing.energy_threshold = smart_wand_params.hi.pulse_energy;
      current_therapy_settings.pulsing.pulse_window = smart_wand_params.hi.pulse_window;
      current_therapy_settings.pump.idle_rps = smart_wand_params.hi.idle_rps;
      current_therapy_settings.pump.quiescent_delay = 0;
      current_therapy_settings.pump.pulse_delay = 0;
      current_therapy_settings.pump.pulse_rps = 0;
    
      current_therapy_settings.pump.pid.min_rps = smart_wand_params.hi.min_rps_default;
      current_therapy_settings.pump.pid.kp1 = smart_wand_params.hi.kp1;
      current_therapy_settings.pump.pid.kp2 = smart_wand_params.hi.kp2;
      current_therapy_settings.pump.pid.kp3 = smart_wand_params.hi.kp3;
      current_therapy_settings.pump.pid.ki1 = smart_wand_params.hi.ki1;
      current_therapy_settings.pump.pid.ki2 = smart_wand_params.hi.ki2;
      current_therapy_settings.pump.pid.ki3 = smart_wand_params.hi.ki3;
      current_therapy_settings.pump.pid.e1 = smart_wand_params.hi.e1;
      current_therapy_settings.pump.pid.e2 = smart_wand_params.hi.e2;
      current_therapy_settings.pump.pid.sp_amps = smart_wand_params.hi.current_sp_default;
      current_therapy_settings.pump.vac.voltage_sp_default = 0;
      current_therapy_settings.pump.vac.pulse_delay = 0;
      current_therapy_settings.pump.vac.peak_voltage = 0;
      current_therapy_settings.pump.vac.peak_time = 0;
      current_therapy_settings.pump.vac.max_time = 0;
      current_therapy_settings.pulsing.target_current = smart_wand_params.hi.ss_time;
      /* Save current mode to revert back once vac key is released */
      current_therapy_settings.treatment.previous_therapy_mode = therapy_mode;
      break;
    
    case(VAC):
      current_therapy_settings.log.level = 0;
      current_therapy_settings.setpoint.level = smart_wand_params.vac.pulsing_voltage;
      current_therapy_settings.pulsing.energy_threshold =  smart_wand_params.vac.pulse_energy;
      current_therapy_settings.pulsing.pulse_window =  smart_wand_params.vac.pulse_window;
      
      current_therapy_settings.pump.quiescent_delay = 0;
      current_therapy_settings.pump.pulse_delay = smart_wand_params.vac.pulse_delay;
      current_therapy_settings.pump.pulse_rps = smart_wand_params.vac.pulse_rps;
      
      current_therapy_settings.pump.pid.min_rps = smart_wand_params.vac.min_rps_default;
      current_therapy_settings.pump.pid.max_rps = smart_wand_params.vac.max_rps_default;
      current_therapy_settings.pump.pid.kp1 = smart_wand_params.vac.kp1;
      current_therapy_settings.pump.pid.kp2 = 0;
      current_therapy_settings.pump.pid.kp3 = 0;
      current_therapy_settings.pump.pid.ki1 = smart_wand_params.vac.ki1;
      current_therapy_settings.pump.pid.ki2 = 0;
      current_therapy_settings.pump.pid.ki3 = 0;
      current_therapy_settings.pump.pid.e1 = 0x7FFF;
      current_therapy_settings.pump.pid.e2 = 0;
      current_therapy_settings.pump.pid.sp_amps = smart_wand_params.vac.current_sp_default;
      current_therapy_settings.pump.vac.voltage_sp_default = smart_wand_params.vac.voltage_sp_default;
      current_therapy_settings.pump.vac.pulse_delay = smart_wand_params.vac.pulse_delay;
      current_therapy_settings.pump.vac.peak_voltage = smart_wand_params.vac.peak_voltage;
      current_therapy_settings.pump.vac.peak_time = smart_wand_params.vac.peak_time;
      current_therapy_settings.pump.vac.max_time = smart_wand_params.vac.max_time;
      current_therapy_settings.pulsing.target_current = smart_wand_params.vac.ss_time;
      break;
    
    default:
      current_therapy_settings.log.level = 0;
      current_therapy_settings.setpoint.level = 0;
    
      current_therapy_settings.pulsing.energy_threshold = 24;
      current_therapy_settings.pulsing.pulse_window = 60;
      current_therapy_settings.pump.idle_rps = 0;
      current_therapy_settings.pump.quiescent_delay = 0;
      current_therapy_settings.pump.pulse_delay = 0;
      current_therapy_settings.pump.pulse_rps = 0;
    
      current_therapy_settings.pump.pid.min_rps = 0;
      current_therapy_settings.pump.pid.max_rps = 0;
      current_therapy_settings.pump.pid.kp1 = 0;
      current_therapy_settings.pump.pid.kp2 = 0;
      current_therapy_settings.pump.pid.kp3 = 0;
      current_therapy_settings.pump.pid.ki1 = 0;
      current_therapy_settings.pump.pid.ki2 = 0;
      current_therapy_settings.pump.pid.ki3 = 0;
      current_therapy_settings.pump.pid.e1 = 0;
      current_therapy_settings.pump.pid.e2 = 0;
      current_therapy_settings.pump.pid.sp_amps = 0;
      current_therapy_settings.pump.vac.voltage_sp_default = 0;
      current_therapy_settings.pump.vac.pulse_delay = 0;
      current_therapy_settings.pump.vac.peak_voltage = 0;
      current_therapy_settings.pump.vac.peak_time = 0;
      current_therapy_settings.pump.vac.max_time = 0;
      current_therapy_settings.pulsing.target_current = 0;
      
      break;
    
  } /* end switch(therapy_mode) */
     
  if ((current_therapy_settings.notify.status & 0xFF) == 0)
  {
    
    current_therapy_settings.audio.p_sound_file = 0;
    
  }
  
    /* reset wand wear monitors */
  current_therapy_settings.wand.irms_wear = (bool)wand_irms_wear_monitor(0,1);
  current_therapy_settings.wand.thandle_wear = (bool)wand_thandle_wear_monitor(0,0,1);
  
  /* limit output to 600 rpm */
    if (current_therapy_settings.pump.pid.max_rps > 10.0f)
    {
    
      current_therapy_settings.pump.pid.max_rps = 10.0f;
    
    }
  
    /* limit output to 600 rpm */
    if (current_therapy_settings.pump.idle_rps > 10.0f) 
    {
    
      current_therapy_settings.pump.idle_rps = 10.0f;
    
    }
  
    /* limit only clockwise rotation */
    if (current_therapy_settings.pump.pid.min_rps < 0.0f) 
    {
    
      current_therapy_settings.pump.pid.min_rps = 0.0f;
    
    }
  
    /* only permit clockwise rotation */
    if (current_therapy_settings.pump.idle_rps < 0.0f) 
    {
    
      current_therapy_settings.pump.idle_rps = 0.0f;
    
    }
  current_therapy_settings.setpoint.ipeak_voltage = smart_wand_params.ipeak.voltage;
  current_therapy_settings.setpoint.trip_load = smart_wand_params.ipeak.trip_load;
  
  Update_Irms_Overcurrent_Thresholds();
  
}

void Switch_Coag_Mode_Parameters(void)
{
  
  if (current_therapy_settings.controller == SPORTS_MED)
  {
    if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA)
    {
      load_coag_params_manta(current_therapy_settings.setpoint.coag);
    }
    else
    {
      load_coag_params_sm(current_therapy_settings.setpoint.coag);
    }
  }
  else
  {
    load_coag_params_ent(current_therapy_settings.setpoint.coag);
  }
  
}


/***************************************************************************//**
This function loads the DAC out and pump speed values accoridng to the coag mode
@param void
@return 0
@outputs
@context
*******************************************************************************/
void load_coag_params_ent(COAG_LEVELS coag_level)
{
  /* subtract 1 b/c wand params don't have values for zero level */
  if (coag_level > (COAG_LEVELS)0)
  {
    coag_level -= (COAG_LEVELS)1;
  }
  
  current_therapy_settings.setpoint.level = smart_wand_params.ent.coag[coag_level].primary_v_limit;
  current_therapy_settings.setpoint.ent_primary_level = smart_wand_params.ent.coag[coag_level].primary_v_limit;
  current_therapy_settings.setpoint.ent_secondary_level = smart_wand_params.ent.coag[coag_level].secondary_v_limit;
  current_therapy_settings.setpoint.ent_coag_power_primary = smart_wand_params.ent.coag[coag_level].primary * ADC_COUNTS_PER_WATT;
  current_therapy_settings.setpoint.ent_coag_power_secondary = smart_wand_params.ent.coag[coag_level].secondary * ADC_COUNTS_PER_WATT;
  current_therapy_settings.setpoint.ent_duty_on_time = smart_wand_params.ent.coag[coag_level].blend_primary_time;
  current_therapy_settings.setpoint.ent_duty_total_time = smart_wand_params.ent.coag[coag_level].blend_total_time;
  current_therapy_settings.pump.idle_rps = 0.0f;
  current_therapy_settings.pump.ent_flow = smart_wand_params.ent.coag[coag_level].pump_spd;
  current_therapy_settings.log.level = coag_level;
  
  if ((current_therapy_settings.notify.status & 0xFF) == 0)
  {
    
    current_therapy_settings.audio.p_sound_file = 0;
    
  }
  
  current_therapy_settings.pulsing.target_current = 0;
  current_therapy_settings.pulsing.energy_threshold = 400;
  current_therapy_settings.pulsing.pulse_window = 999;
  
  Update_Irms_Overcurrent_Thresholds();
      
}

/***************************************************************************//**
This function loads the DAC out and pump speed values accoridng to the coag mode
@param void
@return 0
@outputs
@context
*******************************************************************************/
void load_coag_params_sm(COAG_LEVELS coag_level)
{
  
  /* Update dac output voltage level */
  switch(coag_level)
  {
        
    case (COAG_1):
  
      current_therapy_settings.setpoint.level = smart_wand_params.coag.coag1_voltage;
      current_therapy_settings.pump.idle_rps = smart_wand_params.coag.coag1_rpm;
      current_therapy_settings.log.level = 0;
      break;
      
    case (COAG_2):
    
      current_therapy_settings.setpoint.level = smart_wand_params.coag.coag2_voltage;
      current_therapy_settings.pump.idle_rps = smart_wand_params.coag.coag1_rpm;
      current_therapy_settings.log.level = 1;
      break;
      
  }
  if ((current_therapy_settings.notify.status & 0xFF) == 0)
  {
    
    current_therapy_settings.audio.p_sound_file = 0;
    
  }
  
  current_therapy_settings.pulsing.target_current = 18866;
  
  current_therapy_settings.pump.quiescent_delay = 0;
  
    
    /* limit output to 600 rpm */
    if (current_therapy_settings.pump.idle_rps > 10.0f) 
    {
    
      current_therapy_settings.pump.idle_rps = 10.0f;
    
    }
  
    /* only permit clockwise rotation */
    if (current_therapy_settings.pump.idle_rps < 0.0f) 
    {
    
      current_therapy_settings.pump.idle_rps = 0.0f;
    }
  
  Update_Irms_Overcurrent_Thresholds();
      
  
}

/***************************************************************************//**
This function loads the DAC out and pump speed values accoridng to the coag mode
@param void
@return 0
@outputs
@context
*******************************************************************************/
void load_coag_params_manta(COAG_LEVELS coag_level)
{
  
  /* Update dac output voltage level */
  current_therapy_settings.setpoint.level = smart_wand_params.manta.coag[coag_level];
  current_therapy_settings.log.level = COAG_0; //For logging COAG_0 = COAG_1
  
  if ((current_therapy_settings.notify.status & 0xFF) == 0)
  {
    current_therapy_settings.audio.p_sound_file = 0;
  }
  
  current_therapy_settings.pulsing.energy_threshold = 400;
  current_therapy_settings.pulsing.pulse_window = 999;
  current_therapy_settings.pulsing.target_current = smart_wand_params.coag.ss_time;//18866;
  current_therapy_settings.pump.quiescent_delay = 0;
  change_flow_manta(coag_level, current_therapy_settings.treatment.flow_rate);
  current_therapy_settings.pump.quiescent_delay = smart_wand_params.manta.prime.pre.rf_delay;
  current_therapy_settings.pump.pulse_delay = smart_wand_params.manta.prime.pre.time;
  current_therapy_settings.pump.pulse_rps = smart_wand_params.manta.prime.pre.spd;
  current_therapy_settings.setpoint.ipeak_voltage = smart_wand_params.manta.ipeak.voltage;
  current_therapy_settings.setpoint.trip_load = smart_wand_params.manta.ipeak.trip_load;
  Update_Irms_Overcurrent_Thresholds();
}

/***************************************************************************//**
This function updates the current limit thresholds according to the mode. It
also updates the delay counter for pulsing.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void Update_Irms_Overcurrent_Thresholds(void)
{
  
  uint16_t counter_delay = 0;
  uint64_t med_threshold = 0;
  uint64_t iec_threshold = 0;
  
  /* joules -> counts = J x 1000 x (4096*65535)/(8*42.5*6.6) */
  uint64_t e_convert = (uint64_t) 119621818u;
  
  
  /* Calculate maximum allowable RMS current */
   
  /**************************************************************************
  * Calculate maximum irms threshold for current voltage set point (Vsp).
  * irms_threshold = 400w / Vsp, where Vsp = 3.3 / 4095 * ( 8 * 42.5 / 3.3)
  *************************************************************************/
  //irms_threshold = 40000000 / ((current_therapy_settings.setpoint.level * 83) / 1000);
  //med_threshold = (current_therapy_settings.pulsing.energy_threshold * 100000) / ((current_therapy_settings.setpoint.level * 83) / 1000);
  /**************************************************************************
  * Convert maximum irms threshold from amps to equivalent measured voltage.
  * Sensor has a gain of approx 1 v / 2.5 amps.
  *************************************************************************/
  //irms_threshold /= 20;
  //med_threshold /= 20;
  /**************************************************************************
  * Convert maximum irms threshold from volts to 16-bit adc counts 
  * irms threshold = Iamps * (3.3 / 65535)
  *************************************************************************/
  //irms_threshold *= 19859;
  //med_threshold *= 19859;
  /**************************************************************************
  * Scale maximum irms threshold by moving decimal point left 4 places. 
  *************************************************************************/
  //iec_threshold = irms_threshold / 10; 
  
  //med_threshold /= 10;
  iec_threshold = 400 * e_convert;
  med_threshold = current_therapy_settings.pulsing.energy_threshold * e_convert;
  
  
  /**************************************************************************
  * IEC requierment is 400J so threshold must be exceeded for 1 second before
  * violation. After 1 second 50 x 1,000 samples would have accumulated so,
  * to meet requirement threshold set by multiplying irms threshold by total
  * number of samples accumulated in 1 second.
  *************************************************************************/
  //iec_threshold = irms_threshold * 1000; 
  
  /**************************************************************************
  * Exceeding low threshold requires RF to remain OFF for up to sec which is
  * not acceptable for normal use so a more conservative, "medium" threshold
  * is set that cycles more rapidly to prevent instrument from ever reaching
  * iec threshold.
  *************************************************************************/
  //med_threshold = current_therapy_settings.pulsing.energy_threshold * 
  //                current_therapy_settings.pulsing.pulse_window;
                   
  /**************************************************************************
  * 
  *************************************************************************/
  counter_delay = (current_therapy_settings.pulsing.pulse_window >> 1) + 1;
  
  current_therapy_settings.pulsing.iec_threshold = iec_threshold;
  current_therapy_settings.pulsing.med_threshold = med_threshold;
  current_therapy_settings.pulsing.counter_delay = counter_delay;
  current_therapy_settings.pulsing.half_iec_threshold = (iec_threshold >> 1);
  
}

/***************************************************************************//**
This function checks to see if the screen displayed is appropriate to start
legacy wand therapy. The therapy can be started even if the displayed screen is
not the legacy wand treatment screen. But it cannot be started on some screens
like error and fault screen. This function scans for the correct screen.
@param screen_id - current ID of the screen
@return 1 if therapy can be started, 0 otherwise
@outputs
@context
*******************************************************************************/
uint8_t 
therapy_start_screen (uint8_t input_screen_id)
{
  if (input_screen_id == DELETE_INDIVIDUAL_USER_PROFILE_CHANCE_TO_BACK_OUT_SCREEN ||
      input_screen_id == SET_LEVEL_SCREEN ||
      input_screen_id == MAIN_SETTINGS_SCREEN ||
      input_screen_id == PROFILE_SAVE_SCREEN ||
      input_screen_id == USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN ||
      input_screen_id == USER_PROFILES_SCREEN || 
      input_screen_id == ENTER_NEW_USER_PROFILE_NAME_SCREEN ||
      input_screen_id == SWITCHING_SETTINGS_SCREEN ||
      input_screen_id == LANGUAGES_SCREEN ||
	  input_screen_id == WAND_INFO_SCREEN ||
	  input_screen_id == RF_2000_WAND_TREATMENT_SCREEN ||
	  input_screen_id == ENT_SMART_WAND_TREATMENT_SCREEN ||
      input_screen_id == LEGACY_6_8_WAND_TREATMENT_SCREEN ||
      input_screen_id == LEGACY_ENT_WAND_TREATMENT_SCREEN ||
        input_screen_id == MANTA_SMART_WAND_TREATMENT_SCREEN)
  {
    return 1;
  }
  else
  {
    return 0;
  }
      
}