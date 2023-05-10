/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2844                 $:
$Date:: 2023-01-17 16:00:04#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      adc.c
\brief     ADC device driver.
\details   This is the ADC device driver which enables the app to talk to the
ADC.
*******************************************************************************/
#include <string.h>
#include "MK26F18.h"
#include "adc.h"
#include "system.h"
#include "buck.h"
#include "cpu.h"
#include "rf.h"
#include "legacy_wand.h"
#include "can_debug.h"
#include "wand.h"
#include "audio.h"

extern legacy_wand_params legacy_wand_params_obj;
extern uint16_t over_energy_current_limit;
static uint16_t adc_result0;
void legacy_ent_timer_wands(void);
bool ent_blendizer(uint16_t prime_time, uint16_t total_time, bool reset);
extern manta_prime prime;

#ifdef DEBUG_K26
uint32_t ipeak_threshold_print;
uint16_t irms2_print;
uint16_t dac_print;
#endif

/***************************************************************************//**
polls the adc.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t poll_adc_data(void)
{

  //uint32_t adc_cmd_word_fsw_present = 0x00000040;
  uint32_t adc_cmd_word_fsw_present = ADC_SC1_AIEN(1) |
  									  ADC_SC1_ADCH(22);

  ADC0_SC1A = adc_cmd_word_fsw_present;

  return 0;


}

/***************************************************************************//**
check for internal temp
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t adc3_coco(void)
{

  //uint32_t adc_cmd_word_temperature = 0x0000005A;
  uint32_t adc_cmd_word_temperature = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(26);
  //uint32_t adc_cmd_word_bandgap = 0x0000005B;
  uint32_t adc_cmd_word_bandgap = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(27);
  //uint32_t adc_cmd_word_stator = 0x00000053;
  uint32_t adc_cmd_word_stator = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(16);

  uint16_t adc_result0 = 0;
  float32_t temp = 0.0f;//, vdd = 0.0f;
  //static uint16_t filt_bg = 19860;
  static uint8_t internal_temp_threshold_count = 0,
                 internal_fault_temp_threshold_count = 0,
                 internal_temp_hysteresis = 0,
                 footswitch_debounce = 0;

   adc_result0 = (unsigned short) ADC0_RA;

   /* internal temperature sensor */
   if (ADC0_SC1A == adc_cmd_word_temperature)
   {

     ADC0_SC1A = adc_cmd_word_bandgap;

     temp = get_temp(adc_result0);
    #ifdef DEBUG_CAN

    CANdebug.adc.temperature = adc_result0;

    #endif

     if (temp >= INTERNAL_TEMP_THRESHOLD)
     {

       internal_temp_threshold_count++;

       if (internal_temp_hysteresis > 0)
       {

         internal_temp_hysteresis = 10;

       }


     }
     else if (internal_temp_hysteresis > 0)
     {

       internal_temp_hysteresis--;

       if (internal_temp_hysteresis == 0)
       {

         put_event(&evt_tbl,CLEAR_INTERNAL_TEMP_ERROR);

       }

     }
     else
     {

       internal_temp_threshold_count = 0;

     }

     if (internal_temp_threshold_count > 30)
     {

       put_event(&evt_tbl,INTERNAL_TEMP_ERROR);
       internal_temp_threshold_count = 0;
       internal_temp_hysteresis = 10;

     }


     if (temp >= INTERNAL_FAULT_TEMP_THRESHOLD)
     {

       internal_fault_temp_threshold_count++;

     }
     else
     {

       internal_fault_temp_threshold_count = 0;

     }

     if (internal_fault_temp_threshold_count > 30)
     {

       put_event(&evt_tbl,INTERNAL_TEMP_FAULT);
       internal_fault_temp_threshold_count = 0;

     }

   }
   /* pmc bandgap voltage */
   else if (ADC0_SC1A == adc_cmd_word_bandgap)
   {

     //filt_bg = (adc_result0 >> 1) + (filt_bg >> 1);
     //vdd = (float32_t) USHRT_MAX / adc_result0;

     /* spec is 1 +/- 0.3 volt */
     if ((adc_result0 < MIN_BANDGAP_SPEC) || (adc_result0 > MAX_BANDGAP_SPEC))
     {

       put_event(&evt_tbl,ADC_BOUNDARY_FAULT);

     }

     ADC0_SC1A = adc_cmd_word_stator;


   }
   /* stator sensor */
   else if (ADC0_SC1A == adc_cmd_word_stator)
   {
    #ifdef DEBUG_CAN
    CANdebug.adc.stator = adc_result0;
    #endif
      //current_therapy_settings.pump.stator =
        //(adc_result0 >> 1) + (current_therapy_settings.pump.stator >> 1);
     
     current_therapy_settings.pump.stator = adc_result0;

   }
   /* foot switch */
   else
   {

     uint8_t power_off = current_therapy_settings.pump.stator < STATOR_POWER_DOWN ? 1 : 0;
     
     ADC0_SC1A = adc_cmd_word_temperature;
      /* determine footswitch type
       *
       * footswitch connected < 300 ohms
       *  v = [24610*(3.3/65535)] = 1.614755474...
       *  r = (499*4.3)/(4.3-v) = 300.070617 ohms
       *
       * rf20000 footswitch if > 200 ohms
       *  v = [18751*(3.3/65535)] = 1.230324254...
       *  r = (499*4.3)/(4.3-v) = 199.9989099 ohms
       */
        if ((adc_result0 <= 32063) && (power_off == 0))
        {

          /* only update vac enabled when footswitch if first connected */
          if (current_therapy_settings.footpedal.wired.present == 0)
          {

            /* less than 200 ohms is legacy */
            if (adc_result0 < 24433)
            {

              current_therapy_settings.treatment.vac_enabled = 0;

            }
            /* between 200 and 300 ohms is rf20000 foot switch */
            else
            {

              current_therapy_settings.treatment.vac_enabled = 1;

            }
                      
          }
          
          if (footswitch_debounce == 1)
          {
            
            current_therapy_settings.footpedal.wired.present = 1;
            
          }
          
          footswitch_debounce = 1;

        }

        else
        {
          footswitch_debounce = 0;
          current_therapy_settings.footpedal.wired.present = 0; //Force to 1. Fix footpedal in K26 board
          
        }

    }

  return 0;

}

/***************************************************************************//**
Converts adc_counts(int) to temperature(float)
@param adc_temp (dac counts)
@return temp (actual temp value)
@outputs
@context
*******************************************************************************/
float32_t get_temp(uint16_t adc_temp)
{//.7012)/.001646

  float32_t vtemp25 = 0.719f, /* 719mV @ 25C from k60 datasheet */
            vtemp_slope = 0.001715f, /* 1.715mV/C from k60 datasheet */
            vtemp = 0.0f,
            temp = 0.0f;

  static uint16_t filt_adc_temp = 13890;

  /* filter */
  filt_adc_temp = (adc_temp >> 1) + (filt_adc_temp >> 1);

  /* convert adc counts to voltage (3.3/2^16) */
  vtemp = filt_adc_temp * 0.000050354f;

  /* convert voltage to temperature */
  temp = 25.0f - ((vtemp - vtemp25)/vtemp_slope);

  return temp;

}

/***************************************************************************//**
Initializes the ADC registers
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t adc_init(void)
{

//  /* Define and initialize adc command words in p-flash */
//  //const uint32_t adc_cmd_word_ipeak1 = 0x00000044;
//  const uint32_t adc_cmd_word_irms1 = 0x000041;
//  //const uint32_t adc_cmd_word_ipeak2 = 0x0000042;
//  const uint32_t adc_cmd_word_irms2 = 0x0000053;
//  //const uint32_t adc_cmd_word_hall = 0x00000054;
//  const uint32_t adc_cmd_word_mod2 = 0x0000004E;
//  //const uint32_t adc_cmd_word_mod1 = 0x0000004F;
//
//  ADC_MemMapPtr pADC0 = ADC0_BASE_PTR;
//  ADC_MemMapPtr pADC1 = ADC1_BASE_PTR;
//  ADC_MemMapPtr pADC2 = ADC2_BASE_PTR;
//  ADC_MemMapPtr pADC3 = ADC3_BASE_PTR;
//
//  /* Enable gate clock */
//  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK |
//               SIM_SCGC6_ADC2_MASK;
//
//  SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK |
//               SIM_SCGC3_ADC3_MASK;
//
//  /* Loop ADC0, ADC1, ADC2 & ADC3 */
//  //SIM_MCR |= SIM_MCR_PDBLOOP_MASK;
//
//  /* Set very high speed operation bit (ADHSC = 1) */
//  //ADC0_CFG2 |= ADC_CFG2_ADHSC_MASK ;
//  //ADC1_CFG2 |= ADC_CFG2_ADHSC_MASK ;
//  //ADC2_CFG2 |= ADC_CFG2_ADHSC_MASK ;
//
//  /* Slow clock to minimum for best calibration result */
//  pADC0->CFG1 |= ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(2); // 9.25MHz
//  pADC1->CFG1 |= ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(2);
//  pADC2->CFG1 |= ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(2);
//  pADC3->CFG1 |= ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(2);
//
//  /* Calibrate */
//  ADC_Cal(pADC0);
//  ADC_Cal(pADC1);
//  ADC_Cal(pADC2);
//  ADC_Cal(pADC3);
//
//  /* Configure single ended 16-bit */
//  pADC0->CFG1 |= ADC_CFG1_MODE(3);
//  pADC1->CFG1 |= ADC_CFG1_MODE(3);
//  pADC2->CFG1 |= ADC_CFG1_MODE(3);
//  pADC3->CFG1 |= ADC_CFG1_MODE(3);
//
//  /* Set software trigger selected bit (ADTRG = 0) */
////  ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK ;
//
//  /* Enable DMA interface */
//  //ADC0_SC2 |= ADC_SC2_DMAEN_MASK;
//
//  /* Select channels */
//  //ADC0_SC1A = ADC_SC1_ADCH(0x00); /* ADC0_RA */
//  //ADC0_SC1B = ADC_SC1_ADCH(0x00); /* ADC0_RB */
//
//  pADC0->SC2 |= ADC_SC2_ADTRG_MASK;
//  pADC1->SC2 |= ADC_SC2_ADTRG_MASK;
//  pADC2->SC2 |= ADC_SC2_ADTRG_MASK;
//  pADC3->SC2 &= ~ADC_SC2_ADTRG_MASK;
//  pADC3->SC3 |= ADC_SC3_AVGE_MASK;
//  /* AIEN = 1 - conversion complete interrupt enabled, ADCH = 0x9 - Channel 9 is selected as
//  SE input */
//  pADC0->SC1[0] = (uint32_t) adc_cmd_word_irms1;
//  /* delete next two lines if the two-time measurement per PWM period is not required */
//  pADC0->SC1[1] = (uint32_t) 0;
//  /* AIEN = 1 - conversion complete interrupt enabled, ADCH = 0x9 - Channel 9 is selected as
//  SE input */
//  pADC1->SC1[0] = (uint32_t) adc_cmd_word_mod2;
//  /* delete next two lines if the two-time measurement per PWM period is not required */
//  pADC1->SC1[1] = (uint32_t) adc_cmd_word_irms2;
//  /* AIEN = 1 - conversion complete interrupt enabled, ADCH = 0x9 - Channel 9 is selected as
//  SE input */
//  //pADC2->SC1[0] = (uint32_t) adc_cmd_word_irms1;
//  /* delete next two lines if the two-time measurement per PWM period is not required */
//  //pADC2->SC1[1] = (uint32_t) adc_cmd_word_irms2;
//  /* AIEN = 1 - conversion complete interrupt enabled, ADCH = 0x9 - Channel 9 is selected as
//  SE input */
//  //pADC3->SC1[0] = (uint32_t) adc_cmd_word_fsw_present;
//  /* delete next two lines if the two-time measurement per PWM period is not required */
//  //pADC3->SC1[1] = (uint32_t) adc_cmd_word_temperature;
//
//
//  /* enable gate clock */
//  SIM_SCGC6 |= SIM_SCGC6_PDB_MASK;
//
//  /* EN = 0x01 | EN = 0x00 - first two pre/triggers are enabled
//  TOS = 0x01 | TOS = 0x00 - channels assert when the counter reaches the channel
//  delay register plus one peripheral clock cycle after PDB trigger */
//  PDB0_MOD = 7400;
//  PDB0_CH0C1 |= PDB_C1_EN(1) | PDB_C1_TOS(1);
//  PDB0_CH1C1 |= PDB_C1_EN(3) | PDB_C1_TOS(3);
//
//  /* set the delay value for the channel's corresponding pre-triggers */
//  PDB0_CH0DLY0 = 3700;
//  //PDB0_CH0DLY1 = 2960;
//  PDB0_CH1DLY0 = 1850;
//  //PDB0_CH1DLY1 = 6936;
//  PDB0_CH1DLY1 = 5550;
//
//  /* PDBEN = 1 - PDB enabled, TRGSEL = 0x9 - FTM1 is a trigger source for PDB,
//  LDOK - writing 1 to this bit updates the internal registers*/
//  PDB0_SC |= PDB_SC_PDBEN_MASK | PDB_SC_TRGSEL(0x7) | PDB_SC_PDBEIE_MASK |
//             PDB_SC_PDBIE_MASK | PDB_SC_CONT_MASK | PDB_SC_PRESCALER(0);
//
//  PDB0_SC |= PDB_SC_LDOK_MASK;
//
//  /* enable adc0 irq */
//  NVIC_EnableIRQ(57); // adc0
//  //NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)[a]
//  NVIC_EnableIRQ(58); // adc1
//  //NVIC_EnableIRQ(102); // adc2
//  NVIC_EnableIRQ(103); // adc3
//
//  /* enable pdb irq */
//  NVIC_EnableIRQ(72);
  
  return 1;

}

/***************************************************************************//**
Monitor and detect over energy events to comply with IEC 60601-2-2. Over energy 
event are defined as when the average root-mean-square of alternating current
measured through the return electrode multiplied by the set point voltage
exceeds 400 watts for a predetermined amount of time, but never to exceed 1
second. The result of over energy events is a "pulsed" RF output.  

@param void
@return 0
@outputs Force RF OFF if energy threshold is breached.
@context Routine is called from the main state machine in STATUP_STATE,
USER_INPUT_STATE, IDLE_STATE, & THERAPY_STATE.  It runs once every 1 millisecond
based off 50 microsecond adc1 irq.  
*******************************************************************************/
uint8_t irms_overcurrent(void)
{

  /* One second pulse accumulator */
  static PULSE_ACCUMULATOR one_sec_accum;

  /* Sub-second (less than one) pulse accumulator 1 */
  static PULSE_ACCUMULATOR sub_sec_accum_1;

  /* Sub-second (less than one) pulse accumulator 2 */
  static PULSE_ACCUMULATOR sub_sec_accum_2;

  static uint16_t hysteresis = 0;

  uint32_t power = 0;

  /* when rf is off force zero in accumulators */
  if (SWOC_FTM(pSmartWandFTM) == FTM_SWOCTRL_CH0OC_MASK)
  {

    power = 0;
    
  }
  else
  {
    
    power = current_therapy_settings.pulsing.mean_irms;
    
  }
  
  /* reset accumulators when coblation inactive */
  if ((current_therapy_settings.treatment.ablate_on == 0) &&
      (current_therapy_settings.treatment.coag_on == 0))
  {

    memset(&one_sec_accum,0,sizeof(PULSE_ACCUMULATOR));
    memset(&sub_sec_accum_1,0,sizeof(PULSE_ACCUMULATOR));
    memset(&sub_sec_accum_2,0,sizeof(PULSE_ACCUMULATOR));
    
  }

  /* check if hysteresis expired and renable RF */
  if ((hysteresis == 1) && (current_therapy_settings.setpoint.dac == 0))
  {
    /* Turn hysteresis OFF */
    hysteresis = 0;

    /* Flush both sub-second accumulators */
    memset(&sub_sec_accum_1,0,sizeof(PULSE_ACCUMULATOR));
    memset(&sub_sec_accum_2,0,sizeof(PULSE_ACCUMULATOR));

    /* reset delay so windows remain out of phase 50% of pulse window */
    current_therapy_settings.pulsing.counter_delay = 
      (current_therapy_settings.pulsing.pulse_window >> 1) + 1;

    /* re-enable RF by clearing fault input */
    GPIOD_PCOR |= RF_SW_DISABLE;

  }
  /* hysteresis still active then just deduct */
  else if (hysteresis > 1)
  {

    hysteresis--;

  }

  /* decrement count till 2nd window will start */
  if (current_therapy_settings.pulsing.counter_delay > 0)
  {

    current_therapy_settings.pulsing.counter_delay--;

  }

  one_sec_accum.sum += power;
  sub_sec_accum_1.sum += power;

  if (current_therapy_settings.pulsing.counter_delay == 0)
  {

    sub_sec_accum_2.sum += power;

  }

  one_sec_accum.fifo[one_sec_accum.sample] = power;
  sub_sec_accum_1.fifo[sub_sec_accum_1.sample] = power;

  if (current_therapy_settings.pulsing.counter_delay == 0)
  {

    sub_sec_accum_2.fifo[sub_sec_accum_2.sample] = power;

  }

  /* when accumulator exceeds threshold halt RF */
  if (one_sec_accum.sum >= current_therapy_settings.pulsing.iec_threshold)
  {

    GPIOD_PSOR |= RF_SW_DISABLE;

  }

  /* when accumulator exceeds threshold halt RF */
  if (sub_sec_accum_1.sum  >= current_therapy_settings.pulsing.med_threshold)
  {

    GPIOD_PSOR |= RF_SW_DISABLE;
    
    /* reset delay so windows remain out of phase 50% of pulse window */
    //current_therapy_settings.pulsing.counter_delay =  
    //  (current_therapy_settings.pulsing.pulse_window >> 1) + 1;
    
    /* check overflow */
    if (current_therapy_settings.pulsing.pulse_window < sub_sec_accum_1.sample)
    {

     hysteresis = 1;

    }

    else
    {

     hysteresis = current_therapy_settings.pulsing.pulse_window - sub_sec_accum_1.sample;
     
    }

  }
  
  if (sub_sec_accum_2.sum >= current_therapy_settings.pulsing.med_threshold)
  {
    
    GPIOD_PSOR |= RF_SW_DISABLE; 
    
    /* reset delay so windows remain out of phase 50% of pulse window */
    //current_therapy_settings.pulsing.counter_delay = 
    //  (current_therapy_settings.pulsing.pulse_window >> 1) + 1;
    
    /* check overflow */
    if (current_therapy_settings.pulsing.pulse_window < sub_sec_accum_2.sample)
    {

     hysteresis = 1;

    }

    else
    {

     hysteresis = current_therapy_settings.pulsing.pulse_window - sub_sec_accum_2.sample;
     
    }
    
  }
    
  /* re-enable RF if one accumulator is less than 1/2 full */
  if ((one_sec_accum.sum < current_therapy_settings.pulsing.half_iec_threshold) &&
      (hysteresis == 0) &&
      (current_therapy_settings.setpoint.dac == 0))
  {

   GPIOD_PCOR |= RF_SW_DISABLE;

  }

  if (sub_sec_accum_1.sample < 999)
  {

    sub_sec_accum_1.sample++;

  }
  else
  {

    sub_sec_accum_1.sample = 0;

  }

  if (sub_sec_accum_1.sample >= current_therapy_settings.pulsing.pulse_window)
  {

    sub_sec_accum_1.sample = 0;

    memset(&sub_sec_accum_1,0,sizeof(PULSE_ACCUMULATOR));

  }

  //sub_sec_accum_1.sum -= sub_sec_accum_1.fifo[sub_sec_accum_1.sample];

  if ((current_therapy_settings.pulsing.counter_delay == 0) && 
      (sub_sec_accum_2.sample < 999))
  {

    sub_sec_accum_2.sample++;

  }

  else
  {

    sub_sec_accum_2.sample = 0;

    memset(&sub_sec_accum_2,0,sizeof(PULSE_ACCUMULATOR));

  }

  if (sub_sec_accum_2.sample >= current_therapy_settings.pulsing.pulse_window)
  {

    sub_sec_accum_2.sample = 0;

    memset(&sub_sec_accum_2,0,sizeof(PULSE_ACCUMULATOR));

  }

  //sub_sec_accum_2.sum -= sub_sec_accum_2.fifo[sub_sec_accum_2.sample];

  /* 1 sec energy limit uses fifo circular accum so calculate next sample */
  one_sec_accum.sample = (one_sec_accum.sample + 1) % (sizeof(one_sec_accum.fifo) >> 2);
  
  /* remove oldest value in fifo from summation */
  one_sec_accum.sum -= one_sec_accum.fifo[one_sec_accum.sample];

  return 0;

}

/***************************************************************************//**
Calibrates the ADC16. Required to meet specifications after reset and before
conversion is initiated.
@param pADC ADC module pointer points to adc0 or adc1 register map base address.
@return 0 Zero indicates success.
@outputs
@context
*******************************************************************************/
uint8_t ADC_Cal(ADC_MemMapPtr pADC)
{

  uint16_t cal_var;

  pADC->SC2 &=  ~ADC_SC2_ADTRG_MASK ; // Enable Software Conversion Trigger for Calibration Process    - ADC0_SC2 = ADC0_SC2 | ADC_SC2_ADTRGW(0);
  pADC->SC3 &= ( ~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK ); // set single conversion, clear avgs bitfield for next writing
  pADC->SC3 |= ( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(AVGS_32) );  // Turn averaging ON and set at max value ( 32 )

  pADC->SC3 |= ADC_SC3_CAL_MASK ;      // Start CAL
  while ( (pADC->SC1[0] & ADC_SC1_COCO_MASK ) == COCO_NOT ); // Wait calibration end

  if ((pADC->SC3 & ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK )
  {
   return(1);    // Check for Calibration fail error and return
  }
  // Calculate plus-side calibration
  cal_var = 0x00;

  cal_var =  pADC->CLP0;
  cal_var += pADC->CLP1;
  cal_var += pADC->CLP2;
  cal_var += pADC->CLP3;
  cal_var += pADC->CLP4;
  cal_var += pADC->CLPS;

  cal_var = cal_var/2;
  cal_var |= 0x8000; // Set MSB

  pADC->PG = ADC_PG_PG(cal_var);


  // Calculate minus-side calibration
  cal_var = 0x00;

  cal_var =  pADC->CLM0;
  cal_var += pADC->CLM1;
  cal_var += pADC->CLM2;
  cal_var += pADC->CLM3;
  cal_var += pADC->CLM4;
  cal_var += pADC->CLMS;

  cal_var = cal_var/2;

  cal_var |= 0x8000; // Set MSB

  pADC->MG = ADC_MG_MG(cal_var);

  pADC->SC3 &= ~ADC_SC3_CAL_MASK ; /* Clear CAL bit */

  /* Restore clock settings */
  //pADC->CFG1 &= ~(ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(3));
  pADC->SC3 &= ~( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(AVGS_32) );  // Turn averaging ON and set at max value ( 32 )

  return(0);
}


/***************************************************************************//**
Finds irms value from previous samples using a filter
@todo Need more description
@param void
@return 0
@outputs
@context: event driven from adc1 result interrupt occuring every 10usec
*******************************************************************************/
uint8_t irms2_adc_result(void)
{

  uint16_t adc_result1;
  //static uint32_t stator_accum = 0;
  static uint8_t fast_idx = 0, slow_idx = 0, ramp_down_active = 0;
  static q15_t irms_buffer[20];
  static uint16_t z_time;
  uint32_t ipeak_threshold = 0;
  
  uint16_t dac = 0;
  static int8_t blendizer_1ms_cnt = 0;


    adc_result1 = (uint16_t) ADC1_RB;

    if (fast_idx >= 200)
    {

      /* save data */
      //memcpy(&current_therapy_settings.pump.irms[0],&irms_buffer[0],40);
      /* compute average */
      arm_mean_q15(&irms_buffer[0], 20, &current_therapy_settings.pump.mean_irms);
      put_event(&evt_tbl,RUN_PUMP_SPEED_CTRL);
      put_event(&evt_tbl,WAND_IRMS_WEAR_MONITOR);
      fast_idx = 0;
      slow_idx = 0;

    }


    if (((FTM3_SWOCTRL & FTM_SWOCTRL_CH0OC_MASK) == 0) && (ramp_down_active == 0))
    {

      if (current_therapy_settings.wand.z_test == FALSE)
      {
        if (current_therapy_settings.controller == ENT)
        {
          if (blendizer_1ms_cnt-- <= 1)
          {
            blendizer_1ms_cnt = 10;
            bool blend = ent_blendizer(current_therapy_settings.setpoint.ent_duty_on_time,
                                       current_therapy_settings.setpoint.ent_duty_total_time,
                                       FALSE);
            if (blend == TRUE)
            {
                  current_therapy_settings.setpoint.level = current_therapy_settings.setpoint.ent_primary_level;
                  current_therapy_settings.setpoint.ent_coag_power = current_therapy_settings.setpoint.ent_coag_power_primary;
            }
            else 
            {
                  current_therapy_settings.setpoint.level = current_therapy_settings.setpoint.ent_secondary_level;
                  current_therapy_settings.setpoint.ent_coag_power = current_therapy_settings.setpoint.ent_coag_power_secondary;
            }
          }        
          if (current_therapy_settings.treatment.coag_on == 1)
          {
            current_therapy_settings.setpoint.dac = 
              set_buck_voltage_constant_power(adc_result1,
                                              current_therapy_settings.setpoint.dac,
                                              current_therapy_settings.setpoint.ent_coag_power,
                                              current_therapy_settings.setpoint.level
                                             );
              set_dac(current_therapy_settings.setpoint.dac);
          }
          else
          {
            current_therapy_settings.setpoint.dac = set_buck_voltage(adc_result1, current_therapy_settings.setpoint.dac);
          }
        }
        else
        {
          current_therapy_settings.setpoint.dac = set_buck_voltage(adc_result1, current_therapy_settings.setpoint.dac);
        }

        z_time = 0;

      }
      else if (current_therapy_settings.setpoint.dac > smart_wand_params.impedance.z_level)
      {
      
        current_therapy_settings.setpoint.dac = 
          ramp_down_buck_voltage(pSmartWandDAC,
                                 current_therapy_settings.setpoint.dac);
        
      }
      else if (get_priming_status() == PRIME_AUTO) /* Removed saline detection feature and therefore commented section below*/
      {
//        set_dac(smart_wand_params.impedance.z_level);
//        current_therapy_settings.setpoint.dac = smart_wand_params.impedance.z_level;
//        if (adc_result1 >= smart_wand_params.manta.prime.autom.rms_threshold)
//        {
//          prime.ap_rf_detect = TRUE;
//          gui_stop_auto_prime();
//        }
      }
      else if (z_time < smart_wand_params.impedance.z_time)
      {

        z_time++;

        set_dac(smart_wand_params.impedance.z_level);
        current_therapy_settings.setpoint.dac = smart_wand_params.impedance.z_level;
//        if ((adc_result1 >= 0xFFFF) ||
//            (adc_result1 <= 0))
        if ((adc_result1 >= smart_wand_params.impedance.z_high_mod) ||
            (adc_result1 <= smart_wand_params.impedance.z_low))
        {
          z_time = 0;
        }

        if (z_time == smart_wand_params.impedance.z_time)
        {

          current_therapy_settings.wand.z_test = FALSE;
          z_time = 0;
          blendizer_1ms_cnt = 0;
          ent_blendizer(0,0,TRUE);

        }

      }

    }
    else 
    {
      /* have to ramp down to get plasma restarted */
      current_therapy_settings.setpoint.dac = 
        ramp_down_buck_voltage(pSmartWandDAC,
                               current_therapy_settings.setpoint.dac);
      
      /* once ramp dwn commences it must continue to zero */
      if (current_therapy_settings.setpoint.dac != 0)
      {  
        
        ramp_down_active = 1;
        
      }
      else
      {
        
        ramp_down_active = 0;
        
      }

      z_time = 0;
      blendizer_1ms_cnt = 0;
      ent_blendizer(0,0,TRUE);
    }

    if (fast_idx % 10 == 0)
    {

      put_event(&evt_tbl,RUN_IEC_OVERCURRENT);

      if (current_therapy_settings.setpoint.level != 0)
      {
          current_therapy_settings.pulsing.mean_irms =
            (adc_result1 *
             vmod.vmod2_lookup[current_therapy_settings.setpoint.dac]);
        }
      else
      {

        current_therapy_settings.pulsing.mean_irms = 0;

      }

      irms_buffer[slow_idx] = (q15_t) ((adc_result1 >> 1) & 0x7FFF);
      slow_idx++;

    }

    dac = vmod.vmod2_lookup[current_therapy_settings.setpoint.dac]; 
    /* case 1: output less than 188 volts */
    if (dac < current_therapy_settings.setpoint.ipeak_voltage) 
      /* DAC used here is compensated value and not the value from CFG file */
    {
      /* if not RF20k wand then trip load would be zero so set to 20 ohms */
      if (current_therapy_settings.setpoint.trip_load == 0)
      {
        /* 2.6 x 20 ohms = 52 */
        current_therapy_settings.setpoint.trip_load = 52;
      }

      ipeak_threshold = dac / current_therapy_settings.setpoint.trip_load;

    }
    /* case 2: output greater than or equal to 188 volts */
    else
    {

      /* formula: (98 / ohms)*Irms + 6*(dac/4096) + 0.6A
                  (128*irms/trip_load) + (4964 counts/A * 6 * dac/4096) + 3 */
      ipeak_threshold = 
        ((adc_result1 << 7) / current_therapy_settings.setpoint.trip_load) + ((29784 * dac) / 4096);
      /* convert to 6-bit */
      ipeak_threshold >>= 10;
      ipeak_threshold += 3;
    }

    /* minimum value that we never want less than 3 */
    if (ipeak_threshold < 8)
    {

      ipeak_threshold = 8;

    }
    /* maximum value we can measure is 9A */
    else if (ipeak_threshold >= 44)
    {

      ipeak_threshold = 44;

    }
    
    dac_print = dac;
    ipeak_threshold_print = ipeak_threshold;
    irms2_print = adc_result1;

    CMP3_DACCR = CMP_DACCR_DACEN_MASK | CMP_DACCR_VRSEL_MASK | CMP_DACCR_VOSEL(ipeak_threshold);

    fast_idx++;

    return 0;

}


/***************************************************************************//**
Finds irms value from previous samples using a filter
@todo Need more description
@param void
@return 0
@outputs
@context: event driven from adc1 result interrupt occuring every 100usec. We have
these peak detect current limits to protect the circuit from getting damaged.
*******************************************************************************/
uint8_t legacy_irms_adc_result(void)
{
  static uint8_t fast_idx = 0;
  static uint16_t z_time = 0;
  uint32_t ipeak_threshold = 0;
  const uint8_t trip_load = 43;
  
  if (fast_idx >= 200)  //event occurs every 20ms
  {
    put_event(&evt_tbl,RUN_PUMP_SPEED_CTRL);
    fast_idx = 0;
  }
  
  adc_result0 = (uint16_t) ADC1_RA; //ADC0_RA
  
  if ((FTM0_SWOCTRL & FTM_SWOCTRL_CH0OC_MASK) == 0)
  {
    if (legacy_wand_params_obj.z_test == FALSE)
    {
    
        current_therapy_settings.legacy_wand_levels.dac = legacy_set_buck_voltage(adc_result0,current_therapy_settings.legacy_wand_levels.dac);
        //current_therapy_settings.setpoint.dac = current_therapy_settings.setpoint.level;
        z_time = 0;
    }
    else if (current_therapy_settings.legacy_wand_levels.dac > legacy_wand_params_obj.impedance.z_level)
    {
       current_therapy_settings.legacy_wand_levels.dac = legacy_ramp_down_buck_voltage(current_therapy_settings.legacy_wand_levels.dac);
    }
    else if(z_time < legacy_wand_params_obj.impedance.z_time)
    {
      z_time++;
      SET_DAC(pLegacyWandDAC,legacy_wand_params_obj.impedance.z_level);
      current_therapy_settings.legacy_wand_levels.dac = legacy_wand_params_obj.impedance.z_level;
      
      if ((adc_result0 >= legacy_wand_params_obj.impedance.z_high) ||
          (adc_result0 < legacy_wand_params_obj.impedance.z_low))
      {
        z_time = 0;
      }
      if (z_time == legacy_wand_params_obj.impedance.z_time)
      {
       legacy_wand_params_obj.z_test = FALSE;
       z_time = 0;
      }
    }
   }
  else 
  {
    /* have to ramp down to get plasma restarted */
    current_therapy_settings.legacy_wand_levels.dac = legacy_ramp_down_buck_voltage(current_therapy_settings.legacy_wand_levels.dac);
    //current_therapy_settings.setpoint.dac = 0;
    //SET_DAC(pSmartWandDAC,0);
    z_time = 0;
    
  }
  if (fast_idx++%10==0)
  {
    put_event(&evt_tbl,LW_RUN_IEC_OVERCURRENT);
    
    if (current_therapy_settings.legacy_wand_levels.topaz_timer)
    {// Do this whole case only for timer wands
      /*
        @ENT
        Keep track of time
      */
      if(current_therapy_settings.controller == SPORTS_MED)
      {
        legacy_timer_wands();
      }
      else
      {
        legacy_ent_timer_wands();
      }
    }
    
  }
  
  /****************************************************************************/
  // dynamic peak current detect and cut off
  //
  //
  /***************************************************************************/
  
   /* case 1: output less than 188 volts */
    if (current_therapy_settings.legacy_wand_levels.dac < 2264)
    {
      ipeak_threshold = current_therapy_settings.legacy_wand_levels.dac / trip_load;

    }
    /* case 2: output greater than or equal to 188 volts */
    else
    {

      /* formula: 2*irms + 6*(dac/4096) + 0.6A
                  2*(irms/2A/volt) + (4964 counts/A * 6 * dac/4096) + 3 */
      ipeak_threshold = ((adc_result0 << 7) / trip_load) + ((29784 * current_therapy_settings.legacy_wand_levels.dac) / 4096);
      /* convert to 6-bit */
      ipeak_threshold >>= 10;
      ipeak_threshold += 3;
    }

    /* minimum value that we never want less than 3 */
    if (ipeak_threshold < 8)
    {

      ipeak_threshold = 8;

    }
    /* maximum value we can measure is 9A */
    else if (ipeak_threshold >= 44)
    {

      ipeak_threshold = 44;

    }

    // use comparator 1 for legacy wand.
    CMP1_DACCR = CMP_DACCR_DACEN_MASK | CMP_DACCR_VRSEL_MASK | CMP_DACCR_VOSEL(ipeak_threshold);
 
//    dac_print = current_therapy_settings.legacy_wand_levels.dac;
//    ipeak_threshold_print = ipeak_threshold;
//    irms2_print = adc_result0;
    
    return 0;
}

/***************************************************************************//**
Monitor and detect over energy events to comply with IEC 60601-2-2.
Over energy event are defined as when the average root-mean-square of
alternating current measured through the return electrode multiplied by the
set point voltage exceeds 400 watts for a predetermined amount of time, but
never to exceed 1 second. The result of over energy events is a pulsed RF
output.
@param void
@return 0
@outputs
@context This function runs every 1 msec. Quantum had a HW overenergy trip 
circuit, which integrated the current and when it reached a certain value,cutoff
the RF off. This function tries to mimic the same functionality. In Quantum, the
max current allowed was 1.25A. 
*******************************************************************************/
uint8_t legacy_irms_over_energy()
{
 
  static uint8_t count = 0;
  static float previous_y = 0;
  static float previous_x = 0;
  static float x = 0;
  static float y = 0;
  x = (float) adc_result0;
  
  // R = 475k
  // C = 0.1uF
  y = (0.010417047409516*(previous_x+x))+(0.979165905180968*previous_y);
  uint16_t over_energy_current_limit2 = over_energy_current_limit;
  
  if (count > 0)
  {
    count--;    
    if (count == 0)
    {
      put_event(&evt_tbl,RF_ON);
    }
  }
  previous_y = y;
  previous_x = x;
  //1.25A 12410
  //1.30A 12907
  //1.33A 13205
  //1.35A 13403
  //1.40A 13900
  //1.60A 15866
  
  // 1.35A limit is used to match quantum performace emperically. 
  // The RF should start pulsing around 217 Ohms for lvl 9. 
  // (Since Quantum and WW 18 pin puts out about 300 V at lvl 9 and lvl 10,
  // the pulsing should start at 300*300/400 = 225 Ohms. 
  // The voltage putout depends on the load resistance and tolerances.
 
  if (y > over_energy_current_limit2)
  {
    put_event(&evt_tbl,RF_OFF);
    count = 30;
    
  }

  static uint8_t peak_detect_delay_count = 0;
   
  if (legacy_wand_params_obj.peak_detect_flag == 1)
  {
    legacy_wand_params_obj.peak_detect_flag = 0;
    peak_detect_delay_count = 1;
  }
  if (peak_detect_delay_count >= 1)
  {
    peak_detect_delay_count++;
  }
  
  if (peak_detect_delay_count > 30) // delay for 40msec
  {
    peak_detect_delay_count = 0;
    put_event(&evt_tbl, RF_ON);
  }
     
  return 0;
}

/***************************************************************************//**
This function is for timer wands. It cuts off RF after a given time (in msec)
@param void
@return 0
@outputs
@context This function runs every 1 msec. When a certain time period expires,the
RF is cutoff.
*******************************************************************************/
void legacy_timer_wands ()
{
  if (current_therapy_settings.treatment.ablate_on ||
      current_therapy_settings.treatment.coag_on )
  {//if RF switches are pressed. The time limit counter starts as soon as therapy
    // starts.  Even if RF is pulsing, the counter keeps incrementing
    if(++legacy_wand_params_obj.timer.current > legacy_wand_params_obj.timer.maximum)
    {
      put_event(&evt_tbl, LW_ABLATE_OFF_EVENT);
      //MAKE SURE IT DOES NOT TURN ON WHEN THE PEDAL IS STILL PRESSED
    }
  }
  else
  {
    legacy_wand_params_obj.timer.current = 0;
  }

}

/***************************************************************************//**
@ENT
This function is for ent timer wands. rings a tone based on the max time
@param void
@return 0
@outputs
@context This function runs every 1 msec. When a certain time period expires,the
tone sounds.
*******************************************************************************/
void legacy_ent_timer_wands(void)
{
    if(current_therapy_settings.treatment.ablate_on)
    {
      if(--legacy_wand_params_obj.timer.current == 1000)
      {
        if(legacy_wand_params_obj.timer.audio_muted == FALSE)
        {
          current_therapy_settings.audio.p_sound_file = lib_sounds[7].p;
          current_therapy_settings.audio.ambient_alarm_page = 0;

          audio_alarmsound();

          current_therapy_settings.gui.alarm_test_sound = 1;
        }
        legacy_wand_params_obj.timer.current = legacy_wand_params_obj.timer.maximum;
      }
    }
    else
    {
      legacy_wand_params_obj.timer.current = legacy_wand_params_obj.timer.maximum;
    }
}
/***************************************************************************//**
Inits the adc. This is a newer version which inits the adc.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t adc_init_2(void)
{

  /* Define and initialize adc command words in p-flash */
  //const uint32_t adc_cmd_word_ipeak1 = 0x00000044;
  const uint32_t adc_cmd_word_irms1 = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(0);
  //const uint32_t adc_cmd_word_ipeak2 = 0x0000042;
  const uint32_t adc_cmd_word_irms2 = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(19);
  //const uint32_t adc_cmd_word_hall = 0x00000054;
  //const uint32_t adc_cmd_word_mod2 = 0x0000004E;
  //const uint32_t adc_cmd_word_mod1 = 0x0000004F;

  ADC_MemMapPtr pADC0 = ADC0_BASE_PTR;
  ADC_MemMapPtr pADC1 = ADC1_BASE_PTR;

  /* Enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;

  SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;

  /* Loop ADC0, ADC1, ADC2 & ADC3 */
  //SIM_MCR |= SIM_MCR_PDBLOOP_MASK;

  /* Set very high speed operation bit (ADHSC = 1) */
  //ADC0_CFG2 |= ADC_CFG2_ADHSC_MASK ;
  //ADC1_CFG2 |= ADC_CFG2_ADHSC_MASK ;
  //ADC2_CFG2 |= ADC_CFG2_ADHSC_MASK ;

  /* Slow clock to minimum for best calibration result */
  pADC0->CFG1 |= ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(3); // 9.25MHz
  pADC1->CFG1 |= ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(3);

  /* Calibrate */
  ADC_Cal(pADC0);
  ADC_Cal(pADC1);

  /* Configure single ended 16-bit */
  pADC0->CFG1 |= ADC_CFG1_MODE(3);
  pADC1->CFG1 |= ADC_CFG1_MODE(3);

  /* Set software trigger selected bit (ADTRG = 0) */
//  ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK ;

  /* Enable DMA interface */
  //ADC0_SC2 |= ADC_SC2_DMAEN_MASK;

  /* Select channels */
  //ADC0_SC1A = ADC_SC1_ADCH(0x00); /* ADC0_RA */
  //ADC0_SC1B = ADC_SC1_ADCH(0x00); /* ADC0_RB */

  pADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;
  pADC0->SC3 |= ADC_SC3_AVGE_MASK;
  pADC1->SC2 |= ADC_SC2_ADTRG_MASK;
  
  /* AIEN = 1 - conversion complete interrupt enabled, ADCH = 0x9 - Channel 9 is selected as
  SE input */
  //pADC0->SC1[0] = (uint32_t) adc_cmd_word_irms1;
  /* delete next two lines if the two-time measurement per PWM period is not required */
  //pADC0->SC1[1] = (uint32_t) 0;
  /* AIEN = 1 - conversion complete interrupt enabled, ADCH = 0x9 - Channel 9 is selected as
  SE input */
  pADC1->SC1[0] = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(14); //vmod1
  /* delete next two lines if the two-time measurement per PWM period is not required */
  pADC1->SC1[1] = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(15); //vmod2
  /* AIEN = 1 - conversion complete interrupt enabled, ADCH = 0x9 - Channel 9 is selected as
  SE input */
  //pADC2->SC1[0] = (uint32_t) adc_cmd_word_irms1;
  /* delete next two lines if the two-time measurement per PWM period is not required */
  //pADC2->SC1[1] = (uint32_t) adc_cmd_word_irms2;
  /* AIEN = 1 - conversion complete interrupt enabled, ADCH = 0x9 - Channel 9 is selected as
  SE input */
  //pADC3->SC1[0] = (uint32_t) adc_cmd_word_fsw_present;
  /* delete next two lines if the two-time measurement per PWM period is not required */
  //pADC3->SC1[1] = (uint32_t) adc_cmd_word_temperature;


  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_PDB_MASK;

  /* EN = 0x01 | EN = 0x00 - first two pre/triggers are enabled
  TOS = 0x01 | TOS = 0x00 - channels assert when the counter reaches the channel
  delay register plus one peripheral clock cycle after PDB trigger */
  PDB0_MOD = 7400;
  //PDB0_CH0C1 |= PDB_C1_EN(1) | PDB_C1_TOS(1);
  PDB0_CH1C1 |= PDB_C1_EN(3) | PDB_C1_TOS(3);

  /* set the delay value for the channel's corresponding pre-triggers */
  //PDB0_CH0DLY0 = 3700;
  //PDB0_CH0DLY1 = 2960;
  PDB0_CH1DLY0 = 3700;
  //PDB0_CH1DLY1 = 6936;
  PDB0_CH1DLY1 = 1850;

  /* PDBEN = 1 - PDB enabled, TRGSEL = 0x9 - FTM1 is a trigger source for PDB,
  LDOK - writing 1 to this bit updates the internal registers*/
  PDB0_SC |= PDB_SC_PDBEN_MASK | PDB_SC_TRGSEL(0x7) | PDB_SC_PDBEIE_MASK |
             PDB_SC_PDBIE_MASK | PDB_SC_CONT_MASK | PDB_SC_PRESCALER(0);

  PDB0_SC |= PDB_SC_LDOK_MASK;

  /* enable adc0 irq */
  NVIC_EnableIRQ(ADC0_IRQn); // adc0
  //NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)[a]
  NVIC_EnableIRQ(ADC1_IRQn); // adc1
  //NVIC_EnableIRQ(102); // adc2
  //NVIC_EnableIRQ(103); // adc3

  /* enable pdb irq */
  NVIC_EnableIRQ(PDB0_IRQn);
  
  return 1;

}

/***************************************************************************//**
check for internal temp
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t adc0_coco_stator(void)
{

  uint32_t adc_cmd_word_stator = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(16);

  uint16_t adc_result0 = 0;

  adc_result0 = (unsigned short) ADC0_RA;

  /* stator sensor */
  if (ADC0_SC1A == adc_cmd_word_stator)
  {
   
   current_therapy_settings.pump.stator = adc_result0;

  }
  else
  {
   
   ADC0_SC1A = adc_cmd_word_stator;
   
  }
   
  return 0;
   
}

bool ent_blendizer(uint16_t prime_time, uint16_t total_time, bool reset)
{
	static int32_t duty = -1;
	bool primary = TRUE;
        
        if (reset == TRUE)
        {
          duty = -1;
	  primary = TRUE;
        }
        else
        {
          if (++duty < (int32_t)prime_time)
	  {
		primary = TRUE;
          }
	  else if (duty <= (int32_t)total_time)
	  {
		primary = FALSE;
	  }
	  /*reset*/
	  if (duty >= (int32_t)total_time)
	  {
                duty = 0;
		primary = TRUE;
	  } 
        }
	
	return primary;
}


uint8_t print_debug (void)
{
  #ifdef DEBUG_K26
  //printf("Ipeak_threshol_calc:(%d), Dac:(%d), Irms2(%d), Ipeak_counts:(%d) \n",ipeak_threshold_print,dac_print,irms2_print,current_therapy_settings.setpoint.trip_load); 
  
  #endif
  return 0;
}
