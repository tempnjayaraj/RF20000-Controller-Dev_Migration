/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2798                 $:
$Date:: 2021-11-17 02:10:26#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     footpedal.h
\brief     Header file for foot pedal driver.
\details   N/A
*******************************************************************************/

#ifndef FOOTPEDAL_H
#define FOOTPEDAL_H

#include "common.h"

/** Masks for identifying different pedal presses */
#define ABLATE_MASK        GPIO_PIN(22)
#define ABLATE_SHIFT       22
#define COAG_MASK          GPIO_PIN(20)
#define COAG_SHIFT         20
#define PRESENT_MASK       GPIO_PIN(21)
#define PRESENT_SHIFT      21
#define MODE_MASK          GPIO_PIN(23)
#define MODE_SHIFT         23
#define FOOTPEDAL_MASK     (MODE_MASK | ABLATE_MASK | COAG_MASK)

/* Function prototypes */
uint8_t poll_footpedal(void);
uint8_t foot_pedal_init(void);
uint8_t check_fs_stuck(void);
bool fs_any_button_pressed(void);

#endif
