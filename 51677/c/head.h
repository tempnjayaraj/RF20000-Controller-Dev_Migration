/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: cwalli00          $:
$Rev:: 1992                 $:
$Date:: 2017-02-01 17:24:49#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      head.h
\brief     Head file for head.s assembly file which lays out the software in
memory.
*******************************************************************************/

#ifndef HEAD_H
#define HEAD_H
#define APP_NAME "RF20000P"
/* macros acct for arm padding requirement */
#define SIZEOF_APP_NAME         ((sizeof(APP_NAME)+2)&~0x3)
#define SIZEOF_COMPILE_DATE     (sizeof(__DATE__)+3)&~0x3
#define SIZEOF_COMPILE_TIME     (sizeof(__TIME__)+3)&~0x3
typedef struct 
{
  uint32_t  ROM_address;   // ROM address, defined in icf
  uint32_t ropi_bytes;    // Number of RO bytes, including this header.
  uint32_t rwpi_data_bytes;    // Number of RW bytes.
  uint32_t rwpi_zi_bytes;
  uint8_t app_name[SIZEOF_APP_NAME];
  uint8_t compile_date[SIZEOF_COMPILE_DATE];
  uint8_t compile_time[SIZEOF_COMPILE_TIME];
  uint32_t complier_ver;
  uint8_t app_version_major;
  uint8_t app_version_minor;
  uint8_t padding0[2];
  uint32_t start_offset;  // Offset from start of code to the main function in ropi_rwpi_program, start of code is the first byte after this header.
}program_header_layout;

typedef struct
{
  uint32_t wireless_pair_timeout;
}program_header_params;

typedef struct
{
  program_header_layout meta;
  program_header_params params;
}program_header;
 

#endif
