/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2830                 $:
$Date:: 2022-04-15 09:55:01#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      esdhc.c
\brief     SD card driver
\details   This is used for SD logging during power cycle
*******************************************************************************/

#include "MK26F18.h"
#include "esdhc.h"
#include "esdhc_def.h"
#include "cpu.h"
#include "ftfe.h"
#include "system.h"
#include "ftfe.h"
#include "wdog.h"
#include "crc.h"
#include "sw_manager.h"
#include "legacy_wand.h"
#include "notifications.h"

const ESDHC_META empty_sd_meta;
extern uint8_t eeram[4096] @ 0x14000000;

/***************************************************************************//**
Initialize SD card peripheral
@param void
@return 0
@outputs
@context
*******************************************************************************/
extern legacy_wand_params legacy_wand_params_obj;

/***************************************************************************//**
Inits SD card interface.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t esdhc_init(void)
{
 
  NVIC_DisableIRQ(SDHC_IRQn);
  
  /* Enable gate clock */
  SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;


  MPU_CESR &= 0xFFFFFFFE; 

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
  
    PORTE_PCR4 = PORT_PCR_MUX(4) |
               PORT_PCR_PE_MASK |
               PORT_PCR_DSE_MASK |
               PORT_PCR_PS_MASK;
  
  PORTE_PCR5 = PORT_PCR_MUX(4) |
               PORT_PCR_PE_MASK | 
               PORT_PCR_DSE_MASK |
               PORT_PCR_PS_MASK;

  /* Soft reset */
  SDHC_SYSCTL |= SDHC_SYSCTL_RSTA_MASK;

#ifndef TEST
  while((SDHC_SYSCTL&SDHC_SYSCTL_RSTA_MASK)!=0); // wait sdhc to clear this bit automatically..
#endif

  SDHC_SYSCTL = (SDHC_SYSCTL_SDCLKFS(8) | SDHC_SYSCTL_DVS(0));
  //SDHC_SYSCTL = (SDHC_SYSCTL_SDCLKFS(0x80) | SDHC_SYSCTL_DVS(0x01));
  SDHC_SYSCTL |= SDHC_SYSCTL_DTOCV(0xE);
  SDHC_SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK |
                 SDHC_SYSCTL_PEREN_MASK |
                 SDHC_SYSCTL_HCKEN_MASK |
                 SDHC_SYSCTL_IPGEN_MASK;
#ifndef TEST
  while((SDHC_PRSSTAT& SDHC_PRSSTAT_SDSTB_MASK)!=SDHC_PRSSTAT_SDSTB_MASK); //waiting sd clock stable
#endif


  /* clear IRQSTAT */
  SDHC_IRQSTAT=0xffffffff; 

  /* use DAT3 to detect card */
  SDHC_PROCTL = SDHC_PROCTL_EMODE(2);
  
    
  SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1)| SDHC_BLKATTR_BLKSIZE(0x200);
  SDHC_PROCTL |= SDHC_PROCTL_DTW(DTW_4BIT);

  SDHC_WML = 	((WML_WML4) <<SDHC_WML_WRWML_SHIFT)
     |((WML_WML4) <<SDHC_WML_RDWML_SHIFT) ; 
    
  /* Initialize card */
  
  
  SDHC_IRQSTATEN = 0;
  // Clear Command Complete bit
  SDHC_IRQSTAT = 0xFFFFFFFF;
  
  SDHC_IRQSIGEN = 0;
  
  NVIC_EnableIRQ(SDHC_IRQn);
    
  sd_card_init(1);
  return 0;
}

/***************************************************************************//**
Download data from the SD card
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t download_sdhc_data(void)
{

  static SDHC_DOWNLOAD_STATE sdhc_download_state = READ_MBR;
  
  SDHC_MemMapPtr pSDHC = SDHC_BASE_PTR;
  
  uint8_t download_complete_flag = 0;
  
  static uint32_t flash_idx = 0,
                  ending_sector = 0;
  
  static uint8_t eeram_idx = 0,
                 sdhc_data[512];
  
  bool flag_do_flash_erase = FALSE;
   
  switch(sdhc_download_state)
  {
    case (READ_MBR):
    {
      uint32_t sector = 0;
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK;  
      memset(&sdhc_data[0],0xFF,512); 
      SDHC_DSADDR = (uint32_t)&sdhc_data[0];	
      /* read block */
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP = MMC_CMD17 |
                     SDHC_XFERTYP_DPSEL_MASK |
                     SDHC_XFERTYP_DTDSEL_MASK |
                     SDHC_XFERTYP_AC12EN_MASK |
                     SDHC_XFERTYP_BCEN_MASK |
                     SDHC_XFERTYP_DMAEN_MASK;
      sdhc_download_state++;
      current_therapy_settings.sw.update_progress = 0;
      break;
        
    }
    case (START_FILE_SEARCH):
    {
   
      uint32_t sector;
      
      /* extract 1st partition entry */
      current_therapy_settings.sdhc.mbr.pt[0].starting_sector = (
         (sdhc_data[457] << 24) |
         (sdhc_data[456] << 16) |
         (sdhc_data[455] << 8) |
         (sdhc_data[454]));
        
      current_therapy_settings.sdhc.mbr.pt[0].size = (
         (sdhc_data[461] << 24) |
         (sdhc_data[460] << 16) |
         (sdhc_data[459] << 8) |
         (sdhc_data[458]));

      current_therapy_settings.sdhc.mbr.pt[0].type = sdhc_data[450];

      /* extract 2nd partition entry */
      current_therapy_settings.sdhc.mbr.pt[1].starting_sector = (
         (sdhc_data[473] << 24) |
         (sdhc_data[472] << 16) |
         (sdhc_data[471] << 8) |
         (sdhc_data[470]));
        
      current_therapy_settings.sdhc.mbr.pt[1].size = (
         (sdhc_data[477] << 24) |
         (sdhc_data[476] << 16) |
         (sdhc_data[475] << 8) |
         (sdhc_data[474]));

      current_therapy_settings.sdhc.mbr.pt[1].type = sdhc_data[466];

      current_therapy_settings.sdhc.mbr.pt[1].ending_sector = 
        current_therapy_settings.sdhc.mbr.pt[1].starting_sector +
        current_therapy_settings.sdhc.mbr.pt[1].size;
  
      sector = current_therapy_settings.sdhc.mbr.pt[1].starting_sector;
      memset(&sdhc_data[0],0xFF,512); 
      SDHC_DSADDR = (uint32_t)&sdhc_data[0];		
      /* read block */
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP = MMC_CMD17 |
                     SDHC_XFERTYP_DPSEL_MASK |
                     SDHC_XFERTYP_DTDSEL_MASK |
                     SDHC_XFERTYP_AC12EN_MASK |
                     SDHC_XFERTYP_BCEN_MASK |
                     SDHC_XFERTYP_DMAEN_MASK;
      sdhc_download_state++;
      break;
      
    }
    case (SEARCH_4_FILE):
    {
      
      uint32_t sector = pSDHC->CMDARG + 1;
      header_layout *p_header;
      uint32_t starting_sector = pSDHC->CMDARG,
               size;
      nonvolatile_data_layout *p_nv_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
                       
      if(memcmp(&sdhc_data[20],"RF20000P",8)==0)
      {

        p_header = (header_layout*)&sdhc_data[0];
        size = (p_header->ropi_bytes / 512) + 1;
        ending_sector = starting_sector + size;

        sdhc_download_state = DOWNLOAD_FILE;
        memset(&eeram[0],0xFF,4096); 
        if (p_nv_data->active_app_ptr == 0x40004)
        {
          flash_idx = 0x10000;
          current_therapy_settings.sw.app1_version_major = p_header->app_version_major;
          current_therapy_settings.sw.app1_version_minor = p_header->app_version_minor;
        }
        else
        {
          flash_idx = 0x40000;
          current_therapy_settings.sw.app2_version_major = p_header->app_version_major;
          current_therapy_settings.sw.app2_version_minor = p_header->app_version_minor;
        }
        put_event(&evt_tbl,SDHC_DOWNLOAD);
          
      }
      else
      {
      
        memset(&sdhc_data[0],0xFF,512); 
        SDHC_DSADDR = (uint32_t)&sdhc_data[0];		
        /* read block */
        SDHC_CMDARG =  sector;
        SDHC_XFERTYP = MMC_CMD17 |
                       SDHC_XFERTYP_DPSEL_MASK |
                       SDHC_XFERTYP_DTDSEL_MASK |
                       SDHC_XFERTYP_AC12EN_MASK |
                       SDHC_XFERTYP_BCEN_MASK |
                       SDHC_XFERTYP_DMAEN_MASK;
          
      }
      break;
       
    }
    case (DOWNLOAD_FILE):
    {
      
      uint32_t sector = pSDHC->CMDARG + 1;
      eeram_idx %= 4;
      
      
      memcpy(&eeram[eeram_idx*512],&sdhc_data[0],512);
      
      if ((eeram_idx == 3) || (sector > ending_sector))
      {

        sdhc_download_state = WRITE2FLASH;
        put_event(&evt_tbl,SDHC_DOWNLOAD);
        flag_do_flash_erase = TRUE;
      }
      else
      {
        
        memset(&sdhc_data[0],0xFF,512); 
        SDHC_DSADDR = (uint32_t)&sdhc_data[0];		
        /* read block */
        SDHC_CMDARG =  sector;
        SDHC_XFERTYP = MMC_CMD17 |
                       SDHC_XFERTYP_DPSEL_MASK |
                       SDHC_XFERTYP_DTDSEL_MASK |
                       SDHC_XFERTYP_AC12EN_MASK |
                       SDHC_XFERTYP_BCEN_MASK |
                       SDHC_XFERTYP_DMAEN_MASK;     
        asm("nop");
        asm("nop");
        asm("nop");
        
        
      }
      
      eeram_idx++;

      break;
      
    }
    case (WRITE2FLASH):
    {

      uint32_t sector = pSDHC->CMDARG + 1;
      
      nonvolatile_data_layout *p_nv_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;
      uint32_t flash_idx_start = 0,
               flash_idx_end = 0;
      
      if (p_nv_data->active_app_ptr == 0x10004)
      {
        
        flash_idx_start = 0x40000;
        flash_idx_end = 0x6FFFF;
        
      }
      else
      {
        
        flash_idx_start = 0x10000;
        flash_idx_end = 0x3FFFF;
        
      }
      
      if ((flash_idx >= flash_idx_start) && (flash_idx < flash_idx_end))
      {
      
        GPIOA_PTOR = BLUE_LED;
        
        if((flash_idx % 4096) == 0)
        //if(flag_do_flash_erase)
        {
          //flag_do_flash_erase = FALSE;
          erase_ftfe_sector(flash_idx);
          
        }
          
          wdog_refresh();
          write_ftfe_section(flash_idx,64);
          
          memcpy(&eeram[0],&eeram[1024],1024);
          write_ftfe_section((flash_idx+0x400),64);
          

        
        GPIOA_PTOR = BLUE_LED;
      
      }
      
      memset(&eeram[0],0xFF,2048);
      
      if (sector > ending_sector)
      {
        uint32_t delay = (*(uint32_t*)0x4003D000) + 3;
        download_complete_flag = 1;
        current_therapy_settings.sw.update_progress = 10;
        /* configure alarm register */
        RTC_TAR = delay;
        //current_therapy_settings.sw.update_complete = TRUE;
        
        
      }
      else
      {
        
        sdhc_download_state = DOWNLOAD_FILE;
        memset(&sdhc_data[0],0xFF,512); 
        SDHC_DSADDR = (uint32_t)&sdhc_data[0];		
        /* read block */
        SDHC_CMDARG =  sector;
        SDHC_XFERTYP = MMC_CMD17 |
                       SDHC_XFERTYP_DPSEL_MASK |
                       SDHC_XFERTYP_DTDSEL_MASK |
                       SDHC_XFERTYP_AC12EN_MASK |
                       SDHC_XFERTYP_BCEN_MASK |
                       SDHC_XFERTYP_DMAEN_MASK;
        flash_idx += 2048;
        
      }
      
      
      
      break;
      
    }
        
  }
        
  return download_complete_flag;
        
}

/***************************************************************************//**
Setup SD card
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t sd_card_init(uint8_t reset)
{
  SDHC_MemMapPtr pSDHC = SDHC_BASE_PTR;
  static uint32_t base_rca = 0,
                  rca = 0;
  static uint8_t data[512];
  static uint32_t ocr_attempts =0,
                  time = 0;
  static uint16_t attempts = 0;
  static SDHC_INIT_STATE sdhc_init_state;
  uint8_t card_ready = 0;
  //static uint8_t tmp[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  //static uint32_t response_2 = 0;
  
  if (reset == 1)
  {
    
    base_rca = 0;
    rca = 0;
    for (int i = 0; i < 512; i++)
    {
      
      data[i] = 0;
      
    }
    ocr_attempts =0;
    attempts = 0;
    sdhc_init_state = CMD0_GO_IDLE;
    current_therapy_settings.sdhc.card_fault = 0;
    time = 0;
  
  }
  
  switch (sdhc_init_state)
  {
    case(CMD0_GO_IDLE):
    {

      SDHC_IRQSTATEN = SDHC_IRQSTATEN_CCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_CCIEN_MASK;
      SDHC_CMDARG =  0x0;
      SDHC_XFERTYP =  MMC_CMD0;
      sdhc_init_state = GET_CARD_VERSION;
      break;
    
    }
    case(GET_CARD_VERSION):
    {

      SDHC_CMDARG 	=  CHECK_PATTEN | VHS;
      SDHC_XFERTYP 	=  SD_CMD8;
      sdhc_init_state = CMD55_CARD_APP_CMD_0;
      break;
        
    }
    case(CMD55_CARD_APP_CMD_0):
    {
      
      SDHC_CMDARG = 0;
      SDHC_XFERTYP = SD_CMD55;
      sdhc_init_state = REQUEST_OCR_0;
      break;
    }
    case(REQUEST_OCR_0):
    {
      
      SDHC_CMDARG =  (1<<30);
      SDHC_XFERTYP =  SD_ACMD41;
      sdhc_init_state = CMD55_CARD_APP_CMD_1;
      break;
    }
    case(CMD55_CARD_APP_CMD_1):
    {
      //response_2 = pSDHC->CMDRSP[0];
      SDHC_CMDARG = 0;
      SDHC_XFERTYP = SD_CMD55;
      sdhc_init_state = REQUEST_OCR_1;
      break;
      
    }
    case(REQUEST_OCR_1):
    {
           
      //SDHC_CMDARG = (response_2|1<<30);
      //SDHC_CMDARG = (response_2);
      SDHC_CMDARG = 0;
      SDHC_XFERTYP = SD_ACMD41;
      sdhc_init_state = CMD55_CARD_APP_CMD_2;    
      rca = 0;
      ocr_attempts = 0;
      break;
      
    }
    case(CMD55_CARD_APP_CMD_2):
    {
      
      if (rca == 0)
      {
        rca = pSDHC->CMDRSP[0];
      }
      SDHC_CMDARG = 0;
      SDHC_XFERTYP = SD_CMD55;
      sdhc_init_state = REQUEST_OCR_2;
      break;
      
    }
    case(REQUEST_OCR_2):
    {

      uint32_t response = pSDHC->CMDRSP[0];
      
      if (attempts > 40)
      {
            put_event(&evt_tbl,INIT_MANAGER);
            current_therapy_settings.sdhc.card_fault = 1;
            NVIC_DisableIRQ(SDHC_IRQn);        
      }
      else if (ocr_attempts >= 1000)
      {
        
        base_rca = 0;
        rca = 0;
        sdhc_init_state = CMD0_GO_IDLE;
        put_event(&evt_tbl,SDHC_DOWNLOAD);
        attempts++;
                
      }
      else if ((response & SD_BUSY) != 0)
      {
      
        sdhc_init_state = REQUEST_ALL_CIDS;
        put_event(&evt_tbl,SDHC_DOWNLOAD);
        
      }
      else
      {
        HCS=1;
        SDHC_CMDARG = (rca|HCS<<30);
        SDHC_XFERTYP = SD_ACMD41;
        sdhc_init_state = CMD55_CARD_APP_CMD_2;  
        ocr_attempts++;
               
      }
      
      break;
      
    }
    case(REQUEST_ALL_CIDS):
    {
         
      SDHC_CMDARG =  0x0;
      SDHC_XFERTYP =  MMC_CMD2;
      sdhc_init_state = GET_RCA;
      break;
      
    }
    case(GET_RCA):
    {

      SDHC_CMDARG =  0x0;
      SDHC_XFERTYP =  MMC_CMD3;
      sdhc_init_state = ADDRESS_CARD;
      base_rca = 0;
      break;
      
    }
    case(ADDRESS_CARD):
    {
      base_rca = (pSDHC->CMDRSP[0] & 0xFFFF0000);
      SDHC_CMDARG =  base_rca;
      SDHC_XFERTYP =  MMC_CMD9;
      sdhc_init_state = SEND_CID;
      for (int i = 0; i < sizeof(data); i++)
      {
        data[i]=0;
      }
      break;
      
    }   
    case(SEND_CID):
    {
      
      /* check csd and cid */
      data[14] = (uint8_t) (pSDHC->CMDRSP[0]  & 0x000000FF);
      data[13] = (uint8_t) ((pSDHC->CMDRSP[0]  & 0x0000FF00) >> 8);
      data[12] = (uint8_t) ((pSDHC->CMDRSP[0]  & 0x00FF0000) >> 16);
      data[11] = (uint8_t) ((pSDHC->CMDRSP[0]  & 0xFF000000) >> 24);
      data[10] = (uint8_t) (pSDHC->CMDRSP[1]  & 0x000000FF);
      data[9] = (uint8_t) ((pSDHC->CMDRSP[1]  & 0x0000FF00) >> 8);
      data[8] = (uint8_t) ((pSDHC->CMDRSP[1]  & 0x00FF0000) >> 16);
      data[7] = (uint8_t) ((pSDHC->CMDRSP[1]  & 0xFF000000) >> 24);
      data[6] = (uint8_t) (pSDHC->CMDRSP[2]  & 0x000000FF);
      data[5] = (uint8_t) ((pSDHC->CMDRSP[2]  & 0x0000FF00) >> 8);
      data[4] = (uint8_t) ((pSDHC->CMDRSP[2]  & 0x00FF0000) >> 16);
      data[3] = (uint8_t) ((pSDHC->CMDRSP[2]  & 0xFF000000) >> 24);
      data[2] = (uint8_t) (pSDHC->CMDRSP[3]  & 0x000000FF);
      data[1] = (uint8_t) ((pSDHC->CMDRSP[3]  & 0x0000FF00) >> 8);
      data[0] = (uint8_t) ((pSDHC->CMDRSP[3]  & 0x00FF0000) >> 16);
      //data[0] = (uint8_t) ((pSDHC->CMDRSP[3]  & 0xFF000000) >> 24);     
      
      SDHC_CMDARG =  base_rca;
      SDHC_XFERTYP =  MMC_CMD10;
      sdhc_init_state = GET_CARD_STATUS;
      break;
      
    }
    case(GET_CARD_STATUS):
    {
            
      SDHC_CMDARG =  base_rca;
      SDHC_XFERTYP =  MMC_CMD13;
      sdhc_init_state = SELECT_CARD;
      break;
      
    }
    case(SELECT_CARD):
    {
        
      SDHC_CMDARG =  base_rca;
      SDHC_XFERTYP =  MMC_CMD7;
      sdhc_init_state = SET_APP_SPECIFIC;
      break;
      
    }
    case(SET_APP_SPECIFIC):
    {
        
      SDHC_CMDARG = base_rca;
      SDHC_XFERTYP = SD_CMD55;
      sdhc_init_state = SET_BUS_WIDTH;
      break;

    }
#ifdef SD_LOCKER
    case(SET_BUS_WIDTH):
    {  
      
      SDHC_CMDARG =  0x2;//bus_width;
      SDHC_XFERTYP =  SD_ACMD6;
      sdhc_init_state = SEND_CSD;
      break;

    } 
    case(SEND_CSD):
    {
      
      uint8_t crctable[256],tcrc = 0;
      volatile uint32_t card_capacity = 0;
      volatile SD_CSD csd;
      
      //GenerateCRCTable(&crctable[0]);
              
      // generate a table value for all 256 possible byte values
      for (int i = 0; i < 256; i++)
      {
          crctable[i] = (i & 0x80) ? i ^ CRC7_POLY : i;
          for (int j = 1; j < 8; j++)
          {
              crctable[i] <<= 1;
              if (crctable[i] & 0x80)
              {
                  crctable[i] ^= CRC7_POLY;
              }
          }
          
       }
      
      //data[0] = (uint8_t) ((pSDHC->CMDRSP[3]  & 0xFF000000) >> 24);
      /* CSD Version 2.0 */
      csd.sturcture = ((data[0] & 0xC0) >> 6);
      csd.taac = data[1];
      csd.nsac = data[2];
      csd.tran_spd = data[3];
      csd.ccc = (((data[4] << 8) | (data[5] & 0xF0)) >> 4);
      csd.read_bl_len = (data[5] & 0xF);
      csd.read_bl_partial = (data[6] & 0x80) >> 7;
      csd.write_blk_misalign = (data[6] & 0x40) >> 6;
      csd.read_blk_misalign = (data[6] & 0x20) >> 5;
      csd.dsr_imp = (data[6] & 0x10) >> 4;
      csd.c_size = ((data[7] & 0x3F) << 16) | ((data[8] & 0xFF) << 8) | (data[9] & 0xFF);     
      csd.erase_blk_en = (data[10] & 0x40) >> 6;
      csd.sector_size = ((data[10] & 0x3F) << 1) | ((data[11] & 0x8) >> 7);
      csd.wp_grp_size = (data[11] & 0x7F);
      csd.wp_grp_enable = ((data[12] & 0x80) >> 7);
      csd.r2w_factor = ((data[12] & 0x1C) >> 2);
      csd.write_bl_len = ((data[12] & 0x3) << 2) | ((data[13] & 0xC0) >> 6) ;
      csd.write_bl_partital = ((data[13] & 0x20) >> 5);
      csd.file_format_grp = ((data[14] & 0x80) >> 7);
      csd.copy = ((data[14] & 0x40) >> 6);
      csd.perm_write_protect = ((data[14] & 0x20) >> 5);
      csd.tmp_write_protect = ((data[14] & 0x10) >> 4);
      csd.file_format = ((data[14] & 0xC) >> 2);
     
      card_capacity = (csd.c_size + 1) * 512000;

      data[14] ^= 0x10;
      
      for (int i = 0; i<15; i++)
      {
        tcrc = crctable[(tcrc << 1) ^ data[i]];
      }
      
      data[15] = (tcrc << 1) | 0x1;      
      
      SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1)| SDHC_BLKATTR_BLKSIZE(0x10);
      // Clear Command Complete bit
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK; 
      SDHC_DSADDR = (uint32_t)&data[0];
      SDHC_CMDARG =  0;
      SDHC_XFERTYP =  MMC_CMD27 |
                      SDHC_XFERTYP_DPSEL_MASK |
                      SDHC_XFERTYP_AC12EN_MASK |
                      SDHC_XFERTYP_BCEN_MASK |
                      SDHC_XFERTYP_DMAEN_MASK;
      sdhc_init_state = WRITE_TEST;
      break;
    } 
#else
    case(SET_BUS_WIDTH):
    {  
      
      SDHC_CMDARG =  0x2;//bus_width;
      SDHC_XFERTYP =  SD_ACMD6;
      sdhc_init_state = WRITE_TEST;
      break;

    }
#endif
    case (WRITE_TEST):
    {
      
      /* time stamp data */
      time = RTC_TSR;
      uint32_t sector = 2049;
      
      for (int i = 4; i < sizeof(data); i++)
      {

        data[i] = 0xFF;

      }
      
      data[0] = (uint8_t) (time & 0x000000FF);
      data[1] = (uint8_t) ((time & 0x0000FF00) >> 8);
      data[2] = (uint8_t) ((time & 0x00FF0000) >> 16);
      data[3] = (uint8_t) ((time & 0xFF000000) >> 24);
      SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1)| SDHC_BLKATTR_BLKSIZE(0x200);
      // Clear Command Complete bit
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK; 
      SDHC_DSADDR = (uint32_t)&data[0];
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP =  MMC_CMD24 |
                      SDHC_XFERTYP_DPSEL_MASK |
                      SDHC_XFERTYP_AC12EN_MASK |
                      SDHC_XFERTYP_BCEN_MASK |
                      SDHC_XFERTYP_DMAEN_MASK;
      sdhc_init_state = READ_TEST;
      break;
        
    }
    case (READ_TEST):
    {
      
      uint32_t sector = 2049;
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK;  
      memset(&data[0],0x00,512); 
      SDHC_DSADDR = (uint32_t)&data[0];	
      /* read block */
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP = MMC_CMD17 |
                     SDHC_XFERTYP_DPSEL_MASK |
                     SDHC_XFERTYP_DTDSEL_MASK |
                     SDHC_XFERTYP_AC12EN_MASK |
                     SDHC_XFERTYP_BCEN_MASK |
                     SDHC_XFERTYP_DMAEN_MASK;
      sdhc_init_state = TEST_COMPLETE;
      break;
        
    }
    case (TEST_COMPLETE):
    {
      
      uint8_t test_fail = 0;
      
      if ((data[0] != (uint8_t) (time & 0x000000FF)) ||
          (data[1] != (uint8_t) ((time & 0x0000FF00) >> 8)) ||
          (data[2] != (uint8_t) ((time & 0x00FF0000) >> 16)) ||
          (data[3] != (uint8_t) ((time & 0xFF000000) >> 24)))
      {
        
          test_fail = 1;
          
      }
      else
      {

        for (int i = 4; i < sizeof(data); i++)
        {

          if (data[i] != 0xFF)
          {
            
            test_fail = 1;
            break;
            
          } 

        }
        
      }
      
      if (test_fail == 1)
      {
        
        put_event(&evt_tbl,INIT_MANAGER);
        current_therapy_settings.sdhc.card_fault = 1;
        NVIC_DisableIRQ(SDHC_IRQn);                    
        
      }
      else
      {
        
        sdhc_init_state = READ_VERSION;
        put_event(&evt_tbl,SDHC_DOWNLOAD);
        
      }
      
      break;
        
    }
    case (READ_VERSION):
    {
      
      uint32_t sector = 2048;
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK;  
      memset(&data[0],0xFF,512); 
      SDHC_DSADDR = (uint32_t)&data[0];	
      /* read block */
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP = MMC_CMD17 |
                     SDHC_XFERTYP_DPSEL_MASK |
                     SDHC_XFERTYP_DTDSEL_MASK |
                     SDHC_XFERTYP_AC12EN_MASK |
                     SDHC_XFERTYP_BCEN_MASK |
                     SDHC_XFERTYP_DMAEN_MASK;
      sdhc_init_state = FIELD_UPGRADE_CHECK;
      break;
        
    }
    case(FIELD_UPGRADE_CHECK):
    {
      
      uint16_t data_cal_crc = 0, 
               data_crc = 0;
      
      uint8_t gsfw_update = 0,
              uifw_major = 0,
              uifw_minor = 0;
      
      /* calculate checksum */
      data_cal_crc = fast_crc16(data_cal_crc,
                           &data[0], 
                           sizeof(data)-2); 
      
      data_crc = ((data[511] << 8) | data[510]);
      
      if ((data_cal_crc == data_crc) &&
          (data_crc != 0))
      { 
        
        gsfw_update = check4gsfw(data[30],data[31]); //For K26 shifting 2 bytes for both UI & GS
        uifw_major = data[14];
        uifw_minor = data[15];
      
      }

      if (gsfw_update)
      {
        
        current_therapy_settings.sdhc.card_ready = 1;
        
      }
      
      current_therapy_settings.sdhc.uifw_major = uifw_major;
      current_therapy_settings.sdhc.uifw_minor = uifw_minor;
      current_therapy_settings.sdhc.gsfw_update_avaliable = gsfw_update;
      current_therapy_settings.sdhc.logging_ready = 1;
      
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      
      memset(&data[0],0xFF,512); 
      
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK;  
      SDHC_DSADDR = (uint32_t)&data[0];	
      /* read block */
      SDHC_CMDARG =  (uint32_t)1044612;
      SDHC_XFERTYP = MMC_CMD17 |
                     SDHC_XFERTYP_DPSEL_MASK |
                     SDHC_XFERTYP_DTDSEL_MASK |
                     SDHC_XFERTYP_AC12EN_MASK |
                     SDHC_XFERTYP_BCEN_MASK |
                     SDHC_XFERTYP_DMAEN_MASK;
      sdhc_init_state = LOGGING_CHECK;
      
      break;
      
    }
    case (LOGGING_CHECK):
    {

      uint16_t data_cal_crc = 0, 
               data_crc = 0;
      uint32_t log_sector =  data[0] |
                            (data[1] << 8) |
                            (data[2] << 16) |
                            (data[3] << 24);
       
      /* calculate checksum */
      data_cal_crc = fast_crc16(data_cal_crc,
                                &data[0], 
                                sizeof(data)-2);
      
      data_crc = ((data[511] << 8) | data[510]);
      
      if ((log_sector == 0) || 
          (log_sector >= CIRCLE_LOG_END_SECTOR) ||
          (data_cal_crc != data_crc))
      {
        
        put_event(&evt_tbl,INIT_MANAGER);
        current_therapy_settings.sdhc.card_fault = 1;
        NVIC_DisableIRQ(SDHC_IRQn); 
        
      }
      else 
      { 
        
        card_ready = 1;
        current_therapy_settings.sdhc.card_detected = 1;
        
      }
      
      
      sdhc_init_state = CMD0_GO_IDLE;
     
      break;
        
    }    
  default :
     {
        //do nothing
     }
     
    
  }
    
 return card_ready;
 
}

/***************************************************************************//**
Initializes the SD card state machine
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t sd_card_rw(void)
{
  
  sd_card_log(0,(SDHC_LOGGING_CMDS)0,0);
  
  return 0;
  
}
/***************************************************************************//**
Writes to the SD card. This function sends out the commands and the data to the
SD card.
@param reset - resets the SD card state machine
cmd - SD card commands
p_data - pointer to data that has to be written to the SD card
@return 0
@outputs
@context
*******************************************************************************/
uint8_t sd_card_log(uint8_t reset,SDHC_LOGGING_CMDS cmd, uint8_t *p_data)
{
  
  static uint8_t sdhc_log_data[512],wand_log_data[4];
  static uint16_t circ_idx = 0;
  static uint32_t sector = 0;
  static SDHC_LOGGING_CMDS sdhc_logging_state,
                           log_cmd;
  static uint8_t log_attempts = 0;
    
  if (reset == 1)
  {
        
    log_cmd = cmd;
    sdhc_logging_state = READ_STATIC;
    circ_idx = 0;
    sector = 0;
    for (int i = 0; i < sizeof(sdhc_log_data); i++)
    {

      sdhc_log_data[i] = 0;

    }
    
    for (int i = 0; i < sizeof(wand_log_data); i++)
    {
        
      wand_log_data[i] = *p_data++;
        
    }
    
  }
  
  if ((current_therapy_settings.sdhc.card_detected == 0) ||
      (log_attempts > 2) ||
      (current_therapy_settings.sdhc.card_fault == 1))
  {
    
    sdhc_logging_state = CARD_NOT_PRESENT;
    
  }
      
  
   
  switch(sdhc_logging_state)
  {
  
    case (READ_STATIC):
    {


      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK;  
      SDHC_DSADDR = (uint32_t)&sdhc_log_data[0];	
      /* read block */
      SDHC_CMDARG =  (uint32_t)1044612;
      SDHC_XFERTYP = MMC_CMD17 |
                     SDHC_XFERTYP_DPSEL_MASK |
                     SDHC_XFERTYP_DTDSEL_MASK |
                     SDHC_XFERTYP_AC12EN_MASK |
                     SDHC_XFERTYP_BCEN_MASK |
                     SDHC_XFERTYP_DMAEN_MASK;
      sdhc_logging_state++;
      break;
        
    }
    case (READ_CIRCLE):
    {

      sector =  sdhc_log_data[0] |
                (sdhc_log_data[1] << 8) |
                (sdhc_log_data[2] << 16) |
                (sdhc_log_data[3] << 24);
      circ_idx = (sdhc_log_data[4] |
                 (sdhc_log_data[5] << 8));
      
      circ_idx &= 0x1FF;
      
      if ((sector <= 0) || (sector >= CIRCLE_LOG_END_SECTOR))
      {
        
        //sector = 1044613;
        circ_idx = 0;
        if (++log_attempts > 2)
        {
          current_therapy_settings.notify.status2 |= GENERIC_ERROR_2_STATUS_2_MASK;
        }
        //put_event(&evt_tbl,MFG_DATE_TIME_FAULT);
        break;
        
      }
      else
      {
        log_attempts = 0;
      }
                          
      for (int i = 0; i < sizeof(sdhc_log_data); i++)
      {

        sdhc_log_data[i] = 0;

      }

      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK;  
      SDHC_DSADDR = (uint32_t)&sdhc_log_data[0];	
      /* read block */
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP = MMC_CMD17 |
                     SDHC_XFERTYP_DPSEL_MASK |
                     SDHC_XFERTYP_DTDSEL_MASK |
                     SDHC_XFERTYP_AC12EN_MASK |
                     SDHC_XFERTYP_BCEN_MASK |
                     SDHC_XFERTYP_DMAEN_MASK;
      sdhc_logging_state=log_cmd;
      break;
        
    }
    case (STATIC_LOG_ERROR):
    {
      
      /* time stamp data */
      uint32_t time = RTC_TSR;

      uint8_t cmd_bytes = 11;
      
      if ((circ_idx + cmd_bytes) >= 512)
      {
              
        sector++;
        circ_idx = 0;
        for (int i = 0; i < sizeof(sdhc_log_data); i++)
        {

          sdhc_log_data[i] = 0;

        }
              
      }      
            
      if (sector >= CIRCLE_LOG_END_SECTOR)
      {
        
        //sector = 1044613;
        circ_idx = 0;
        /* skip to max attempts so logging is disabled */
        log_attempts = 3;
        current_therapy_settings.notify.status2 |= GENERIC_ERROR_2_STATUS_2_MASK;
        //put_event(&evt_tbl,MFG_DATE_TIME_FAULT);
        break;
                
      }

      sdhc_log_data[circ_idx] = (uint8_t) (time & 0x000000FF); 
      sdhc_log_data[circ_idx+1] = (uint8_t) ((time >> 8) & 0x000000FF);
      sdhc_log_data[circ_idx+2] = (uint8_t) ((time >> 16) & 0x000000FF);
      sdhc_log_data[circ_idx+3] = (uint8_t) ((time >> 24) & 0x000000FF);

      sdhc_log_data[circ_idx+4] = serial_number[6];
      sdhc_log_data[circ_idx+5] = serial_number[7];
      sdhc_log_data[circ_idx+6] = serial_number[8];
      sdhc_log_data[circ_idx+7] = serial_number[9];
      
      /* embed cmd in record */
      sdhc_log_data[circ_idx+8] = (uint8_t) log_cmd;

      sdhc_log_data[circ_idx+9] = cmd_bytes;      
      sdhc_log_data[circ_idx+10] = (current_therapy_settings.notify.status & 0xFF);
                   
      // Clear Command Complete bit
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK; 
      SDHC_DSADDR = (uint32_t)&sdhc_log_data[0];
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP =  MMC_CMD24 |
                      SDHC_XFERTYP_DPSEL_MASK |
                      SDHC_XFERTYP_AC12EN_MASK |
                      SDHC_XFERTYP_BCEN_MASK |
                      SDHC_XFERTYP_DMAEN_MASK;
      sdhc_logging_state = WRITE_STATIC;
      break;
      

      
    }
    case (STATIC_LOG_THERAPY_DATA):
    {
            
      /* time stamp data */
      uint32_t end_time = RTC_TSR;

      uint8_t cmd_bytes = 23;
            
      if ((circ_idx + cmd_bytes) >= 512)
      {
              
        sector++;
        circ_idx = 0;
        for (int i = 0; i < sizeof(sdhc_log_data); i++)
        {

        sdhc_log_data[i] = 0;

        }
              
      }      
            
      if (sector >= CIRCLE_LOG_END_SECTOR)
      {
        
        //sector = 1044613;
        circ_idx = 0;
        /* skip to max attempts so logging is disabled */
        log_attempts = 3;
        current_therapy_settings.notify.status2 |= GENERIC_ERROR_2_STATUS_2_MASK;
        //put_event(&evt_tbl,MFG_DATE_TIME_FAULT);
        
      }

      sdhc_log_data[circ_idx] = (uint8_t) (end_time & 0x000000FF); 
      sdhc_log_data[circ_idx+1] = (uint8_t) ((end_time >> 8) & 0x000000FF);
      sdhc_log_data[circ_idx+2] = (uint8_t) ((end_time >> 16) & 0x000000FF);
      sdhc_log_data[circ_idx+3] = (uint8_t) ((end_time >> 24) & 0x000000FF);

      sdhc_log_data[circ_idx+4] = serial_number[6];
      sdhc_log_data[circ_idx+5] = serial_number[7];
      sdhc_log_data[circ_idx+6] = serial_number[8];
      sdhc_log_data[circ_idx+7] = serial_number[9];
      
      /* embed cmd in record */
      sdhc_log_data[circ_idx+8] = (uint8_t) log_cmd;

      sdhc_log_data[circ_idx+9] = cmd_bytes;      
      
      sdhc_log_data[circ_idx+10] = smart_wand_params.mfg.final_assy[0];
      sdhc_log_data[circ_idx+11] = smart_wand_params.mfg.final_assy[1];
      sdhc_log_data[circ_idx+12] = smart_wand_params.mfg.final_assy[2];
            
      sdhc_log_data[circ_idx+13] = smart_wand_params.mfg.work_order[0];
      sdhc_log_data[circ_idx+14] = smart_wand_params.mfg.work_order[1];
      sdhc_log_data[circ_idx+15] = smart_wand_params.mfg.work_order[2];
      sdhc_log_data[circ_idx+16] = smart_wand_params.mfg.work_order[3];

      sdhc_log_data[circ_idx+17] = smart_wand_params.mfg.unique_id[0];
      sdhc_log_data[circ_idx+18] = smart_wand_params.mfg.unique_id[1];
      
      sdhc_log_data[circ_idx+19] = wand_log_data[0];
      sdhc_log_data[circ_idx+20] = wand_log_data[1];
      sdhc_log_data[circ_idx+21] = wand_log_data[2];
      sdhc_log_data[circ_idx+22] = wand_log_data[3];
                        
      // Clear Command Complete bit
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK; 
      SDHC_DSADDR = (uint32_t)&sdhc_log_data[0];
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP =  MMC_CMD24 |
                      SDHC_XFERTYP_DPSEL_MASK |
                      SDHC_XFERTYP_AC12EN_MASK |
                      SDHC_XFERTYP_BCEN_MASK |
                      SDHC_XFERTYP_DMAEN_MASK;
      sdhc_logging_state = WRITE_STATIC;
      break;
        
    }
    case (STATIC_LOG_LEGACY_THERAPY_DATA):
    {
      
      /* time stamp data */
      uint32_t end_time = RTC_TSR;

      uint8_t cmd_bytes = 12;
      
      if ((circ_idx + cmd_bytes) >= 512)
      {
              
        sector++;
        circ_idx = 0;
        for (int i = 0; i < sizeof(sdhc_log_data); i++)
        {

        sdhc_log_data[i] = 0;

        }
              
      }      
            
      if (sector >= CIRCLE_LOG_END_SECTOR)
      {
        
        //sector = 1044613;
        circ_idx = 0;
        put_event(&evt_tbl,MFG_DATE_TIME_FAULT);
        
      }
      
      sdhc_log_data[circ_idx] = (uint8_t) (end_time & 0x000000FF); 
      sdhc_log_data[circ_idx+1] = (uint8_t) ((end_time >> 8) & 0x000000FF);
      sdhc_log_data[circ_idx+2] = (uint8_t) ((end_time >> 16) & 0x000000FF);
      sdhc_log_data[circ_idx+3] = (uint8_t) ((end_time >> 24) & 0x000000FF);

      /* embed cmd in record */
      sdhc_log_data[circ_idx+4] = (uint8_t) log_cmd;

      sdhc_log_data[circ_idx+5] = cmd_bytes;      
      sdhc_log_data[circ_idx+6] = (uint8_t) (legacy_wand_params_obj.R1 & 0x00FF);
      sdhc_log_data[circ_idx+7] = (uint8_t) ((legacy_wand_params_obj.R1 >> 8) & 0x00FF);
      sdhc_log_data[circ_idx+8] = (uint8_t) (legacy_wand_params_obj.R2 & 0x00FF);
      sdhc_log_data[circ_idx+9] = (uint8_t) ((legacy_wand_params_obj.R2 >> 8) & 0x00FF);
      sdhc_log_data[circ_idx+10] = 
        (current_therapy_settings.legacy_wand_levels.legacy_wand_coag_level & 0xF);
      sdhc_log_data[circ_idx+10] |= 
        ((current_therapy_settings.legacy_wand_levels.legacy_wand_ablate_level & 0xF) << 4);
      sdhc_log_data[circ_idx+11] =
        (current_therapy_settings.log.legacy_ablate_on & 0x1);
      sdhc_log_data[circ_idx+11] |= 
        ((current_therapy_settings.log.legacy_coag_on & 0x1) << 1);
                   
      // Clear Command Complete bit
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK; 
      SDHC_DSADDR = (uint32_t)&sdhc_log_data[0];
      SDHC_CMDARG =  sector;
      SDHC_XFERTYP =  MMC_CMD24 |
                      SDHC_XFERTYP_DPSEL_MASK |
                      SDHC_XFERTYP_AC12EN_MASK |
                      SDHC_XFERTYP_BCEN_MASK |
                      SDHC_XFERTYP_DMAEN_MASK;
      sdhc_logging_state = WRITE_STATIC;      
      break;
      
    }
    case (WRITE_STATIC):
    {
     
      uint16_t data_cal_crc = 0;
      
      circ_idx += sdhc_log_data[circ_idx+9];
      
      for (int i = 0; i < sizeof(sdhc_log_data); i++)
      {

        sdhc_log_data[i] = 0;

      }
            
      sdhc_log_data[0] = (uint8_t) (sector & 0x000000FF); 
      sdhc_log_data[1] = (uint8_t) ((sector >> 8) & 0x000000FF);
      sdhc_log_data[2] = (uint8_t) ((sector >> 16) & 0x000000FF);
      sdhc_log_data[3] = (uint8_t) ((sector >> 24) & 0x000000FF);
      sdhc_log_data[4] = (uint8_t) (circ_idx & 0x00FF);
      sdhc_log_data[5] = (uint8_t) ((circ_idx >> 8) & 0x00FF);
            
      /* calculate checksum */
      data_cal_crc = fast_crc16(data_cal_crc,
                                &sdhc_log_data[0], 
                                sizeof(sdhc_log_data)-2);
      
      sdhc_log_data[510] = (uint8_t)(data_cal_crc & 0x00FF);
      sdhc_log_data[511] = (uint8_t)((data_cal_crc & 0xFF00) >> 8);
    
      // Clear Command Complete bit
      SDHC_IRQSTATEN = SDHC_IRQSTATEN_TCSEN_MASK;
      // Clear Command Complete bit
      SDHC_IRQSTAT = 0xFFFFFFFF;
      SDHC_IRQSIGEN = SDHC_IRQSIGEN_TCIEN_MASK; 
      SDHC_DSADDR = (uint32_t)&sdhc_log_data[0];
      SDHC_CMDARG =  (uint32_t)1044612;
      SDHC_XFERTYP =  MMC_CMD24 |
                      SDHC_XFERTYP_DPSEL_MASK |
                      SDHC_XFERTYP_AC12EN_MASK |
                      SDHC_XFERTYP_BCEN_MASK |
                      SDHC_XFERTYP_DMAEN_MASK;
      sdhc_logging_state++;
      break;
        
    }
    case(LOGGING_COMPLETE):
    {
      
      sdhc_logging_state = READ_STATIC;
      break;
      
    }
  default:
     {
        // do nothing
     }
     
    
  }
  
  return 0;
  
}
/***************************************************************************//**
This function is called from the main app to write to the SD card. This function
is used to log errors to the SD card.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t log2sdhc(void)
{
  
  static uint8_t previous_notify_state = 0;
  uint8_t idx = 0;
  uint8_t notify_state = (current_therapy_settings.notify.status & 0xFF);
    
  if ((notify_state != previous_notify_state) &&
      (notify_state > 0))
  {
    
    current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][0] = STATIC_LOG_ERROR;
    current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][1] = (SDHC_LOGGING_CMDS)0;
    current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][2] = (SDHC_LOGGING_CMDS)0;
    current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][3] = (SDHC_LOGGING_CMDS)0;
    current_therapy_settings.sdhc.cmd_que[current_therapy_settings.sdhc.cmd_que_idx][4] = (SDHC_LOGGING_CMDS)0;
    
    current_therapy_settings.sdhc.cmd_que_idx++;
    
  }
  
  previous_notify_state = notify_state;
  idx = current_therapy_settings.sdhc.cmd_que_idx;
  
  if (current_therapy_settings.sdhc.cmd_que_idx > 0)
  {
    
    sd_card_log(1,current_therapy_settings.sdhc.cmd_que[idx-1][0],
                  (uint8_t*)&current_therapy_settings.sdhc.cmd_que[idx-1][1]);
    current_therapy_settings.sdhc.cmd_que_idx--;
    
  }
  
  return 0;
  
}

uint8_t sd_card_init_callback(void)
{
  
  uint8_t card_ready = 0;
  
  card_ready = sd_card_init(0);
  
  return card_ready;
  
}