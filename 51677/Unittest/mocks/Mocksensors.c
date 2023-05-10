/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mocksensors.h"

typedef struct _CMOCK_door_sensor_init_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;

} CMOCK_door_sensor_init_CALL_INSTANCE;

typedef struct _CMOCK_poll_pump_sensors_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;

} CMOCK_poll_pump_sensors_CALL_INSTANCE;

typedef struct _CMOCK_tube_sensors_init_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;

} CMOCK_tube_sensors_init_CALL_INSTANCE;

static struct MocksensorsInstance
{
  int door_sensor_init_IgnoreBool;
  uint8_t door_sensor_init_FinalReturn;
  CMOCK_door_sensor_init_CALLBACK door_sensor_init_CallbackFunctionPointer;
  int door_sensor_init_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE door_sensor_init_CallInstance;
  int poll_pump_sensors_IgnoreBool;
  uint8_t poll_pump_sensors_FinalReturn;
  CMOCK_poll_pump_sensors_CALLBACK poll_pump_sensors_CallbackFunctionPointer;
  int poll_pump_sensors_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE poll_pump_sensors_CallInstance;
  int tube_sensors_init_IgnoreBool;
  uint8_t tube_sensors_init_FinalReturn;
  CMOCK_tube_sensors_init_CALLBACK tube_sensors_init_CallbackFunctionPointer;
  int tube_sensors_init_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE tube_sensors_init_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mocksensors_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  if (Mock.door_sensor_init_IgnoreBool)
    Mock.door_sensor_init_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.door_sensor_init_CallInstance, cmock_line, "Function 'door_sensor_init' called less times than expected.");
  if (Mock.door_sensor_init_CallbackFunctionPointer != NULL)
    Mock.door_sensor_init_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.poll_pump_sensors_IgnoreBool)
    Mock.poll_pump_sensors_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.poll_pump_sensors_CallInstance, cmock_line, "Function 'poll_pump_sensors' called less times than expected.");
  if (Mock.poll_pump_sensors_CallbackFunctionPointer != NULL)
    Mock.poll_pump_sensors_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.tube_sensors_init_IgnoreBool)
    Mock.tube_sensors_init_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.tube_sensors_init_CallInstance, cmock_line, "Function 'tube_sensors_init' called less times than expected.");
  if (Mock.tube_sensors_init_CallbackFunctionPointer != NULL)
    Mock.tube_sensors_init_CallInstance = CMOCK_GUTS_NONE;
}

void Mocksensors_Init(void)
{
  Mocksensors_Destroy();
}

void Mocksensors_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.door_sensor_init_CallbackFunctionPointer = NULL;
  Mock.door_sensor_init_CallbackCalls = 0;
  Mock.poll_pump_sensors_CallbackFunctionPointer = NULL;
  Mock.poll_pump_sensors_CallbackCalls = 0;
  Mock.tube_sensors_init_CallbackFunctionPointer = NULL;
  Mock.tube_sensors_init_CallbackCalls = 0;
}

uint8_t door_sensor_init(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_door_sensor_init_CALL_INSTANCE* cmock_call_instance = (CMOCK_door_sensor_init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.door_sensor_init_CallInstance);
  Mock.door_sensor_init_CallInstance = CMock_Guts_MemNext(Mock.door_sensor_init_CallInstance);
  if (Mock.door_sensor_init_CallbackFunctionPointer != NULL)
  {
    return Mock.door_sensor_init_CallbackFunctionPointer(Mock.door_sensor_init_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'door_sensor_init' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.door_sensor_init_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.door_sensor_init_FinalReturn;
    Mock.door_sensor_init_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  return cmock_call_instance->ReturnVal;
}

void door_sensor_init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_door_sensor_init_CALL_INSTANCE));
  CMOCK_door_sensor_init_CALL_INSTANCE* cmock_call_instance = (CMOCK_door_sensor_init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.door_sensor_init_CallInstance = CMock_Guts_MemChain(Mock.door_sensor_init_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.door_sensor_init_IgnoreBool = (int)1;
}

void door_sensor_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_door_sensor_init_CALL_INSTANCE));
  CMOCK_door_sensor_init_CALL_INSTANCE* cmock_call_instance = (CMOCK_door_sensor_init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.door_sensor_init_CallInstance = CMock_Guts_MemChain(Mock.door_sensor_init_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void door_sensor_init_StubWithCallback(CMOCK_door_sensor_init_CALLBACK Callback)
{
  Mock.door_sensor_init_CallbackFunctionPointer = Callback;
}

uint8_t poll_pump_sensors(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_poll_pump_sensors_CALL_INSTANCE* cmock_call_instance = (CMOCK_poll_pump_sensors_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.poll_pump_sensors_CallInstance);
  Mock.poll_pump_sensors_CallInstance = CMock_Guts_MemNext(Mock.poll_pump_sensors_CallInstance);
  if (Mock.poll_pump_sensors_CallbackFunctionPointer != NULL)
  {
    return Mock.poll_pump_sensors_CallbackFunctionPointer(Mock.poll_pump_sensors_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'poll_pump_sensors' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.poll_pump_sensors_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.poll_pump_sensors_FinalReturn;
    Mock.poll_pump_sensors_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  return cmock_call_instance->ReturnVal;
}

void poll_pump_sensors_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_poll_pump_sensors_CALL_INSTANCE));
  CMOCK_poll_pump_sensors_CALL_INSTANCE* cmock_call_instance = (CMOCK_poll_pump_sensors_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.poll_pump_sensors_CallInstance = CMock_Guts_MemChain(Mock.poll_pump_sensors_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.poll_pump_sensors_IgnoreBool = (int)1;
}

void poll_pump_sensors_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_poll_pump_sensors_CALL_INSTANCE));
  CMOCK_poll_pump_sensors_CALL_INSTANCE* cmock_call_instance = (CMOCK_poll_pump_sensors_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.poll_pump_sensors_CallInstance = CMock_Guts_MemChain(Mock.poll_pump_sensors_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void poll_pump_sensors_StubWithCallback(CMOCK_poll_pump_sensors_CALLBACK Callback)
{
  Mock.poll_pump_sensors_CallbackFunctionPointer = Callback;
}

uint8_t tube_sensors_init(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_tube_sensors_init_CALL_INSTANCE* cmock_call_instance = (CMOCK_tube_sensors_init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.tube_sensors_init_CallInstance);
  Mock.tube_sensors_init_CallInstance = CMock_Guts_MemNext(Mock.tube_sensors_init_CallInstance);
  if (Mock.tube_sensors_init_CallbackFunctionPointer != NULL)
  {
    return Mock.tube_sensors_init_CallbackFunctionPointer(Mock.tube_sensors_init_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'tube_sensors_init' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (Mock.tube_sensors_init_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.tube_sensors_init_FinalReturn;
    Mock.tube_sensors_init_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  return cmock_call_instance->ReturnVal;
}

void tube_sensors_init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_tube_sensors_init_CALL_INSTANCE));
  CMOCK_tube_sensors_init_CALL_INSTANCE* cmock_call_instance = (CMOCK_tube_sensors_init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.tube_sensors_init_CallInstance = CMock_Guts_MemChain(Mock.tube_sensors_init_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.tube_sensors_init_IgnoreBool = (int)1;
}

void tube_sensors_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_tube_sensors_init_CALL_INSTANCE));
  CMOCK_tube_sensors_init_CALL_INSTANCE* cmock_call_instance = (CMOCK_tube_sensors_init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.tube_sensors_init_CallInstance = CMock_Guts_MemChain(Mock.tube_sensors_init_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void tube_sensors_init_StubWithCallback(CMOCK_tube_sensors_init_CALLBACK Callback)
{
  Mock.tube_sensors_init_CallbackFunctionPointer = Callback;
}
