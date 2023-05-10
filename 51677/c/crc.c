/********************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
*******************************************************************************/
/*******************************************************************************
$Author: Harshil.Shah $
$Date: 2021-11-17 02:10:26 -0600 (Wed, 17 Nov 2021) $
$Modtime: 7/02/13 11:17a $
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     crc.c
\brief     This file contains crc routines.
\details   N/A
*******************************************************************************/

/**************************************************
 * Copyright 2007 IAR Systems. All rights reserved.
 *
 * $Revision: 2798 $
 *
 * A simple implementation of the crc-16 checksum algorithm.
 *
 * Note that if you use this algorithm/function, you need to call it with
 * an extra number of zero bytes equal to the size of the checksum
 * (in this case 2), to "rotate out the answer".
 *
 * Example of usage:
 *
 * unsigned short calc;
 * // Run the checksum algorithm
 * calc = slow_crc16(0, p, size);
 * // Rotate out the answer
 * unsigned char zeroes[2] = {0, 0};
 * calc = slow_crc16(calc, zeroes, 2);
 * // Test the checksum
 * if (calc != the_checksum)
 * {
 *   // Some error handling...
 * }
 *
 **************************************************/

#include "crc.h"

const
uint16_t
crc16_table[256]  = {
    0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50a5U, 0x60c6U, 0x70e7U, 0x8108U, 
    0x9129U, 0xa14aU, 0xb16bU, 0xc18cU, 0xd1adU, 0xe1ceU, 0xf1efU, 0x1231U, 0x0210U, 
    0x3273U, 0x2252U, 0x52b5U, 0x4294U, 0x72f7U, 0x62d6U, 0x9339U, 0x8318U, 0xb37bU, 
    0xa35aU, 0xd3bdU, 0xc39cU, 0xf3ffU, 0xe3deU, 0x2462U, 0x3443U, 0x0420U, 0x1401U, 
    0x64e6U, 0x74c7U, 0x44a4U, 0x5485U, 0xa56aU, 0xb54bU, 0x8528U, 0x9509U, 0xe5eeU, 
    0xf5cfU, 0xc5acU, 0xd58dU, 0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76d7U, 0x66f6U, 
    0x5695U, 0x46b4U, 0xb75bU, 0xa77aU, 0x9719U, 0x8738U, 0xf7dfU, 0xe7feU, 0xd79dU, 
    0xc7bcU, 0x48c4U, 0x58e5U, 0x6886U, 0x78a7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U, 
    0xc9ccU, 0xd9edU, 0xe98eU, 0xf9afU, 0x8948U, 0x9969U, 0xa90aU, 0xb92bU, 0x5af5U, 
    0x4ad4U, 0x7ab7U, 0x6a96U, 0x1a71U, 0x0a50U, 0x3a33U, 0x2a12U, 0xdbfdU, 0xcbdcU, 
    0xfbbfU, 0xeb9eU, 0x9b79U, 0x8b58U, 0xbb3bU, 0xab1aU, 0x6ca6U, 0x7c87U, 0x4ce4U, 
    0x5cc5U, 0x2c22U, 0x3c03U, 0x0c60U, 0x1c41U, 0xedaeU, 0xfd8fU, 0xcdecU, 0xddcdU, 
    0xad2aU, 0xbd0bU, 0x8d68U, 0x9d49U, 0x7e97U, 0x6eb6U, 0x5ed5U, 0x4ef4U, 0x3e13U, 
    0x2e32U, 0x1e51U, 0x0e70U, 0xff9fU, 0xefbeU, 0xdfddU, 0xcffcU, 0xbf1bU, 0xaf3aU, 
    0x9f59U, 0x8f78U, 0x9188U, 0x81a9U, 0xb1caU, 0xa1ebU, 0xd10cU, 0xc12dU, 0xf14eU, 
    0xe16fU, 0x1080U, 0x00a1U, 0x30c2U, 0x20e3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U, 
    0x83b9U, 0x9398U, 0xa3fbU, 0xb3daU, 0xc33dU, 0xd31cU, 0xe37fU, 0xf35eU, 0x02b1U, 
    0x1290U, 0x22f3U, 0x32d2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U, 0xb5eaU, 0xa5cbU, 
    0x95a8U, 0x8589U, 0xf56eU, 0xe54fU, 0xd52cU, 0xc50dU, 0x34e2U, 0x24c3U, 0x14a0U, 
    0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U, 0xa7dbU, 0xb7faU, 0x8799U, 0x97b8U, 
    0xe75fU, 0xf77eU, 0xc71dU, 0xd73cU, 0x26d3U, 0x36f2U, 0x0691U, 0x16b0U, 0x6657U, 
    0x7676U, 0x4615U, 0x5634U, 0xd94cU, 0xc96dU, 0xf90eU, 0xe92fU, 0x99c8U, 0x89e9U, 
    0xb98aU, 0xa9abU, 0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18c0U, 0x08e1U, 0x3882U, 
    0x28a3U, 0xcb7dU, 0xdb5cU, 0xeb3fU, 0xfb1eU, 0x8bf9U, 0x9bd8U, 0xabbbU, 0xbb9aU, 
    0x4a75U, 0x5a54U, 0x6a37U, 0x7a16U, 0x0af1U, 0x1ad0U, 0x2ab3U, 0x3a92U, 0xfd2eU, 
    0xed0fU, 0xdd6cU, 0xcd4dU, 0xbdaaU, 0xad8bU, 0x9de8U, 0x8dc9U, 0x7c26U, 0x6c07U, 
    0x5c64U, 0x4c45U, 0x3ca2U, 0x2c83U, 0x1ce0U, 0x0cc1U, 0xef1fU, 0xff3eU, 0xcf5dU, 
    0xdf7cU, 0xaf9bU, 0xbfbaU, 0x8fd9U, 0x9ff8U, 0x6e17U, 0x7e36U, 0x4e55U, 0x5e74U, 
    0x2e93U, 0x3eb2U, 0x0ed1U, 0x1ef0U, 
  };

 
   
/****************************************************************************//**
performs a crc check (slower implementation)
@param sum - previous checksum , *p - pointer to data, len - length of data
@return checksum
@outputs
@context
********************************************************************************/
uint16_t slow_crc16(uint16_t sum, uint8_t *p, uint32_t len)
{
  
  while (len--)
  {
    int32_t i;
    uint8_t byte = *(p++);
  
    for (i = 0; i < 8; ++i)
    {
      
      uint32_t osum = sum;
      sum <<= 1;
      
      if (byte & 0x80)
      {
        
        sum |= 1;
        
      }
      
      if (osum & 0x8000)
      {
        
        sum ^= 0x1021;
        
      }
      
      byte <<= 1;
      
    }
    
  }
  
  return sum;
  
}

/****************************************************************************//**
performs a faster implementation of the crc check
@param sum - previous checksum , *p - pointer to data, len - length of data
@return checksum
@outputs
@context
same algorithm as ielftool; its like CRC-CCITT but slightly different:
* polynomial=1021
* initial value=0
* XorOut=0
* refin=false
* refout=false
********************************************************************************/
uint16_t
fast_crc16(uint16_t sum, uint8_t *p, uint32_t len)
{
  while (len--)
    sum = crc16_table[(sum >> 8) ^ *p++] ^ (sum << 8);
  return sum;
}

