/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              isr.c
Author:            Chris Wallis
Date:              01-FEB-2013
Modified:          
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
 * Comments:
 * Configure interrupt vector table and run interrupt service routines.
*******************************************************************************/

#include "isr.h"
#include "MK26F18.h"
#include <stdio.h>

/******************************************************************************
* Vector Table
******************************************************************************/
//typedef void (*vector_entry)(void);

#pragma location = ".intvec" 
const vector_entry  __vector_table[] = //@ ".intvec" =
{
   VECTOR_000,           /* Initial SP           */
   VECTOR_001,           /* Initial PC           */
   VECTOR_002,
   VECTOR_003,
   VECTOR_004,
   VECTOR_005,
   VECTOR_006,
   VECTOR_007,
   VECTOR_008,
   VECTOR_009,
   VECTOR_010,
   VECTOR_011,
   VECTOR_012,
   VECTOR_013,
   VECTOR_014,
   VECTOR_015,
   VECTOR_016,
   VECTOR_017,
   VECTOR_018,
   VECTOR_019,
   VECTOR_020,
   VECTOR_021,
   VECTOR_022,
   VECTOR_023,
   VECTOR_024,
   VECTOR_025,
   VECTOR_026,
   VECTOR_027,
   VECTOR_028,
   VECTOR_029,
   VECTOR_030,
   VECTOR_031,
   VECTOR_032,
   VECTOR_033,
   VECTOR_034,
   VECTOR_035,
   VECTOR_036,
   VECTOR_037,
   VECTOR_038,
   VECTOR_039,
   VECTOR_040,
   VECTOR_041,
   VECTOR_042,
   VECTOR_043,
   VECTOR_044,
   VECTOR_045,
   VECTOR_046,
   VECTOR_047,
   VECTOR_048,
   VECTOR_049,
   VECTOR_050,
   VECTOR_051,
   VECTOR_052,
   VECTOR_053,
   VECTOR_054,
   VECTOR_055,
   VECTOR_056,
   VECTOR_057,
   VECTOR_058,
   VECTOR_059,
   VECTOR_060,
   VECTOR_061,
   VECTOR_062,
   VECTOR_063,
   VECTOR_064,
   VECTOR_065,
   VECTOR_066,
   VECTOR_067,
   VECTOR_068,
   VECTOR_069,
   VECTOR_070,
   VECTOR_071,
   VECTOR_072,
   VECTOR_073,
   VECTOR_074,
   VECTOR_075,
   VECTOR_076,
   VECTOR_077,
   VECTOR_078,
   VECTOR_079,
   VECTOR_080,
   VECTOR_081,
   VECTOR_082,
   VECTOR_083,
   VECTOR_084,
   VECTOR_085,
   VECTOR_086,
   VECTOR_087,
   VECTOR_088,
   VECTOR_089,
   VECTOR_090,
   VECTOR_091,
   VECTOR_092,
   VECTOR_093,
   VECTOR_094,
   VECTOR_095,
   VECTOR_096,
   VECTOR_097,
   VECTOR_098,
   VECTOR_099,
   VECTOR_100,
   VECTOR_101,
   VECTOR_102,
   VECTOR_103,
   VECTOR_104,
   VECTOR_105,
   VECTOR_106,
   VECTOR_107,
   VECTOR_108,
   VECTOR_109,
   VECTOR_110,
   VECTOR_111,
   VECTOR_112,
   VECTOR_113,
   VECTOR_114,
   VECTOR_115,
   CONFIG_1,        /* Flash configuration field values */
   CONFIG_2,        /* Flash configuration field values */
   CONFIG_3,        /* Flash configuration field values */
   CONFIG_4,        /* Flash configuration field values */

}; /* VECTOR_TABLE end */

/******************************************************************************
* nmi_isr(void)
*
* Non-maskable interrupt.
*
* Note: Current hardware does not support this function so switch pin function
* to avoid getting this interrupt.
******************************************************************************/
void nmi_isr(void)
{
  /* Turn on gate clock */
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
  
  /* Switch pin function to GPIO */
  PORTA_PCR4 = PORT_PCR_MUX(1);  
  
//  /* Alert that unhandled interrupt occured */
//  GPIOA_PCOR = 0x4000000;
  
}

/******************************************************************************
* default_isr(void)
*
* Default ISR definition.
*
* In:  n/a
* Out: n/a
******************************************************************************/
void default_isr(void)
{
   #define VECTORNUM (*(volatile uint8_t*)(0xE000ED04))

  //printf("\n****default_isr entered on vector %d*****\r\n\n",VECTORNUM);
  
  /* Alert that unhandled interrupt occured */
  //GPIOA_PCOR = 0x4000000;

  for(short n=0;n<200;n++)
  {
    asm("nop");
  } 
    
  return;
  
}
/******************************************************************************/

/******************************************************************************
* hard_fault_handler_asm(void)
*
* 
******************************************************************************/

void hard_fault_handler_asm(void)
{
//  asm("TST LR, #4");
//  asm("ITE EQ");
//  asm("MRSEQ R0, MSP");
//  asm("MRSNE R0, PSP");
//  asm("B hard_fault_handler_c");
}


/******************************************************************************
* hard_fault_handler_c(void)
*
* 
******************************************************************************/
void hard_fault_handler_c (unsigned int * hardfault_args)
{
  
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;
 
  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);
 
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr = ((unsigned long) hardfault_args[5]);
  stacked_pc = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);
 
  printf ("\n\n[Hard fault handler - all numbers in hex]\n");
  printf ("R0 = %x\n", stacked_r0);
  printf ("R1 = %x\n", stacked_r1);
  printf ("R2 = %x\n", stacked_r2);
  printf ("R3 = %x\n", stacked_r3);
  printf ("R12 = %x\n", stacked_r12);
  printf ("LR [R14] = %x  subroutine call return address\n", stacked_lr);
  printf ("PC [R15] = %x  program counter\n", stacked_pc);
  printf ("PSR = %x\n", stacked_psr);
  printf ("BFAR = %x\n", (*((volatile unsigned long *)(0xE000ED38))));
  printf ("CFSR = %x\n", (*((volatile unsigned long *)(0xE000ED28))));
  printf ("HFSR = %x\n", (*((volatile unsigned long *)(0xE000ED2C))));
  printf ("DFSR = %x\n", (*((volatile unsigned long *)(0xE000ED30))));
  printf ("AFSR = %x\n", (*((volatile unsigned long *)(0xE000ED3C))));
  //printf ("SCB_SHCSR = %x\n", SCB->SHCSR);
 
  while(1);
  
}