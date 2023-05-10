/******************************************************************************
                   ArthroCare Proprietary and Confidential
                   (c) ArthroCare Corporation 2012
                           All Rights Reserved
*******************************************************************************/
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
#define GPIO_PIN(x)                     (((1)<<((x) & GPIO_PIN_MASK)))
#define CORE_CLK_MHZ                    148     
#define BUS_CLK_MHZ                     74
#define RF_FREQ_KHZ                     100
#define BLUE_LED                        GPIO_PIN(26)
#define YELLOW_LED                      GPIO_PIN(25)
#define RED_LED                         GPIO_PIN(24)
#define WAND_OUTPUT_SELECT              GPIO_PIN(10)
#define WAND_PILOT_SELECT               GPIO_PIN(9)
#define EXT_CTRL_2                      GPIO_PIN(6)
#define EXT_CTRL_3                      GPIO_PIN(7)
/* number of levels that can be represented using a 12bit number */
#define MAX_12BIT_NUM                   4096u 

#define RF_SW_DISABLE                   GPIO_PDOR_PDO(GPIO_PIN(8))


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

/* Convert half precision float to single */
#define htos(h) (((h)&0x8000)<<16) | ((((h)&0x7c000)+0x1c000)<<13) | (((h)&0x03FF)<<13)
 typedef enum {FALSE, TRUE} bool;                                         
/* Common data types */

typedef char * STRING;


#endif

