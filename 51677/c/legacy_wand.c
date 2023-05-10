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
\file:     legacy_wand.c
\brief     Driver for legacy wand.
\details   N/A
*******************************************************************************/

#include "ad7265.h"
#include "MK26F18.h"
#include "dma.h"
#include "legacy_wand.h"
#include "fuse_blow.h"
#include "gui.h"
#include "system.h"
#include "wand.h"
#include "rf.h"
#include "notifications.h"
#include "wand.h"
#include "stdlib.h"
#include "sensors.h"


#define MAX_DAC                 4095
#define WAND_PULL_UP_OHM        499

extern uint32_t start_duration;
static const uint8_t MAX_DISPLAY_TEMP = 127;

static const uint16_t resTable[NUM_OF_RES_TABLE_ENTRY][3] = 
                                     {{R5_MIN_DAC,    R5_MAX_DAC,    0},
                                      {R200_MIN_DAC,  R200_MAX_DAC,  1},
                                      {R412_MIN_DAC,  R412_MAX_DAC,  2},
                                      {R576_MIN_DAC,  R576_MAX_DAC,  3},
                                      {R750_MIN_DAC,  R750_MAX_DAC,  4},
                                      {R909_MIN_DAC,  R909_MAX_DAC,  5},
                                      {R1050_MIN_DAC, R1050_MAX_DAC, 6},
                                      {R121_MIN_DAC,  R121_MAX_DAC,  7},
                                      {R147_MIN_DAC,  R147_MAX_DAC,  8},
                                      {R169_MIN_DAC,  R169_MAX_DAC,  9},
                                      {R243_MIN_DAC,  R243_MAX_DAC,  10},
                                      {R287_MIN_DAC,  R287_MAX_DAC,  11},
                                      {R324_MIN_DAC,  R324_MAX_DAC,  12},
                                      {R365_MIN_DAC,  R365_MAX_DAC,  13},
                                      {R464_MIN_DAC,  R464_MAX_DAC,  14},
                                      {R649_MIN_DAC,  R649_MAX_DAC,  15},
                                      {R1210_MIN_DAC, R1210_MAX_DAC, 16},
                                      {R1400_MIN_DAC, R1400_MAX_DAC, 17},
                                      {R1650_MIN_DAC, R1650_MAX_DAC, 18},
                                      {R1960_MIN_DAC, R1960_MAX_DAC, 19},
                                      {R2400_MIN_DAC, R2400_MAX_DAC, 20},
                                      {R3010_MIN_DAC, R3010_MAX_DAC, 21},
                                      {R3920_MIN_DAC, R3920_MAX_DAC, 22},
                                      {R5600_MIN_DAC, R5600_MAX_DAC, 23},
                                      {R9090_MIN_DAC, R9090_MAX_DAC, 24},
                                      {R10500_MIN_DAC, MAX_DAC,      25} };

// characteristics of default and max set points for ablate, 
// default and max set points for coag
// timer in mSec, 
// T/C, and 
// current limit values for different devices organized according to their tags.
wand_types wand_type_array[25]=
{
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,},     // 0 R1 = 5
  {6,8,1,2,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},    // 1 R1 = 200
  {1,2,1,2,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},    // 2 R1 = 412
  {7,9,1,2,0,YES_COAG_ADJ,YES_TC,NO_DUAL_EL,NO_SETUP,PUMP_ENABLE,1},    // 3 R1 = 576
  {4,4,0,0,500,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},  // 4 R1 = 750   
  {7,9,1,2,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_ENABLE,1},     // 5 R1 = 909
  {5,5,0,0,500,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},  // 6 R1 = 1050   
  {7,10,1,2,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_ENABLE,1},    // 7 R1 = 121   
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 8 R1 = 147  
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 9 R1 = 169 
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 10 R1 = 243
  {4,4,0,0,500,NO_COAG_ADJ,NO_TC,YES_DUAL_EL,NO_SETUP,PUMP_DISABLE,1}, // 11 R1 = 287     
  {1,2,1,2,0,YES_COAG_ADJ,YES_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},   // 12  R1 =324  
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 13 R1 = 365
  {7,10,1,2,0,YES_COAG_ADJ,YES_TC,NO_DUAL_EL,NO_SETUP,PUMP_ENABLE,1},   // 14 R1 = 464 
  {6,8,1,2,0,YES_COAG_ADJ,YES_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},   // 15 R1 = 649
  {7,9,1,2,0,YES_COAG_ADJ,YES_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},    // 16 R1 = 1210
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 17 R1 = 1400
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 18 R1 = 1650
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 19 R1 = 1960
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 20 R1 = 2400   
  {4,6,1,2,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},    // 21 R1 = 3010
  {0,0,0,0,0,NO_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,0},     // 22 R1 = 3920
  {9,9,1,2,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},    // 23 R1 = 5600
  {7,10,1,2,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1} }; // 24 R1 = 9090  
/*
  @ENT
  define the types of CII wands available
*/
wand_types ent_wand_type_array[3]=
{
  {4,6,2,4,10999,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},  // 0 R2 = 750
  {7,9,3,5,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},  // 1 R2 = 909
  {6,9,6,9,0,YES_COAG_ADJ,NO_TC,NO_DUAL_EL,NO_SETUP,PUMP_DISABLE,1},  // 2 R2 = 1400
};

// Buffer used to filter ambient temp
static uint8_t temp_buffer[TEMP_FILTER_BUFF_LENGTH];

// The ADC channel is pulled up when no wand is connected. So for start up
// make all these max_value instead of 0.
STATIC_ARTC uint16_t buffer_r1[] = {4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095};
STATIC_ARTC uint16_t buffer_r2[]= {4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095,
4095,4095,4095,4095};

uint16_t R1_wand_type ;
uint16_t R2_adc_value ;

legacy_wand_params legacy_wand_params_obj;
adc_temp_convertor temp_table[10];

/** DAC values for different ablate levels */
const uint16_t legacy_wand_ablate_dac_values [11] = 
{
 0,
1171,
1473,
1797,
2109,
2432,
2756,
3063,
3379,
3694,
3756
};

/** DAC values for different coag levels */
const uint16_t legacy_wand_coag_dac_values[3] = 
{
  0,
757,
1171
};

/*
// These are current limits for respective setpoints from 1 to 10.
// The limits are given below in Amps
 
1 2.6
2 2.27
3 2.08
4 1.9
5 1.74
6 1.61
7 1.54
8 1.44
9 1.39
10 1.33

*/
const uint16_t legacy_over_energy_current_limits_for_ablate[11] = 
{
0,      // For level 0
25817,
22540,
20653,
18866,
17277,
15987,
15292,
14299,
13802,
13206
};

/* 
C1 3.19A
C2 2.6A
*/
const uint16_t legacy_over_energy_current_limits_for_coag[3] = 
{
  0,   // For level 0
31675,
25817
};

////////////////////////////////Special Limits /////////////////////////////////

/*
// These are current limits for respective setpoints from 1 to 10.
// The limits are given below in Amps for R1= 324, 649, 1210
 
1 2.0
2 1.9
3 1.69
4 1.39
5 1.3
6 1.21
7 1.18
8 1.18
9 1.16
10 0.5 (Level is not reached by any of these wands, so lvl 10 is set very low
        0 for safety reasons)

*/
const uint16_t legacy_over_energy_current_limits_for_ablate_27pin[11] = 
{
0,      // For level 0
19879,
18975,
16807,
13825,
12908,
12108,
11717,
11717,
11518,
4965
};


/* This is for R1 = 324, 649, 1210
C1 2.70A
C2 2.0A
*/
const uint16_t legacy_over_energy_current_limits_for_coag_27pin[3] = 
{
  0,   // For level 0
26884,
19879 
};

/*
  @ENT
  wave form characterizations are different between SportsMed and ENT
*/
//DAC values for different ENT ablate levels
const uint16_t ent_legacy_wand_ablate_dac_values [10] =
{
  0,
  1204, // 100V
  1536, // 127.5V
  1867, // 155V
  2198, // 182.5V
  2529, // 210V
  2861, // 237.5V
  3192, // 265V
  3493, // 290V
  3614, // 300V
};

//DAC values for different ENT coag levels
const uint16_t ent_legacy_wand_coag_dac_values[10] =
{
  0,
  783,  // 65V
  843,  // 70V
  903,  // 75V
  975,  // 81V
  1048, // 87V
  1120, // 93V
  1204, // 100V
  1301, // 108V
  1385, // 115V
};

/*
// These are current limits for respective setpoints from 1 to 10.
// The limits are given below in Amps

1 2.6
2 2.27
3 2.08
4 1.9
5 1.74
6 1.61
7 1.54
8 1.44
9 1.39
10 1.33

*/
const uint16_t ent_legacy_over_energy_current_limits_for_ablate[10] =
{
  0,      // For level 0
  25817,
  22540,
  20653,
  18866,
  17277,
  15987,
  15292,
  14299,
  13802
};

const uint16_t ent_legacy_over_energy_current_limits_for_coag[10] =
{
  0,   // For level 0
  17873, // C1 = 1.8 Amps
  18866, // C2 = 1.9 Amps
  18866, // C3 = 1.9 Amps
  19859, // C4 = 2.0 Amps
  22838, // C5 = 2.3 Amps
  24824, // C6 = 2.5 Amps
  25817, // C7 = 2.6 Amps
  26810, // C8 = 2.7 Amps
  26810, // C9 = 2.7 Amps
};

const float ent_wand_speed[6] =
{ //rps    //rpm
  0.00,    //0
  0.25,    //15
  0.55,    //33
  0.82,    //49
  1.08,    //65
  1.48     //89
};

uint16_t over_energy_current_limit;


static const uint8_t LW_ABLATE_MAX = sizeof(legacy_wand_ablate_dac_values)/
                                      sizeof(legacy_wand_ablate_dac_values[0]);


static const uint8_t LW_COAG_MAX = sizeof(legacy_wand_coag_dac_values)/
                                    sizeof(legacy_wand_coag_dac_values[0]);

// Flag which tells if the wand connected is a new wand or not. 0 is old wand
// 1 is new wand
static uint8_t new_wand_flag = 1;
STATIC_ARTC uint8_t wand_type_value = 0;
static float cal_factor;
static uint8_t one_sec_counter = 0;
static const uint32_t LW_7_MIN_ACTIVE_TIME_LIMIT = 50*60*7; // this is in 20msec intervals
static const uint32_t LW_40_MIN_ACTIVE_TIME_LIMIT = 50*60*40; // this is in 20msec intervals
static const uint32_t LW_8_HOUR_TIME_LIMIT = 60*60*8; // This is in seconds
uint8_t lw_temp_value;

uint8_t new_fuse_flag = 0;
uint32_t lw_activation_time_limit = 0;
uint32_t lw_overall_time_limit = 0; 
previous_wands previous_wands_database[LW_DB_SZ];
uint8_t previous_wands_database_index = 0;

void legacy_ent_wand_defaults_loader(R2_TYPES entry_number);
uint8_t lw_ent_get_index(R2_TYPES entry_number);
void lw_ent_reflex_setup(StateAction);
bool lw_ent_timer_needed(void);
uint8_t flow_val_apl_ent_bkp;

/***************************************************************************//**
Inits the legacy wand ASDC, DMA, LEDS (for debug purposes only) and pulls the
fuse blow line low.
@param void
@return uint8_t
@outputs
@context
*******************************************************************************/
uint8_t legacy_wand_init (void)
{
  dma_mux_init();
  
  AD7265_Init();
  
  LegacyWand_PWMInit();
  
  /* Diagnostic LED's port control */
  PORTA_PCR24 = PORT_PCR_MUX(1); /* RED */
  PORTA_PCR25 = PORT_PCR_MUX(1); /* YELLOW */
  PORTA_PCR26 = PORT_PCR_MUX(1); /* BLUE */
  
  // Making sure the fuse blow line is low.
  clear_fuse_blow();
  legacy_wand_params_obj.slope = 4.5042;
  legacy_wand_params_obj.intercept = -17;
  // Anything less than 35 Ohms is considered short. 
  // For low voltage measure we are putting out  Z_level which is 35 Vrms.
  // So Irms = 1A
  // 6.6A = 65535, 1 A = 9929
  legacy_wand_params_obj.impedance.z_high = 65535; // opening the limits up for testing
  legacy_wand_params_obj.impedance.z_low = 0; 
  legacy_wand_params_obj.impedance.z_time = 125; // 12.5 msec
  legacy_wand_params_obj.impedance.z_level = 120; // 10 Volts
  return 1;
}

/***************************************************************************//**
Read the legacy wand values every 20 msec. It called the ADC driver to read the
value and then resets to DMA to go read from the external ADC in another 20msec.
@param void
@return uint8_t
@outputs
@context
*******************************************************************************/
uint8_t legacy_wand_read_adc_values (void)
{
  uint8_t PrimeRunning = get_priming_status();
  if ((PrimeRunning == PRIME_OFF) || legacy_wand_params_obj.wand_connected)
  {
    GenTypeDef controller = UNKNOWN_GEN;
    // Step 1: 
    // Read the resistor divider, it must always read 2.5v. If not,
    // go to fault.
    uint16_t rdivider_adc_value = AD7265_read_value(A6);
    
    // if adc values are off, we can use this call factor
    cal_factor = (float)(((float)2048)/rdivider_adc_value);    
    
    // Step 2: 
    // read the R1 resistor ADC value, calibrate it, filter it 
    // and use it to see if a wand is connected, this step is used for fast
    // return from this function when no wand is connected
    uint16_t R1_adc_value = (uint16_t)(AD7265_read_value(B5));
    R1_adc_value =   (uint16_t)(cal_factor*R1_adc_value);
    uint16_t filtered_R1_adc_value;
    uint16_t r1_filter_settled = filter_adc_value_r1(&R1_adc_value,&filtered_R1_adc_value);
    /***************************************************************************/
    // Step 3: Run the R2 ADC values through an averaging filter
    uint16_t filtered_R2_adc_value;
    uint16_t r2_filter_settled;
    uint16_t R2_adc_value = (uint16_t)(cal_factor*AD7265_read_value(B6));
    r2_filter_settled = filter_adc_value_r2(&R2_adc_value, &filtered_R2_adc_value); 
    
    // Step 4: Check to see if filter values are settled
    if (r1_filter_settled && r2_filter_settled)
    {
      legacy_wand_params_obj.R1 = filtered_R1_adc_value;
      legacy_wand_params_obj.R2  = filtered_R2_adc_value;
      
      /*
      @ENT
      If R1 is 1650 then we know we have an 8to18 pin adaptor connected
      */
      if ( (filtered_R1_adc_value >= R1650_MIN_DAC) &&
          (filtered_R1_adc_value <= R1650_MAX_DAC) )
      {
        controller = ENT;
        
        //high value in R2 indicate no wand connected
        if(filtered_R2_adc_value > R9090_MAX_DAC)
        {
          if (legacy_wand_params_obj.wand_connected == 1)
          {
            lw_disconnect();
          }
          /* Poll AD7265 by reseting DMA ERQ's for SPI0 */
          DMA_SERQ |= DMA_SERQ_SERQ(4);
          DMA_SERQ |= DMA_SERQ_SERQ(5);
          return 0;
        }
        //      else
        //      {
        //        legacy_wand_params_obj.wand_connected = 1;
        //        put_event(&evt_tbl,LW_INCOMPATIBLE);
        //        DMA_SERQ |= DMA_SERQ_SERQ(4);
        //        DMA_SERQ |= DMA_SERQ_SERQ(5);
        //        return 0;
        //      }
      }
      else if ( (filtered_R1_adc_value >= R2400_MIN_DAC) &&
               (filtered_R1_adc_value <= R2400_MAX_DAC) )
      {
        controller = UNKNOWN_GEN;
        //high value in R2 indicate no wand connected
        if(filtered_R2_adc_value > R9090_MAX_DAC)
        {
          if (legacy_wand_params_obj.wand_connected == 1)
          {
            lw_disconnect();
          }
          /* Poll AD7265 by reseting DMA ERQ's for SPI0 */
          DMA_SERQ |= DMA_SERQ_SERQ(4);
          DMA_SERQ |= DMA_SERQ_SERQ(5);
          return 0;
        }
      }
      else
      {
        /***************************************************************************/ 
        // When the load box is connected with the switch set to "Not attached", 
        // the voltage on R2 side pops up to 3.9v at the adc R2 pin.
        // This is also the case when no wand is attached.
        // 3.9v and above on R2 is nothing attached.
        // This is a design flaw in the load box and this code will bypass the 
        // rest of the control if that case is detected
        if (filtered_R2_adc_value >= 3600 && (new_fuse_flag == 0 || new_fuse_flag >= 10)) 
        {
          if (legacy_wand_params_obj.wand_connected == 1)
          {
            lw_disconnect();
          }
          /* Poll AD7265 by reseting DMA ERQ's for SPI0 */
          DMA_SERQ |= DMA_SERQ_SERQ(4);
          DMA_SERQ |= DMA_SERQ_SERQ(5);
          return 0;
        }
        else if ((filtered_R1_adc_value > R9090_MAX_DAC) &&
                 (filtered_R2_adc_value < 3600))
        {
          
          legacy_wand_params_obj.wand_connected = 1;
          put_event(&evt_tbl,LW_INCOMPATIBLE);
          DMA_SERQ |= DMA_SERQ_SERQ(4);
          DMA_SERQ |= DMA_SERQ_SERQ(5);
          return 0;
        }
      }
      
      // Step 3:
      // Looks like a wand is present, read all the switches, resistors and 
      // thermocouple
      uint16_t ablate_adc_value = (uint16_t)(cal_factor*AD7265_read_value(B1));
      uint16_t mode_adc_value = (uint16_t)(cal_factor*AD7265_read_value(B2));
      uint16_t coag_adc_value = (uint16_t)(cal_factor*AD7265_read_value(B3));
      float temp_adc_value = (cal_factor*(float)AD7265_read_value(A3));
      // Filter has settled
      // Everything else happens here
      /***************************************************************************/
      // Step 4: Call the wand type identifier func with the filtered adc_value
      wand_type_value = legacy_wand_type_recognizer(filtered_R1_adc_value);
      /***************************************************************************/    
      //Step 5: Check if wand type is valid
      if (wand_type_value >= NUM_OF_RES_TABLE_ENTRY-1)
      { // Wand type is not valid, does this mean a  no wand or is a bad R1 value
        // Step 6: Check to see if no wand or bad R1
        if (filtered_R1_adc_value > R9090_MAX_DAC)   
        {   
          lw_disconnect();
        }
        else if (wand_type_value == 255)// Step 6: Check to see if no wand or bad R1
        { // Wand ID error. The R1 value is less than MAX expected.
          // This means R1 value is within total range, but is not falling
          // within the range of given R1 value limits.
          
          // NOTE: legacy_wand_params_obj.wand_connected = 1 means a wand is 
          // physically present in the slot, it does not mean it is validated and 
          // ready to use
          legacy_wand_params_obj.wand_connected = 1;
          put_event(&evt_tbl,LW_ID_ERROR);
        }
        else if ( (wand_type_value == 100) &&
                 /*
                 @ENT
                 make sure 1650 is not valid for sports med
                 */
                 (controller != ENT) )
        {
          // NOTE: legacy_wand_params_obj.wand_connected = 1 means a wand is 
          // physically present in the slot, it does not mean it is validated and 
          // ready to use
          
          legacy_wand_params_obj.wand_connected = 1;
          put_event(&evt_tbl,LW_INCOMPATIBLE);
        }
      }
      // Step 7: We have a valid wand
      else if (wand_type_value > 0 && wand_type_value < (NUM_OF_RES_TABLE_ENTRY-1))
      { // So it is a valid R1. 
        
        //Step 8: See if the wand has already gone through R2 section of the code
        // if it did, just read the buttons and escape out of this section.
        // This check is put in place first, so that the code will return faster
        if (new_wand_flag == 0) 
        { // This wand has already gone through the
          // R2 logic
          // It is the same wand, dont have to do anything else, read the buttons.
          legacy_wand_params_obj.wand_connected = 1;
          
          if (controller != ENT)
          {
            legacy_wand_button_read (ablate_adc_value,
                                     coag_adc_value,
                                     mode_adc_value,
                                     temp_adc_value);
          }
          
          /*
          @ENT
          update reflex timer needs
          no tube - show timer / tube connected - show flow rate
          */
          if(lw_ent_timer_needed() == TRUE)
          {
            uint8_t tube_present = ((current_therapy_settings.pump.sensor_status & DISCHARGE_TUBE_SENSOR_ON) == DISCHARGE_TUBE_SENSOR_ON) ||
              ((current_therapy_settings.pump.sensor_status & SUCTION_TUBE_SENSOR_ON) == SUCTION_TUBE_SENSOR_ON);
            
            if ((current_therapy_settings.treatment.ablate_on == 0) &&
                (current_therapy_settings.treatment.coag_on == 0))
            {
              if(!tube_present)
              {
                if(current_therapy_settings.legacy_wand_levels.topaz_timer != 1)
                {
                  lw_ent_reflex_setup(SET);
                }
              }
              else
              {
                if(current_therapy_settings.legacy_wand_levels.topaz_timer != 0)
                {
                  lw_ent_reflex_setup(RESET);
                }
              }
            }
          }
        }
        else if (filtered_R2_adc_value < R9090_MAX_DAC)
        { // Step 9: The wand has just been plugged in and has not gone throught
          // the R2 case. There are two cases,
          // 1. New fuse or
          // 2. Previously plugged in wand.
          
          // see if it is a new fuse or not
          R2_TYPES wand_valid = legacy_wand_use_limit_check(filtered_R2_adc_value,r2_filter_settled);
          
          if ( (controller == ENT) &&
              ( (wand_valid == WAND_750) ||
               (wand_valid == WAND_909) ||
                 (wand_valid == WAND_1400) ) )
          {
            /*
            @ENT
            determine CII wand
            */
            current_therapy_settings.active_wand.legacy_wand = controller;
            legacy_ent_wand_defaults_loader(wand_valid);
            put_event(&evt_tbl,LW_PLUGGED_IN);
          }
          else if ((wand_type_value == 18) && (wand_valid == NON_909))   //Invalid 8-pin combinations
          {
            legacy_wand_params_obj.wand_connected = 1;
            put_event(&evt_tbl,LW_INCOMPATIBLE);
          }
          else if ((wand_type_value == 18) && (filtered_R2_adc_value < R5_MAX_DAC))   //Satisfy Muls(R2 ~ 0 ohms) condition
          {
            legacy_wand_params_obj.wand_connected = 1;
            put_event(&evt_tbl,LW_EXPIRED);
          }
          // Step 10: Check to see if this a new fuse
          else if (((wand_valid == NEW_FUSE) ||
                    (new_fuse_flag >= 1)))
          {
            // New fuse flag is used to notify the program that wand is still new
            // fuse , even after its fuse is blown to accomodate for fuse wait times
            // Step 10a: If it is a new fuse, blow the fuse
            if (new_fuse_flag == 1)
            { 
              // blow the fuse
              blow_fuse();
            }
            // Step 10b: Wait for some time after the fuse is blown and clear the
            // fuse blow line. The fuse gets blown in about 200 useconds. So waiting
            // for 20 msec is more than enough. Waiting for a long time introduces 
            // the problem of the wand being unplugged before the fuse transistor
            // is turned off.
            else if (new_fuse_flag == 2)
            { // The fuse was blown and enough time has passed.
              // Reset the fuse blow line
              clear_fuse_blow();
            }
            // Step 10c: Wait for some time after clearing the line to read the
            // resistor value
            // waiting for 200 msec to line to settle
            else if (new_fuse_flag > 12) 
            { // wait for some more time after clearing the fuse blow line
              // new_fuse_flag
              // Step 7: Since this is a new wand, do a use limit check
              //R2_TYPES wand_valid = legacy_wand_use_limit_check(filtered_R2_adc_value);
              
              // Step 10c1: See if we have valid R2       
              if ((wand_valid == WAND_909 || wand_valid == WAND_750 || wand_valid == WAND_1400 || wand_valid == NON_909))
              {
                // Find a suitable location to keep track of this new wand
                previous_wands_database_index = LW_DB_SZ;
                
                // Search previous_wand database too see if a wand of the 
                // same type is present. If so, then use that slot.
                for(uint8_t i = 0; i < LW_DB_SZ; i++)
                {
                  if(previous_wands_database[i].R1_wand_type == wand_type_value &&
                     equal_adc_value(previous_wands_database[i].R2_adc_value,filtered_R2_adc_value,50))
                  { // expand the range of R2 value
                    previous_wands_database_index = i;
                    break;
                  }
                }
                // If the search did not find a similar wand in the previous slot,
                // then search for an empty slot
                if (previous_wands_database_index == LW_DB_SZ)
                {
                  // search previous_wand database for an empty slot
                  for(uint8_t i = 0; i < LW_DB_SZ; i++)
                  {
                    
                    if(previous_wands_database[i].R1_wand_type == 0 &&
                       previous_wands_database[i].R2_adc_value == 0)
                    {
                      previous_wands_database_index = i;
                      break;
                    }
                  }
                }
                
                // Implement the FIFO for the previous wands
                // Copy data in A[1] to A[n] to A[0] to A[n-1]
                if (previous_wands_database_index == LW_DB_SZ)
                {
                  for(uint8_t i = 1; i < LW_DB_SZ; i++)
                  { // Note the index for the for loop starts at 1, and not as zero
                    // because the values from A[1] are copied to A[0], there by
                    // deleting A[0];
                    previous_wands_database[i-1].start_time = previous_wands_database[i].start_time;
                    previous_wands_database[i-1].activation_time_sum = previous_wands_database[i].activation_time_sum;
                    previous_wands_database[i-1].R1_wand_type = previous_wands_database[i].R1_wand_type;
                    previous_wands_database[i-1].R2_adc_value = previous_wands_database[i].R2_adc_value;
                  }
                  
                  // Put the new wand in the last location
                  previous_wands_database_index = LW_DB_SZ-1;
                }
                
                previous_wands_database[previous_wands_database_index].R1_wand_type =
                  wand_type_value;
                
                previous_wands_database[previous_wands_database_index].R2_adc_value =
                  filtered_R2_adc_value;
                
                previous_wands_database[previous_wands_database_index].start_time = (*(uint32_t*)0x4003D000);
                
                previous_wands_database[previous_wands_database_index].activation_time_sum = 0;
                
                // Step 10c2: Load the correct R2 dependant values
                lw_use_limit_time_loader (wand_valid, previous_wands_database_index);
                new_fuse_flag = 0;
                
                
              }
              else
              { // Step 10c1 else clause, wait for the filter to settle
                
              }
            }
            new_fuse_flag++;
          }
          // Step 11: It is not a new wand. So is it a valid R2 value     
          else if ((wand_valid == WAND_909 || wand_valid == WAND_750 || wand_valid == WAND_1400 || wand_valid == NON_909))
          {// Step 11a: If it is a valid R2 check to see if it present in the
            // previous wand database.
            previous_wands_database_index = LW_DB_SZ;
            
            uint32_t stop_time = (*(uint32_t*)0x4003D000);
            
            // search previous_wand database for R1 and R2 adc values
            for(uint8_t i = 0; i < LW_DB_SZ; i++)
            {
              if (stop_time - previous_wands_database[i].start_time > LW_8_HOUR_TIME_LIMIT)
              { // Check and make sure previous wands are within the 24 hour limit
                previous_wands_database[i].R1_wand_type = 0;
                previous_wands_database[i].R2_adc_value = 0;
                previous_wands_database[i].start_time = 0;
                previous_wands_database[i].activation_time_sum = 0;
              }
              
              if(previous_wands_database[i].R1_wand_type == wand_type_value &&
                 equal_adc_value(previous_wands_database[i].R2_adc_value,filtered_R2_adc_value,50))
              { // expand the range of R2 value
                previous_wands_database_index = i;
                break;
              }
            }
            // Call the use limit time loader for the particular previous_wand
            // database_index
            lw_use_limit_time_loader (wand_valid,previous_wands_database_index);
            
          }
          else
          { // R2 filter not settled 
            // This code is to deal with the load box voltage drop.
            // When using the load box, if a R1 resistor is turned ON even
            // without the "WAND ATTACHED" switch set to connected, there is a
            // voltage drop on the R1 Channel. This drop brings the voltage 
            // in the same range of 5600 Ohm R1 resistor, the control gets to
            // part (R1 = 5600, R2 = fuse (not 909 or anything revognized),
            // the code snippet below prevents a error being thrown when the 
            // load box is used for testing.
            
          }
          
        }
        else
        {
          new_wand_flag = 1;
        }
      }
    }
    else
    { // Filter settle for R1
      // just wait till filter has settled
    }
    
    
    /* Poll AD7265 by reseting DMA ERQ's for SPI0 */
    DMA_SERQ |= DMA_SERQ_SERQ(4);
    DMA_SERQ |= DMA_SERQ_SERQ(5);
    
    return current_therapy_settings.active_wand.lw_active ;
  }
}

/***************************************************************************//**
This function is called to read the button of the legacy wand and update current
therapy settings.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint8_t legacy_wand_button_read (uint16_t ablate_adc_value,
                                 uint16_t coag_adc_value,
                                 uint16_t mode_adc_value,
                                 float temp_adc_value)
{
  if(legacy_wand_params_obj.wand_connected == 1)
  {
    uint32_t stop_time = (*(uint32_t*)0x4003D000);
    if (previous_wands_database_index < LW_DB_SZ)
    {
      if (stop_time - previous_wands_database[previous_wands_database_index].start_time >
          lw_overall_time_limit)
      {
        // Display screen saying WAND expired
        put_event(&evt_tbl,LW_USE_LIMIT_ERROR);
        previous_wands_database[previous_wands_database_index].R1_wand_type = 0;
        previous_wands_database[previous_wands_database_index].R2_adc_value = 0;
        previous_wands_database[previous_wands_database_index].start_time = 0;
        previous_wands_database[previous_wands_database_index].activation_time_sum = 0;
      }
      
      if (legacy_wand_params_obj.therapy_on)
      {
        previous_wands_database[previous_wands_database_index].activation_time_sum++;
      }
      
      if (previous_wands_database[previous_wands_database_index].activation_time_sum
          > lw_activation_time_limit)
      {
        // Display screen saying WAND expired
        put_event(&evt_tbl,LW_USE_LIMIT_ERROR);
        previous_wands_database[previous_wands_database_index].R1_wand_type = 0;
        previous_wands_database[previous_wands_database_index].R2_adc_value = 0;
        previous_wands_database[previous_wands_database_index].start_time = 0;
        previous_wands_database[previous_wands_database_index].activation_time_sum = 0;
      }
    }
    
    if (current_therapy_settings.treatment.finger_switch_en)
    {
      /* Check if ablate key is pressed */
      uint8_t ablate = 0;
      if (ablate_adc_value < 850)
      {
        ablate = 1;
        
      }
      else
      {
        ablate = 0;
        
      }
      /* Check to make sure the key press is unique, i.e it was released and
      pressed again */
      if (ablate != legacy_wand_params_obj.ablate_flag)
      {
        if ((ablate == 1) &&
            (current_therapy_settings.legacy_wand_levels.keys_active == 1))
        {
          current_therapy_settings.legacy_wand_levels.cut_key = 1;
          put_event(&evt_tbl,LW_ABLATE_ON_EVENT);
          
        }
        else
        {
          current_therapy_settings.legacy_wand_levels.cut_key = 0;
          put_event(&evt_tbl,LW_ABLATE_OFF_EVENT);
          
        }
      }
      legacy_wand_params_obj.ablate_flag = ablate;
      
      /* Check if Coag key is pressed */
      uint8_t coag = 0;
      if (coag_adc_value < 850)
      {
        coag = 1;
        
      }
      else
      {
        coag = 0;
        
      }
      
      if (coag != legacy_wand_params_obj.coag_flag)
      {
        if ((coag == 1) &&
            (current_therapy_settings.legacy_wand_levels.keys_active == 1))
        {
          
          current_therapy_settings.legacy_wand_levels.coag_key = 1;
          put_event(&evt_tbl,LW_COAG_ON_EVENT);
          
        }
        else
        {
          current_therapy_settings.legacy_wand_levels.coag_key = 0;
          put_event(&evt_tbl,LW_COAG_OFF_EVENT);
        }
      }
      legacy_wand_params_obj.coag_flag = coag;

      
      
      /* Check to see if mode key pressed */
      uint8_t mode = 0;    
      if (mode_adc_value < 850)
      {
        mode = 1;
        
      }
      else
      {
        mode = 0;
        
      }
      if (mode != legacy_wand_params_obj.mode_flag)
      {
        if ((mode == 1) &&
            (current_therapy_settings.legacy_wand_levels.keys_active == 1))
        {
          put_event(&evt_tbl,CLICK_SOUND);
          current_therapy_settings.legacy_wand_levels.mode_key = 1;
          put_event(&evt_tbl,LW_MODE_ON_EVENT);
        }
        else
        {
          current_therapy_settings.legacy_wand_levels.mode_key = 0;
          put_event(&evt_tbl,LW_MODE_OFF_EVENT);
        }
      }
      legacy_wand_params_obj.mode_flag = mode; 
    }
      /* ablate/coag keys active (not blocked) for initial plug-in */
      current_therapy_settings.legacy_wand_levels.keys_active = 1;
      // Write the levels to the global data structure.
      current_therapy_settings.legacy_wand_levels.legacy_wand_ablate_level = 
        legacy_wand_params_obj.ablate_level;
      
      current_therapy_settings.legacy_wand_levels.legacy_wand_coag_level = 
        legacy_wand_params_obj.coag_level;
      
      uint8_t temp_ok =  legacy_adc_temp_curve_fit(temp_adc_value, &lw_temp_value);
      if (one_sec_counter++ >= 49) //20msec*50 = 1 sec
      {
         one_sec_counter = 0;
         current_therapy_settings.ambient.legacy_temperature = lw_temp_value;
      }
      
      if (current_therapy_settings.active_wand.lw_active)
      {
        if (temp_ok)
        {
          if (legacy_wand_params_obj.ambient_capable)
          {
            if (lw_temp_value < 150)
            {
              legacy_wand_common_params.ambient_sensor_flag = 0;
            }
            else
            {
              put_event(&evt_tbl,LW_NO_AMBIENT_SENSOR);
            }
          
          // The gui can only display upto MAX_DISPLAY_TEMP. 
          // So anything over MAX_DISPLAY_TEMP, peg at MAX_DISPLAY_TEMP
          lw_temp_value = lw_temp_value>MAX_DISPLAY_TEMP?MAX_DISPLAY_TEMP:lw_temp_value;
          
          
          if (one_sec_counter == 0) //20msec*50 = 1 sec
          {
             if((current_therapy_settings.controller != ENT) &&
                (current_therapy_settings.gui.screenID == LEGACY_6_8_WAND_TREATMENT_SCREEN) )
              {// Update the temp only if the right screen is being displayed)
                current_therapy_settings.ambient.temperature = lw_temp_value;
              }
          }
          
          current_therapy_settings.ambient.thermometer_color = 
            thermometer_color(current_therapy_settings.ambient.temperature);
        }
          current_therapy_settings.ambient.capable = 
            legacy_wand_params_obj.ambient_capable;
        }
        
      }
    
  }
  return 0;
}

/***************************************************************************//**
This function is called to check the validity of the wand. 
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
R2_TYPES legacy_wand_use_limit_check (uint16_t curr_filtered_R2_adc_value,uint16_t r2_filter_settle)
{
  R2_TYPES return_value;
  
  if (r2_filter_settle)
  {
    if (curr_filtered_R2_adc_value <= R5_MAX_DAC)
    {// gets here is fuse is not blown. So blow the fuse.

      return_value = NEW_FUSE;
      // Throw a wait state in here for the value to settle
     
    }
    else if (curr_filtered_R2_adc_value >= (R2_R909_MIN_DAC) &&  
             curr_filtered_R2_adc_value <= (R2_R909_MAX_DAC))
    {// Opening up the limits to read the 909 R2 resistor. They might be 
     // leakage current which might not give the resistor divider a full 5v, so
     // the R2 values might be lower.
      
      return_value = WAND_909;
    }
    /*
      @ENT
      squeeze in the CII wand possibilities (750 & 1400)
    */
    else if (curr_filtered_R2_adc_value >= (R750_MIN_DAC) &&
             curr_filtered_R2_adc_value <= (R750_MAX_DAC))
    {
      return_value = WAND_750;
    }
    else if (curr_filtered_R2_adc_value >= (R1400_MIN_DAC) &&
             curr_filtered_R2_adc_value <= (R1400_MAX_DAC))
    {
      return_value = WAND_1400;
    }
    else
    {
      return_value = NON_909;
    }
    
  }
  return return_value;
}

/***************************************************************************//**
This function is called when a legacy wand is connected and the RF switches to
the legacy wand. All the smart wand parameters in the CurrentTherapySettings
database is cleared.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint8_t reset_therapy_settings ()
{
  memset(&current_therapy_settings,0,sizeof(GEN_SETTINGS));
  return 0;
}

/***************************************************************************//**
This function is used to increment the ablate level when the mode key pressed.
The mode key increments the ablate level and rolls it over when max is reached.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint8_t increment_mode ()
{
  // The whole thing must only work when the legacy wand screen is being 
  // displayed
  
  /* lookup table of just settings type screens */
  const uint8_t settings_screens[MAX_NUM_SCREENS] =
  {
    /* 0-6   */  0,0,0,0,0,0,0,
    /* 7     */  DELETE_INDIVIDUAL_USER_PROFILE_CHANCE_TO_BACK_OUT_SCREEN,
    /* 8     */  SET_LEVEL_SCREEN,
    /* 9     */  0,
    /* 10    */  MAIN_SETTINGS_SCREEN,
    /* 11    */  PROFILE_SAVE_SCREEN,
    /* 12    */  USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN,
    /* 13    */  USER_PROFILES_SCREEN,
    /* 14    */  ENTER_NEW_USER_PROFILE_NAME_SCREEN,
    /* 15-17 */  0,0,0,
    /* 18    */  SWITCHING_SETTINGS_SCREEN,
    /* 19    */  LANGUAGES_SCREEN,
    /* 20-41 */  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 42    */  LEGACY_6_8_WAND_TREATMENT_SCREEN,
    /* 43-50 */  0,0,0,0,0,0,0,0,
    /* 51    */  LEGACY_ENT_WAND_TREATMENT_SCREEN,
    /* 52    */  0,
  };
  
  if (settings_screens[current_therapy_settings.gui.screenID] == current_therapy_settings.gui.screenID)
  {
    current_therapy_settings.gui.depth_idx = 0;
    if(current_therapy_settings.controller == ENT)
    {
      current_therapy_settings.gui.screenID = LEGACY_ENT_WAND_TREATMENT_SCREEN;
    }
    else
    {
      current_therapy_settings.gui.screenID = LEGACY_6_8_WAND_TREATMENT_SCREEN;
    }
  }

  if ( (current_therapy_settings.gui.screenID == LEGACY_6_8_WAND_TREATMENT_SCREEN) ||
       (current_therapy_settings.gui.screenID == LEGACY_ENT_WAND_TREATMENT_SCREEN) )
  {
    uint8_t level = legacy_wand_params_obj.ablate_level;
    level++;
    if (level > legacy_wand_params_obj.max_ablate)
    { // rolling over
      if (legacy_wand_params_obj.min_ablate == 0)
      {// but dont roll over to zero, goto 1
        /*
          @ENT
          disable ablate
        */
        if(current_therapy_settings.controller != ENT)
        {
          level = legacy_wand_params_obj.min_ablate+1;
        }
        else
        {
          level = 0;
        }
      }
      else
      {
        level = legacy_wand_params_obj.min_ablate;
      }
    }
    
    legacy_wand_params_obj.ablate_level = level;
    current_therapy_settings.legacy_wand_levels.legacy_wand_ablate_level = level;
  }
  return 0;
}

/***************************************************************************//**
This function is used to increment the coag level. If the level is incremented
below the max level, it rolls over to min.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint8_t legacy_coag_increment ()
{
   uint8_t level = legacy_wand_params_obj.coag_level;
   level++;
   if (level > legacy_wand_params_obj.max_coag)
   {
      // No rolling over
      level = legacy_wand_params_obj.max_coag;
   }
   legacy_wand_params_obj.coag_level = level;
   current_therapy_settings.legacy_wand_levels.legacy_wand_coag_level = level;
   return 0;
}

/***************************************************************************//**
This function is used to decrement the coag level. If the level is decremented
below the min, it stays at min.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint8_t legacy_coag_decrement ()
{
   uint8_t level = legacy_wand_params_obj.coag_level;
   if (level <= legacy_wand_params_obj.min_coag)
   {
     level = legacy_wand_params_obj.min_coag;
   }
   else
   {
     level--;
   }
   legacy_wand_params_obj.coag_level = level;
   current_therapy_settings.legacy_wand_levels.legacy_wand_coag_level = level;
   return 0;
}

/***************************************************************************//**
This function is used to increment the ablate level. If the level is incremented
below the max level, it rolls over to min.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/ 
uint8_t legacy_ablate_increment ()
{
   uint8_t level = legacy_wand_params_obj.ablate_level;
   level++;
   if (level > legacy_wand_params_obj.max_ablate)
   {
      // No rolling over
      level = legacy_wand_params_obj.max_ablate;
   }
   legacy_wand_params_obj.ablate_level = level;
   current_therapy_settings.legacy_wand_levels.legacy_wand_ablate_level = level;
   return 0;
}

/***************************************************************************//**
This function is used to decrement the ablate level. If the level is decremented
below the min, it stays at min.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/  
uint8_t legacy_ablate_decrement ()
{
   uint8_t level = legacy_wand_params_obj.ablate_level;
   if (level <= legacy_wand_params_obj.min_ablate)
   {
     level = legacy_wand_params_obj.min_ablate;
   }
   else
   {
     level--;
   }
   legacy_wand_params_obj.ablate_level = level;
   current_therapy_settings.legacy_wand_levels.legacy_wand_ablate_level = level;
   return 0;
}

/***************************************************************************//**
This function converts adc_value into a temperature value in degree celcius.
The wands have a t type thermocouple whereas the board has a K type instrument
amplifier, so " adc_value = (float)((temp_adc_value*5000/4096)+0.98)/122.4;" backs
out the non linearity in the instrumentation amplifier and directly gives us the
junction voltage, which is then used along with the inverse temp coefficients
for a t type theromocouple to determine the temp.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint8_t legacy_adc_temp_curve_fit (float input_value, uint8_t *temp_out)
{
  double temp_adc_value = (double)input_value;
  double temp;
  uint8_t filtered_temp;
  double adc_value = ((temp_adc_value*(double)1.221)+0.98)/122.4;
  double conv_coeffs[4] = {25.92800,-0.7602961,0.04637791,-0.002165394};
  temp = conv_coeffs[0]*adc_value + 
         conv_coeffs[1]*adc_value*adc_value +
         conv_coeffs[2]*adc_value*adc_value*adc_value +
         conv_coeffs[3]*adc_value*adc_value*adc_value*adc_value;
  
  // After some emphirical analysis, it was found out that the adc was 
  // reading 2C higher than what was expected with a calibrated meter.
  // So subtracting 2 degrees to bring the temp to the norm.
  if (temp > 2.0)
  {
    temp = temp - 2.0;
  }
  uint8_t rounded_temp;
  if (temp < 0)
  {// special case for dealing with negative numbers. This occurs when there 
   // is no sensor detected. In that case, the line is pulled high near 4095 ,
   // which when subsituted into the above equation yields negative numbers 
    rounded_temp = 0xFF;
  }
  else
  {
  // throwing away the fractional part. Essentially rounding down.
  rounded_temp = (uint8_t)temp;
  }
  uint8_t temp_settle = filter_temp(&rounded_temp , &filtered_temp);
  *temp_out = filtered_temp;

  return temp_settle;
}

/***************************************************************************//**
This function recognizes if the given R1_wand_type corresponds to a known wand. 
@param void
@return 0
@outputs 
returns a table index for a valid wand
returns 100 for incompatible wands(wands present in the table but not supported)
returns 255 for wands not the table
@context: 
*******************************************************************************/  
uint8_t
legacy_wand_type_recognizer (uint16_t r_value)
{
  uint8_t entry_number = 255;  
  for (uint8_t index = 1; index < NUM_OF_RES_TABLE_ENTRY; index++)
  { // The index goes from 1 because the first value is 5ohms which does not
    // occurs in the R1 slot.
     if ((r_value > resTable[index][MIN_ENTRY]) && 
         (r_value < resTable[index][MAX_ENTRY]))
     {
        entry_number = resTable[index][PROBE_ENTRY];
     }
  }
  
  if (entry_number == 8  || //R1 = 147
      entry_number == 9  || //R1 = 169
      entry_number == 10 || //R1 = 243  
      entry_number == 13 || //R1 = 365
      entry_number == 17 || //R1 = 1400
      /*
        @ENT
        R1 = 1650Ohms is the 8to18 pin adapator

      entry_number == 18 || //R1 = 1650
      */
      entry_number == 19 || //R1 = 1960
      //entry_number == 20 || //R1 = 2400
      entry_number == 22 )  //R1 = 3920
  {
    // Incompatible Wand
    entry_number = 100; // Returns code 100 for incompatible wands
  }
  
  return entry_number;
}
/***************************************************************************//**
@ENT
This function sets up the max ablate, min ablate, max coag, min coag, default
ablate, default coag and flow values given the type of wand. The type of wand is
determined using the R2 value.
@param void
@return 0
@outputs
@context:
*******************************************************************************/
void legacy_ent_wand_defaults_loader(R2_TYPES ent_entry_number)
{
  uint8_t r2_array_index = lw_ent_get_index(ent_entry_number);
  if(r2_array_index >= sizeof(ent_wand_type_array))
  {
    /* shouldn't get here */
    put_event(&evt_tbl,LW_ID_ERROR);
    return;
  }

  //setup ablate
  legacy_wand_params_obj.max_ablate =
    ent_wand_type_array[r2_array_index].max_ablate;
  legacy_wand_params_obj.min_ablate = 0;
  legacy_wand_params_obj.ablate_level =
    ent_wand_type_array[r2_array_index].default_ablate;
  current_therapy_settings.legacy_wand_levels.legacy_wand_ablate_level =
      legacy_wand_params_obj.ablate_level;

  //setup coag
  legacy_wand_params_obj.max_coag = ent_wand_type_array[r2_array_index].max_coag;
  legacy_wand_params_obj.min_coag = 0;
  legacy_wand_params_obj.coag_level =
    ent_wand_type_array[r2_array_index].default_coag;
  current_therapy_settings.legacy_wand_levels.legacy_wand_coag_level =
      legacy_wand_params_obj.coag_level;

  legacy_wand_params_obj.coag_enable = 1; //coag always enabled for ENT
  current_therapy_settings.treatment.flow_auto = FALSE;
  if(((smart_wand_params.mfg.indication == SMART_WAND_MFG_INDICATION_MANTA) && current_therapy_settings.wand.valid) || 
    ((current_therapy_settings.previous_wand_indication == SMART_WAND_MFG_INDICATION_MANTA) && (!current_therapy_settings.wand.valid)))
  {
    flow_val_apl_ent_bkp = current_therapy_settings.treatment.flow_rate;
  }
  current_therapy_settings.treatment.flow_rate = DEFAULT_FLOW_RATE;

  if(ent_entry_number == WAND_750)
  {
    lw_ent_reflex_setup(SET);
  }
  else
  {
    lw_ent_reflex_setup(RESET);
  }

  new_wand_flag = 0;
  legacy_wand_params_obj.wand_connected = 1;
  current_therapy_settings.user_data.settings.active_idx = 0;
  current_therapy_settings.active_wand.legacy_wand = ENT;
}

/***************************************************************************//**
@ENT
returns the index of ent_wand_type_array based on given R2 value
@param void
@return 0
@outputs
@context:
*******************************************************************************/
uint8_t lw_ent_get_index(R2_TYPES entry_number)
{
  uint8_t EntIndex = 0xFF; //init to invalid value

  // 0 R2 = 750
  // 1 R2 = 909
  // 2 R2 = 1400
  if(entry_number == WAND_750)
  {
    EntIndex = 0;
  }
  else if(entry_number == WAND_909)
  {
    EntIndex = 1;
  }
  else if(entry_number == WAND_1400)
  {
    EntIndex = 2;
  }
  else
  {
    //keep invalid value
  }

  return(EntIndex);

}

/***************************************************************************//**
@ENT
sets up the reflex timer values
@param void
@return 0
@outputs
@context:
*******************************************************************************/
void lw_ent_reflex_setup(StateAction Setup)
{
  u8 r2_array_index = lw_ent_get_index(WAND_750);

  if(Setup != RESET)
  {
    //expect timer value to be set in ENT R2 array
    current_therapy_settings.legacy_wand_levels.topaz_timer = 1;
    legacy_wand_params_obj.timer.maximum =
        ent_wand_type_array[r2_array_index].timer;
    legacy_wand_params_obj.timer.current = legacy_wand_params_obj.timer.maximum;
    legacy_wand_params_obj.timer.audio_muted = FALSE;
  }
  else
  {
    current_therapy_settings.legacy_wand_levels.topaz_timer = 0;
    legacy_wand_params_obj.timer.maximum = 0;
    legacy_wand_params_obj.timer.current = 0;
    pump_set_speed(lw_get_pump_speed(0));
  }
}

/***************************************************************************//**
This function sets up the max ablate, min ablate, max coag, min coag, default
ablate and default coag values given the type of wand. The type of wand is
determined using the R1 value.
@param void
@return 0
@outputs
@context:
*******************************************************************************/
void legacy_wand_defaults_loader(uint8_t entry_number)
{
  if(entry_number < 1  || entry_number > 24)
  {
    // wand ID error
    put_event(&evt_tbl,LW_ID_ERROR);
    return;
  }
  
  if (wand_type_array[entry_number].used == 1)
  {
    
    legacy_wand_params_obj.min_ablate = 0;
    legacy_wand_params_obj.max_coag = 2;
    legacy_wand_params_obj.min_coag = 0;
    legacy_wand_params_obj.coag_level = 1;
    if (wand_type_array[entry_number].max_ablate < LW_ABLATE_MAX)
    {
      legacy_wand_params_obj.max_ablate = 
        wand_type_array[entry_number].max_ablate;
    }
    else
    {
      legacy_wand_params_obj.max_ablate = LW_ABLATE_MAX-1;
    }
    
    if (wand_type_array[entry_number].max_coag < LW_COAG_MAX)
    {
      legacy_wand_params_obj.max_coag = wand_type_array[entry_number].max_coag;
    }
    else
    {
      legacy_wand_params_obj.max_coag = LW_COAG_MAX-1;
    }
    if (wand_type_array[entry_number].default_coag < LW_COAG_MAX)
    {
      legacy_wand_params_obj.coag_level = 
        wand_type_array[entry_number].default_coag;
    }
    else
    {
      legacy_wand_params_obj.coag_level = 0;
    }
    
    if (wand_type_array[entry_number].default_ablate < LW_ABLATE_MAX)
    {
      legacy_wand_params_obj.ablate_level = 
        wand_type_array[entry_number].default_ablate;
    }
    else
    {
      legacy_wand_params_obj.ablate_level=0;
    }
    
    if(wand_type_array[entry_number].coag_adj == NO_COAG_ADJ)
    {
      legacy_wand_params_obj.coag_level = 0;
      legacy_wand_params_obj.max_coag = 0;
      legacy_wand_params_obj.min_coag = 0;
    }
    
    if(wand_type_array[entry_number].timer != 0)
    {
      current_therapy_settings.legacy_wand_levels.topaz_timer = 1;
      legacy_wand_params_obj.timer.maximum =
        wand_type_array[entry_number].timer;
      legacy_wand_params_obj.timer.current = 0;
    }
    
    legacy_wand_params_obj.coag_enable = wand_type_array[wand_type_value].coag_adj;

    current_therapy_settings.active_wand.legacy_wand = SPORTS_MED;

  }
  else
  {
    // These wands are not used, therefore not compatible
    
  }
  
  current_therapy_settings.legacy_wand_levels.legacy_wand_ablate_level = 
      legacy_wand_params_obj.ablate_level;
    
  current_therapy_settings.legacy_wand_levels.legacy_wand_coag_level = 
      legacy_wand_params_obj.coag_level;
  
#ifdef LW_DEBUG
  // For debug purposes only
  legacy_wand_params_obj.max_ablate = 10;
  legacy_wand_params_obj.min_ablate = 0;
  legacy_wand_params_obj.max_coag = 2;
  legacy_wand_params_obj.min_coag = 0;
  
#endif
  
}

/***************************************************************************//**
Filters the given input through an averaging filter. Used for legacy wand R1 
value. The R1 value varies during powerup and during connecting the wand. This
filter smoothes out the noise and gives a steady value. Filters the raw ADC
value for ID resistor R1.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint16_t 
filter_adc_value_r1 (uint16_t * value, uint16_t * ret_value)
{
  static uint8_t index =0 ;
  const uint8_t buffer_size = 64;
  buffer_r1[index++%buffer_size] = *value;
  
  uint16_t filt_settle;
  // Create 4 buffers
  uint16_t b1[16],b2[16],b3[16],b4[16];
  
  // copy the main buffer into 4 different buffers
  memcpy(&b1[0], &buffer_r1[0],32);
  memcpy(&b2[0], &buffer_r1[16],32);
  memcpy(&b3[0], &buffer_r1[32],32);
  memcpy(&b4[0], &buffer_r1[48],32);
  
  //Sort the buffers
  qsort(&b1[0],16, sizeof(b1[0]), compare_func);
  qsort(&b2[0],16, sizeof(b2[0]), compare_func);
  qsort(&b3[0],16, sizeof(b3[0]), compare_func);
  qsort(&b4[0],16, sizeof(b4[0]), compare_func);
  
  // Just average the middle 4 values
  uint16_t ave1 = (b1[4]+b1[5]+b1[6]+b1[7]+b1[8]+b1[9]+b1[10]+b1[11])/8;
  uint16_t ave2 = (b2[4]+b2[5]+b2[6]+b2[7]+b2[8]+b2[9]+b2[10]+b2[11])/8;
  uint16_t ave3 = (b3[4]+b3[5]+b3[6]+b3[7]+b3[8]+b3[9]+b3[10]+b3[11])/8;
  uint16_t ave4 = (b4[4]+b4[5]+b4[6]+b4[7]+b4[8]+b4[9]+b4[10]+b4[11])/8;

  if (abs(ave1 - ave2) <= NOISE_BAND &&
      abs(ave1 - ave3) <= NOISE_BAND &&
      abs(ave1 - ave4) <= NOISE_BAND )
  {
    filt_settle = 1;
    *ret_value = (ave1+ave2+ave3+ave4)/4;
  }
  else
  {
    filt_settle = 0;
    *ret_value =  4095;
  }
  return filt_settle;
}

/***************************************************************************//**
compare function used by qsort. Since qsort is stdlib, it uses int, instead of
int8_t.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
int compare_func (const void * val1, const void* val2)
{
  return (*(int*)val1 - * (int *)val2);
}

/***************************************************************************//**
Filters the given input through an averaging filter. Used for legacy wand R1 
value. The R1 value varies during powerup and during connecting the wand. This
filter smoothes out the noise and gives a steady value.Filters the raw ADC
value for ID resistor R2.
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint16_t 
filter_adc_value_r2 (uint16_t * value, uint16_t * ret_value)
{
  static uint8_t index =0 ;
  const uint8_t buffer_size = 64;
  buffer_r2[index++%buffer_size] = *value;
  
  uint16_t filt_settle;
  
  // Create 4 buffers
  uint16_t b1[16],b2[16],b3[16],b4[16];
  
  // copy the main buffer into 4 different buffers
  memcpy(&b1[0], &buffer_r2[0],32);
  memcpy(&b2[0], &buffer_r2[16],32);
  memcpy(&b3[0], &buffer_r2[32],32);
  memcpy(&b4[0], &buffer_r2[48],32);
  
  //Sort the buffers
  qsort(&b1[0],16, sizeof(b1[0]), compare_func);
  qsort(&b2[0],16, sizeof(b2[0]), compare_func);
  qsort(&b3[0],16, sizeof(b3[0]), compare_func);
  qsort(&b4[0],16, sizeof(b4[0]), compare_func);
  
  // Just average the middle 4 values
  uint16_t ave1 = (b1[4]+b1[5]+b1[6]+b1[7]+b1[8]+b1[9]+b1[10]+b1[11])/8;
  uint16_t ave2 = (b2[4]+b2[5]+b2[6]+b2[7]+b2[8]+b2[9]+b2[10]+b2[11])/8;
  uint16_t ave3 = (b3[4]+b3[5]+b3[6]+b3[7]+b3[8]+b3[9]+b3[10]+b3[11])/8;
  uint16_t ave4 = (b4[4]+b4[5]+b4[6]+b4[7]+b4[8]+b4[9]+b4[10]+b4[11])/8;

  if (abs(ave1 - ave2) <= NOISE_BAND &&
      abs(ave1 - ave3) <= NOISE_BAND &&
      abs(ave1 - ave4) <= NOISE_BAND )
  {
    filt_settle = 1;
    *ret_value = (ave1+ave2+ave3+ave4)/4;
  }
  else
  {
    filt_settle = 0;
    *ret_value =  4095;
  }
  return filt_settle;
}
/***************************************************************************//**
Filters the given input through an averaging filter. Used for legacy wand temp
@todo Need more description
@param void
@return 0
@outputs
@context: 
*******************************************************************************/
uint8_t 
filter_temp (uint8_t* value, uint8_t* ret_value)
{
  static uint8_t index =0 ;
  static uint32_t sum;
  uint8_t max = 0;
  uint8_t min = 255;
  uint8_t filt_settle;
  
  temp_buffer[index++%TEMP_FILTER_BUFF_LENGTH] = *value;
  sum = 0;
  for (uint8_t i =0; i <TEMP_FILTER_BUFF_LENGTH; i++)
  { 
    sum+=temp_buffer[i];
    if (max < temp_buffer[i])
    {
      max = temp_buffer[i];
    }
    if (min > temp_buffer[i])
    {
      min = temp_buffer[i];
    }
    
  }
  sum /= TEMP_FILTER_BUFF_LENGTH;

  *ret_value = sum;
  
  if (abs(sum-max) < NOISE_BAND_TEMP &&
       abs(sum-min) < NOISE_BAND_TEMP)
  {
    filt_settle = 1;
  }
  else
  {
    filt_settle = 0;
  }
  
  return filt_settle;
  
  
}

/***************************************************************************//**
Checks to see if ADC values are close enough to each other
@param adc_value1, adc_value2
@return 0 if values are different than const defined in function, else returns 1
if the values are close enough
@outputs
@context: It has a static const which defines how close the adc values must be
to be called equal
*******************************************************************************/
uint8_t equal_adc_value(uint16_t adc_value1, uint16_t adc_value2, 
                        uint16_t EQUAL_RANGE)
{
  uint8_t return_value = 0;
  if (adc_value1>adc_value2)
  {
    if(adc_value1-adc_value2 <= EQUAL_RANGE)
    {
      return_value = 1;
    }
    else
    {
      return_value = 0;
    }
  }
  else if (adc_value2>adc_value1)
  {
    if(adc_value2-adc_value1 <= EQUAL_RANGE)
    {
      return_value = 1;
    }
    else
    {
      return_value = 0;
    }
  }
  else
  {
    return_value = 1;
  }
  return return_value;
}

/***************************************************************************//**
Transitions the state machine to FAULT state
@none
@return 1
@outputs
@context: This function is called when the ADC reads values other than the
reference value. This means that there is some fault with the circuit.
*******************************************************************************/
uint8_t legacy_wand_adc_fault (void)
{
  current_therapy_settings.notify.status |= ADC_BOUNDARY_FAULT_MASK;
  return 1;
}

/***************************************************************************//**
Transitions the state machine from LW_UI_STATE to IDLE_STATE
@none
@return 1
@outputs
@context: This function is called when the wand is unplugged when the SM is in
LW_UI_STATE
*******************************************************************************/
uint8_t legacy_wand_unplugged (void)
{
  switch_to_ww_wand();
  return 1;
}

/***************************************************************************//**
Transitions the state machine from other states to LW_UI_STATE
@none
@return 1
@outputs
@context: This function is called during the entry into LW_UI_STATE
*******************************************************************************/
uint8_t legacy_wand_UI_state_entry (void)
{
  switch_to_legacy_wand();
  /* as long as gis isn't actively updating, you are permitted to cancel */
  if (current_therapy_settings.sw.update_progress == 0)
  {
    put_event(&evt_tbl,CANCEL_SW_UPDATE);   
  }
  return 1;
}

/***************************************************************************//**
This function is used to switch to legacy wand states
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t switch_to_legacy_wand(void)
{
  current_therapy_settings.active_wand.lw_active = 1;
  current_therapy_settings.ambient.thermometer_color = 
            thermometer_color(current_therapy_settings.ambient.temperature);
  
  current_therapy_settings.ambient.capable = legacy_wand_params_obj.ambient_capable;
  current_therapy_settings.ambient.enable = (current_therapy_settings.ambient.capable &
    current_therapy_settings.ambient.enable_user_pref);
  
  
  if (legacy_wand_common_params.ambient_sensor_flag)
  {
    current_therapy_settings.notify.status |= 
      AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
    current_therapy_settings.notify.legacy_wand_no_ambient = 1;
  }
  else
  {
    current_therapy_settings.notify.status &= 
      ~AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
    current_therapy_settings.notify.legacy_wand_no_ambient = 0;
  }
  return 1;
}

/***************************************************************************//**
Handles legacy wand switch stuck fault
@none
@return 1
@outputs
@context: During wand plug in the switches should not be in pressed position,
this is a switch stuck fault
*******************************************************************************/
uint8_t lw_wand_switch_stuck_fault(void)
{
  
  current_therapy_settings.notify.status |= SWITCH_STUCK_FAULT_MASK;
  
  current_therapy_settings.notify.legacy_wand_stuck_switch = 1;
  
  return 1;
  
}

/***************************************************************************//**
Handles legacy wand expired error
@none
@return 1
@outputs
@context: If the wand's fuse is already blown,
                         this error is thrown
*******************************************************************************/
uint8_t lw_wand_expired_error(void)
{
  
  current_therapy_settings.notify.status |= WAND_REUSE_NOTIFICATION_MASK;
  current_therapy_settings.notify.legacy_wand_expired = 1;
    
  return 1;

}

/***************************************************************************//**
Handles legacy wand expired error
@none
@return 1
@outputs
@context: If the wand has passed is use time, this error is thrown.
*******************************************************************************/
uint8_t lw_wand_use_limit_error(void)
{
  
  current_therapy_settings.notify.status |= 
    WAND_USE_LIMIT_LIFE_EXPIRED_NOTIFICATION_MASK;
  current_therapy_settings.notify.legacy_wand_use_limit = 1;
    
  return 1;

}

/***************************************************************************//**
Sets the legacy wand ambient sensor flag
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t lw_ambient_sensor (void)
{
  legacy_wand_common_params.ambient_sensor_flag = 1;
  current_therapy_settings.ambient.temperature = 0;
  // Since this function is called only the LW states, we can
  // directly set this field. 
  if (current_therapy_settings.active_wand.lw_active == 1)
  {
   current_therapy_settings.notify.status |= 
      AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
   current_therapy_settings.notify.legacy_wand_no_ambient = 1;
  }
  return 1;
}
 
/***************************************************************************//**
Sets the legacy wand ambient sensor flag
@none
@return 1
@outputs
@context: 
*******************************************************************************/
void  lw_use_limit_time_loader (R2_TYPES wand_valid, uint8_t index)
{
  if (wand_valid == WAND_909)
  {
    // Special wand. No activation time limit
    lw_overall_time_limit = UINT32_MAX;
    lw_activation_time_limit = UINT32_MAX;
    new_wand_flag = 0;
    if (wand_type_array[wand_type_value].tc == 1)
    {
      legacy_wand_params_obj.ambient_capable = 1;
    }
    else
    {
      legacy_wand_params_obj.ambient_capable = 0;
    }
    
    legacy_wand_params_obj.wand_909 = 1;
    // New wand load R1 defaults
    legacy_wand_defaults_loader(wand_type_value);
    legacy_wand_params_obj.wand_connected = 1;
    put_event(&evt_tbl,LW_PLUGGED_IN);
  }
  else if ((wand_valid == NON_909) || wand_valid == WAND_750 || wand_valid == WAND_1400)
  {
    if (index < LW_DB_SZ)
    {
      if (wand_type_value == 16) // Check if R1 = 1210
      {
        lw_activation_time_limit = LW_7_MIN_ACTIVE_TIME_LIMIT;
        lw_overall_time_limit = LW_8_HOUR_TIME_LIMIT;
        new_wand_flag = 0;
        
      }
      else
      {
        
        new_wand_flag = 0;
        // Must be set to 40 mins (in 20msec intervals)
        lw_activation_time_limit = LW_40_MIN_ACTIVE_TIME_LIMIT;
        lw_overall_time_limit = LW_8_HOUR_TIME_LIMIT;
        
      }
      // R1 Resistor values of 324, 649, 1210 and 2400 have special current
      // limits which mirror the 27pin wand current limits. 
      if (wand_type_value == 12 || //324
          wand_type_value == 15 || // 649
          wand_type_value == 16 || // 1210
          wand_type_value == 20 ) //2400
      {
        legacy_wand_params_obj.lower_current_limit = 1;
      }
      else
      {
        legacy_wand_params_obj.lower_current_limit = 0;
      }
            
      if (wand_type_array[wand_type_value].tc == 1)
      {
        legacy_wand_params_obj.ambient_capable = 1;
      }
      else
      {
        legacy_wand_params_obj.ambient_capable = 0;
      }
      // New wand load R1 defaults
      legacy_wand_defaults_loader(wand_type_value);
      legacy_wand_params_obj.wand_connected = 1;
      put_event(&evt_tbl,LW_PLUGGED_IN);
    }
    else
    {
      legacy_wand_params_obj.wand_connected = 1;
      // This wand has expired
      put_event(&evt_tbl,LW_EXPIRED);
    }
  }

}

/***************************************************************************//**
Resets the temp buffer and wand buttons and flags when the wand is disconnected
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t
clear_lw_settings (void)
{
  current_therapy_settings.ambient.temperature = 0;
  
  current_therapy_settings.notify.status &= 
    ~AMBIENT_SENSOR_NOT_DETECTED_NOTIFICATION_MASK;
  current_therapy_settings.notify.legacy_wand_no_ambient = 0;
  
  legacy_wand_common_params.ambient_sensor_flag = 0;
  
  current_therapy_settings.legacy_wand_levels.cut_key = 0;
  current_therapy_settings.legacy_wand_levels.coag_key = 0;
  current_therapy_settings.legacy_wand_levels.mode_key = 0;
  
  for (uint8_t i =0; i <TEMP_FILTER_BUFF_LENGTH; i++)
  { 
    temp_buffer[i] = 0;
  }
  return 1;
}
/***************************************************************************//**
Checks for stuck switch during start up.
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t check_18p_fs_stuck(void)
{
  
  uint16_t ablate_adc_value = (uint16_t)(AD7265_read_value(B1));
  uint16_t mode_adc_value = (uint16_t)(AD7265_read_value(B2));
  uint16_t coag_adc_value = (uint16_t)(AD7265_read_value(B3));
  // Check and make sure the switches are not stuck.
  /* Poll AD7265 by reseting DMA ERQ's for SPI0 */
  DMA_SERQ |= DMA_SERQ_SERQ(4);
  DMA_SERQ |= DMA_SERQ_SERQ(5);
  
  if (ablate_adc_value < 850 ||
      coag_adc_value < 850 ||
      mode_adc_value < 850)
  {// Switch is stuck while plugging in the wand, throw a fault
    
    put_event(&evt_tbl,LW_SWITCH_STUCK);
    return 1;
    
  }
  else
  {
    
    return 0;
    
  }

}

/***************************************************************************//**
Sets the flags correctly after a legacy wand disconnect is detected.
@none
@return none
@outputs
@context: 
*******************************************************************************/
void lw_disconnect (void)
{
  /* wand not plugged in. */
  /* Specific scenario for APL wands; When APL already connected, Leg ENT connected, 
  controller restores APL's FLOW value before ENT is disconnected */ 

  if ((current_therapy_settings.active_wand.legacy_wand == ENT) && 
      (current_therapy_settings.active_wand.smart_wand == ARTHROPLASTY) && 
        (current_therapy_settings.wand.valid == TRUE) && (flow_val_apl_ent_bkp))
    {
      current_therapy_settings.treatment.flow_rate = flow_val_apl_ent_bkp;
      current_therapy_settings.pump.ent_flow = change_flow_manta(current_therapy_settings.setpoint.coag, 
                                                                     current_therapy_settings.treatment.flow_rate);
    }
  /* ablate/coag key not active (blocked) for initial plug-in */
  current_therapy_settings.legacy_wand_levels.keys_active = 0;
  legacy_wand_params_obj.wand_connected = 0;
  current_therapy_settings.active_wand.lw_active = 0;
  legacy_wand_params_obj.time.activation_time_sum = 0;
  legacy_wand_params_obj.time.overall_time_limit_sum = 0;
  current_therapy_settings.legacy_wand_levels.topaz_timer = 0;
  legacy_wand_params_obj.ambient_capable = 0;
  if (current_therapy_settings.wand.valid == TRUE)
  {
    put_event(&evt_tbl,LW_UNPLUGGED_LOAD_SMARTWAND);
  }
  else
  {
    pump_set_speed(lw_get_pump_speed(0));
    put_event(&evt_tbl,LW_UNPLUGGED);
  }
  current_therapy_settings.active_wand.legacy_wand = UNKNOWN_GEN;
  new_wand_flag = 1;
  new_fuse_flag = 0;
  legacy_wand_params_obj.wand_909 = 0;
  legacy_wand_params_obj.lower_current_limit = 0;
  
  // reset these limits as soon as want is plugged out
  lw_activation_time_limit = LW_40_MIN_ACTIVE_TIME_LIMIT; //40 mins
  lw_overall_time_limit = LW_8_HOUR_TIME_LIMIT; //8 hours
  
  /* switch off buck */
  GPIOE_PSOR |= GPIO_PDOR_PDO(BUCK_18PIN_ON); 
  
  // clear temp buffer
  clear_lw_settings();
  // The fuse blow transistor is turned off when the wand is unplugged
  // This catches situations when the wand is plugged in during fuseblow
  clear_fuse_blow();
}

/***************************************************************************//**
@ENT - correlate pump speed with flow setting
legacy_wand_get_pump_speed
@none
@return none
@outputs
@context:
*******************************************************************************/
float lw_get_pump_speed(const u8 Index)
{
  float WandSpeed = 0.00;

  if(Index <= sizeof(ent_wand_speed))
  {
    WandSpeed = ent_wand_speed[Index];
  }
  else
  {
    ASSERT(0);
  }

  return(WandSpeed);

} //legacy_wand_get_pump_speed

/***************************************************************************//**
@ENT - reflex wand connected?
lw_ent_timer_needed
checks if an ENT reflex wand is connected.
@return TRUE | FALSE
@outputs
@context:
*******************************************************************************/
bool lw_ent_timer_needed(void)
{
  bool TimerWand = FALSE;

  if((current_therapy_settings.controller == ENT) &&
     (legacy_wand_params_obj.R2 >= R750_MIN_DAC) &&
     (legacy_wand_params_obj.R2 <= R750_MAX_DAC))
  {
    TimerWand = TRUE;
  }
  else
  {
    TimerWand = FALSE;
  }

  return(TimerWand);
}