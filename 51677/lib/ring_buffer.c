#include "ring_buffer.h"

/* Pointers */
volatile uint8_t *OUT_StartAddress;
volatile uint8_t *OUT_EndAddress;
volatile uint8_t *OUT_UsbPointer;
volatile uint8_t *OUT_SciPointer;

/* Variables */
volatile uint8_t gu8BufferMaxSize;
volatile uint8_t gu8BufferOverFlow;


/********************************************************/
void Buffer_Init(uint8_t* pu8BufferPointer ,uint8_t u8MaxSize)
{
    /* Buffer Initialization */
    OUT_EndAddress = pu8BufferPointer + u8MaxSize - 1;
    OUT_StartAddress = pu8BufferPointer;
    gu8BufferMaxSize = u8MaxSize;
    OUT_UsbPointer = pu8BufferPointer;
    OUT_SciPointer = pu8BufferPointer;
    gu8BufferOverFlow = 0;
}


/********************************************************/
uint8_t Buffer_Request(uint8_t* pu8DataPointer ,uint16_t u8RequestSize)
{
    uint8_t u8FreeSpace;
    
    // Check for OverFlow
    
    if(gu8BufferOverFlow)
        return(NOT_ENOUGH_SPACE);
    
    
    // Calculate Free Space
    if(OUT_UsbPointer < OUT_SciPointer)
        u8FreeSpace=(uint8_t)(OUT_SciPointer-OUT_UsbPointer);
    
    else
        u8FreeSpace=gu8BufferMaxSize-(OUT_UsbPointer-OUT_SciPointer);
    
    // Validate requested size
    if(u8FreeSpace<u8RequestSize)
        return(NOT_ENOUGH_SPACE);
    
    
    if(u8FreeSpace==u8RequestSize)
        gu8BufferOverFlow=1;
    
    // Buffer Copy
    while(u8RequestSize--)
    {
        *OUT_UsbPointer=*pu8DataPointer;
        OUT_UsbPointer++;
        pu8DataPointer++;
        if(OUT_UsbPointer > OUT_EndAddress)
        {
            OUT_UsbPointer=OUT_StartAddress;
        }
    }
    return(OK);
}