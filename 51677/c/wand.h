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
\file      wand.h
\brief     Header file for smart wand driver.
\details   NA
*******************************************************************************/

#ifndef WAND_H
#define WAND_H

#include "common.h"
#ifdef TEST
#include "artc_arm_math.h"
#else
#include "arm_math.h"
#endif

/* Packet sizes */
#define PARAM_PAGE_SIZE         64
#define STATIC_PAGE_SIZE        8
//#define STATIC_MAX_NUM_PAGES    15
#define STATIC_MAX_NUM_PAGES    12
#define MAX_TX_DATA_BYTES       64
#define PASSCODE_DATA_SIZE      16
#define MFG_DATA_SIZE           64
#define CIRC_LOG_PAGE_SIZE      8
#define PERIODIC_DATA_SIZE      0
#define MAX_STATIC_LOG_UNITS    5
#define MAX_WORN_WAND_DETECTION_WINDOW 30
#define MIN_WORN_WAND_DETECTION_WINDOW 2

#define PARAM_PAGE_0            0        
#define PARAM_PAGE_1            1
#define PARAM_PAGE_2            2
#define PARAM_PAGE_3            3
#define PARAM_PAGE_4            4

typedef enum {
  LOG_UNIT_SECONDS,
  LOG_UNIT_5_SECONDS,
  LOG_UNIT_MINUTES,
  LOG_UNIT_HOURS,
}LOG_TIME_UNITS;

#pragma pack(1)
typedef struct{
  uint8_t original_mode_active_time[16];
  uint8_t type;
  uint8_t mode_active_time[16];
  uint8_t unused[7];
  struct {
    uint8_t id;
    uint32_t sn;
    uint8_t errors;
    uint8_t unused[2];
    uint32_t use_time;
    uint16_t start_idx;
    uint16_t end_idx;
  }unit[MAX_STATIC_LOG_UNITS];
}STATIC_LOG;

/* Passcode unlock command template */

typedef struct {
    uint8_t head;
    uint8_t type;
    uint8_t number;
    uint8_t temp1;
    uint8_t temp2;
    uint8_t key;
    uint8_t size;
    uint8_t cmd;
    uint8_t idx;
    uint8_t data[PARAM_PAGE_SIZE];
    uint8_t checksum;
}RX_PARAM_PACKET;


typedef struct {
    uint8_t head;
    uint8_t type;
    uint8_t number;
    uint8_t temp1;
    uint8_t temp2;
    uint8_t key;
    uint8_t size;
    uint8_t cmd;
    uint8_t idx;
    uint8_t data[STATIC_PAGE_SIZE];
    uint8_t checksum;
}RX_STATIC_READ_PACKET;


typedef struct {
    uint8_t head;
    uint8_t type;
    uint8_t number;
    uint8_t temp1;
    uint8_t temp2;
    uint8_t key;
    uint8_t size;
    uint8_t cmd;
    uint8_t idx;
    uint8_t checksum;
}RX_STATIC_WRITE_PACKET;


typedef struct {
    uint8_t head;
    uint8_t type;
    uint8_t number;
    uint8_t temp1;
    uint8_t temp2;
    uint8_t key;
    uint8_t size;
    uint8_t cmd;
    uint8_t idx;
    uint8_t data[PASSCODE_DATA_SIZE];
    uint8_t checksum;
}RX_PASSCODE_PACKET;


typedef struct {
    uint8_t head;
    uint8_t type;
    uint8_t number;
    uint8_t temp1;
    uint8_t temp2;
    uint8_t key;
    uint8_t size;
    uint8_t cmd;
    uint8_t idx;
    uint8_t data[MFG_DATA_SIZE];
    uint8_t checksum;
}RX_MFG_PACKET;    


typedef struct {
    uint8_t head;
    uint8_t type;
    uint8_t number;
    uint8_t temp1;
    uint8_t temp2;
    uint8_t key;
    uint8_t size;
    uint8_t cmd;
    uint8_t idx;
    uint8_t data[CIRC_LOG_PAGE_SIZE];
    uint8_t checksum;
}RX_CIRC_LOG_READ_PACKET;


typedef struct {
    uint8_t head;
    uint8_t type;
    uint8_t number;
    uint8_t temp1;
    uint8_t temp2;
    uint8_t key;
    uint8_t size;
    uint8_t cmd;
    uint8_t idx;
    uint8_t checksum;
}RX_CIRC_LOG_WRITE_PACKET;


typedef struct {
    uint8_t head;
    uint8_t type;
    uint8_t number;
    uint8_t size;
    uint8_t cmd;
    uint8_t idx;
    uint8_t data[MAX_TX_DATA_BYTES];
    uint8_t checksum;
}TX_PACKET,*TX_PACKET_PTR;


typedef struct {
  uint8_t head;
  uint8_t type;
  uint8_t number;
  uint8_t temp1;
  uint8_t temp2;
  uint8_t key;
  uint8_t size;
  uint8_t checksum;
}RX_PERIODIC_PACKET,*RX_PERIODIC_PACKET_PTR;


typedef struct {
  uint8_t page_read;
  uint8_t page_write;
  uint8_t unit_num;
  RX_STATIC_READ_PACKET page[16+7];
  RX_STATIC_WRITE_PACKET write_echo;
}RX_STATIC_LOG;


typedef struct {
  uint8_t roll;
  uint8_t page;
  uint8_t idx;
  uint32_t mode_active_accum[16+7];
  RX_CIRC_LOG_READ_PACKET segment;
  RX_CIRC_LOG_WRITE_PACKET write_echo;
}RX_CIRCLE_LOG;

typedef struct {
  RX_PASSCODE_PACKET passcode;
  RX_MFG_PACKET mfg;
  RX_CIRCLE_LOG circle_log;
  RX_PARAM_PACKET param[5];
  RX_PERIODIC_PACKET periodic;
  RX_STATIC_LOG static_log;
}SMART_WAND_DATABASE;
  
/* Wand commands */
typedef enum {
  SMART_WAND_UNLOCK = 0,
  SMART_WAND_LOCK,
  SMART_WAND_READ_MFG_DATA,
  SMART_WAND_WRITE_MFG_DATA,
  SMART_WAND_READ_PARAM_DATA,
  SMART_WAND_WRITE_PARAM_DATA,
  SMART_WAND_READ_STATIC_LOG,
  SMART_WAND_WRITE_STATIC_LOG,
  SMART_WAND_READ_CIRC_LOG,
  SMART_WAND_WRITE_CIRC_LOG,
  SMART_WAND_DEFAULT
}SmartWandCmdLib;

typedef enum {
  HEAD,
  TYPE,
  NUMBER,
  TEMP1,
  TEMP2,
  KEY,
  SIZE,
  CMD,
  IDX,
  DATA,
  CHECKSUM
}PACKET_STATE;

typedef enum {
  LOCK,
  UNLOCK,
  PARAM_0,
  PARAM_1,
  PARAM_2,
  PARAM_3,
  PARAM_4,
  STATIC_LOG_READ,
  STATIC_LOG_WRITE_1,
  STATIC_LOG_WRITE_2,
  CIRCLE_LOG_READ,
  ERASE_STATIC_LOGS,
  ERASE_CIRCLE_LOGS,
  SWITCH_BOOT_TO_LOG,
  PERIODIC
}WAND_STATE;

#pragma pack(1)
typedef struct{
  uint8_t idx;
  struct {
    uint8_t size;
    uint8_t txcount;
    uint8_t data[MAX_TX_DATA_BYTES];
  }cmd[10];
}WAND_TX_PACKET_QUEUE;

//TYPEDEFS

typedef struct {
  bool pump_running;
  bool blink;
  bool ap_rf_detect;
} manta_prime;

/* Smart wand mfg file indication byte lets controller know the wand type */
enum Smart_Wand_MFG_Indication 
{
  SMART_WAND_MFG_INDICATION_SPORTS_MED,	/**< Enum value 0 Sport Med indication  */
  SMART_WAND_MFG_INDICATION_ENT,		/**< Enum value 1 ENT indication */
  SMART_WAND_MFG_INDICATION_MANTA,		/**< Enum value 2 MANTA indication */
  NO_SMART_WAND_CONNECTED = 255,
};

/* Packet preambles */
#define PACKET_HEAD_TX       0x95u
#define PACKET_HEAD_RX       0x94u
#define PACKET_TYPE          0x10u

/* Key board masks */
#define MODE_KEY_MASK           0x1
#define MODE_KEY_SHIFT          0   
#define COAG_KEY_MASK           0x2
#define COAG_KEY_SHIFT          1
#define ABLATE_KEY_MASK         0x4
#define ABLATE_KEY_SHIFT        2        

/* Criteria (in packet qyt) for transitioning to VAC mode */ 
#define VAC_MODE_DELAY_TIME_MS  500
#define PACKET_PERIOD_MS        20 

#define SMART_WAND_PASSCODE_RX_PACKET_SIZE       (sizeof(RX_PASSCODE_PACKET))
#define SMART_WAND_CIRC_LOG_READ_RX_PACKET_SIZE  (sizeof(RX_CIRC_LOG_READ_PACKET))
#define SMART_WAND_CIRC_LOG_WRITE_RX_PACKET_SIZE (sizeof(RX_CIRC_LOG_WRITE_PACKET))
#define SMART_WAND_STATIC_READ_RX_PACKET_SIZE    (sizeof(RX_STATIC_READ_PACKET))
#define SMART_WAND_STATIC_WRITE_RX_PACKET_SIZE   (sizeof(RX_STATIC_WRITE_PACKET))
#define SMART_WAND_PARAM_RX_PACKET_SIZE          (sizeof(RX_PARAM_PACKET))
#define SMART_WAND_PERIODIC_PACKET_SIZE          (sizeof(RX_PERIODIC_PACKET))
#define SMART_WAND_MFG_PACKET_SIZE               (sizeof(RX_MFG_PACKET))
#define SMART_WAND_DATABASE_SIZE                 (sizeof(SMART_WAND_DATABASE))
#define Q62_TEMPERATURE_SCALE                    ((10 << 2) + 2)

/* Function Prototypes */
void SmartWandSendCMD (volatile uint8_t*, uint8_t);
uint32_t smartwand_prepare_tx_packet(uint8_t, uint8_t, uint8_t *, uint8_t);
void SmartWand_DMA_Init(void);
uint8_t smartwand_uart_init(void);
void SmartWand_GPIO_Init(void);
void SmartWand_Timeout_Init(void);
uint8_t smartwand_timeout(void);
void smartwand_periodic_packet_handler(void);
uint8_t wand_periodic_packet_parse(void);
float32_t half2float(uint16_t);
uint16_t float2half(float32_t);
uint8_t wand_validate(void);
uint8_t wand_download_parm_pg_0(void);
uint8_t wand_download_parm_pg_1(void);
uint8_t wand_download_parm_pg_2(void);
uint8_t wand_download_parm_pg_3(void);
uint8_t wand_download_parm_pg_4(void);
uint8_t wand_unlock(void);
uint8_t wand_lock(void);
uint8_t wand_tx(void);
uint8_t wand_static_read(void);
uint8_t wand_static_write(void);
void smartwand_send_packet(void);
uint8_t wand_circle_log_read(void);
void log_use_data(uint32_t);
uint8_t wand_use_limit_check(void);
uint8_t wand_expired_check(void);
uint8_t thermometer_color(uint8_t temperature);
void extract_wand_params(void);
uint8_t convert_q62_ambient_to_7bit(uint8_t);
uint8_t periodic_log_errors2wand(void);
uint8_t wand_time_limit_check(uint8_t*, uint16_t*);
uint32_t set_use_limit_time(uint32_t *);
uint8_t wand_circle_write(void);
uint8_t wand_thandle_wear_monitor(uint8_t, uint8_t, uint8_t);
uint8_t wand_wear_monitor(void);
uint8_t wand_irms_wear_monitor(uint8_t, uint8_t);
uint8_t switch_to_ww_wand(void);
void WandFIR_Init(q15_t);
//uint8_t logerror2wand(uint8_t);
void wand_mode_life_histogram(uint32_t *, uint8_t *);
uint8_t wand_not_active_periodic_packet_parse(void);
uint8_t wand_worn_use_limit(void);
uint16_t z_limit_adjust4temp(uint8_t);
uint8_t get_ww_temp (void);
uint8_t manta_coag_mode_increment(void);
void manta_boundary_limit_flow_vals(void);
extern bool sm_wand_disconnect;

static const uint16_t VAC_MODE_PACKET_DELAY = VAC_MODE_DELAY_TIME_MS/PACKET_PERIOD_MS;
extern uint8_t SmartWandDatabase[];
#ifdef DEBUG_CAN
extern SMART_WAND_DATABASE db;
#endif

#endif

