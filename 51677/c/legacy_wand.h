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
#ifndef LEGACY_WAND_H
#define LEGACY_WAND_H

#include "cpu.h"
#include "ad7265.h"

#define BUFFER_LENGTH  8
// Sampled at 20 msec intervals, need to average of 1 sec, so BUFF length is 50
#define TEMP_FILTER_BUFF_LENGTH 20 

#define NOISE_BAND 15
#define NOISE_BAND_TEMP 15

// Number of wands to keep in memory
#define LW_DB_SZ 3

/******************************************************************************/
// This table is taken directly from Quantum code to match the exact paramters
/******************************************************************************/
// Resistor limits in DAC counts:
#define R5_MIN_DAC                 0
// Due to the leaky diode, this number is increased way higher than 45 (which is
// 5 ohms). Since the lowest resistor value to detect is 753, increasing this
// limit to 150 does not pose any problems.
#define R5_MAX_DAC                 150 





#define R2_R909_MIN_DAC            2549 // Creating a wider range for R2
#define R2_R909_MAX_DAC            2707 // because of Zener diode leakage


#define R121_MIN_DAC               753
#define R121_MAX_DAC               846

#define R147_MIN_DAC               881
#define R147_MAX_DAC               970

#define R169_MIN_DAC               982
#define R169_MAX_DAC               1091

#define R200_MIN_DAC               1110
#define R200_MAX_DAC               1234 

#define R243_MIN_DAC               1274
#define R243_MAX_DAC               1408 

#define R287_MIN_DAC               1424
#define R287_MAX_DAC               1550

#define R324_MIN_DAC               1553
#define R324_MAX_DAC               1669

#define R365_MIN_DAC               1670
#define R365_MAX_DAC               1788

#define R412_MIN_DAC               1789
#define R412_MAX_DAC               1912 

#define R464_MIN_DAC               1913
#define R464_MAX_DAC               2053

#define R576_MIN_DAC               2110
#define R576_MAX_DAC               2252

#define R649_MIN_DAC               2253
#define R649_MAX_DAC               2373

#define R750_MIN_DAC               2375
#define R750_MAX_DAC               2542

#define R909_MIN_DAC               2615 // 881 ohms 
#define R909_MAX_DAC               2672 // 936 ohms

#define R1050_MIN_DAC              2720
#define R1050_MAX_DAC              2828

#define R1210_MIN_DAC              2847
#define R1210_MAX_DAC              2948

#define R1400_MIN_DAC              2969
#define R1400_MAX_DAC              3064

#define R1650_MIN_DAC              3098
#define R1650_MAX_DAC              3186

#define R1960_MIN_DAC              3222
#define R1960_MAX_DAC              3302

#define R2400_MIN_DAC              3353
#define R2400_MAX_DAC              3423

#define R3010_MIN_DAC              3481
#define R3010_MAX_DAC              3541

#define R3920_MIN_DAC              3606
#define R3920_MAX_DAC              3656

#define R5600_MIN_DAC              3740
#define R5600_MAX_DAC              3777

#define R9090_MIN_DAC              3860
#define R9090_MAX_DAC              3900

#define R10500_MIN_DAC             3909
#define NUM_OF_RES_TABLE_ENTRY     26
//array definitions 
#define MIN_ENTRY                  0 
#define MAX_ENTRY                  1
#define PROBE_ENTRY                2

#define MAX_DAC                    4095     

// open TC value
#define OPEN_TC                   0x300  // 192 degC          

// coag adjustment for each resistor value. 
#define YES_COAG_ADJ              1	
#define NO_COAG_ADJ               0

// tc setting for each resistor value
#define YES_TC                    1
#define NO_TC                     0

// dual electrode setting for each resistor value
#define YES_DUAL_EL               1
#define NO_DUAL_EL                0

// setup screen required for dual electrode and t/c enabled wands
#define YES_SETUP                 1
#define NO_SETUP                  0



typedef enum {     PUMP_DISABLE = 0,
                   PUMP_ENABLE = 1 
                   } pump_setting;

typedef enum
{
  FUSE_BLOWN,
  NON_909,
  NEW_FUSE,
  WAND_909,
  /*
    @ENT - adding ENT R1 values
  */
  WAND_750,
  WAND_1400,
}R2_TYPES;

typedef struct{
uint8_t default_ablate;
uint8_t max_ablate;
uint8_t default_coag;
uint8_t max_coag;
uint16_t timer;
// bit fields
uint8_t  coag_adj:1;
uint8_t  tc:1;
uint8_t  dual_elect:1;
uint8_t  setup_req:1;
uint8_t  pump_en:1;
uint8_t  used:1; // this bit denotes if a paritcular R1 is being used or not.
}wand_types;         


typedef struct
{
  uint32_t current_time;
  uint32_t activation_time_sum;
  uint32_t overall_time_limit_sum;
}time_keeper;


typedef struct 
{
  uint16_t adc_value;
  uint8_t temp_celcius;
}adc_temp_convertor;

typedef struct
{
  uint32_t start_time;
  uint32_t activation_time_sum;
  uint16_t R1_wand_type;
  uint16_t R2_adc_value;
}previous_wands;
  



uint8_t legacy_wand_init (void);
uint8_t legacy_wand_read_adc_values (void);
uint8_t reset_therapy_settings (void);
uint8_t increment_mode (void);
uint8_t legacy_coag_increment(void);
uint8_t legacy_coag_decrement(void);
uint8_t legacy_ablate_increment(void);
uint8_t legacy_ablate_decrement(void);
uint8_t legacy_adc_temp_curve_fit (float,uint8_t*);
uint8_t legacy_wand_type_recognizer (uint16_t);
uint16_t filter_adc_value_r1 (uint16_t*,uint16_t*);
uint16_t filter_adc_value_r2 (uint16_t*,uint16_t*);
//uint16_t filter_adc_value_r2 (uint16_t);
uint8_t filter_temp (uint8_t*, uint8_t* );
uint8_t legacy_wand_button_read (uint16_t, uint16_t,uint16_t,float);
R2_TYPES legacy_wand_use_limit_check (uint16_t,uint16_t);
void legacy_wand_defaults_loader (uint8_t);
uint8_t equal_adc_value(uint16_t, uint16_t,uint16_t);
uint8_t legacy_wand_adc_fault (void);
uint8_t legacy_wand_unplugged (void);
uint8_t legacy_wand_UI_state_entry (void);
uint8_t lw_wand_switch_stuck_fault(void);
uint8_t lw_wand_expired_error(void);
uint8_t lw_wand_use_limit_error(void);
uint8_t switch_to_legacy_wand(void);
uint8_t lw_ambient_sensor (void);
void  lw_use_limit_time_loader (R2_TYPES,uint8_t);
uint8_t clear_lw_settings (void);
void lw_disconnect (void);
int compare_func (const void * val1, const void* val2);
float lw_get_pump_speed(u8 Index);
uint8_t check_18p_fs_stuck(void);
//uint8_t legacy_wand_r2_type_check (uint16_t );

#endif /*LEGACY_WAND_H*/
