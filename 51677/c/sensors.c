/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2798                 $:
$Date:: 2021-11-17 02:10:26#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     sensors.c
\brief     Driver system sensors door sensor,tube sensor and pump sensor.
\details   N/A
*******************************************************************************/

#include "MK26F18.h"
#include "sensors.h"
#include "system.h"
#include "led.h"
#include "wand.h"


extern manta_prime prime;
extern prime_mode_t PrimeState;
/***************************************************************************//**
Initializes door sensor.
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t door_sensor_init(void)
{
 
  /* Enable gate clock */
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
  
  /* Configure port pin control registers */
  PORTA_PCR5 = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK;
  
  /* Configure  pins as inputs */
  GPIOA_PDDR &= ~GPIO_PDDR_PDD(DOOR_SENSOR_MASK);
  
  return 1;
  
}

/***************************************************************************//**
Initializes tube sensor.
@param void
@return void
@outputs
@context
*******************************************************************************/
uint8_t tube_sensors_init(void)
{
 
  /* Enable gate clock */
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  
  /* Configure port pin control registers */
  PORTC_PCR5 = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK;
  PORTC_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK;
  
  /* Configure  pins as inputs */
  GPIOA_PDDR &= ~GPIO_PDDR_PDD(SUCTION_TUBE_SENSOR_MASK | DISCHARGE_TUBE_SENSOR_MASK);
  
  return 1;
  
}

/***************************************************************************//**
Initializes pump sensor.
@param void
@return 0
@outputs
@context
*******************************************************************************/
uint8_t poll_pump_sensors(void)
{
   
   uint8_t door, discharge_tube, suction_tube;
   static uint8_t insert_tube_status_change_filter = 0,
                  close_pump_door_status_change_filter = 0,
                  open_pump_door_status_change_filter = 0,
                  error_status_change_filter = 0,
                  sensor_ok_status_change_filter = 0;
//   
//   const uint16_t truth_table[2][2][2] = {
//     {{TUBE_ABSENT,       ERROR},
//      { DOOR_CLOSE, TUBE_ABSENT}},
//     {{  DOOR_OPEN,       ERROR},
//      {TUBE_ABSENT,  SENSORS_OK}}
//   };

/*******************************************************************************
 *                               truth table                                   *
 *******************************************************************************
*      pump |         |        |        |
       on   | outtube | intube |  door  | 	
*       0   |	0     |	   0   |   0    | TUBE_ABSENT
*       0   |	0     |	   0   |   1    | DOOR_CLOSE
*       0   |	0     |	   1   |   0    | TUBE_ABSENT
*       0   |	0     |	   1   |   1    | TUBE_ABSENT
*       0   |	1     |	   0   |   0    | TUBE_ABSENT
*       0   |	1     |	   0   |   1    | TUBE_ABSENT
*       0   |	1     |	   1   |   0    | DOOR_OPEN
*       0   |	1     |	   1   |   1    | SENSORS_OK
*       1   |	0     |	   0   |   0    | ERROR
*       1   |	0     |	   0   |   1    | ERROR
*       1   |	0     |	   1   |   0    | DOOR_OPEN
*       1   |	0     |	   1   |   1    | SENSORS_OK
*       1   |	1     |	   0   |   0    | ERROR
*       1   |	1     |	   0   |   1    | ERROR
*       1   |	1     |	   1   |   0    | DOOR_OPEN
*       1   |	1     |	   1   |   1    | SENSORS_OK
 ******************************************************************************/
   
   const uint16_t truth_table[2][2][2][2] =
   {   
    TUBE_ABSENT, 
    DOOR_CLOSE,  
    TUBE_ABSENT, 
    TUBE_ABSENT, 
    TUBE_ABSENT, 
    TUBE_ABSENT, 
    DOOR_OPEN,   
    SENSORS_OK,  
    ERROR,       
    ERROR,  
    DOOR_OPEN,
    SENSORS_OK,
    ERROR,
    ERROR,
    DOOR_OPEN,
    SENSORS_OK,
   };
   
   uint16_t status = 0;
   
   door = ~(GPIOA_PDIR >> DOOR_SENSOR_SHIFT) & 1;
   discharge_tube = ~(GPIOC_PDIR >> DISCHARGE_TUBE_SENSOR_SHIFT) & 1;
   suction_tube = ~(GPIOC_PDIR >> SUCTION_TUBE_SENSOR_SHIFT) & 1;
   //previous_status = (current_therapy_settings.pump.status & SENSOR_STATES);

   /* 0.5V threshold */
   __istate_t s = __get_interrupt_state();
   __disable_interrupt();
     
   if (door == 1)
   {
     
     current_therapy_settings.pump.sensor_status |= DOOR_SENSOR_ON;
     
   }
   else
   {
     
     current_therapy_settings.pump.sensor_status &= ~DOOR_SENSOR_ON;
     
   }
   
   if (discharge_tube == 1)
   {
     
     current_therapy_settings.pump.sensor_status |= DISCHARGE_TUBE_SENSOR_ON;
     
   }
   else
   {
     
     current_therapy_settings.pump.sensor_status &= ~DISCHARGE_TUBE_SENSOR_ON;
     
   }
   
   
   if (suction_tube == 1)
   {
     
     current_therapy_settings.pump.sensor_status |= SUCTION_TUBE_SENSOR_ON;
     
   }
   else
   {
     
     current_therapy_settings.pump.sensor_status &= ~SUCTION_TUBE_SENSOR_ON;
     
//     if (current_therapy_settings.pump.on == 1)
//     {
//       
//       put_event(&evt_tbl,SUCTION_TUBE_ERROR);
//             
//     }
     
   }
   
  
   if (((current_therapy_settings.pump.sensor_status & SUCTION_TUBE_SENSOR_ON) == SUCTION_TUBE_SENSOR_ON) &&
       ((current_therapy_settings.pump.sensor_status & DISCHARGE_TUBE_SENSOR_ON) == DISCHARGE_TUBE_SENSOR_ON))
   {  

     current_therapy_settings.pump.tube_present = 1;
     
   }
   else
   {
          
     current_therapy_settings.pump.tube_present = 0;
          
   }
   
     
   if ((current_therapy_settings.pump.sensor_status & DOOR_SENSOR_ON) == DOOR_SENSOR_ON)
   {  

     current_therapy_settings.pump.door_present = 1;
     
   }
   else
   {
          
     current_therapy_settings.pump.door_present = 0;
     
   }   
 
  status = truth_table[current_therapy_settings.pump.on]
                      [discharge_tube]
                      [suction_tube]
                      [current_therapy_settings.pump.door_present];
     
   switch(status)
   {
     
    case(TUBE_ABSENT):
    {

      if (insert_tube_status_change_filter < 10)
      {

        insert_tube_status_change_filter++;

      }
      else
      {
        insert_tube_status_change_filter = 0;
        if (suction_tube == 0)
        {

          put_event(&evt_tbl,THERAPY_OFF);

        }
        current_therapy_settings.pump.status &= ~SENSOR_STATES;
        current_therapy_settings.pump.status |= TUBE_ABSENT;
        
      }
          
      close_pump_door_status_change_filter = 0;
      open_pump_door_status_change_filter = 0;
      error_status_change_filter = 0;
      sensor_ok_status_change_filter = 0;
      
      break;
    }
    
    case(DOOR_CLOSE):
    {
      
      if (open_pump_door_status_change_filter < 10)
      {

        open_pump_door_status_change_filter++;

      }
      else
      {
        open_pump_door_status_change_filter = 0;

        current_therapy_settings.pump.status &= ~SENSOR_STATES;
        current_therapy_settings.pump.status |= DOOR_CLOSE;
      }
        
      insert_tube_status_change_filter = 0;
      close_pump_door_status_change_filter = 0;
      error_status_change_filter = 0;
      sensor_ok_status_change_filter = 0;
      
      break;
    }
    
    case(DOOR_OPEN):
    {
      
      if (close_pump_door_status_change_filter < 10)
      {

        close_pump_door_status_change_filter++;

      }
      else
      {
        
        close_pump_door_status_change_filter = 0;
        if ((current_therapy_settings.pump.status & DOOR_OPEN) != DOOR_OPEN)
        {

          put_event(&evt_tbl,THERAPY_OFF);

        }
        current_therapy_settings.pump.status &= ~SENSOR_STATES;
        current_therapy_settings.pump.status |= DOOR_OPEN;
      }
      if(PrimeState == PRIME_MANUAL)
      {
        gui_stop_manual_prime(); 
      } 
      insert_tube_status_change_filter = 0;
      open_pump_door_status_change_filter = 0;
      error_status_change_filter = 0;
      sensor_ok_status_change_filter = 0;
      
      break;
      
    }
    
    case(SENSORS_OK):
    {
    
      if (sensor_ok_status_change_filter < 10)
      {

        sensor_ok_status_change_filter++;

      }
      else
      {
        sensor_ok_status_change_filter = 0;
        current_therapy_settings.pump.status &= ~SENSOR_STATES;
        current_therapy_settings.pump.status |= SENSORS_OK;
      }
        
      insert_tube_status_change_filter = 0;
      close_pump_door_status_change_filter = 0;
      open_pump_door_status_change_filter = 0;
      error_status_change_filter = 0;
      
      break;
            
    }
    
    case(ERROR):
    {
      
      if (error_status_change_filter < 10)
      {

        error_status_change_filter++;

      }
      else
      {
        error_status_change_filter = 0;          
        put_event(&evt_tbl,SUCTION_TUBE_ERROR);
        if(PrimeState == PRIME_MANUAL)
        {
          gui_stop_manual_prime(); 
        }
        current_therapy_settings.pump.status &= ~SENSOR_STATES;
        current_therapy_settings.pump.status |= ERROR;
      }
        
      insert_tube_status_change_filter = 0;
      close_pump_door_status_change_filter = 0;
      open_pump_door_status_change_filter = 0;
      sensor_ok_status_change_filter = 0;
      
      break;
    }
    
   }
   
//   if (((current_therapy_settings.pump.status & SENSOR_STATES) != previous_status) &&
//        (previous_status == ERROR))
//   {
//     
//     put_event(&evt_tbl,SUCTION_TUBE_ERROR);
//     
//   }
      
//   if (((current_therapy_settings.pump.tube_present == 0) &&
//      (current_therapy_settings.pump.door_present == 1) &&
//     ((current_therapy_settings.pump.sensor_status & 1) == 1)) ||
//     ((current_therapy_settings.pump.tube_present == 0) &&
//      (current_therapy_settings.pump.door_present == 0) &&
//     ((current_therapy_settings.pump.sensor_status & 1) == 0)) ||
//     ((current_therapy_settings.pump.tube_present == 1) &&
//      (current_therapy_settings.pump.door_present == 1) &&
//     ((current_therapy_settings.pump.sensor_status & 1) == 0)))
//  {
//    
//    
//      
//    if (insert_tube_status_change_filter < 20)
//    {
//
//      insert_tube_status_change_filter++;
//
//    }
//    else
//    {
//      insert_tube_status_change_filter = 20;
//      current_therapy_settings.pump.status &= ~SENSOR_STATES;
//      current_therapy_settings.pump.status |= TUBE_ABSENT;
//      
//    }
//        
//    close_pump_door_status_change_filter = 0;
//    open_pump_door_status_change_filter = 0;
//    error_status_change_filter = 0;
//      
//    
//    
//  }
//  
//  else if (((current_therapy_settings.pump.tube_present == 1) &&
//            (current_therapy_settings.pump.door_present == 0) &&
//           ((current_therapy_settings.pump.sensor_status & 1) == 0)))
//  {
//        
//       if (close_pump_door_status_change_filter < 20)
//      {
//
//        close_pump_door_status_change_filter++;
//
//      }
//      else
//      {
//        
//        close_pump_door_status_change_filter = 20;
//        if ((current_therapy_settings.pump.status & DOOR_OPEN) != DOOR_OPEN)
//        {
//
//          put_event(&evt_tbl,THERAPY_OFF);
//
//        }
//        current_therapy_settings.pump.status &= ~SENSOR_STATES;
//        current_therapy_settings.pump.status |= DOOR_OPEN;
//        
//      }
//        
//      insert_tube_status_change_filter = 0;
//      open_pump_door_status_change_filter = 0;
//      error_status_change_filter = 0;
//    
//  }
//  
//  else if (((current_therapy_settings.pump.tube_present == 0) &&
//           (current_therapy_settings.pump.door_present == 1) &&
//          ((current_therapy_settings.pump.sensor_status & 1) == 0)))
//  {
//
//      if (open_pump_door_status_change_filter < 20)
//      {
//
//        open_pump_door_status_change_filter++;
//
//      }
//      else
//      {
//        open_pump_door_status_change_filter = 20;
//        current_therapy_settings.pump.status &= ~SENSOR_STATES;
//        current_therapy_settings.pump.status |= DOOR_CLOSE;
//      }
//        
//      insert_tube_status_change_filter = 0;
//      close_pump_door_status_change_filter = 0;
//      error_status_change_filter = 0;
//    
//  }
//  
//  else if ((((current_therapy_settings.pump.tube_present == 0) &&
//            (current_therapy_settings.pump.door_present == 0) &&
//           ((current_therapy_settings.pump.sensor_status & 1) == 1)) ||
//           ((current_therapy_settings.pump.tube_present == 1) &&
//            (current_therapy_settings.pump.door_present == 0) &&
//           ((current_therapy_settings.pump.sensor_status & 1) == 1))))
//  {
//       
//      if (error_status_change_filter < 20)
//      {
//
//        error_status_change_filter++;
//
//      }
//      else
//      {
//        error_status_change_filter = 20;
//        if ((current_therapy_settings.pump.status & ERROR) != ERROR)
//        {
//          
//          //put_event(&evt_tbl,THERAPY_OFF);
//          put_event(&evt_tbl,SUCTION_TUBE_ERROR);
//          
//        }
//        current_therapy_settings.pump.status &= ~SENSOR_STATES;
//        current_therapy_settings.pump.status |= ERROR;
//      }
//        
//      insert_tube_status_change_filter = 0;
//      close_pump_door_status_change_filter = 0;
//      open_pump_door_status_change_filter = 0;
//    
//  }
//  else
//  {
//    
//    current_therapy_settings.pump.status &= ~SENSOR_STATES;
//    current_therapy_settings.pump.status |= SENSORS_OK;
//      
//    insert_tube_status_change_filter = 0;
//    close_pump_door_status_change_filter = 0;
//    open_pump_door_status_change_filter = 0;
//    error_status_change_filter = 0;
//      
//  }
  
  
  /* flag to shut off pump when sensor is not on */
//  if (((current_therapy_settings.pump.sensor_status & SUCTION_TUBE_SENSOR_ON) == SUCTION_TUBE_SENSOR_ON) &&
//     ((current_therapy_settings.pump.sensor_status & DOOR_SENSOR_ON) == DOOR_SENSOR_ON) &&
//     ((current_therapy_settings.pump.sensor_status & STATOR_SENSOR_ON) == STATOR_SENSOR_ON))
//  {
//    
//    current_therapy_settings.pump.sensor_check = TRUE;
//    
//  }
//  else
//  {
//    
//    current_therapy_settings.pump.sensor_check = FALSE;
//    
//  }
   
  __set_interrupt_state(s);
  
  return 0; 
   
  
}


  
  
