/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: cwalli00          $:
$Rev:: 2794                 $:
$Date:: 2021-11-04 10:49:01#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      fuse_blow.c
\brief     Driver to blow the fuse on the legacy wand.
\details   This module is used to blow the fuse
on older type wands. The microcontroller, polls the ADC channels to see if a
wand is connected to the board. Once a wand is detected, the micro waits for
one second to allow for switch debouncing to settle and then pulls the
FUSE_BLOW line low. This activates a FET which connects a charged cap across the
fuse blowing it. Once the fuse is blow, the micro reads the ADC values again and
makes sure, the fuse is blown.
*******************************************************************************/
#include "MK26F18.h"
#include "ad7265.h"
#include "fuse_blow.h"

/***************************************************************************//**
This function pulls the FUSE_BLOW line high which blows the fuse.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void blow_fuse (void)
{
   // The FUSE_BLOW line is connected to port C pin 16. Each pin can be
   // configured in many ways. Setting the PCR register MUX value to 1,
   // makes the pin a GPIO.
   PORTC_PCR16 = PORT_PCR_MUX(1);
   //Set the direction of the pin to output (Set bit 16 to 1)
   // 0 - Input , 1 -  output
   GPIOC_PDDR |= GPIO_PDDR_PDD(0x00010000);
   // Now set the bit to high
   GPIOC_PSOR |= 0x00010000;

}

/***************************************************************************//**
This function pulls the FUSE_BLOW line low.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void clear_fuse_blow (void)
{
   // The FUSE_BLOW line is connected to port C pin 16. Each pin can be
   // configured in many ways. Setting the PCR register MUX value to 1,
   // makes the pin a GPIO.
   PORTC_PCR16 = PORT_PCR_MUX(1);
   //Set the direction of the pin to output (Set bit 16 to 1)
   // 0 - Input , 1 -  output
   GPIOC_PDDR |= GPIO_PDDR_PDD(0x00010000);
   
   // Now clear the bit
   GPIOC_PCOR |= 0x00010000;
   
}




