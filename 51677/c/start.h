/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1052                 $:
$Date:: 2014-04-23 13:48:59#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      start.h
\brief     Header file for startup module.
\details   NA
*******************************************************************************/

#ifndef START_H
#define START_H

/* Function Prototypes */

extern void main(void);
void rom2ram(void);
extern void __iar_data_init3(void);
//extern int ropi_rwpi_main(void);

#endif
