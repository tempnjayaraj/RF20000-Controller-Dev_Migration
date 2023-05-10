/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1046                 $:
$Date:: 2014-04-23 10:10:13#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     psu.h
\brief     Header file for power supply unit
\details   N/A
*******************************************************************************/

#ifndef PSU_H
#define PSU_H

#include "common.h"

#define PWR_BUTTON_MASK     1
#define PWR_BUTTON_ON_MASK  GPIO_PIN(7)
#define PWR_BUTTON_ON_SHIFT 7
#define RF_PWR_ON_MASK      GPIO_PIN(28)
#define MOTOR_PWR_ON_MASK   GPIO_PIN(27)
#define TWELVE_VOLT_ON_MASK GPIO_PIN(29)
#define GLB_PWR_EN_MASK     GPIO_PIN(3)
#define GLB_PWR_EN_SHIFT     3
#define GUI_RESET_MASK      GPIO_PIN(15)

/* Function prototypes */
void psu_init(void);
uint8_t power_board_on(void);
uint8_t poll_pwr_button(void);
uint8_t high_volt_off(void);
uint8_t power_button_status(void);
uint8_t psu_off(void);

#endif
