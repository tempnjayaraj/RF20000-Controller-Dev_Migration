/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: cwalli00          $:
$Rev:: 2794                 $:
$Date:: 2021-11-04 10:49:01#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     rcm.c
\brief     Reset Control Module (rcm) driver
\details   N/A
*******************************************************************************/
#include "MK26F18.h"
#include "event_buffer.h"
#include "system.h"
#include "rcm.h"

/****************************************************************************//**
Performs a soft reset of the system
@param void
@return void
@outputs
@context
********************************************************************************/
void software_reset()
{

  uint32_t temp_AIRCR=SCB_AIRCR;
  
  /* clear top 16-bits */
  temp_AIRCR &= 0x0000FFFF;
  
  /* special vectkey that allows writes */
  temp_AIRCR |= SCB_AIRCR_VECTKEY(0x5FA); 
   
  /* force soft reset request */
  temp_AIRCR |=SCB_AIRCR_SYSRESETREQ_MASK;
  
  /* reboot using sys ctrl block */
  SCB_AIRCR = temp_AIRCR;

}

/****************************************************************************//**
Monitors the system for watch dog reset
@param void
@return invalid_srs returns 1 if a valid reset condition is present
@outputs
@context
********************************************************************************/
uint8_t reset_monitor(void)
{

  uint8_t invalid_srs = 0;
  
/* if (RCM_SRS1 & RCM_SRS1_TAMPER_MASK)
     //printf("Tamper Detect Reset\n");*/

//  if (RCM_SRS1 & RCM_SRS1_SACKERR_MASK)
//  //printf("Stop Mode Acknowledge Error Reset\n");
//  if (RCM_SRS1 & RCM_SRS1_EZPT_MASK)
//  //printf("EzPort Reset\n");
//  if (RCM_SRS1 & RCM_SRS1_MDM_AP_MASK)
//  //printf("MDM-AP Reset\n");
//  if (RCM_SRS1 & RCM_SRS1_SW_MASK)
//  //printf("Software Reset\n");
//  if (RCM_SRS1 & RCM_SRS1_LOCKUP_MASK)
//  //printf("Core Lockup Event Reset\n");
//  if (RCM_SRS1 & RCM_SRS1_JTAG_MASK)
//  //printf("JTAG Reset\n");
//  if (RCM_SRS0 & RCM_SRS0_POR_MASK)
//  //printf("Power-on Reset\n");
//  if (RCM_SRS0 & RCM_SRS0_PIN_MASK)
//  //printf("External Pin Reset\n");
  
  if ((RCM_SRS0 & RCM_SRS0_WDOG_MASK) == RCM_SRS0_WDOG_MASK)
  {
  
    //printf("Watchdog(COP) Reset\n");
    invalid_srs = 1;
    put_event(&evt_tbl,WDOG_COP_RESET);
    
  }
  
//  if (RCM_SRS0 & RCM_SRS0_LOC_MASK)
//  //printf("Loss of Clock Reset\n");
//  if (RCM_SRS0 & RCM_SRS0_LVD_MASK)
  //printf("Low-voltage Detect Reset\n");
//  if (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK)
//  {
//    printf("[outSRS]Wakeup bit set from low power mode exit\n");
//    printf("[outSRS]SMC_PMPROT   = %#02X \r\n", (SMC_PMPROT))  ;
//    printf("[outSRS]SMC_PMCTRL   = %#02X \r\n", (SMC_PMCTRL))  ;
//    printf("[outSRS]SMC_VLLSCTRL   = %#02X \r\n", (SMC_VLLSCTRL))  ;
//    printf("[outSRS]SMC_PMSTAT   = %#02X \r\n", (SMC_PMSTAT))  ;
//
//    if ((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 3)
//      printf("[outSRS] LLS exit \n") ;
//    if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 1))
//      printf("[outSRS] VLLS1 exit \n") ;
//    if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 2))
//      printf("[outSRS] VLLS2 exit \n") ;
//    if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 3))
//      printf("[outSRS] VLLS3 exit \n") ; 
//  }

//  if ((RCM_SRS0 == 0) && (RCM_SRS1 == 0)) 
//  {
//    printf("[outSRS]RCM_SRS0 is ZERO   = %#02X \r\n", (RCM_SRS0))  ;
//    printf("[outSRS]RCM_SRS1 is ZERO   = %#02X \r\n", (RCM_SRS1))  ;	 
//  }

  return invalid_srs;
  
}
