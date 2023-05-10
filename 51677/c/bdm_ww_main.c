
#include "bdm_ww_main.h"

extern uint8_t                          prepareBdmResponse(uint8_t, uint8_t*);
extern tBDT                             tBDTtable[16];
extern uint8_t                          gu8USB_Flags;
extern uint8_t                          gu8EP3_OUT_ODD_Buffer[EP3_SIZE];
extern uint8_t                          gu8EP2_IN_ODD_Buffer[EP2_SIZE];
extern uint8_t                          *BufferPointer[];
extern wcd_bdm_port_status_t            port_status_msg;
extern wcd_therapy_settings_t           get_therapy_msg;
extern legacy_wand_params               legacy_wand_params_obj;
extern GEN_SETTINGS                     current_therapy_settings;
extern SMART_WAND_PARAMETERS            smart_wand_params;
extern uint8_t                          *bdm_msg_out;
extern bool                             isWWConnected;
extern bool                             isWWReady;
extern bool                             sendPortStatus;
extern bool                             sendTherapyStatus;
extern bool                             sendDevInfoStatus;
extern u32                              port_stats_msg_cnt;
extern bdm_serial_msg_container_t       data_in;
extern u8                               port_status_ack_received;
extern u8                               therapy_status_ack_received;

uint8_t dataSize                        = 31;
bool evt_port_status                    = FALSE;
bool evt_therapy_status                 = FALSE;
bool wait_for_therapy_status_ack        = FALSE;
bool wait_for_port_status_ack           = FALSE;
bool expired_timer_port_status_ack      = FALSE;
wcd_bdm_port_status_t                   port_status_msg_copy;
wcd_therapy_settings_t                  get_therapy_msg_copy;
u8 timeout_cnt                          = 0;
u8 retry_cnt_port_status_ack            = 0;
u8 retry_cnt_therapy_status_ack         = 0;
u8 timer_port_status_ack_value          = 0;
u8 timer_therapy_status_ack_value       = 0;


bool prepare_therapy_message(void)
{
  get_therapy_msg.smartAbCoagSetPoint.bits.ablate_low_setpoint = current_therapy_settings.setpoint.lo;
  get_therapy_msg.smartAbCoagSetPoint.bits.ablate_med_setpoint = current_therapy_settings.setpoint.med;
  get_therapy_msg.smartAbCoagSetPoint.bits.ablate_hi_setpoint = current_therapy_settings.setpoint.hi;
  get_therapy_msg.smartAbCoagSetPoint.bits.coag_setpoint = current_therapy_settings.setpoint.coag;
  get_therapy_msg.AbCoag18SetPoint.bits.legacy_ablate_setpoint = legacy_wand_params_obj.ablate_level;
  get_therapy_msg.AbCoag18SetPoint.bits.legacy_coag_setpoint = legacy_wand_params_obj.coag_level;
  get_therapy_msg.therapy_set.bits.therapy_modes = current_therapy_settings.treatment.therapy_mode;
  //get_therapy_msg.therapy_set.bits.ablate = current_therapy_settings.treatment.ablate_enable;
  get_therapy_msg.therapy_set.bits.ablate_status = current_therapy_settings.treatment.ablate_on;
  if(current_therapy_settings.wand.valid && !current_therapy_settings.active_wand.lw_active)
  {
    get_therapy_msg.therapy_set.bits.coag = smart_wand_params.mfg.coag_enable;
  }
  else if(legacy_wand_params_obj.wand_connected && current_therapy_settings.active_wand.lw_active)
  {
    get_therapy_msg.therapy_set.bits.coag = legacy_wand_params_obj.coag_enable;
  }
  else
  {
    get_therapy_msg.therapy_set.bits.coag                      = FALSE;        
  }
  get_therapy_msg.therapy_set.bits.coag_status = current_therapy_settings.treatment.coag_on;
  get_therapy_msg.therapy_set.bits.finger_switch_status = current_therapy_settings.treatment.finger_switch_en;
  get_therapy_msg.amb_thres_sp = current_therapy_settings.ambient.alarm_threshold;
  get_therapy_msg.pump_settings.bits.pump_flow_settings = (u8)current_therapy_settings.pump.idle_rps;
  get_therapy_msg.amb_settings.bits.feature = (current_therapy_settings.ambient.capable & current_therapy_settings.ambient.enable_user_pref);
  get_therapy_msg.amb_settings.bits.notification = current_therapy_settings.ambient.audio_enabled;
  get_therapy_msg.amb_settings.bits.volume_level = current_therapy_settings.ambient.volume;
  get_therapy_msg.gen_settings.bits.gen_volume_level = current_therapy_settings.audio.volume;
  get_therapy_msg.gen_settings.bits.prime_auto_manual = current_therapy_settings.treatment.flow_auto;
  get_therapy_msg.gen_settings.bits.wireless_foot_pedal_enable = current_therapy_settings.treatment.wireless_enable;
  get_therapy_msg.reflex_timer.bits.reflex_timer = legacy_wand_params_obj.timer.current;
  get_therapy_msg.reflex_timer.bits.reflex_notification_en = legacy_wand_params_obj.timer.audio_muted;
  if (!sendTherapyStatus)
  {
    if ( (get_therapy_msg_copy.AbCoag18SetPoint.val == get_therapy_msg.AbCoag18SetPoint.val) &&
        (get_therapy_msg_copy.smartAbCoagSetPoint.val == get_therapy_msg.smartAbCoagSetPoint.val) &&
          (get_therapy_msg_copy.gen_settings.val == get_therapy_msg.gen_settings.val) &&
            (get_therapy_msg_copy.therapy_set.val == get_therapy_msg.therapy_set.val) &&
              (get_therapy_msg_copy.amb_settings.val == get_therapy_msg.amb_settings.val) &&
                (get_therapy_msg_copy.amb_thres_sp == get_therapy_msg.amb_thres_sp) &&
                  (get_therapy_msg_copy.reflex_timer.val == get_therapy_msg.reflex_timer.val) &&
                    (get_therapy_msg_copy.pump_settings.val == get_therapy_msg.pump_settings.val) ) 
    {
      evt_therapy_status = FALSE;
    }
    else 
    {
      if(evt_port_status == TRUE)
      {
#ifdef DEBUG
        printf("Port Status transmission in progress...\n");
#endif
      }
      else
      {
        therapy_status_ack_received = 0;
        get_therapy_msg_copy = get_therapy_msg; 
        prepareBdmResponse(WCD_TO_BDM_THERAPY_SETTINGS_STATUS, bdm_msg_out);
        wait_for_therapy_status_ack = TRUE;
        timer_therapy_status_ack_value = 1;
        retry_cnt_therapy_status_ack = 0;
#ifdef DEBUG
        printf("Therapy Status Msg Retry Number:(%d)\n",retry_cnt_therapy_status_ack);
#endif
      }
      evt_therapy_status = TRUE;  
    }
  }
  
  return evt_therapy_status; 
}


bool prepare_port_message(void)
{
  port_status_msg.foot_info.bits.wired = current_therapy_settings.footpedal.wired.present;
  port_status_msg.foot_info.bits.wireless = (current_therapy_settings.footpedal.wireless.present > 0)?1:0;
  port_status_msg.foot_info.bits.wireless_battery = (current_therapy_settings.footpedal.wireless.batt_low_count >= 50) ? TRUE : FALSE;
  if (legacy_wand_params_obj.wand_connected)
  {
    port_status_msg.wand_info.bits.legacy_wand = TRUE;
    
    if(current_therapy_settings.active_wand.lw_active)
    {
      port_status_msg.wand_info.bits.pri_wand = LEGACY_SM; //Legacy SM
      if(current_therapy_settings.active_wand.legacy_wand == ENT)
      {
        port_status_msg.wand_info.bits.pri_wand = NO_WAND;
      }
      port_status_msg.wand_info.bits.reflex_timer = FALSE; //Check values
      port_status_msg.wand_info.bits.topaz_timer = current_therapy_settings.legacy_wand_levels.topaz_timer; 
    }
  }
  else
  {
    port_status_msg.wand_info.bits.legacy_wand = FALSE;
    
  }
  
  if (current_therapy_settings.wand.valid)
  {
    port_status_msg.wand_info.bits.smart_wand = TRUE;
    if(!current_therapy_settings.active_wand.lw_active)
    {
      if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_SPORTS_MED)
      {
        port_status_msg.wand_info.bits.pri_wand = SMART_SM; //Smart wand
      }
      else if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_ENT)
      {
        port_status_msg.wand_info.bits.pri_wand = NO_WAND;//SMART_ENT;
      }
      else if(smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA)
      {
        port_status_msg.wand_info.bits.pri_wand = NO_WAND;//SMART_ARTHROPLASTY;
      }
      port_status_msg.wand_info.bits.reflex_timer = FALSE; //Check values
      port_status_msg.wand_info.bits.topaz_timer = FALSE;
    }
  }
  else
    port_status_msg.wand_info.bits.smart_wand = FALSE;
  
  if(!legacy_wand_params_obj.wand_connected && !current_therapy_settings.wand.valid)
  {
    port_status_msg.wand_info.val = 0;
  }
  
  if (legacy_wand_params_obj.wand_connected)
  {
    port_status_msg.legacy_therapy_max.bits.ablate_max = legacy_wand_params_obj.max_ablate;
    port_status_msg.legacy_therapy_max.bits.coag_max = legacy_wand_params_obj.max_coag;
  }
  else
  {
    port_status_msg.legacy_therapy_max.bits.ablate_max = 0;
    port_status_msg.legacy_therapy_max.bits.coag_max = 0;
  }
  if(!current_therapy_settings.active_wand.lw_active && (smart_wand_params.mfg.indication == 2))
  {
    port_status_msg.devMode = ARTHROPLASTY;
  }
  else
  {
    port_status_msg.devMode = current_therapy_settings.controller;
  }
  
  if(!port_status_msg.devMode)
  {
    port_status_msg.devMode = SPORTS_MED;
  }
  port_status_msg.language_id = current_therapy_settings.gui.languageID;
  
  if ((legacy_wand_params_obj.wand_connected) && current_therapy_settings.active_wand.lw_active)  //Legacy 
  {
    port_status_msg.wand_cap_info.bits.wand_lo_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_med_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_hi_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_vac_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_coag_capable = legacy_wand_params_obj.coag_enable;
    port_status_msg.wand_cap_info.bits.wand_ablate_capable = TRUE;
    port_status_msg.wand_cap_info.bits.ambient_capable = legacy_wand_params_obj.ambient_capable;
  }
  else if ((current_therapy_settings.wand.valid) && !current_therapy_settings.active_wand.lw_active) //Smart Wand
  {
    port_status_msg.wand_cap_info.bits.wand_lo_capable = smart_wand_params.mfg.low_enable;
    port_status_msg.wand_cap_info.bits.wand_med_capable = smart_wand_params.mfg.med_enable;
    port_status_msg.wand_cap_info.bits.wand_hi_capable = smart_wand_params.mfg.hi_enable;
    port_status_msg.wand_cap_info.bits.wand_vac_capable = smart_wand_params.mfg.vac_enable;
    port_status_msg.wand_cap_info.bits.wand_coag_capable = TRUE; //Always True
    port_status_msg.wand_cap_info.bits.wand_ablate_capable = TRUE;
    port_status_msg.wand_cap_info.bits.ambient_capable = current_therapy_settings.ambient.capable;
  }
  else                                          
  {
    port_status_msg.wand_cap_info.bits.wand_lo_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_med_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_hi_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_vac_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_coag_capable = FALSE;
    port_status_msg.wand_cap_info.bits.wand_ablate_capable = FALSE;
    port_status_msg.wand_cap_info.bits.ambient_capable = FALSE;
  } 
  if (current_therapy_settings.wand.valid  && (current_therapy_settings.active_wand.lw_active == 0))
  {
    memcpy(&port_status_msg.wand_name,&smart_wand_params.mfg.name,WAND_NAME_CHARS);
    memcpy(&port_status_msg.wand_cat_num,&smart_wand_params.mfg.catalog,WAND_CAT_NUM_CHARS);
    memcpy(&port_status_msg.wand_lot_num,&smart_wand_params.mfg.work_order,sizeof(u32));
    memcpy(&port_status_msg.wand_id_num,&smart_wand_params.mfg.unique_id,sizeof(u16));
  }
  else 
  {
    memset(&port_status_msg.wand_name,0,WAND_NAME_CHARS);
    memset(&port_status_msg.wand_cat_num,0,WAND_CAT_NUM_CHARS);
    memset(&port_status_msg.wand_lot_num,0,sizeof(u32));
    memset(&port_status_msg.wand_id_num,0,sizeof(u16));
  }
  if (!sendPortStatus)
  {
    if ( (port_status_msg_copy.foot_info.val == port_status_msg.foot_info.val) &&
        (port_status_msg_copy.language_id == port_status_msg.language_id) &&
          (port_status_msg_copy.legacy_therapy_max.val == port_status_msg.legacy_therapy_max.val) &&
            (port_status_msg_copy.wand_cap_info.val == port_status_msg.wand_cap_info.val) &&
              (port_status_msg_copy.wand_info.val == port_status_msg.wand_info.val) &&
                (port_status_msg_copy.wand_id_num == port_status_msg.wand_id_num) &&
                  (port_status_msg_copy.devMode == port_status_msg.devMode) ) 
    {
      evt_port_status = FALSE;
    }
    else 
    {
      port_status_ack_received = 0;
      port_status_msg_copy = port_status_msg;
      prepareBdmResponse(WCD_TO_BDM_PORT_STATUS, bdm_msg_out);
      wait_for_port_status_ack = TRUE;
      timer_port_status_ack_value = 1;
      retry_cnt_port_status_ack = 0;
#ifdef DEBUG
      printf("Port Msg Retry Number:(%d)\n",retry_cnt_port_status_ack);
#endif
      evt_port_status = TRUE;
    }
  }
  return evt_port_status;
}

uint8_t bdm_poll_usb()
{
  // USB CDC service routine
  CDC_Engine();
  
  // If data transfer arrives
  if(FLAG_CHK(EP_OUT,gu8USB_Flags))
  {
    (void)USB_EP_OUT_SizeCheck(EP_OUT);         
    usbEP_Reset(EP_OUT);
    usbSIE_CONTROL(EP_OUT);
    FLAG_CLR(EP_OUT,gu8USB_Flags);
    in_bdm_wcd_msg(); //check if message 
  }
  else
  {
    if(isWWConnected && isWWReady)
    {
      prepare_port_message();
      prepare_therapy_message();
    }
    
    if(sendTherapyStatus == TRUE)
    {
      prepare_therapy_message();
      //prepareBdmResponse(WCD_TO_BDM_THERAPY_SETTINGS_STATUS, bdm_msg_out);
      sendTherapyStatus = FALSE;
      isWWConnected = TRUE;
    }
    if(sendPortStatus == TRUE)
    {
      prepare_port_message();
      //prepareBdmResponse(WCD_TO_BDM_PORT_STATUS, bdm_msg_out);
      sendPortStatus = FALSE;
      sendTherapyStatus = TRUE;
    }
    
    if(timer_port_status_ack_value)
      timer_port_status_ack_value++;
    
    if(timer_therapy_status_ack_value)
      timer_therapy_status_ack_value++;
    
    if((wait_for_port_status_ack && (timer_port_status_ack_value == EVT_MSG_TIMEOUT)) || (wait_for_port_status_ack && port_status_ack_received)) //If Timeout(300ms) || NACK
    {
      if(retry_cnt_port_status_ack < EVT_MSG_RETRY_CNT)
      {
        
        port_status_ack_received = 0;
        timer_port_status_ack_value = 1;
        retry_cnt_port_status_ack++;
        prepareBdmResponse(WCD_TO_BDM_PORT_STATUS, bdm_msg_out);
#ifdef DEBUG
        printf("Port Status Msg Retry Number:(%d)\n",retry_cnt_port_status_ack);
#endif
      }
      else
      {
#ifdef DEBUG
        printf("Port Status Msg Retry Failed\n");
#endif
        isWWConnected = FALSE;
        retry_cnt_port_status_ack = 0; 
        timer_port_status_ack_value = 0; 
        wait_for_port_status_ack = FALSE;
      }
    }
    if((wait_for_therapy_status_ack && (timer_therapy_status_ack_value == EVT_MSG_TIMEOUT)) || (wait_for_therapy_status_ack && therapy_status_ack_received)) //If Timeout(300ms) || NACK
    {
      if(retry_cnt_therapy_status_ack < EVT_MSG_RETRY_CNT)
      {
        
        therapy_status_ack_received = 0;
        timer_therapy_status_ack_value = 1;
        retry_cnt_therapy_status_ack++;
        prepareBdmResponse(WCD_TO_BDM_THERAPY_SETTINGS_STATUS, bdm_msg_out);
#ifdef DEBUG
        printf("Therapy Msg Retry Number:(%d)\n",retry_cnt_therapy_status_ack);
#endif
      }
      else
      {
#ifdef DEBUG
        printf("Therapy Status Msg Retry Failed\n");
#endif
        isWWConnected = FALSE;
        retry_cnt_therapy_status_ack = 0; 
        timer_therapy_status_ack_value = 0; 
        wait_for_therapy_status_ack = FALSE;
      }
    }
    if (sendDevInfoStatus == TRUE)
    {
      data_in.cmd_data_len = 0;
      //prepareBdmResponse(WCD_TO_BDM_GET_GENERATOR_INFO_RESP, bdm_msg_out);
      sendPortStatus = TRUE;
      sendDevInfoStatus = FALSE;
    }
    
  }
  
  
  return 1;
}


