/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2821                 $:
$Date:: 2022-03-07 12:49:31#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/

/***************************************************************************//**
\file      ftfe.c
\brief     Driver for flash memory module
\details  
\todo More description
*******************************************************************************/
#include "ftfe.h"
#include "system.h"
#include <string.h>
#include <intrinsics.h>
#include "nv_data.h"
#include "profiles.h"
#include "crc.h"
#include "legacy_wand.h"
#include "wand.h"

extern previous_wands previous_wands_database[LW_DB_SZ];
extern uint8_t current_coag_setpoint;

__no_init uint8_t eeram[4096] @ 0x14000000;

/***************************************************************************//**
Program flash once
@todo More description
@param location, size and  record
@return 0
@outputs
@context
*******************************************************************************/
uint8_t program_ftfe_once(uint8_t* ptr, uint8_t size, uint8_t record)        
{

  uint8_t error = 0;
  uint8_t data[8] = {0,0,0,0,0,0,0,0};
  
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
  
  /* boundary check arguments */
  if (size > 8)
  {
    
    size = 8;
    
  }
  
  if (record > 7)
  {
    
    record = 7;
    
  }
  
  memcpy(&data[0],ptr,size);
#ifndef TEST
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);
#endif  
  /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();                        
  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* write one-time programmable in p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_PROGRAM_ONCE_CMD;
  FTFE_FCCOB1 = record;
  FTFE_FCCOB2 = 0;
  FTFE_FCCOB3 = 0;
  FTFE_FCCOB4 = data[0];
  FTFE_FCCOB5 = data[1];
  FTFE_FCCOB6 = data[2];
  FTFE_FCCOB7 = data[3];
  FTFE_FCCOB8 = data[4];
  FTFE_FCCOB9 = data[5];
  FTFE_FCCOBA = data[6];
  FTFE_FCCOBB = data[7];
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);
#endif  
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}

/***************************************************************************//**
Read the flash
@todo More description
@param location, size and record
@return 0
@outputs
@context
*******************************************************************************/
uint8_t read_ftfe_once(uint8_t* ptr, uint8_t size, uint8_t record)
{
   
  uint8_t error = 0;
  uint8_t data[8] = {0,0,0,0,0,0,0,0};
  
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
  
  /* boundary check arguments */
  if (size > 8)
  {
    
    size = 8;
    
  }
  
  if (record > 7)
  {
    
    record = 7;
    
  }
#ifndef TEST      
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);
#endif  
  /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();                        
  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* read one-time programmable from p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_READ_ONCE_CMD;
  FTFE_FCCOB1 = record;
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
#ifndef TEST  
  /* wait for cmd complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);
#endif  
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  data[0] = FTFE_FCCOB4;
  data[1] = FTFE_FCCOB5;
  data[2] = FTFE_FCCOB6;
  data[3] = FTFE_FCCOB7; 
  data[4] = FTFE_FCCOB8;
  data[5] = FTFE_FCCOB9;
  data[6] = FTFE_FCCOBA;
  data[7] = FTFE_FCCOBB;
  
  /* copy mfg date time to ram */
  for (uint8_t i = 0; i < size; i++)
  {

    *ptr++ = data[i];    
    
  }
        
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;
  
}

/***************************************************************************//**
Erase Sector
@todo More description
@param location
@return 0
@outputs
@context
*******************************************************************************/
uint8_t erase_ftfe_sector(uint32_t destination)        
{

  uint8_t error = 0;
  
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
     
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();  
#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                     
#endif  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* write one-time programmable in p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_ERASE_SECTOR;
  FTFE_FCCOB1 = ((destination >> 16) & 0xFF);
  FTFE_FCCOB2 = ((destination >> 8) & 0xFF);
  FTFE_FCCOB3 = (destination & 0xFF);
  FTFE_FCCOB4 = 0;
  FTFE_FCCOB5 = 0;
  FTFE_FCCOB6 = 0;
  FTFE_FCCOB7 = 0;
  FTFE_FCCOB8 = 0;
  FTFE_FCCOB9 = 0;
  FTFE_FCCOBA = 0;
  FTFE_FCCOBB = 0;
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 
#endif
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}

/***************************************************************************//**
Write to OTP
@todo More description
@param location, size and record
@return 0
@outputs
@context
*******************************************************************************/
uint8_t write_ftfe_phrase(void)        
{

  uint8_t error = 0;
    
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
     
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();   
#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                  
#endif  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* write one-time programmable in p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_PROGRAM_PHRASE;
  FTFE_FCCOB1 = 0x07;
  FTFE_FCCOB2 = 0xE0;
  FTFE_FCCOB3 = 0x00;
  FTFE_FCCOB4 = 0xFF;
  FTFE_FCCOB5 = 0xFF;
  FTFE_FCCOB6 = current_therapy_settings.ambient.volume;
  FTFE_FCCOB7 = current_therapy_settings.audio.volume;
  FTFE_FCCOB8 = 0xFF;
  FTFE_FCCOB9 = 0xFF;
  FTFE_FCCOBA = 0xFF;
  FTFE_FCCOBB = 0xFF;
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
#ifndef TEST
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 
#endif
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}

/***************************************************************************//**
Write flash section
@todo More description
@param location, size 
@return 0
@outputs
@context
*******************************************************************************/
uint8_t write_ftfe_section(uint32_t destination,uint16_t nwords)        
{

  uint8_t error = 0;
      
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
  
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();   
#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                  
#endif  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* write one-time programmable in p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_PROGRAM_SECTION;
  FTFE_FCCOB1 = ((destination >> 16) & 0xFF);
  FTFE_FCCOB2 = ((destination >> 8) & 0xFF);
  FTFE_FCCOB3 = (destination & 0xFF);
  FTFE_FCCOB4 = ((nwords >> 8) & 0xFF);
  FTFE_FCCOB5 = (nwords & 0xFF);
  FTFE_FCCOB6 = 0;
  FTFE_FCCOB7 = 0;
  FTFE_FCCOB8 = 0;
  FTFE_FCCOB9 = 0;
  FTFE_FCCOBA = 0;
  FTFE_FCCOBB = 0;
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 
#endif
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}


static uint16_t num_flash_words(uint16_t bytes)
{
  /* round up to minimum 16-byte flash word */
  return (((bytes) + 15) >> 4);
}

/***************************************************************************//**
Save parameters to Flash
@todo More description
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t save_params2flash(void)
{
  
  uint16_t nwords = num_flash_words(sizeof(nonvolatile_data_layout));
  /* pointer to "cache" */
  nonvolatile_data_layout *p_cache = (nonvolatile_data_layout*)&eeram[0];
  
  memset(p_cache,0xFF,0x560);
  //memset((p_cache+0x400),0x44,0x150);
  get_cached_nv_data();
  current_therapy_settings.gui.languageID = ENGLISH;
  /* restore default profile */
  p_cache->user_data.profile[0].ambient.alarm_threshold = 45;
  p_cache->user_data.profile[0].ambient.audio_enabled = 1;
  p_cache->user_data.profile[0].ambient.enable = 1;
  p_cache->user_data.profile[0].finger_switch_en = (bool)1;
  p_cache->user_data.profile[0].record_full = TRUE;
  //p_cache->user_data.profile[0].wireless_enable = (bool)0;
  p_cache->user_data.profile[0].setpoint.lo = ABLATE_DEFAULT;
  p_cache->user_data.profile[0].setpoint.med = ABLATE_DEFAULT;
  p_cache->user_data.profile[0].setpoint.hi = ABLATE_DEFAULT;
  
  erase_ftfe_sector(NONVOLATILE_DATA_ADDRESS);
  read_ftfe_section(NONVOLATILE_DATA_ADDRESS,nwords);
  write_ftfe_section(NONVOLATILE_DATA_ADDRESS,64);
 
  memcpy(&eeram[0],&eeram[1024],352);
  write_ftfe_section((NONVOLATILE_DATA_ADDRESS+0x400),32);
  
  current_therapy_settings.sw.shutdown = TRUE; 
  memset(p_cache,0xFF,0x560);
  return 1;
  
}

uint8_t *get_cached_nv_data(void)
{
  /* restore FlexRAM to known state */
  memset(&eeram[0],0xFF,4096);
  uint16_t nwords = num_flash_words(sizeof(nonvolatile_data_layout));  
  /* pointer to current nv data for re-writing */
  nonvolatile_data_layout *p_current_nv_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
  /* pointer to "cache" */
  nonvolatile_data_layout *p_cache = (nonvolatile_data_layout*)&eeram[0];
  
  p_cache->master_volume = current_therapy_settings.audio.volume;
  p_cache->ambient_volume = current_therapy_settings.ambient.volume;
  p_cache->active_app_ptr = p_current_nv_data->active_app_ptr;
  memset(&p_cache->serial_number[0],0xFF,MFG_SERIAL_NUM_CHARS);
  memset(&p_cache->mfg_date_time[0],0xFF,MFG_TIME_NUM_CHARS);
  p_cache->user_data = current_therapy_settings.user_data;
  p_cache->vbatt_tamper_detect = current_therapy_settings.rtc.tamper_detected;
  p_cache->app1_version_major = current_therapy_settings.sw.app1_version_major;
  p_cache->app1_version_minor = current_therapy_settings.sw.app1_version_minor;
  p_cache->app2_version_major = current_therapy_settings.sw.app2_version_major;
  p_cache->app2_version_minor = current_therapy_settings.sw.app2_version_minor;
  p_cache->languageID = (uint8_t)current_therapy_settings.gui.languageID;
  p_cache->nwords = nwords;
  for (uint8_t i = 0; i < sizeof(p_cache->smart_wand.work_order); i++)
  {
    p_cache->smart_wand.work_order[i] = smart_wand_params.mfg.work_order[i];
  }
  
  for (uint8_t i = 0; i < sizeof(p_cache->smart_wand.unique_id); i++)
  {
    p_cache->smart_wand.unique_id[i] = smart_wand_params.mfg.unique_id[i];
  }
  for (uint8_t i = 0; i < LW_DB_SZ; i++)
  {
    p_cache->nv_previous_wands_database[i].activation_time_sum = 
     previous_wands_database[i].activation_time_sum;
    p_cache->nv_previous_wands_database[i].R1_wand_type = 
     previous_wands_database[i].R1_wand_type;
    p_cache->nv_previous_wands_database[i].R2_adc_value = 
     previous_wands_database[i].R2_adc_value;
    p_cache->nv_previous_wands_database[i].start_time = 
     previous_wands_database[i].start_time;
  }
  if ((current_therapy_settings.rtc.power_down_time) < (RTC_TSR & RTC_TSR_TSR_MASK)) 
  {
    p_cache->rtc = (RTC_TSR & RTC_TSR_TSR_MASK);
  }
  else
  {
    p_cache->rtc = p_current_nv_data->rtc;
  }
  p_cache->wireless_en = current_therapy_settings.treatment.wireless_enable;
  p_cache->wireless_married_latch = current_therapy_settings.footpedal.wireless.married_latch;
  p_cache->previous_wand_indication = current_therapy_settings.previous_wand_indication;
  if(current_therapy_settings.previous_wand_indication == SMART_WAND_MFG_INDICATION_MANTA)
  {
    p_cache->previous_wand_coag = current_coag_setpoint;//current_therapy_settings.setpoint.coag;
    p_cache->previous_wand_flow_rate = current_therapy_settings.treatment.flow_rate;
  }
  uint16_t nbytes = nwords << 4;
  /* calculate checksum */
  p_cache->checksum = fast_crc16(0,
                                 &eeram[0]+2, 
                                 nbytes-2);
  
  return (uint8_t*)p_cache;
  
}

/***************************************************************************//**
Set FlexRAM Function
@todo More description
@param location
@return 0
@outputs
@context
*******************************************************************************/
uint8_t set_ftfe_function (void)        
{

  uint8_t error = 0;
  
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
     
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();  
//#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                     
//#endif  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* write one-time programmable in p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_SET_FLEXRAM_FN;
  FTFE_FCCOB1 = 0;
  FTFE_FCCOB2 = 0;
  FTFE_FCCOB3 = 0;
  FTFE_FCCOB4 = 0;
  FTFE_FCCOB5 = 0;
  FTFE_FCCOB6 = 0;
  FTFE_FCCOB7 = 0;
  FTFE_FCCOB8 = 0;
  FTFE_FCCOB9 = 0;
  FTFE_FCCOBA = 0;
  FTFE_FCCOBB = 0;
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
//#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 
//#endif
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}

/***************************************************************************//**
Program Partition
@todo More description
@param location
@return 0
@outputs
@context
*******************************************************************************/
uint8_t program_ftfe_partition (void)        
{

  uint8_t error = 0;
  
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
     
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();  
//#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                     
//#endif  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* write one-time programmable in p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_PROGRAM_PARTITION;
  FTFE_FCCOB1 = 0;
  FTFE_FCCOB2 = 0;
  FTFE_FCCOB3 = 0;
  FTFE_FCCOB4 = 0x02;
  FTFE_FCCOB5 = 0x08;
  FTFE_FCCOB6 = 0;
  FTFE_FCCOB7 = 0;
  FTFE_FCCOB8 = 0;
  FTFE_FCCOB9 = 0;
  FTFE_FCCOBA = 0;
  FTFE_FCCOBB = 0;
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
//#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 
//#endif
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}

/***************************************************************************//**
Read flash section
@todo More description
@param location, size 
@return 0
@outputs
@context
*******************************************************************************/
uint8_t read_ftfe_section(uint32_t destination,uint16_t nwords)        
{

  uint8_t error = 0;
      
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
  
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();   
#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                  
#endif  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* write one-time programmable in p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_READ_SECTION;
  FTFE_FCCOB1 = ((destination >> 16) & 0xFF);
  FTFE_FCCOB2 = ((destination >> 8) & 0xFF);
  FTFE_FCCOB3 = (destination & 0xFF);
  FTFE_FCCOB4 = ((nwords >> 8) & 0xFF);
  FTFE_FCCOB5 = (nwords & 0xFF);
  FTFE_FCCOB6 = 0; //normal read level
  FTFE_FCCOB7 = 0;
  FTFE_FCCOB8 = 0;
  FTFE_FCCOB9 = 0;
  FTFE_FCCOBA = 0;
  FTFE_FCCOBB = 0;
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
#ifndef TEST  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 
#endif
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}
