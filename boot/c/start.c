/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              start.c
Author:            Chris Wallis
Date:              01-FEB-2013
Modified:          
CPU:               Freescale Kinetis P/N MK20DN512ZVLQ10 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments:
Start up routines before branching to main application.

NOTE:
See Microcontroller Software Consideration notes.
http://www.freescale.com/infocenter/topic/microcontrollers/index.html

*******************************************************************************/

#include "MK26F18.h"
#include "start.h"
#include "system.h"
#include "wdog.h"
#include "crc.h"
#include <stdlib.h>

__root const unsigned char checksum_end[4] @ "checksum_end_mark" = {0,0,0,0xEE};

void start(void)
{
  
  uint16_t calc_main = 0;
  
  /* disable all interrupts */
  asm(" CPSID i");
  
  /* disable watchdog */
  wdog_disable();
  
  /* copy rom data to ram */
  rom2ram();
  
  /* calculate checksum for main app in rom -- takes ~400ms to complete */
  calc_main = fast_crc16(calc_main,
                         (uint8_t *)&__checksum_begin_main, 
                         (uint32_t)&checksum_end[3] - (uint32_t)&__checksum_begin_main + 1);
  
  /* compare the calculated checksum with the stored */
  if ((uint16_t)__checksum_main != calc_main)
  {
    
    abort();
          
  }
      

  
  /* enable port clocks */
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK |
                SIM_SCGC5_PORTB_MASK |
                SIM_SCGC5_PORTC_MASK |
                SIM_SCGC5_PORTD_MASK |
                SIM_SCGC5_PORTE_MASK );
  
  /* Initialize LEDs on pcb*/
  PORTA_PCR24 = PORT_PCR_MUX(1); // GPIO, RED LED
  PORTA_PCR25 = PORT_PCR_MUX(1); // GPIO, YELLOW LED
  PORTA_PCR26 = PORT_PCR_MUX(1); // GPIO, BLUE LED

  /* set pcb LED pins as outputs */
  GPIOA_PDDR |= GPIO_PDDR_PDD(RED_LED | BLUE_LED | YELLOW_LED);
    
  /* turn pcb LEDs OFF */
  GPIOA_PSOR |= GPIO_PDOR_PDO(RED_LED | BLUE_LED | YELLOW_LED);
  
  /* start application */
  main();
  
  /* disable all interrupts */
  asm(" CPSID i");

  /* halt system */
  exit(EXIT_FAILURE);
  
}

/******************************************************************************
* rom2ram(void)
*
* Copy all ROM data to RAM.
*
******************************************************************************/
void rom2ram(void)
{
  
  #pragma section = ".data"
  #pragma section = ".data_init"
  #pragma section = ".bss"

  /* Declare a counter we'll use in all of the copy loops */
  uint32_t n;

  /* Declare pointers for various data sections. These pointers
   * are initialized using values pulled in from the linker file
   */
  uint8_t * data_ram, * data_rom, * data_rom_end;
  uint8_t * bss_start, * bss_end;

  /* Get the addresses for the .data section (initialized data section) */
  data_ram = __section_begin(".data");
  data_rom = __section_begin(".data_init");
  data_rom_end = __section_end(".data_init");
  n = data_rom_end - data_rom;
              
  /* Copy initialized data from ROM to RAM */
  while (n--)
    *data_ram++ = *data_rom++;
           
  /* Get the addresses for the .bss section (zero-initialized data) */
  bss_start = __section_begin(".bss");
  bss_end = __section_end(".bss");

  /* Clear the zero-initialized data section */
  n = bss_end - bss_start;
  while(n--)
  *bss_start++ = 0;
  
}
/*****************************************************************************/