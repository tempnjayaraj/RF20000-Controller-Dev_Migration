/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2804                 $:
$Date:: 2022-01-19 22:35:53#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      zigbee.h
\brief     Header file for Zigbee module driver.
\details   Zigbee module is used to interface with the wireless footpedal
This module is to interface with the OEM Zigbee card. The Zigbee card is
connected to UART5 RX/TX. The MK60 can send commands to the Zigbee module using
this interface. The UART specs are given below:

Data Rate: 38,400 baud
Data Bits: 8
Parity: None
Stop bits: 1
Flow Control: None
(From Receiver Communications Protocol Rev: 006
Product: Medical RF receiver 5/5/2011 )
The commands are listed below:
*******************************************************************************/

#include "MK26F18.h"
#include "common.h"

#ifndef ZIGBEE_H
#define ZIGBEE_H

#define WIRELESS_1 GPIO_PIN(11)
#define WIRELESS_2 GPIO_PIN(7)
#define WIRELESS_3 GPIO_PIN(13)
#define ZIGBEE_CNCT GPIO_PIN(14)
#define SOPT5_LPUART0TXSRC_LPUART_TX  0x00u

/*!
 * @brief Set LPUART clock source.
 *
 * @param src The value to set LPUART clock source.
 */
static inline void CLOCK_SetLpuartClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_LPUARTSRC_MASK) | SIM_SOPT2_LPUARTSRC(src));
}

typedef enum {
  SWITCH_STATUS,
  MARRIAGE_STATUS,
  SYSTEM_STATUS,
}WIRELESS_STATUS;

typedef struct{
  uint8_t idx;
  struct {
    uint8_t data[4];
    uint8_t size;
  }cmd[2];
}WIRELESS_CMD;

#define MAX_WIRELESS_NUM_CHARS   3

uint8_t zigbee_init(void);
void UART5_Init(void);
void enable_datastream (void);
//void request_statusbyte0(void);
//void uart_putchar (UART_MemMapPtr, uint8_t);
//void request_reset (void);
//uint8_t uart_getchar_present (UART_MemMapPtr);
//uint8_t uart_getchar (UART_MemMapPtr );
//uint8_t read_statusbyte0(void);
uint8_t request_statusbytes(void);
void send_wireless_cmd(void);
void receive_wireless_cmd(void);
uint8_t wireless_handshake(void);


#endif
