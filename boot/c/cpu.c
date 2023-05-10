/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              cpu.c
Author:            Chris Wallis
Date:              01-FEB-2013
Modified:          
CPU:               Freescale Kinetis P/N MK20DN512ZVLQ10 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments:
Generic high-level routines for ARM Cortex M4 Processor.

NOTE: N/A

*******************************************************************************/

#include "MK60F15.h"

/******************************************************************************
* Function: Write_Vtor
*
* Purpose: Write to Cortex System Control Block Vector Table Offset Register.
******************************************************************************/

void Write_Vtor (int vtor)
{
  
  /* Write the VTOR with the new value */
  SCB_VTOR = vtor;	
  
}


/******************************************************************************
* Function: Enable_IRQ
*
* Purpose: Initialize the NVIC to enable the specified IRQ.
*
* Input: irq number to be enabled (NOT vector number).
******************************************************************************/

void enable_irq (uint8_t irq)
{
  uint8_t div;
  
  /* Determine which of the NVICISERs corresponds to the irq */
  div = irq/32;
  
  switch (div)
  {
    case 0x0:
      NVICICPR0 = 1 << (irq%32);
      NVICISER0 = 1 << (irq%32);
      break;
    case 0x1:
      NVICICPR1 = 1 << (irq%32);
      NVICISER1 = 1 << (irq%32);
      break;
    case 0x2:
      NVICICPR2 = 1 << (irq%32);
      NVICISER2 = 1 << (irq%32);
      break;
    case 0x3:
      NVICICPR3 = 1 << (irq%32);
      NVICISER3 = 1 << (irq%32);
      break;
  }              
}

/******************************************************************************
* Function: Disable_IRQ
*
* Purpose: Initialize the NVIC to enable the specified IRQ.
*
* Input: irq number to be enabled (NOT vector number).
******************************************************************************/

void disable_irq (uint8_t irq)
{
  uint8_t div;
  
  /* Determine which of the NVICISERs corresponds to the irq */
  div = irq/32;
  
switch (div)
  {
    case 0x0:
      NVICICPR0 = 1 << (irq%32);
      NVICICER0 = 1 << (irq%32);
      break;
    case 0x1:
      NVICICPR1 = 1 << (irq%32);
      NVICICER1 = 1 << (irq%32);
      break;
    case 0x2:
      NVICICPR2 = 1 << (irq%32);
      NVICICER2 = 1 << (irq%32);
      break;
    case 0x3:
      NVICICPR3 = 1 << (irq%32);
      NVICICER3 = 1 << (irq%32);
      break;
  }               
}

uint8_t system_reset(void)
{
  
  /* request software reset */     
  SCB_AIRCR = SCB_AIRCR_VECTKEY(0x5FA)| SCB_AIRCR_SYSRESETREQ_MASK;
  
  return 0;
  
}

