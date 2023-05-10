/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2813                 $:
$Date:: 2022-02-16 08:40:23#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      start.c
\brief     Start up module
\details   Start up routines before branching to main application.
NOTE:
See Microcontroller Software Consideration notes.
http://www.freescale.com/infocenter/topic/microcontrollers/index.html
*******************************************************************************/

#include "MK26F18.h"
#include "start.h"
#include "cpu.h"
#include "crc.h"
#include "system.h"
#include "wdog.h"
#include "mcg.h"
#include "psu.h"
#include "pit.h"
#include <stdlib.h>
#include <yfuns.h>
#include "init.h"
#include "isr.h"
#include "adc.h"
#include "fpu.h"
#include "fuse_blow.h"
#include "systick.h"

#include "data_init3.h"

#include "../../48386/lib/lib.h"

#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam"

extern void CDC_Init(void);
extern void cbtmr_init(void);

__root extern const uint8_t checksum_end[4] @ "checksum_end_mark" = {0,0,0,0xEE};

/***************************************************************************//**
Start up code which is executed before branching to main.
@todo More description
@param void
@return 0
@outputs
@context
*******************************************************************************/
void start(void)
{
  uint16_t calc_audio = 0;
  uint32_t n = 0;
      extern uint8_t __VECTOR_RAM2[];
  uint8_t * p_invect = (uint8_t*)&__vector_table2[0];
  
  /* disable all interrupts */
  asm(" CPSID i");
  
  /* disable watchdog */
  wdog_disable();
     
  /* calculate checksum for audio files in rom -- takes ~400ms to complete */
  calc_audio = fast_crc16(calc_audio,
                         (uint8_t *)&__checksum_begin, 
                         (uint32_t)&__checksum_end - (uint32_t)&__checksum_begin + 1);
  
  //calc = slow_crc16(calc, zeros, 2);        
  
  /* copy rom data to ram */
  __iar_data_init3();
  //rom2ram();


  /* Copy the vector table to RAM */
  for (n = 0; n < 0x410; n++)
      __VECTOR_RAM2[n] = *p_invect++;
  /* Point the VTOR to the new copy of the vector table */
  Write_Vtor((uint32_t)__VECTOR_RAM2);
  
  if (__checksum != calc_audio)
  {
    
    put_event(&evt_tbl,CRC_FAULT);
    
  }
////  else
////  {
////    
////    /* kick-off state machine normally */
////    put_event(&evt_tbl,POLL_PWR_SWITCH);
////  
////  }
  
  //Write_Vtor((uint32_t)&__vector_table2);
  /* enable port clocks */
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK |
                SIM_SCGC5_PORTB_MASK |
                SIM_SCGC5_PORTC_MASK |
                SIM_SCGC5_PORTD_MASK |
                SIM_SCGC5_PORTE_MASK );
  
  /* Initialize LEDs on pcb*/
  PORTA_PCR24 = PORT_PCR_MUX(1); // GPIO, RED LED
  PORTA_PCR25 = PORT_PCR_MUX(1); // GPIO, YELLOW LED
  PORTA_PCR26 = PORT_PCR_MUX(1); // GPIO, BLUE LED
  PORTE_PCR6 = PORT_PCR_MUX(1); // EXT_CTRL PIN 2 (J73)
  PORTE_PCR7 = PORT_PCR_MUX(1); // EXT_CTRL PIN 3 (J73)
  PORTB_PCR9 = PORT_PCR_MUX(1) | 
               PORT_PCR_PFE_MASK | 
               PORT_PCR_PE_MASK | 
               PORT_PCR_PS_MASK; // J16

  /* set pcb LED pins as outputs */
  GPIOA_PDDR |= GPIO_PDDR_PDD(RED_LED | BLUE_LED | YELLOW_LED);
  GPIOE_PDDR |= GPIO_PDDR_PDD(EXT_CTRL_2 | EXT_CTRL_3);
  
  /* turn pcb LEDs OFF */
  GPIOA_PSOR |= GPIO_PDOR_PDO(RED_LED | BLUE_LED | YELLOW_LED);
  //GPIOA_PCOR |= GPIO_PDOR_PDO(BLUE_LED);
  GPIOE_PSOR |= GPIO_PDOR_PDO(EXT_CTRL_2 | EXT_CTRL_3);
  
  /* Configure port pins as inputs */
  GPIOB_PDDR &= ~GPIO_PDDR_PDD(GPIO_PIN(9));
  
  /*Make sure the fuse blow line remains low */
  clear_fuse_blow();
    
  /* configure clock */ 
  PLL_Init();
  cbtmr_init(); 
  
  /* config i/o to ctrl power supply */
  psu_init();
  
  /* initialize periodic interrupt timer for general use */
  pit_init();


  /* copy audio data references to sram */
  /* For an application, use library_p to access the entry struct */
  //struct lib_func *library_p = (struct lib_func *)LIB_ENTRY_ADDR;
  //library_p->init();
  
  //lib_init();
      
  /* initialize watchdog */
  wdog_enable(); 
  
  /* turn yellow LED OFF */
  //GPIOA_PTOR |= YELLOW_LED;
  
  /* enable all interrupts */
  asm(" CPSIE i");
    
  adc_init_2();
  
  /* initial step to enable fpu co-processor*/
  MCM_ISCR = MCM_ISCR_FIDCE_MASK |
             MCM_ISCR_FIXCE_MASK |
             MCM_ISCR_FUFCE_MASK |
             MCM_ISCR_FOFCE_MASK |
             MCM_ISCR_FDZCE_MASK;

  #ifndef TEST 
  /* final step to enable the fpu co-processor */
  __enable_FPU();
  #endif    
          
  /* enable 1 volt bandgap */
  PMC_REGSC |= PMC_REGSC_BGBE_MASK;
  
  // USB CDC Initialization
  CDC_Init();
  
  /* start application */
  main();
  
  /* disable all interrupts */
  asm(" CPSID i");

  /* halt system */
  exit(EXIT_FAILURE);
  
}

/***************************************************************************//**
Copy all ROM data to RAM.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void rom2ram(void) {

#if (defined(CW))	
    extern char __START_BSS[];
    extern char __END_BSS[];
    extern uint32_t __DATA_ROM[];
    extern uint32_t __DATA_RAM[];
    extern char __DATA_END[];
#endif

  /* Declare a counter we'll use in all of the copy loops */
  uint32_t n;

  /* Declare pointers for various data sections. These pointers
   * are initialized using values pulled in from the linker file
   */
  uint8_t * data_ram, * data_rom, * data_rom_end;
  uint8_t * bss_start, * bss_end;

  /* Addresses for VECTOR_TABLE and VECTOR_RAM come from the linker file */
//#ifdef DEBUG_CODE
//  extern uint32_t __VECTOR_TABLE[];
//  extern uint8_t __VECTOR_RAM2[];
//  uint8_t * p_invect = (uint8_t*)&__vector_table2[0];

  /* Copy the vector table to RAM */
//  if (__VECTOR_RAM != __VECTOR_TABLE)
//  {
//      for (n = 0; n < 0x410; n++)
//          __VECTOR_RAM2[n] = *p_invect++;
  //}
  /* Point the VTOR to the new copy of the vector table */
//  Write_Vtor((uint32_t)__VECTOR_RAM2);
//#else
  
//#endif

  /* Get the addresses for the .data section (initialized data section) */
  #if (defined(CW))
    data_ram = (uint8_t *)__DATA_RAM;
    data_rom = (uint8_t *)__DATA_ROM;
    data_rom_end  = (uint8_t *)__DATA_END; /* This is actually a RAM address in CodeWarrior */
    n = data_rom_end - data_ram;
  #elif (defined(IAR))
    data_ram = __section_begin(".data");
    data_rom = __section_begin(".data_init");
    data_rom_end = __section_end(".data_init");
    n = data_rom_end - data_rom;
  #endif		
              
  /* Copy initialized data from ROM to RAM */
  while (n--)
    *data_ram++ = *data_rom++;
           
  /* Get the addresses for the .bss section (zero-initialized data) */
  #if (defined(CW))
    bss_start = (uint8_t *)__START_BSS;
    bss_end = (uint8_t *)__END_BSS;
  #elif (defined(IAR))
    bss_start = __section_begin(".bss");
    bss_end = __section_end(".bss");
  #endif

  /* Clear the zero-initialized data section */
  n = bss_end - bss_start;
  while(n--)
  *bss_start++ = 0;
  
  /* Get addresses for any code sections that need to be copied from ROM to RAM.
   * The IAR tools have a predefined keyword that can be used to mark individual
   * functions for execution from RAM. Add "__ramfunc" before the return type in
   * the function prototype for any routines you need to execute from RAM instead
   * of ROM. ex: __ramfunc void foo(void);
   */
  #if (defined(IAR))
    uint8_t* code_relocate_ram = __section_begin("CodeRelocateRam");
    uint8_t* code_relocate = __section_begin("CodeRelocate");
    uint8_t* code_relocate_end = __section_end("CodeRelocate");
  
    /* Copy functions from ROM to RAM */
    n = code_relocate_end - code_relocate;
    while (n--)
      *code_relocate_ram++ = *code_relocate++;
  #endif
    
  //Write_Vtor((uint32_t)&__vector_table[0]);
    //Write_Vtor((uint32_t)&__vector_table2);
}
/*****************************************************************************/

//// RWPI data base address must be set in R9 before ropi_rwpi_main() is called, and at this address a sufficient amount of memory must be reserved.
//int ropi_rwpi_main()
//{
//  
//  //__iar_data_init3();   // IAR data initialization function, normally called from __cmain() at startup
//   
//  start();
//  
//  return 1;
//  
//}

void __enable_FPU(void)
{
//  #define VFP_Enable	0x00F00000
//  asm("MOVW	R1, #60808");
//  asm("MOVT	R1, #57344		// CPACR base");
//  asm("LDR  R0, [R1]");
//  //asm("ORR.W	R0, R0, #15728640");
//  asm("ORR       R1,R1,#(0xF << 20)");
//  asm("STR	R0, [R1]");
//  asm("BX	LR");
      
asm("LDR.W   R0, =0xE000ED88");
asm("LDR     R1, [R0]");
asm("ORR     R1, R1, #(0xF << 20)");
asm("STR     R1, [R0]");

}
