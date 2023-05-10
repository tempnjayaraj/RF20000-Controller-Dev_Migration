/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              crc.h
Author:            Chris Wallis
Date:              20-MAY-2013
Modified:          
CPU:               Freescale Kinetis P/N MK20DN512ZVLQ10 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments: N/A

NOTE: N/A

*******************************************************************************/

#ifndef CRC_H
#define CRC_H

#include <stdint.h>

extern uint16_t   __checksum;
extern uint32_t   __checksum_begin;
extern uint32_t   __checksum_end;

extern uint16_t   __checksum_main;
extern uint32_t   __checksum_begin_main;
extern uint32_t   __checksum_end_main;

extern uint16_t slow_crc16(uint16_t, 
                           uint8_t *, 
                           uint32_t);

extern uint16_t fast_crc16(uint16_t, 
                           uint8_t *, 
                           uint32_t);

#endif
