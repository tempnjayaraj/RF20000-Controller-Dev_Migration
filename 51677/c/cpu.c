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
\file      cpu.c
\brief     Driver for the CPU
\details   Generic high-level routines for ARM Cortex M4 Processor.
*******************************************************************************/

#include "MK26F18.h"

/***************************************************************************//**
Write to Cortex System Control Block Vector Table Offset Register.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void Write_Vtor (int vtor)
{
  
  /* Write the VTOR with the new value */
  SCB_VTOR = vtor;	
  
}


/***************************************************************************//**
Software reset of the system
@param 0
@return 0
@outputs
@context
*******************************************************************************/
uint8_t system_reset(void)
{
  
  /* request software reset */     
  SCB_AIRCR = SCB_AIRCR_VECTKEY(0x5FA)| SCB_AIRCR_SYSRESETREQ_MASK;
  
  return 0;
  
}

