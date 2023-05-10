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
\file      cpu.h
\brief     Header file for CPU driver
\details   Generic high-level routines for ARM Cortex M4 Processor.
*******************************************************************************/

#ifndef CPU_H
#define CPU_H
#include <stdint.h>

/* Includes ------------------------------------------------------- */
#include "MK26F18.h"
#ifdef DEBUG
  #include <stdio.h>
#endif

/* Exported Macros & Defines --------------------------------------- */
#ifndef NULL
#define NULL	(0)
#endif

/* number of levels that can be represented using a 12bit number */
/* #define MAX_12BIT_NUM                   (1 << 12) */

#define U8_MAX     (255)            //0xFF
#define S8_MAX     (127)
#define S8_MIN     (-128)
#define U16_MAX    (65535u)         //0xFFFF
#define S16_MAX    (32767)
#define S16_MIN    (-32768)
#define U32_MAX    (4294967295uL)   //0xFFFFFFFF
#define S32_MAX    (2147483647)
#define S32_MIN    (-2147483648uL)

#define ABS(a)         (((a) < 0) ? (-(a)) : (a))
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))


/* Endianness adjustment utility */
/* Note this utility can be replaced by uint32_t __rev16(uint32_t) from NEON */
#define swap_4bytes(ptr)                {uint8_t byte;          \
					byte = (ptr)[0];        \
                                        (ptr)[0] = (ptr)[3];    \
                                        (ptr)[3]=byte;          \
					 byte = (ptr)[1];       \
                                        (ptr)[1] = (ptr)[2];    \
                                        (ptr)[2]=byte;          \
                                        }

#define _ENABLE_ALL_INTERRUPTS           asm(" CPSIE i")
#define _DISABLE_ALL_INTERRUPTS          asm(" CPSID i")
#define _DO_NOTHING                      asm("nop")

#ifdef DEBUG
  #define ASSERT(expr)                            \
    if (!(expr))                                  \
      assert_failed(__FILE__, __LINE__)
#else
  #define ASSERT(expr)
#endif

/* Common data types */
/* typedef enum {FALSE = 0, TRUE = !FALSE} bool; */
typedef enum {FUNC_ERROR = 0, FUNC_SUCCESS = !FUNC_ERROR} ErrorStatus;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, BitStatus, BitAction, StateAction;
typedef enum {DISABLED = 0, ENABLE = !DISABLED} FunctionalState;
/* typedef char * STRING; */

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;

/* Function Prototypes */
void Write_Vtor(int);
void enable_irq(uint8_t);
void disable_irq(uint8_t);
uint8_t system_reset(void);

#endif
