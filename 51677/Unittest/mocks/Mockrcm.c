/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockrcm.h"

typedef struct _CMOCK_reset_monitor_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;

} CMOCK_reset_monitor_CALL_INSTANCE;

typedef struct _CMOCK_software_reset_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_software_reset_CALL_INSTANCE;

static struct MockrcmInstance
{
  int reset_monitor_IgnoreBool;
  uint8_t reset_monitor_FinalReturn;
  CMOCK_reset_monitor_CALLBACK reset_monitor_CallbackFunctionPointer;
  int reset_monitor_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE reset_monitor_CallInstance;
  int software_reset_IgnoreBool;
  CMOCK_software_reset_CALLBACK software_reset_CallbackFunctionPointer;
  int software_reset_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE software_reset_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockrcm_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  if (Mock.reset_monitor_IgnoreBool)
    Mock.reset_monitor_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.reset_monitor_CallInstance, cmock_line, "Function 'reset_monitor' called less times than expected.");
  if (Mock.reset_monitor_CallbackFunctionPointer != NULL)
    Mock.reset_monitor_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.software_reset_IgnoreBool)
    Mock.software_reset_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.software_reset_CallInstance, cmock_line, "Function 'software_reset' called less times than expected.");
  if (Mock.software_reset_CallbackFunctionPointer != NULL)
    Mock.software_reset_CallInstance = CMOCK_GUTS_NONE;
}

void Mockrcm_Init(void)
{
  Mockrcm_Destroy();
}

void Mockrcm_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.reset_monitor_CallbackFunctionPointer = NULL;
  Mock.reset_monitor_CallbackCalls = 0;
  Mock.software_reset_CallbackFunctionPointer = NULL;
  Mock.software_reset_CallbackCalls = 0;
}

uint8_t reset_monitor(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_reset_monitor_CALL_INSTANCE* cmock_call_instance = (CMOCK_reset_monitor_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.reset_monitor_CallInstance);
  Mock.reset_monitor_CallInstance = CMock_Guts_MemNext(Mock.reset_monitor_CallInstance);
  if (Mock.reset_monitor_CallbackFunctionPointer != NULL)
  {
    return Mock.reset_monitor_CallbackFunctionPointer(Mock.reset_monitor_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'reset_monitor' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.reset_monitor_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.reset_monitor_FinalReturn;
    Mock.reset_monitor_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  return cmock_call_instance->ReturnVal;
}

void reset_monitor_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_reset_monitor_CALL_INSTANCE));
  CMOCK_reset_monitor_CALL_INSTANCE* cmock_call_instance = (CMOCK_reset_monitor_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.reset_monitor_CallInstance = CMock_Guts_MemChain(Mock.reset_monitor_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.reset_monitor_IgnoreBool = (int)1;
}

void reset_monitor_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_reset_monitor_CALL_INSTANCE));
  CMOCK_reset_monitor_CALL_INSTANCE* cmock_call_instance = (CMOCK_reset_monitor_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.reset_monitor_CallInstance = CMock_Guts_MemChain(Mock.reset_monitor_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void reset_monitor_StubWithCallback(CMOCK_reset_monitor_CALLBACK Callback)
{
  Mock.reset_monitor_CallbackFunctionPointer = Callback;
}

void software_reset(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_software_reset_CALL_INSTANCE* cmock_call_instance = (CMOCK_software_reset_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.software_reset_CallInstance);
  Mock.software_reset_CallInstance = CMock_Guts_MemNext(Mock.software_reset_CallInstance);
  if (Mock.software_reset_CallbackFunctionPointer != NULL)
  {
    Mock.software_reset_CallbackFunctionPointer(Mock.software_reset_CallbackCalls++);
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'software_reset' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.software_reset_IgnoreBool)
  {
    return;
  }
}

void software_reset_CMockIgnore(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_software_reset_CALL_INSTANCE));
  CMOCK_software_reset_CALL_INSTANCE* cmock_call_instance = (CMOCK_software_reset_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.software_reset_CallInstance = CMock_Guts_MemChain(Mock.software_reset_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  Mock.software_reset_IgnoreBool = (int)1;
}

void software_reset_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_software_reset_CALL_INSTANCE));
  CMOCK_software_reset_CALL_INSTANCE* cmock_call_instance = (CMOCK_software_reset_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.software_reset_CallInstance = CMock_Guts_MemChain(Mock.software_reset_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void software_reset_StubWithCallback(CMOCK_software_reset_CALLBACK Callback)
{
  Mock.software_reset_CallbackFunctionPointer = Callback;
}

