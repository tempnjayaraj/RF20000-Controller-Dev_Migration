/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: cwalli00          $:
$Rev:: 1763                 $:
$Date:: 2015-02-18 18:04:10#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      esdhc.h
\brief     Header file for SD card driver
\details   This is used for SD logging during power cycle
*******************************************************************************/
#ifndef ESDHC_H
#define ESDHC_H

#include "common.h"


#define BIT_MODE 			DTW_4BIT
#define SD_CARD_DOWNLOAD_COMPLETE_FLAG  0xFFFFFFFF
#define CIRCLE_LOG_END_SECTOR           5044610
/* polynomial used for CSD CRCs */                 
#define CRC7_POLY                       0x89
#pragma pack(1)
typedef struct{
   struct{
      uint8_t  type;
      uint32_t starting_sector;
      uint32_t size;
      uint32_t ending_sector;
   }pt[2];
}mbr_sector;

typedef enum {
   READ_MBR,
   START_FILE_SEARCH,
   SEARCH_4_FILE,
   DOWNLOAD_FILE,
   WRITE2FLASH,
}SDHC_DOWNLOAD_STATE;

typedef enum {
   CMD0_GO_IDLE,
   GET_CARD_VERSION,
   CMD55_CARD_APP_CMD_0,
   REQUEST_OCR_0,
   CMD55_CARD_APP_CMD_1,
   REQUEST_OCR_1,
   CMD55_CARD_APP_CMD_2,
   REQUEST_OCR_2,
   REQUEST_ALL_CIDS,
   GET_RCA,
   ADDRESS_CARD,
   SEND_CID,
   GET_CARD_STATUS,
   SELECT_CARD,
   SET_APP_SPECIFIC,
   SET_BUS_WIDTH,
  SEND_CSD,
  WRITE_TEST,
   READ_TEST,
   TEST_COMPLETE,
   READ_VERSION,
   FIELD_UPGRADE_CHECK,
   LOGGING_CHECK,
   SDHC_INIT_COMPLETE,
}SDHC_INIT_STATE;

typedef struct 
{
   uint8_t checksum[2];
   uint8_t empty_space[2];
   uint32_t ROM_address;   
   uint32_t ropi_bytes;
   uint32_t rwpi_data_bytes;
   uint32_t rwpi_zi_bytes;
   uint8_t app_name[8];
   uint8_t compile_date[11];
   uint8_t empty_space2[1];
   uint8_t compile_time[8];
   uint8_t empty_space3[4];
   uint32_t complier_ver;
   uint8_t app_version_major;
   uint8_t app_version_minor;
   uint8_t empty_space4[2];
   uint32_t start_offset; 
}header_layout;

typedef enum {
   READ_STATIC,
   READ_CIRCLE,  
   STATIC_LOG_ERROR,
   STATIC_LOG_THERAPY_DATA,
   STATIC_LOG_LEGACY_THERAPY_DATA,
   WRITE_STATIC,
   LOGGING_COMPLETE,
   CARD_NOT_PRESENT,
}SDHC_LOGGING_CMDS;

typedef struct {
  bool card_loaded : 1;
  bool update_uis : 1;
  uint8_t card_ready : 1;
  uint8_t card_detected : 1;
  uint8_t gsfw_update_avaliable : 1;
  uint8_t uifw_update_avaliable : 1;
  uint8_t uifw_major : 8;
  uint8_t uifw_minor : 8;
  uint8_t logging_ready : 1;
  uint8_t card_fault : 1;
  SDHC_LOGGING_CMDS cmd_que[8][5];
  uint8_t cmd_que_idx : 8;
  mbr_sector mbr;
}ESDHC_META;

//typedef struct {
//  uint8_t sturcture : 2;
//  uint8_t reserved_1 : 6;
//  uint8_t taac : 8;
//  uint8_t nsac : 8;
//  uint8_t tran_spd : 8;
//  uint16_t ccc : 12;
//  uint8_t read_bl_len : 4;
//  uint8_t read_bl_partial : 1;
//  uint8_t write_blk_misalign : 1;
//  uint8_t read_blk_misalign : 1;
//  uint8_t dsr_imp : 1;
//  uint8_t reserved_2 : 2;
//  uint16_t c_size : 12;
//  uint8_t vdd_r_curr_min : 3;
//  uint8_t vdd_r_curr_max : 3;
//  uint8_t vdd_w_curr_min : 3;
//  uint8_t vdd_w_curr_max : 3;
//  uint8_t c_size_mult : 3;
//  uint8_t erase_blk_en : 1;
//  uint8_t sector_size : 7;
//  uint8_t wp_grp_size : 7;
//  uint8_t wp_grp_enable : 1;
//  uint8_t reserved_3 : 2;
//  uint8_t r2w_factor : 3;
//  uint8_t write_bl_len : 4;
//  uint8_t write_bl_partital : 1;
//  uint8_t reserved_4 : 5;
//  uint8_t file_format_grp : 1;
//  uint8_t copy : 1;
//  uint8_t perm_write_protect : 1;
//  uint8_t tmp_write_protect : 1;
//  uint8_t file_format : 2;
//  uint8_t reserved_5 : 2;
//  uint8_t crc : 7;
//  uint8_t always_one : 1;
//} SD_CSD;

typedef struct {
  uint8_t sturcture;
  uint8_t taac;
  uint8_t nsac;
  uint8_t tran_spd;
  uint16_t ccc;
  uint8_t read_bl_len;
  uint8_t read_bl_partial;
  uint8_t write_blk_misalign;
  uint8_t read_blk_misalign;
  uint8_t dsr_imp;
  uint32_t c_size;
//  uint8_t vdd_r_curr_min;
//  uint8_t vdd_r_curr_max;
//  uint8_t vdd_w_curr_min;
//  uint8_t vdd_w_curr_max;
//  uint8_t c_size_mult;
  uint8_t erase_blk_en;
  uint8_t sector_size;
  uint8_t wp_grp_size;
  uint8_t wp_grp_enable;
  uint8_t r2w_factor;
  uint8_t write_bl_len;
  uint8_t write_bl_partital;
  uint8_t file_format_grp;
  uint8_t copy;
  uint8_t perm_write_protect;
  uint8_t tmp_write_protect;
  uint8_t file_format;
  uint8_t crc;
  uint8_t always_one;
} SD_CSD;


uint8_t download_sdhc_data(void);
uint8_t esdhc_init(void);
uint8_t sd_card_init(uint8_t);
uint8_t sd_card_init_callback(void);
uint8_t sd_card_rw(void);
uint8_t sd_card_log(uint8_t,SDHC_LOGGING_CMDS,uint8_t*);
uint8_t log2sdhc(void);
extern const ESDHC_META empty_sd_meta;

#endif
