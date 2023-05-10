// $Revision: 23302 $ $Date: 2021-07-29 11:48:27 -0700 (Thu, 29 Jul 2021) $
// Copyright (c) Bullseye Testing Technology
// This source file contains confidential proprietary information.
//
// BullseyeCoverage small footprint run-time for IAR Systems DLIB run-time library
// This implementation uses the i/o functions described in the IAR C/C++ Development Guide
//   section "The DLIB low-level I/O interface"

// Suppress MISRA warnings
#pragma system_include

// Suppress warning "non-native end of line sequence detected"
#pragma diag_suppress=Pa050
// Suppress MISRA warnings
#pragma diag_suppress=Pm001,Pm002,Pm003,Pm004,Pm005,Pm006,Pm007,Pm008,Pm009,Pm010
#pragma diag_suppress=Pm011,Pm012,Pm013,Pm014,Pm015,Pm016,Pm017,Pm018,Pm019,Pm020
#pragma diag_suppress=Pm021,Pm022,Pm023,Pm024,Pm025,Pm026,Pm027,Pm028,Pm029,Pm030
#pragma diag_suppress=Pm031,Pm032,Pm033,Pm034,Pm035,Pm036,Pm037,Pm038,Pm039,Pm040
#pragma diag_suppress=Pm041,Pm042,Pm043,Pm044,Pm045,Pm046,Pm047,Pm048,Pm049,Pm050
#pragma diag_suppress=Pm051,Pm052,Pm053,Pm054,Pm055,Pm056,Pm057,Pm058,Pm059,Pm060
#pragma diag_suppress=Pm061,Pm062,Pm063,Pm064,Pm065,Pm066,Pm067,Pm068,Pm069,Pm070
#pragma diag_suppress=Pm071,Pm072,Pm073,Pm074,Pm075,Pm076,Pm077,Pm078,Pm079,Pm080
#pragma diag_suppress=Pm081,Pm082,Pm083,Pm084,Pm085,Pm086,Pm087,Pm088,Pm089,Pm090
#pragma diag_suppress=Pm091,Pm092,Pm093,Pm094,Pm095,Pm096,Pm097,Pm098,Pm099,Pm100
#pragma diag_suppress=Pm101,Pm102,Pm103,Pm104,Pm105,Pm106,Pm107,Pm108,Pm109,Pm110
#pragma diag_suppress=Pm111,Pm112,Pm113,Pm114,Pm115,Pm116,Pm117,Pm118,Pm119,Pm120
#pragma diag_suppress=Pm121,Pm122,Pm123,Pm124,Pm125,Pm126,Pm127,Pm128,Pm129,Pm130
#pragma diag_suppress=Pm131,Pm132,Pm133,Pm134,Pm135,Pm136,Pm137,Pm138,Pm139,Pm140
#pragma diag_suppress=Pm141,Pm142,Pm143,Pm144,Pm145,Pm146,Pm147,Pm148,Pm149,Pm150
#pragma diag_suppress=Pm151,Pm152,Pm153,Pm154,Pm155
// Suppress warning 'zero used for undefined preprocessing identifier "..."'

#if _BullseyeCoverage
	#pragma BullseyeCoverage off
#endif

#include <stddef.h>
#if __IAR_SYSTEMS_ICC__ >= 9
	#include <LowLevelIOInterface.h>
#else
	#include <yfuns.h>
#endif
#define O_CREAT _LLIO_CREAT
#define O_TRUNC _LLIO_TRUNC
#define O_WRONLY _LLIO_WRONLY
#define open(path, oflag, mode) __open(path, oflag)
#define close __close
#define read(fildes, buf, nbyte) __read(fildes, (unsigned char*)(buf), nbyte)
#define write(fildes, buf, nbyte) __write(fildes, (const unsigned char*)(buf), nbyte)
#include "stub-getpid.h"

#include "libcov-atomic.h"
#include "libcov.h"
#include "libcov-version.h"
#include "libcov-core-small.h"
