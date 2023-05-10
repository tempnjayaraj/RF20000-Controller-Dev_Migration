/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2821                 $:
$Date:: 2022-03-07 12:49:31#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/

/***************************************************************************//**
\file      ftfe.c
\brief     Driver for flash memory module
\details  
\todo More description
*******************************************************************************/
#ifndef FTFE_H
#define FTFE_H

#include <stdint.h>
#include "MK26F18.h"

#define FTFE_PROGRAM_PHRASE     0x07
#define FTFE_ERASE_SECTOR       0x09
#define FTFE_PROGRAM_ONCE_CMD   0x43
#define FTFE_READ_ONCE_CMD      0x41
#define FTFE_PROGRAM_SECTION    0x0B
#define FTFE_ERASE_ALL_BLOCKS   0X44
#define FTFE_PROGRAM_PARTITION  0x80
#define FTFE_SET_FLEXRAM_FN     0x81
#define FTFE_READ_SECTION       0x01

#define FTFE_ERR_FLAGS_MASK     (FTFE_FSTAT_MGSTAT0_MASK    \
                                 | FTFE_FSTAT_FPVIOL_MASK   \
                                 | FTFE_FSTAT_ACCERR_MASK   \
                                 | FTFE_FSTAT_RDCOLERR_MASK)
                                 
uint8_t program_ftfe_once(uint8_t*, uint8_t, uint8_t); 
uint8_t read_ftfe_once(uint8_t*, uint8_t, uint8_t);
uint8_t erase_ftfe_sector(uint32_t);
uint8_t write_ftfe_phrase(void); 
uint8_t save_params2flash(void);
uint8_t write_ftfe_section(uint32_t,uint16_t);
uint8_t *get_cached_nv_data(void);
uint8_t set_ftfe_function (void);
uint8_t program_ftfe_partition (void);
uint8_t read_ftfe_section(uint32_t destination,uint16_t nwords);

#endif
