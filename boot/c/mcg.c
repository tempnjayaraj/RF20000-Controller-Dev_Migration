/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              mcg.c
Author:            Chris Wallis
Date:              01-FEB-2013
Modified:          
CPU:               Freescale Kinetis P/N MK20DN512ZVLQ10 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments:
Multipurpose clock generator driver.

NOTE: N/A

*******************************************************************************/

#include "mcg.h"
#include "MK60F15.h"

/******************************************************************************
* Function: pll_init
*
* Purpose: Ramp up the system clock
******************************************************************************/

void PLL_Init(void) {
  
  
  SIM_CLKDIV4 &= ~SIM_CLKDIV4_TRACEDIV(1);
  
  SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;
  
  PORTA_PCR6 |= PORT_PCR_MUX(7); // TRACE_CLKOUT
  
  // Need to start a timeout here so different clock config can be started if
  // the one intended doesn't start correctly.
  
  //SIM_SCGC1 |= SIM_SCGC1_OSC1_MASK;
  
  //OSC0_CR |= OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
  //OSC1_CR |= OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
  
  /* Oscillator used, change XTAL0 to PTA19 GPIO */
  PORTA_PCR19 = PORT_PCR_MUX(2); 
  
  /* First transition FEI (reset default) to FBE mode */
  MCG_C2 = MCG_C2_IRCS_MASK | MCG_C2_RANGE0(1);
  //MCG_C10 = MCG_C10_RANGE1(2) | MCG_C10_HGO1_MASK;

  /* Select external reference clock & FLL divider 32MHz/1024 = 31.250kHz */
  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(5);
  
  /* Wait for reference clock to switch to external reference */
  while (MCG_S & MCG_S_IREFST_MASK);

  /* Wait for MCGOUT to switch over to the external reference clock */
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2);

  /* Set PLL reference frequency to 32MHz / 4 = 8 MHz */
  MCG_C5 = MCG_C5_PRDIV0(3); 
  
  /* Transition FBE to BLPE to PBE mode */
  MCG_C6 = MCG_C6_CME0_MASK | MCG_C6_PLLS_MASK | MCG_C6_VDIV0(21);

  /* Wait until the source of the PLLS clock has switched to the PLL */
  while (!(MCG_S & MCG_S_PLLST_MASK));
  
  /* Wait until the PLL has achieved lock */
  while (!(MCG_S & MCG_S_LOCK0_MASK));
  
  /* --------------------------------------------------------------------------
   * Set up clock dividers BEFORE switching to the PLL to ensure system clock
   * speeds are in spec.
   * core = PLL (148 MHz), bus = PLL/2 (74 MHz), flexbus = PLL/3 (49.3 MHz), 
   * flash = PLL/4 (37MHz)
   * -------------------------------------------------------------------------*/
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) 
              | SIM_CLKDIV1_OUTDIV3(2) | SIM_CLKDIV1_OUTDIV4(6);

  /* Transition into PEE */
  MCG_C1 &= ~MCG_C1_CLKS_MASK;
  
  /* Wait for MCGOUT to switch over to the PLL, MCGOUTCLK is 148MHz */
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){}
    
  /* Switch ON MCGIRCLK */
  MCG_C1 |= MCG_C1_IRCLKEN_MASK;
  


}