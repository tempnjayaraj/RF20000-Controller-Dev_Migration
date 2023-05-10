
#ifndef __MSG_WEREWOLF_H__
#define __MSG_WEREWOLF_H__


#include "system.h"


//////////////////////////////////
// prototocol definitions
//////////////////////////////////
#define PROTOCOL_BDM_TO_WCD             0x36
#define PROTOCOL_WCD_TO_BDM             0x63
#define FOX_CHARLIE                     0xFC
#define AMBIENT_THRESHOLD_MAX           60
#define AMBIENT_THRESHOLD_MIN           20
#define MAX_VOLUME_VALUE                10
#define WAND_NAME_CHARS                 12
#define WAND_CAT_NUM_CHARS              12
#define MAX_BLOB_PAGES                  30
#define EVT_MSG_RETRY_CNT               2
#define EVT_MSG_TIMEOUT                 17 /* (17-1)*20ms = 320ms timeout */


uint8_t packet_cksm(uint8_t,uint8_t *);
void in_bdm_wcd_msg(void);
uint8_t prepareBdmResponse(uint8_t , uint8_t*);

typedef enum {
   // chapter 5
   CAPDEV_WW_GENERIC_REPLY = 0x30,
  
   // chapter 6
   CAPDEV_WW_DISCOVERY,

   // Chapter 7
   CAPDEV_WW_HEARTBEAT,

   // chapter 8
   CAPDEV_WW_PORT_STATUS,
   CAPDEV_WW_GET_PORT_STATUS,

   // Chapter 9
   CAPDEV_WW_SET_THERAPY_SETTINGS,

   // Chapter 10
   CAPDEV_WW_THERAPY_SETTINGS_STATUS,
   CAPDEV_WW_GET_THERAPY_SETTINGS_STATUS,

   // chapter 11
   CAPDEV_WW_GET_ERROR_INFO,

   // chapter 11
   CAPDEV_WW_GET_AMBIENT_SETTINGS,

   // chapter 13
   CAPDEV_WW_GET_WAND_LIFE_INFO,

   // chapter 14
   CAPDEV_WW_GET_GENERATOR_SERIAL_INFO,
   
   // chapter 15
   CAPDEV_WW_GET_CONFIG_BLOB,
   
   // chapter 16
   CAPDEV_WW_SET_CONFIG_BLOB,//0x3D
   
   MAX_CAPDEV_WW_CMD,

} CAPDEV_WW_CMD_TYPES;


#define BDM_TO_WCD_GENERIC_RESPONSE CAPDEV_WW_GENERIC_REPLY
#define WCD_TO_BDM_GENERIC_RESPONSE CAPDEV_WW_GENERIC_REPLY

/******************************
///////////////////////////////
/// Discovery
///////////////////////////////
*******************************/
#define BDM_TO_WCD_DISCOVERY_REQUEST  CAPDEV_WW_DISCOVERY
#define WCD_TO_BDM_DISCOVERY_RESPONSE CAPDEV_WW_DISCOVERY

/**********************************
///////////////////////////////////
* HEARTBEAT MESSAGES
///////////////////////////////////
***********************************/
#define BDM_TO_WCD_HEARTBEAT_CMD   CAPDEV_WW_HEARTBEAT
#define WCD_TO_BDM_HEARTBEAT_CMD   CAPDEV_WW_HEARTBEAT 

/***************
 * BDM TO WCD 
 ***************/
typedef struct hb_bdm_wcd {
   u8  capital_device_ready;  // 0 == OFF, 0x11 is ON
} hb_bdm_wcd_t, *pHbBdmWcd;

#define HB_BDM_WCD_DATA_LENGTH (sizeof(hb_bdm_wcd_t)) // 1

typedef enum {
  ACK,
  GENERAL_ERR_NAK,
  NOT_APPLICABLE_ERR_NAK,
  BUSY_ERR_NAK,
  CMD_LENGTH_ERR_NAK,
  CMD_ID_ERR_NAK,
  CHECKSUM_ERR_NAK,
  FRAMING_ERR_NAK,
  PROTOCOL_ERR_NAK,
  HIGHEST_PRIOIRTY_NAK_ERR,
}ACK_NACK_PRIROITY_T;

typedef enum {
  /*  0 */ ACK_ID,
  /*  1 */ PROTOCOL_ERR_NAK_ID,
  /*  2 */ CMD_ID_ERR_NAK_ID,
  /*  3 */ CMD_LENGTH_ERR_NAK_ID,
  /*  4 */ CHECKSUM_ERR_NAK_ID,
  /*  5 */ FRAMING_ERR_NAK_ID,
  /*  6 */ BUSY_ERR_NAK_ID,
  /*  7 */ NOT_APPLICABLE_ERR_NAK_ID,
  /*  8 */ GENERAL_ERR_NAK_ID,
  /*  9 */ MAX_NUM_OF_ERR_NACK_ID,
}ACK_NACK_ID_T;
 
  /***************
 * WCD to BDM
 ***************/
typedef struct {
  ACK_NACK_ID_T ack_nack_detail;
} ACK_NACK_DETAIL_T;

#define ACK_NAK_SIZE (sizeof(ACK_NACK_DETAIL_T))

/**********************************
///////////////////////////////////
* MACROS
///////////////////////////////////
***********************************/
#define NACK_ERR_MASK(x) (uint32_t)(1<<((ACK_NACK_PRIROITY_T)(x)))


/***************
 * WCD to BDM
 ***************/
#define HB_DEVICE_READY_BIT_MASK     0x01

typedef union {
  u8 val;
  struct {
    u8 ready:1;
    u8 reserved:7;
  } bits;  
} dev_status_val, dev_status_bits;

typedef enum {
  NO_WAND,
  LEGACY_SM,
  SMART_SM,
  LEGACY_ENT,
  SMART_ENT,
  SMART_ARTHROPLASTY,
} primary_wand_type;

typedef union {
  u8 val;
  struct{
    u8 pri_wand:4;
    u8 smart_wand:1;
    u8 legacy_wand:1;
    u8 topaz_timer:1;
    u8 reflex_timer:1;
  } bits;
} wandInfo_val, wandInfo_bits;

typedef union {
  u8 val;
  struct {
    u8 ablate_low_setpoint:2;
    u8 ablate_med_setpoint:2;
    u8 ablate_hi_setpoint:2;
    u8 coag_setpoint:2;
  } bits;
} smartAbCoagSetPoint_val, smartAbCoagSetPoint_bits;

typedef union {
  u8 val;
  struct {
    u8 legacy_ablate_setpoint:4;
    u8 legacy_coag_setpoint:4;
  } bits;
} AbCoag18SetPoint_val,AbCoag18SetPoint_bits;

typedef union {
  u8 val;
  struct {
    u8 pump_flow_settings:4;
    u8 reserved:4;
  } bits;
} pump_settings_val,pump_settings_bits;

typedef union {
  u8 val;
  struct {
    u8 reflex_timer:4;
    u8 reflex_notification_en:1;
    u8 reserved:3;
  } bits;
} reflex_settings_val,reflex_settings_bits;


typedef union {
  u8 val;
  struct {
    u8 therapy_modes:3;
    u8 coag:1;
    u8 ablate:1;
    u8 finger_switch_status:1;
    u8 ablate_status:1;
    u8 coag_status:1;
  } bits;
} therapy_set_val,therapy_set_bits;

typedef union {
  u8 val;
  struct {
    u8 gen_volume_level:4;
    u8 wireless_foot_pedal_enable:1;
    u8 wired_foot_pedal_status:1;
    u8 wireless_foot_pedal_status:1;
    u8 prime_auto_manual:1;
  } bits;
} gen_set_val,gen_set_bits;

typedef union {
  u8 val;
  struct {
    u8 notification:1;
    u8 feature:1;
    u8 color:2;
    u8 volume_level:4;
  } bits;
} amb_set_val,amb_set_bits;

typedef union {
  u8 val;
  struct {
    u8 notification:1;
    u8 feature:1;
    u8 volume_level:4;
    u8 reserved:2;
  } bits;
} set_amb_set_val,set_amb_set_bits;

typedef union {
  u8 val;
  struct {
    u8 color:2;
    u8 capability:1;
    u8 capability_smart:1;
    u8 reserved:4;
  } bits;
} leg_amb_set_val,leg_amb_set_bits;

typedef union {
  u8 val;
  struct {
    u8 over_current:1;
    u8 id:1;
    u8 ambient_patient:1;
    u8 multiple_button_press:1;
    u8 fp_press_wo_wand:1;
    u8 expired:1;
    u8 wand_short:1;
    u8 con_expired_wand:1;
  } bits;
} wand_errors_val,wand_errors_bits;


typedef union {
  u8 val;
  struct {
    u8 ambient:1;
    u8 internal_temp:1;
    u8 suction_tube:1;
    u8 reserved:5;
  } bits;
} sensor_errors_val,sensor_errors_bits;


typedef struct hb_wcd_bdm {
  
  dev_status_bits dev_status;
  wandInfo_val wand_info;
  smartAbCoagSetPoint_val smartAbCoagSetPoint;
  AbCoag18SetPoint_val AbCoag18SetPoint;
  therapy_set_val therapy_set;
  u8 amb_thres_sp;
  gen_set_val gen_set;
  amb_set_val amb_set;
  u8 amb_temp;
  leg_amb_set_val leg_amb_set;
  u8 leg_amb_temp;
  u8 priority;      
} hb_wcd_bdm_t, *pHbWcdBdm;
 
#define HB_WCD_BDM_DATA_LENGTH (sizeof(hb_wcd_bdm_t)) 

///////////////////////////
////// Port Status ////////
///////////////////////////
#define WCD_TO_BDM_PORT_STATUS          CAPDEV_WW_PORT_STATUS
#define BDM_TO_WCD_PORT_STATUS_RESP     CAPDEV_WW_PORT_STATUS    
#define BDM_TO_WCD_GET_PORT_STATUS      CAPDEV_WW_GET_PORT_STATUS
#define WCD_TO_BDM_GET_PORT_STATUS_RESP CAPDEV_WW_PORT_STATUS

typedef union {
  u8 val;
  struct {
    u8 wired:1;
    u8 wireless:1;
    u8 wireless_battery:1;
    u8 reserved:5;
  } bits;
} foot_info_val,foot_info_bits;

typedef union {
  u8 val;
  struct {
    u8 wand_lo_capable:1;
    u8 wand_med_capable:1;
    u8 wand_hi_capable:1;
    u8 wand_vac_capable:1;
    u8 wand_coag_capable:1;
    u8 wand_ablate_capable:1;
    u8 ambient_capable:1;
    u8 reserved:1;
  } bits;
} wand_capable_info_val,wand_capable_info_bits;

typedef union {
  u8 val;
  struct {
    u8 ablate_max:4;
    u8 coag_max:4;
  }bits;
} legacy_therapy_max_val,legacy_therapy_max_bits;


 __packed typedef struct wcd_bdm_port_status {
   foot_info_val  foot_info;
   wandInfo_val  wand_info;
   legacy_therapy_max_val legacy_therapy_max;
   u8 devMode;
   u8  language_id;
   wand_capable_info_val  wand_cap_info;
   u8  wand_name[WAND_NAME_CHARS];
   u8  wand_cat_num[WAND_CAT_NUM_CHARS];
   u32 wand_lot_num;
   u16 wand_id_num;   
} wcd_bdm_port_status_t, *pWcdPortStat;

#define WCD_PORT_STATUS_DATA_LEN             (sizeof(wcd_bdm_port_status_t))
#define BDM_TO_WCD_PORT_STATUS_RESP_DATA_LEN (1) // simple ack/nack byte
#define BDM_TO_WCD_GET_PORT_STATUS_DATA_LEN  (0)

///////////////////////////////////////////////////////////////////
/////////// this is for both set/get therapy settings /////////////
///////////////////////////////////////////////////////////////////
#define BDM_TO_WCD_SET_THERAPY_SETTINGS    CAPDEV_WW_SET_THERAPY_SETTINGS
#define WCD_TO_BDM_SET_THERAPY_RESPONSE    CAPDEV_WW_SET_THERAPY_SETTINGS
#define WCD_TO_BDM_THERAPY_SETTINGS_STATUS CAPDEV_WW_THERAPY_SETTINGS_STATUS
#define BDM_TO_WCD_THERAPY_STATUS_RESPONSE CAPDEV_WW_THERAPY_SETTINGS_STATUS
#define BDM_TO_WCD_GET_THERAPY_SETTINGS    CAPDEV_WW_GET_THERAPY_SETTINGS_STATUS
#define WCD_TO_BDM_GET_THERAPY_RESPONSE    CAPDEV_WW_THERAPY_SETTINGS_STATUS

typedef struct wcd_therapy_settings {
  smartAbCoagSetPoint_val smartAbCoagSetPoint;
  AbCoag18SetPoint_val AbCoag18SetPoint;
  therapy_set_val therapy_set;
  u8 amb_thres_sp;
  pump_settings_val pump_settings;
  set_amb_set_val amb_settings;
  gen_set_val gen_settings;
  reflex_settings_val reflex_timer;
} wcd_therapy_settings_t, *pTherapySettings;

#define WCD_THERAPY_SETTINGS_DATA_LEN (sizeof(wcd_therapy_settings_t))

///////////////////////////////////////////////////////////////////
////////////////// ERROR NOTIFICATION RESPONSE ///////////////////
///////////////////////////////////////////////////////////////////
#define BDM_TO_WCD_GET_ERROR_INFO      CAPDEV_WW_GET_ERROR_INFO
#define WCD_TO_BDM_GET_ERROR_INFO_RESP CAPDEV_WW_GET_ERROR_INFO

typedef union {
  u8 val;
  struct {
    u8 reserved1:4;
    u8 uis_touch_screen:1;
    u8 reserved2:3;
  }bits;
} misc_errors;

typedef struct error_notification {
   u8 priority;
//   u8 wand_errors;
//   u8 sensor_errors;
   misc_errors misc_errors;
} error_notification_t, *pErrorNot;
#define WCD_ERROR_STATUS_DATA_LEN (sizeof(error_notification_t))

///////////////////////////////////////////////////////////////////
////////////////// AMBIENT SETTINGS ///////////////////
///////////////////////////////////////////////////////////////////
#define BDM_TO_WCD_GET_AMBIENT_SETTINGS      CAPDEV_WW_GET_AMBIENT_SETTINGS
#define WCD_TO_BDM_GET_AMBIENT_SETTINGS_RESP CAPDEV_WW_GET_AMBIENT_SETTINGS

typedef struct ambient_settings {
   amb_set_val amb_set;
   u8 amb_temp;
   u8 handle_temp;
   u8 amb_thresh_sp;
   leg_amb_set_val leg_amb_set;
   u8 leg_amb_temp;
} ambient_settings_t, *pAmbSet;

#define WCD_AMBIENT_STATUS_DATA_LEN (sizeof(ambient_settings_t))

///////////////////////////////////////////////////////////////////
////////////////// WAND INFO SETTINGS ///////////////////
///////////////////////////////////////////////////////////////////
#define BDM_TO_WCD_GET_WAND_LIFE_INFO      CAPDEV_WW_GET_WAND_LIFE_INFO
#define WCD_TO_BDM_GET_WAND_LIFE_INFO_RESP CAPDEV_WW_GET_WAND_LIFE_INFO
__packed typedef struct want_life_info {
   u16 wand_life_low;
   u16 wand_life_med;
   u16 wand_life_high;
   u16 wand_life_vac;
   u16 wand_life_coag;
} wand_life_info_t, *pWandInfo;

#define WAND_LIFE_INFO_DATA_LEN (sizeof(wand_life_info_t))

///////////////////////////////////////////////////////////////////
////////////////// GENERATOR SETTINGS ///////////////////
///////////////////////////////////////////////////////////////////
#define BDM_TO_WCD_GET_GENERATOR_INFO      CAPDEV_WW_GET_GENERATOR_SERIAL_INFO
#define WCD_TO_BDM_GET_GENERATOR_INFO_RESP CAPDEV_WW_GET_GENERATOR_SERIAL_INFO
__packed typedef struct generator_info {
   u8 serialNum[MFG_SERIAL_NUM_CHARS];
   u32 born_date;
   u8 app_version_major;
   u8 app_version_minor;
   u8 gui_version_major;
   u8 gui_version_minor;
} generator_info_t, *pGenInfo;

#define GENERATOR_SERIAL_INFO_DATA_LEN (sizeof(generator_info_t))
/**********************************
///////////////////////////////////
* Blob INFO stuff
///////////////////////////////////
***********************************/
#define NUM_BLOB_DATA_DEFAULT               3

#define BDM_TO_WCD_GET_BLOB_CONFIG_REQUEST  CAPDEV_WW_GET_CONFIG_BLOB
#define WCD_TO_BDM_GET_BLOB_CONFIG_RESPONSE CAPDEV_WW_GET_CONFIG_BLOB // data blob
#define BDM_TO_WCD_GET_BLOB_CONFIG_DATA_ACK CAPDEV_WW_GET_CONFIG_BLOB_DATA_RESPONSE

#define BDM_TO_WCD_SET_BLOB_CONFIG_REQUEST  CAPDEV_WW_SET_CONFIG_BLOB // data blob
#define WCD_TO_BDM_SET_BLOB_CONFIG_RESPONSE CAPDEV_WW_SET_CONFIG_BLOB // ack

#define WW_BLOB_DATA_SIZE       (232)
#define WW_TOTAL_BLOB_DATA_SIZE (WW_BLOB_DATA_SIZE + 1)

#endif // __MSG_WEREWOLF_H__
