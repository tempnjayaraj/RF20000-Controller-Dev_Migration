/********************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author: Harshil.Shah $
$Rev:: Rev$:
$Date: 2021-12-07 10:49:57 -0600 (Tue, 07 Dec 2021) $
$Modtime: 7/23/13 1:57p $
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/

/***************************************************************************//**
\file:     buck.h
\brief     Header file for DC-DC buck converter.
\details   N/A
*******************************************************************************/

#ifndef BUCK_H
#define BUCK_H

#include "MK26F18.h"
#include "common.h"
#ifdef TEST
#include "artc_arm_math.h"
#else
#include "arm_math.h"
#endif

#define SET_DAC(pDAC,x)  (pDAC)->DAT[1].DATL = ((x) & 0xFF); \
                         (pDAC)->DAT[1].DATH = ((x) & 0xF00) >> 8; \
                         (pDAC)->C2 = DAC_C2_DACBFUP(1) | DAC_C2_DACBFRP(1); \
                         (pDAC)->DAT[0].DATL = ((x) & 0xFF); \
                         (pDAC)->DAT[0].DATH = ((x) & 0xF00) >> 8; \
                         (pDAC)->C2 = DAC_C2_DACBFUP(1) | DAC_C2_DACBFRP(0);

#define GET_DAC(pDAC)    ((((pDAC)->DAT[0].DATH << 8) & 0xF00) | \
                         ((pDAC)->DAT[0].DATL & 0xFF));

#define BUCK_WW_ON      GPIO_PIN(24)
#define BUCK_18PIN_ON   GPIO_PIN(26)

typedef enum {
  RAMP_UP,
  VMOD_INTERP,
  VMOD_TEST_HOLD,
  RAMP_DOWN,
  VMOD_TEST_DONE,
  VMOD_TEST_RESET,
}VMOD_TEST_STATUS;

typedef struct {
  uint16_t mod1_meas;
  uint16_t mod2_meas;
  uint16_t vmod2_meas[MAX_12BIT_NUM];
  //uint16_t vmod1_meas[MAX_12BIT_NUM];
  uint16_t vmod2_lookup[MAX_12BIT_NUM];
  VMOD_TEST_STATUS vmod2_test_status;
  VMOD_TEST_STATUS vmod1_test_status;
}VMOD_COMPENSATION;

/* Function prototypes */
uint8_t buck_init(void);
uint16_t set_buck_voltage(uint16_t, uint16_t);
uint16_t ramp_down_buck_voltage(DAC_MemMapPtr,uint16_t);
uint8_t buck_inactive_shutdown(void);
uint8_t vmod2_test(void);
uint16_t set_dac(uint16_t);
void sort_vmeas(uint16_t*);
uint8_t interp2(uint8_t);
VMOD_TEST_STATUS vmod2_ramp_test(uint8_t);
uint16_t ramp_down_buck_test_voltage(DAC_MemMapPtr, uint16_t, uint8_t);
uint16_t legacy_set_buck_voltage(uint16_t, uint16_t);
uint16_t legacy_ramp_down_buck_voltage(uint16_t);
uint8_t vmod_interp(void);
VMOD_TEST_STATUS vmod1_ramp_test(uint8_t);
uint8_t vmod1_test(void);
uint16_t set_buck_voltage_constant_power(uint16_t irms, uint16_t current_dac, float32_t power_target, uint16_t vlimit);
unsigned char vmod1_adc_coco(void);
unsigned char vmod2_adc_coco(void);

extern const DAC_MemMapPtr pLegacyWandDAC;
extern const DAC_MemMapPtr pSmartWandDAC;
extern const uint16_t buck_log_sample_vector[512];
extern VMOD_COMPENSATION vmod;


#endif
