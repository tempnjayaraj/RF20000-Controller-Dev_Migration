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
\file      nv_data.
\brief     Declaration for the non volatile data structure.
\details   NA
*******************************************************************************/

#ifndef NV_DATA_H
#define NV_DATA_H  

#include "profiles.h"
#include "legacy_wand.h"

#define MFG_SERIAL_NUM_CHARS            16                                   
#define MFG_TIME_NUM_CHARS              5      
#define NONVOLATILE_DATA_ADDRESS        0xE000u

#pragma pack(1)
typedef struct
{
  
  uint16_t checksum;
  uint16_t nwords;
  uint8_t master_volume;
  uint8_t ambient_volume;
  uint32_t active_app_ptr;
  uint8_t mfg_date_time[MFG_TIME_NUM_CHARS];
  uint8_t serial_number[MFG_SERIAL_NUM_CHARS];
  uint32_t rtc;
  uint8_t vbatt_tamper_detect;
  struct {
    uint8_t work_order[4];
    uint8_t unique_id[2];

  }smart_wand;
  uint8_t app1_version_major;
  uint8_t app1_version_minor;
  uint8_t app2_version_major;
  uint8_t app2_version_minor;
  uint8_t languageID;
  user_data_record user_data;
  previous_wands nv_previous_wands_database[LW_DB_SZ];
  uint8_t wireless_en;
  uint8_t wireless_married_latch;
  uint8_t previous_wand_indication;
  COAG_LEVELS previous_wand_coag : 4;
  uint8_t previous_wand_flow_rate : 4;
}nonvolatile_data_layout;

#endif
