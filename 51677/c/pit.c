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
\file:     pit.c
\brief     Programmable interrupt timer(s).
\details   N/A
*******************************************************************************/
#include "MK26F18.h"
#include "pit.h"
#include "cpu.h"

/****************************************************************************//**
Initialize PIT.
@param void
@return void
@outputs
@context
********************************************************************************/
void pit_init(void){
  
  uint8_t channel = 0;
  
  uint32_t start_value[4] = {370000000,112000000,7400000,740000};
  
  /* Enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; 
  
  /* Enable clock for timer */
  PIT_MCR &= ~PIT_MCR_MDIS_MASK;
  
  for (channel = 0; channel < 4; channel++)
  {
    
    PIT_Config(channel, start_value[channel]);
    
  }
  
 
//  /* Enable pit1 irq */
//  NVIC_EnableIRQ(69);
//  
//  /* Enable pit2 irq */
//  NVIC_EnableIRQ(70);
//  
  
  /* enable pit3 irq */
  NVIC_EnableIRQ(PIT3_IRQn);
    
}

/****************************************************************************//**
Configure PIT
@param ch - channel number
@param value - 32 bit timeout value
@return void
@outputs
@context
********************************************************************************/
void PIT_Config(uint8_t ch, uint32_t value){

  PIT_MemMapPtr pPIT = PIT_BASE_PTR;
                 
    /* Set 100 ms timeout value */
  pPIT->CHANNEL[ch].LDVAL = value;
                
  /* Clear pending interrupts */
  pPIT->CHANNEL[ch].TFLG |= PIT_TFLG_TIF_MASK;
  
  /* Enable interrupt & timer */
  pPIT->CHANNEL[ch].TCTRL = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
    
}

///******************************************************************************
//* Function: PIT1_Init
//*
//* Purpose: Timer to disable pump due to inactivity. 
//******************************************************************************/
//
//void PIT1_Init(void) {
//  
//  /* Set 100 ms timeout value */
//  PIT_LDVAL1 =  7400000;
//                
//  
//  /* Enable interrupt */
//  PIT_TCTRL1 |= PIT_TCTRL_TIE_MASK;
//  
//  RESET_PIT(1);
//  
//
//  
//}
//
///******************************************************************************
//* Function: PIT0_init
//*
//* Purpose: Initialize timeout timer.
//*
//* Note: 
//******************************************************************************/
//
//void PIT0_Init(void) {
//  
//  /* Clear pending interrupts */
//  PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
//  
//  /* Enable interrupt */
//  PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK;
//  
//}
//
//
///******************************************************************************
//* Function: PIT2_init
//*
//* Purpose: Initialize timeout timer.
//*
//* Note: 
//******************************************************************************/
//
//void PIT2_Init(void)
//{
//
//  /* Set 100 ms timeout value */
//  PIT_LDVAL2 =  7400000;
//                
//  
//  /* Clear pending interrupts */
//  PIT_TFLG2 |= PIT_TFLG_TIF_MASK;
//  
//  /* Enable interrupt */
//  PIT_TCTRL2 |= PIT_TCTRL_TIE_MASK;
//  
//  /* Enable end of timer period interrupt request in core */
//  NVIC_EnableIRQ(70);
//  
//  /* Enable timeout */
//  PIT_TCTRL2 |= PIT_TCTRL_TEN_MASK;
//  
//}


