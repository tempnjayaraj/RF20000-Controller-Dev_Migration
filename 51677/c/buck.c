/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
*******************************************************************************/
/*******************************************************************************
$Author: Harshil.Shah $
$Date: 2022-06-29 10:53:19 -0500 (Wed, 29 Jun 2022) $
$Modtime: 7/02/13 5:10p $
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file     buck.c
\brief     DC-DC buck converter.
\details   N/A
*******************************************************************************/

#include "buck.h"
#include "pit.h"
#include "system.h"
#include "legacy_wand.h"

#define BUCK_MAX_DAC                    4095u
/** buck 18-pin dac output */
const DAC_MemMapPtr pLegacyWandDAC = DAC0_BASE_PTR;
VMOD_COMPENSATION vmod;

/** buck smart wand dac output */
const DAC_MemMapPtr pSmartWandDAC = DAC1_BASE_PTR;

/** legacy wand parameters */
extern legacy_wand_params legacy_wand_params_obj;

static const uint8_t LEGACY_RAMP_DOWN_CONST = 30;
//static const uint8_t LEGACY_RAMP_UP_CONST = 96; // 8.0v/100usec
static const uint8_t LEGACY_RAMP_UP_CONST = 15; // 1.2v/100usec

extern bool unitTest;

/***************************************************************************//**
Initialize buck
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t buck_init(void)
{
  /* Initialize */
  PORTE_PCR24 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // buck_ww_on LTC2927
  PORTE_PCR26 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // buck_18pin_on LTC2927

  /* Set pins as outputs */
  GPIOE_PDDR |= GPIO_PDDR_PDD(BUCK_WW_ON | BUCK_18PIN_ON);
  GPIOE_PSOR |= GPIO_PDOR_PDO(BUCK_WW_ON | BUCK_18PIN_ON);

  /* enable gate clocks */
  SIM_SCGC2 |= SIM_SCGC2_DAC0_MASK | SIM_SCGC2_DAC1_MASK;

  /* configure dacs */
  pLegacyWandDAC->C0 |= DAC_C0_DACEN_MASK | /* Enable dac */
                        DAC_C0_DACRFS_MASK;// | /* use DACREF_2 (VDDA) */


  pSmartWandDAC->C0 |= DAC_C0_DACEN_MASK | /* Enable dac */
                       DAC_C0_DACRFS_MASK;// | /* use DACREF_2 (VDDA) */


  /* set both outputs to zero */
  SET_DAC(pSmartWandDAC,0);
  SET_DAC(pLegacyWandDAC,0);

  /* enable buffer */
  pLegacyWandDAC->C1 |= DAC_C1_DACBFMD(2) | DAC_C1_DACBFEN_MASK;
  pSmartWandDAC->C1 |= DAC_C1_DACBFMD(2) | DAC_C1_DACBFEN_MASK;

  return 1;

}

/***************************************************************************//**
*sets the WW dac based on irms current value and current dac value. Used by
WW only
*@param irms RMS current
*@param current_dac curent DAC value
*@return the voltage the DAC is set
*******************************************************************************/
uint16_t set_buck_voltage(uint16_t irms, uint16_t current_dac)
{

  uint32_t adjustment;

  if (irms <= current_therapy_settings.pulsing.target_current)
  {

    adjustment = (4 * (current_therapy_settings.pulsing.target_current - irms));
    adjustment /= 100;
    if (adjustment > smart_wand_params.ramp_rate.increase)
    {

      adjustment = smart_wand_params.ramp_rate.increase;

    }

    if (current_therapy_settings.setpoint.level < current_dac)
    {
      if ((current_dac - current_therapy_settings.setpoint.level) > smart_wand_params.ramp_rate.decrease)
      {
        current_dac -= smart_wand_params.ramp_rate.decrease;
      }
      else
      {
        current_dac = current_therapy_settings.setpoint.level;
      }
    }
    else if ((current_therapy_settings.setpoint.level - current_dac) < adjustment)
    {
       current_dac = current_therapy_settings.setpoint.level;
    }
    else
    {
       current_dac += adjustment;
    }

  }
  else
  {

    adjustment = (4 * (irms - current_therapy_settings.pulsing.target_current));
    adjustment /= 100;
    if (adjustment > smart_wand_params.ramp_rate.decrease)
    {
       adjustment = smart_wand_params.ramp_rate.decrease;
    }

    if (current_dac >= adjustment)
    {
       current_dac -= adjustment;
    }
    else
    {
       current_dac = 0;
    }

  }
  set_dac(current_dac);
  return current_dac;
}
/***************************************************************************//**
*sets the legacy wand DAC voltage in accordance with the prescribed slope
This function is exclusively used by 18 pin because it ramps down is different
from WW.
*@param irms RMS current
*@param current_dac curent DAC value
*@return the voltage the DAC is set to
*******************************************************************************/
uint16_t legacy_set_buck_voltage(uint16_t irms, uint16_t current_dac)
{
   uint32_t adjustment;

   adjustment = LEGACY_RAMP_UP_CONST;

   if ((legacy_wand_params_obj.level <= current_dac) ||
       ((legacy_wand_params_obj.level - current_dac) < adjustment))
   {
      current_dac = legacy_wand_params_obj.level;
   }
   else
   {
      current_dac += adjustment;
   }
   SET_DAC(pLegacyWandDAC,current_dac);
   return current_dac;
}

/***************************************************************************//**
*Ramps down the legacy wand DAC voltage in accordance with the prescribed slope
This function is exclusively used by 18 pin because it ramps down is different
from WW.
*@param current_dac - value at which the 18 pin is set right now
*@return The new (and lower) value to which the dac has been set
*******************************************************************************/
uint16_t legacy_ramp_down_buck_voltage(uint16_t current_dac)
{

  /* 25 V / ms = 24 counts / 10 us */
  if (current_dac > LEGACY_RAMP_DOWN_CONST)
  {

    current_dac -= LEGACY_RAMP_DOWN_CONST;

  }
  else
  {
     current_dac = 0;
  }

  SET_DAC(pLegacyWandDAC,current_dac);
  return current_dac;
}


/***************************************************************************//**
*This function is used to ramp down the buck voltage during the testing and
calibration phase
*@param pdac - pointer to dac (ww or 18 pin)
*@param dac_cmd - desired dac value
*@param reset - resets ramp down
*@return the voltage the DAC is set to
*******************************************************************************/
uint16_t ramp_down_buck_test_voltage(DAC_MemMapPtr pdac, uint16_t dac_cmd, uint8_t reset)
{

  uint16_t ramp_target[5] = {3855,2891,1927,963,481};
  uint16_t updated_dac = 0;
  static uint8_t test_cnt = 0;

  if (reset == 1)
  {
    test_cnt = 0;

  }
  /* 25 V / ms = 24 counts / 10 us */
  if (dac_cmd > 240)
  {

    updated_dac = dac_cmd - 240;

  }
  else
  {

    updated_dac = 0;

  }
  

  for (uint8_t i = 0; i < 5; i++)
  {

    if ((updated_dac <= ramp_target[i]) && (dac_cmd >= ramp_target[i]))
    {

      if (test_cnt++ <= 4)
      {

        updated_dac = ramp_target[i];

      }

      break;

    }

  }

  if (test_cnt > 5)
  {

    test_cnt = 0;

  }

  if (pdac == pLegacyWandDAC)
  {
    
    SET_DAC(pdac,updated_dac);
    
  }
  else
  {
    
    SET_DAC(pdac,vmod.vmod2_lookup[updated_dac]);
    
  }

  return updated_dac;

}


/***************************************************************************//**
*Ramps down the WW wand buck voltage according to the prescribed slope (which is
need for the HW to shut down properly)
*@param pdac - pointer to the DAC (WW DAC or 18 pin DAC). This function is used
just exclusively by WW because the ramps may be different
*@param current_dac curent DAC value
*@return the voltage the DAC is set to
*******************************************************************************/
uint16_t ramp_down_buck_voltage(DAC_MemMapPtr pdac, uint16_t current_dac)
{

  /* 25 V / ms = 24 counts / 10 us */
  if (current_dac > 240)
  {

    current_dac -= 240;

  }
  else
  {

    current_dac = 0;

  }

  SET_DAC(pdac,current_dac);

  return current_dac;

}

/***************************************************************************//**
*Shuts down WW buck
*@param void
*@return 0
*******************************************************************************/
uint8_t buck_inactive_shutdown(void)
{

  /* switch off buck */
  GPIOE_PSOR |= GPIO_PDOR_PDO(BUCK_WW_ON);

  return 0;

}
/***************************************************************************//**
*Kicks off the vmod ramping sequence.
*@param 0
*@return 0
*******************************************************************************/
uint8_t vmod2_test(void)
{

  vmod.vmod2_test_status = vmod2_ramp_test(0);
  
  return 0;

}

/***************************************************************************//**
*Kicks off the vmod ramping sequence.
*@param 0
*@return 0
*******************************************************************************/
uint8_t vmod1_test(void)
{

  vmod.vmod1_test_status = vmod1_ramp_test(0);
  
  return 0;

}

/***************************************************************************//**
This function ramps the vmod voltage from 0 to BUCK_MAX_DAC. At each stop the actual
output is measured, compared to the desired value and is used in calibrating the
vmod out. 
*@param resets the ramp up.
*@return the satus of the VMOD calibration sequence
*******************************************************************************/
VMOD_TEST_STATUS vmod2_ramp_test(uint8_t reset)
{

  const uint16_t dac = GET_DAC(pSmartWandDAC);
  static uint16_t sample_idx = 0,
                  dac_cmd = 0;
  VMOD_TEST_STATUS status = vmod.vmod2_test_status;
  const uint16_t vmod_ulimit = 2560,
                 vmod_llimit = 1536;

  if (reset == 1)
  {

    status = VMOD_TEST_RESET;

  }


  switch(status)
  {

    case(RAMP_UP):
    {

      uint16_t next_dac = 0;
      uint16_t ulimit_dac,
               llimit_dac;

      if (dac == 0)
      {

        ulimit_dac = 1;

      }
      /* 20% more than 3,412 is 12-bit max */
      else if (dac > 3412)
      {

        ulimit_dac = BUCK_MAX_DAC;

      }
      else
      {

        ulimit_dac = (dac * 12) / 10;

      }
      
      if (dac < 2)
      {
        
        llimit_dac = 0;
        
      }
      else
      {
        
        llimit_dac = (dac * 8) / 10;
        
      }

      if (sample_idx < 511)
      {

        next_dac = buck_log_sample_vector[sample_idx++];

      }
      else
      {

        next_dac = buck_log_sample_vector[511];

      }

      if (dac == 0)
      {

        vmod.vmod2_meas[dac] = 0;

        SET_DAC(pSmartWandDAC,next_dac);

      }
      else if (dac == 2056)
      {
        
        vmod.vmod2_meas[dac] =
        (vmod.mod2_meas >> 4) & 0xFFF;
        
        /* test if vmod is within 25% */
        if ((vmod.vmod2_meas[2056] > vmod_ulimit) ||
            (vmod.vmod2_meas[2056] < vmod_llimit))
        {
          
          put_event(&evt_tbl,VMOD_FAULT);
          
        }
        
        SET_DAC(pSmartWandDAC,next_dac);
       
      }
      else if (dac < BUCK_MAX_DAC)
      {

        vmod.vmod2_meas[dac] =
          (vmod.mod2_meas >> 4) & 0xFFF;

        SET_DAC(pSmartWandDAC,next_dac);

      }
      else
      {

        vmod.vmod2_meas[dac] =
          (vmod.mod2_meas >> 4) & 0xFFF;

        status = VMOD_INTERP;
        interp2(1);

      }

      if (vmod.vmod2_meas[dac] > ulimit_dac)
      {
        
        vmod.vmod2_meas[dac] = ulimit_dac;
        
      }
      else if (vmod.vmod2_meas[dac] < llimit_dac)
      {
        
        vmod.vmod2_meas[dac] = llimit_dac;
        
      }
      
      if (dac > 0)
      {

        if (vmod.vmod2_meas[dac-1] >
            vmod.vmod2_meas[dac])
        {

          vmod.vmod2_meas[dac] =
            vmod.vmod2_meas[dac-1];

        }

      }

      break;

    }
    case(VMOD_INTERP):
    {
      break;
    }
    case(VMOD_TEST_HOLD):
    {
      sort_vmeas(&vmod.vmod2_meas[0]);
      status = RAMP_DOWN;
      dac_cmd = ramp_down_buck_test_voltage(pSmartWandDAC,dac,1);

      break;

    }
    case(RAMP_DOWN):
    {

      dac_cmd = ramp_down_buck_test_voltage(pSmartWandDAC,dac_cmd,0);

      if (dac_cmd == 0)
      {

        status = VMOD_TEST_DONE;

      }

      break;

    }
    case(VMOD_TEST_DONE):
    {

      status = VMOD_TEST_DONE;


      break;

    }
    case(VMOD_TEST_RESET):
    {

      sample_idx = 0;
      status = RAMP_UP;
      dac_cmd = 0;

      break;

    }
    default:
    {

      sample_idx = 0;
      status = VMOD_TEST_RESET;
      dac_cmd = 0;

    }

  }

  return status;

}

/***************************************************************************//**
This function ramps the vmod voltage from 0 to BUCK_MAX_DAC. At each stop the actual
output is measured, compared to the desired value and is used in calibrating the
vmod out. 
*@param resets the ramp up.
*@return the satus of the VMOD calibration sequence
*******************************************************************************/
VMOD_TEST_STATUS vmod1_ramp_test(uint8_t reset)
{

  const uint16_t dac = GET_DAC(pLegacyWandDAC);
  static uint16_t vmod1_sample_idx = 0,
                  vmod1_dac_cmd = 0;
  VMOD_TEST_STATUS status = vmod.vmod1_test_status;
  const uint16_t vmod_ulimit = 2560,
                 vmod_llimit = 1536;

  if (reset == 1)
  {

    status = VMOD_TEST_RESET;

  }


  switch(status)
  {

    case(RAMP_UP):
    {

      uint16_t next_dac = 0,
               vmod1_2056 = 0;

      if (vmod1_sample_idx < 511)
      {

        next_dac = buck_log_sample_vector[vmod1_sample_idx++];

      }
      else
      {

        next_dac = buck_log_sample_vector[511];

      }

      if (dac == 0)
      {

        SET_DAC(pLegacyWandDAC,next_dac);

      }
      else if (dac == 2056)
      {
        
        vmod1_2056 = (vmod.mod1_meas >> 4) & 0xFFF;
        
        /* test if vmod is within 25% */
        if ((vmod1_2056 > vmod_ulimit) ||
            (vmod1_2056 < vmod_llimit))
        {
          
          put_event(&evt_tbl,VMOD_FAULT);
          
        }
        
        SET_DAC(pLegacyWandDAC,next_dac);
       
      }
      else if (dac < BUCK_MAX_DAC)
      {

        SET_DAC(pLegacyWandDAC,next_dac);

      }
      else
      {

        status = VMOD_TEST_HOLD;

      }

      break;

    }
    case(VMOD_TEST_HOLD):
    {
    
      status = RAMP_DOWN;
      vmod1_dac_cmd = ramp_down_buck_test_voltage(pLegacyWandDAC,dac,1);

      break;

    }
    case(RAMP_DOWN):
    {

      vmod1_dac_cmd = ramp_down_buck_test_voltage(pLegacyWandDAC,vmod1_dac_cmd,0);

      if (vmod1_dac_cmd == 0)
      {

        status = VMOD_TEST_DONE;

      }

      break;

    }
    case(VMOD_TEST_DONE):
    {

      status = VMOD_TEST_DONE;


      break;

    }
    case(VMOD_TEST_RESET):
    {

      vmod1_sample_idx = 0;
      status = RAMP_UP;
      vmod1_dac_cmd = 0;

      break;

    }
    default:
    {

      vmod1_sample_idx = 0;
      status = VMOD_TEST_RESET;
      vmod1_dac_cmd = 0;

    }

  }

  return status;

}

/***************************************************************************//**
*sets the WW wand DAC to a give value
*@param the desired DAC output
*@return the voltage the DAC is set to
*******************************************************************************/
uint16_t set_dac(uint16_t command)
{

  uint16_t dac_value = 0;

  if (command < vmod.vmod2_lookup[0])
  {

    dac_value = 0;
    SET_DAC(pSmartWandDAC,dac_value);

  }
  else if (command >=vmod.vmod2_lookup[BUCK_MAX_DAC])
  {

    dac_value = vmod.vmod2_lookup[BUCK_MAX_DAC];
    SET_DAC(pSmartWandDAC,dac_value);

  }
  else
  {
    dac_value = vmod.vmod2_lookup[command];
    SET_DAC(pSmartWandDAC,dac_value);

  }

  return dac_value;

}

/***************************************************************************//**
*Sorts the measured voltage for vmod look up.
*@param pointer to measured voltage
*@return void
*******************************************************************************/
void sort_vmeas(uint16_t *p_vmeas)
{

  uint16_t i = 0;
  
  do
  {
    vmod.vmod2_lookup[*p_vmeas] = i;
  }while ((i++ < BUCK_MAX_DAC) && (*p_vmeas++ < BUCK_MAX_DAC));

  for (uint16_t i = 0; i < BUCK_MAX_DAC; i++)
  {

    if (vmod.vmod2_lookup[i] >
        vmod.vmod2_lookup[i+1])
    {

      vmod.vmod2_lookup[i+1] =
        vmod.vmod2_lookup[i] ;

    }

  }

}

/***************************************************************************//**
This is the interpolation function which is used to interpolate the vmod values
inbetween measured values used in vmod calibration.
@param reset - resets the interpolation variables.
@return 0
@outputs
@context
*******************************************************************************/
uint8_t interp2(uint8_t reset)
{
  
  uint16_t x0 = 0,
           x1 = 0,
           y0 = 0,
           y1 = 0,
           complete = 0;
  
  static uint16_t vmod_idx = 0,
                  match_idx = 0;
  
  if (reset == 1)
  {
    
    vmod_idx = 0;
    match_idx = 0;
    
  }
    
  for (uint16_t j = match_idx; j < 511; j++)
  {

    if ((buck_log_sample_vector[j] < vmod_idx) && (buck_log_sample_vector[j+1] > vmod_idx))
    {
      
      x0 = buck_log_sample_vector[j];
      x1 = buck_log_sample_vector[j+1];
      match_idx = j;
      
      y0 = vmod.vmod2_meas[x0];
      y1 = vmod.vmod2_meas[x1];
      
      vmod.vmod2_meas[vmod_idx] =
        y0 + (y1 - y0) * (vmod_idx - x0) / (x1 - x0);
      
    }
    
  }
  
  vmod_idx++;
  
  if (vmod_idx >= 4096)
  {
    
    complete = 1;
    match_idx = 0;
    
  }
  else
  {
    #ifndef TEST
     // When unit testing, this throws the the put event
     // 4096 times which makes CMOCK run out of memory
     // So this statement is comment out just for testing
    put_event(&evt_tbl,VMOD_LINEAR_INTERP);
    #endif
  }
  
  return complete;
      
}

/***************************************************************************//**
This function starts the vmod interpolation function used in vmod calibration.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t vmod_interp(void)
{
  
  uint8_t complete = 0;
  
  complete = interp2(0);
  
  if (complete == 1)
  {
    
    vmod.vmod2_test_status = VMOD_TEST_HOLD;
    
  }
  
  return 0;
  
}

/***************************************************************************//**
*sets the legacy wand DAC voltage in accordance with the prescribed slope
This function is exclusively used by 18 pin because it ramps down is different
from WW.
*@param irms RMS current
*@param current_dac curent DAC value
*@return the voltage the DAC is set to
*******************************************************************************/
uint16_t set_buck_voltage_constant_power(uint16_t irms, uint16_t dac, float32_t power_target, uint16_t vlimit)
{

  float32_t target_dac_f32 = 0.0f;
  uint16_t target_dac_u16 = 0u; 
  volatile float32_t ohms = 0.0f;
  volatile float32_t temp = 0.0f;
  
  if (irms > 0u)
  {
    ohms = (float32_t) dac / irms;  
  }
  else
  {
    ohms = (float32_t) dac;
  }
  
  if (ohms == 0)
  {
    ohms = 1;
  }
  
  temp = (float32_t) power_target * ohms;
  
  /* calculate new target voltage */
  if (!unitTest) arm_sqrt_f32(temp,&target_dac_f32);
  else target_dac_f32 = 4095.5;
    
  if (target_dac_f32 >= 4094.4f)
  {
    target_dac_u16 = BUCK_MAX_DAC;
  }
  else
  {
    target_dac_u16 = (uint16_t)(target_dac_f32 + 0.5f);
  }
  
  /* no reason the target should ever be more than vlimit */
  if (target_dac_u16 > vlimit)
  {
    target_dac_u16 = vlimit;
  }

/* set the dac */  
if (target_dac_u16 > dac) 
{
	if (dac > vlimit)
	{
		if ((dac - vlimit) > 240u)
		{
			dac -= 240u;
		}
		else
		{
			dac = vlimit;
		}
	}
	else
	{
		if ((target_dac_u16 - dac) > 400u)
		{
			dac += 400u;
		}
		else
		{
			dac = target_dac_u16;
		}	
	}
}
else if (target_dac_u16 < dac)
{
        if ((dac - target_dac_u16) > 240u)
        {
                dac -= 240u;
        }
        else
        {
                dac = target_dac_u16;
        }
	
}
else if (dac > vlimit)
{
        if ((dac - vlimit) > 240u)
        {
                dac -= 240u;
        }
        else
        {
                dac = vlimit;
        }
}
  
  return dac;
}

unsigned char vmod1_adc_coco(void)
{
  static uint8_t vmod1_sa_cnt = 0;
  volatile uint16_t adc_result0 = (uint16_t)ADC1_RA;
    
  if (vmod.vmod1_test_status == VMOD_TEST_DONE)
  {
    PDB0_SC &= ~PDB_SC_PDBEN_MASK;
    ADC1_SC1A = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(0); //irms1
    PDB0_SC |= PDB_SC_PDBEN_MASK;
  }
  else
  {
    if (adc_result0 > 56301)
    {
      adc_result0 = 0xFFFFu;   
    }
    else
    {    
      adc_result0 = ((adc_result0 * 1164u) / 1000u) & 0xFFFFu;   
    }
    /* weighted filter */
    vmod.mod1_meas =
    (adc_result0 >> 1) + (vmod.mod1_meas >> 1);      
    if (vmod1_sa_cnt == 0)
    {
      put_event(&evt_tbl,VMOD1_TEST);
    }
    vmod1_sa_cnt = (vmod1_sa_cnt + 1) % 20;
    //ADC1_SC1A = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(14); //vmod1
  }
  return 0;
}

unsigned char vmod2_adc_coco(void)
{ 
  static uint8_t vmod2_sa_cnt = 0;
  volatile uint16_t adc_result0 = (uint16_t)ADC1_RB;

  if (vmod.vmod2_test_status == VMOD_TEST_DONE)
  {
    PDB0_SC &= ~PDB_SC_PDBEN_MASK;
    ADC1_SC1B = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(19); //irms2
    PDB0_SC |= PDB_SC_PDBEN_MASK;
  }
  else
  {
    if (adc_result0 > 56301)
    {
      adc_result0 = 0xFFFFu;   
    }
    else
    {    
      adc_result0 = ((adc_result0 * 1164u) / 1000u) & 0xFFFFu;   
    }
    /* weighted filter */
    vmod.mod2_meas =
    (adc_result0 >> 1) + (vmod.mod2_meas >> 1);
    if (vmod2_sa_cnt == 0)
    {
      put_event(&evt_tbl,VMOD2_TEST);
    }
    vmod2_sa_cnt = (vmod2_sa_cnt + 1) % 20; 
    //ADC1_SC1B = ADC_SC1_AIEN(1) | ADC_SC1_ADCH(15); //vmod2
  }
  return 0;
}


