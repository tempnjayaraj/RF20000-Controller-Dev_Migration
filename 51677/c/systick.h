/*******************************************************************************
                   ArthroCare Proprietary and Confidential
                   (c) ArthroCare Corporation 2012
                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2798                 $:
$Date:: 2021-11-17 02:10:26 -0600#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/******************************************************************************
\file:     systick.h
\brief     Header file for system tick timer driver.
\details   N/A
*******************************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H

/* Includes -------------------------------------------------------- */
#include "cpu.h"

/* Exported Macros & Defines ---------------------------------------  */

/* Exported TypeDef ------------------------------------------------ */
/* <callback timer function pointer  */
typedef void(*cbtmr_func_t)(void); //not function must return and receive nothing

typedef enum _cbtmr_mode_t
{
  SINGLE_CBTMR = 0,     /**< Single callback mode */
  CONTINUOUS_CBTMR      /**< Continuous callback mode */
} cbtmr_mode_t;

/* Exported Variables & Constants ---------------------------------- */

/* Exported Functions ---------------------------------------------- */
u8 systick_init(void);
u8 cbtmr_isr(void);
extern void cbtmr_init(void);
ErrorStatus cbtmr_reg_callback(cbtmr_func_t cbtmr_func,
                                      FlagStatus *cbtmr_flag,
                                      u32 cbtmr_time_ms,
                                      cbtmr_mode_t cbtmr_mode,
                                      u8 *cbtmr_id);
ErrorStatus cbtmr_unreg_callback(u8 UnregID);
extern void cbtmr_reset_timer(u8 ResetTimerID);

#endif
