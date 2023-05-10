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

void test_CDC_Engine1(void)
{
	u8CDCState = WAITING_FOR_ENUMERATION;
	CDC_Engine();
}

void test_CDC_Engine2(void)
{
	gu8USB_State = uPOWER;
	u8CDCState = WAITING_FOR_ENUMERATION;
	CDC_Engine();
}

void test_CDC_Engine3(void)
{
	u8CDCState = SET_LINE_CODING;
	CDC_Engine();
}

void test_CDC_Engine4(void)
{
	FLAG_SET(EP0,gu8USB_Flags);
	u8CDCState = SET_LINE_CODING;
	CDC_Engine();
}

void test_CDC_Engine5(void)
{
	u8CDCState = SET_CONTROL_LINE_STATE;
	CDC_Engine();
}

void test_CDC_Engine6(void)
{
	gu8USB_State = uENUMERATED;
	u8CDCState = WAITING_FOR_ENUMERATION;
	CDC_Engine();
}

void test_CDC_InterfaceReq_Handler1(void)
{
	Setup_Pkt->bRequest = GET_INTERFACE;
	CDC_InterfaceReq_Handler();
}

void test_CDC_InterfaceReq_Handler2(void)
{
	Setup_Pkt->bRequest = GET_LINE_CODING;
	CDC_InterfaceReq_Handler();
}

void test_CDC_InterfaceReq_Handler3(void)
{
	Setup_Pkt->bRequest = SET_LINE_CODING;
	CDC_InterfaceReq_Handler();
}

void test_CDC_InterfaceReq_Handler4(void)
{
	Setup_Pkt->bRequest = SET_CONTROL_LINE_STATE;
	CDC_InterfaceReq_Handler();
}

void test_CDC_InterfaceReq_Handler5(void)
{
	Setup_Pkt->bRequest = LOADER_MODE;
	CDC_InterfaceReq_Handler();
}

void test_LWordSwap(void)
{
	uint32_t return_value;
	uint32_t u32DataSwap = 0x00002244;
	return_value = LWordSwap(u32DataSwap);
	TEST_ASSERT_EQUAL(0x44220000,return_value);
}


void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
