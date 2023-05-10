/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              cpu.h
Author:            Chris Wallis
Date:              01-FEB-2013
Modified:          
CPU:               Freescale Kinetis P/N MK20DN512ZVLQ10 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments: N/A

NOTE: N/A

*******************************************************************************/

#ifndef CPU_H
#define CPU_H
#include <stdint.h>

/* Function Prototypes */
void Write_Vtor(int);
void enable_irq(uint8_t);
void disable_irq(uint8_t);
uint8_t system_reset(void);

#endif
