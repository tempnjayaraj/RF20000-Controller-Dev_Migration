/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2824                 $:
$Date:: 2022-03-07 12:57:31#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     rf.c
\brief     RF driver for controlled ablation i.e. "coblation".
\details   N/A
*******************************************************************************/
#include "MK26F18.h"
#include "system.h"
#include "rf.h"
#include "cpu.h"
#include "buck.h"
#include "legacy_wand.h"
#include "wand.h"

extern legacy_wand_params legacy_wand_params_obj;

const uint32_t rf_modulo = RF_MODULO;
const uint32_t rf_cv = (RF_MODULO / 2);
const FTM_MemMapPtr pLegacyWandFTM = FTM0_BASE_PTR;
const FTM_MemMapPtr pSmartWandFTM = FTM3_BASE_PTR;
const CMP_MemMapPtr piPeak1CMP = CMP1_BASE_PTR;
const CMP_MemMapPtr piPeak2CMP = CMP3_BASE_PTR;
volatile bool ss_complete_flag = FALSE;

const uint16_t ss_cv[8] = {179, 545, 182, 524, 164, 548, 161, 561};
extern uint16_t manta_pwm_mod;
extern uint16_t manta_pwm_C0V;
extern uint16_t manta_pwm_C1V;
extern uint16_t manta_pwm_C2V;
extern uint16_t manta_pwm_C3V;
extern uint16_t manta_pwm_C4V;
extern uint16_t manta_pwm_C5V;
extern uint16_t manta_pwm_C6V;
extern uint16_t manta_pwm_C7V;
extern bool manta_pwm_values_valid;

/****************************************************************************//**
Initialize PWM0 to drive bridge that generates alternating current.
@param void
@return void
@outputs
@context
********************************************************************************/
uint8_t smartwand_pwm_init(void)
{
  
  /* comparitor input used for fault ctrl */
  const uint8_t cmp3_in2 = 2;
  
  /* Enable flex timer module 3 gate clock */
  SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;
  
  /* Configure ports */
  PORTD_PCR1 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // FTM3_CH1
  PORTD_PCR2 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // FTM3_CH2
  PORTE_PCR10 = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; // FTM3_CH5
  PORTE_PCR11 = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; // FTM3_CH6
  
  if((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) && 
    (manta_pwm_values_valid == TRUE))
  {
    FTM_Init_MANTA(pSmartWandFTM);
  }
  else
  {
    FTM_Init(pSmartWandFTM);
  }
  
  
  ScopeSaver_Init(piPeak2CMP, cmp3_in2);
  
  /* Configure gpio controlled by over energy algorithm as fault input 3 */
  //IECOverEnergyGPIO_Init();
  
  /* enable fault 0 for iec over energy input*/
  FTM3_FLTCTRL |= FTM_FLTCTRL_FAULT0EN_MASK;
  
  /* enable PTD12 to control fault input 0*/
  PORTD_PCR12 = PORT_PCR_MUX(3); // FTM3_FLT0
  
  /* enable PTD8 as GPIO to control FLT3 & FLT0*/
  PORTD_PCR8 = PORT_PCR_MUX(1);
  
  /* set PTB6 data direction output */
  GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(8));
  
  /* set PTB6 to zero */
  GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(8));
  
  /* enable fault input 1 (CMP3_OUT) */
  pSmartWandFTM->FLTCTRL |= FTM_FLTCTRL_FAULT2EN_MASK;
    
  /* enable irq */
  NVIC_EnableIRQ(FTM3_IRQn);
  
  return 1;
  
}

/****************************************************************************//**
Initialize PWM to drive bridge that generates alternating current for legacy
wands.
@TODO Need more documentation
@param void
@return void
@outputs
@context
********************************************************************************/
void LegacyWand_PWMInit(void)
{
  
  /* comparitor input used for fault ctrl */
  const uint8_t cmp1_in5 = 5;
  
  /* Enable flex timer module 0 gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
  
  /* Configure ports */
  PORTC_PCR2 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // FTM0_CH1
  PORTC_PCR3 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // FTM0_CH2
  PORTD_PCR5 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // FTM0_CH4
  PORTD_PCR6 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // FTM0_CH7
  
  FTM_Init(pLegacyWandFTM);
  
  /* Configure system integration module so CMP1_OUT drives FTM0 fault 1 */
  SIM_SOPT4 |= SIM_SOPT4_FTM0FLT1_MASK;
  
  legacy_wand_scopesaver_init(piPeak1CMP, cmp1_in5);
  
  /* Disable write protection */
  //FTM0_MODE |= FTM_MODE_WPDIS_MASK;
  
  /* Enable fault input 1 (CMP1_OUT) */
  pLegacyWandFTM->FLTCTRL |= FTM_FLTCTRL_FAULT1EN_MASK;
  
  /* Enable irq */
  NVIC_EnableIRQ(FTM0_IRQn);
  
}

/****************************************************************************//**
Initializes the Flexible Timer module (FTM)
@param pFTM pointer to the FTM module
@return void
@outputs
@context
********************************************************************************/
void FTM_Init(FTM_MemMapPtr pFTM)
{
  
  /* Disable write protection */
  pFTM->MODE |= FTM_MODE_WPDIS_MASK;
      
  /* Fault control all channels w/ manual fault clearing. */
  pFTM->MODE |= FTM_MODE_FTMEN_MASK | FTM_MODE_FAULTM(2) | FTM_MODE_FAULTIE_MASK;
  
  pFTM->CONF |= FTM_CONF_BDMMODE(3);
  
  /* Set Frequency to 100kHz  
   * NOTE: MOD calculation: 740 = (74MHz / 100kHz) */
  pFTM->MOD = rf_modulo - 1;
  
  /* Set initial count value */
  pFTM->CNTIN = 0;
    
  /* Clear any pending faults */
  pFTM->FMS &= ~FTM_FMS_FAULTF_MASK;
  
  /* Combine channel 0 & 1; enable fault & sync ctrl on ch 0 to 4 */
  pFTM->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK |
                          FTM_COMBINE_FAULTEN0_MASK | FTM_COMBINE_FAULTEN1_MASK |
                          FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_SYNCEN1_MASK |
                          FTM_COMBINE_COMBINE1_MASK | 
                          FTM_COMBINE_COMBINE2_MASK |// FTM_COMBINE_COMP2_MASK |
                          FTM_COMBINE_COMBINE3_MASK | 
                          FTM_COMBINE_FAULTEN2_MASK | FTM_COMBINE_FAULTEN3_MASK |
                          FTM_COMBINE_SYNCEN2_MASK | FTM_COMBINE_SYNCEN3_MASK;
                          //FTM_COMBINE_DTEN2_MASK | FTM_COMBINE_DTEN3_MASK;
  
  /* Enable software control to force outputs low */
  pFTM->SWOCTRL |= FTM_SWOCTRL_CH0OC_MASK |
                  FTM_SWOCTRL_CH1OC_MASK |
                  FTM_SWOCTRL_CH2OC_MASK |
                  FTM_SWOCTRL_CH3OC_MASK |
                  FTM_SWOCTRL_CH4OC_MASK |
                  FTM_SWOCTRL_CH5OC_MASK |
                  FTM_SWOCTRL_CH6OC_MASK |
                  FTM_SWOCTRL_CH7OC_MASK;
  
  /* Generate trigger when FTM counter is equal to the CNTIN register. */
  pFTM->EXTTRIG |= FTM_EXTTRIG_INITTRIGEN_MASK | FTM_EXTTRIG_CH1TRIG_MASK;
  
  /* Set initial state for channel output */
  pFTM->OUTINIT = 0;
  
  /* Set deadtime to zero */
  pFTM->DEADTIME = 0; 
  
  /* Configure channel 0 & 1 as edge-aligned with high pulses */
  pFTM->CONTROLS[0].CnSC |= FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK;
  pFTM->CONTROLS[1].CnSC |= FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK;
  
  /* Configure channel 2 as edge-aligned with low pulses */
  pFTM->CONTROLS[2].CnSC |= FTM_CnSC_ELSB_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_DMA_MASK | FTM_CnSC_CHIE_MASK;
  pFTM->CONTROLS[3].CnSC |= FTM_CnSC_MSB_MASK;
  
  /* Configure channel 4 & 5 as edge-aligned with high pulses */
  pFTM->CONTROLS[4].CnSC |= FTM_CnSC_MSB_MASK;
  pFTM->CONTROLS[5].CnSC |= FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK;
    
  /* Configure channel 7 as edge-aligned with low pulses */
  pFTM->CONTROLS[6].CnSC |= FTM_CnSC_ELSB_MASK | FTM_CnSC_MSB_MASK;
  pFTM->CONTROLS[7].CnSC |= FTM_CnSC_MSB_MASK;
  
  /* Set duty cycle values */
  pFTM->CONTROLS[0].CnV = ss_cv[0];
  pFTM->CONTROLS[1].CnV = ss_cv[1];
  pFTM->CONTROLS[2].CnV = ss_cv[2];
  pFTM->CONTROLS[3].CnV = ss_cv[3];
  pFTM->CONTROLS[4].CnV = ss_cv[4];
  pFTM->CONTROLS[5].CnV = ss_cv[5];
  pFTM->CONTROLS[6].CnV = ss_cv[6];
  pFTM->CONTROLS[7].CnV = ss_cv[7];
  
  pFTM->INVCTRL = FTM_INVCTRL_INV0EN_MASK | FTM_INVCTRL_INV3EN_MASK;
  
  /* Initialize channel output */
  pFTM->MODE |= FTM_MODE_INIT_MASK;
   
  /* Enable enhanced PWM synchronization. Updates to MOD, CNTIN, CV, Counter, 
   * and SWOCTRL registers initiated by software trigger */
  pFTM->SYNCONF |= FTM_SYNCONF_SYNCMODE_MASK | //FTM_SYNCONF_SWRSTCNT_MASK |
                  FTM_SYNCONF_SWWRBUF_MASK | FTM_SYNCONF_SWSOC_MASK |
                  FTM_SYNCONF_SWOC_MASK | FTM_SYNCONF_INVC_MASK | FTM_SYNCONF_SWINVC_MASK;
  
  /* Counter updated with initial value when trigger detected.  Sync occurs when 
   * counter reaches its maximum value. */
  pFTM->SYNC |= FTM_SYNC_REINIT_MASK | FTM_SYNC_CNTMAX_MASK;
  
  /* SWITCH RF ON PERMANENTLY -- DEBUG ONLY!!! */
  //pFTM->SWOCTRL &= ~(FTM_SWOCTRL_CH1OC_MASK  | FTM_SWOCTRL_CH2OC_MASK);
  
  /* Select FTM system clock (a.k.a. bus clock) as source */
  pFTM->SC |= FTM_SC_CLKS(1);
  
  /* Set software trigger to sync PWM */
  pFTM->SYNC |= FTM_SYNC_SWSYNC_MASK;
 
}  

/****************************************************************************//**
Initializes the Flexible Timer module (FTM)
@param pFTM pointer to the FTM module
@return void
@outputs
@context
********************************************************************************/
void FTM_Init_MANTA(FTM_MemMapPtr pFTM)
{
  
  /* Disable write protection */
  pFTM->MODE |= FTM_MODE_WPDIS_MASK;
      
  /* Fault control all channels w/ manual fault clearing. */
  pFTM->MODE |= FTM_MODE_FTMEN_MASK | FTM_MODE_FAULTM(2) | FTM_MODE_FAULTIE_MASK;
  
  pFTM->CONF |= FTM_CONF_BDMMODE(3);
  
  /* Set Frequency to x kHz  
   * NOTE: MOD calculation: 740 = (74MHz / xkHz) */
  pFTM->MOD = manta_pwm_mod;
  
  /* Set initial count value */
  pFTM->CNTIN = 0;
    
  /* Clear any pending faults */
  pFTM->FMS &= ~FTM_FMS_FAULTF_MASK;
  
  /* Combine channel 0 & 1; enable fault & sync ctrl on ch 0 to 4 */
  pFTM->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK |
                          FTM_COMBINE_FAULTEN0_MASK | FTM_COMBINE_FAULTEN1_MASK |
                          FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_SYNCEN1_MASK |
                          FTM_COMBINE_COMBINE1_MASK | 
                          FTM_COMBINE_COMBINE2_MASK |// FTM_COMBINE_COMP2_MASK |
                          FTM_COMBINE_COMBINE3_MASK | 
                          FTM_COMBINE_FAULTEN2_MASK | FTM_COMBINE_FAULTEN3_MASK |
                          FTM_COMBINE_SYNCEN2_MASK | FTM_COMBINE_SYNCEN3_MASK;
                          //FTM_COMBINE_DTEN2_MASK | FTM_COMBINE_DTEN3_MASK;
  
  /* Enable software control to force outputs low */
  pFTM->SWOCTRL |= FTM_SWOCTRL_CH0OC_MASK |
                  FTM_SWOCTRL_CH1OC_MASK |
                  FTM_SWOCTRL_CH2OC_MASK |
                  FTM_SWOCTRL_CH3OC_MASK |
                  FTM_SWOCTRL_CH4OC_MASK |
                  FTM_SWOCTRL_CH5OC_MASK |
                  FTM_SWOCTRL_CH6OC_MASK |
                  FTM_SWOCTRL_CH7OC_MASK;
  
  /* Generate trigger when FTM counter is equal to the CNTIN register. */
  pFTM->EXTTRIG |= FTM_EXTTRIG_INITTRIGEN_MASK | FTM_EXTTRIG_CH1TRIG_MASK;
  
  /* Set initial state for channel output */
  pFTM->OUTINIT = 0;
  
  /* Set deadtime to zero */
  pFTM->DEADTIME = 0; 
  
  /* Configure channel 0 & 1 as edge-aligned with high pulses */
  pFTM->CONTROLS[0].CnSC |= FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK;
  pFTM->CONTROLS[1].CnSC |= FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK;
  
  /* Configure channel 2 as edge-aligned with low pulses */
  pFTM->CONTROLS[2].CnSC |= FTM_CnSC_ELSB_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_DMA_MASK | FTM_CnSC_CHIE_MASK;
  pFTM->CONTROLS[3].CnSC |= FTM_CnSC_MSB_MASK;
  
  /* Configure channel 4 & 5 as edge-aligned with high pulses */
  pFTM->CONTROLS[4].CnSC |= FTM_CnSC_MSB_MASK;
  pFTM->CONTROLS[5].CnSC |= FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK;
    
  /* Configure channel 7 as edge-aligned with low pulses */
  pFTM->CONTROLS[6].CnSC |= FTM_CnSC_ELSB_MASK | FTM_CnSC_MSB_MASK;
  pFTM->CONTROLS[7].CnSC |= FTM_CnSC_MSB_MASK;
  
  /* Set duty cycle values */
  pFTM->CONTROLS[0].CnV = manta_pwm_C0V;
  pFTM->CONTROLS[1].CnV = manta_pwm_C1V;
  pFTM->CONTROLS[2].CnV = manta_pwm_C2V;
  pFTM->CONTROLS[3].CnV = manta_pwm_C3V;
  pFTM->CONTROLS[4].CnV = manta_pwm_C4V;
  pFTM->CONTROLS[5].CnV = manta_pwm_C5V;
  pFTM->CONTROLS[6].CnV = manta_pwm_C6V;
  pFTM->CONTROLS[7].CnV = manta_pwm_C7V;
  
  pFTM->INVCTRL = FTM_INVCTRL_INV0EN_MASK | FTM_INVCTRL_INV3EN_MASK; //Inverting bit.
  
  /* Initialize channel output */
  pFTM->MODE |= FTM_MODE_INIT_MASK;
   
  /* Enable enhanced PWM synchronization. Updates to MOD, CNTIN, CV, Counter, 
   * and SWOCTRL registers initiated by software trigger */
  pFTM->SYNCONF |= FTM_SYNCONF_SYNCMODE_MASK | //FTM_SYNCONF_SWRSTCNT_MASK |
                  FTM_SYNCONF_SWWRBUF_MASK | FTM_SYNCONF_SWSOC_MASK |
                  FTM_SYNCONF_SWOC_MASK | FTM_SYNCONF_INVC_MASK | FTM_SYNCONF_SWINVC_MASK;
  
  /* Counter updated with initial value when trigger detected.  Sync occurs when 
   * counter reaches its maximum value. */
  pFTM->SYNC |= FTM_SYNC_REINIT_MASK | FTM_SYNC_CNTMAX_MASK;
  
  /* SWITCH RF ON PERMANENTLY -- DEBUG ONLY!!! */
  //pFTM->SWOCTRL &= ~(FTM_SWOCTRL_CH1OC_MASK  | FTM_SWOCTRL_CH2OC_MASK);
  
  /* Select FTM system clock (a.k.a. bus clock) as source */
  pFTM->SC |= FTM_SC_CLKS(1);
  
  /* Set software trigger to sync PWM */
  pFTM->SYNC |= FTM_SYNC_SWSYNC_MASK;
 
}  

/****************************************************************************//**
Initialize high-speed comparator to limit current during a short.
@note Disable filter so comparator immediatly sets output high when ipeak
is greater than 1.9 volts with 30 mV of hysteresis. Using this configuration,
when the input signal goes above X volts, it takes 5 cycles for fault control
to switch output OFF.
@param pCMP pointer to the comparator module
@param input selects the channel that needs to be compared
@return void
@outputs
@context
********************************************************************************/
void ScopeSaver_Init (CMP_MemMapPtr pCMP, uint8_t input)
{
  
  /* Enable comparator gate clock */
  SIM_SCGC4 |= SIM_SCGC4_CMP_MASK;
    
  /* Disable filter, 30mV max hysteresis */
  pCMP->CR0 |= CMP_CR0_FILTER_CNT(7) | CMP_CR0_HYSTCTR(3);
  
  /* Invert output, enable in high-speed */
  pCMP->CR1 = CMP_CR1_INV_MASK | CMP_CR1_EN_MASK | CMP_CR1_PMODE_MASK;
  
  /* Disable filter */
  pCMP->FPR = CMP_FPR_FILT_PER(2);  
  
  /* Output set to (3.3/64) x 46 = 2.37V = 9.4875 Amps @ 4Amp/V */
  pCMP->DACCR = CMP_DACCR_DACEN_MASK | CMP_DACCR_VRSEL_MASK | CMP_DACCR_VOSEL(46);  
    
  /* P-input is 6b DAC */
  pCMP->MUXCR = CMP_MUXCR_PSEL(7) | CMP_MUXCR_MSEL(input);
    
  /* Enable comparator output pin */
  pCMP->CR1 |= CMP_CR1_OPE_MASK;
      
}
  
/****************************************************************************//**
Initialize high-speed comparator to limit current during a short.
@note Disable filter so comparator immediatly sets output high when ipeak
is greater than 1.9 volts with 30 mV of hysteresis. Using this configuration,
when the input signal goes above X volts, it takes 5 cycles for fault control
to switch output OFF. THIS IS FOR LEGACY WAND
@param pCMP pointer to the comparator module
@param input selects the channel that needs to be compared
@return void
@outputs
@context
********************************************************************************/
void legacy_wand_scopesaver_init (CMP_MemMapPtr pCMP, uint8_t input)
{

  /* Enable comparator gate clock */
  SIM_SCGC4 |= SIM_SCGC4_CMP_MASK;
    
  /* Disable filter, 30mV max hysteresis */
  pCMP->CR0 |= CMP_CR0_FILTER_CNT(7) | CMP_CR0_HYSTCTR(3);
  
  /* Invert output, enable in high-speed */
  pCMP->CR1 = CMP_CR1_INV_MASK | CMP_CR1_EN_MASK | CMP_CR1_PMODE_MASK;
  
  /* Disable filter */
  pCMP->FPR = CMP_FPR_FILT_PER(2);  
  
  /* Output set to (3.3/64) x 20 = 2.37V = 4.125 Amps @ 4Amp/V */
  pCMP->DACCR = CMP_DACCR_DACEN_MASK | CMP_DACCR_VRSEL_MASK | CMP_DACCR_VOSEL(44);  
    
  /* P-input is 6b DAC */
  pCMP->MUXCR = CMP_MUXCR_PSEL(7) | CMP_MUXCR_MSEL(input);
    
  /* Enable comparator output pin */
  pCMP->CR1 |= CMP_CR1_OPE_MASK;
      
}


/****************************************************************************//**
Handle interrupt triggered by FTM fault controller.
@param pFTM pointer to the FTM module
@param iPeakFault_Mask - Mask to detect Peak fault
@param iRMSFault_Mask - Mask to detect RMS fault
@return void
@outputs
@context
********************************************************************************/
void IRQ_PWM_Fault (FTM_MemMapPtr pFTM, uint8_t iPeakFault_Mask, uint8_t iRMSFault_Mask) {
 
  //if ((FTM_FMS_REG(pFTM) & iRMSFault_Mask) == iRMSFault_Mask)
  //{
    
    /* switch off buck */
    //GPIOE_PSOR |= GPIO_PDOR_PDO(BUCK_WW_ON | BUCK_18PIN_ON);  
  
    /* Force PWM outputs low */
    RF_OFF(pFTM);
    
    /* set both outputs to zero */
    //SET_DAC(pSmartWandDAC,0);
    //SET_DAC(pLegacyWandDAC,0);
        
    /* Update therapy settings */
    //current_therapy_settings.treatment.ablate_on = 0;
    //current_therapy_settings.treatment.coag_on = 0;
//    if (current_therapy_settings.treatment.therapy_mode == VAC)
//    {
//      
//      current_therapy_settings.pump.pulse_flag = 1;
//      
//    }
    
    if ((FTM3_FMS & FTM_FMS_FAULTF2_MASK) == FTM_FMS_FAULTF2_MASK)
    {
      
      //put_event(&evt_tbl, MEASURE_IMPEDANCE);
      //put_event(&evt_tbl, RF_ON);
      /* Turn yellow LED OFF */
      //GPIOA_PSOR |= YELLOW_LED;
      GPIOE_PCOR = EXT_CTRL_3;
      put_event(&evt_tbl,WAND_SHORT);
      put_event(&evt_tbl,OVER_CURRENT);
      //current_therapy_settings.pump.pulse_flag = 1;
            
    }
    else if (current_therapy_settings.wand.tube_warn == 0)
    {
      
      put_event(&evt_tbl, RF_ON);
      //GPIOE_PCOR = EXT_CTRL_2;
      put_event(&evt_tbl,OVER_ENERGY);
    
    }
        
//    if (current_therapy_settings.treatment.therapy_mode == VAC)
//    {
//      
//      current_therapy_settings.pump.pulse_flag = 1;
//      
//    }

    /* Disable fault interrupt */
    pFTM->MODE &= ~FTM_MODE_FAULTIE_MASK;
    
  //}
  
//  if ((FTM_FMS_REG(pFTM) & iPeakFault_Mask) == iPeakFault_Mask)
//  {
//
//    pFTM->FMS &= ~FTM_FMS_FAULTF_MASK;
//    
//  }
  
}

/****************************************************************************//**
Handle interrupt triggered by FTM fault controller.
@param pFTM pointer to the FTM module
@param iPeakFault_Mask - Mask to detect Peak fault
@param iRMSFault_Mask - Mask to detect RMS fault
@return void
@outputs
@context
********************************************************************************/
void legacy_wand_IRQ_PWM_fault (FTM_MemMapPtr pFTM, uint8_t iPeakFault_Mask, uint8_t iRMSFault_Mask)
{
  /* Force PWM outputs low if it is a peak fault*/
  RF_OFF(pFTM);

  if ((pFTM->FMS & iPeakFault_Mask) == iPeakFault_Mask)
  {
    /* Turn yellow LED OFF */
    GPIOE_PCOR = EXT_CTRL_3;
    put_event(&evt_tbl,WAND_SHORT);
    legacy_wand_params_obj.peak_detect_flag = 1;
  }
  /* Disable fault interrupt */
  pFTM->MODE &= ~FTM_MODE_FAULTIE_MASK;

}


