#ifndef __CircBuffer__
#define __CircBuffer__

/* Includes */
//#include "arm_cm4.h"
#include <stdint.h>

/* Error Codes */
#define     OK                  0
#define     NOT_ENOUGH_SPACE    1


/* Extern variables */
volatile extern uint8_t *OUT_StartAddress;        
volatile extern uint8_t *OUT_EndAddress;          
volatile extern uint8_t *OUT_UsbPointer;          
volatile extern uint8_t *OUT_SciPointer;          
volatile extern uint8_t gu8BufferOverFlow;


/* Prototypes */
void Buffer_Init(uint8_t* pu8BufferPointer ,uint8_t u8MaxSize);
uint8_t Buffer_Request(uint8_t* pu8DataPointer ,uint16_t u8RequestSize);


#endif /* __CircBuffer__*/