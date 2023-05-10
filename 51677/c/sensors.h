/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1051                 $:
$Date:: 2014-04-23 11:50:46#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     sensors.h
\brief     Header file for sensor driver.
\details   N/A
*******************************************************************************/

#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"

#define DOOR_SENSOR_MASK  GPIO_PIN(5)
#define DOOR_SENSOR_SHIFT 5
#define SUCTION_TUBE_SENSOR_MASK  GPIO_PIN(5)
#define SUCTION_TUBE_SENSOR_SHIFT 5
#define DISCHARGE_TUBE_SENSOR_MASK  GPIO_PIN(4)
#define DISCHARGE_TUBE_SENSOR_SHIFT 4

#define DOOR_SENSOR_ON                  2
#define DISCHARGE_TUBE_SENSOR_ON        4
#define SUCTION_TUBE_SENSOR_ON          8

/* Function prototypes */
uint8_t door_sensor_init(void);
uint8_t poll_pump_sensors(void);
uint8_t tube_sensors_init(void);


#endif
