/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1053                 $:
$Date:: 2014-04-23 14:13:28#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     state_machine.h
\brief     Header file for state machine.
\details   N/A
*******************************************************************************/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

typedef enum
{
  POWERUP_STATE,
  STARTUP_STATE,
  IDLE_STATE,
  USER_INPUT_STATE,
  THERAPY_STATE,
  FAULT_STATE,
  SHUTDOWN_STATE,
  SW_UPDATE_STATE,
  LW_UI_STATE,
  LW_THERAPY_STATE,
  MAXIMUM_STATES
}state;

/** a pointer which points to a function which take
 in void and returns uin8_t */
typedef uint8_t (*tran_function)(void);

//#pragma pack(1)
typedef struct
{
  state next_state;
  tran_function entry;
  tran_function action;
  tran_function exit;
}state_object;


//void dispatch_events (state *,event*);
uint8_t do_nothing(void);
//uint8_t two_sec_delay(void);
uint8_t init_shutdown(void);
uint8_t state_switch(void);



#endif
