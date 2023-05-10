/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2844                 $:
$Date:: 2023-01-17 16:00:04#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      system.h
\brief     Header file for the main application module.
\details   This file declares all the global datastructures used by the
application.
*******************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#ifndef TEST
 #define STATIC_ARTC static
#else
 #define STATIC_ARTC
#endif

//#define DEBUG
//#define PRODUCTION
#define DEBUG_K26
#define STATOR_POWER_DOWN   50000

#include "common.h"
#include "pump.h"
#ifdef TEST
#include "artc_arm_math.h"
#else
#include "arm_math.h"
#endif
#include "event_buffer.h"
#include "gui.h"
#include "dma.h"
#include "esdhc.h"
#include "nv_data.h"
#include "profiles.h"
#include "head.h"
#include "buck.h"
#include <stdio.h>

typedef struct{
  uint8_t minutes;
  uint8_t seconds;
  uint8_t hours;
  uint8_t month;
  uint8_t week_day;
  uint8_t day;
  uint8_t year;
}TIME;

/*
  @ENT
  define the types of generators (Ear Nose & Throat or Sports Medicine)
*/

typedef enum
{
  UNKNOWN_GEN           = 0,
  SPORTS_MED            = 1,
  ENT                   = 2,
  ARTHROPLASTY          = 3,
} GenTypeDef;

#define PUMP_ALARM	        0x1
#define PUMP_DISABLED           0x2
#define PUMP_FAULT              0x4
#define PUMP_MOVING             0x8
#define PUMP_JOGGING            0x10
#define PUMP_MOTION             0x20
#define PUMP_READY              0x40
#define TUBE_ABSENT             0x80
#define DOOR_OPEN               0x100
#define DOOR_CLOSE              0x200
#define ERROR                   0x400
#define SENSORS_OK              0x800

#define SENSOR_STATES           (TUBE_ABSENT | \
                                 DOOR_OPEN | \
                                 DOOR_CLOSE | \
                                 ERROR | \
                                 SENSORS_OK)


#define PUMP_ON                 (PUMP_READY | \
                                 PUMP_JOGGING | \
                                 PUMP_MOVING)                                   

#define PUMP_RUNNING            (PUMP_MOTION | \
                                 PUMP_JOGGING | \
                                 PUMP_MOVING) 

#define DRIVE_STATUS_CMD_FIELDS (PUMP_ALARM | \
                                 PUMP_DISABLED | \
                                 PUMP_FAULT | \
                                 PUMP_MOVING | \
                                 PUMP_JOGGING | \
                                 PUMP_MOTION | \
                                  PUMP_READY)
                                   
#define PUMP_ALARM_FAULT         (PUMP_FAULT | \
                                  PUMP_ALARM) 

#define PUMP_DISABLE_ALARM_FAULT (PUMP_FAULT | \
                                  PUMP_DISABLED | \
                                  PUMP_ALARM) 
                                   
#define MFG_SERIAL_NUM_CHARS    16                                   
#define MFG_TIME_NUM_CHARS      5                                                            

#define PUMP_DISABLE_ALARM_FAULT (PUMP_FAULT | \
                                  PUMP_DISABLED | \
                                  PUMP_ALARM)

/*
    @ENT
    add default flow rate level
*/
#define DEFAULT_FLOW_RATE   (3)

/* Various treatment modes */
typedef enum {
  NONE,
  LO,
  MED,
  HI,
  VAC,
  COAG,
  NUM_OF_MODES
}MODES;



/* PCF8575 states for LEDs */
typedef enum {
  LED_OFF,
  LED_GRN,
  LED_RED,
  LED_FLASHING_GRN,  
}LED_STATE;

typedef enum {
  COMPLETE,
  TX_PENDING,
  TX_READY,
  TX_SENT,
  RX_PENDING
}WAND_WRITE_LOG_STATES;

typedef enum {
  Z_LOW = -1,
  Z_OK = 0,
  Z_HIGH = 1
}Z_LEVELS;

typedef enum {
  ENGLISH = 0,
  CHINESE = 1,
  DANISH = 2,
  DUTCH = 3,
  FINNISH = 4,
  FRENCH = 5,
  GERMAN = 6,
  GREEK = 7,
  ITALIAN = 8,
  JAPANESE = 9,
  NORWEGIAN = 10,
  PORTUGUESE = 11,
  SPANISH = 12,
  RUSSIAN = 13,
  SWEDISH = 14,
  TURKISH = 15,
  KOREAN = 16,
  BULGARIAN = 17,
  CROATIAN = 18,
  CZECH = 19,
  ESTONIAN = 20,
  ICELANDIC = 21,
  LATVIAN = 22,
  POLISH = 23,
  ROMANIAN = 24,
  SERBIAN = 25,
  SLOVAK = 26,
  SLOVENIAN = 27,  
  MAX_NUM_LANGUAGES,
}SUPPORTED_LANGUAGES;

/* Declare structure of therapy parameters */
#pragma pack(1)
typedef struct {
  bool updated : 1;
  struct {
    uint8_t ablate_on : 1;
    uint8_t coag_on : 1;
    uint8_t adj_coag : 1;
    MODES therapy_mode : 3;
    MODES previous_therapy_mode : 3;
    bool pulsing : 1;
    uint8_t vac_enabled : 1;
    uint8_t flow_rate : 4;
    bool flow_auto : 1;
    bool finger_switch_en : 1;
    bool wireless_enable : 1;
    bool auto_finger_switch_en : 1;
    bool auto_wireless_en : 1;
    bool ablate_enable : 1;
    ENT_FLOW_SP_T ent_flow_sp[NUM_OF_MODES][NUM_CUT_LEVELS];
    uint8_t ent_flow_sp_enable[MAX_ENT_FLOW_SP];
    bool ent_turbo_mode : 1;
    bool prime_completed : 1;
    bool pre_prime : 1;
    bool prime_glow : 1;
  } treatment;
  struct {
    CUT_LEVELS lo : 3;
    CUT_LEVELS med : 3;
    CUT_LEVELS hi : 3;
    uint8_t vac : 2;
    COAG_LEVELS coag : 4;
    uint8_t default_sp : 2;
    uint8_t max : 8;
    uint16_t level : 16;
    uint16_t ent_primary_level : 16;
    uint16_t ent_secondary_level : 16;
    uint16_t ent_duty_on_time : 16;
    uint16_t ent_duty_total_time : 16;
    uint16_t dac : 12;
    uint16_t trip_load : 8;
    uint16_t ipeak_voltage;
    float32_t ent_coag_power;
    float32_t ent_coag_power_primary;
    float32_t ent_coag_power_secondary;
    //uint16_t vmod_error : 12;
    //uint8_t vmod_high : 1;
  } setpoint;
  struct {
    //bool on : 1;
    uint16_t *p_sound_file;
    uint16_t initial_count : 8;
    uint8_t volume;
    uint8_t ambient_alarm_page : 8;
    uint16_t splash_volume;
    uint8_t error_burst_length : 8;
    uint8_t change_mode_burst_length : 8;
  }audio;
  struct {
    bool valid : 1;
    uint8_t timeout : 8;
    uint8_t cut_key : 1;
    uint8_t mode_key : 1;
    uint8_t coag_key : 1;
    uint8_t vac_key : 1;
    uint8_t handle_temp : 8;
    bool unlocked : 1;
    uint8_t *pdb;
    uint8_t passcode : 8;
    uint8_t name[17];
    bool z_test : 1;
    bool tube_warn : 1;
    bool tube_alert : 1;
    bool tube_warn_pulse : 1;
    uint8_t short_circuit : 1;
    bool thandle_wear : 1;
    bool irms_wear : 1;
    uint8_t worn_active_time : 8;
    uint8_t worn_timer_en : 1;
    uint8_t irms_worn_count : 8;
    uint8_t ambient_temp : 8;
    uint8_t worn_latch : 1;
    uint8_t arm_buttons : 1;
  } wand;
  struct {
    uint8_t enable : 1;
    uint8_t on : 1;
    uint8_t speed : 8;
    uint8_t flow : 8;
    uint8_t idle : 1;
    uint16_t inactivity_seconds : 16;
    uint8_t timeout : 8;
    uint8_t suction : 1;
    uint16_t pulse_delay : 16;
    float32_t pulse_rps;
    float32_t idle_rps;
    uint16_t quiescent_delay : 16;
    struct {
      float32_t min_rps;
      float32_t max_rps;
      float32_t kp1;
      float32_t kp2;
      float32_t kp3;
      float32_t ki1;
      float32_t ki2;
      float32_t ki3;
      float32_t e1;
      float32_t e2;
      q15_t coeffs[8];
      float32_t sp_amps;
      float32_t Ti;
    }pid;
    bool ctrl_en : 1;
    struct {
      uint16_t voltage_sp_default;
      uint32_t pulse_delay;
      uint16_t peak_voltage;
      uint32_t peak_time;
      uint32_t max_time;
    }vac;
    uint32_t sensor_status : 32;
    uint16_t stator : 16;
    uint8_t tube_present : 1;
    uint8_t door_present : 1;
    uint32_t status;
    q15_t mean_irms;
    float32_t ent_flow;
  } pump;
  struct {
    uint16_t pulse_window :16;
    uint16_t energy_threshold :16;
    uint8_t irms_peak : 8;
    uint16_t counter_delay : 16;
    uint64_t med_threshold : 64;
    uint64_t iec_threshold : 64;
    uint32_t irms_threshold : 32;
    uint16_t target_current : 16;
    uint32_t mean_irms : 32;
    uint8_t count : 8;
    uint64_t half_iec_threshold : 64;
  }pulsing;
  struct {
    uint8_t legacy_temperature : 8;
    uint8_t smart_wand_temperature : 8;
    uint8_t enable : 1;
    uint8_t enable_user_pref : 1;
    uint8_t thermometer_color : 3; 
    uint8_t temperature : 8;

    uint8_t alarm_threshold : 8;
    uint8_t capable : 1;
    uint8_t alarm_active : 1;
    uint8_t volume : 8;
    uint8_t audio_enabled: 1;
    uint8_t temp_sync: 1;
  } ambient;
  struct {
    uint8_t ablate : 1;
    uint8_t coag : 1;
    uint8_t present : 1;
    uint8_t mode : 1;
    uint8_t vac_key : 1;
    uint8_t mode_key : 1;
    struct {
      uint8_t ablate : 1;
      uint8_t coag : 1;
      uint8_t mode : 1;
      uint8_t safeguard : 1;
      uint8_t rf_signal_strength : 2;
      uint8_t byte_id0 : 2;
      uint8_t marriage_err : 1;
      uint8_t marriage_req : 1;
      uint8_t station2_response : 1;
      uint8_t station3_response : 1;
      uint8_t married2station12 : 1;
      uint8_t married2station1 : 1;
      uint8_t byte_id1 : 2;
      uint8_t audible_alert: 1;
      uint8_t xmit_batt : 1;
      uint8_t locator_mode : 1;
      uint8_t married : 1;
      uint8_t marry_in_progress : 1;
      uint8_t byte_id2 : 2;
      uint8_t handshake_phase : 2;
      uint8_t attempts : 8;
      uint8_t present : 2;
      uint8_t batt_low_count : 8;
      uint8_t married_latch : 1;
      uint32_t pair_timeout : 32;
    }wireless;
    struct {
      uint8_t ablate : 1;
      uint8_t coag : 1;
      uint8_t present : 1;
      uint8_t mode : 1;
      uint8_t vac_key : 1;
    }wired;
  }footpedal;
  struct {
    uint8_t screenID : 8;
    SUPPORTED_LANGUAGES languageID : 5;
    bool save_button_en : 1;
    uint32_t tx_count : 32;
    bool tx_wdog_err : 1;
    uint8_t alarm_test_sound : 1;
    uint8_t previous_screenID[5];
    uint8_t auto_revert_back_screenID : 8;
    uint8_t depth_idx : 8;
    struct {
      uint32_t tx_cntr_idx : 31;
      uint8_t err : 1;
      uint16_t time : 8;
      uint8_t wdog_err_cnt : 8;
    }rx_wdog;
    struct {
      GRAPHIC_ID GraphicNumber : 8;
      uint8_t GraphicValue: 8;
      uint16_t time : 8;
    }graphics;
    struct {
      uint16_t time : 8;
      bool crc_fault : 1;
      uint8_t update_status : 8;
    }sw;
    struct {
      uint32_t echo_l : 32;
      uint32_t echo_h : 32;
      uint16_t time : 8;
    } debug;
    struct {
      uint8_t sw : 1;
      uint8_t transport : 1;
      uint8_t touch : 1;
      uint8_t render : 1;
    }faults;
    struct {
      uint8_t can : 1;
      uint8_t transport : 1;
      uint8_t touch : 1;
    }errors;
  } gui;
  struct {
    bool debug_mode : 1;
    bool shutdown : 1;
    bool update_complete : 1;
    bool shutdown_imminent : 8;
    uint8_t update_progress : 8;
    uint8_t app1_version_major : 8;
    uint8_t app1_version_minor : 8;
    uint8_t app2_version_major : 8;
    uint8_t app2_version_minor : 8;
  }sw;
  struct {
    uint16_t temperature : 16;
  }hw;
  struct {                                 
    LED_STATE wand;
    LED_STATE pwr;
    LED_STATE odu;
    LED_STATE lemo;
    LED_STATE pump;
    bool startup : 1;
  }leds;
  struct {
    uint32_t power_down_time : 32;
    uint8_t tamper_detected : 1;
    TIME time;
  }rtc;
  struct{
    MODES mode;
    uint8_t level : 2;
    uint32_t start_time : 32;
    uint8_t legacy_ablate_on : 1;
    uint8_t legacy_coag_on : 1;

  }log;
  struct {
    uint32_t status : 32;
    uint32_t status2 : 32;
    uint8_t smart_wand_short : 1;
    uint8_t legacy_wand_short : 1;
    uint8_t smart_wand_use_limit : 1;
    uint8_t legacy_wand_use_limit : 1;
    uint8_t smart_wand_no_ambient : 1;
    uint8_t legacy_wand_no_ambient : 1;
    uint8_t smart_wand_id_error : 1;
    uint8_t legacy_wand_id_error : 1;
    uint8_t smart_wand_expired : 1;
    uint8_t legacy_wand_expired : 1;
    uint8_t smart_wand_stuck_switch : 1;
    uint8_t legacy_wand_stuck_switch : 1;
    uint8_t smart_wand_disconnected : 1;
    uint8_t legacy_wand_disconnected : 1;
    uint8_t fp_switch_stuck : 1;
    uint8_t legacy_wand_incompatible_error : 1;
    uint8_t legacy_wand_short_led_sec_countdown : 8;
    uint8_t smart_wand_incompatible_error : 8;
  }notify;
  user_data_record user_data;
  ESDHC_META sdhc;
  struct {
    uint8_t keys_active;
    uint8_t cut_key;
    uint8_t coag_key;
    uint8_t mode_key;
    uint8_t legacy_wand_coag_level : 4;
    uint8_t legacy_wand_ablate_level : 4;
    uint16_t dac;
    uint8_t topaz_timer:1;
  }legacy_wand_levels;
  struct {
    uint8_t lw_active;
    GenTypeDef smart_wand;
    GenTypeDef legacy_wand;
  }active_wand;
  /*
    @ENT
    add controller type to overall system settings
  */
  GenTypeDef controller;
  //uint8_t reserved[44];
  uint8_t previous_wand_indication;
}GEN_SETTINGS;

typedef struct {
  uint8_t final_assy[3];
  uint8_t work_order[4];
  uint8_t unique_id[2];
  uint32_t born_date;
  uint32_t expiration_date;
  uint8_t use_limit_time_en;
  uint8_t use_limit_activation_en;
  uint32_t use_limit_time_allowance;
  uint32_t max_activation_seconds;
  uint8_t name[12];
  uint8_t catalog[12];
  uint32_t use_limit_time;
  uint8_t default_coag_mode;
  uint8_t default_ablate_mode;
  uint8_t suction_en;
  uint8_t keyboard_en;
  uint8_t ambient_en;
  uint8_t indication;
  uint8_t mfg_format;
  uint8_t wand_wear_enable;
  uint8_t suction_enable;
  uint8_t finger_switch_enable;
  uint8_t ambient_enable;
  uint8_t low_enable;
  uint8_t med_enable;
  uint8_t hi_enable;
  uint8_t vac_enable;
  uint8_t coag_enable;
  uint8_t tube_temperature_enable;
  uint8_t format;
  uint8_t version;
  ENT_FLOW_SP_T default_flow_setting;
  uint8_t flow_enable;
}MFG_PARAMETERS;  
  
typedef struct {
  uint16_t z_high;
  uint16_t z_low;
  uint16_t z_level;
  uint32_t z_time;
  uint8_t temp_comp_gain;
  uint16_t z_high_mod;
}IMPEDANCE_PARAMETERS;

typedef struct {
  uint16_t z_high;
  uint16_t z_low;
  uint16_t z_level;
  uint32_t z_time;
}MANTA_IMPEDANCE_PARAMETERS;

typedef struct {
  q15_t coeffs[8];
}FILTER_PARAMETERS;

typedef struct {
  float32_t cc;
  float32_t ic_delay;
  float32_t ic;
  float32_t max_accel;
  float32_t max_decel;
}PUMP_PARAMETERS;

typedef struct {
  uint16_t coag1_voltage;
  float32_t coag1_rpm;
  uint16_t coag2_voltage;
  float32_t coag2_rpm;
  uint16_t coag3_voltage;
  float32_t coag3_rpm;
  uint16_t coag4_voltage;
  float32_t coag4_rpm;
  uint16_t coag5_voltage;
  float32_t coag5_rpm;
  uint16_t coag6_voltage;
  uint16_t coag7_voltage;
  uint16_t coag8_voltage;
  uint16_t coag9_voltage;
  uint16_t coag10_voltage;
  uint16_t ss_time;
}COAG_PARAMETERS;

typedef struct {
  uint16_t coag_voltage;
  float32_t coag_rpm;
}COAG_MANTA_BKUP;

typedef struct {
  float32_t min_rps_default;
  float32_t max_rps_default;
  float32_t kp1;
  float32_t kp2;
  float32_t kp3;
  float32_t ki1;
  float32_t ki2;
  float32_t ki3;
  float32_t e1;
  float32_t e2;
  uint16_t voltage_sp_default;
  float32_t current_sp_default;
  float32_t idle_rps;
  uint32_t ss_time;
  uint16_t pulse_window;
  uint16_t pulse_energy;
  uint32_t quiescent_delay;
  float32_t current_sp_plus;
  uint16_t voltage_sp_plus;
  float32_t current_sp_minus;
  uint16_t voltage_sp_minus;
}LO_PARAMETERS;

typedef struct {  
  float32_t min_rps_default;
  float32_t max_rps_default;
  float32_t kp1;
  float32_t kp2;
  float32_t kp3;
  float32_t ki1;
  float32_t ki2;
  float32_t ki3;
  float32_t e1;
  float32_t e2;
  uint16_t voltage_sp_default;
  float32_t current_sp_default;
  float32_t idle_rps;
  uint32_t ss_time;
  uint16_t pulse_window;
  uint16_t pulse_energy;
  float32_t current_sp_plus;
  uint16_t voltage_sp_plus;
  float32_t current_sp_minus;
  uint16_t voltage_sp_minus;
}MED_PARAMETERS;

typedef struct {
  float32_t min_rps_default;
  float32_t max_rps_default;
  float32_t kp1;
  float32_t kp2;
  float32_t kp3;
  float32_t ki1;
  float32_t ki2;
  float32_t ki3;
  float32_t e1;
  float32_t e2;
  uint16_t voltage_sp_default;
  float32_t current_sp_default;
  float32_t idle_rps;
  uint16_t pulse_window;
  uint16_t pulse_energy;
  uint16_t ss_time;
  uint16_t voltage_sp_plus;
  float32_t max_rps_plus;
  uint16_t voltage_sp_minus;
  float32_t max_rps_minus;
  uint16_t ss_pause;
}HI_PARAMETERS;

typedef struct {
  float32_t min_rps_default;
  float32_t max_rps_default;
  float32_t kp1;  
  float32_t ki1;
  uint16_t voltage_sp_default;
  float32_t current_sp_default;
  float32_t idle_rps;
  uint16_t pulse_window;
  uint16_t pulse_energy;
  uint32_t ss_time;
  uint16_t pulse_delay;
  float32_t pulse_rps;
  uint16_t peak_voltage;
  uint16_t pulsing_voltage;
  uint16_t peak_time;
  uint16_t max_time;
}VAC_PARAMETERS;

typedef struct {
  uint16_t high_warn_en;
  uint16_t med_warn_en;
  uint16_t warn_disable;
}TUBING_PARMETERS;

/* wand software version/copyright */
typedef struct {
  uint8_t name[8];   /* product code */
  uint8_t hw_version;/* hardware major/minor */
  uint8_t sw_version;/* software major/minor */
  uint8_t year[2];   /* release year */
  uint8_t month;     /* release month */
  uint8_t day;       /* release day */
  uint16_t checksum; /* product code checksum */
}WAND_SOFTWARE_HEAD;

typedef struct {
  uint16_t voltage;
  uint16_t trip_load;
}SCOPE_SAVER_PARAMETERS;

typedef struct {
  uint16_t lo : 16;
  uint16_t med: 16;
  uint16_t hi : 16;
  uint16_t vac : 16;
}LIFE_PARAMETERS;

typedef struct {
  uint8_t detect_duration : 8;
  uint32_t lo_irms_threshold : 32;
  uint32_t med_irms_threshold : 32;
  uint32_t hi_irms_threshold : 32;
  uint32_t lo_temp_threshold : 32;
  uint32_t med_temp_threshold : 32;
  uint32_t hi_temp_threshold : 32;
}WAND_WEAR_PARAMETERS;

typedef struct {
  float32_t spd;
  uint8_t time;
  uint8_t rf_delay;
}ENT_PRE_PRIME_PARAMS_T;

typedef struct {
  float32_t spd;
  uint16_t time;
}ENT_AUTO_PRIME_PARAMS_T;


typedef struct {
  uint16_t time;
  float32_t adj;
  float32_t initial_spd;
  float32_t final_spd;
  uint16_t rms_threshold;
}MANTA_AUTO_PRIME_PARAMS_T;

typedef struct {
  float32_t spd;
}MANTA_MANUAL_PRIME_PARAMS_T;

typedef struct {
  ENT_PRE_PRIME_PARAMS_T pre;
  ENT_AUTO_PRIME_PARAMS_T autom;
}ENT_PRIME_PARAMS_T;

typedef struct {
  ENT_PRE_PRIME_PARAMS_T pre;
  MANTA_MANUAL_PRIME_PARAMS_T man;
  MANTA_AUTO_PRIME_PARAMS_T autom;
}MANTA_PRIME_PARAMS_T;

typedef struct {
  uint16_t primary_voltage;
  uint16_t secondary_voltage;
  uint16_t blend_primary_voltage_time;
  uint16_t blend_total_time;
  float32_t flow[MAX_ENT_FLOW_SP];
}ENT_CUT_LEVEL_PARAMS_T;

typedef struct {
  ENT_CUT_LEVEL_PARAMS_T level[MAX_NUM_OF_CUT_LEVELS_PER_MODE];
  uint32_t rms_current_limit;
  uint16_t pulse_window;
  uint16_t pulse_energy;
}ENT_ABLATE_MODE_PARAMS_T;

typedef struct {
  uint16_t increase;
  uint16_t decrease;
}ABLATE_RAMP_RATE_PARAMS_T;
  
typedef struct {
  uint16_t primary;
  uint16_t secondary;
  uint16_t blend_primary_time;
  uint16_t blend_total_time;
  uint16_t primary_v_limit;
  uint16_t secondary_v_limit;
  float32_t pump_spd;
}ENT_COAG_MODE_PARAMS_T;

typedef struct {
  uint16_t primary;
}MANTA_COAG_MODE_PARAMS_T;

typedef struct{
  //ID_PARAMETERS id;
  //LIFE_PARAMETERS life;
  MFG_PARAMETERS mfg;
  IMPEDANCE_PARAMETERS impedance;
  FILTER_PARAMETERS filter;
  PUMP_PARAMETERS pump;
  COAG_PARAMETERS coag;
  LO_PARAMETERS lo;
  MED_PARAMETERS med;
  HI_PARAMETERS hi;
  VAC_PARAMETERS vac;
  TUBING_PARMETERS handle;
  WAND_SOFTWARE_HEAD header;
  SCOPE_SAVER_PARAMETERS ipeak;
  LIFE_PARAMETERS life;
  WAND_WEAR_PARAMETERS worn;
  ABLATE_RAMP_RATE_PARAMS_T ramp_rate;
  struct{
	ENT_PRIME_PARAMS_T prime;
	ENT_ABLATE_MODE_PARAMS_T lo;
	ENT_ABLATE_MODE_PARAMS_T med;
	ENT_ABLATE_MODE_PARAMS_T hi;
	ENT_COAG_MODE_PARAMS_T coag[MAX_NUM_OF_ENT_COAG_LEVELS];
	SCOPE_SAVER_PARAMETERS ipeak;
  }ent;
  struct{
	MANTA_PRIME_PARAMS_T prime;
	uint16_t coag[NUM_COAG_LEVELS];
        float32_t manta_flow_sp[NUM_COAG_LEVELS][MAX_MANTA_FLOW_SP];
	SCOPE_SAVER_PARAMETERS ipeak;
  }manta;
  uint16_t coag_life;
  uint16_t cfg_version;
}SMART_WAND_PARAMETERS;

typedef struct {
  uint8_t ambient_sensor_flag;
}COMMON_WAND_PARAMS;

typedef struct 
{
  uint16_t R1;
  uint16_t R2;
  uint8_t  fuse_blown_flag; // 1 - fuse blow, 0 - fuse not blown
  uint8_t  temp;
  // NOTE: legacy_wand_params_obj.wand_connected = 1 means a wand is 
  // physically present in the slot, it does not mean it is validated and 
  // ready to use
  uint8_t  wand_connected; 
  uint8_t coag_level:4;
  uint8_t ablate_level:4;
  uint8_t ablate_flag;
  uint8_t coag_flag;
  uint8_t mode_flag;
  uint8_t min_coag;
  uint8_t max_coag;
  uint8_t min_ablate;
  uint8_t max_ablate;
  float slope;
  int8_t intercept;
  uint16_t dac;
  uint16_t level;
  uint8_t peak_detect_flag;
  IMPEDANCE_PARAMETERS impedance;
  bool z_test : 1;
  time_keeper time;
  struct
  {
    bool audio_muted;
    uint16_t maximum;
    u16 current;
  }timer;
  uint8_t ambient_capable;
  uint8_t wand_909;
  uint8_t coag_enable;
  uint8_t therapy_on;
  uint8_t lower_current_limit;
}legacy_wand_params;


/* Declare ptr to global setting data */
extern GEN_SETTINGS current_therapy_settings;
extern SMART_WAND_PARAMETERS smart_wand_params;
extern event_table evt_tbl;
extern uint8_t serial_number[MFG_SERIAL_NUM_CHARS];
extern uint8_t bcd_serial_number[MFG_SERIAL_NUM_CHARS];
extern uint8_t mfg_date_time[MFG_TIME_NUM_CHARS];
extern COMMON_WAND_PARAMS smart_wand_common_params;
extern COMMON_WAND_PARAMS legacy_wand_common_params;

uint8_t IDLE_state_entry (void);
extern bool unitTest;
#endif

