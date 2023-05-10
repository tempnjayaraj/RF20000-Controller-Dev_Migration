/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              crt0.s
Author:            Chris Wallis
Date:              01-FEB-2013
Modified:          
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments:
Initialize processor general purpose registers (R0-R12) to zero.

NOTE:
See Microcontroller Software Consideration notes.
http://www.freescale.com/infocenter/topic/microcontrollers/index.html

*******************************************************************************/

;         AREA   Crt0, CODE, READONLY      ; name this block of code



        SECTION .noinit : CODE       
  	EXPORT  __startup
__startup

    MOV     r0,#0                   ; Initialize the GPRs
	MOV     r1,#0
	MOV     r2,#0
	MOV     r3,#0
	MOV     r4,#0
	MOV     r5,#0
	MOV     r6,#0
	MOV     r7,#0
	MOV     r8,#0
	MOV     r9,#0
	MOV     r10,#0
	MOV     r11,#0
	MOV     r12,#0
	CPSID   i                       ; mask all interrupts
        import start
        BL      start                  ; call the C code
__done
        B       __done


        END
