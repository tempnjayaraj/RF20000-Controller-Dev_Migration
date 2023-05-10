/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2822                 $:
$Date:: 2022-03-07 12:56:09#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     event_buffer.h
\brief     A queue for system events
\details   
*******************************************************************************/

#ifndef EVENT_BUFFER_H
#define EVENT_BUFFER_H

#include <stdint.h>
#include <intrinsics.h>

typedef enum
{
  NULL_EVENT,                   /*  1   */
  WAND_PARAM_0_RX,              /*  2   */
  WAND_PARAM_1_RX,              /*  3   */
  WAND_PARAM_2_RX,              /*  4   */
  WAND_PARAM_3_RX,              /*  5   */
  WAND_PARAM_4_RX,              /*  6   */
  WAND_MFG_RX,                  /*  7   */
  WAND_PASSCODE_RX,             /*  8   */
  WAND_PERIODIC_RX,             /*  9   */
  CHANGE_MODE,                  /*  10  */
  POLL_FOOTPEDAL,               /*  11  */
  VOLUME_CTRL,                  /*  12  */
  RESET_ALARM_AUDIO,            /*  13  */
  RUN_PUMP_SPEED_CTRL,          /*  14  */
  THERAPY_ON,                   /*  15  */
  THERAPY_OFF,                  /*  16  */
  GUI_GRAPHIC,                  /*  17  */
  POWER_UP,                     /*  18  */
  POLL_PWR_SWITCH,              /*  19  */
  NOTIFY_EVENT,                 /*  20  */
  FAULT_EVENT,                  /*  21  */
  SD_CARD_EVENT,                /*  22  */
  CTRL_LED_INPUTS,              /*  23  */
  WAND_UNPLUGGED,               /*  24  */
  WAND_TX,                      /*  25  */
  WAND_TIMEOUT,                 /*  26  */
  POLL_PUMP_SENSORS,            /*  27  */
  UPDATE_SCREEN,                /*  28  */
  RF_ON,                        /*  29  */
  RUN_IEC_OVERCURRENT,          /*  30  */
  PUMP_INACTIVITY_MONITOR,      /*  31   this isn't currently used for anything */
  POLL_ADC_DATA,                /*  32  */
  CAN_RX,                       /*  33  */
  CLICK_SOUND,                  /*  34  */
  CAN_TX,                       /*  35  */
  RF_OFF,                       /*  36  */
  MEASURE_IMPEDANCE,            /*  37  */
  PUMP_CAN_TX_DEBUG,            /*  38  */
  PUMP_CMD_ACK,                 /*  39  */
  PUMP_RESET,                   /*  40  */
  PUMP_TX,                      /*  41  */
  DRIVE_STATUS_RX,              /*  42  */
  PUMP_CMD_NACK,                /*  43  */
  CAN_WDOG_RX,                  /*  44  */
  INIT,                         /*  45  */
  POST,                         /*  46  */
  CAN_TIMEOUT,                  /*  47  */
  WAND_SHORT,                   /*  48  */
  WRITE_SDHC_EOL,               /*  49  */
  RESET_ERROR,                  /*  50  */
  SUCTION_TUBE_ERROR,           /*  51  */
  NO_AMBIENT_SENSOR,            /*  52  */
  ADC0_COCO,                    /*  53  */
  INTERNAL_TEMP_ERROR,          /*  54  */
  CLEAR_INTERNAL_TEMP_ERROR,    /*  55  */
  ADC_BOUNDARY_FAULT,           /*  56  */
  CRC_FAULT,                    /*  57  */
  PUMP_COM_FAULT,               /*  58  */
  VMOD_FAULT,                   /*  59  */
  INTERNAL_TEMP_FAULT,          /*  60  */
  IRMS2_ADC_RESULT,             /*  61  */
  WAND_STATIC_READ_RX,          /*  62  */
  WAND_STATIC_WRITE_RX,         /*  63  */
  WAND_CIRCLE_READ_RX,          /*  64  */
  WDOG_COP_RESET,               /*  65  */
  MFG_SN_FAULT,                 /*  66  */        
  MFG_DATE_TIME_FAULT,          /*  67  */
  DRIVE_POWER_MODE_RX,          /*  68  */
  PROBE_TEST_COMPLETE,          /*  69  */
  TWO_SEC_DELAY,                /*  70  */
  INIT_LPT,                     /*  71  */
  INIT_PUMP_UART,               /*  72  */
  INIT_DMA_MUX,                 /*  73  */
  INIT_SYSTICK,                 /*  74  */
  INIT_AUDIO_VOL_CTRL,          /*  75  */
  INIT_AUDIO_PWM,               /*  76  */
  INIT_FOOT_PEDAL,              /*  77  */
  INIT_BUCK,                    /*  78  */
  INIT_SMART_WAND_PWM,          /*  79  */
  INIT_ADC,                     /*  80  */
  INIT_FLEXCAN,                 /*  81  */
  INIT_SMARTWAND_UART,          /*  82  */
  INIT_DOOR_SENSOR,             /*  83  */
  INIT_TUBE_SENSOR,             /*  84  */
  INIT_LED,                     /*  85 */
  INIT_LEGACY_WAND,             /*  86  */
  INIT_FOOT_PEDAL_ZIGBEE,       /*  87  */
  INIT_ESDHC,                   /*  88  */
  INIT_AUDIO_AMP,               /*  89  */
  INIT_MANAGER,                 /*  90  */
  REFRESH_WDOG,                 /*  91  */
  SAVE_PARAMS_TO_FLASH,         /*  92  */
  WAND_SWITCH_STUCK,            /*  93  */
  WAND_EXPIRED,                 /*  94  */
  WAND_USE_LIMIT_ERROR,         /*  95  */
  WAND_USE_LIMIT_CHECK,         /*  96  */
  WAND_ID_ERROR,                /*  97  */
  FOOT_SWITCH_STUCK,            /*  98  */
  OVER_CURRENT,                 /*  99  */
  OVER_ENERGY,                  /*  100 */
  GET_MFG_DATE_TIME,            /*  101 */
  GET_SERIAL_NUM,               /*  102 */
  WAND_CAN_TX_DEBUG,            /*  103 */
  RESET_ROTOR_PULSE_CNT,        /*  104 */
  SYSTEM_RESET,                 /*  105 */
  WAND_LOG_ERRORS,              /*  106 */
  BUCK_INACTIVE_SHUTDOWN,       /*  107 */
  WAND_CIRCLE_WRITE_RX,         /*  108 */
  SLOW_CAN_TX,                  /*  109 */
  POLL_WIRELESS_STATUS,         /*  110 */
  WIRELESS_STATUS_CAN_TX,       /*  111 */
  WIRELESS_HANDSHAKE,           /*  112 */
  UI_UPDATE_STATUS,             /*  113 */
  COMMENCE_UIS_SW_UPDATE,       /*  114 */
  COMMENCE_GS_SW_UPDATE,        /*  115 */
  SDHC_DOWNLOAD,                /*  116 */
  WAND_IRMS_WEAR_MONITOR,       /*  117 */
  WAND_WEAR_CAN_DEBUG,          /*  118 */
  LW_READ,                      /*  119 */
  LW_ABLATE_ON_EVENT,           /*  120 */
  LW_COAG_ON_EVENT,             /*  121 */
  LW_MODE_ON_EVENT,             /*  122 */
  LW_ABLATE_OFF_EVENT,          /*  123 */
  LW_COAG_OFF_EVENT,            /*  124 */
  LW_MODE_OFF_EVENT,            /*  125 */
  IRMS1_ADC_RESULT,             /*  126 */ 
  LW_RUN_IEC_OVERCURRENT,       /*  127 */ 
  LW_PLUGGED_IN,                /*  128 */
  CHECK_ELECTRODE_ERROR,        /*  129 */
  LW_UNPLUGGED,                 /*  130 */
  LW_ADC_FAULT,                 /*  131 */
  LW_SWITCH_STUCK,              /*  132 */
  LW_EXPIRED,                   /*  133 */
  LW_USE_LIMIT_ERROR,           /*  134 */
  LW_NO_AMBIENT_SENSOR,         /*  135 */
  CANCEL_SW_UPDATE,             /*  136 */
  VMOD2_TEST,                   /*  137 */
  SDHC_LOG_DATA,                /*  138 */
  LW_ID_ERROR,                  /*  139 */
  LW_INCOMPATIBLE,              /*  140 */
  VMOD_LINEAR_INTERP,           /*  141 */
  GEN_CAN_TIMEOUT,              /*  142 */
  BOTH_WANDS_DISCONNECT_ERROR,  /*  143 */
  VMOD1_TEST,                   /*  144 */
  UI_FAULT,                     /*  145 */
  PUMP_CMD_EACK,                /*  146 */
  SYSTICK,                      /*  147 */
  LW_UNPLUGGED_LOAD_SMARTWAND,  /*  148 */
  SWITCH_WAND,                  /*  149 */
  POLL_USB,                     /*  150 */
  ENT_SM_INCOMPATIBLE,          /*  151 */     
  MAX_EVENT
}event;

#define EVENT_BUFFER_SIZE 50

typedef struct event_table 
{
  event buff[EVENT_BUFFER_SIZE];
  uint8_t index;
} event_table;


uint8_t put_event (event_table*, event);
event get_event (event_table*);
/* 
uint8_t is_empty (event_table*);
uint8_t is_full (event_table*);
*/
void flush_buffer(event_table*);

#endif

