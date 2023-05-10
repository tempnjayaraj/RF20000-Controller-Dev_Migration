/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
*******************************************************************************/
/*******************************************************************************
$Author:: Harshil.Shah    $: Author
$Rev:: 2798               $: Revision of last commit
$Date:: 2021-11-17 02:10:#$: Date of last commit

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
@file     ad7265.h
@brief     ADC7265 device driver
@details   This is the header file for ADC7265 device driver which enables the
app to talk to the ADC7265 which interfaces with legacy wand.
*******************************************************************************/

#ifndef AD7265_H
#define AD7265_H

#include "common.h"

#ifdef REV4

#define AD7265_MAX_CH               6
#define AD7265_MUX_CLEAR            0x0E
#define AD7265_MUX_CH1              0x00
#define AD7265_MUX_CH2              0x08
#define AD7265_MUX_CH3              0x04
#define AD7265_MUX_CH4              0x0C
#define AD7265_MUX_CH5              0x02
#define AD7265_MUX_CH6              0x0A

#elif REV5
/* For REV5 board
  A0 is connected to PTD0
  A1 is connected to PTC19
  A2 is connected to PTC18

So for this implementation, A0, A1 and A2 are defined
separately and the code has to toggle those pins
accordingly

*/
//This will correspond to BIT0. This should be used on PORTD
#define ISO_A0_A2D (0x1)
#define ISO_A1_A2D (((uint32_t)0x1)<<19) // This will be pin 19
#define ISO_A2_A2D (((uint32_t)0x1)<<18) // This will be pin 18
//#define VREF 2500 //2500 mv
#define RANGE 2 // This gives effective ref voltage of RANGE*REF

// B is listed before A because data for B is stored before data for
// A. (The DMA is negatively incremented to help with Endianess)

typedef enum
{
  B1,
  A1,
  B2,
  A2,
  B3,
  A3,
  B4,
  A4,
  B5,
  A5,
  B6,
  A6} CHAN_NUM;


#endif

void AD7265_Init(void);
uint16_t IRQ_AD7265_Read(char channel);
void AD7265_DMA_Init(void);
uint16_t AD7265_read_value (CHAN_NUM channel_num);

#endif
