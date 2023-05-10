/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: sseeth00          $:
$Rev:: 1037                 $:
$Date:: 2014-04-22 14:05:22#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     led.h
\brief     Header file for led driver.
\details   N/A
*******************************************************************************/

#ifndef LED_H
#define LED_H

#define MWSR                   0x00  /* Master write  */
#define MRSW                   0x01  /* Master read */

#define I2C1_START()            I2C1_C1 |= I2C_C1_TX_MASK;\
                               I2C1_C1 |= I2C_C1_MST_MASK
                                 
#define I2C1_WAIT()             while((I2C1_S & I2C_S_IICIF_MASK)==0) {} \
                               I2C1_S |= I2C_S_IICIF_MASK;

#define I2C1_STOP()             I2C1_C1 &= ~I2C_C1_MST_MASK;\
                               I2C1_C1 &= ~I2C_C1_TX_MASK
                                                        
#define PCF8575_I2C_ADDRESS    0x4E
#define PCF8575_I2C_WRITE_MASK 0x00
#define PCF8575_I2C_READ_MASK  0x01
#define PCF8575_I2C_P00_MASK   0x01                               
#define PCF8575_I2C_P01_MASK   0x02                                 
#define PCF8575_I2C_P02_MASK   0x04                                 
#define PCF8575_I2C_P03_MASK   0x08
#define PCF8575_I2C_P04_MASK   0x10
#define PCF8575_I2C_P05_MASK   0x20                                 
#define PCF8575_I2C_P06_MASK   0x40
#define PCF8575_I2C_P07_MASK   0x80
#define PCF8575_I2C_P10_MASK   0x01                               
#define PCF8575_I2C_P11_MASK   0x02                                 
#define PCF8575_I2C_P12_MASK   0x04                                 
#define PCF8575_I2C_P13_MASK   0x08
#define PCF8575_I2C_P14_MASK   0x10
#define PCF8575_I2C_P15_MASK   0x20                                 
#define PCF8575_I2C_P16_MASK   0x40
#define PCF8575_I2C_P17_MASK   0x80
                                 
#define WAND_LED_RED           PCF8575_I2C_P11_MASK
#define WAND_LED_GRN           PCF8575_I2C_P10_MASK
#define PWR_LED_RED            PCF8575_I2C_P05_MASK
#define PWR_LED_GRN            PCF8575_I2C_P04_MASK
#define ODU_LED_RED            PCF8575_I2C_P03_MASK
#define ODU_LED_GRN            PCF8575_I2C_P02_MASK
#define LEMO_LED_RED           PCF8575_I2C_P01_MASK
#define LEMO_LED_GRN           PCF8575_I2C_P00_MASK                                 
#define PUMP_LED_RED           PCF8575_I2C_P07_MASK                                 
#define PUMP_LED_GRN           PCF8575_I2C_P06_MASK                                 
                           
                                 
/* Function Prototypes */
void pcf8575_iic_write(uint8_t);
void PCF8575_I2C_StartTransmission (uint8_t);
uint8_t led_init(void);
uint8_t update_leds(void);
uint8_t startup_leds(void);
uint8_t shutdown_led_sequence(void);
uint8_t startup_led_init(void);
uint8_t startup_leds(void);
void startup_led_sequence(uint8_t);


#endif
