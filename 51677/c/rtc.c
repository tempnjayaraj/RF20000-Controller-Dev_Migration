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
\file:     rtc.c
\brief     Driver for real time clock peripheral.
\details   N/A
*******************************************************************************/
#include "rtc.h"
#include "pit.h"
#include "MK26F18.h"
#include "cpu.h"

/***************************************************************************//**
Initialize real time clock.
@param void
@return uint32_t seconds
@TODO seconds is always set to zero
@outputs
@context
*******************************************************************************/
uint32_t rtc_init(void) 
{
  
  uint32_t seconds = 0;  
  
  /* Enable gate clock */
  SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
    
  /* Set software reset bit so everything is in a known state*/
  //RTC_CR  = RTC_CR_SWR_MASK;
  
  /* Clear software reset bit*/
  RTC_CR  &= ~RTC_CR_SWR_MASK;
  
  /* enable the oscillator and */
  RTC_CR |= RTC_CR_OSCE_MASK | RTC_CR_SC8P_MASK | RTC_CR_SC4P_MASK;
    
  /* disable compensation */
  RTC_TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);
  
  /* set time compensation parameters */
  RTC_TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);
  
  /* configure alarm register */
  RTC_TAR = 0xFFFFFFFF;
  
  /*Enable the interrupt*/
  NVIC_EnableIRQ(RTC_IRQn);  // alarm
  NVIC_EnableIRQ(RTC_Seconds_IRQn);  // one second tick
     
  /* enable the counter*/
  RTC_SR |= RTC_SR_TCE_MASK;
  
  RTC_IER |= RTC_IER_TSIE_MASK | RTC_IER_TAIE_MASK;
  
  /* rtc_clkout */
  PORTE_PCR0 = PORT_PCR_MUX(7);
  
  /* clock output */
  RTC_CR &= ~RTC_CR_CLKO_MASK;
  
  /* disable rtc trace output; pin is used for sdhc */
  SIM_SOPT2 &= ~SIM_SOPT2_RTCCLKOUTSEL_MASK;
    
  seconds = RTC_TSR;
  
  return seconds;
  
}

/***************************************************************************//**
Check for RTC errors
@param void
@return void
@outputs
@context
*******************************************************************************/
void rtc_check_errors(void) 
{
   
   if((RTC_SR & RTC_SR_TIF_MASK)== 0x01)
     {
           //printf("SRTC time invalid interrupt entered...\r\n");
   	   RTC_SR &= 0x07;  //clear TCE, or RTC_TSR can  not be written
   	   RTC_TSR = 0x00000000;  //clear TIF 
     }	
   else if((RTC_SR & RTC_SR_TOF_MASK) == 0x02)
   {
   	   //printf("SRTC time overflow interrupt entered...\r\n");
   	   RTC_SR &= 0x07;  //clear TCE, or RTC_TSR can  not be written
   	   RTC_TSR = 0x00000000;  //clear TOF
   }	 	
   else if((RTC_SR & RTC_SR_TAF_MASK) == 0x04)
   {
   	   //printf("SRTC alarm interrupt entered...\r\n");
           //printf("Time Seconds Register value is: %i\n", RTC_TSR);
   	   RTC_TAR = 0xFFFFFFFF;
           
   }	
   else
   {
           //printf("No valid Flag was set!\n");
   }
   return;
}


///*
//** ===================================================================
//**     Method      :  RTC1_ConvertSecondsToDateAndTime (component RTC_LDD)
//**
//**     Description :
//**         Computes date and time from the number of seconds till the 
//**         given date
//**         This method is internal. It is used by Processor Expert only.
//** ===================================================================
//*/
//void Seconds2DateAndTime(void)
//{
//  
//  uint32_t x;
//  uint32_t days, seconds;
//  TIME rtc_time;
//  
//  dMonth[12] = {0,DMonth(2)= 31, DMonth(7)= 181
// DMonth(8)= 212
//DMonth(3)= 59 DMonth(9)= 243
//DMonth(4)= 90 DMonth(10)=273
//DMonth(5)=120 DMonth(11)=304
//DMonth(6)=151 DMonth(12)=334
//DMonth(13)=365
//  
//  seconds = RTC_TSR;
//  
//  days = seconds / 86400U;             /* Days */
//  seconds = seconds % 86400U;          /* Seconds left */
//  time.hour = seconds / 3600U;     /* Hours */
//  seconds = seconds % 3600u;           /* Seconds left */
//  time.minutes = seconds / 60U;     /* Minutes */
//  time.seconds = seconds % 60U;     /* Seconds */
//  time.week_day = (days + 6U) % 7U; /* Day of week */
//  time.year = (4U * (days / ((4U * 365U) + 1U))) + 2000U; /* Year */
//  days = days % ((4U * 365U) + 1U);
//  /* 59 */
//  if (days == ((0U * 365U) + 59U))
//  { 
//  
//    time.day = 29U;
//    time.month = 2U;
//    return;
//  
//  } 
//  
//  else if (days > ((0U * 365U) + 59U))
//  {
//  
//    days--;
//    
//  } 
//  
//  x =  days / 365U;
//  time.year += x;
//  days -= x * 365U;
//  for (x=1U; x <= 12U; x++)
//  {
//    
//    if (days < ULY[x])
//    {
//      
//      time.month = x;
//      break;
//      
//    } 
//    else
//    {
//   
//      Days -= ULY[x];
//   
//    }
//  
//  }
//  
//  time.day = days + 1U;
//  
//}
