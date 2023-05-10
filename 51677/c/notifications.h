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
\file      notifications.h
\brief     Header file for notification manager
\details   NA
*******************************************************************************/
#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H  

#include "system.h"

#define WAND_SHORTED_NOTIFICATION_MASK                          0x100
#define TUBE_ENGAGEMENT_NOTIFICATION_MASK                       0x200 
#define SENSOR_NOTIFICATION_MASK                                0x400
#define AMBIENT_ALARM_NOTIFICATION_MASK                         0x800
#define MULTIPLE_BUTTONS_NOTIFICATION_MASK                      0x1000 
#define WAND_DISCONNECTED_NOTIFICATION_MASK                     0x2000 
#define WAND_USE_LIMIT_LIFE_EXPIRED_NOTIFICATION_MASK           0x4000 
#define AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK           0x8000 
#define INTERNAL_TEMPERATURE_NOTIFICATION_MASK                  0x10000 
#define WAND_ID_READ_NOTIFICATION_MASK                          0x20000  
#define WAND_REUSE_NOTIFICATION_MASK                            0x40000 
#define MEMORY_FAULT_MASK                                       0x80000 
#define WDOG_COP_FAULT_MASK                                     0x100000 
#define VMOD_FAULT_MASK                                         0x200000
#define ADC_BOUNDARY_FAULT_MASK                                 0x400000
#define SWITCH_STUCK_FAULT_MASK                                 0x800000
#define SYS_OVERTEMP_FAULT_MASK                                 0x1000000
#define SYS_COM_FAULT_MASK                                      0x2000000
#define PUMP_FAULT_MASK                                         0x4000000

//0x8000000
#define SERIAL_NUMBER_FAULT_MASK                                0x10000000
#define DATE_TIME_FAULT_MASK                                    0x20000000
//0x40000000
//0x80000000
#define CHECK_ELECTRODE_NOTIFICATION_MASK                       0x1
#define WAND_NOT_COMPATIBLE__NOTIFICATION_MASK                  0x2
#define GENERIC_ERROR_1_STATUS_2_MASK                           0x4
#define GENERIC_ERROR_2_STATUS_2_MASK                           0x8
#define GENERIC_ERROR_3_STATUS_2_MASK                           0x10

#define UI_FAULT_MASK                                           0x100

#define FAULT_MASK_1                                            0x3FF80000
#define FAULT_MASK_2                                            0x100

/*
1  “Wand is shorted.\n\nPlease replace wand.”
2  “Suction tube installation error.\n\nPlease check suction tube.”
5  “Multiple buttons or foot pedals have been pressed simultaneously.”
6  “Wand has been disconnected.\n\nPlease reconnect wand.”
7  “Wand has reached end of life.\n\nPlease replace wand.”
8  “Wand Ambient sensor not detected.\n\nPlease replace wand.”
9  “System over temperature.\n\nPlease check fan circulation at back of unit and wait while system cools.”
10 “Wand identification error.\n\nPlease replace wand.”
11 “Wand has passed its expiration date.\n\nPlease replace wand.”
*/
//#define WAND_SHORTED_GUI_TXT_PRIORITY_ID_MASK                   1
//#define TUBE_ENGAGEMENT_GUI_TXT_PRIORITY_ID_MASK                2
//#define MULTIPLE_BUTTON_GUI_TXT_PRIORITY_ID_MASK                5
//#define WAND_DISCONNECTED_GUI_TXT_PRIORITY_ID_MASK              6
//#define WAND_USE_LIMIT_LIFE_EXPIRED_GUI_TXT_PRIORITY_ID_MASK    7
//#define AMBIENT_SENSOR_NOT_DETECTED_GUI_TXT_PRIORITY_ID_MASK    8
//#define INTERNAL_TEMPERATURE_GUI_TXT_PRIORITY_ID_MASK           9
//#define WAND_ID_READ_GUI_TXT_PRIORITY_ID_MASK                   10
//#define WAND_REUSE_GUI_TXT_PRIORITY_ID_MASK                     11

typedef enum {
  /*  0  */ NO_TXT,
  /*  1  */ WAND_SHORT_ERROR_TXT,
  /*  2  */ TUBE_ENGAGEMENT_ERROR_TXT,
  /*  3  */ SUCTION_SENSOR_FAULT_TXT,
  /*  4  */ NOT_IMPLIMENTED_YET_1,
  /*  5  */ MULTI_BUTTON_ERROR_TXT,
  /*  6  */ WAND_DISCONNECT_ERROR_TXT,
  /*  7  */ WAND_LIFE_ERROR_TXT,
  /*  8  */ AMBIENT_ERROR_TXT,
  /*  9  */ SYS_OVERTEMP_ERROR_TXT,
  /*  10 */ WAND_ID_ERROR_TXT,
  /*  11 */ WAND_EXPIRED_ERROR_TXT,
  /*  12 */ MEM_FAULT_TXT,
  /*  13 */ WDOG_FAULT_TXT,
  /*  14 */ VMOD_FAULT_TXT,
  /*  15 */ ADC_BOUNDARY_FAULT_TXT,
  /*  16 */ SWITCH_STUCK_FAULT_TXT,
  /*  17 */ SYS_OVERTEMP_FAULT_TXT,
  /*  18 */ SYS_COM_FAULT_TXT,
  /*  19 */ PUMP_FAULT_TXT,
  /*  20 */ NOT_IMPLIMENTED_YET_2,
  /*  21 */ SERIAL_NUMBER_FAULT_TXT,
  /*  22 */ DATE_TIME_FAULT_TXT,
  /*  23 */ NOT_USED_1_TXT,
  /*  24 */ WAIT_FOR_RINSE_ERROR_TXT,
  /*  25 */ CHECK_ELECTRODE_ERROR_TXT,
  /*  26 */ WAND_NOT_COMPATIBLE_ERROR_TXT,
  /*  27 */ GENERIC_ERROR_1_TXT,
  /*  28 */ GENERIC_ERROR_2,
  /*  29 */ TWO_WANDS_CONNECTED_TXT,
  /*  30 */ GENERIC_ERROR_4,
  /*  31 */ GENERIC_FAULT_1,
  /*  32 */ GENERIC_FAULT_2,
  /*  33 */ UIS_SOFTWARE_FAULT_TXT,
  /*  33 */ MAX_NUM_NOTIFY_TXT,
}NOTIFY_TXT;
  
  

/*
3 “Suction sensor error.\n\nPlease power down the system and call for service.”
12 “Memory fault.\n\nPlease power down the system and call for service.”
13 “System hardware fault.\n\nPlease power down the system and call for service.”
14 “System hardware fault.\n\nPlease power down the system and call for service.”
15 “System hardware fault.\n\nPlease power down the system and call for service.”
16 “System hardware fault.\n\nPlease power down the system and call for service.”
17 “System hardware fault.\n\nPlease power down the system and call for service.”
18 “System communication fault.\n\nPlease power down the system and call for service.”
19 “System communication fault.\n\nPlease power down the system and call for service.”
21 “System logging fault.\n\nPlease power down the system and call for service.”
22 “System logging fault.\n\nPlease power down the system and call for service.”
*/


void notification_manager(void);
uint8_t gui_com_fault(void);
uint8_t peak_hit_cntr(void);
uint8_t legacy_wand_short_error(void);
uint8_t reset_error(void);
uint8_t suction_tube_error(void);
uint8_t no_ambient_error(void);
uint8_t clear_internal_temp_error(void);
uint8_t internal_temp_error(void);
uint8_t adc_boundary_fault(void);
uint8_t crc_fault(void);
uint8_t wand_switch_stuck_fault(void);
uint8_t fp_switch_stuck_fault(void);
uint8_t pump_fault(void);
uint8_t vmod_fault(void);
uint8_t can_timeout(void);
uint8_t internal_temp_fault(void);
uint8_t wdog_fault(void);
uint8_t fault_init(void);
uint8_t date_time_fault(void);
uint8_t serial_num_fault(void);
uint8_t wand_expired_error(void);
uint8_t wand_use_limit_error(void);
uint8_t clear_wand_expired_error(void);
uint8_t clear_wand_use_limit_error(void);
uint8_t wand_id_error(void);
uint8_t lw_id_error(void);
uint8_t wand_disconnected_error(void);
uint8_t check_electrode_error(void);
uint8_t wand_error_type_decipher(NOTIFY_TXT);
uint32_t clear_wand_errors(void);
uint8_t lw_incompatible_error(void);
uint8_t legacy_wand_disconnected_error(void);
uint8_t gen_com_fault(void);
uint8_t two_wand_disconnected_error(void);
uint32_t multiple_button_pressed_test(uint32_t);
uint8_t ui_fault(void);
uint8_t smart_wand_incompatible_error(void);




#endif
