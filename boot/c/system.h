/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              start.h
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

#ifndef SYSTEM_H
#define SYSTEM_H

/* Function Prototypes */
//void main(void);

#define FLASH_DATA_PTR                  0xE000
#define APP1_ADDRESS                    0x10004
#define APP2_ADDRESS                    0x40004

#define GPIO_PIN_MASK                   0x1Fu
#define GPIO_PIN(x)                     (((1)<<(x & GPIO_PIN_MASK)))
#define BLUE_LED                        GPIO_PIN(26)
#define YELLOW_LED                      GPIO_PIN(25)
#define RED_LED                         GPIO_PIN(24)

static void jump2application(uint32_t);
uint32_t calc_jmp_ptr(void);
uint8_t search4valid_app(uint32_t, uint8_t*, uint8_t*);

extern uint8_t __APP_PTR[];


#endif
