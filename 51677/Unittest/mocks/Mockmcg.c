/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockmcg.h"

typedef struct _CMOCK_PLL_Init_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_PLL_Init_CALL_INSTANCE;

static struct MockmcgInstance
{
  int PLL_Init_IgnoreBool;
  CMOCK_PLL_Init_CALLBACK PLL_Init_CallbackFunctionPointer;
  int PLL_Init_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE PLL_Init_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockmcg_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  if (Mock.PLL_Init_IgnoreBool)
    Mock.PLL_Init_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.PLL_Init_CallInstance, cmock_line, "Function 'PLL_Init' called less times than expected.");
  if (Mock.PLL_Init_CallbackFunctionPointer != NULL)
    Mock.PLL_Init_CallInstance = CMOCK_GUTS_NONE;
}

void Mockmcg_Init(void)
{
  Mockmcg_Destroy();
}

void Mockmcg_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.PLL_Init_CallbackFunctionPointer = NULL;
  Mock.PLL_Init_CallbackCalls = 0;
}

void PLL_Init(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_PLL_Init_CALL_INSTANCE* cmock_call_instance = (CMOCK_PLL_Init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.PLL_Init_CallInstance);
  Mock.PLL_Init_CallInstance = CMock_Guts_MemNext(Mock.PLL_Init_CallInstance);
  if (Mock.PLL_Init_CallbackFunctionPointer != NULL)
  {
    Mock.PLL_Init_CallbackFunctionPointer(Mock.PLL_Init_CallbackCalls++);
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'PLL_Init' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.PLL_Init_IgnoreBool)
  {
    return;
  }
}

void PLL_Init_CMockIgnore(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_PLL_Init_CALL_INSTANCE));
  CMOCK_PLL_Init_CALL_INSTANCE* cmock_call_instance = (CMOCK_PLL_Init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.PLL_Init_CallInstance = CMock_Guts_MemChain(Mock.PLL_Init_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  Mock.PLL_Init_IgnoreBool = (int)1;
}

void PLL_Init_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_PLL_Init_CALL_INSTANCE));
  CMOCK_PLL_Init_CALL_INSTANCE* cmock_call_instance = (CMOCK_PLL_Init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.PLL_Init_CallInstance = CMock_Guts_MemChain(Mock.PLL_Init_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void PLL_Init_StubWithCallback(CMOCK_PLL_Init_CALLBACK Callback)
{
  Mock.PLL_Init_CallbackFunctionPointer = Callback;
}

