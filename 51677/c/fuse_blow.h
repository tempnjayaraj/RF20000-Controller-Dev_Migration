/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1479                 $:
$Date:: 2014-10-03 14:50:26#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      fuse_blow.h
\brief     Header file for fuse blow driver.
\details   This is the header file for fuse_blow.c. This module is used to blow
the fuse on older type wands. The microcontroller, polls the ADC channels to see
if a wand is connected to the board. Once a wand is detected, the micro waits
for one second to allow for switch debouncing to settle and then pulls the
FUSE_BLOW line low. This activates a FET which connects a charged cap across the
fuse blowing it. Once the fuse is blow, the micro reads the ADC values again and
makes sure, the fuse is blown.
*******************************************************************************/
#ifndef FUSE_BLOW_H
#define FUSE_BLOW_H

void blow_fuse (void);
void clear_fuse_blow (void);

#endif // FUSE_BLOW_H
