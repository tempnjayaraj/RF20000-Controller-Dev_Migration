

#include "ftfe.h"
#include "system.h"
#include <string.h>
#include <intrinsics.h>
#include "nv_data.h"
#include "crc.h"

__no_init uint8_t eeram[4096] @ 0x14000000;

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
    
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);
  
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
  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);
  
  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}



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
      
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);
  
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
  
  /* wait for cmd complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);
  
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


uint8_t erase_ftfe_sector(uint32_t destination)        
{

  uint8_t error = 0;
  
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
     
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();  
  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                     
  
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
  
    /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 

  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}

uint8_t write_ftfe_phrase(uint32_t destination,uint8_t *ptr)        
{

  uint8_t error = 0;
  uint8_t phrase[8] = {0,0,0,0,0,0,0,0};

  for (int i = 0; i < 8; i++)
  {
    
    phrase[i] = *ptr++;
    
  }
    
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
     
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();   
  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                  
  
  /* clear any errors */
  FTFE_FSTAT = FTFE_FSTAT_FPVIOL_MASK |
               FTFE_FSTAT_ACCERR_MASK |
               FTFE_FSTAT_RDCOLERR_MASK; 

  /* write one-time programmable in p-flash 0 IFR */
  FTFE_FCCOB0 = FTFE_PROGRAM_PHRASE;
  FTFE_FCCOB1 = ((destination >> 16) & 0xFF);
  FTFE_FCCOB2 = ((destination >> 8) & 0xFF);
  FTFE_FCCOB3 = (destination & 0xFF);
  FTFE_FCCOB4 = phrase[3];
  FTFE_FCCOB5 = phrase[2];
  FTFE_FCCOB6 = phrase[1];
  FTFE_FCCOB7 = phrase[0];
  FTFE_FCCOB8 = phrase[7];
  FTFE_FCCOB9 = phrase[6];
  FTFE_FCCOBA = phrase[5];
  FTFE_FCCOBB = phrase[4];
  
  /* initiate command */
  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK;
  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 

  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}

uint8_t write_ftfe_section(uint32_t destination,uint16_t nwords)        
{

  uint8_t error = 0;
      
  /* enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
  
    /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();   
  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0);                  
  
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
  
  /* wait for previous cmd to complete */
  while(( FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK) == 0); 

  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  /* check for errors */
  error = ((FTFE_FSTAT & FTFE_ERR_FLAGS_MASK) != 0);

  return error;

}

uint8_t save_jump_address(uint32_t address)
{
  
  uint16_t nwords;
  nonvolatile_data_layout *p_ee_data = (nonvolatile_data_layout*)&eeram[0];
  uint8_t *p_flash_data = (uint8_t *)FLASH_DATA_PTR;
  uint16_t nv_data_crc = 0,
           nbytes = 0;
        
  /* copy program nonvolatile variables to eeram */
  memset(&eeram[0],0xFF,0x560);
  
  for (int i = 0; i < sizeof(eeram); i++)
  {
    
    eeram[i] = *p_flash_data++;
    
  }
  
  /* more than 255 or 0 means no data resides in nv mem */
  if ((p_ee_data->nwords > 255) || (p_ee_data->nwords == 0))
  {
    
    /* add 15 so resulting 128-bit words rounds up */
    nwords = (sizeof(nonvolatile_data_layout) + 15) >> 4;
    
  }
  else
  {
    
    nwords = p_ee_data->nwords;
    
  }
  
  /* 128-bit words */
  nbytes = nwords << 4;
  
  //memcpy(&eeram[0],(uint8_t*)NONVOLATILE_DATA_ADDRESS,sizeof(nonvolatile_data_layout));
  p_ee_data->active_app_ptr = address;
  p_ee_data->nwords = nwords;
    
  /* calculate checksum */
  p_ee_data->checksum = fast_crc16(nv_data_crc,
                                   &eeram[0]+2, 
                                   nbytes-2);
  
  erase_ftfe_sector(NONVOLATILE_DATA_ADDRESS);
  write_ftfe_section(NONVOLATILE_DATA_ADDRESS,64);
  
  //memset(&eeram[1025],0x22,1);
  memcpy(&eeram[0],&eeram[1024],352);
  write_ftfe_section((NONVOLATILE_DATA_ADDRESS+0x400),32);
  
  return 1;
  
}


