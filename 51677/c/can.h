/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************
$Author:: Harshil.Shah    $: Author
$Rev:: 2798               $: Revision of last commit
$Date:: 2021-11-17 02:10:#$: Date of last commit

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.30.4.3295
*******************************************************************************/

/***************************************************************************//**
@file      can.h
@brief     FlexCAN driver.
@details   Header file for the can driver.
*******************************************************************************/

#ifndef CAN_H
#define CAN_H

#include "common.h"
#include "MK26F18.h"
#include "system.h"

#define NUMBER_OF_CAN_MB               					16
#define NUMBER_OF_CAN_RX_MB           					5

/* Message 0x80 GEN Watchdog Message */
#define CAN_GEN_TXCOUNT_SHIFT     				        0
#define CAN_GEN_WDOG_ERROR_SHIFT				        31

/* Message 0x81 GEN Therapy Message */
#define CAN_GEN_CUT_LO_SP_SHIFT						0
#define CAN_GEN_CUT_MED_SP_SHIFT					2
#define CAN_GEN_CUT_HI_SP_SHIFT						4
#define CAN_GEN_COAG_SP_SHIFT						6
#define CAN_GEN_REFLEX_TIME_SHIFT                                       8
#define CAN_GEN_THERAPY_MODE_SHIFT					12
#define CAN_GEN_COAG_EN_SHIFT						15
#define CAN_GEN_COAG18_SP                                               16
#define CAN_GEN_CUT18_SP                                                20
//#define CAN_GEN_CUT_SP_DEFAULT_SHIFT          			20
//#define CAN_GEN_PUMP_ON_SHIFT                  			24
//#define CAN_GEN_DUAL_ELECT_EN_SHIFT            			25
#define CAN_GEN_WIRELESS_FOOTPEDAL_EN                                   24
#define CAN_GEN_FINGER_SWITCH_EN                                        25
#define CAN_GEN_CUT_ON_SHIFT                   				26
#define CAN_GEN_COAG_ON_SHIFT                  				27
#define CAN_GEN_TIMER_EN_SHIFT                 				28      
#define CAN_GEN_AMBIENT_EN_SHIFT               				29
#define CAN_GEN_AMBIENT_COLOR_SHIFT            				30
#define CAN_GEN_AMBIENT_TEMP_SHIFT					32 - 32   
#define CAN_GEN_AMBIENT_CAPABLE_SHIFT					39 - 32   
//#define CAN_GEN_PROFILE_CTRL_SHIFT					39 - 32  
//#define CAN_GEN_TIMER_VAL_SHIFT                			40 - 32
#define CAN_GEN_FLOW_RATE_SHIFT                                         40 - 32  
#define CAN_GEN_FLOW_RATE_AUTO_SHIFT                                    44 - 32 
#define CAN_GEN_FOOTPEDAL_CONNECTED_SHIFT      				45 - 32
#define CAN_GEN_WIRELESS_FOOTPEDAL_CONNECTED_SHIFT                      46 - 32
//#define CAN_GEN_WAND_TYPE_SHIFT                			44 - 32
//#define CAN_GEN_WAND_CONNECTED_SHIFT    				45 - 32
#define CAN_GEN_ENT_TURBO_MODE                                          47 - 32
#define CAN_GEN_AMIBENT_NOTIFICATION_THRESHOLD_SHIFT			48 - 32
#define CAN_GEN_COAG_SP_EXTENDED_SHIFT                                  54 - 32
#define CAN_GEN_PRE_PRIME_SHIFT                                         56 - 32
#define CAN_GEN_GLOW_PRIME_SHIFT                                        57 - 32


/* Message 0x82 GEN Graphical Managment Message */
#define CAN_GEN_SCREEN_ID_SHIFT						0
#define CAN_GEN_LANGUAGE_ID_SHIFT					8
#define CAN_GEN_SAVE_EN_SHIFT                  				16
#define CAN_GEN_NEW_EN_SHIFT                                            17        
#define CAN_GEN_WIRELESS_HANDSHAKE_PHASE_SHIFT                          18
#define CAN_GEN_PROFILE_ID_SHIFT                                        24
#define CAN_GEN_WAND_INDICATION_SHIFT                                   32 - 32

/* Message 0x83 GEN Software Status Message */
#define CAN_GEN_FIRMWARE_VERSION_MAJOR_SHIFT				0
#define CAN_GEN_FIRMWARE_VERSION_MINOR_SHIFT    			8
#define CAN_GEN_FW_UPDATE_CARD_LOADED_SHIFT				16
#define CAN_GEN_FW_UPDATE_UIS_EN_SHIFT					17
#define CAN_GEN_DEBUG_MODE_FLAG_SHIFT		        		18
#define CAN_GEN_SHUTDOWN_SHIFT                                          19
#define CAN_GEN_FW_UPDATE_GS_PROGRESS_SHIFT                             20

/* Message 0x84 GEN Notification Status Message */
#define CAN_GEN_NOTIFICATION_PRIORITY_SHIFT				0
#define CAN_GEN_PERSISTENT_WAND_OVERCURRENT_NOTIFICATION_SHIFT  	8
#define CAN_GEN_SUCTION_TUBE_ENGAGEMENT_NOTIFICATION_SHIFT		9
#define CAN_GEN_PATIENT_AMBIENT_TEMPERATURE_NOTIFICATION_SHIFT	        10
#define CAN_GEN_MULTIPLE_BUTTON_PRESSED_NOTIFICATION_SHIFT		11
#define CAN_GEN_WAND_DISCONNECTED_NOTIFICATION_SHIFT			12
#define CAN_GEN_WAND_USE_LIMIT_LIFE_EXPIRED_NOTIFICATION_SHIFT	        13
#define CAN_GEN_WAND_SHORTED_NOTIFICATION_SHIFT				14
#define CAN_GEN_AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_SHIFT	        15
#define CAN_GEN_INTERNAL_TEMPERATURE_NOTIFICATION_SHIFT			16
#define CAN_WAND_ID_READ_ERROR_NOTIFICATION_SHIFT			17
#define CAN_WAND_FUSE_REUSE_ERROR_NOTIFICATION_SHIFT			18

/* Message 0x85 GEN Notification Setting Message */
#define CAN_GEN_AMBIENT_NOTIFICATION_VOL_SHIFT				0
#define CAN_GEN_AMBIENT_NOTIFICATION_EN_SHIFT				6
#define CAN_GEN_GENERAL_NOTIFICATION_VOL_SHIFT				8
#define CAN_GEN_REFLEX_TIMER_NOTIFICATION_EN_SHIFT                      14

/* Message 0x86 GEN Wand Trade Name Message 1 */
#define CAN_GEN_TRADE_NAME_CHAR_0_SHIFT					0
#define CAN_GEN_TRADE_NAME_CHAR_1_SHIFT					8
#define CAN_GEN_TRADE_NAME_CHAR_2_SHIFT					16
#define CAN_GEN_TRADE_NAME_CHAR_3_SHIFT					24
#define CAN_GEN_TRADE_NAME_CHAR_4_SHIFT					32 - 32
#define CAN_GEN_TRADE_NAME_CHAR_5_SHIFT					40 - 32
#define CAN_GEN_TRADE_NAME_CHAR_6_SHIFT					48 - 32
#define CAN_GEN_TRADE_NAME_CHAR_7_SHIFT					56 - 32

/* Message 0x87 GEN Wand Trade Name Message 2 */
#define CAN_GEN_TRADE_NAME_CHAR_8_SHIFT					0
#define CAN_GEN_TRADE_NAME_CHAR_9_SHIFT 				8
#define CAN_GEN_TRADE_NAME_CHAR_10_SHIFT				16
#define CAN_GEN_TRADE_NAME_CHAR_11_SHIFT				24
//#define CAN_GEN_TRADE_NAME_CHAR_12_SHIFT				32 - 32
//#define CAN_GEN_TRADE_NAME_CHAR_13_SHIFT				40 - 32
//#define CAN_GEN_TRADE_NAME_CHAR_14_SHIFT			        48 - 32
//#define CAN_GEN_TRADE_NAME_CHAR_15_SHIFT				56 - 32
#define CAN_GEN_WAND_LO_CAPABLE                                         32 - 32
#define CAN_GEN_WAND_MED_CAPABLE                                        33 - 32
#define CAN_GEN_WAND_HI_CAPABLE                                         34 - 32
#define CAN_GEN_WAND_VAC_CAPABLE                                        35 - 32

/* Message 0x88 GEN Wand Trade Name Message 1 */
#define CAN_GEN_CATALOG_NUM_CHAR_0_SHIFT                                0
#define CAN_GEN_CATALOG_NUM_CHAR_1_SHIFT                                8
#define CAN_GEN_CATALOG_NUM_CHAR_2_SHIFT                                16
#define CAN_GEN_CATALOG_NUM_CHAR_3_SHIFT                                24
#define CAN_GEN_CATALOG_NUM_CHAR_4_SHIFT                                32 - 32
#define CAN_GEN_CATALOG_NUM_CHAR_5_SHIFT                                40 - 32
#define CAN_GEN_CATALOG_NUM_CHAR_6_SHIFT                                48 - 32
#define CAN_GEN_CATALOG_NUM_CHAR_7_SHIFT                                56 - 32

/* Message 0x89 GEN Wand Trade Name Message 1 */
#define CAN_GEN_CATALOG_NUM_CHAR_8_SHIFT                                0
#define CAN_GEN_CATALOG_NUM_CHAR_9_SHIFT                                8
#define CAN_GEN_CATALOG_NUM_CHAR_10_SHIFT                               16
#define CAN_GEN_CATALOG_NUM_CHAR_11_SHIFT                               24
#define CAN_GEN_WO_NUM_0_SHIFT                                          32 - 32
#define CAN_GEN_WO_NUM_1_SHIFT                                          40 - 32
#define CAN_GEN_WO_NUM_2_SHIFT                                          48 - 32
#define CAN_GEN_WO_NUM_3_SHIFT                                          56 - 32

/* Message 0x8A GEN Wand Active Life Message */
#define CAN_GEN_WAND_ACTIVE_LIFE_LO_SHIFT                               0
#define CAN_GEN_WAND_ACTIVE_LIFE_MED_SHIFT                              16
#define CAN_GEN_WAND_ACTIVE_LIFE_HI_SHIFT                               32 - 32
#define CAN_GEN_WAND_ACTIVE_LIFE_VAC_SHIFT                              48 - 32

/* Message 0x8B GEN Serial Number Message */
#define CAN_GEN_SERIAL_NUM_CHAR_0_SHIFT                                 0
#define CAN_GEN_SERIAL_NUM_CHAR_1_SHIFT                                 6
#define CAN_GEN_SERIAL_NUM_CHAR_2_SHIFT                                 12
#define CAN_GEN_SERIAL_NUM_CHAR_3_SHIFT                                 18
#define CAN_GEN_SERIAL_NUM_CHAR_4_SHIFT                                 24
#define CAN_GEN_SERIAL_NUM_CHAR_5A_SHIFT                                30
#define CAN_GEN_SERIAL_NUM_CHAR_5B_SHIFT                                32-32
#define CAN_GEN_SERIAL_NUM_CHAR_6_SHIFT                                 36-32
#define CAN_GEN_SERIAL_NUM_CHAR_7_SHIFT                                 42-32
#define CAN_GEN_SERIAL_NUM_CHAR_8_SHIFT                                 48-32
#define CAN_GEN_SERIAL_NUM_CHAR_9_SHIFT                                 54-32

/* Message 0x8A GEN Wand Active COAG Life Message */
#define CAN_GEN_WAND_ACTIVE_LIFE_COAG_SHIFT                             0

/* Message 0x8F GEN Debug Message */
#define CAN_GEN_DEBUG_MSG_SHIFT						0

/* Message 0x90 GEN Watchdog Message */
#define CAN_UI_TXCOUNT_SHIFT     				        0
#define CAN_UI_TXFLAG_SHIFT					        31

/* Message 0x91 UI Alarm Reset Message */
#define CAN_UI_FW_UPDATE_STATUS_SHIFT                   		0
#define CAN_UI_CAN_SW_ERROR_SHIFT                                       8
#define CAN_UI_CAN_TRANSPORT_SW_ERROR_SHIFT                             9
#define CAN_UI_TOUCH_SW_ERROR_SHIFT                                     10
#define CAN_UI_CAN_SW_FAULT_SHIFT                                       16
#define CAN_UI_CAN_TRANSPORT_SW_FAULT_SHIFT                             17
#define CAN_UI_TOUCH_SW_FAULT_SHIFT                                     18
#define CAN_UI_RENDER_SW_FAULT_SHIFT                                    19

/* Message 0x92 UI Status Message */
#define CAN_UI_GRAPHIC_NUMBER_SHIFT            				0
#define CAN_UI_GRAPHIC_VALUE_SHIFT     					8

/* Message 0x93 UI Software Status Message */
#define CAN_UI_KERNEL_VERSION_MAJOR_SHIFT				0
#define CAN_UI_KERNEL_VERSION_MINOR_SHIFT				8
#define CAN_UI_BSP_VERSION_MAJOR_SHIFT					16
#define CAN_UI_BSP_VERSION_MINOR_SHIFT					24
#define CAN_UI_APPLICATION_VERSION_MAJOR_SHIFT				32 - 32
#define CAN_UI_APPLICATION_VERSION_MINOR_SHIFT				40 - 32
#define CAN_UI_GRAPHIC_VERSION_MAJOR_SHIFT				48 - 32
#define CAN_UI_GRAPHICS_VERSION_MINOR_SHIFT				56 - 32

/* Message 0x9F GEN Debug Message */
#define CAN_UI_DEBUG_MSG_ECHO_L_SHIFT					0
#define CAN_UI_DEBUG_MSG_ECHO_H_SHIFT					0

#define MB_CODE_TX_INACTIVE                                             0x8
#define MB_CODE_TX_ABORT                                                0x9
#define MB_CODE_TX_DATA                                                 0xC

#define PWORD_CLEAR(x, y)                      memset((x),0,sizeof(uint32_t)); \
                                               memset((y),0,sizeof(uint32_t));

#define PWORD_GET(x,y  )                       (*(x)) =  mb_buffer.data[0].word0; \
                                               swap_4bytes((uint8_t *)(x)); \
                                               (*(y)) =  mb_buffer.data[0].word1; \
                                               swap_4bytes((uint8_t *)(y));
                                               
#define GEN_WDOG_MSG_MB                                                 8
#define GEN_THERAPY_MSG_MB                                              9
#define GEN_GRAPHIC_MSG_MB                                              10
#define GEN_SS_MSG_MB                                                   11        
#define GEN_ALARM_MB                                                    12
#define GEN_NOTIFICATION_MSG_MB                                         13
#define GEN_WAND_NAME_1_MSG_MB                                          14                                
#define GEN_WAND_NAME_2_MSG_MB                                          15
#define GEN_ACTIVE_LIFE_MSG_MB                                          8                                               
#define GEN_SERIAL_NUM_MSG_MB                                           9
#define GEN_CATALOG_1_MSG_MB                                            10
#define GEN_CATALOG_2_MSG_MB                                            11                                              
//#define GEN_DEBUG_MSG_MB                                                7                                               
                      
typedef struct{
  struct {
  uint32_t cs;
  uint32_t id;
  uint32_t word0;
  uint32_t word1;
  }data[10];
  uint8_t idx;
} CAN_MB;
                                               
/* Function Prototypes */
void flexCAN_readMB(void);
uint8_t read_flexcan(void);
uint8_t flexCAN_write_fast_msgs(void);
void FlexCAN_Write0x82Msg(void);
uint8_t flexcan_init(void);
uint8_t can_wdog_rx(void);
uint8_t flexCAN_write_slow_msgs(void);


#endif
