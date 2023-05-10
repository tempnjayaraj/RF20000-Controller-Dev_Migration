/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2802                 $:
$Date:: 2021-12-07 10:49:57#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
@file     adc.h
@brief     ADC device driver
@details   This is the header file for ADC device driver which enables the
app to talk to the ADC.
*******************************************************************************/

#ifndef ADC_H
#define ADC_H

#ifdef TEST
#include "artc_arm_math.h"
#else
#include "arm_math.h"
#endif

/** Conversion Complete (COCO) mask */
#define COCO_NOT          0x00

/** How many to average prior to "interrupting" the MCU?  4, 8, 16, or 32 */
#define AVGS_4             0x00
#define AVGS_8             0x01
#define AVGS_16            0x02
#define AVGS_32            0x03

#define INTERNAL_TEMP_THRESHOLD 75.0f
#define INTERNAL_FAULT_TEMP_THRESHOLD 100.0f
/** minimum datasheet spec for pmc bandgap is 0.97v */
#define MIN_BANDGAP_SPEC        19263
/** maximum datasheet spec for pmc bandgap is 1.03v */
#define MAX_BANDGAP_SPEC        20455


/** Structure for one second or less pulse accumulator */
typedef struct {
  uint16_t sample;
  //uint64_t previous_sum;
  uint32_t fifo[1000];
  uint64_t sum;
}PULSE_ACCUMULATOR;

uint8_t adc_init(void);
void ADC_DMA_Init(void);
uint8_t irms_overcurrent(void);
uint8_t ADC_Cal(ADC_MemMapPtr);
uint8_t poll_adc_data(void);
float32_t get_temp(uint16_t);
uint8_t adc3_coco(void);
uint8_t irms2_adc_result(void);
uint8_t legacy_irms_adc_result(void);
uint8_t legacy_irms_over_energy(void);
void legacy_timer_wands (void);
uint8_t adc_init_2(void);
uint8_t adc0_coco_stator(void);
uint8_t print_debug (void);


#endif
