/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              can.h
Author:            Chris Wallis
Date:              10-SEPT-2012
Modified:          
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.30.4.3295
*******************************************************************************/

/*******************************************************************************
Comments: N/A

NOTE: N/A

*******************************************************************************/

#ifdef DEBUG_CAN

#ifndef CAN_DEBUG_H
#define CAN_DEBUG_H

#include "common.h"
#include "MK26F18.h"
#ifdef TEST
#include "artc_arm_math.h"
#else
#include "arm_math.h"
#endif

/* Message 0x10 GEN Debug 1 Message */
#define CAN_GEN_TX_PV_SHIFT                                             0
#define CAN_GEN_TX_FILTPV_SHIFT                                         16

/* Message 0x11 GEN Debug 2 Message */
#define CAN_GEN_TX_ERROR_SHIFT                                          0
#define CAN_GEN_TX_PID_OUTPUT_SHIFT                                     16
#define CAN_GEN_TX_RPS_CHAR_0_SHIFT                                     0
#define CAN_GEN_TX_RPS_CHAR_1_SHIFT                                     8
#define CAN_GEN_TX_RPS_CHAR_2_SHIFT                                     16
#define CAN_GEN_TX_RPS_CHAR_3_SHIFT                                     24
#define CAN_GEN_TX_PWM_C0V_SHIFT                                        0
#define CAN_GEN_TX_PWM_C1V_SHIFT                                        16
#define CAN_GEN_TX_PWM_C2V_SHIFT                                        0
#define CAN_GEN_TX_PWM_C3V_SHIFT                                        16
#define CAN_GEN_TX_PWM_C4V_SHIFT                                        0
#define CAN_GEN_TX_PWM_C5V_SHIFT                                        16
#define CAN_GEN_TX_PWM_C6V_SHIFT                                        0
#define CAN_GEN_TX_PWM_C7V_SHIFT                                        16
#define CAN_GEN_TX_IRMS_SHIFT                                           0
#define CAN_GEN_TX_HANDLE_SHIFT                                         0
#define CAN_GEN_TX_AMBIENT_SHIFT                                        8
#define MB_CODE_TX_INACTIVE                                             0x8
#define MB_CODE_TX_ABORT                                                0x9
#define MB_CODE_TX_DATA                                                 0xC

#define PWORD_CLEAR(x, y)                      memset((x),0,sizeof(uint32_t)); \
                                               memset((y),0,sizeof(uint32_t));

                                               
#define GEN_DEBUG_1_MSG_MB                                              11
#define GEN_DEBUG_2_MSG_MB                                              12
#define GEN_DEBUG_3_MSG_MB                                              13
#define GEN_DEBUG_4_MSG_MB                                              14        
#define GEN_DEBUG_5_MSG_MB                                              15
#define GEN_DEBUG_6_MSG_MB                                              8 
#define GEN_DEBUG_7_MSG_MB                                              9
#define GEN_DEBUG_8_MSG_MB                                              15
#define GEN_DEBUG_9_MSG_MB                                              9
#define GEN_DEBUG_10_MSG_MB                                             10
#define GEN_DEBUG_11_MSG_MB                                             14
                                               
#define GEN_DEBUG_1_MSG_ARB_ID                                          0x10
#define GEN_DEBUG_2_MSG_ARB_ID                                          0x11
#define GEN_DEBUG_3_MSG_ARB_ID                                          0x12
#define GEN_DEBUG_4_MSG_ARB_ID                                          0x13
#define GEN_DEBUG_5_MSG_ARB_ID                                          0x14
#define GEN_DEBUG_6_MSG_ARB_ID                                          0x15
#define GEN_DEBUG_7_MSG_ARB_ID                                          0x16
#define GEN_DEBUG_8_MSG_ARB_ID                                          0x17
#define GEN_DEBUG_9_MSG_ARB_ID                                          0x18
#define GEN_DEBUG_10_MSG_ARB_ID                                         0x19
#define GEN_DEBUG_11_MSG_ARB_ID                                         0x1A
#define NUM_WAND_DATABASE_PAGES               (sizeof(SMART_WAND_DATABASE)-8)

                                         
/* Function Prototypes */
uint8_t flexCAN_write_pumpdebug(void);
void FlexCAN_Write_PWMDebug(void);
uint8_t flexCAN_write_wanddebug(void);
uint8_t flexCAN_write_wireless(void);
uint8_t flexCAN_write_wand_wear_debug(void);

#pragma pack(1)
typedef struct {
  struct{
    q15_t pv;
    float32_t filt_pv;
    float32_t error;
    float32_t pid_output;
    uint8_t rps[4];
  }pump;
  struct{
    uint16_t c0v;
    uint16_t c1v;
    uint16_t c2v;
    uint16_t c3v;
    uint16_t c4v;
    uint16_t c5v;
    uint16_t c6v;
    uint16_t c7v;
  }pwm;
  struct{
    uint8_t ambient;
    uint8_t handle;
    uint32_t irms_wear[7];
    uint16_t filt_wear_irms;
    uint32_t irms_wear_threshold_lo;
    uint32_t irms_wear_threshold_medhi;
    uint32_t thandle_wear_threshold_lo;
    uint32_t thandle_wear_threshold_medhi;
    uint8_t thandle_wear[7];
    uint8_t thandle_worn;
    uint8_t irms_worn;
    uint8_t worn_count;
    uint16_t z_high_mod;
  }wand;
  struct{
    uint16_t vmod1;
    uint16_t vmod2;
    uint16_t irms1;
    uint16_t irms2;
    uint16_t stator;
    uint16_t temperature;
  }adc;
}DEBUG;

extern DEBUG CANdebug;

#endif

#endif
