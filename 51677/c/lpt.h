/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1039                 $:
$Date:: 2014-04-22 14:09:38#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/

/***************************************************************************//**
\file:     lpt.h
\brief     Header file for lpt (low power timer) driver.
\details   N/A
*******************************************************************************/

#ifndef LPT_H
#define LPT_H

uint8_t lpt_init(void);
uint8_t reset_pulse_cnt(void);

#endif
