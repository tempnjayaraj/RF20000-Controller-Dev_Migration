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
\file      switches.h
\brief     Header file for switches module
\details   NA
*******************************************************************************/

#ifndef SWITCHES_H
#define SWITCHES_H

//#include "system.h"
#include <stdint.h>

uint8_t change_mode(void);
uint8_t therapy_off(void);
uint8_t therapy_on(void);
uint8_t switch_ablate_parameters(void);
void Switch_Coag_Mode_Parameters(void);
void Update_Irms_Overcurrent_Thresholds(void);
uint8_t rf_on(void);
uint8_t rf_off(void);
uint8_t measure_z(void);
uint8_t legacy_wand_therapy_on (void);
uint8_t legacy_wand_therapy_off(void);
uint8_t lw_rf_on(void);
uint8_t lw_rf_off(void);
uint8_t therapy_start_screen (uint8_t);

#endif
