/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
*******************************************************************************/
/*******************************************************************************
$Author:: Harshil.Shah    $: Author
$Rev:: 2836               $: Revision of last commit
$Date:: 2022-06-29 04:23:#$: Date of last commit
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
@file     ad7265.c
@brief     ADC7265 device driver
@details   This is the source file for ADC7265 device driver which enables the
app to talk to the ADC7265 which interfaces with legacy wand.
*******************************************************************************/
#include "ad7265.h"
#include "MK26F18.h"
#include "cpu.h"
#include "system.h"


// 6 channels for A and 6 channels for B, there 12 values;
static uint16_t ADC_DMA_buffer[12];
extern bool unitTest;
/***************************************************************************//**
Initialize driver for AD7265.
@param void
@return 0
@outputs
@context
*******************************************************************************/
void AD7265_Init(void) {

  /* Configure i/o for spi0 */
  PORTA_PCR14 = 0x00000200; // CS
  PORTA_PCR15 = 0x00000200; // CK
  //PORTA_PCR16 = 0x00000200; // MOSI
  PORTA_PCR17 = 0x00000200; // MISO

   /* Configure gpio pins for multiplex ctrl */
  PORTD_PCR0 = PORT_PCR_MUX(1); /* AD7265, A0 */
  PORTC_PCR19 = PORT_PCR_MUX(1); /* AD7265, A1 */
  PORTC_PCR18 = PORT_PCR_MUX(1); /* AD7265, A2 */

   /* Config pins connected to AD7265 multiplexer */
#ifdef REV4
  GPIOD_PDDR |= GPIO_PDDR_PDD(AD7265_MUX_CLEAR);
#elif REV5
  GPIOD_PDDR |= GPIO_PDDR_PDD(ISO_A0_A2D); // Clear A0
  GPIOC_PDDR |= GPIO_PDDR_PDD(ISO_A1_A2D); // Clear A1
  GPIOC_PDDR |= GPIO_PDDR_PDD(ISO_A2_A2D); // Clear A2
#endif

  /* Set mux to channel 0 */
#ifdef REV4
  GPIOD_PCOR |= AD7265_MUX_CLEAR;
#elif REV5
  GPIOD_PCOR |= ISO_A0_A2D; // Clear A0
  GPIOC_PCOR |= ISO_A1_A2D; // Clear A1
  GPIOC_PCOR |= ISO_A2_A2D; // Clear A2
#endif
  
  /* purge buffer */
  for (int i = 0; i < 12; i+=2)
  {
    
    ADC_DMA_buffer[i] = 0xFF;
    
  }

  /* Enable gate clock for spi0 */
  SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;

  SPI0_MCR = SPI_MCR_HALT_MASK;

  SPI0_MCR |= SPI_MCR_MSTR_MASK |
             //SPI_MCR_CONT_SCKE_MASK |
             //SPI_MCR_ROOE_MASK |
             SPI_MCR_DCONF(0) |
             SPI_MCR_PCSIS(1) |
             SPI_MCR_CLR_TXF_MASK |
             SPI_MCR_CLR_RXF_MASK;

  /* SPI Clock is based on Bus clock  which is 74 Mhz
  SPI clock needs to be around 1.5Mhz
  So 74Mhz*(1/3)*(1/16) = 1.5Mhz
  so PBR = 3 (set it to 01)  and BR = 16 (set it 03) */

  SPI0_CTAR0 = SPI_CTAR_FMSZ(7) |
               SPI_CTAR_CPHA_MASK |
               SPI_CTAR_PCSSCK(3) |
               SPI_CTAR_PASC(0) |
               SPI_CTAR_PDT(0) |
               SPI_CTAR_PBR(0) |
               SPI_CTAR_CSSCK(0) |
               SPI_CTAR_ASC(1) |
               SPI_CTAR_DT(0)|
               SPI_CTAR_BR(3);

  SPI0_CTAR1 = SPI_CTAR_FMSZ(7) |
               SPI_CTAR_CPHA_MASK |
               SPI_CTAR_PCSSCK(3) |
               SPI_CTAR_PASC(0) |
               SPI_CTAR_PDT(0) |
               SPI_CTAR_PBR(0) |
               SPI_CTAR_CSSCK(0) |
               SPI_CTAR_ASC(1) |
               SPI_CTAR_DT(0)|
               SPI_CTAR_BR(3);



  SPI0_RSER = 0x13030000;

  SPI0_MCR &= ~SPI_MCR_HALT_MASK;

  AD7265_DMA_Init();

  //NVICIP26 = 0x20; // spi0 priority 1
  NVIC_SetPriority (SPI0_IRQn,0x20); /* set Priority for Systick Interrupt */
  NVIC_EnableIRQ(SPI0_IRQn); // spi0

}

/***************************************************************************//**
This function is used to set up the address lines to read the different channels
in AD7265
@param channel number
@return ADC value
@outputs
@context
*******************************************************************************/
uint16_t IRQ_AD7265_Read(char channel) {

  uint16_t *data;

  while ((SPI0_SR & 0xF0) != 0)
  {
    if (unitTest) break;
  }
  GPIOD_PCOR |= ISO_A0_A2D; // Clear A0
  GPIOC_PCOR |= ISO_A1_A2D; // Clear A1
  GPIOC_PCOR |= ISO_A2_A2D; // Clear A2


  switch (channel) {
  case 0:
    //  A0 = 0, A1 = 0, A2 =0
    GPIOD_PCOR |= ISO_A0_A2D; // Clear A0
    GPIOC_PCOR |= ISO_A1_A2D; // Clear A1
    GPIOC_PCOR |= ISO_A2_A2D; // Clear A2
    break;
  case 1:
    //  A0 = 1 and A1 = 0 and A2 = 0
    GPIOD_PSOR |= ISO_A0_A2D; // Set A0
    GPIOC_PCOR |= ISO_A1_A2D; // Clear A1
    GPIOC_PCOR |= ISO_A2_A2D; // Clear A2
    break;
  case 2:
    //  A0 = 0 and A1 = 1 and A2 = 0
    GPIOD_PCOR |= ISO_A0_A2D; // Clear A0
    GPIOC_PSOR |= ISO_A1_A2D; // Set A1
    GPIOC_PCOR |= ISO_A2_A2D; // Clear A2
    break;
  case 3:
    //  A0 = 1 and A1 = 1 and A2 = 0
    GPIOD_PSOR |= ISO_A0_A2D; // Set A0
    GPIOC_PSOR |= ISO_A1_A2D; // Set A1
    GPIOC_PCOR |= ISO_A2_A2D; // Clear A2
    break;
  case 4:
    // A0 = 0 and A1 = 0 and A2 = 1
    GPIOD_PCOR |= ISO_A0_A2D; // Clear A0
    GPIOC_PCOR |= ISO_A1_A2D; // Clear A1
    GPIOC_PSOR |= ISO_A2_A2D; // Set A2
    break;
  case 5:
    // A0 = 1 and A1 = 0 and A2 = 1
    GPIOD_PSOR |= ISO_A0_A2D; // Set A0
    GPIOC_PCOR |= ISO_A1_A2D; // Clear A1
    GPIOC_PSOR |= ISO_A2_A2D; // Set A2
    break;
  default:
    // It should never get here.
    put_event(&evt_tbl,LW_ADC_FAULT);
  }

  uint8_t* data_addr = (uint8_t*) &ADC_DMA_buffer[(channel)*2];
   // increment by 3 bytes to get the addr of last byte of both the channels.
  data_addr = data_addr + 3;
  data = (uint16_t*)data_addr;

  DMA_TCD4_DADDR = (uint32_t) data;

  SPI0_MCR |= SPI_MCR_CLR_TXF_MASK | SPI_MCR_CLR_RXF_MASK;
  SPI0_TCR = 0;
  SPI0_SR |= SPI_SR_EOQF_MASK;

  /* Check data in RX FIFO has been moved to memory receive queue */
  while ((SPI0_SR & SPI_SR_RXCTR_MASK) != 0)
  {
    if (unitTest) break;
  }

  return *data;
}

/***************************************************************************//**
Sets up the DMA channel for the SPI bus which connects to the AD7265
param void
@return 0
@outputs
@context
*******************************************************************************/
void AD7265_DMA_Init(void)
{

    static uint32_t command_spi[4] = {0x84010000,
                                      0x80010000,
                                      0x80010000,
                                      0x08010000}; 



  /* Enable gate clock */
  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

  /* Set DMAMUX channel 4 request source to SPI0 recieve */
  DMAMUX_CHCFG4 = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(14);//0x90;

  /* Set DMAMUX channel 5 request source to SPI0 transmit */
  DMAMUX_CHCFG5 = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(15);//0x91;

  /* Source address is SPI0_POPR RX FIFO */
  DMA_TCD4_SADDR = 0x4002C038;

  /* Destination address is sram */
  DMA_TCD4_DADDR = (uint32_t) &ADC_DMA_buffer[0];

  /* Source offset disabled */
  DMA_TCD4_SOFF = 0;

  /* Source and Destination Modulo off, source and destination size 0 = 8 bits */
  DMA_TCD4_ATTR = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);

  /* Transfer 4 bytes per transaction */
  DMA_TCD4_NBYTES_MLNO = 0x01;

  /* No adjust needed */
  DMA_TCD4_SLAST = 0x00;

  /* Destination offset -1 byte */
  DMA_TCD4_DOFF = -1;

  /* No link channel to channel, 1 transaction */
  DMA_TCD4_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(4);

  /* Adjustment destination address 4 bytes after major loop complete */
  DMA_TCD4_DLASTSGA = 4;

  DMA_TCD4_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(4);

  DMA_TCD4_CSR = 0;

  /* Clear ERQ bit when major loop complete */
  DMA_TCD4_CSR = DMA_CSR_DREQ_MASK;


  /******************************************************************************
   * Configure DMA Channel 5
   *  TCD transfers SPI command word to TX FIFO Register (PUSHR).
   ******************************************************************************/

  /* Source address */
  DMA_TCD5_SADDR =  (uint32_t) &command_spi[0];

  /* Destination address is SPI0_PUSHR */
  DMA_TCD5_DADDR = 0x4002C034;

  /* Source offset is 4 bytes */
  DMA_TCD5_SOFF = 4;

  /* Source and destination size is 2 = 32 bits */
  DMA_TCD5_ATTR = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);

  /* Transfer 4 bytes per transaction */
  DMA_TCD5_NBYTES_MLNO = 4;

  /* Adjust source address by -4 bytes */
  /* Need to go back by 16, i.e -16, which can be represented as 
  0xFFFF FFF0 */
  DMA_TCD5_SLAST = 0xFFFFFFF0;

  /* Destination offset disabled */
  DMA_TCD5_DOFF = 0;

  /* No link channel to channel, 4 transactions */
  DMA_TCD5_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(4);

  /* No adjustment to destination address */
  DMA_TCD5_DLASTSGA = 0;

  /* No link channel to channel, 4 transactions */
  DMA_TCD5_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(4);

  DMA_TCD5_CSR = 0;

  /* Clear ERQ bit when major loop complete */
  DMA_TCD5_CSR = DMA_CSR_DREQ_MASK;

}

/***************************************************************************//**
Reads the ADC value from the array. The SPI ISR stores the values from the
SPI bus in an array. This function is used to read the value from the array and
return it.
@param Channel Number (A1-A6, B1-B6)
@return 16 bit ADC value
@outputs
@context
*******************************************************************************/
uint16_t AD7265_read_value (CHAN_NUM channel_num)
{
  uint16_t* base_address = ADC_DMA_buffer;
  uint16_t* channel_address = base_address + (channel_num);

  // The ADC returns 12 bit 2s complement values. The 12 bits are packed
  // into 2 bytes (16 bits). The first two most significant bits are zero
  // and the last two significant bits are zero. Hence the 0x3FFC mask.
  // In order to preserve the sign, the result is left shifted twice.
  int16_t value = (0x3FFC & *channel_address)<<2;
  
  // Preserving the sign and right shifting by 4
  value /= 16;

  // ADC expects voltages from -VREF to +VREF. 0v input is returned as
  // NEGATIVE_MAX, so shifting it by 2048 moves the values in the positive
  // direction.
  value += 2048;

  //After this sign bit is not of concern, so casting the value to unsigned
  uint16_t unsigned_value = (uint16_t)value;

  return unsigned_value;
}
