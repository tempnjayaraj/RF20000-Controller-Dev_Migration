/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              start.c
Author:            Chris Wallis
Date:              01-FEB-2013
Modified:          
CPU:               Freescale Kinetis P/N MK20DN512ZVLQ10 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments:
Start up routines before branching to main application.

NOTE:
See Microcontroller Software Consideration notes.
http://www.freescale.com/infocenter/topic/microcontrollers/index.html

*******************************************************************************/

#include "MK26F18.h"
#include "start.h"
#include "system.h"
#include "crc.h"
#include "ftfe.h"
#include "../../51677/c/head.h"
#include "nv_data.h"
#include <string.h>

volatile __no_init uint32_t* rwpi_data @ R9;

nonvolatile_data_layout *p_flash_data = (nonvolatile_data_layout*)NONVOLATILE_DATA_ADDRESS;

void main(void)
{
  
  uint32_t jmp_ptr, jmp_address;
  program_header_layout *pheader;
  //uint8_t flash_data[8];
  
  jmp_ptr = calc_jmp_ptr();
  pheader = (program_header_layout*)jmp_ptr;
    
  /* strobe lights */
  GPIOA_PCOR = RED_LED | BLUE_LED | YELLOW_LED;


  /* disable all interrupts */
  asm(" CPSID i");

  save_jump_address(jmp_ptr);

  /* set rwpi base address */
  rwpi_data = (uint32_t*)(0x1FFF0410);
  //__set_SB((uint32_t*)(0x1FFF0410));

  /* calculate jump address for application */
  jmp_address = (uint32_t)(jmp_ptr+pheader->start_offset+sizeof(program_header_layout));

  /* jump to application */
  asm volatile ("bx %0" : : "r" (jmp_address));
            
  
}

uint32_t calc_jmp_ptr(void)
{
  
  uint32_t jmp_ptr_lookup[2][2] = {0,
                                   APP2_ADDRESS,
                                   APP1_ADDRESS,
                                   1};
  uint32_t jmp_ptr = 0;
  uint8_t app1_valid = 0,
          app2_valid = 0,
          app1_major = 0,
          app1_minor = 0,
          app2_major = 0,
          app2_minor = 0;
    
  app1_valid = search4valid_app(APP1_ADDRESS, &app1_major, &app1_minor);
  app2_valid = search4valid_app(APP2_ADDRESS, &app2_major, &app2_minor);
  
  jmp_ptr = jmp_ptr_lookup[app1_valid][app2_valid];
  
//  app1_major = p_flash_data->app1_version_major;
//  app1_minor = p_flash_data->app1_version_minor;
//  app2_major = p_flash_data->app2_version_major;
//  app2_minor = p_flash_data->app2_version_minor;
  
  if ((app1_major == 0xFF) &&
      (app1_minor == 0xFF))
  {
    
    app1_major = 0;
    app1_minor = 0;
    
  }
  
  if ((app2_major == 0xFF) &&
      (app2_minor == 0xFF))
  {
    
    app2_major = 0;
    app2_minor = 0;
    
  }
  
  if (jmp_ptr == 1)
  {
    
    if (p_flash_data->active_app_ptr == APP1_ADDRESS)
    {
      
      if (app2_major > app1_major)
      {
        
        jmp_ptr = APP2_ADDRESS;
        
      }
      else if ((app2_major == app1_major) &&
               (app2_minor > app1_minor))
      {
        
        jmp_ptr = APP2_ADDRESS;
        
      }
      else
      {
        
        jmp_ptr = APP1_ADDRESS;
        
      }
      
    }
    else
    {
      
      if (app1_major > app2_major)
      {
        
        jmp_ptr = APP1_ADDRESS;
        
      }
      else if ((app1_major == app2_major) &&
               (app1_minor > app2_minor))
      {
        
        jmp_ptr = APP1_ADDRESS;
        
      }
      else
      {
        
        jmp_ptr = APP2_ADDRESS;
        
      }
      
    }
    
  }
  
  return jmp_ptr;
    
}


uint8_t search4valid_app(uint32_t app_ptr, uint8_t* pmajor, uint8_t* pminor)
{
  
  program_header_layout *pheader = (program_header_layout*) app_ptr; 
  uint16_t *p_app_checksum = (uint16_t*)(app_ptr - 4);
  uint8_t *p_app_checksum_begin = (uint8_t*)(app_ptr - 2);
  uint8_t app_valid = 0;
  uint16_t calc_checksum = 0;
  
  if (memcmp(&pheader->app_name[0],"RF20000P",8)==0)
  {
    
//    app_checksum = ((pheader->checksum[1] << 8) |
//                    (pheader->checksum[0]));
    
    /* calculate checksum for main app in rom -- takes ~400ms to complete */
    calc_checksum = fast_crc16(calc_checksum,
                               p_app_checksum_begin,
                               pheader->ropi_bytes + 2);

    /* compare the calculated checksum with the stored */
//    if (app_checksum == calc_checksum)
    if (*p_app_checksum == calc_checksum)
    {

      app_valid = 1;
      *pmajor = pheader->app_version_major;
      *pminor = pheader->app_version_minor;
      
    }
    
  }
  
  return app_valid;
  
}
  