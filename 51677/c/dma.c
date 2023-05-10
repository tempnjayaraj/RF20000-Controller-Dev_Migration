/********************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
*******************************************************************************/
/*******************************************************************************
$Author: Harshil.Shah $
$Date: 2021-12-01 22:47:34 -0600 (Wed, 01 Dec 2021) $
$Modtime: 7/02/13 11:17a $
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     dma.c
\brief     Direct memory access driver.
\details   N/A
*******************************************************************************/
#include "MK26F18.h"

/****************************************************************************//**
Initialize DMA.
@param void
@return void
@outputs
@context
********************************************************************************/
uint8_t dma_mux_init(void)
{

  /* Enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
    
  /* Clear errors */
  DMA_CERR |= DMA_CERR_CAEI_MASK;
  
  /* Clear all enable requests */
  DMA_CERQ |= DMA_CERQ_CAER_MASK;
  
  /* Clear all done bits */
  DMA_CDNE |= DMA_CDNE_CADN_MASK;

  /* Clear all interrupt requests */
  DMA_CINT |= DMA_CINT_CAIR_MASK;
  
// /*****************************************************************************
//  * Rearrange priority:
//  *  1. swap ch 0 w/ ch 9 (SmartWandUART0) remedy O.R. issue
//  *  2. swap ch 0 - 4 w/ ch 10, 11, 15, 6, & 12
//  *****************************************************************************/  
//  DMA_DCHPRI9 &= ~DMA_DCHPRI9_CHPRI_MASK;
//  DMA_DCHPRI10 = DMA_DCHPRI10_CHPRI(1);
//  DMA_DCHPRI11 = DMA_DCHPRI11_CHPRI(2);
//  DMA_DCHPRI15 = DMA_DCHPRI15_CHPRI(3);
//  DMA_DCHPRI6 =  DMA_DCHPRI6_CHPRI(4);
//  DMA_DCHPRI12 = DMA_DCHPRI12_CHPRI(5);
//  DMA_DCHPRI0 = DMA_DCHPRI0_ECP_MASK | DMA_DCHPRI0_CHPRI(6);
//  DMA_DCHPRI1 = DMA_DCHPRI1_ECP_MASK | DMA_DCHPRI1_CHPRI(7);
//  DMA_DCHPRI2 = DMA_DCHPRI2_ECP_MASK | DMA_DCHPRI2_CHPRI(8);
//  DMA_DCHPRI3 = DMA_DCHPRI3_ECP_MASK | DMA_DCHPRI3_CHPRI(9);
//  DMA_DCHPRI4 =  DMA_DCHPRI4_CHPRI(10);
//  DMA_DCHPRI5 =  DMA_DCHPRI5_CHPRI(11);
//  DMA_DCHPRI7 =  DMA_DCHPRI7_CHPRI(12);
//  DMA_DCHPRI8 =  DMA_DCHPRI8_CHPRI(13);
//  DMA_DCHPRI13 = DMA_DCHPRI13_CHPRI(14);
//  DMA_DCHPRI14 = DMA_DCHPRI14_CHPRI(15);
  
  return 1;
    
}
 
