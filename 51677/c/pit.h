/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1043                 $:
$Date:: 2014-04-22 15:30:27#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     pit.h
\brief     Header file for Programmable interrupt timer(s).
\details   N/A
*******************************************************************************/

#ifndef PIT_H
#define PIT_H

#include "common.h"

/* PIT reset and start utility */
#define RESET_PIT(x)       PIT_TFLG_REG(PIT_BASE_PTR,(x)) |= PIT_TFLG_TIF_MASK; \
                           PIT_TCTRL_REG(PIT_BASE_PTR,(x)) &= ~PIT_TCTRL_TEN_MASK; \
                           PIT_TCTRL_REG(PIT_BASE_PTR,(x)) |= PIT_TCTRL_TEN_MASK; 

#define DISABLE_PIT(x)     PIT_TCTRL_REG(PIT_BASE_PTR,(x)) &= ~PIT_TCTRL_TEN_MASK; \
                           PIT_TFLG_REG(PIT_BASE_PTR,(x)) |= PIT_TFLG_TIF_MASK;

/* Function prototype */
void pit_init(void);
//void PIT3_Init(void);
//void PIT0_Init(void);
//void PIT1_Init(void);
void PIT_Config(uint8_t, uint32_t);

#endif
