/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKCRC_H
#define _MOCKCRC_H

#include "Types.h"
#include "crc.h"

void Mockcrc_Init(void);
void Mockcrc_Destroy(void);
void Mockcrc_Verify(void);




#define slow_crc16_IgnoreAndReturn(cmock_retval) slow_crc16_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void slow_crc16_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint16_t cmock_to_return);
#define slow_crc16_ExpectAndReturn(cmock_arg1, cmock_arg2, cmock_arg3, cmock_retval) slow_crc16_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_arg2, cmock_arg3, cmock_retval)
void slow_crc16_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint16_t cmock_arg1, uint8_t* cmock_arg2, uint32_t cmock_arg3, uint16_t cmock_to_return);
typedef uint16_t (* CMOCK_slow_crc16_CALLBACK)(uint16_t cmock_arg1, uint8_t* cmock_arg2, uint32_t cmock_arg3, int cmock_num_calls);
void slow_crc16_StubWithCallback(CMOCK_slow_crc16_CALLBACK Callback);
#define fast_crc16_IgnoreAndReturn(cmock_retval) fast_crc16_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fast_crc16_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint16_t cmock_to_return);
#define fast_crc16_ExpectAndReturn(cmock_arg1, cmock_arg2, cmock_arg3, cmock_retval) fast_crc16_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_arg2, cmock_arg3, cmock_retval)
void fast_crc16_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint16_t cmock_arg1, uint8_t* cmock_arg2, uint32_t cmock_arg3, uint16_t cmock_to_return);
typedef uint16_t (* CMOCK_fast_crc16_CALLBACK)(uint16_t cmock_arg1, uint8_t* cmock_arg2, uint32_t cmock_arg3, int cmock_num_calls);
void fast_crc16_StubWithCallback(CMOCK_fast_crc16_CALLBACK Callback);

#endif
