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
\file:     state_machine.c
\brief     State machine implemtation.
\details   N/A
*******************************************************************************/
#include "state_machine.h"
#include "pit.h"
#include "MK26F18.h"
#include "system.h"
#include "ftfe.h"
#include "psu.h"
#include "audio.h"
#include "esdhc.h"

//void
//dispatch_events (state *s, event* e)
//{
//  uint8_t transition_flag = 0;
//  state_object state_exec = state_matrix[*s][*e];
//  (*state_exec.action)();
//  
//  if (transition_flag)
//  {
//    //Switch States
//    // Can add entry and exit function if we need
//    //(*current_state.exit)();
//    *s = state_exec.next_state;
//    // Can add entry and exit function if we need
//    //(*current_state.entry)();
//   
//  }
//}

/****************************************************************************//**
This function does nothing. It is just a stub for undefined state transitions.
@param void
@return 0
@outputs
@context
********************************************************************************/
uint8_t do_nothing (void)
{
  
  return 0;

}

/****************************************************************************//**
This function initiates the shutdown process. It turns off the high voltage
section of the board.
@param void
@return 0
@outputs
@context
********************************************************************************/
uint8_t init_shutdown(void)
{
  

  /* hardware shutdown anyway */
  high_volt_off();
  RTC_TAR = 0xFFFFFFFF;
  current_therapy_settings.notify.status = 0;
  current_therapy_settings.notify.status2 = 0;
  //current_therapy_settings.ambient.audio_enabled = 0;
  DISABLE_AUDIO;
  current_therapy_settings.footpedal.coag = 0;
  current_therapy_settings.footpedal.ablate = 0;
  current_therapy_settings.footpedal.mode = 0;
  current_therapy_settings.footpedal.mode_key = 0;
  current_therapy_settings.sdhc = empty_sd_meta;
  current_therapy_settings.gui.sw.update_status = 0;
  
  return 0;
  
}

/***************************************************************************//**
This function is used to switch between states.
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t state_switch(void)
{
  put_event(&evt_tbl,LW_PLUGGED_IN);
  return 1;
}






//uint8_t two_sec_delay(void)
//{
//  
//  static uint8_t timer_started = 0;
//  
//  uint8_t timer_expired = 0;
//  
///******************************************************************************
//* 2 sec delay while board is energized. 
//******************************************************************************/
//  
//  if (timer_started == 0)
//  {
//    
//    DISABLE_PIT(0);
//    
//    //PIT_LDVAL0 =  148000000;
//    
//    RESET_PIT(0);
//    
//    //GPIOA_PTOR |= YELLOW_LED;
//    
//    //put_event(&evt_tbl,INIT_MANAGER);
//    
//    timer_started = 1;
//        
//  }
//  else if (PIT_CVAL0 > 148000000)
//  {
//      
//    DISABLE_PIT(0);  
//    
//    /* reset pump timeout back to 4 seconds */
//    //PIT_LDVAL0 = 296000000;
//    
//    /* turn yellow LED OFF */
//    //GPIOA_PTOR |= YELLOW_LED;
//    
//    timer_expired = 1;
//          
//  }
//
//    
//  put_event(&evt_tbl,TWO_SEC_DELAY);
//
//  
//  return timer_expired;
//  
//}
