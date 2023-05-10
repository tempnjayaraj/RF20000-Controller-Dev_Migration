/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              wdog.c
Author:            Chris Wallis
Date:              27-JUN-2013
Modified:
CPU:               Freescale Kinetis P/N MK20DN512ZVLQ10 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments:
Smart wand driver.

NOTE: N/A

*******************************************************************************/

#include "MK26F18.h"
#include "wdog.h"

void wdog_disable(void)
{
  
  /* Disable the watchdog timer w/ unlock sequence */
  WDOG_UNLOCK = 0xC520;
  WDOG_UNLOCK = 0xD928;

  /* Clear the WDOGEN bit to disable the watchdog */
  WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
  
}