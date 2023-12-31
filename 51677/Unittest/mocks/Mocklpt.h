/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKLPT_H
#define _MOCKLPT_H

#include "Types.h"
#include "lpt.h"

void Mocklpt_Init(void);
void Mocklpt_Destroy(void);
void Mocklpt_Verify(void);




#define lpt_init_IgnoreAndReturn(cmock_retval) lpt_init_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void lpt_init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define lpt_init_ExpectAndReturn(cmock_retval) lpt_init_CMockExpectAndReturn(__LINE__, cmock_retval)
void lpt_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_lpt_init_CALLBACK)(int cmock_num_calls);
void lpt_init_StubWithCallback(CMOCK_lpt_init_CALLBACK Callback);
#define reset_pulse_cnt_IgnoreAndReturn(cmock_retval) reset_pulse_cnt_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void reset_pulse_cnt_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define reset_pulse_cnt_ExpectAndReturn(cmock_retval) reset_pulse_cnt_CMockExpectAndReturn(__LINE__, cmock_retval)
void reset_pulse_cnt_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_reset_pulse_cnt_CALLBACK)(int cmock_num_calls);
void reset_pulse_cnt_StubWithCallback(CMOCK_reset_pulse_cnt_CALLBACK Callback);

#endif
