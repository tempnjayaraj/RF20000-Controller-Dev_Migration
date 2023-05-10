/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author::               $:
$Rev::                  $:
$Date::                 $:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
********************************************************************************
*******************************************************************************/

/***************************************************************************//**
\file      msgWerewolf.c
\brief     USB message communicator
\details   This application file implements and executes WCD serial protocol 
\messages.Also processes messages received from external USB device.
*******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "commonTypes.h"
#include "msgWerewolf.h"
#include "usb.h"
#include "ct_app_ww.h"
#include "system.h"
#include "Settings.h"
#include "sw_manager.h"
#include "ftfe.h"
#include "wand.h"

//-----------LOCAL FUNCTIONS______-------------------
ACK_NACK_ID_T nak_cmd(uint8_t* ptemp_buff, uint8_t cmd, uint32_t nack);
ACK_NACK_ID_T nack_priority(uint32_t nack);

//-----------INITIALIZED VARIABLES-------------------

bool incoming_bdm_msg           = FALSE;
bool packet_in_valid            = FALSE;
bool isWWConnected              = FALSE;
//bool isWWConnected              = TRUE //bypasses protocol discovery to all commands
bool isWWReady                  = FALSE;
bool sendPortStatus             = FALSE;
bool sendTherapyStatus          = FALSE;
bool sendDevInfoStatus          = FALSE;
bool usb_mutex                  = FALSE;
u32  rxHeartbeatCount           = 0;
u32  txHeartbeatCount           = 0;
u32  rxHeartbeatMiss            = 0;
u8 request_num_mask             = 0x80;
u8 port_status_ack_received     = 0;
u8 therapy_status_ack_received  = 0;
#ifdef DEBUG
u32 port_stats_msg_cnt          = 0;

#endif

//-----------UN-INITIALIZED VARIABLES-------------------
uint8_t                         EPSize; 
uint8_t                         bdm_rx_pack_cksm;
req_num                         seq_num_out;
uint8_t                         *bdm_msg_in;
uint8_t                         *bdm_msg_out;
uint8_t                         tx_msg_len,EP;
bdm_serial_msg_container_t      data_in;

//-----------MESSAGE VARIABLES---------------------------
hb_wcd_bdm_t                    heart_beat_msg;
capdev_to_bdm_discovery_t       cap_disc_msg;
wcd_bdm_port_status_t           port_status_msg;
wcd_therapy_settings_t          set_therapy_msg;
wcd_therapy_settings_t          get_therapy_msg;
error_notification_t            error_info_msg;
ambient_settings_t              amb_info_msg;
wand_life_info_t                wand_life_msg;
generator_info_t                gen_info_msg;


//-----EXTERNALS---------------------------
extern uint8_t                  *pu8IN_DataPointer;
extern uint8_t                  gu8USB_Flags;
extern uint8_t                  gu8USBClearFlags;
extern uint8_t                  *pu8IN_DataPointer;
extern uint8_t                  gu8IN_Counter;
extern uint8_t                  gu8USB_Toogle_flags;
extern uint8_t                  gu8USB_PingPong_flags;
extern uint8_t                  gu8Dummy;
extern uint16_t                 gu8Status;
extern uint8_t                  gu8Interface;
extern uint8_t                  gu8HALT_EP;
extern uint8_t                  gu8USB_State;
extern tUSB_Setup               *Setup_Pkt;
extern tBDT                     tBDTtable[16];
extern const uint8_t            cEP_Size[];
extern uint8_t                  *BufferPointer[];
extern legacy_wand_params       legacy_wand_params_obj;
extern GEN_SETTINGS             current_therapy_settings;
extern SMART_WAND_PARAMETERS    smart_wand_params;
extern bool                     evt_port_status;
extern bool                     wait_for_therapy_status_ack;
extern bool                     wait_for_port_status_ack;
extern u8                       timer_port_status_ack_value;
extern u8                       timer_therapy_status_ack_value;
extern u8                       retry_cnt_port_status_ack;
extern u8                       retry_cnt_therapy_status_ack;


#pragma pack(1)
typedef struct
{
  u8 pid;
  u8 cmd;
  u8 seq;
  u8 length;
}REPLY_PACKET_HEADER_T;

#pragma pack(1)
typedef struct
{
  u8 page;
  void *data;
}SETUP_BLOB_CMD_DATA_T;

#pragma pack(1)
typedef struct
{
  u8 crc;
  u8 eol;
}REPLY_PACKET_TAIL_T;

#pragma pack(1)
typedef struct
{
  u8 version;
  u8 length_head;
  u16 length_data;
  u8 type;
  u8 sub_type;
  u8 name[16];
  u8 crc;
}SETUP_BLOB_HEADER_T;

#pragma pack(1)
typedef struct
{
  u8 page;
  u8 ack;
}BLOB_REPLY_T;

uint16_t smart_wand_coag_life;
extern SMART_WAND_DATABASE db;
extern STATIC_LOG static_log;


void wand_coag_life(uint32_t *ptr_1, uint8_t *ptr_2)
{
  smart_wand_coag_life = *(ptr_1+12);
  smart_wand_coag_life += (*(ptr_2+12))*15;
  smart_wand_coag_life += *(ptr_1+13);
  smart_wand_coag_life += (*(ptr_2+13))*15;
  smart_wand_coag_life += *(ptr_1+14);
  smart_wand_coag_life += (*(ptr_2+14))*15;   
}

/***************************************************************************//**
Update Therapy Settings. Updates therapy settings received from external USB 
to Werewolf.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void update_therapy_settings(void)
{
  
  if((current_therapy_settings.wand.valid) && (port_status_msg.wand_info.bits.pri_wand != NO_WAND))
  {
    
    if ((set_therapy_msg.smartAbCoagSetPoint.bits.ablate_low_setpoint > 0) && (set_therapy_msg.smartAbCoagSetPoint.bits.ablate_low_setpoint <=3))
    {
      current_therapy_settings.setpoint.lo                  = (CUT_LEVELS)set_therapy_msg.smartAbCoagSetPoint.bits.ablate_low_setpoint;
    }
    
    if ((set_therapy_msg.smartAbCoagSetPoint.bits.ablate_med_setpoint > 0) && (set_therapy_msg.smartAbCoagSetPoint.bits.ablate_med_setpoint <=3))
    {
      current_therapy_settings.setpoint.med                  = (CUT_LEVELS)set_therapy_msg.smartAbCoagSetPoint.bits.ablate_med_setpoint;
    }
    
    if ((set_therapy_msg.smartAbCoagSetPoint.bits.ablate_hi_setpoint > 0) && (set_therapy_msg.smartAbCoagSetPoint.bits.ablate_hi_setpoint <=3))
    {
      current_therapy_settings.setpoint.hi                  = (CUT_LEVELS)set_therapy_msg.smartAbCoagSetPoint.bits.ablate_hi_setpoint;
    }
    
    if ((set_therapy_msg.smartAbCoagSetPoint.bits.coag_setpoint > 0) && (set_therapy_msg.smartAbCoagSetPoint.bits.coag_setpoint <=2))
    {
      current_therapy_settings.setpoint.coag                  = (COAG_LEVELS)set_therapy_msg.smartAbCoagSetPoint.bits.coag_setpoint;
    }
    
    if ((set_therapy_msg.therapy_set.bits.therapy_modes > NONE) && (set_therapy_msg.therapy_set.bits.therapy_modes < VAC)) 
    {
      if((set_therapy_msg.therapy_set.bits.therapy_modes == LO) && (smart_wand_params.mfg.low_enable))
      {
        current_therapy_settings.treatment.therapy_mode = (MODES)set_therapy_msg.therapy_set.bits.therapy_modes;
        current_therapy_settings.treatment.previous_therapy_mode = current_therapy_settings.treatment.therapy_mode;
      }
      if((set_therapy_msg.therapy_set.bits.therapy_modes == MED) && (smart_wand_params.mfg.med_enable))
      {
        current_therapy_settings.treatment.therapy_mode = (MODES)set_therapy_msg.therapy_set.bits.therapy_modes;
        current_therapy_settings.treatment.previous_therapy_mode = current_therapy_settings.treatment.therapy_mode;
      }
      if((set_therapy_msg.therapy_set.bits.therapy_modes == HI) && (smart_wand_params.mfg.hi_enable))
      {
        current_therapy_settings.treatment.therapy_mode = (MODES)set_therapy_msg.therapy_set.bits.therapy_modes;
        current_therapy_settings.treatment.previous_therapy_mode = current_therapy_settings.treatment.therapy_mode;
      }
    }
    //current_therapy_settings.treatment.ablate_enable      = set_therapy_msg.therapy_set.bits.ablate;
    //smart_wand_params.mfg.coag_enable               = set_therapy_msg.therapy_set.bits.coag; //Coag cant be disabled
  }
  
  if((legacy_wand_params_obj.wand_connected) && (port_status_msg.wand_info.bits.pri_wand != NO_WAND))
  {
    if ((set_therapy_msg.AbCoag18SetPoint.bits.legacy_ablate_setpoint <= legacy_wand_params_obj.max_ablate) && (set_therapy_msg.AbCoag18SetPoint.bits.legacy_ablate_setpoint <= 10))
    {
      legacy_wand_params_obj.ablate_level                   = set_therapy_msg.AbCoag18SetPoint.bits.legacy_ablate_setpoint;
    }
    
    if ((set_therapy_msg.AbCoag18SetPoint.bits.legacy_coag_setpoint <= legacy_wand_params_obj.max_coag) && (set_therapy_msg.AbCoag18SetPoint.bits.legacy_coag_setpoint <= 2))
    {
      legacy_wand_params_obj.coag_level = set_therapy_msg.AbCoag18SetPoint.bits.legacy_coag_setpoint;
    }
    //current_therapy_settings.treatment.ablate_enable      = set_therapy_msg.therapy_set.bits.ablate;
    //legacy_wand_params_obj.coag_enable              = set_therapy_msg.therapy_set.bits.coag; //Coag cant be disabled
  }
  
  current_therapy_settings.treatment.finger_switch_en   = (bool)set_therapy_msg.therapy_set.bits.finger_switch_status;
  
  if((set_therapy_msg.gen_settings.bits.gen_volume_level <= MAX_VOLUME_VALUE) && (set_therapy_msg.gen_settings.bits.gen_volume_level > 0))
  {
    current_therapy_settings.audio.volume = set_therapy_msg.gen_settings.bits.gen_volume_level;
  }
  if((current_therapy_settings.wand.valid && !current_therapy_settings.active_wand.lw_active) || //smart wand
     (legacy_wand_params_obj.wand_connected && current_therapy_settings.active_wand.lw_active) || //legacy wand
       (!current_therapy_settings.wand.valid && !legacy_wand_params_obj.wand_connected) )  //no wand
  {
    current_therapy_settings.ambient.enable_user_pref     = set_therapy_msg.amb_settings.bits.feature;
    current_therapy_settings.ambient.enable               = (current_therapy_settings.ambient.capable & current_therapy_settings.ambient.enable_user_pref);
    
    if(current_therapy_settings.ambient.enable)
    {
      current_therapy_settings.ambient.alarm_threshold      = ((set_therapy_msg.amb_thres_sp > AMBIENT_THRESHOLD_MAX) || (set_therapy_msg.amb_thres_sp < AMBIENT_THRESHOLD_MIN))? 
        current_therapy_settings.ambient.alarm_threshold : set_therapy_msg.amb_thres_sp;
        current_therapy_settings.ambient.audio_enabled        = set_therapy_msg.amb_settings.bits.notification;
        if ((set_therapy_msg.amb_settings.bits.volume_level <= MAX_VOLUME_VALUE) && (set_therapy_msg.amb_settings.bits.volume_level > 0))
        {
          current_therapy_settings.ambient.volume  = set_therapy_msg.amb_settings.bits.volume_level;
        }
    }
  }
  if(current_therapy_settings.footpedal.wireless.married_latch == 1)// && !current_therapy_settings.footpedal.wireless.present)
  {
    current_therapy_settings.treatment.wireless_enable    = (bool)set_therapy_msg.gen_settings.bits.wireless_foot_pedal_enable;
  }
}

/***************************************************************************//**
Get Next Transmit Sequence Number. Determines the correct request number for message 
transmission to external device.
@param void
@return next request number
@outputs
@context
*******************************************************************************/
u8 getNextTxSeqSerial(void)
{
  if (data_in.cmd_sequence_number.bits.req_num_msb == TRUE)
  {
    seq_num_out.bits.request_num = data_in.cmd_sequence_number.bits.request_num;
    seq_num_out.bits.req_num_msb = ~data_in.cmd_sequence_number.bits.req_num_msb;
    data_in.cmd_sequence_number.val = 0;
  }
  else
  {
    if(seq_num_out.bits.request_num == 0x7F)
    {
      seq_num_out.bits.request_num = 1;
    }
    else
    {
      ++seq_num_out.bits.request_num;
    }
    seq_num_out.bits.req_num_msb = TRUE;
  }
  return (seq_num_out.val);
}

/***************************************************************************//**
Send Serial Message. This function interfaces with USB driver to send data to 
external device.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void send_serial_msg()
{
  while ((USB_CTL_TXSUSPENDTOKENBUSY_MASK & USB0_CTL) == USB_CTL_TXSUSPENDTOKENBUSY_MASK);
  
  uint8_t u8EndPointFlag = 0;
  uint16_t u16Length=0;
  tx_msg_len += 6;
  EP=10;
  /* Check if is a pending transfer */
  if(FLAG_CHK(fIN,gu8USBClearFlags))
  {
    gu8IN_Counter=tx_msg_len;       
    u16Length=(Setup_Pkt->wLength_h<<8)+Setup_Pkt->wLength_l ;
    if((u16Length < tx_msg_len) && (EP==2))
    {
      gu8IN_Counter=Setup_Pkt->wLength_l;
    }
  }
  /* Check transfer Size */
  if(gu8IN_Counter > cEP_Size[EP])
  {
    EPSize = cEP_Size[EP];
    gu8IN_Counter-=cEP_Size[EP];
    FLAG_CLR(fIN,gu8USBClearFlags);
  }
  else
  {
    EPSize = gu8IN_Counter;
    gu8IN_Counter=0;
    FLAG_SET(fIN,gu8USBClearFlags);
  } 
  
  /* Copy User buffer to EP buffer */
  tBDTtable[EP].Cnt=(EPSize);
  
  /* USB Flags Handling */
  if(FLAG_CHK(u8EndPointFlag,gu8USB_Toogle_flags))
  {
    tBDTtable[EP].Stat._byte= kUDATA0;
    FLAG_CLR(u8EndPointFlag,gu8USB_Toogle_flags);
  } 
  else
  {
    tBDTtable[EP].Stat._byte= kUDATA1;
    FLAG_SET(u8EndPointFlag,gu8USB_Toogle_flags);
  }
}

/*******************************************************************************
Heart Beat Command. This function prepares the response for heart beat message 
requested by external USB device.
@param pointer to a buffer
@return N/A
@outputs
@context
*******************************************************************************/
void heart_beat_cmd( uint8_t* ptemp_buff)
{
  heart_beat_msg.dev_status.bits.ready = TRUE;
  
  if (legacy_wand_params_obj.wand_connected)
  {
    heart_beat_msg.wand_info.bits.legacy_wand = TRUE;
    if(current_therapy_settings.active_wand.lw_active)
    {
      if(current_therapy_settings.active_wand.legacy_wand == SPORTS_MED)
      {
        heart_beat_msg.wand_info.bits.pri_wand = LEGACY_SM; //Legacy SM
      }
      else if(current_therapy_settings.active_wand.legacy_wand == ENT)
      {
        heart_beat_msg.wand_info.bits.pri_wand = NO_WAND; //LEGACY_ENT
      }
      heart_beat_msg.wand_info.bits.topaz_timer = current_therapy_settings.legacy_wand_levels.topaz_timer; 
      heart_beat_msg.wand_info.bits.reflex_timer = FALSE;
    }
  }
  else
  {
    heart_beat_msg.wand_info.bits.legacy_wand = FALSE;
  }  
  
  if ((current_therapy_settings.wand.valid))
  {
    heart_beat_msg.wand_info.bits.smart_wand = TRUE;
    if(!current_therapy_settings.active_wand.lw_active)
    {
      if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_SPORTS_MED)
      {
        heart_beat_msg.wand_info.bits.pri_wand = SMART_SM; //Smart wand
      }
      else if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_ENT)
      {
        heart_beat_msg.wand_info.bits.pri_wand = NO_WAND;  //SMART_ENT
      }
      else if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA)
      {
        heart_beat_msg.wand_info.bits.pri_wand = NO_WAND;  //SMART_ARTHROPLASTY
      }
      heart_beat_msg.wand_info.bits.topaz_timer = FALSE;
      heart_beat_msg.wand_info.bits.reflex_timer = FALSE;
    }
  }
  else
    heart_beat_msg.wand_info.bits.smart_wand = FALSE;
  
  if(!legacy_wand_params_obj.wand_connected && !current_therapy_settings.wand.valid)
  {
    heart_beat_msg.wand_info.val = 0;
  }
  
  heart_beat_msg.smartAbCoagSetPoint.bits.ablate_low_setpoint = current_therapy_settings.setpoint.lo;
  heart_beat_msg.smartAbCoagSetPoint.bits.ablate_med_setpoint = current_therapy_settings.setpoint.med;
  heart_beat_msg.smartAbCoagSetPoint.bits.ablate_hi_setpoint = current_therapy_settings.setpoint.hi;
  heart_beat_msg.smartAbCoagSetPoint.bits.coag_setpoint = current_therapy_settings.setpoint.coag;
  heart_beat_msg.AbCoag18SetPoint.bits.legacy_ablate_setpoint = legacy_wand_params_obj.ablate_level;
  heart_beat_msg.AbCoag18SetPoint.bits.legacy_coag_setpoint = legacy_wand_params_obj.coag_level;
  heart_beat_msg.therapy_set.bits.therapy_modes = current_therapy_settings.treatment.therapy_mode;
  heart_beat_msg.therapy_set.bits.ablate_status = current_therapy_settings.treatment.ablate_on;
  if(current_therapy_settings.wand.valid && !current_therapy_settings.active_wand.lw_active)
  {
    heart_beat_msg.therapy_set.bits.coag = smart_wand_params.mfg.coag_enable;
  }
  else if(legacy_wand_params_obj.wand_connected && current_therapy_settings.active_wand.lw_active)
  {
    heart_beat_msg.therapy_set.bits.coag = legacy_wand_params_obj.coag_enable;
  }
  else
  {
    heart_beat_msg.therapy_set.bits.coag                      = FALSE;        
  }
  heart_beat_msg.therapy_set.bits.coag_status = current_therapy_settings.treatment.coag_on;
  heart_beat_msg.therapy_set.bits.finger_switch_status = current_therapy_settings.treatment.finger_switch_en;
  heart_beat_msg.amb_thres_sp = current_therapy_settings.ambient.alarm_threshold;
  heart_beat_msg.gen_set.bits.gen_volume_level = current_therapy_settings.audio.volume;
  heart_beat_msg.gen_set.bits.prime_auto_manual = current_therapy_settings.treatment.flow_auto;
  heart_beat_msg.gen_set.bits.wired_foot_pedal_status = current_therapy_settings.footpedal.wired.present;
  heart_beat_msg.gen_set.bits.wireless_foot_pedal_enable = current_therapy_settings.treatment.wireless_enable;
  heart_beat_msg.gen_set.bits.wireless_foot_pedal_status = (current_therapy_settings.footpedal.wireless.present > 0)?1:0; 
  heart_beat_msg.amb_set.bits.color = current_therapy_settings.ambient.thermometer_color;
  heart_beat_msg.amb_set.bits.feature = (current_therapy_settings.ambient.capable & current_therapy_settings.ambient.enable_user_pref);
  heart_beat_msg.amb_set.bits.notification = current_therapy_settings.ambient.audio_enabled;
  heart_beat_msg.amb_set.bits.volume_level = current_therapy_settings.ambient.volume;
  heart_beat_msg.amb_temp = current_therapy_settings.ambient.smart_wand_temperature;
  heart_beat_msg.leg_amb_set.bits.capability = (legacy_wand_params_obj.wand_connected == TRUE)? legacy_wand_params_obj.ambient_capable: FALSE;
  heart_beat_msg.leg_amb_set.bits.color = current_therapy_settings.ambient.thermometer_color;
  heart_beat_msg.leg_amb_set.bits.capability_smart = (current_therapy_settings.wand.valid == TRUE)? smart_wand_params.mfg.ambient_enable: FALSE;
  heart_beat_msg.leg_amb_set.bits.reserved = 0;
  heart_beat_msg.leg_amb_temp = current_therapy_settings.ambient.legacy_temperature;
  if((current_therapy_settings.active_wand.lw_active == SPORTS_MED) ||
     (current_therapy_settings.controller == SPORTS_MED) && (smart_wand_params.mfg.indication == 0) ||
        (current_therapy_settings.controller == UNKNOWN_GEN))
  {
    heart_beat_msg.priority = current_therapy_settings.notify.status & 0xFF;
  }
  else if((current_therapy_settings.controller == ENT) || 
          (smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA))
  {
    heart_beat_msg.priority = 0;
  }
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = CAPDEV_WW_HEARTBEAT;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = HB_WCD_BDM_DATA_LENGTH;
  *ptemp_buff++ = tx_msg_len;
  *ptemp_buff++ = heart_beat_msg.dev_status.val;
  *ptemp_buff++ = heart_beat_msg.wand_info.val; 
  *ptemp_buff++ = heart_beat_msg.smartAbCoagSetPoint.val;
  *ptemp_buff++ = heart_beat_msg.AbCoag18SetPoint.val;
  *ptemp_buff++ = heart_beat_msg.therapy_set.val;
  *ptemp_buff++ = heart_beat_msg.amb_thres_sp;
  *ptemp_buff++ = heart_beat_msg.gen_set.val;
  *ptemp_buff++ = heart_beat_msg.amb_set.val;
  *ptemp_buff++ = heart_beat_msg.amb_temp;
  *ptemp_buff++ = heart_beat_msg.leg_amb_set.val;
  *ptemp_buff++ = heart_beat_msg.leg_amb_temp;
  *ptemp_buff++ = heart_beat_msg.priority;
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE;
}

/***************************************************************************//**
Capital Device Discovery Command. This function prepares the response for capital
device discovery message requested by external USB device.
@param pointer to a buffer
@return N/A
@outputs
@context
*******************************************************************************/
void cap_disc_cmd(uint8_t* ptemp_buff)
{
  cap_disc_msg.version_number = PROT_VERSION;
  cap_disc_msg.device_state = isWWReady;
  cap_disc_msg.devType = COBLATION;
  cap_disc_msg.devSubType = WEREWOLF;
  
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = CAPDEV_WW_DISCOVERY;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = CAPDEV_TO_BDM_DISCOVERY_RESP_LENGTH;
  *ptemp_buff++ = tx_msg_len;
  *ptemp_buff++ = cap_disc_msg.version_number;
  *ptemp_buff++ = cap_disc_msg.device_state;
  *ptemp_buff++ = cap_disc_msg.devType;
  *ptemp_buff++ = cap_disc_msg.devSubType;
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE;
}

/***************************************************************************//**
Port Status Command. This function prepares the response for port status message.
@param pointer to a buffer
@return N/A
@outputs
@context
*******************************************************************************/
void port_status_cmd(uint8_t cmd, uint8_t* ptemp_buff)
{  
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = CAPDEV_WW_PORT_STATUS;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = WCD_PORT_STATUS_DATA_LEN;
  *ptemp_buff++ = tx_msg_len;
  *ptemp_buff++ = port_status_msg.foot_info.val;
  *ptemp_buff++ = port_status_msg.wand_info.val;
  *ptemp_buff++ = port_status_msg.legacy_therapy_max.val;
  *ptemp_buff++ = port_status_msg.devMode;
  *ptemp_buff++ = port_status_msg.language_id;
  *ptemp_buff++ = port_status_msg.wand_cap_info.val;
  
  memcpy(ptemp_buff,&port_status_msg.wand_name, WAND_NAME_CHARS);
  for (u8 i = 0;i < WAND_NAME_CHARS; i++)
  {
    *ptemp_buff++;
  }
  memcpy(ptemp_buff,&port_status_msg.wand_cat_num, WAND_CAT_NUM_CHARS);
  for (u8 j = 0;j < WAND_CAT_NUM_CHARS; j++)
  {
    *ptemp_buff++;
  }
  memcpy(ptemp_buff,&port_status_msg.wand_lot_num, sizeof(u32));
  for (u8 k = 0; k < sizeof(u32); k++)
  {
    *ptemp_buff++;
  }
  memcpy(ptemp_buff,&port_status_msg.wand_id_num, sizeof(u16));
  for (u8 l = 0; l < sizeof(u16); l++)
  {
    *ptemp_buff++;
  }
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE;
}

/***************************************************************************//**
This function checks if reflex timer values recieved from external device can bet set?
@param void
@return bool
@outputs
@context
*******************************************************************************/
bool reflex_ent_timer_avaliable(void)
{
  return (bool)((current_therapy_settings.controller == ENT) &&
                (current_therapy_settings.legacy_wand_levels.topaz_timer == 1));
}



/***************************************************************************//**
This function sets the controller with therapy settings received from external 
USB device.
@param uint8_t, *uint8_t
@return void
@outputs
@context
*******************************************************************************/
void set_therapy_cmd(uint8_t cmd, uint8_t* ptemp_buff)
{
  
  set_therapy_msg.therapy_set.val = data_in.cmd_data[2];
  set_therapy_msg.amb_thres_sp = data_in.cmd_data[3];
  set_therapy_msg.pump_settings.val = data_in.cmd_data[4];
  set_therapy_msg.amb_settings.val = data_in.cmd_data[5];
  set_therapy_msg.gen_settings.val = data_in.cmd_data[6];
  
  if (current_therapy_settings.wand.unlocked == TRUE)
  {
    set_therapy_msg.smartAbCoagSetPoint.val = data_in.cmd_data[0];
  }  
  
  if (legacy_wand_params_obj.wand_connected == TRUE)
  {
    set_therapy_msg.AbCoag18SetPoint.val = data_in.cmd_data[1];
  }
  
  if (reflex_ent_timer_avaliable())
  {
    set_therapy_msg.reflex_timer.val = data_in.cmd_data[7];
  }
  
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = WCD_TO_BDM_SET_THERAPY_RESPONSE;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = ONE_BYTE_ACK_RESPONSE_DATA_LEN;
  *ptemp_buff++ = tx_msg_len;
  *ptemp_buff++ = ACK;
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE;
}

/***************************************************************************//**
This function prepares response for therapy settings requested by external USB device.
@param uint8_t, *uint8_t
@return void
@outputs
@context
*******************************************************************************/
void get_therapy_cmd(uint8_t cmd, uint8_t* ptemp_buff)
{
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = CAPDEV_WW_THERAPY_SETTINGS_STATUS;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = WCD_THERAPY_SETTINGS_DATA_LEN;
  *ptemp_buff++ = tx_msg_len; 
  *ptemp_buff++ = get_therapy_msg.smartAbCoagSetPoint.val;
  *ptemp_buff++ = get_therapy_msg.AbCoag18SetPoint.val;
  *ptemp_buff++ = get_therapy_msg.therapy_set.val;
  *ptemp_buff++ = get_therapy_msg.amb_thres_sp;
  *ptemp_buff++ = get_therapy_msg.pump_settings.val;
  *ptemp_buff++ = get_therapy_msg.amb_settings.val;
  *ptemp_buff++ = get_therapy_msg.gen_settings.val;
  *ptemp_buff++ = get_therapy_msg.reflex_timer.val;
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE;  
}

/*******************************************************************************
This function prepares response for error priority and notification information 
requested by external USB device.
@param received command ID, pointer to USB output buffer
@return void
@outputs
@context
*******************************************************************************/
void error_info_get_cmd(uint8_t cmd, uint8_t* ptemp_buff)
{
  error_info_msg.priority = current_therapy_settings.notify.status & 0xFF;
  error_info_msg.misc_errors.bits.uis_touch_screen = current_therapy_settings.gui.faults.touch;
  
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = cmd;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = WCD_ERROR_STATUS_DATA_LEN;
  *ptemp_buff++ = tx_msg_len;
  *ptemp_buff++ = error_info_msg.priority;
  *ptemp_buff++ = error_info_msg.misc_errors.val;
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE; 
}

/*******************************************************************************
This function prepares response for ambient sensor information requested by 
external USB device.
@param received command ID, pointer to USB output buffer
@return void
@outputs
@context
*******************************************************************************/
void amb_get_cmd(uint8_t cmd, uint8_t* ptemp_buff)
{
  amb_info_msg.amb_set.bits.color = current_therapy_settings.ambient.thermometer_color;
  amb_info_msg.amb_set.bits.feature = (current_therapy_settings.ambient.capable & current_therapy_settings.ambient.enable_user_pref);
  amb_info_msg.amb_set.bits.notification = current_therapy_settings.ambient.audio_enabled;
  amb_info_msg.amb_set.bits.volume_level = current_therapy_settings.ambient.volume;
  amb_info_msg.amb_temp = current_therapy_settings.ambient.smart_wand_temperature;
  amb_info_msg.amb_thresh_sp = current_therapy_settings.ambient.alarm_threshold;
  amb_info_msg.handle_temp = convert_q62_ambient_to_7bit(current_therapy_settings.wand.handle_temp);
  amb_info_msg.leg_amb_set.bits.capability = (legacy_wand_params_obj.wand_connected == TRUE)? legacy_wand_params_obj.ambient_capable: FALSE;
  amb_info_msg.leg_amb_set.bits.capability_smart = (current_therapy_settings.wand.valid == TRUE)? smart_wand_params.mfg.ambient_enable: FALSE;
  amb_info_msg.leg_amb_set.bits.color = current_therapy_settings.ambient.thermometer_color;
  amb_info_msg.leg_amb_temp = current_therapy_settings.ambient.legacy_temperature;
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = cmd;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = WCD_AMBIENT_STATUS_DATA_LEN;
  *ptemp_buff++ = tx_msg_len;
  *ptemp_buff++ = amb_info_msg.amb_set.val;
  *ptemp_buff++ = amb_info_msg.amb_temp;
  *ptemp_buff++ = amb_info_msg.handle_temp;
  *ptemp_buff++ = amb_info_msg.amb_thresh_sp;
  *ptemp_buff++ = amb_info_msg.leg_amb_set.val;
  *ptemp_buff++ = amb_info_msg.leg_amb_temp;
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE; 
}

/*******************************************************************************
This function prepares response for wand life information requested by external 
USB device.
@param received command ID, pointer to USB output buffer
@return void
@outputs
@context
*******************************************************************************/
void wand_life_info_get_cmd(uint8_t cmd, uint8_t* ptemp_buff)
{
  wand_life_msg.wand_life_low = smart_wand_params.life.lo;
  wand_life_msg.wand_life_med = smart_wand_params.life.med;
  wand_life_msg.wand_life_high = smart_wand_params.life.hi;
  wand_life_msg.wand_life_vac = smart_wand_params.life.vac;
  wand_coag_life(&db.circle_log.mode_active_accum[0],&static_log.original_mode_active_time[0]);
  wand_life_msg.wand_life_coag = smart_wand_coag_life;
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = cmd;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = WAND_LIFE_INFO_DATA_LEN;
  *ptemp_buff++ = tx_msg_len;
  memcpy(ptemp_buff,&wand_life_msg.wand_life_low,sizeof(u16));
  *ptemp_buff++; 
  *ptemp_buff++;
  memcpy(ptemp_buff,&wand_life_msg.wand_life_med,sizeof(u16));
  *ptemp_buff++; 
  *ptemp_buff++;
  memcpy(ptemp_buff,&wand_life_msg.wand_life_high,sizeof(u16));
  *ptemp_buff++; 
  *ptemp_buff++;
  memcpy(ptemp_buff,&wand_life_msg.wand_life_vac,sizeof(u16));
  *ptemp_buff++; 
  *ptemp_buff++;
  memcpy(ptemp_buff,&wand_life_msg.wand_life_coag,sizeof(u16));
  *ptemp_buff++; 
  *ptemp_buff++;
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE; 
}

/*******************************************************************************
This function prepares response for generator information like serial number, 
software version,etc,information requested by external USB device.
@param received command ID, pointer to USB output buffer
@return void
@outputs
@context
*******************************************************************************/
void gen_serial_get_cmd(uint8_t cmd, uint8_t* ptemp_buff)
{
  current_therapy_settings.sw.app1_version_major = pheader->meta.app_version_major;
  current_therapy_settings.sw.app1_version_minor = pheader->meta.app_version_minor;
  memcpy(&gen_info_msg.serialNum,&serial_number,MFG_SERIAL_NUM_CHARS);
  memcpy(&gen_info_msg.born_date,&mfg_date_time,(MFG_TIME_NUM_CHARS-1));
  gen_info_msg.app_version_major = current_therapy_settings.sw.app1_version_major;
  gen_info_msg.app_version_minor = current_therapy_settings.sw.app1_version_minor;
  gen_info_msg.gui_version_major = current_therapy_settings.sw.app2_version_major;
  gen_info_msg.gui_version_minor = current_therapy_settings.sw.app2_version_minor;
  
  *ptemp_buff++ = PROTOCOL_WCD_TO_BDM;
  *ptemp_buff++ = cmd;
  *ptemp_buff++ = getNextTxSeqSerial();
  tx_msg_len = GENERATOR_SERIAL_INFO_DATA_LEN;
  *ptemp_buff++ = tx_msg_len;
  memcpy(ptemp_buff,&gen_info_msg.serialNum, MFG_SERIAL_NUM_CHARS);
  for (u8 i = 0;i < MFG_SERIAL_NUM_CHARS; i++)
  {
    *ptemp_buff++;
  }
  memcpy(ptemp_buff,&gen_info_msg.born_date, sizeof(u32));
  for (u8 j = 0;j < sizeof(u32); j++)
  {
    *ptemp_buff++;
  }
  *ptemp_buff++ = gen_info_msg.app_version_major;
  *ptemp_buff++ = gen_info_msg.app_version_minor;
  *ptemp_buff++ = gen_info_msg.gui_version_major;
  *ptemp_buff++ = gen_info_msg.gui_version_minor;
  
  uint8_t temp;
  temp = packet_cksm((tx_msg_len+HEADER_SIZE_WCD_MSGS),ptemp_buff);
  *ptemp_buff++ = temp;
  *ptemp_buff++ = FOX_CHARLIE;
}

/*******************************************************************************
This function determines NAK priority information in case there are more than 1
errors found in an incoming message from external USB device.
@param previously calculated nack mask
@return error priority
@outputs
@context
*******************************************************************************/
ACK_NACK_ID_T nack_priority(uint32_t nack)
{
  
  uint8_t priority = 0;
  
  /* this list is sorted by pre-determined priority starting w/ lowest */
  const ACK_NACK_ID_T err_id_lookup [] =
  {
    ACK_ID,                          /* 0 */
    GENERAL_ERR_NAK_ID,              /* 1 */
    NOT_APPLICABLE_ERR_NAK_ID,       /* 2 */
    BUSY_ERR_NAK_ID,                 /* 3 */
    CMD_LENGTH_ERR_NAK_ID,           /* 4 */
    CMD_ID_ERR_NAK_ID,               /* 5 */
    CHECKSUM_ERR_NAK_ID,             /* 6 */
    FRAMING_ERR_NAK_ID,              /* 7 */
    PROTOCOL_ERR_NAK_ID,             /* 8 */
  };
  
  /* determine highest priority nack */
  while (nack>1)
  {
    nack >>= 1;
    priority++;
  }
  
  /* boundary check in case priority somehow exceeds max */
  if (priority >= HIGHEST_PRIOIRTY_NAK_ERR)
  {
    priority = (uint8_t)ACK;
  }
  
  return err_id_lookup[priority];
  
}

/*******************************************************************************
This function prepares the NAK response for an incoming message from external USB device.
@param pointer to output buffer, Incoming command ID, previously calculated nack mask
@return highest priority NAK
@outputs
@context
*******************************************************************************/
ACK_NACK_ID_T nak_cmd(uint8_t* pbuff, uint8_t cmd, uint32_t nack)
{
  
  ACK_NACK_ID_T priority = nack_priority(nack);
  
  REPLY_PACKET_HEADER_T header  = 
  {
    .pid = PROTOCOL_WCD_TO_BDM,
    .cmd = cmd,
    .seq = getNextTxSeqSerial(),
    .length = ACK_NAK_SIZE,
  };
  
  REPLY_PACKET_TAIL_T tail = 
  {
    .crc = 0,
    .eol = FOX_CHARLIE,
  };
  
  /* look-up id of highest priority nack */
  ACK_NACK_DETAIL_T data =
  {
    .ack_nack_detail = priority,
  };
  
  /* send_serial_msg() already adds 6 bytes to every packet */
  tx_msg_len = header.length;
  /* prepare response */
  *pbuff = header.pid;
  tail.crc += *pbuff++;
  *pbuff = header.cmd;
  tail.crc += *pbuff++;
  *pbuff = header.seq;
  tail.crc += *pbuff++;
  *pbuff = header.length;
  tail.crc += *pbuff++;
  *pbuff = data.ack_nack_detail;
  tail.crc += *pbuff++;
  tail.crc = ~(tail.crc) + 1;
  *pbuff++ = tail.crc;
  *pbuff = tail.eol;  
  
  return data.ack_nack_detail;
}

/*******************************************************************************
This function calculates the checksum for an outgoing message to external USB device.
@param size of the packet(header + data), temporary buffer pointer
@return calculated checksum
@outputs
@context
*******************************************************************************/
uint8_t packet_cksm(uint8_t size, uint8_t *buff_4)
{
  u8 sum = 0;
  while(size-- > 0)
  {
    sum += *(--buff_4);
  }
  return (~sum+1);
}

/*******************************************************************************
This function sets "setup blob" settings received from external USB device.
@param temporary buffer pointer, calculated nack error code
@return void
@outputs
@context
*******************************************************************************/
void set_setup_blob_cmd(uint8_t* pbuff, uint32_t nack)
{
  static u8 set_blob[48];
  static u8 set_blob_head[sizeof(SETUP_BLOB_HEADER_T)];
  static u8 checksum = 0;
  static struct Blob_Settings
  {
    u8 master_volume;
    u8 ambient_volume;
    u8 language;
    u8 wireless_enable;
  } blob_settings;
  
  SETUP_BLOB_CMD_DATA_T cmd = 
  {
    .page = data_in.cmd_data[0],
    .data = (u8*)&data_in.cmd_data[1],
  };
  
  REPLY_PACKET_HEADER_T header  = 
  {
    .pid = PROTOCOL_WCD_TO_BDM,
    .cmd = CAPDEV_WW_SET_CONFIG_BLOB,
    .seq = getNextTxSeqSerial(),
    .length = sizeof(BLOB_REPLY_T),
  };
  
  BLOB_REPLY_T reply_data = 
  {
    .page = cmd.page,
    .ack = (uint8_t)nack_priority(nack),
  };
  
  REPLY_PACKET_TAIL_T tail = 
  {
    .crc = 0,
    .eol = FOX_CHARLIE,
  };
  
  if (reply_data.ack != 0)
  {
    cmd.page = 0xFF;
  }
  
  if (cmd.page == 1)
  {
    for (int i = 0; i < sizeof(set_blob); i++)
    {
      set_blob[i] = 0;
    }
    
    for (int i = 0; i < sizeof(set_blob_head); i++)
    {
      set_blob_head[i] = ((u8*)cmd.data)[i];
    }    
    
    checksum = 0;
  }
  else if (cmd.page == 2)
  {
    for (int i = 0; i < sizeof(set_blob); i++)
    {
      set_blob[i] = ((u8*)cmd.data)[i];
      checksum += set_blob[i];
    }
    
    blob_settings.master_volume = set_blob[4];
    blob_settings.ambient_volume = set_blob[5];
    blob_settings.language = set_blob[46];    
  }
  else if (cmd.page <= 30)
  {
    /* we only need page 1 so we can ignore remaining pages for now */
    for (int i = 0; i < sizeof(set_blob); i++)
    {
      set_blob[i] = ((u8*)cmd.data)[i];
      checksum += set_blob[i];
    }
  }
  
  if (cmd.page == 30)
  {
    blob_settings.wireless_enable = set_blob[22];
    
    checksum -= set_blob[47];
    checksum = ~(checksum) + 1;
    if (checksum == set_blob[47])
    {
      if ((blob_settings.master_volume <= 10) && (blob_settings.master_volume > 0))
      {
        current_therapy_settings.audio.volume = blob_settings.master_volume;
      }
      
      if ((blob_settings.ambient_volume <= 10) && (blob_settings.ambient_volume > 0))
      {
        current_therapy_settings.ambient.volume = blob_settings.ambient_volume;
      }
      
      if (blob_settings.language < MAX_NUM_LANGUAGES)
      {
        current_therapy_settings.gui.languageID = (SUPPORTED_LANGUAGES) blob_settings.language;
      }
      
      if ((blob_settings.wireless_enable <= 1) && 
          (current_therapy_settings.footpedal.wireless.married_latch == 1))
      {
        current_therapy_settings.treatment.wireless_enable = (bool)blob_settings.wireless_enable;
      }
    }
  }  
  /* send_serial_msg() already adds 6 bytes to every packet */
  tx_msg_len = header.length;
  /* prepare response */
  *pbuff = header.pid;
  tail.crc += *pbuff++;
  *pbuff = header.cmd;
  tail.crc += *pbuff++;
  *pbuff = header.seq;
  tail.crc += *pbuff++;
  *pbuff = header.length;
  tail.crc += *pbuff++;
  *pbuff = reply_data.page;
  tail.crc += *pbuff++;
  *pbuff = reply_data.ack;
  tail.crc += *pbuff++;
  tail.crc = ~(tail.crc) + 1;
  *pbuff++ = tail.crc;
  *pbuff = tail.eol;  
  
}

/*******************************************************************************
This function sends "setup blob" settings requested by external USB device.
@param temporary buffer pointer, requested page number
@return void
@outputs
@context
*******************************************************************************/
void get_setup_blob_cmd(uint8_t* pbuff, u8 page)
{
  u8 page_size = 0;
  static u32 nv_data_address = 0;
  
  REPLY_PACKET_HEADER_T header  = 
  {
    .pid = PROTOCOL_WCD_TO_BDM,
    .cmd = CAPDEV_WW_GET_CONFIG_BLOB,
    .seq = getNextTxSeqSerial(),
    .length = sizeof(SETUP_BLOB_CMD_DATA_T) - 4, // -4 b/c SETUP_BLOB_CMD_DATA_T contains a ptr
  };
  
  SETUP_BLOB_CMD_DATA_T cmd = 
  {
    .page = page,
    .data = NULL,
  };
  REPLY_PACKET_TAIL_T tail = 
  {
    .crc = 0,
    .eol = FOX_CHARLIE,
  };
  
  const SETUP_BLOB_HEADER_T blob_head_data = 
  {
    .version = 0x10,
    .length_head = 0x17,
    .length_data = 0x6F05,
    .type = 0x03,
    .sub_type = 0x01,
    .name = {0x57,0x45,0x52,0x45,0x57,0x4F,0x4C,0x46,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    .crc = 0x0C,
  };
  
  if (page == 1)
  {
    struct Data_Out
    {
      REPLY_PACKET_HEADER_T header;
      struct Cmd
      {
        u8 page;
        u8 data[23];
      } cmd;
      REPLY_PACKET_TAIL_T tail;
    } * const p_data_out = (struct Data_Out*) pbuff;
    
    cmd.data = (SETUP_BLOB_HEADER_T*)&blob_head_data;
    header.length += blob_head_data.length_head;
    page_size = sizeof(SETUP_BLOB_HEADER_T);
    
    p_data_out->header.pid = header.pid;
    tail.crc += p_data_out->header.pid;
    p_data_out->header.cmd = header.cmd;
    tail.crc += p_data_out->header.cmd;
    p_data_out->header.seq = header.seq;
    tail.crc += p_data_out->header.seq;
    p_data_out->header.length = header.length;
    tail.crc += p_data_out->header.length;
    p_data_out->cmd.page = cmd.page;
    tail.crc += p_data_out->cmd.page;
    for (u8 i = 0; i < (page_size); i++)
    {
      p_data_out->cmd.data[i] = ((u8*)cmd.data)[i];
      tail.crc += p_data_out->cmd.data[i];
    }
    tail.crc = ~(tail.crc) + 1;
    p_data_out->tail.crc = tail.crc;
    p_data_out->tail.eol = tail.eol;
    
    /* load data we want to send */
    nv_data_address = (uint32_t) get_cached_nv_data();
    
  }
  else
  {
    
    struct Data_Out
    {
      REPLY_PACKET_HEADER_T header;
      struct Cmd
      {
        u8 page;
        u8 data[48];
      } cmd;
      REPLY_PACKET_TAIL_T tail;
    } * const p_data_out = (struct Data_Out*) pbuff;
    
    header.length += 48;
    page_size = 48;
    if (nv_data_address == 0)
    {
      /* re-load data we want to send */
      nv_data_address = (uint32_t) get_cached_nv_data();
    }
    cmd.data = (u8*)(nv_data_address + ((48) * (page - 2)));
    
    if (page == MAX_BLOB_PAGES)
    {
      u8 checksum = 0;
      const u8 *p_nv_data = (u8*)(nv_data_address);
      const u8 *p_nv_data_eof = (u8*)(nv_data_address + 0x56F);
      while (p_nv_data < p_nv_data_eof)
      {
        checksum += *p_nv_data++;
      }
      checksum = ~(checksum) + 1;
      ((u8*)cmd.data)[47] = checksum;
    }
    
    p_data_out->header.pid = header.pid;
    tail.crc += p_data_out->header.pid;
    p_data_out->header.cmd = header.cmd;
    tail.crc += p_data_out->header.cmd;
    p_data_out->header.seq = header.seq;
    tail.crc += p_data_out->header.seq;
    p_data_out->header.length = header.length;
    tail.crc += p_data_out->header.length;
    p_data_out->cmd.page = cmd.page;
    tail.crc += p_data_out->cmd.page;
    for (u8 i = 0; i < (page_size); i++)
    {
      p_data_out->cmd.data[i] = ((u8*)cmd.data)[i];
      tail.crc += p_data_out->cmd.data[i];
    }        
    tail.crc = ~(tail.crc) + 1;
    p_data_out->tail.crc = tail.crc;
    p_data_out->tail.eol = tail.eol;
  }
    
  /* send_serial_msg() already adds 6 bytes to every packet */
  tx_msg_len = header.length;// + sizeof(REPLY_PACKET_HEADER_T) + sizeof(REPLY_PACKET_TAIL_T); 
  
}

/*******************************************************************************
This function prepares and sends the command response requested by external USB device.
@param requested command ID, output buffer pointer
@return uint8_t
@outputs
@context
*******************************************************************************/
uint8_t prepareBdmResponse(uint8_t command, uint8_t* bufferOut)
{
  
  switch (command)
  {
  case (CAPDEV_WW_DISCOVERY):
    cap_disc_cmd(bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_HEARTBEAT):
    if(!sendPortStatus && !sendTherapyStatus) //ignoring heart beat messages until port/therapy status sent
    {
      heart_beat_cmd(bufferOut);
      send_serial_msg();   
    }
    return 1;
    break;
  case (CAPDEV_WW_PORT_STATUS):  
    port_status_cmd(CAPDEV_WW_PORT_STATUS,bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_GET_PORT_STATUS):
    port_status_cmd(CAPDEV_WW_PORT_STATUS,bufferOut);
    port_status_cmd(CAPDEV_WW_GET_PORT_STATUS,bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_THERAPY_SETTINGS_STATUS):
    get_therapy_cmd(CAPDEV_WW_THERAPY_SETTINGS_STATUS,bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_GET_THERAPY_SETTINGS_STATUS):
    get_therapy_cmd(CAPDEV_WW_GET_THERAPY_SETTINGS_STATUS,bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_SET_THERAPY_SETTINGS):
    set_therapy_cmd(CAPDEV_WW_SET_THERAPY_SETTINGS,bufferOut);
    send_serial_msg();
    update_therapy_settings();
    return 1;
    break;
  case (CAPDEV_WW_GET_ERROR_INFO):
    error_info_get_cmd(CAPDEV_WW_GET_ERROR_INFO,bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_GET_AMBIENT_SETTINGS):
    amb_get_cmd(CAPDEV_WW_GET_AMBIENT_SETTINGS,bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_GET_WAND_LIFE_INFO):
    wand_life_info_get_cmd(CAPDEV_WW_GET_WAND_LIFE_INFO,bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_GET_GENERATOR_SERIAL_INFO):
    gen_serial_get_cmd(CAPDEV_WW_GET_GENERATOR_SERIAL_INFO,bufferOut);
    send_serial_msg();
    return 1;
    break;
  case (CAPDEV_WW_GET_CONFIG_BLOB):
    {
      BLOB_REPLY_T *p_blob_request = (BLOB_REPLY_T*)(data_in.cmd_data);
      if (p_blob_request->page <= MAX_BLOB_PAGES)
      {
        get_setup_blob_cmd(bufferOut, p_blob_request->page);
#ifdef DEBUG
        printf("Language:(%d)\n",current_therapy_settings.gui.languageID);
        printf("Wireless FP:(%d)\n",current_therapy_settings.treatment.wireless_enable);
#endif
      }
      else
      {
        return 0;
        break;
      }
      send_serial_msg();
      return 1;
    }
  case (CAPDEV_WW_SET_CONFIG_BLOB):
    set_setup_blob_cmd(bufferOut,0);
#ifdef DEBUG
    printf("Language:(%d)\n",current_therapy_settings.gui.languageID);
    printf("Wireless FP:(%d)\n",current_therapy_settings.treatment.wireless_enable);
#endif
    send_serial_msg();
    return 1;
  default:
    nak_cmd(bufferOut, data_in.command_id, NACK_ERR_MASK(CMD_ID_ERR_NAK));
    send_serial_msg();
    return 0;
    break;
  }
}

/*******************************************************************************
This message parses and processes the message received from external USB device.
@param void
@return void
@outputs
@context
*******************************************************************************/
void in_bdm_wcd_msg ()
{
  uint32_t nack_mask = 0;
  bdm_msg_in = BufferPointer[12];
  bdm_msg_out = BufferPointer[10];
  static uint8_t cmd,data_in_crc,data_in_end_byte;
  static uint8_t packet_in_size;
  static bdm_to_capdev_discovery_t bdm_ww_cap_discovery_data;
  
  if(*bdm_msg_in == PROTOCOL_BDM_TO_WCD)
  {
    uint32_t nack_mask = 0;
    data_in.protocol_id = *bdm_msg_in++;
    data_in.command_id = *bdm_msg_in++;
    data_in.cmd_sequence_number.val = *bdm_msg_in++;
    data_in.cmd_data_len = *bdm_msg_in++;
    
    if ((data_in.command_id > CAPDEV_WW_GENERIC_REPLY) &&
        (data_in.command_id < MAX_CAPDEV_WW_CMD))
    {
      uint8_t cmd_data_len [] = 
      {
        3, /* 30 General Request */
        1, /* 31 Discovery Request */
        1, /* 32 Heart Beat Status */
        1, /* 33 Port Status Reply */
        0, /* 34 Get Port Status */
        8, /* 35 Set Therapy Settings */
        1, /* 36 Therapy Settings ACK/NACK */
        0, /* 37 Get Therapy Settings */
        0, /* 38 Error Notifiation */
        0, /* 39 Ambient Settings */
        0, /* 3A Wand Active Life Info */
        0, /* 3B Get Dev Info */
        1, /* 3C Device Blob Get */
        //24, /* 3D Set Device Blob Page 1 */
        //49, /* 3D Set Device Blob Page 2...n */
      };
      
      if (data_in.command_id == CAPDEV_WW_SET_CONFIG_BLOB)
      {
        uint8_t page = (uint8_t)*bdm_msg_in;
        
        /* special case b/c length depends on cmd data */
        if ((data_in.cmd_data_len != 24) &&
            (page == 1))
        {
          nack_mask |= NACK_ERR_MASK(CMD_LENGTH_ERR_NAK);
          /* length err so override value sent */
          data_in.cmd_data_len = 24;
        }
        else if ((data_in.cmd_data_len != 49) &&
                 (page > 1))
        {
          nack_mask |= NACK_ERR_MASK(CMD_LENGTH_ERR_NAK);
          /* length err so override value sent */
          data_in.cmd_data_len = 49;
        }         
      }
      else 
      {
        uint8_t len = cmd_data_len[(data_in.command_id - CAPDEV_WW_GENERIC_REPLY)];
        if (len != data_in.cmd_data_len)
        {
          nack_mask |= NACK_ERR_MASK(CMD_LENGTH_ERR_NAK);
          /* length err so override value sent */
          data_in.cmd_data_len = len;
        }
      }
    }
    else
    {
      data_in.cmd_sequence_number.bits.request_num = 0;
      nack_mask |= NACK_ERR_MASK(CMD_ID_ERR_NAK);
    }
    
    for (u8 i = 0; i < data_in.cmd_data_len; i++)
    {
      data_in.cmd_data[i] = *bdm_msg_in++;
    }
    packet_in_size = data_in.cmd_data_len + HEADER_SIZE_WCD_MSGS;
    bdm_rx_pack_cksm = packet_cksm(packet_in_size, bdm_msg_in);
    data_in_crc = *bdm_msg_in++;
    data_in_end_byte = *bdm_msg_in;
    
    if (data_in_end_byte != FOX_CHARLIE) 
    {
      nack_mask |= NACK_ERR_MASK(FRAMING_ERR_NAK);
    }
    
    if (data_in_crc != bdm_rx_pack_cksm)
    {
      nack_mask |= NACK_ERR_MASK(CHECKSUM_ERR_NAK);
    }
    
    if (nack_mask != 0)
    {
      if(isWWConnected)
      {
        packet_in_valid = FALSE;
        /* blob is special b/c it has one more byte in ack reply */
        if (data_in.command_id == CAPDEV_WW_SET_CONFIG_BLOB)
        {
          set_setup_blob_cmd(bdm_msg_out, nack_mask);
          send_serial_msg();
        }
        else
        {
#ifdef DEBUG
          printf("Nack Mask: (%d)\n",nack_mask);
#endif
          if(data_in.command_id != CAPDEV_WW_GENERIC_REPLY)
          {
            nak_cmd(bdm_msg_out, data_in.command_id, nack_mask);
            nack_mask = 0;
            send_serial_msg();
          }
        }
      }
    }
    else
    {
      packet_in_valid = TRUE;
      incoming_bdm_msg = TRUE;
    }
    
  }
  else
  {
    if(isWWConnected)
    {
      bdm_msg_in++;
      data_in.command_id = *bdm_msg_in++;
      data_in.cmd_sequence_number.val = *bdm_msg_in++;
      nack_mask |= NACK_ERR_MASK(PROTOCOL_ERR_NAK);
      nak_cmd(bdm_msg_out, data_in.command_id, nack_mask);
      send_serial_msg();
    }
  }
  if (incoming_bdm_msg)
  {
    memset(bdm_msg_out,0,EP2_SIZE);
    bool therapy_on = FALSE;
    
    if ((current_therapy_settings.treatment.ablate_on == 1) ||
        (current_therapy_settings.treatment.coag_on == 1))
    {
      therapy_on = TRUE;
    }
    
    incoming_bdm_msg = FALSE;
    if((data_in.command_id == CAPDEV_WW_DISCOVERY) && data_in.cmd_sequence_number.bits.req_num_msb)
    {
      bdm_ww_cap_discovery_data.version_number = *data_in.cmd_data;
      isWWReady = TRUE;
      prepareBdmResponse(WCD_TO_BDM_DISCOVERY_RESPONSE, bdm_msg_out);
      sendDevInfoStatus = TRUE;
      sendPortStatus = FALSE;
      sendTherapyStatus = FALSE;
      isWWConnected = FALSE;
#ifdef DEBUG
      printf("Received Discovery Message...\n");
#endif
    }
    if(isWWConnected)
    {
      if ((data_in.command_id == BDM_TO_WCD_HEARTBEAT_CMD) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
        prepareBdmResponse(WCD_TO_BDM_HEARTBEAT_CMD, bdm_msg_out);        
      }
      else if((data_in.command_id == BDM_TO_WCD_PORT_STATUS_RESP) && !data_in.cmd_sequence_number.bits.req_num_msb)
      {
        port_status_ack_received = *data_in.cmd_data; 
        if(port_status_ack_received == 0)
        {
          wait_for_port_status_ack = FALSE;
          timer_port_status_ack_value = 0;
#ifdef DEBUG
          printf("Received Port Status ACK:(%d)\n",retry_cnt_port_status_ack);
#endif
        }
        else
        {
#ifdef DEBUG
          printf("Received Port Status NACK\n");
#endif
        }
      }
      else if((data_in.command_id == BDM_TO_WCD_GET_PORT_STATUS) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
#ifdef DEBUG
        printf("Received Get Port Status Request...\n");
#endif
        prepareBdmResponse(WCD_TO_BDM_GET_PORT_STATUS_RESP, bdm_msg_out);
      }
      else if((data_in.command_id == BDM_TO_WCD_SET_THERAPY_SETTINGS) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
        if (therapy_on == TRUE)
        {
          nak_cmd(bdm_msg_out, data_in.command_id, NACK_ERR_MASK(BUSY_ERR_NAK));
          send_serial_msg();
        }
        else
        {
          prepareBdmResponse(WCD_TO_BDM_SET_THERAPY_RESPONSE, bdm_msg_out);
        }
      }
      else if((data_in.command_id == BDM_TO_WCD_GET_THERAPY_SETTINGS) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
        prepareBdmResponse(WCD_TO_BDM_GET_THERAPY_RESPONSE, bdm_msg_out);
      }
      else if((data_in.command_id == BDM_TO_WCD_THERAPY_STATUS_RESPONSE) && !data_in.cmd_sequence_number.bits.req_num_msb)
      {
        therapy_status_ack_received = *data_in.cmd_data; 
        if(therapy_status_ack_received == 0)
        {
          wait_for_therapy_status_ack = FALSE;
          timer_therapy_status_ack_value = 0;
#ifdef DEBUG
          printf("Received Therapy Status ACK:(%d)\n",retry_cnt_therapy_status_ack);
#endif
        }
        else
        {
#ifdef DEBUG
          printf("Received Therapy Status NACK\n");
#endif
        }
      }
      else if((data_in.command_id == BDM_TO_WCD_GET_ERROR_INFO) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
        prepareBdmResponse(WCD_TO_BDM_GET_ERROR_INFO_RESP, bdm_msg_out);
      }
      else if((data_in.command_id == BDM_TO_WCD_GET_AMBIENT_SETTINGS) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
        prepareBdmResponse(WCD_TO_BDM_GET_AMBIENT_SETTINGS_RESP, bdm_msg_out);
      }
      else if((data_in.command_id == BDM_TO_WCD_GET_WAND_LIFE_INFO) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
        prepareBdmResponse(WCD_TO_BDM_GET_WAND_LIFE_INFO_RESP, bdm_msg_out);
      }
      else if((data_in.command_id == BDM_TO_WCD_GET_GENERATOR_INFO) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
#ifdef DEBUG
        printf("Received Device Info Request...\n");
#endif
        prepareBdmResponse(WCD_TO_BDM_GET_GENERATOR_INFO_RESP, bdm_msg_out);
      }
      else if ((data_in.command_id == CAPDEV_WW_GET_CONFIG_BLOB) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
        cmd = CAPDEV_WW_GET_CONFIG_BLOB;
        if (therapy_on == TRUE)
        {
          set_setup_blob_cmd(bdm_msg_out,NACK_ERR_MASK(BUSY_ERR_NAK));
          send_serial_msg();
        }
        else
        {
          prepareBdmResponse(cmd, bdm_msg_out);
        }
      }
      else if ((data_in.command_id == CAPDEV_WW_SET_CONFIG_BLOB) && data_in.cmd_sequence_number.bits.req_num_msb)
      {
        cmd = CAPDEV_WW_SET_CONFIG_BLOB;
        if (therapy_on == TRUE)
        {
          set_setup_blob_cmd(bdm_msg_out,NACK_ERR_MASK(BUSY_ERR_NAK));
          send_serial_msg();
        }
        else
        {
          prepareBdmResponse(cmd, bdm_msg_out);
        }
      }
      else
      {
        //prepareBdmResponse(cmd, bdm_msg_out);
      }
    }
  }  
}