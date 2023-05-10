#include "unity.h"
#include "MK26F18.h"
#include "system.h"
#include "Settings.h"
#include <string.h>
#include "cpu.h"
#include "usb.h"
#include "usb_cdc.h"
#include "ring_buffer.h"

uint8_t *bdm_msg_out;
extern tUSB_Setup               *Setup_Pkt;
extern uint8_t 					gu8HALT_EP;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_poll_usb(void)
{
	poll_usb();
}

void test_USB_Testing_Read_Registers(void)
{
	USB_Testing_Read_Registers();
}

void test_USB_Testing_Write_Registers(void)
{
	USB_Testing_Write_Registers();
}

void test_USB_Init(void)
{
	//USB_Init();
}



void test_EP_OUT_Transfer(void)
{
	uint8_t u8EP;
	uint8_t *pu8DataPointer;
	EP_OUT_Transfer(u8EP,pu8DataPointer);
}

void test_USB_EP_OUT_SizeCheck(void)
{
	uint8_t u8EP;
	USB_EP_OUT_SizeCheck(u8EP);
}

void test_USB_Set_Interface(void)
{
	USB_Set_Interface();
}

void test_USB_StdReq_Handler(void)
{
	USB_StdReq_Handler();
}

void test_USB_StdReq_Handler_cases(void)
{
	Setup_Pkt->bRequest = mSET_ADDRESS;
	USB_StdReq_Handler();
	
	Setup_Pkt->bRequest = mGET_DESC;
	USB_StdReq_Handler();
	
	Setup_Pkt->bRequest = mGET_DESC;
	Setup_Pkt->wValue_h = mDEVICE;
	USB_StdReq_Handler();
	
	Setup_Pkt->bRequest = mGET_DESC;
	Setup_Pkt->wValue_h = mCONFIGURATION;
	USB_StdReq_Handler();
	
	Setup_Pkt->bRequest = mGET_DESC;
	Setup_Pkt->wValue_h = mSTRING;
	USB_StdReq_Handler();
	
	Setup_Pkt->bRequest = mSET_CONFIG;
	Setup_Pkt->wValue_h = 1;
	Setup_Pkt->wValue_l;
	USB_StdReq_Handler();
	
	Setup_Pkt->bRequest = mGET_CONFIG;
	USB_StdReq_Handler();
	
	Setup_Pkt->bRequest = mGET_STATUS;
	USB_StdReq_Handler();
}

void test_USB_Setup_Handler(void)
{
	USB_Setup_Handler();
	
	Setup_Pkt->bmRequestType = DEVICE_REQ;
	USB_Setup_Handler();
	
	Setup_Pkt->bmRequestType = INTERFACE_REQ;
	USB_Setup_Handler();
	
	Setup_Pkt->bmRequestType = INTERFACE_REQ;
	Setup_Pkt->bRequest = SET_LINE_CODING;
	CDC_InterfaceReq_Handler();
	USB_Setup_Handler();
	
	Setup_Pkt->bmRequestType = ENDPOINT_REQ;
	USB_Setup_Handler();
	
	Setup_Pkt->bmRequestType = 0xFF;
	USB_Setup_Handler();
}

void test_USB_Endpoint_Setup_Handler(void)
{
	USB_Endpoint_Setup_Handler();
	
	Setup_Pkt->bRequest = GET_STATUS;
	USB_Endpoint_Setup_Handler();
	
	FLAG_SET(Setup_Pkt->wIndex_h,gu8HALT_EP);
	USB_Endpoint_Setup_Handler();
	
	Setup_Pkt->bRequest = CLEAR_FEATURE;
	USB_Endpoint_Setup_Handler();
	
	Setup_Pkt->bRequest = SET_FEATURE;
	USB_Endpoint_Setup_Handler();
}

void test_USB_Handler(void)
{
	USB_Handler();
	
	USB0_STAT = 0x38;
	USB_Handler();
	
	USB0_STAT = 0x30;
	USB_Handler();
	
	USB0_STAT = 0;
	USB_Handler();
}

void test_USB_EP0_IN_Handler(void)
{
	USB_EP0_IN_Handler();
}

void test_USB_EP0_Stall(void)
{
	USB_EP0_Stall();
}

void test_USB_EP0_OUT_Handler(void)
{
	USB_EP0_OUT_Handler();
}

void test_USB_Stall_Handler(void)
{
	USB_Stall_Handler();
}

void test_USB_Reset_Handler(void)
{
	USB_Reset_Handler();
}

void test_usb_isr(void)
{
	usb_isr();
	
	FLAG_CLR(USB_ISTAT_USBRST_SHIFT,USB0_ISTAT);
	FLAG_SET(USB_ISTAT_SOFTOK_SHIFT,USB0_ISTAT);
	usb_isr();
	
	FLAG_CLR(USB_ISTAT_SOFTOK_SHIFT,USB0_ISTAT);
	FLAG_SET(USB_ISTAT_STALL_SHIFT,USB0_ISTAT);
	usb_isr();
	
	FLAG_SET(USB_ISTAT_TOKDNE_SHIFT,USB0_ISTAT);
	usb_isr();
	
	FLAG_SET(USB_ISTAT_SLEEP_SHIFT,USB0_ISTAT);
	usb_isr();
	
	FLAG_SET(USB_ISTAT_ERROR_SHIFT,USB0_ISTAT);
	usb_isr();
	
}

void test_rand_num(void)
{
	uint32_t return_value;
	return_value = rand_num();
}

void test_d_atoc(void)
{
	unsigned char return_value;
	unsigned char *s;
	*s = 'A';
	return_value = d_atoc(s);
}

void test_EP_IN_Transfer(void)
{
	uint8_t u8EP;
	uint8_t *pu8DataPointer;
	uint8_t *temp;
	uint8_t u8DataSize;
	EP_IN_Transfer(u8EP,pu8DataPointer,u8DataSize);
	
	u8DataSize = 8;
	u8EP = 2;
	temp = pu8DataPointer;
	EP_IN_Transfer(u8EP,pu8DataPointer,u8DataSize);
	pu8DataPointer = temp;
	
	*pu8DataPointer = 0xFC;
	EP_IN_Transfer(u8EP,pu8DataPointer,u8DataSize);
	pu8DataPointer = temp;
	
	
}


void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
