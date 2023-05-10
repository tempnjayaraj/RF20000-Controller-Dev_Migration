/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              common.h
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

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define GPIO_PIN_MASK                   0x1Fu
#define GPIO_PIN(x)                     (((1)<<(x & GPIO_PIN_MASK)))
#define BLUE_LED                        GPIO_PIN(26)
#define YELLOW_LED                      GPIO_PIN(25)
#define RED_LED                         GPIO_PIN(24)

/* Endianness adjustment utility */
/* Note this utility can be replaced by uint32_t __rev16(uint32_t) from NEON */
#define swap_4bytes(ptr)                {uint8_t byte; \
					byte = (ptr)[0]; \
                                        (ptr)[0] = (ptr)[3]; \
                                        (ptr)[3]=byte; \
					 byte = (ptr)[1]; \
                                        (ptr)[1] = (ptr)[2]; \
                                        (ptr)[2]=byte; \
                                        }
                                          
/* Common data types */
typedef enum {FALSE, TRUE} bool;
typedef char * STRING;

#endif

