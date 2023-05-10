/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKBDM_WW_MAIN_H
#define _MOCKBDM_WW_MAIN_H

#include "Types.h"
#include "bdm_ww_main.h"

void Mockbdm_ww_main_Init(void);
void Mockbdm_ww_main_Destroy(void);
void Mockbdm_ww_main_Verify(void);




#define bdm_poll_usb_IgnoreAndReturn(cmock_retval) bdm_poll_usb_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void bdm_poll_usb_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define bdm_poll_usb_ExpectAndReturn(cmock_retval) bdm_poll_usb_CMockExpectAndReturn(__LINE__, cmock_retval)
void bdm_poll_usb_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_bdm_poll_usb_CALLBACK)(int cmock_num_calls);
void bdm_poll_usb_StubWithCallback(CMOCK_bdm_poll_usb_CALLBACK Callback);
#define in_bdm_wcd_msg_Ignore() in_bdm_wcd_msg_CMockIgnore(__LINE__)
void in_bdm_wcd_msg_CMockIgnore(UNITY_LINE_TYPE cmock_line);
#define in_bdm_wcd_msg_Expect() in_bdm_wcd_msg_CMockExpect(__LINE__)
void in_bdm_wcd_msg_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_in_bdm_wcd_msg_CALLBACK)(int cmock_num_calls);
void in_bdm_wcd_msg_StubWithCallback(CMOCK_in_bdm_wcd_msg_CALLBACK Callback);
#define prepare_therapy_message_IgnoreAndReturn(cmock_retval) prepare_therapy_message_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void prepare_therapy_message_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define prepare_therapy_message_ExpectAndReturn(cmock_retval) prepare_therapy_message_CMockExpectAndReturn(__LINE__, cmock_retval)
void prepare_therapy_message_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
typedef bool (* CMOCK_prepare_therapy_message_CALLBACK)(int cmock_num_calls);
void prepare_therapy_message_StubWithCallback(CMOCK_prepare_therapy_message_CALLBACK Callback);
#define prepare_port_message_IgnoreAndReturn(cmock_retval) prepare_port_message_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void prepare_port_message_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define prepare_port_message_ExpectAndReturn(cmock_retval) prepare_port_message_CMockExpectAndReturn(__LINE__, cmock_retval)
void prepare_port_message_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
typedef bool (* CMOCK_prepare_port_message_CALLBACK)(int cmock_num_calls);
void prepare_port_message_StubWithCallback(CMOCK_prepare_port_message_CALLBACK Callback);

#endif