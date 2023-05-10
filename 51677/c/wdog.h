/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1059                 $:
$Date:: 2014-04-23 16:40:42#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      wdog.h
\brief     Header file for watchdog module.
\details   NA
*******************************************************************************/

#ifndef WDOG_H
#define WDOG_H

#include <intrinsics.h>

void wdog_enable(void);
void wdog_disable(void);
uint8_t wdog_refresh(void);

#endif
