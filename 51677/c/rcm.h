/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1048                 $:
$Date:: 2014-04-23 11:33:25#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      rcm.h
\brief     Header file reset module
\details   NA
*******************************************************************************/

#ifndef RCM_H
#define RCM_H

#include <stdint.h>

uint8_t reset_monitor(void);  
void software_reset();

#endif
