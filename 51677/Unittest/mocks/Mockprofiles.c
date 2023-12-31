/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockprofiles.h"

typedef struct _CMOCK_profile_manager_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_profile_manager_CALL_INSTANCE;

typedef struct _CMOCK_save_profile_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;
  uint8_t Expected_cmock_arg1;

} CMOCK_save_profile_CALL_INSTANCE;

typedef struct _CMOCK_load_profile_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;
  uint8_t Expected_cmock_arg1;

} CMOCK_load_profile_CALL_INSTANCE;

typedef struct _CMOCK_cancel_new_profile_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;

} CMOCK_cancel_new_profile_CALL_INSTANCE;

static struct MockprofilesInstance
{
  int profile_manager_IgnoreBool;
  CMOCK_profile_manager_CALLBACK profile_manager_CallbackFunctionPointer;
  int profile_manager_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE profile_manager_CallInstance;
  int save_profile_IgnoreBool;
  uint8_t save_profile_FinalReturn;
  CMOCK_save_profile_CALLBACK save_profile_CallbackFunctionPointer;
  int save_profile_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE save_profile_CallInstance;
  int load_profile_IgnoreBool;
  uint8_t load_profile_FinalReturn;
  CMOCK_load_profile_CALLBACK load_profile_CallbackFunctionPointer;
  int load_profile_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE load_profile_CallInstance;
  int cancel_new_profile_IgnoreBool;
  uint8_t cancel_new_profile_FinalReturn;
  CMOCK_cancel_new_profile_CALLBACK cancel_new_profile_CallbackFunctionPointer;
  int cancel_new_profile_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE cancel_new_profile_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockprofiles_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  if (Mock.profile_manager_IgnoreBool)
    Mock.profile_manager_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.profile_manager_CallInstance, cmock_line, "Function 'profile_manager' called less times than expected.");
  if (Mock.profile_manager_CallbackFunctionPointer != NULL)
    Mock.profile_manager_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.save_profile_IgnoreBool)
    Mock.save_profile_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.save_profile_CallInstance, cmock_line, "Function 'save_profile' called less times than expected.");
  if (Mock.save_profile_CallbackFunctionPointer != NULL)
    Mock.save_profile_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.load_profile_IgnoreBool)
    Mock.load_profile_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.load_profile_CallInstance, cmock_line, "Function 'load_profile' called less times than expected.");
  if (Mock.load_profile_CallbackFunctionPointer != NULL)
    Mock.load_profile_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.cancel_new_profile_IgnoreBool)
    Mock.cancel_new_profile_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.cancel_new_profile_CallInstance, cmock_line, "Function 'cancel_new_profile' called less times than expected.");
  if (Mock.cancel_new_profile_CallbackFunctionPointer != NULL)
    Mock.cancel_new_profile_CallInstance = CMOCK_GUTS_NONE;
}

void Mockprofiles_Init(void)
{
  Mockprofiles_Destroy();
}

void Mockprofiles_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.profile_manager_CallbackFunctionPointer = NULL;
  Mock.profile_manager_CallbackCalls = 0;
  Mock.save_profile_CallbackFunctionPointer = NULL;
  Mock.save_profile_CallbackCalls = 0;
  Mock.load_profile_CallbackFunctionPointer = NULL;
  Mock.load_profile_CallbackCalls = 0;
  Mock.cancel_new_profile_CallbackFunctionPointer = NULL;
  Mock.cancel_new_profile_CallbackCalls = 0;
}

void profile_manager(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_profile_manager_CALL_INSTANCE* cmock_call_instance = (CMOCK_profile_manager_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.profile_manager_CallInstance);
  Mock.profile_manager_CallInstance = CMock_Guts_MemNext(Mock.profile_manager_CallInstance);
  if (Mock.profile_manager_CallbackFunctionPointer != NULL)
  {
    Mock.profile_manager_CallbackFunctionPointer(Mock.profile_manager_CallbackCalls++);
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'profile_manager' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.profile_manager_IgnoreBool)
  {
    return;
  }
}

void profile_manager_CMockIgnore(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_profile_manager_CALL_INSTANCE));
  CMOCK_profile_manager_CALL_INSTANCE* cmock_call_instance = (CMOCK_profile_manager_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.profile_manager_CallInstance = CMock_Guts_MemChain(Mock.profile_manager_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  Mock.profile_manager_IgnoreBool = (int)1;
}

void profile_manager_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_profile_manager_CALL_INSTANCE));
  CMOCK_profile_manager_CALL_INSTANCE* cmock_call_instance = (CMOCK_profile_manager_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.profile_manager_CallInstance = CMock_Guts_MemChain(Mock.profile_manager_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void profile_manager_StubWithCallback(CMOCK_profile_manager_CALLBACK Callback)
{
  Mock.profile_manager_CallbackFunctionPointer = Callback;
}

uint8_t save_profile(uint8_t cmock_arg1)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_save_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_save_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.save_profile_CallInstance);
  Mock.save_profile_CallInstance = CMock_Guts_MemNext(Mock.save_profile_CallInstance);
  if (Mock.save_profile_CallbackFunctionPointer != NULL)
  {
    return Mock.save_profile_CallbackFunctionPointer(cmock_arg1, Mock.save_profile_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'save_profile' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.save_profile_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.save_profile_FinalReturn;
    Mock.save_profile_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_cmock_arg1, cmock_arg1, cmock_line, "Function 'save_profile' called with unexpected value for argument 'cmock_arg1'.");
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_save_profile(CMOCK_save_profile_CALL_INSTANCE* cmock_call_instance, uint8_t cmock_arg1)
{
  cmock_call_instance->Expected_cmock_arg1 = cmock_arg1;
}

void save_profile_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_save_profile_CALL_INSTANCE));
  CMOCK_save_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_save_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.save_profile_CallInstance = CMock_Guts_MemChain(Mock.save_profile_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.save_profile_IgnoreBool = (int)1;
}

void save_profile_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_arg1, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_save_profile_CALL_INSTANCE));
  CMOCK_save_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_save_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.save_profile_CallInstance = CMock_Guts_MemChain(Mock.save_profile_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_save_profile(cmock_call_instance, cmock_arg1);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void save_profile_StubWithCallback(CMOCK_save_profile_CALLBACK Callback)
{
  Mock.save_profile_CallbackFunctionPointer = Callback;
}

uint8_t load_profile(uint8_t cmock_arg1)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_load_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_load_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.load_profile_CallInstance);
  Mock.load_profile_CallInstance = CMock_Guts_MemNext(Mock.load_profile_CallInstance);
  if (Mock.load_profile_CallbackFunctionPointer != NULL)
  {
    return Mock.load_profile_CallbackFunctionPointer(cmock_arg1, Mock.load_profile_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'load_profile' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.load_profile_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.load_profile_FinalReturn;
    Mock.load_profile_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_cmock_arg1, cmock_arg1, cmock_line, "Function 'load_profile' called with unexpected value for argument 'cmock_arg1'.");
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_load_profile(CMOCK_load_profile_CALL_INSTANCE* cmock_call_instance, uint8_t cmock_arg1)
{
  cmock_call_instance->Expected_cmock_arg1 = cmock_arg1;
}

void load_profile_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_load_profile_CALL_INSTANCE));
  CMOCK_load_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_load_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.load_profile_CallInstance = CMock_Guts_MemChain(Mock.load_profile_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.load_profile_IgnoreBool = (int)1;
}

void load_profile_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_arg1, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_load_profile_CALL_INSTANCE));
  CMOCK_load_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_load_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.load_profile_CallInstance = CMock_Guts_MemChain(Mock.load_profile_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_load_profile(cmock_call_instance, cmock_arg1);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void load_profile_StubWithCallback(CMOCK_load_profile_CALLBACK Callback)
{
  Mock.load_profile_CallbackFunctionPointer = Callback;
}

uint8_t cancel_new_profile(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_cancel_new_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_cancel_new_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.cancel_new_profile_CallInstance);
  Mock.cancel_new_profile_CallInstance = CMock_Guts_MemNext(Mock.cancel_new_profile_CallInstance);
  if (Mock.cancel_new_profile_CallbackFunctionPointer != NULL)
  {
    return Mock.cancel_new_profile_CallbackFunctionPointer(Mock.cancel_new_profile_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'cancel_new_profile' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.cancel_new_profile_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.cancel_new_profile_FinalReturn;
    Mock.cancel_new_profile_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  return cmock_call_instance->ReturnVal;
}

void cancel_new_profile_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_cancel_new_profile_CALL_INSTANCE));
  CMOCK_cancel_new_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_cancel_new_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.cancel_new_profile_CallInstance = CMock_Guts_MemChain(Mock.cancel_new_profile_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.cancel_new_profile_IgnoreBool = (int)1;
}

void cancel_new_profile_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_cancel_new_profile_CALL_INSTANCE));
  CMOCK_cancel_new_profile_CALL_INSTANCE* cmock_call_instance = (CMOCK_cancel_new_profile_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.cancel_new_profile_CallInstance = CMock_Guts_MemChain(Mock.cancel_new_profile_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void cancel_new_profile_StubWithCallback(CMOCK_cancel_new_profile_CALLBACK Callback)
{
  Mock.cancel_new_profile_CallbackFunctionPointer = Callback;
}

