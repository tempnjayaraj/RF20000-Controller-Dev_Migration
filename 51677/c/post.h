/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1044                 $:
$Date:: 2014-04-22 16:20:36#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      post.h
\brief     Header file for power on self test code
\details   NA
*******************************************************************************/

#ifndef POST_H
#define POST_H

uint8_t post(void);
uint8_t post_init(void);
uint8_t post_exit(void);
uint8_t get_mfg_date(void);
uint8_t get_serial_num(void);
void sn_acii2bcd(uint8_t *, uint8_t *);

#endif
