/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: cwalli00          $:
$Rev:: 1457                 $:
$Date:: 2014-09-30 10:57:24#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/

/***************************************************************************//**
\file:     init.h
\brief     Header file for init.c
*******************************************************************************/

#ifndef INIT_H
#define INIT_H

#include "system.h"

void Init(void);
void Post(void);
//void Statup_Delay_Timer(void);

uint8_t sys_init(void);
uint8_t init_manager(void);
SUPPORTED_LANGUAGES load_default_language(uint8_t);

#endif
