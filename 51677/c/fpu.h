/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: cwalli00          $:
$Rev:: 2794                 $:
$Date:: 2021-11-04 10:49:01#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     fpu.h
\brief     Header file for floating point unit
\details   N/A
*******************************************************************************/

#ifndef __FPU_H_
#define __FPU_H_

//#define SCB_BASE	0xE000ED00


// Structure type to access the SCB registers
typedef struct _scb_type_regs
{
	unsigned long cpuid;	// 0x00
	unsigned long icsr;	// 0x04
	unsigned long vtor;	// 0x08
	unsigned long aircr;	// 0x0C
	unsigned long scr;	// 0x10
	unsigned long ccr;	// 0x14
	unsigned long shpr1;	// 0x18
	unsigned long shpr2;	// 0x1C
	unsigned long shpr3;	// 0x20
	unsigned long shcsr;	// 0x24
	unsigned long cfsr;	// 0x28
	unsigned long hfsr;	// 0x2C
	unsigned long dfsr;	// 0x30
	unsigned long mmfar;	// 0x34
	unsigned long bfar;	// 0x38
	unsigned long afsr;	// 0x3C
	unsigned long rsvd[22];	// 0x40 - 0x87
	unsigned long cpacr;	// 0x88
} scb_type;

typedef struct _scb_type_fp_regs
{
	unsigned long fpccr;	// 0x00
	unsigned long fpcar;	// 0x04
	unsigned long fpdscr;	// 0x08
	unsigned long mvfr0;	// 0x0C
	unsigned long mvfr1;	// 0x10
} scb_fb_type;

#define FPSCR_N		0x80000000
#define FPSCR_Z		0x40000000
#define FPSCR_C		0x20000000
#define FPSCR_V		0x10000000
#define FPSCR_AHP	0x04000000	// Alternative half-precision
#define FPSCR_DN	0x02000000	// Default NaN mode
#define FPSCR_FZ	0x01000000	// Flush-to-zero mode
#define FPSCR_RM_MASK	0xFF3FFFFF	// Rounding Mode mask
#define FPSCR_RM_RN	0x00000000	// Round to nearest
#define FPSCR_RM_RP	0x00400000	// Round towards Plus infinity
#define FPSCR_RM_RM	0x00800000	// Round towards minus infinity
#define FPSCR_RM_RZ	0x00C00000	// Round towards Zero
#define FPSCR_IDC	0x00000080	// Input Denormal cumalative exception
#define FPSCR_IXC	0x00000010	// Inexact cumulative
#define FPSCR_UFC	0x00000008	// Underflow cumulative
#define FPSCR_OFC	0x00000004	// Overflow cumulative
#define FPSCR_DZC	0x00000002	// Division by Zero cumulative
#define FPSCR_IOC	0x00000001	// Invalid Operation cumulative

void __enable_FPU(void);
//__intrinsic unsigned long __get_FPSCR(void);
//__intrinsic void __set_FPSCR(unsigned long value);
//__intrinsic unsigned long __vmov_fptogp(float value);
//__intrinsic unsigned long __vnull(void);

void __vmov_scalar(void);
void __vmov_fp_regs(void);
void __vmrs_vmsr(void);
void __vpush_vpop(void);
void __vstr_vldr(void);
void __vstm_vldm(void);
void __vcvt_test(void);
void __vabs_test(void);
void __vneg_test(void);
void __vcmp_test(void);
void __vsqrt_test(void);
void __vmla_vmls(void);
void __vnmla_vnmls_vnmul(void);
void __vmul_test(void);
void __vadd_test(void);
void __vsub_test(void);
void __vdiv_test(void);
void __fcvt_issue_test(void);

void print_float(float value);

void fpu_instructions(void);
int fcvt_main(double d);
int fpinfo2_main(void);
int chkinexact_main(void);

#endif
