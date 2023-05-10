#include "unity.h"
#include "MK26F18.h"
#include "Settings.h"
#include "system.h"
#include <string.h>
#include "cpu.h"
#include "usb.h"
#include "usb_cdc.h"
#include "ring_buffer.h"

/* CDC Global Structures */
// extern CDC_Line_Coding LineCoding;
//extern uint8_t u8CDCState=WAITING_FOR_ENUMERATION;
//extern uint8_t CDC_OUT_Data[CDC_BUFFER_SIZE];
extern uint8_t                  gu8USB_State;
extern uint8_t                  gu8USB_Flags;
extern tUSB_Setup               *Setup_Pkt;

uint8_t *bdm_msg_out;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Buffer_Init(void)
{
	uint8_t* pu8BufferPointer;
	uint8_t u8MaxSize = 64;
	Buffer_Init(pu8BufferPointer,u8MaxSize);
}

void test_Buffer_Request1(void)
{
	uint8_t return_value = 0;
	uint8_t* pu8DataPointer;
	uint16_t u8RequestSize = 8;
	return_value = Buffer_Request(pu8DataPointer ,u8RequestSize);
}

void test_Buffer_Request2(void)
{
	uint8_t return_value = 0;
	gu8BufferOverFlow = TRUE;
	uint8_t* pu8DataPointer;
	uint16_t u8RequestSize = 8;
	return_value = Buffer_Request(pu8DataPointer ,u8RequestSize);
}

void test_Buffer_Request3(void)
{
	gu8BufferOverFlow = FALSE;
	uint8_t return_value = 0;
	uint8_t* pu8BufferPointer;
	uint8_t* pu8DataPointer;
	uint8_t u8MaxSize = 8;
	uint16_t u8RequestSize = 8;
	*pu8BufferPointer = 0;
	Buffer_Init(pu8BufferPointer,u8MaxSize);
	return_value = Buffer_Request(pu8DataPointer ,u8RequestSize);
}

void test_Buffer_Request4(void)
{
	gu8BufferOverFlow = FALSE;
	uint8_t return_value = 0;
	uint8_t* pu8BufferPointer;
	uint8_t* pu8DataPointer;
	uint8_t u8MaxSize = 6;
	uint16_t u8RequestSize = 8;
	*pu8BufferPointer = 0;
	Buffer_Init(pu8BufferPointer,u8MaxSize);
	return_value = Buffer_Request(pu8DataPointer ,u8RequestSize);
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
