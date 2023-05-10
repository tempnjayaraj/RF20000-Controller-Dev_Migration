#define VFP_Enable	0x00F00000

	SECTION .noinit : CODE

	EXPORT	__enable_FPU
	EXPORT	__get_FPSCR
	EXPORT	__set_FPSCR

	EXPORT	__vmov_fptogp
	EXPORT	__vnull

	EXPORT	__vmov_scalar
	EXPORT	__vmov_fp_regs
	EXPORT	__vmrs_vmsr
	EXPORT	__vpush_vpop
	EXPORT	__vstr_vldr
	EXPORT	__vstm_vldm
	EXPORT	__vcvt_test
	EXPORT	__vabs_test
	EXPORT	__vneg_test
	EXPORT	__vcmp_test
	EXPORT	__vsqrt_test
	EXPORT	__vmla_vmls
	EXPORT	__vnmla_vnmls_vnmul
	EXPORT	__vmul_test
	EXPORT	__vadd_test
	EXPORT	__vsub_test
	EXPORT	__vdiv_test
	EXPORT	__fcvt_issue_test

__enable_FPU:
	movw	r1, #60808
	movt	r1, #57344		// CPACR base
	ldr	r0, [r1]
	orr.w	r0, r0, #VFP_Enable
	str	r0, [r1]
	bx	lr

__get_FPSCR:
	vmrs	r0, FPSCR
	bx	lr

__set_FPSCR:
	vmsr	FPSCR, r0
	bx	lr

// dummy function for test
__vnull:
	vmov.f32	s0, #1
	bx	lr

// FPU instruction function
__vmov_fptogp:
	vmov	r0, s0
	bx	lr

__vabs:
//	VABS 	r0, r1
	bx	lr

// test functions

__vmov_scalar:
	// r13 & r15 can't be used
	// a total of 16 64-bit doubleword
	movw	r0, #0x2848
	movt	r0, #0x3476
	movw	r1, #0x7592
	movt	r1, #0x9273
	movw	r2, #0x9524
	movt	r2, #0x2764
	movw	r3, #0x9592
	movt	r3, #0x2818
	movw	r4, #0x7828
	movt	r4, #0x0274
	movw	r5, #0x8914
	movt	r5, #0x1184
	movw	r6, #0x1734
	movt	r6, #0x0836
	movw	r7, #0x7746
	movt	r7, #0x0012
	movw	r8, #0x2372
	movt	r8, #0x0003
	movw	r9, #0x5234
	movt	r9, #0x3348
	movw	r10, #0x5868
	movt	r10, #0x4029
	movw	r11, #0x9579
	movt	r11, #0x5205
	movw	r12, #0x0696
	movt	r12, #0x6265

	vmov.f32	D0[0], r0
	vmov.f32	D0[1], r1
	vmov.f32	D1[0], r2
	vmov.f32	D1[1], r3
	vmov.f32	D2[0], r4
	vmov.f32	D2[1], r5
	vmov.f32	D3[0], r6
	vmov.f32	D3[1], r7
	vmov.f32	D4[0], r8
	vmov.f32	D4[1], r9
	vmov.f32	D5[0], r10
	vmov.f32	D5[1], r11
	vmov.f32	D6[0], r12
	vmov.f32	D6[1], r14
	vmov.f32	D7[0], r13
	vmov.f32	D7[1], r15
	vmov.f32	D8[0], r0
	vmov.f32	D8[1], r1
	vmov.f32	D9[0], r2
	vmov.f32	D9[1], r3
	vmov.f32	D10[0], r4
	vmov.f32	D10[1], r5
	vmov.f32	D11[0], r6
	vmov.f32	D11[1], r7
	vmov.f32	D12[0], r7
	vmov.f32	D12[1], r6
	vmov.f32	D13[0], r5
	vmov.f32	D13[1], r4
	vmov.f32	D14[0], r3
	vmov.f32	D14[1], r2
	vmov.f32	D15[0], r1
	vmov.f32	D15[1], r0

	vmov.f32	r0, D0[0]
	vmov.f32	r1, D0[1]
	vmov.f32	r2, D1[0]
	vmov.f32	r3, D1[1]
	vmov.f32	r4, D2[0]
	vmov.f32	r5, D2[1]
	vmov.f32	r6, D3[0]
	vmov.f32	r7, D3[1]
	vmov.f32	r0, D4[0]
	vmov.f32	r1, D4[1]
	vmov.f32	r2, D5[0]
	vmov.f32	r3, D5[1]
	vmov.f32	r4, D6[0]
	vmov.f32	r5, D6[1]
	vmov.f32	r6, D7[0]
	vmov.f32	r7, D7[1]
	vmov.f32	r0, D8[0]
	vmov.f32	r1, D8[1]
	vmov.f32	r2, D9[0]
	vmov.f32	r3, D9[1]
	vmov.f32	r4, D10[0]
	vmov.f32	r5, D10[1]
	vmov.f32	r6, D11[0]
	vmov.f32	r7, D11[1]
	vmov.f32	r0, D12[0]
	vmov.f32	r1, D12[1]
	vmov.f32	r2, D13[0]
	vmov.f32	r3, D13[1]
	vmov.f32	r4, D14[0]
	vmov.f32	r5, D14[1]
	vmov.f32	r6, D15[0]
	vmov.f32	r7, D15[1]
	bx	lr

__vmov_fp_regs:
	// move from FPU to CPU
	vmov.f32	s0, r0
	vmov.f32	s1, r1
	vmov.f32	s2, r2
	vmov.f32	s3, r3
	vmov.f32	s4, r4
	vmov.f32	s5, r5
	vmov.f32	s6, r6
	vmov.f32	s7, r7
	vmov.f32	s8, r8
	vmov.f32	s9, r9
	vmov.f32	s10, r10
	vmov.f32	s11, r11
	vmov.f32	s12, r12
	vmov.f32	s13, r13
	vmov.f32	s14, r14
	vmov.f32	s15, r15
	vmov.f32	s16, r7
	vmov.f32	s17, r6
	vmov.f32	s18, r5
	vmov.f32	s19, r4
	vmov.f32	s20, r3
	vmov.f32	s21, r2
	vmov.f32	s22, r1
	vmov.f32	s23, r0
	vmov.f32	s24, r10
	vmov.f32	s25, r9
	vmov.f32	s26, r8
	vmov.f32	s27, r7
	vmov.f32	s28, r6
	vmov.f32	s29, r5
	vmov.f32	s30, r4
	vmov.f32	s31, r3

	vmov.f32	r0, s0
	vmov.f32	r1, s1
	vmov.f32	r2, s2
	vmov.f32	r3, s3
	vmov.f32	r4, s4
	vmov.f32	r5, s5
	vmov.f32	r6, s6
	vmov.f32	r7, s7
	vmov.f32	r0, s8
	vmov.f32	r1, s9
	vmov.f32	r2, s10
	vmov.f32	r3, s11
	vmov.f32	r4, s12
	vmov.f32	r5, s13
	vmov.f32	r6, s14
	vmov.f32	r7, s15
	vmov.f32	r0, s16
	vmov.f32	r1, s17
	vmov.f32	r2, s18
	vmov.f32	r3, s19
	vmov.f32	r4, s20
	vmov.f32	r5, s21
	vmov.f32	r6, s22
	vmov.f32	r7, s23
	vmov.f32	r0, s24
	vmov.f32	r1, s25
	vmov.f32	r2, s26
	vmov.f32	r3, s27
	vmov.f32	r4, s28
	vmov.f32	r5, s29
	vmov.f32	r6, s30
	vmov.f32	r7, s31
	bx	lr

__vmrs_vmsr:
	// vmrs
	vmrs	r0, fpscr
	vmrs	r1, fpscr
	vmrs	r2, fpscr
	vmrs	r3, fpscr
	vmrs	r4, fpscr
	vmrs	r5, fpscr
	vmrs	r6, fpscr
	vmrs	r7, fpscr
	vmrs	r8, fpscr
	vmrs	r9, fpscr
	vmrs	r10, fpscr
	vmrs	r11, fpscr
	vmrs	r12, fpscr

	mov	r3, r13
	mov	r4, r14
	vmrs	r13, fpscr
	vmrs	r14, fpscr
	mov	r13, r3
	mov	r14, r4

	mov	r5, r15
	add	r5, r5, #8
	vmrs	r15, fpscr
	mov	r15, r5

	// vmsr
	vmsr	fpscr, r15
	vmsr	fpscr, r1
	vmsr	fpscr, r2
	vmsr	fpscr, r3
	vmsr	fpscr, r4
	vmsr	fpscr, r5
	vmsr	fpscr, r6
	vmsr	fpscr, r7
	vmsr	fpscr, r8
	vmsr	fpscr, r9
	vmsr	fpscr, r10
	vmsr	fpscr, r11
	vmsr	fpscr, r12
	vmsr	fpscr, r13
	vmsr	fpscr, r14
	vmsr	fpscr, r0
	bx	lr

__vpush_vpop:
	vpush.32	{ s0-s31 }
	vpush.64	{ s0,s1 }
	vpop.64		{ s0,s1 }
	vpop.32		{ s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20,s21,s22,s23,s24,s25,s26,s27,s28,s29,s30,s31 }
	bx	lr

__vstr_vldr:
	// r13 & r15 can't be used
	// a total of 16 64-bit doubleword
	movw	r0, #0x0000
	movt	r0, #0x2000
	movw	r1, #0x0004
	movt	r1, #0x2000
	movw	r2, #0x0008
	movt	r2, #0x2000
	movw	r3, #0x000c
	movt	r3, #0x2000
	movw	r4, #0x0010
	movt	r4, #0x2000
	movw	r5, #0x0014
	movt	r5, #0x2000
	movw	r6, #0x0018
	movt	r6, #0x2000
	movw	r7, #0x001c
	movt	r7, #0x2000
	movw	r8, #0x0020
	movt	r8, #0x2000
	movw	r9, #0x0024
	movt	r9, #0x2000
	movw	r10, #0x0028
	movt	r10, #0x2000
	movw	r11, #0x002c
	movt	r11, #0x2000
	movw	r12, #1020
	movt	r12, #0x2000

	// vstr encoding T1, single
	vstr	s0, [r0]
	vstr	s1, [r1]
	vstr	s2, [r2]
	vstr	s3, [r3]
	vstr	s4, [r4]
	vstr	s5, [r5]
	vstr	s6, [r6]
	vstr	s7, [r7]
	vstr	s8, [r8]
	vstr	s9, [r9]
	vstr	s10, [r10]
	vstr	s11, [r11]
	vstr	s12, [r12]
	vstr	s13, [r0]
	vstr	s14, [r1]
	vstr	s15, [r2]
	vstr	s16, [r3]
	vstr	s17, [r4]
	vstr	s18, [r5]
	vstr	s19, [r6]
	vstr	s20, [r7]
	vstr	s21, [r8]
	vstr	s22, [r0]
	vstr	s23, [r1]
	vstr	s24, [r2]
	vstr	s25, [r3]
	vstr	s26, [r4]
	vstr	s27, [r5]
	vstr	s28, [r6]
	vstr	s29, [r7]
	vstr	s30, [r8]
	vstr	s31, [r9]

	// vstr encoding T1, single with offset
	vstr	s0, [r0,#8]
	vstr	s1, [r0,#80]
	vstr	s2, [r0,#128]
	vstr	s3, [r0,#256]
	vstr	s4, [r0,#512]
	vstr	s5, [r0,#768]
	vstr	s6, [r0,#1020]
	vstr	s7, [r0,#0]
	vstr	s8, [r1,#-4]
	vstr	s9, [r5,#-8]
	vstr	s10, [r12,#-128]
	vstr	s11, [r12,#-512]
	vstr	s12, [r12,#-1020]
	vstr	s13, [r8,#12]
	vstr	s14, [r3,#4]
	vstr	s15, [r7,#16]
	vstr	s16, [r11,#-16]
	vstr	s17, [r2,#20]
	vstr	s18, [r7,#-20]
	vstr	s19, [r1,#28]
	vstr	s20, [r2,#64]
	vstr	s21, [r3,#72]
	vstr	s22, [r3,#-8]
	vstr	s23, [r7,#-28]
	vstr	s24, [r10,#88]
	vstr	s25, [r11,#-40]
	vstr	s26, [r11,#-36]
	vstr	s27, [r12,#-768]
	vstr	s28, [r12,#-512]
	vstr	s29, [r12,#-500]
	vstr	s30, [r12,#-200]
	vstr	s31, [r12,#-100]

	// encoding T2, double overwritten
	vstr	d0, [r0]
	vstr	d1, [r1]
	vstr	d2, [r2]
	vstr	d3, [r3]
	vstr	d4, [r4]
	vstr	d5, [r5]
	vstr	d6, [r6]
	vstr	d7, [r7]
	vstr	d8, [r8]
	vstr	d9, [r9]
	vstr	d10, [r0]
	vstr	d11, [r2]
	vstr	d12, [r4]
	vstr	d13, [r6]
	vstr	d14, [r8]
	vstr	d15, [r10]

	vstr	d0, [r0, #8]
	vstr	d1, [r1, #80]
	vstr	d2, [r2, #256]
	vstr	d3, [r3, #512]
	vstr	d4, [r4, #768]
	vstr	d5, [r5, #1020]

	//
	// load
	// vstr encoding T1, single
	vldr	s0, [r0]
	vldr	s1, [r1]
	vldr	s2, [r2]
	vldr	s3, [r3]
	vldr	s4, [r4]
	vldr	s5, [r5]
	vldr	s6, [r6]
	vldr	s7, [r7]
	vldr	s8, [r8]
	vldr	s9, [r9]
	vldr	s10, [r10]
	vldr	s11, [r11]
	vldr	s12, [r12]
	vldr	s13, [r0]
	vldr	s14, [r1]
	vldr	s15, [r2]
	vldr	s16, [r3]
	vldr	s17, [r4]
	vldr	s18, [r5]
	vldr	s19, [r6]
	vldr	s20, [r7]
	vldr	s21, [r0]
	vldr	s22, [r1]
	vldr	s23, [r2]
	vldr	s24, [r3]
	vldr	s25, [r10]
	vldr	s26, [r9]
	vldr	s27, [r8]
	vldr	s28, [r7]
	vldr	s29, [r6]
	vldr	s30, [r5]
	vldr	s31, [r4]

	// vldr encoding T1, single with offset
	vldr	s0, [r0,#8]
	vldr	s1, [r0,#80]
	vldr	s2, [r0,#128]
	vldr	s3, [r0,#256]
	vldr	s4, [r0,#512]
	vldr	s5, [r0,#768]
	vldr	s6, [r0,#1020]
	vldr	s7, [r0,#0]
	vldr	s8, [r1,#-4]
	vldr	s9, [r5,#-8]
	vldr	s10, [r12,#-128]
	vldr	s11, [r12,#-512]
	vldr	s12, [r12,#-1020]
	vldr	s13, [r8,#12]
	vldr	s14, [r3,#4]
	vldr	s15, [r7,#16]
	vldr	s16, [r11,#-16]
	vldr	s17, [r2,#20]
	vldr	s18, [r7,#-20]
	vldr	s19, [r1,#28]
	vldr	s20, [r2,#64]
	vldr	s21, [r3,#72]
	vldr	s22, [r3,#-8]
	vldr	s23, [r7,#-28]
	vldr	s24, [r10,#88]
	vldr	s25, [r11,#-40]
	vldr	s26, [r11,#-36]
	vldr	s27, [r12,#-768]
	vldr	s28, [r12,#-512]
	vldr	s29, [r12,#-500]
	vldr	s30, [r12,#-200]
	vldr	s31, [r12,#-100]

	vldr	s0, [PC]
	vldr	s1, [PC]
	vldr	s2, [PC]
	vldr	s3, [PC]
	vldr	s4, [PC]
	vldr	s5, [PC]
	vldr	s6, [PC]
	vldr	s7, [PC]
	vldr	s8, [PC]
	vldr	s9, [PC]
	vldr	s10, [PC]
	vldr	s11, [PC]
	vldr	s12, [PC]
	vldr	s13, [PC]
	vldr	s14, [PC]
	vldr	s15, [PC]
	vldr	s16, [PC,#8]
	vldr	s17, [PC,#16]
	vldr	s18, [PC,#32]
	vldr	s19, [PC,#64]
	vldr	s20, [PC,#128]
	vldr	s21, [PC,#256]
	vldr	s22, [PC,#512]
	vldr	s23, [PC,#1020]
	vldr	s24, [PC,#-256]
	vldr	s25, [PC,#-128]
	vldr	s26, [PC,#-80]
	vldr	s27, [PC,#-36]
	vldr	s28, [PC,#-24]
	vldr	s29, [PC,#-12]
	vldr	s30, [PC,#-4]
	vldr	s31, [PC,#0]

	vldr	d0, [r0]
	vldr	d1, [r1]
	vldr	d2, [r2]
	vldr	d3, [r3]
	vldr	d4, [r4]
	vldr	d5, [r5]
	vldr	d6, [r6]
	vldr	d7, [r7]
	vldr	d8, [r8]
	vldr	d9, [r9]
	vldr	d10, [r0]
	vldr	d11, [r2]
	vldr	d12, [r4]
	vldr	d13, [r6]
	vldr	d14, [r8]
	vldr	d15, [r10]

	vldr	d0, [r0, #8]
	vldr	d1, [r1, #80]
	vldr	d2, [r2, #256]
	vldr	d3, [r3, #512]
	vldr	d4, [r4, #768]
	vldr	d5, [r5, #1020]
	vldr	d6, [r8, #-8]
	vldr	d7, [r12, #-80]
	vldr	d8, [r12, #-256]
	vldr	d9, [r12, #-512]
	vldr	d10, [r0, #100]
	vldr	d11, [r1, #200]
	vldr	d11, [r2, #400]
	vldr	d12, [r3, #800]
	
	vldr	d13, [r4, #-4]
	vldr	d14, [r4, #-8]
	vldr	d15, [r12, #-100]

	//vldr	s0, __vmov_scalar
	bx	lr

__vstm_vldm:
	movw	r0, #0x0000
	movt	r0, #0x2000
	movw	r1, #0x0080
	movt	r1, #0x2000
	movw	r2, #0x0100
	movt	r2, #0x2000
	movw	r3, #0x0180
	movt	r3, #0x2000
	movw	r4, #0x0200
	movt	r4, #0x2000
	movw	r5, #0x0280
	movt	r5, #0x2000
	movw	r6, #0x0300
	movt	r6, #0x2000
	movw	r7, #0x0380
	movt	r7, #0x2000
	movw	r8, #0x0400
	movt	r8, #0x2000
	movw	r9, #0x0480
	movt	r9, #0x2000
	movw	r10, #0x0500
	movt	r10, #0x2000
	movw	r11, #0x0580
	movt	r11, #0x2000

	vstmia.f32	r0, {s0-s31}
	vstmia.f32	r1!, {s0-s31}
	vstmia.f32	r2, {s0-s31}
	vstmia.f32	r3!, {s0-s31}
	vstmdb.f32	r4!, {s0-s31}
	vstmdb.f32	r5!, {s0-s31}

	vldmia.f32	r6, {s0-s31}
	vldmia.f32	r7!, {s0-s31}
	vldmia.f32	r8, {s0-s31}
	vldmia.f32	r9, {s0-s31}
	vldmdb.f32	r10!, {s0-s31}
	vldmdb.f32	r11!, {s0-s31}
	bx	lr

__vcvt_test:
	bx	lr

__vabs_test:
	// fpu registers should have some random values
	vabs.f32	s16, s0
	vabs.f32	s17, s1
	vabs.f32	s18, s2
	vabs.f32	s19, s3
	vabs.f32	s20, s4
	vabs.f32	s21, s5
	vabs.f32	s22, s6
	vabs.f32	s23, s7
	vabs.f32	s24, s8
	vabs.f32	s25, s9
	vabs.f32	s26, s10
	vabs.f32	s27, s11
	vabs.f32	s28, s12
	vabs.f32	s29, s13
	vabs.f32	s30, s14
	vabs.f32	s31, s15
	bx	lr

__vneg_test:
	vneg.f32	s16, s0
	vneg.f32	s17, s1
	vneg.f32	s18, s2
	vneg.f32	s19, s3
	vneg.f32	s20, s4
	vneg.f32	s21, s5
	vneg.f32	s22, s6
	vneg.f32	s23, s7
	vneg.f32	s24, s8
	vneg.f32	s25, s9
	vneg.f32	s26, s10
	vneg.f32	s27, s11
	vneg.f32	s28, s12
	vneg.f32	s29, s13
	vneg.f32	s30, s14
	vneg.f32	s31, s15
	bx	lr

__vcmp_test:
	// make 8 the same
	vmov.f32	s16, s0
	vmov.f32	s17, s1
	vmov.f32	s18, s2
	vmov.f32	s19, s3
	vmov.f32	s20, s4
	vmov.f32	s21, s5
	vmov.f32	s22, s6
	vmov.f32	s23, s7

	vcmp.f32	s16, s0
	vcmp.f32	s17, s1
	vcmp.f32	s18, s2
	vcmp.f32	s19, s3
	vcmp.f32	s20, s4
	vcmp.f32	s21, s5
	vcmp.f32	s22, s6
	vcmp.f32	s23, s7
	vcmp.f32	s24, s8
	vcmp.f32	s25, s9
	vcmp.f32	s26, s10
	vcmp.f32	s27, s11
	vcmp.f32	s28, s12
	vcmp.f32	s29, s13
	vcmp.f32	s30, s14
	vcmp.f32	s31, s15

	// assigned two fpu regs with 0.0
	vmov.f32	s0, #1
	vmov.f32	s1, #1
	vsub.f32	s0, s0, s1
	vmov.f32	s1, s0

	vcmp.f32	s0, s1
	vcmp.f32	s0, #0.0
	vcmp.f32	s0, s15

	bx	lr

__vsqrt_test:
	vmov.f32	s0, #1
	vmov.f32	s1, #2
	vmov.f32	s2, #3
	vmov.f32	s3, #4
	vmov.f32	s4, #5
	vmov.f32	s5, #6
	vmov.f32	s6, #7
	vmov.f32	s7, #8
	vmov.f32	s8, #9

	vsqrt.f32	s0, s0
	vsqrt.f32	s1, s1
	vsqrt.f32	s2, s2
	vsqrt.f32	s3, s3
	vsqrt.f32	s4, s4
	vsqrt.f32	s5, s5
	vsqrt.f32	s6, s6
	vsqrt.f32	s7, s7
	vsqrt.f32	s8, s8
	vsqrt.f32	s9, s9
	vsqrt.f32	s10, s10
	vsqrt.f32	s11, s11
	vsqrt.f32	s12, s12
	vsqrt.f32	s13, s13
	vsqrt.f32	s14, s14
	vsqrt.f32	s15, s15
	vsqrt.f32	s16, s16
	vsqrt.f32	s17, s17
	vsqrt.f32	s18, s18
	vsqrt.f32	s19, s19
	bx	lr

__vadd_test:
	vadd.f32	s0, s0, s16
	vadd.f32	s1, s1, s17
	vadd.f32	s2, s2, s18
	vadd.f32	s3, s3, s19
	vadd.f32	s4, s4, s20
	vadd.f32	s5, s5, s21
	vadd.f32	s6, s6, s22
	vadd.f32	s7, s7, s23
	vadd.f32	s8, s8, s24
	vadd.f32	s9, s9, s25
	vadd.f32	s10, s10, s26
	vadd.f32	s11, s11, s27
	vadd.f32	s12, s12, s28
	vadd.f32	s13, s13, s29
	vadd.f32	s14, s14, s30
	vadd.f32	s15, s15, s31
	bx	lr

__vsub_test:
	vsub.f32	s0, s0, s16
	vsub.f32	s1, s1, s17
	vsub.f32	s2, s2, s18
	vsub.f32	s3, s3, s19
	vsub.f32	s4, s4, s20
	vsub.f32	s5, s5, s21
	vsub.f32	s6, s6, s22
	vsub.f32	s7, s7, s23
	vsub.f32	s8, s8, s24
	vsub.f32	s9, s9, s25
	vsub.f32	s10, s10, s26
	vsub.f32	s11, s11, s27
	vsub.f32	s12, s12, s28
	vsub.f32	s13, s13, s29
	vsub.f32	s14, s14, s30
	vsub.f32	s15, s15, s31
	bx	lr

__vmul_test:
	vmul.f32	s0, s0, s16
	vmul.f32	s1, s1, s17
	vmul.f32	s2, s2, s18
	vmul.f32	s3, s3, s19
	vmul.f32	s4, s4, s20
	vmul.f32	s5, s5, s21
	vmul.f32	s6, s6, s22
	vmul.f32	s7, s7, s23
	vmul.f32	s8, s8, s24
	vmul.f32	s9, s9, s25
	vmul.f32	s10, s10, s26
	vmul.f32	s11, s11, s27
	vmul.f32	s12, s12, s28
	vmul.f32	s13, s13, s29
	vmul.f32	s14, s14, s30
	vmul.f32	s15, s15, s31
	bx	lr

__vmla_vmls:
	movw	r0, #0x0000
	movt	r0, #0x2000
	vldmia.f32	r0, {s0-s31}

	vmla.f32	s0, s0, s16
	vmla.f32	s0, s1, s17
	vmla.f32	s0, s2, s18
	vmla.f32	s0, s3, s19
	vmla.f32	s0, s4, s20
	vmla.f32	s0, s5, s21
	vmla.f32	s0, s6, s22
	vmla.f32	s0, s7, s23

	vmls.f32	s1, s8, s24
	vmls.f32	s1, s9, s25
	vmls.f32	s1, s10, s26
	vmls.f32	s1, s11, s27
	vmls.f32	s1, s12, s28
	vmls.f32	s1, s13, s29
	vmls.f32	s1, s14, s30
	vmls.f32	s1, s15, s31
	bx	lr

__vnmla_vnmls_vnmul:
	movw	r0, #0x0000
	movt	r0, #0x2000
	vldmia.f32	r0, {s0-s31}

	vnmul.f32	s3, s0, s16
	vnmul.f32	s4, s1, s17
	vnmul.f32	s5, s2, s18
	vnmul.f32	s6, s3, s19
	vnmul.f32	s7, s4, s20

	vnmla.f32	s1, s5, s21
	vnmla.f32	s1, s6, s22
	vnmla.f32	s1, s7, s23
	vnmla.f32	s1, s8, s24
	vnmla.f32	s1, s9, s25

	vnmls.f32	s2, s10, s26
	vnmls.f32	s2, s11, s27
	vnmls.f32	s2, s12, s28
	vnmls.f32	s2, s13, s29
	vnmls.f32	s2, s14, s30
	bx	lr

__vdiv_test:
	movw	r0, #0x0000
	movt	r0, #0x2000
	vldmia.f32	r0, {s0-s31}

	vdiv.f32	s0, s0, s16
	vdiv.f32	s1, s1, s17
	vdiv.f32	s2, s2, s18
	vdiv.f32	s3, s3, s19
	vdiv.f32	s4, s4, s20
	vdiv.f32	s5, s5, s21
	vdiv.f32	s6, s6, s22
	vdiv.f32	s7, s7, s23
	vdiv.f32	s8, s8, s24
	vdiv.f32	s9, s9, s25
	vdiv.f32	s10, s10, s26
	vdiv.f32	s11, s11, s27
	vdiv.f32	s12, s12, s28
	vdiv.f32	s13, s13, s29
	vdiv.f32	s14, s14, s30
	vdiv.f32	s15, s15, s31
	bx	lr

__fcvt_issue_test:
	movw	r0, #0xFF01
	movt	r0, #0xFFFF
	movw	r2, #0x0000
	movt	r2, #0x4f80
	vmov.f32	s0, r0
	vmov.f32	r1, s0
	vmov.f32	s2, r2
	vcvt.f32.u32	s1, s0

	bx	lr

        END