/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
*******************************************************************************/
/********************************************************************************
$Author: sseeth00 $
$Date: 2014-10-09 15:58:14 -0500 (Thu, 09 Oct 2014) $
$Modtime:  $
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
********************************************************************************/
/****************************************************************************//**
\file     buck_test.c
\brief     Calibration data points for DC-DC buck converter.
\details   Calibration data points for DC-DC buck converter.
This table is used to calibrate the buck. It is a logrithamic increment of the
sample points from 100 to MAX_DAC value. It starts at a 100 because, below the
DAC value of 100, the buck is very inaccurate. So the whole table is
shifted by 100. Also, a log scale is used because the buck is more inaccurate
in the lower levels so more samples are taken the the lower end than at the 
higher end.
********************************************************************************/


#include "buck.h"

__root const uint16_t buck_log_sample_vector[512] = 
  { 
    0, 100, 100, 100, 100, 100, 100, 100,
100, 100, 100, 100, 100, 100, 100, 100,
100, 100, 100, 100, 100, 100, 100, 100,
100, 101, 101, 101, 101, 101, 101, 101,
101, 101, 101, 101, 101, 101, 101, 101,
101, 101, 101, 101, 101, 101, 101, 101,
101, 101, 101, 101, 101, 101, 101, 101,
101, 102, 102, 102, 102, 102, 102, 102,
102, 102, 102, 102, 102, 102, 102, 102,
102, 102, 102, 102, 102, 102, 103, 103,
103, 103, 103, 103, 103, 103, 103, 103,
103, 103, 103, 103, 103, 104, 104, 104,
104, 104, 104, 104, 104, 104, 104, 104,
104, 104, 105, 105, 105, 105, 105, 105,
105, 105, 105, 105, 106, 106, 106, 106,
106, 106, 106, 106, 106, 107, 107, 107,
107, 107, 107, 107, 108, 108, 108, 108,
108, 108, 108, 109, 109, 109, 109, 109,
109, 110, 110, 110, 110, 110, 110, 111,
111, 111, 111, 111, 112, 112, 112, 112,
112, 113, 113, 113, 113, 114, 114, 114,
114, 115, 115, 115, 115, 116, 116, 116,
116, 117, 117, 117, 118, 118, 118, 118,
119, 119, 119, 120, 120, 120, 121, 121,
122, 122, 122, 123, 123, 123, 124, 124,
125, 125, 126, 126, 126, 127, 127, 128,
128, 129, 129, 130, 130, 131, 131, 132,
132, 133, 133, 134, 135, 135, 136, 136,
137, 138, 138, 139, 139, 140, 141, 141,
142, 143, 144, 144, 145, 146, 147, 147,
148, 149, 150, 151, 151, 152, 153, 154,
155, 156, 157, 158, 159, 160, 161, 162,
163, 164, 165, 166, 167, 168, 169, 170,
172, 173, 174, 175, 176, 178, 179, 180,
182, 183, 184, 186, 187, 189, 190, 192,
193, 195, 196, 198, 199, 201, 203, 204,
206, 208, 210, 211, 213, 215, 217, 219,
221, 223, 225, 227, 229, 231, 233, 236,
238, 240, 242, 245, 247, 250, 252, 255,
257, 260, 262, 265, 268, 271, 273, 276,
279, 282, 285, 288, 291, 294, 297, 301,
304, 307, 311, 314, 318, 321, 325, 329,
332, 336, 340, 344, 348, 352, 356, 361,
365, 369, 374, 378, 383, 387, 392, 397,
402, 407, 412, 417, 422, 427, 433, 438,
444, 449, 455, 461, 467, 473, 479, 485,
491, 498, 504, 511, 518, 525, 532, 539,
546, 553, 561, 568, 576, 584, 592, 600,
608, 616, 625, 633, 642, 651, 660, 669,
678, 688, 697, 707, 717, 727, 738, 748,
759, 769, 780, 792, 803, 814, 826, 838,
850, 862, 875, 888, 900, 914, 927, 940,
954, 968, 982, 997, 1012, 1026, 1042, 1057,
1073, 1089, 1105, 1121, 1138, 1155, 1172, 1190,
1208, 1226, 1244, 1263, 1282, 1301, 1321, 1341,
1361, 1382, 1403, 1424, 1446, 1468, 1491, 1513,
1536, 1560, 1584, 1608, 1633, 1658, 1683, 1709,
1736, 1763, 1790, 1817, 1845, 1874, 1903, 1933,
1963, 1993, 2024, 2056, 2088, 2120, 2153, 2187,
2221, 2256, 2291, 2327, 2363, 2400, 2438, 2476,
2515, 2555, 2595, 2636, 2677, 2719, 2762, 2806,
2850, 2895, 2941, 2987, 3035, 3083, 3132, 3181,
3232, 3283, 3335, 3388, 3442, 3496, 3552, 3608,
3666, 3724, 3784, 3844, 3905, 3967, 4031, 4095,

  };