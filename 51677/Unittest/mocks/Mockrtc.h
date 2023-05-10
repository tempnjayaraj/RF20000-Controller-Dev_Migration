/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKRTC_H
#define _MOCKRTC_H

#include "Types.h"
#include "rtc.h"

void Mockrtc_Init(void);
void Mockrtc_Destroy(void);
void Mockrtc_Verify(void);




#define rtc_init_IgnoreAndReturn(cmock_retval) rtc_init_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void rtc_init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
#define rtc_init_ExpectAndReturn(cmock_retval) rtc_init_CMockExpectAndReturn(__LINE__, cmock_retval)
void rtc_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
typedef uint32_t (* CMOCK_rtc_init_CALLBACK)(int cmock_num_calls);
void rtc_init_StubWithCallback(CMOCK_rtc_init_CALLBACK Callback);
#define rtc_check_errors_Ignore() rtc_check_errors_CMockIgnore(__LINE__)
void rtc_check_errors_CMockIgnore(UNITY_LINE_TYPE cmock_line);
#define rtc_check_errors_Expect() rtc_check_errors_CMockExpect(__LINE__)
void rtc_check_errors_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_rtc_check_errors_CALLBACK)(int cmock_num_calls);
void rtc_check_errors_StubWithCallback(CMOCK_rtc_check_errors_CALLBACK Callback);

#endif
