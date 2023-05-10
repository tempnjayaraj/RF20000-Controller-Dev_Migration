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
\file      pump.h
\brief     Header file for pump driver
\details   NA
*******************************************************************************/

#ifndef PUMP_H
#define PUMP_H

#include "common.h"
#ifdef TEST
#include "artc_arm_math.h"
#else
#include "arm_math.h"
#endif
#include <stdlib.h>

#define PUMP_DSBL       GPIO_PIN(9)
#define MAX_NUM_CHARS   20


#pragma pack(1)
typedef struct {
  float32_t Kp;
  float32_t Ki;
  float32_t output;
  float32_t Ui;
  float32_t error;
  float32_t Up;
}PID;

#pragma pack(1)
typedef enum
{
  OFF,                  /* 0  */
  QUIESCENT,            /* 1  */
  STARTING,             /* 2  */
  ON,                   /* 3  */
  STOPPING,             /* 4  */
  VAC_START,            /* 5  */
  PULSING_DELAY,        /* 6  */
  PEAK_START,           /* 7  */
  PEAK_DELAY,           /* 8  */
  VAC_CTRL,             /* 9  */
  COAG_MODE,            /* 10  */
  CYCLING,              /* 11  */
  PRE_PRIME,            /* 12  */
  ON_ENT,               /* 13  */
  ENT_PAUSE_RF,         /* 14  */
  ON_ENT_8PIN,          /* 15  */
  NUM_SPEED_CTRL_STATES 
}SPEED_CTRL_STATES;     

#pragma pack(1)
typedef enum
{
  /* 0  */ PROTOCOL,
  /* 1  */ REQUEST_PWR_MODE,
  /* 2  */ CHANGE_BAUD,
  /* 3  */ CMD_MODE,
  /* 4  */ MOTOR_DISABLE,
  /* 5  */ MAX_CURRENT,
  /* 6  */ CONT_CURRENT,
  /* 7  */ AR_FILT_FREQ,
  /* 8  */ AR_FILT_GAIN,
  /* 9  */ PROBE_MOTOR,
  /* 10 */ DELAY_TIME,
  /* 11 */ IDLE_CURRENT,
  /* 12 */ HARMONIC_FILT_PHASE,
  /* 13 */ HARMONIC_FILT_GAIN,
  /* 14 */ JOG_ACCEL,
  /* 15 */ JOG_DECEL,
  /* 16 */ JOG_SPD,
  /* 17 */ DRIVE_STATUS,
  /* 18 */ MOTOR_ENABLE,  
  /* 19 */ COMMENCE_JOG,
  /* 20 */ IDLE_SPEED,  
  /* 21 */ ALARM_RESET,
  /* 22 */ STOP_KILL,
  /* 23 */ ZERO_SPEED,
  /* 24 */ VAC_PULSE_SPEED,
  /* 25 */ SOFT_RESET,
  /* 26 */ PWR_MODE_SCL,
  /* 27 */ SCL_MODE,
  /* 28 */ CONTROL,
  /* 29 */ WAIT_PROBE_TEST_COMPLETE,
}PUMP_STATE;

#pragma pack(1)
typedef struct{
  PUMP_STATE state;
  uint16_t inactive_count;
  uint8_t idx;
  uint8_t attempts;
  uint8_t status_data[MAX_NUM_CHARS];
  struct {
    uint8_t data[8];
    uint8_t size;
  }cmd[15];
}PUMP_CMD;

#pragma pack(1)
typedef enum
{
  
  BAUD_9600,
  BAUD_38400,
  MAX_BAUD_SETTINGS
    
}BAUD_RATE;

/* Function prototype */
float32_t PumpPID(q15_t pv, bool);
uint8_t pump_inactivity_monitor(void);
uint8_t pump_speed_ctrl(void);
void PumpFIR_Init(q15_t);
void PumpUART4_Init(BAUD_RATE);
uint8_t pump_inactivity_monitor(void);
uint8_t pump_speed_ctrl(void);
void send_pump_cmd(void);
uint8_t receive_pump_cmd(void);
uint8_t pump_packetizer(void);
uint8_t pump_reset(void);
uint8_t pump_cmd_ack(void);
uint8_t drive_status(void);
uint8_t pump_cmd_nack(void);
uint8_t drive_power_mode(void);
uint8_t probe_test_complete(void);
uint8_t over_current(void);
uint8_t over_energy(void);
uint8_t pump_uart_init(void);
uint8_t pump_cmd_eack(void);
uint8_t pump_startup_delay(void);
extern PUMP_CMD pump;
void pump_set_speed(float Speed);

#endif
