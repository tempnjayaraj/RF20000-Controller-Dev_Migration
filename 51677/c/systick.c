/*******************************************************************************
                   ArthroCare Proprietary and Confidential
                   (c) ArthroCare Corporation 2012
                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2817                 $:
$Date:: 2022-02-18 03:41:55 -0600#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     systick.c
\brief     System tick timer driver.
\details   N/A
*******************************************************************************/

/* Private Macros & Defines ---------------------------------------- */
#define SYSTICK_LENGTH_MS  ((u32)1)
#define MAX_NBR_OF_CALLBACKS    ((u8)16)

/* Includes -------------------------------------------------------- */
#include "systick.h"
#include "mcg.h"
#include "common.h"
#include "system.h"
/* Private TypeDef ------------------------------------------------- */
/* <struct describing a user registered callback */
typedef struct _cbtmr_t
{
    bool          in_use;         ///<flag to trak availablility
    u8            id;             ///<callback identifier
    u32           trig_lvl;       ///<given time to "do something"
    u32           count;          ///<current tick value
    cbtmr_mode_t  cbtmr_mode;     ///<single or continuous?
    FlagStatus    *cbtmr_flag;    ///<returns flag to indicate "time to do something"
    cbtmr_func_t  cbtmr_func;     ///<callback function
} cbtmr_t;

// Private Variables -----------------------------------------------
///< keep track of call-back timers
volatile cbtmr_t cbtmr_register[MAX_NBR_OF_CALLBACKS];

//id starts at 1 in order to use 0 as an indicator for when id is not used
volatile u8 last_id = 1;    /**< keeping track of last id nbr used */

// Private Constants -----------------------------------------------

// Private Function Prototypes -------------------------------------
static void ctrl_systick_status(FunctionalState EnableSystick);

// Private Functions -----------------------------------------------
//------------------------------------------------------------------
/// @fn      ctrl_systick_status
/// @brief   Enables/Disables Systick interrupt
/// @param   EnableSystick [in] ENABLE || DISABLE
/// @param   None [out]
/// @return  None
//------------------------------------------------------------------
void ctrl_systick_status(FunctionalState EnableSystick)
{
  if (EnableSystick)
  {
    SYST_CSR |= SysTick_CSR_ENABLE_MASK;
  }
  else
  {
    SYST_CSR &= ~SysTick_CSR_ENABLE_MASK;
  }
} /* ctrl_systick_status */

//------------------------------------------------------------------
/// @fn      cbtmr_init
/// @brief   Init the callback timer
/// @details Initialize the callback timer. Uses SysTick which provides
///          an interrupt every ms.
/// @param   None [in]
/// @param   None [out]
/// @return  None
//------------------------------------------------------------------
void cbtmr_init(void)
{
  //u32 TimerClk;
  //get_clk_frequency(CoreClock, &TimerClk);

  /* set reload value to defined systick time */
  //SYST_RVR = ((TimerClk / 1000) * SYSTICK_LENGTH_MS);

  /* Initialize cb_register */
  for (u8 i = 0U; i < MAX_NBR_OF_CALLBACKS; i++)
  {
    cbtmr_register[i].in_use = FALSE;
  }

  SYST_CSR |= SysTick_CSR_TICKINT_MASK |  // Enable interrupt
              SysTick_CSR_CLKSOURCE_MASK; //Clock source to core clk

  //ctrl_systick_status(ENABLE);

} /* cbtmr_init */

//------------------------------------------------------------------
/// @fn      systick_isr
/// @brief   System tick ISR.
/// @param   [in]   None
/// @param   [out]  None
/// @return  None
//------------------------------------------------------------------
uint8_t cbtmr_isr(void)
{
  for (u8 slot_nbr = 0; slot_nbr < MAX_NBR_OF_CALLBACKS; slot_nbr++)
  {
    /* if the slot isn't used then don't update it */
    if (TRUE == cbtmr_register[slot_nbr].in_use)
    {
      /* increment counter */
      cbtmr_register[slot_nbr].count++;

      /* check if it is time to execute callback and/or raise flag */
      if(cbtmr_register[slot_nbr].trig_lvl == cbtmr_register[slot_nbr].count)
      {
          /* execute callback if supplied */
          if(NULL != cbtmr_register[slot_nbr].cbtmr_func)
          {
              (cbtmr_register[slot_nbr].cbtmr_func)();
          }
          else
          {
            /* no callback function */
          }

          /* raise flag if supplied */
          if(NULL != cbtmr_register[slot_nbr].cbtmr_flag)
          {
              *(cbtmr_register[slot_nbr].cbtmr_flag) = SET;
          }
          else
          {
            /* no flag to set */
          }

          //if continuous then reset count for next use
          if(CONTINUOUS_CBTMR == cbtmr_register[slot_nbr].cbtmr_mode)
          {
              cbtmr_register[slot_nbr].count = 0UL;
          }
          else
          {
              //else mark slot as unused
              cbtmr_register[slot_nbr].in_use = FALSE;
          }
      }
      else
      {
        /* do nothing -  not time to do anything */
      }
    }
  }
  ctrl_systick_status(ENABLE);
  return 0;
} /* systick_isr */

//------------------------------------------------------------------
/// @fn     cbtmr_reg_callback
/// @brief  Register a callback
/// @details Register a callback which will be called after a user defined
///          time period. The user can either provide a function pointer,
///          flag or both. If an option is not used it must be set to NULL.
/// @param  cbtmr_func    [in]  Function pointer which will be executed.
/// @param  cbtmr_flag    [in]  Pointer to flag which will be raised.
/// @param  cbtmr_time_ms [in]  User defined time period until event in milliseconds
/// @param  cbtmr_mode    [in]  Callback type (continuous or single)
/// @param  cbtmr_id      [out] Callback id.
/// @return error status flag
//------------------------------------------------------------------
ErrorStatus cbtmr_reg_callback(cbtmr_func_t cbtmr_func,
                               FlagStatus *cbtmr_flag,
                               const u32 cbtmr_time_ms,
                               const cbtmr_mode_t cbtmr_mode,
                               u8 *cbtmr_id)
{
  //printf("calling cbtmr_reg_callback...\n");
    ErrorStatus error = FUNC_ERROR;
    u8 slot_nbr = 0;

    //stop systick
    ctrl_systick_status(DISABLED);

    //find next available slot number
    while ( (MAX_NBR_OF_CALLBACKS > slot_nbr) &&
            (cbtmr_register[slot_nbr].in_use) )
    {
        slot_nbr++;
    }

    if (MAX_NBR_OF_CALLBACKS > slot_nbr)
    {
        /* setup the unused slot*/
        cbtmr_register[slot_nbr].in_use = TRUE;
        cbtmr_register[slot_nbr].id = last_id++;
        cbtmr_register[slot_nbr].trig_lvl = cbtmr_time_ms;
        cbtmr_register[slot_nbr].count = 0UL;
        cbtmr_register[slot_nbr].cbtmr_mode = cbtmr_mode;
        cbtmr_register[slot_nbr].cbtmr_flag = cbtmr_flag;
        cbtmr_register[slot_nbr].cbtmr_func = cbtmr_func;

        /* save callback id */
        if (NULL != cbtmr_id)
        {
            *cbtmr_id = cbtmr_register[slot_nbr].id;
        }

        /* slot available */
        error = FUNC_SUCCESS;
    }
    else
    {
      /* keep error status */
    }

    /* start systick */
    ctrl_systick_status(ENABLE);

    return (error);

} /* cbtmr_reg_callback */

//------------------------------------------------------------------
/// @fn      cbtmr_unreg_callback
/// @brief   Unregisters the callback with supplied id
/// @details Unregister a callback with callback id #
/// @param   cbtmrID [in] Callback id.
/// @param   [out] None
/// @return  error status flag
//------------------------------------------------------------------
ErrorStatus cbtmr_unreg_callback(const u8 UnregID)
{
    ErrorStatus error = FUNC_ERROR;

    //stop systick
    ctrl_systick_status(DISABLED);

    for(u8 slot_nbr = 0U; slot_nbr < MAX_NBR_OF_CALLBACKS; slot_nbr++)
    {
        if(TRUE == cbtmr_register[slot_nbr].in_use)
        {
            if(UnregID == cbtmr_register[slot_nbr].id)
            {
                cbtmr_register[slot_nbr].in_use = FALSE;
                cbtmr_register[slot_nbr].cbtmr_func = NULL;
                error = FUNC_SUCCESS;
            }
            else
            {
              //this is not the ID you're looking for
            }
        }
        else
        {
          //cannot unregister an ID that is not in use
        }
    }

    //start systick
    ctrl_systick_status(ENABLE);

    return (error);

} /* cbtmr_unreg_callback */

//------------------------------------------------------------------
/// @fn      cbtmr_reset_timer
/// @brief   Reset the timer for callback id #
/// @param   ResetTimerID [in] Callback id.
/// @param   [out] None
/// @return  None
//------------------------------------------------------------------
void cbtmr_reset_timer(const u8 ResetTimerID)
{
    //stop systick
    ctrl_systick_status(DISABLED);

    for(u8 slot_nbr = 0U; slot_nbr < MAX_NBR_OF_CALLBACKS; slot_nbr++)
    {
        if(TRUE == cbtmr_register[slot_nbr].in_use)
        {
            if(ResetTimerID == cbtmr_register[slot_nbr].id)
            {
                cbtmr_register[slot_nbr].count = 0UL;
            }
            else
            {
              //this is not the id you're looking for
            }
        }
        else
        {
          //connect reset the timmer for an ID that is not in use
        }
    }

    //start systick
    ctrl_systick_status(ENABLE);

} /* cbtmr_reset_timer */

/***************************************************************************//**
Not used
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t systick_init(void)
{
  //_DO_NOTHING;
    /* max reload value @ 148 MHz core clk = 100 ms */
  SYST_RVR = 148000U;
  SYST_CSR = SysTick_CSR_TICKINT_MASK | SysTick_CSR_CLKSOURCE_MASK;    
  
  return 1;
}

// End of file -----------------------------------------------------
/// @}
