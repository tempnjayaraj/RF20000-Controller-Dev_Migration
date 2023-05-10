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
\file      isr.h
\brief     Header file for interrupt service routines
*******************************************************************************/

/*******************************************************************************
Comments: 
Flash configuration fields at end of this file. Please be careful when modifying
since these can secure the flash (possibly permanently).  CONFIG_4 flash option
byte has been modified to disable the EZPORT so NMI is not trigger during boot.
rf20000.board has been changed to write this modification to the flash option
byte in memory.  If configuring for the 1st time, the power will need to be
cycled to fully impliment this change.  

See video @ "$PROJ_DIR$\docs\IAR Flash Loader.7z" for further reference.

*******************************************************************************/

#ifndef ISR_H
#define ISR_H

/* Function prototypes */
//#include "usb.h"
extern void usb_isr(void);
void default_isr(void);
void nmi_isr(void);
//void dma9_isr(void);
//void dma12_isr(void);
void dma13_isr(void);
//void dma30_isr(void);
//void portb_isr(void);
void pit1_isr(void);
void pit2_isr(void);
void pit3_isr(void);
void ftm0_isr(void);
void can0_mb_isr(void);
void uart0_isr(void);
void adc0_isr(void);
void lpt_isr(void);
void pdb_isr(void);
void ftm3_isr(void);
void adc1_isr(void);
//void adc2_isr(void);
void uart4_isr(void);
//void dma23_isr(void);
//void pit0_isr(void);
void rtc_isr(void);
void rtc_sec_isr(void);
void i2c0_isr(void);
void i2c1_isr(void);
void pit0_isr(void);
void sdhc_isr(void);
void systick_isr(void);
void uart5_isr(void);
void spi0_isr (void);


void hard_fault_handler_c(unsigned int * hardfault_args);
void hard_fault_handler_asm(void);

/* Interrupt Vector Table Function Pointers */
typedef void pointer(void);
typedef void (*vector_entry)(void);

extern vector_entry  __vector_table2[];
extern void start(void);
//extern void __startup(void);

extern unsigned long __BOOT_STACK_ADDRESS[];
extern void __iar_program_start(void);
                                        // Address     Vector IRQ   Source module   Source description
#define VECTOR_000      (pointer*)__BOOT_STACK_ADDRESS	//          ARM core        Initial Supervisor SP
#define VECTOR_001      start	// 0x0000_0004 1 -          ARM core        Initial Program Counter
#define VECTOR_002      nmi_isr     // 0x0000_0008 2 -          ARM core        Non-maskable Interrupt (NMI)
#define VECTOR_003      hard_fault_handler_asm     // 0x0000_000C 3 -          ARM core        Hard Fault
#define VECTOR_004      default_isr     // 0x0000_0010 4 -
#define VECTOR_005      default_isr     // 0x0000_0014 5 -          ARM core         Bus Fault
#define VECTOR_006      default_isr     // 0x0000_0018 6 -          ARM core         Usage Fault
#define VECTOR_007      default_isr     // 0x0000_001C 7 -                           
#define VECTOR_008      default_isr     // 0x0000_0020 8 -                           
#define VECTOR_009      default_isr     // 0x0000_0024 9 -
#define VECTOR_010      default_isr     // 0x0000_0028 10 -
#define VECTOR_011      default_isr     // 0x0000_002C 11 -         ARM core         Supervisor call (SVCall)
#define VECTOR_012      default_isr     // 0x0000_0030 12 -         ARM core         Debug Monitor
#define VECTOR_013      default_isr     // 0x0000_0034 13 -                          
#define VECTOR_014      default_isr     // 0x0000_0038 14 -         ARM core         Pendable request for system service (PendableSrvReq)
#define VECTOR_015      systick_isr     // 0x0000_003C 15 -         ARM core         System tick timer (SysTick)
#define VECTOR_016      default_isr        // 0x0000_0040 16     0     DMA              DMA Channel 0 transfer complete
#define VECTOR_017      default_isr     // 0x0000_0044 17     1     DMA              DMA Channel 1 transfer complete
#define VECTOR_018      default_isr     // 0x0000_0048 18     2     DMA              DMA Channel 2 transfer complete
#define VECTOR_019      default_isr     // 0x0000_004C 19     3     DMA              DMA Channel 3 transfer complete
#define VECTOR_020      default_isr     // 0x0000_0050 20     4     DMA              DMA Channel 4 transfer complete
#define VECTOR_021      default_isr     // 0x0000_0054 21     5     DMA              DMA Channel 5 transfer complete
#define VECTOR_022      default_isr     // 0x0000_0058 22     6     DMA              DMA Channel 6 transfer complete
#define VECTOR_023      default_isr     // 0x0000_005C 23     7     DMA              DMA Channel 7 transfer complete
#define VECTOR_024      default_isr     // 0x0000_0060 24     8     DMA              DMA Channel 8 transfer complete
#define VECTOR_025      default_isr        // 0x0000_0064 25     9     DMA              DMA Channel 9 transfer complete
#define VECTOR_026      default_isr     // 0x0000_0068 26    10     DMA              DMA Channel 10 transfer complete
#define VECTOR_027      default_isr     // 0x0000_006C 27    11     DMA              DMA Channel 11 transfer complete
#define VECTOR_028      default_isr     // 0x0000_0070 28    12     DMA              DMA Channel 12 transfer complete
#define VECTOR_029      dma13_isr       // 0x0000_0074 29    13     DMA              DMA Channel 13 transfer complete
#define VECTOR_030      default_isr     // 0x0000_0078 30    14     DMA              DMA Channel 14 transfer complete
#define VECTOR_031      default_isr     // 0x0000_007C 31    15     DMA              DMA Channel 15 transfer complete
#define VECTOR_032      default_isr     // 0x0000_0080 32    16     DMA              DMA Error Interrupt Channels 0-15
#define VECTOR_033      default_isr     // 0x0000_0084 33    17     MCM              Normal interrupt
#define VECTOR_034      default_isr     // 0x0000_0088 34    18     Flash memory     Command Complete
#define VECTOR_035      default_isr     // 0x0000_008C 35    19     Flash memory     Read Collision
#define VECTOR_036      default_isr     // 0x0000_0090 36    20     Mode Controller  Low Voltage Detect,Low Voltage Warning, Low Leakage Wakeup
#define VECTOR_037      default_isr     // 0x0000_0094 37    21     LLWU
#define VECTOR_038      default_isr     // 0x0000_0098 38    22     WDOG
#define VECTOR_039      default_isr     // 0x0000_009C 39    23	    RNGB
#define VECTOR_040      i2c0_isr        // 0x0000_00A0 40    24     I2C0
#define VECTOR_041      i2c1_isr        // 0x0000_00A4 41    25     I2C1
#define VECTOR_042      spi0_isr     // 0x0000_00A8 42    26     SPI0             Single interrupt vector for all sources
#define VECTOR_043      default_isr     // 0x0000_00AC 43    27     SPI1             Single interrupt vector for all sources
#define VECTOR_044      default_isr     // 0x0000_00B0 44    28     SPI2             Single interrupt vector for all sources
#define VECTOR_045      default_isr
#define VECTOR_046      default_isr
#define VECTOR_047      uart0_isr
#define VECTOR_048      default_isr
#define VECTOR_049      default_isr
#define VECTOR_050      default_isr
#define VECTOR_051      default_isr
#define VECTOR_052      default_isr
#define VECTOR_053      default_isr
#define VECTOR_054      default_isr
#define VECTOR_055      adc0_isr
#define VECTOR_056      default_isr
#define VECTOR_057      default_isr
#define VECTOR_058      ftm0_isr
#define VECTOR_059      default_isr
#define VECTOR_060      default_isr
#define VECTOR_061      default_isr
#define VECTOR_062      rtc_isr
#define VECTOR_063      rtc_sec_isr
#define VECTOR_064      pit0_isr
#define VECTOR_065      pit1_isr
#define VECTOR_066      pit2_isr
#define VECTOR_067      pit3_isr
#define VECTOR_068      pdb_isr
#define VECTOR_069      usb_isr
#define VECTOR_070      default_isr
#define VECTOR_071      default_isr
#define VECTOR_072      default_isr
#define VECTOR_073      default_isr
#define VECTOR_074      lpt_isr
#define VECTOR_075      default_isr
#define VECTOR_076      default_isr
#define VECTOR_077      default_isr
#define VECTOR_078      default_isr
#define VECTOR_079      default_isr
#define VECTOR_080      default_isr
#define VECTOR_081      default_isr
#define VECTOR_082      uart4_isr
#define VECTOR_083      default_isr
#define VECTOR_084      default_isr
#define VECTOR_085      default_isr
#define VECTOR_086      default_isr
#define VECTOR_087      ftm3_isr
#define VECTOR_088      default_isr
#define VECTOR_089      adc1_isr
#define VECTOR_090      default_isr
#define VECTOR_091      can0_mb_isr
#define VECTOR_092      default_isr
#define VECTOR_093      default_isr
#define VECTOR_094      default_isr
#define VECTOR_095      default_isr
#define VECTOR_096      default_isr
#define VECTOR_097      sdhc_isr
#define VECTOR_098      default_isr
#define VECTOR_099      default_isr
#define VECTOR_100      default_isr
#define VECTOR_101      default_isr
#define VECTOR_102      uart5_isr
#define VECTOR_103      default_isr
#define VECTOR_104      default_isr
#define VECTOR_105      default_isr
#define VECTOR_106      default_isr
#define VECTOR_107      default_isr
#define VECTOR_108      default_isr
#define VECTOR_109      default_isr
#define VECTOR_110      default_isr
#define VECTOR_111      default_isr
#define VECTOR_112      default_isr
#define VECTOR_113      default_isr
#define VECTOR_114      default_isr
#define VECTOR_115      default_isr
#define VECTOR_116      default_isr     // 0x0000_01D0 116   100
#define VECTOR_117      default_isr        // 0x0000_01D4 117   101
#define VECTOR_118      default_isr     // 0x0000_01D8 118   102   ADC2
#define VECTOR_119      default_isr        // 0x0000_01DC 119   103   ADC3
#define VECTOR_120      default_isr     // 
#define VECTOR_121      default_isr     // 
#define VECTOR_122      default_isr     // 
#define VECTOR_123      default_isr     // 
#define VECTOR_124      default_isr     // 
#define VECTOR_125      default_isr     // 
#define VECTOR_126      default_isr     // 
#define VECTOR_127      default_isr     // 
#define VECTOR_128      default_isr     // 
#define VECTOR_129      default_isr     // 
#define VECTOR_130      default_isr     // 
#define VECTOR_131      default_isr     // 
#define VECTOR_132      default_isr     // 
#define VECTOR_133      default_isr     // 
#define VECTOR_134      default_isr     // 
#define VECTOR_135      default_isr     // 
#define VECTOR_136      default_isr     // 
#define VECTOR_137      default_isr     // 
#define VECTOR_138      default_isr     // 
#define VECTOR_139      default_isr     // 
#define VECTOR_140      default_isr     // 
#define VECTOR_141      default_isr     // 
#define VECTOR_142      default_isr     // 
#define VECTOR_143      default_isr     // 
#define VECTOR_144      default_isr     // 
#define VECTOR_145      default_isr     // 
#define VECTOR_146      default_isr     // 
#define VECTOR_147      default_isr     // 
#define VECTOR_148      default_isr     // 
#define VECTOR_149      default_isr     // 
#define VECTOR_150      default_isr     // 
#define VECTOR_151      default_isr     // 
#define VECTOR_152      default_isr     // 
#define VECTOR_153      default_isr     // 
#define VECTOR_154      default_isr     // 
#define VECTOR_155      default_isr     // 
#define VECTOR_156      default_isr     // 
#define VECTOR_157      default_isr     // 
#define VECTOR_158      default_isr     // 
#define VECTOR_159      default_isr     // 
#define VECTOR_160      default_isr     // 
#define VECTOR_161      default_isr     // 
#define VECTOR_162      default_isr     // 
#define VECTOR_163      default_isr     // 
#define VECTOR_164      default_isr     // 
#define VECTOR_165      default_isr     // 
#define VECTOR_166      default_isr     // 
#define VECTOR_167      default_isr     // 
#define VECTOR_168      default_isr     // 
#define VECTOR_169      default_isr     // 
#define VECTOR_170      default_isr     // 
#define VECTOR_171      default_isr     // 
#define VECTOR_172      default_isr     // 
#define VECTOR_173      default_isr     // 
#define VECTOR_174      default_isr     // 
#define VECTOR_175      default_isr     // 
#define VECTOR_176      default_isr     // 
#define VECTOR_177      default_isr     // 
#define VECTOR_178      default_isr     // 
#define VECTOR_179      default_isr     // 
#define VECTOR_180      default_isr     // 
#define VECTOR_181      default_isr     // 
#define VECTOR_182      default_isr     // 
#define VECTOR_183      default_isr     // 
#define VECTOR_184      default_isr     // 
#define VECTOR_185      default_isr     // 
#define VECTOR_186      default_isr     // 
#define VECTOR_187      default_isr     // 
#define VECTOR_188      default_isr     // 
#define VECTOR_189      default_isr     // 
#define VECTOR_190      default_isr     // 
#define VECTOR_191      default_isr     // 
#define VECTOR_192      default_isr     // 
#define VECTOR_193      default_isr     // 
#define VECTOR_194      default_isr     // 
#define VECTOR_195      default_isr     // 
#define VECTOR_196      default_isr     // 
#define VECTOR_197      default_isr     // 
#define VECTOR_198      default_isr     // 
#define VECTOR_199      default_isr     // 
#define VECTOR_200      default_isr     // 
#define VECTOR_201      default_isr     // 
#define VECTOR_202      default_isr     // 
#define VECTOR_203      default_isr     // 
#define VECTOR_204      default_isr     // 
#define VECTOR_205      default_isr     // 
#define VECTOR_206      default_isr     // 
#define VECTOR_207      default_isr     // 
#define VECTOR_208      default_isr     // 
#define VECTOR_209      default_isr     // 
#define VECTOR_210      default_isr     // 
#define VECTOR_211      default_isr     // 
#define VECTOR_212      default_isr     // 
#define VECTOR_213      default_isr     // 
#define VECTOR_214      default_isr     // 
#define VECTOR_215      default_isr     // 
#define VECTOR_216      default_isr     // 
#define VECTOR_217      default_isr     // 
#define VECTOR_218      default_isr     // 
#define VECTOR_219      default_isr     // 
#define VECTOR_220      default_isr     // 
#define VECTOR_221      default_isr     // 
#define VECTOR_222      default_isr     // 
#define VECTOR_223      default_isr     // 
#define VECTOR_224      default_isr     // 
#define VECTOR_225      default_isr     // 
#define VECTOR_226      default_isr     // 
#define VECTOR_227      default_isr     // 
#define VECTOR_228      default_isr     // 
#define VECTOR_229      default_isr     // 
#define VECTOR_230      default_isr     // 
#define VECTOR_231      default_isr     // 
#define VECTOR_232      default_isr     // 
#define VECTOR_233      default_isr     // 
#define VECTOR_234      default_isr     // 
#define VECTOR_235      default_isr     // 
#define VECTOR_236      default_isr     // 
#define VECTOR_237      default_isr     // 
#define VECTOR_238      default_isr     // 
#define VECTOR_239      default_isr     // 
#define VECTOR_240      default_isr     // 
#define VECTOR_241      default_isr     // 
#define VECTOR_242      default_isr     // 
#define VECTOR_243      default_isr     // 
#define VECTOR_244      default_isr     // 
#define VECTOR_245      default_isr     // 
#define VECTOR_246      default_isr     // 
#define VECTOR_247      default_isr     // 
#define VECTOR_248      default_isr     // 
#define VECTOR_249      default_isr     // 
#define VECTOR_250      default_isr     // 
#define VECTOR_251      default_isr     // 
#define VECTOR_252      default_isr     // 
#define VECTOR_253      default_isr     // 
#define VECTOR_254      default_isr     // 
#define VECTOR_255      default_isr     // 

/* Flash option byte configuration values */
#define CONFIG_1	(pointer*)0xffffffff 
#define CONFIG_2	(pointer*)0xffffffff 
#define CONFIG_3	(pointer*)0xffffffff
#define CONFIG_4	(pointer*)0xfffffffe

#endif /*__VECTORS_H*/

/* End of "vectors.h" */
