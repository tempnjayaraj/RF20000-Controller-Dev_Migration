/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: cwalli00          $:
$Rev:: 1994                 $:
$Date:: 2017-02-02 12:58:07#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      rtc.h
\brief     Header file real time clock module
\details   NA
*******************************************************************************/

#ifndef RTC_H
#define RTC_H

#include <stdint.h>


uint32_t rtc_init(void); 
void rtc_check_errors(void);

#endif
