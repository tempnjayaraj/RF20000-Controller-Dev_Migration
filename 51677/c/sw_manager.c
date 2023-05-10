/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2832                 $:
$Date:: 2022-04-26 14:12:49#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     software_manager.c
\brief     software manager module.
\todo      Add more description
\details   N/A
*******************************************************************************/

#include "sw_manager.h"
#include "system.h"
#include "MK26F18.h"
#include "esdhc.h"
#include "cpu.h"

__root program_header *pheader;

/***************************************************************************//**
This function kicks off the User Interface SW update process
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t commence_uis_sw_update(void)
{
     
  current_therapy_settings.sdhc.card_loaded = TRUE;
  current_therapy_settings.sdhc.update_uis = TRUE;
  
  if(current_therapy_settings.sdhc.uifw_update_avaliable)
  {
    
  /* relinquish ctrl of sdhc */
  NVIC_DisableIRQ(SDHC_IRQn);  
  PORTE_PCR4 = PORT_PCR_MUX(0);
  PORTE_PCR0 = PORT_PCR_MUX(0);
  PORTE_PCR1 = PORT_PCR_MUX(0);
  PORTE_PCR2 = PORT_PCR_MUX(0);
  PORTE_PCR3 = PORT_PCR_MUX(0);
  PORTE_PCR5 = PORT_PCR_MUX(0);
  }
  
  put_event(&evt_tbl,UI_UPDATE_STATUS);
  
  return 1;
  
}

/***************************************************************************//**
This function kicks off the generator SW update process
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t commence_gs_sw_update(void)
{

  uint32_t delay = (*(uint32_t*)0x4003D000) + 10;
  
  /* retain ctrl of sdhc */
  PORTE_PCR4 = PORT_PCR_MUX(4);
  
  PORTE_PCR0 = PORT_PCR_MUX(4) |
               PORT_PCR_PE_MASK |
               PORT_PCR_DSE_MASK |
               PORT_PCR_PS_MASK;
  
  PORTE_PCR1 = PORT_PCR_MUX(4) |
               PORT_PCR_PE_MASK |
               PORT_PCR_DSE_MASK |
               PORT_PCR_PS_MASK;
  
  PORTE_PCR2 = PORT_PCR_MUX(4) |
               PORT_PCR_PE_MASK |
               PORT_PCR_DSE_MASK |
               PORT_PCR_PS_MASK;
  
  PORTE_PCR3 = PORT_PCR_MUX(4) |
               PORT_PCR_PE_MASK |
               PORT_PCR_DSE_MASK |
               PORT_PCR_PS_MASK;
  
  PORTE_PCR5 = PORT_PCR_MUX(4) |
               PORT_PCR_PE_MASK | 
               PORT_PCR_DSE_MASK |
               PORT_PCR_PS_MASK;
  
  SDHC_IRQSTAT = 0xFFFFFFFF;
  
  NVIC_EnableIRQ(SDHC_IRQn);
  
  if (current_therapy_settings.sdhc.gsfw_update_avaliable == 1)
  {
    
    /* configure alarm register */
    RTC_TAR = delay;
    put_event(&evt_tbl,SDHC_DOWNLOAD);
    return 0;
    
  }
  else if (current_therapy_settings.sdhc.card_ready == 0)
  {
    
    current_therapy_settings.sw.update_progress = 0;
    return 1;
    
  }
  else if  (current_therapy_settings.sdhc.uifw_update_avaliable == 1)
  {
    
    RTC_TAR = delay;
    current_therapy_settings.sw.update_progress = 10;
    return 0;
    
  }
  else
  {
    
    current_therapy_settings.sw.update_progress = 10;
    return 0;
    
  }
    
              
}

//uint8_t gs_update_status(void)
//{
//  
//  static uint32_t sd_card_sector = 0;
//  
//  uint8_t update_complete = 0;
//      
//  /* probably should periodically check if card still inserted */
//    
//  sd_card_sector = download_sdhc_data2flash(sd_card_sector);
//  
//  if (sd_card_sector == SD_CARD_DOWNLOAD_COMPLETE_FLAG)
//  {
//    
//    update_complete = 1;
//    current_therapy_settings.sw.update_progress = 10;
//    
//  }
//  else
//  {
//    
//    put_event(&evt_tbl,COMMENCE_GS_SW_UPDATE);
//    
//  }
// 
//  return update_complete;
//  
//}

/***************************************************************************//**
This function updates the update_complete flag when the UI software update is
done
@none
@return the update complete status
@outputs
@context: 
*******************************************************************************/
uint8_t ui_update_status(void)
{
  enum UPDATE_GUI_STATUS{
  UPDATE_NOT_STARTED,
  UPDATE_IN_PROGRESS,
  UPDATE_COMPLETED,
  UPDATE_ABORTED};

  uint8_t update_complete = 0;
  
  //current_therapy_settings.gui.sw.update_status = UPDATE_COMPLETED;
  
  if (current_therapy_settings.gui.sw.update_status >= UPDATE_COMPLETED)
  {

     current_therapy_settings.sdhc.update_uis = FALSE;
     //current_therapy_settings.sdhc.card_loaded = FALSE;
     //current_therapy_settings.sdhc.card_ready = 0;
     //current_therapy_settings.sw.update_progress = 10;
     //put_event(&evt_tbl,COMMENCE_GS_SW_UPDATE);
     SYST_CSR |= SysTick_CSR_ENABLE_MASK;
     
     update_complete = 1;
     
  }
  
  return update_complete;
  
}

/***************************************************************************//**
This function checks if the generator firmware needs to be updated.
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t check4gsfw(uint8_t major, uint8_t minor)
{

  uint16_t gsfw_ver = 0,
           gsfw_sdhc_ver = 0;
  
  uint8_t gsfw_update = 0;  
    
  gsfw_sdhc_ver = (major << 8) | (minor);
  gsfw_ver = ((pheader->meta.app_version_major << 8) |
              (pheader->meta.app_version_minor));
           
  if (gsfw_sdhc_ver > gsfw_ver)
  {

    gsfw_update = 1;

  }
  
  // Comment this condition for K26. 
  // K60 sw Sd-card should not work on K26 controller
  // This condition means that the SD-card is only to update UI sw
//  if (gsfw_sdhc_ver == 0)
//  {
//    
//    gsfw_update = 1;
//    
//  }
    
  return gsfw_update;
    
}

/***************************************************************************//**
This function checks if the User interface firmware needs to be updated.
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t check4uifw(uint8_t major, uint8_t minor)
{

  uint16_t uifw_ver = 0,
           uifw_sdhc_ver = 0;
  
  uint8_t uifw_update = 0;
  
  uifw_ver = (major << 8) | (minor);
  uifw_sdhc_ver = ((current_therapy_settings.sdhc.uifw_major << 8) |
                   (current_therapy_settings.sdhc.uifw_minor));
  
  /* GUI 1.3 not backwards compatible so do not update */
  if (uifw_ver < 0x0103)
  {
    return 0;
  }
  
  if (uifw_sdhc_ver > uifw_ver)
  {

    uifw_update = 1;

  }
  
  if ((current_therapy_settings.sdhc.gsfw_update_avaliable == 1) &&
      (uifw_sdhc_ver == 0))
  {
    
    uifw_update = 1;
    
  }
      
  return uifw_update;
    
}

/***************************************************************************//**
This function allows the user to cancel the SW update.
@none
@return 1
@outputs
@context: 
*******************************************************************************/
uint8_t cancel_sw_update(void)
{
  
  current_therapy_settings.sdhc.card_ready = 0;
  current_therapy_settings.sdhc.update_uis = FALSE;
  current_therapy_settings.sdhc.gsfw_update_avaliable = 0;
  current_therapy_settings.sdhc.uifw_update_avaliable = 0; 
  put_event(&evt_tbl,COMMENCE_GS_SW_UPDATE);
  
  return 0;
  
}

uint8_t ui_gs_firmware_update_pause(void)
{
  
  static uint16_t systick_count_firmware_pause = 0;
  
  if(current_therapy_settings.sdhc.card_loaded == TRUE)
  {
   
    if (systick_count_firmware_pause++ == 500)
    {
      
      systick_count_firmware_pause = 0;
      put_event(&evt_tbl,COMMENCE_GS_SW_UPDATE);
      
    }
    else
    {
      SYST_CSR |= SysTick_CSR_ENABLE_MASK;
    }
    
  }

  return 0;
  
}