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
\file      profiles.h
\brief     Header file for profiles
\details   NA
*******************************************************************************/

#ifndef PROFILES_H
#define PROFILES_H

#define MAX_NUM_OF_PROFILES 25
#define MAX_NUM_OF_SPORT_MED_COAG_LEVELS	2
#define MAX_NUM_OF_ENT_COAG_LEVELS		3
#define MAX_NUM_OF_MANTA_COAG_LEVELS		9
#define MAX_NUM_OF_CUT_LEVELS_PER_MODE		3

typedef enum {
  ABLATE_ZILCH,
  ABLATE_MINUS,
  ABLATE_DEFAULT,
  ABLATE_PLUS,
  NUM_CUT_LEVELS
}CUT_LEVELS;

typedef enum {
  COAG_0,
  COAG_1,
  COAG_2,
  COAG_3,
  COAG_4,
  COAG_5,
  COAG_6,
  COAG_7,
  COAG_8,
  COAG_9,
  COAG_10,
  NUM_COAG_LEVELS
}COAG_LEVELS;

typedef enum {
  ENT_FLOW_0,
  ENT_FLOW_1,
  ENT_FLOW_2,
  ENT_FLOW_3,
  ENT_FLOW_4,
  ENT_FLOW_5,
  MAX_ENT_FLOW_SP,
}ENT_FLOW_SP_T;

typedef enum {
  MANTA_FLOW_0,
  MANTA_FLOW_1,
  MANTA_FLOW_2,
  MANTA_FLOW_3,
  MANTA_FLOW_4,
  MANTA_FLOW_5,
  MAX_MANTA_FLOW_SP,
}MANTA_FLOW_SP_T;

#pragma pack(1)
typedef struct {
  uint8_t record_full : 1;
  uint8_t finger_switch_en : 1;
  //uint8_t wireless_enable : 1;
  uint8_t languageID : 5;
  struct {
    CUT_LEVELS lo : 3;
    CUT_LEVELS med : 3;
    CUT_LEVELS hi : 3;
    COAG_LEVELS coag : 3;
  }setpoint;
  struct {
    uint8_t enable : 1;
    uint8_t enable_user_pref : 1;
    uint8_t alarm_threshold : 6;
    uint8_t audio_enabled : 1;
  } ambient;
}profile_record;

#pragma pack(1)
typedef struct {
  struct {
    uint8_t new_en : 1;
    uint8_t active_idx : 8;
    uint8_t idx_b4_pwr_cycle : 8;
  }settings;
  profile_record profile[256];
}user_data_record;

void profile_manager(void);
uint8_t save_profile(uint8_t);
uint8_t load_profile(uint8_t);
uint8_t cancel_new_profile(void);


#endif
