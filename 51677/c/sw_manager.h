/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: cwalli00          $:
$Rev:: 1989                 $:
$Date:: 2017-02-01 17:04:11#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     software_manager.h
\brief     Header file for software manager module.
\details   N/A
*******************************************************************************/

#ifndef SW_MANAGER_H
#define SW_MANAGER_H

#include <stdint.h>
#include "head.h"

typedef enum {
  UPDATE_NOT_STARTED,
  UPDATE_IN_PROGRESS,
  UPDATE_COMPLETED,
  UPDATE_ABORTED,
}gui_update_status;

uint8_t commence_uis_sw_update(void);
uint8_t commence_gs_sw_update(void);
uint8_t ui_update_status(void);
uint8_t check4gsfw(uint8_t, uint8_t);
uint8_t check4uifw(uint8_t, uint8_t);
uint8_t cancel_sw_update(void);
uint8_t ui_gs_firmware_update_pause(void);
extern program_header *pheader;

#endif
