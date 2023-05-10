/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2817                 $:
$Date:: 2022-02-18 03:41:55#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/

/***************************************************************************//**
\file:     lpt.c
\brief     Low Power Timer driver.
\details   N/A
*******************************************************************************/
#include "MK26F18.h"
#include "cpu.h"
/***************************************************************************//**
Initializes the low power timer.
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t lpt_init(void)
{

  /* enable gate clock */
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
  
  /* clear ctrl & status */
  LPTMR0_CSR = 0;
  
  /* lptmr0_alt1 */
  PORTA_PCR19 = PORT_PCR_MUX(6);
  
  /* use MCGIRCLK */
  LPTMR0_PSR = LPTMR_PSR_PCS(0);
  
  /* enable prescaler */
  //LPTMR0_PSR &= ~LPTMR_PSR_PBYP_MASK;
  
  /* set prescale 2MHz/256 = 7.8125kHz */
  LPTMR0_PSR = LPTMR_PSR_PRESCALE(7) | LPTMR_PSR_PBYP_MASK;
  
  /* set compare to 9 pulses */
  LPTMR0_CMR = 11;
  
  /* pulse counter mode, lptmr0_alt1 input, falling edge */
  LPTMR0_CSR = LPTMR_CSR_TPS(1) |
               LPTMR_CSR_TPP_MASK |
               LPTMR_CSR_TMS_MASK |
               LPTMR_CSR_TEN_MASK |
               LPTMR_CSR_TIE_MASK;

  NVIC_EnableIRQ(LPTMR0_IRQn);
  
  return 1;
  
}

/***************************************************************************//**
Reset the low power timer pulse counter
@param void
@return 1
@outputs
@context
*******************************************************************************/
uint8_t reset_pulse_cnt(void)
{

  /* strobe lpt pulse counter */
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
  
  return 1;

}
