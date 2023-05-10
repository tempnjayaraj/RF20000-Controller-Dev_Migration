/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockstate_machine.h"

typedef struct _CMOCK_do_nothing_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;

} CMOCK_do_nothing_CALL_INSTANCE;

typedef struct _CMOCK_init_shutdown_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;

} CMOCK_init_shutdown_CALL_INSTANCE;

typedef struct _CMOCK_state_switch_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;

} CMOCK_state_switch_CALL_INSTANCE;

static struct Mockstate_machineInstance
{
  int do_nothing_IgnoreBool;
  uint8_t do_nothing_FinalReturn;
  CMOCK_do_nothing_CALLBACK do_nothing_CallbackFunctionPointer;
  int do_nothing_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE do_nothing_CallInstance;
  int init_shutdown_IgnoreBool;
  uint8_t init_shutdown_FinalReturn;
  CMOCK_init_shutdown_CALLBACK init_shutdown_CallbackFunctionPointer;
  int init_shutdown_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE init_shutdown_CallInstance;
  int state_switch_IgnoreBool;
  uint8_t state_switch_FinalReturn;
  CMOCK_state_switch_CALLBACK state_switch_CallbackFunctionPointer;
  int state_switch_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE state_switch_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockstate_machine_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  if (Mock.do_nothing_IgnoreBool)
    Mock.do_nothing_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.do_nothing_CallInstance, cmock_line, "Function 'do_nothing' called less times than expected.");
  if (Mock.do_nothing_CallbackFunctionPointer != NULL)
    Mock.do_nothing_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.init_shutdown_IgnoreBool)
    Mock.init_shutdown_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.init_shutdown_CallInstance, cmock_line, "Function 'init_shutdown' called less times than expected.");
  if (Mock.init_shutdown_CallbackFunctionPointer != NULL)
    Mock.init_shutdown_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.state_switch_IgnoreBool)
    Mock.state_switch_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.state_switch_CallInstance, cmock_line, "Function 'state_switch' called less times than expected.");
  if (Mock.state_switch_CallbackFunctionPointer != NULL)
    Mock.state_switch_CallInstance = CMOCK_GUTS_NONE;
}

void Mockstate_machine_Init(void)
{
  Mockstate_machine_Destroy();
}

void Mockstate_machine_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.do_nothing_CallbackFunctionPointer = NULL;
  Mock.do_nothing_CallbackCalls = 0;
  Mock.init_shutdown_CallbackFunctionPointer = NULL;
  Mock.init_shutdown_CallbackCalls = 0;
  Mock.state_switch_CallbackFunctionPointer = NULL;
  Mock.state_switch_CallbackCalls = 0;
}

uint8_t do_nothing(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_do_nothing_CALL_INSTANCE* cmock_call_instance = (CMOCK_do_nothing_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.do_nothing_CallInstance);
  Mock.do_nothing_CallInstance = CMock_Guts_MemNext(Mock.do_nothing_CallInstance);
  if (Mock.do_nothing_CallbackFunctionPointer != NULL)
  {
    return Mock.do_nothing_CallbackFunctionPointer(Mock.do_nothing_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'do_nothing' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.do_nothing_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.do_nothing_FinalReturn;
    Mock.do_nothing_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  return cmock_call_instance->ReturnVal;
}

void do_nothing_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_do_nothing_CALL_INSTANCE));
  CMOCK_do_nothing_CALL_INSTANCE* cmock_call_instance = (CMOCK_do_nothing_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.do_nothing_CallInstance = CMock_Guts_MemChain(Mock.do_nothing_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.do_nothing_IgnoreBool = (int)1;
}

void do_nothing_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_do_nothing_CALL_INSTANCE));
  CMOCK_do_nothing_CALL_INSTANCE* cmock_call_instance = (CMOCK_do_nothing_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.do_nothing_CallInstance = CMock_Guts_MemChain(Mock.do_nothing_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void do_nothing_StubWithCallback(CMOCK_do_nothing_CALLBACK Callback)
{
  Mock.do_nothing_CallbackFunctionPointer = Callback;
}

uint8_t init_shutdown(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_init_shutdown_CALL_INSTANCE* cmock_call_instance = (CMOCK_init_shutdown_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.init_shutdown_CallInstance);
  Mock.init_shutdown_CallInstance = CMock_Guts_MemNext(Mock.init_shutdown_CallInstance);
  if (Mock.init_shutdown_CallbackFunctionPointer != NULL)
  {
    return Mock.init_shutdown_CallbackFunctionPointer(Mock.init_shutdown_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'init_shutdown' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.init_shutdown_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.init_shutdown_FinalReturn;
    Mock.init_shutdown_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  return cmock_call_instance->ReturnVal;
}

void init_shutdown_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_init_shutdown_CALL_INSTANCE));
  CMOCK_init_shutdown_CALL_INSTANCE* cmock_call_instance = (CMOCK_init_shutdown_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.init_shutdown_CallInstance = CMock_Guts_MemChain(Mock.init_shutdown_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.init_shutdown_IgnoreBool = (int)1;
}

void init_shutdown_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_init_shutdown_CALL_INSTANCE));
  CMOCK_init_shutdown_CALL_INSTANCE* cmock_call_instance = (CMOCK_init_shutdown_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.init_shutdown_CallInstance = CMock_Guts_MemChain(Mock.init_shutdown_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void init_shutdown_StubWithCallback(CMOCK_init_shutdown_CALLBACK Callback)
{
  Mock.init_shutdown_CallbackFunctionPointer = Callback;
}

uint8_t state_switch(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_state_switch_CALL_INSTANCE* cmock_call_instance = (CMOCK_state_switch_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.state_switch_CallInstance);
  Mock.state_switch_CallInstance = CMock_Guts_MemNext(Mock.state_switch_CallInstance);
  if (Mock.state_switch_CallbackFunctionPointer != NULL)
  {
    return Mock.state_switch_CallbackFunctionPointer(Mock.state_switch_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'state_switch' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.state_switch_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.state_switch_FinalReturn;
    Mock.state_switch_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  return cmock_call_instance->ReturnVal;
}

void state_switch_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_state_switch_CALL_INSTANCE));
  CMOCK_state_switch_CALL_INSTANCE* cmock_call_instance = (CMOCK_state_switch_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.state_switch_CallInstance = CMock_Guts_MemChain(Mock.state_switch_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.state_switch_IgnoreBool = (int)1;
}

void state_switch_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_state_switch_CALL_INSTANCE));
  CMOCK_state_switch_CALL_INSTANCE* cmock_call_instance = (CMOCK_state_switch_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.state_switch_CallInstance = CMock_Guts_MemChain(Mock.state_switch_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void state_switch_StubWithCallback(CMOCK_state_switch_CALLBACK Callback)
{
  Mock.state_switch_CallbackFunctionPointer = Callback;
}
