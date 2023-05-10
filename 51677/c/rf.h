/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2798                 $:
$Date:: 2021-11-17 02:10:26#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     rf.h
\brief     Header file for RF driver for controlled ablation i.e. "coblation".
\details   N/A
*******************************************************************************/

#ifndef RF_H
#define RF_H

#include <stdlib.h>
#include "pit.h"
#include "MK26F18.h"

#define MAX_FTM_CHANNELS 8
#define RF_MODULO                       ((BUS_CLK_MHZ * 1e3) / RF_FREQ_KHZ)

#define FAULT_FTM(x) (FTM_FMS_REG((x)) & FTM_FMS_FAULTIN_MASK)

#define SWOC_FTM(x) (FTM_SWOCTRL_REG((x)) & FTM_SWOCTRL_CH0OC_MASK)

#define RF_ON(x) FTM_SWOCTRL_REG(x) &= ~FTM_SWOCTRL_CH0OC_MASK & \
                                       ~FTM_SWOCTRL_CH1OC_MASK & \
                                       ~FTM_SWOCTRL_CH2OC_MASK & \
                                       ~FTM_SWOCTRL_CH3OC_MASK & \
                                       ~FTM_SWOCTRL_CH4OC_MASK & \
                                       ~FTM_SWOCTRL_CH5OC_MASK & \
                                       ~FTM_SWOCTRL_CH6OC_MASK & \
                                       ~FTM_SWOCTRL_CH7OC_MASK;  \
                 FTM_SYNCONF_REG((x)) |= FTM_SYNCONF_SWRSTCNT_MASK; \
                 FTM_SYNC_REG((x)) |= FTM_SYNC_REINIT_MASK | \
                                      FTM_SYNC_CNTMAX_MASK | \
                                      FTM_SYNC_SWSYNC_MASK;


#define RF_OFF(x) FTM_SWOCTRL_REG((x)) |= FTM_SWOCTRL_CH0OC_MASK | \
                                          FTM_SWOCTRL_CH1OC_MASK | \
                                          FTM_SWOCTRL_CH2OC_MASK | \
                                          FTM_SWOCTRL_CH3OC_MASK | \
                                          FTM_SWOCTRL_CH4OC_MASK | \
                                          FTM_SWOCTRL_CH5OC_MASK | \
                                          FTM_SWOCTRL_CH6OC_MASK | \
                                          FTM_SWOCTRL_CH7OC_MASK;  \
                  FTM_SYNCONF_REG((x)) &= ~FTM_SYNCONF_SWRSTCNT_MASK; \
                  FTM_SYNC_REG((x)) |= FTM_SYNC_REINIT_MASK | \
                                       FTM_SYNC_CNTMAX_MASK | \
                                       FTM_SYNC_SWSYNC_MASK;

/* DMA->FTM channel map */
#define FTM0_SYNC_DMA_CH        12
#define FTM0_C3V_DMA_CH         11        
#define FTM0_C1V_DMA_CH         10   
#define FTM0_C5V_DMA_CH         15
#define FTM0_C7V_DMA_CH         6
                                       
#define FTM0_C0V_DMA_CH         8
#define FTM0_C2V_DMA_CH         9                                       
#define FTM0_C4V_DMA_CH         3
#define FTM0_C6V_DMA_CH         2

#define FTM3_SYNC_DMA_CH        24
#define FTM3_C0V_DMA_CH         16                                               
#define FTM3_C1V_DMA_CH         17
#define FTM3_C2V_DMA_CH         18
#define FTM3_C3V_DMA_CH         19
#define FTM3_C4V_DMA_CH         20                                      
#define FTM3_C5V_DMA_CH         21
#define FTM3_C6V_DMA_CH         22                                       
#define FTM3_C7V_DMA_CH         23

//#define PWM_SMART_WAND  FTM0_BASE_PTR
                              
/* Function Prototypes */
uint8_t smartwand_pwm_init(void);
void LegacyWand_PWMInit(void);
void FTM_Init(FTM_MemMapPtr);
void FTM_Init_MANTA(FTM_MemMapPtr);
void ScopeSaver_Init(CMP_MemMapPtr, uint8_t);
void legacy_wand_scopesaver_init(CMP_MemMapPtr, uint8_t);
void IRQ_PWM_Fault(FTM_MemMapPtr, uint8_t, uint8_t);
void legacy_wand_IRQ_PWM_fault(FTM_MemMapPtr, uint8_t, uint8_t);
bool PWM_SoftStart(FTM_MemMapPtr,uint8_t);
void PWM_SoftStart_DMA_Init (uint16_t, FTM_MemMapPtr, uint16_t);
//void IECOverEnergyGPIO_Init(void);

extern const FTM_MemMapPtr pSmartWandFTM;
extern const FTM_MemMapPtr pLegacyWandFTM;
extern volatile bool ss_complete_flag;

#endif
