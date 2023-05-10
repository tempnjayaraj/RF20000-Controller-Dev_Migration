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
\file      wdog.c
\brief     Watchdog module.
\details   NA
*******************************************************************************/

#include "MK26F18.h"
#include "wdog.h"
#include "common.h"

/***************************************************************************//**
Enable HW watch dog
@param void
@return 0
@outputs
@context
*******************************************************************************/
void wdog_enable(void)
{

  /* enable updates */
  WDOG_STCTRLH |= WDOG_STCTRLH_ALLOWUPDATE_MASK;
  
  /* Disable the watchdog timer w/ unlock sequence */
  WDOG_UNLOCK = 0xC520;
  WDOG_UNLOCK = 0xD928;
   
  /* Clear the WDOGEN bit to disable the watchdog */
  //WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
  
  /* set time out value 500 msec @ 1kHz */
  WDOG_TOVALL = WDOG_TOVALL_TOVALLOW(0x1F4);
  WDOG_TOVALH = WDOG_TOVALH_TOVALHIGH(0);
      
  /* divide clock by 2 */
  WDOG_PRESC = WDOG_PRESC_PRESCVAL(0);
    
  /* enable watchdog, use dedicated wdog clock (1kHz lpo) */
  WDOG_STCTRLH = WDOG_STCTRLH_WDOGEN_MASK | /* enable wdog */
                 //WDOG_STCTRLH_DBGEN_MASK |             
                 WDOG_STCTRLH_STOPEN_MASK | /* enable during cpu stop */
                 WDOG_STCTRLH_WAITEN_MASK;  /* enable during cpu wait */
  
  wdog_refresh();
  
}

/***************************************************************************//**
Disable HW watch dog
@param void
@return 0
@outputs
@context
*******************************************************************************/
void wdog_disable(void)
{
  
  /* Disable the watchdog timer w/ unlock sequence */
  WDOG_UNLOCK = 0xC520;
  WDOG_UNLOCK = 0xD928;

  /* Clear the WDOGEN bit to disable the watchdog */
  WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
  
}

/***************************************************************************//**
Refresh HW watch dog
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t wdog_refresh(void)
{
  
  /* disable interrupts */
  __istate_t s = __get_interrupt_state();
  __disable_interrupt();
  
  /* write refresh sequence */
  WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xA602);
  WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xB480);
  
  /* turn yellow LED OFF */
  GPIOA_PTOR |= YELLOW_LED;

  /* re-enable interrupts */
  __set_interrupt_state(s);
  
  return 0;
    
}
