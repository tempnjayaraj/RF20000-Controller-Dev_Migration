/* AUTOGENERATED FILE. DO NOT EDIT. */

//=======Test Runner Used To Run Each Test Below=====
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT() && !TEST_IS_IGNORED) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

//=======Automagically Detected Files To Include=====
#include "unity.h"
#include "Types.h"
#include <setjmp.h>
#include <stdio.h>

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_poll_usb(void);
extern void test_USB_Testing_Read_Registers(void);
extern void test_USB_Testing_Write_Registers(void);
extern void test_USB_Init(void);
extern void test_EP_OUT_Transfer(void);
extern void test_USB_EP_OUT_SizeCheck(void);
extern void test_USB_Set_Interface(void);
extern void test_USB_StdReq_Handler(void);
extern void test_USB_StdReq_Handler_cases(void);
extern void test_USB_Setup_Handler(void);
extern void test_USB_Endpoint_Setup_Handler(void);
extern void test_USB_Handler(void);
extern void test_USB_EP0_IN_Handler(void);
extern void test_USB_EP0_Stall(void);
extern void test_USB_EP0_OUT_Handler(void);
extern void test_USB_Stall_Handler(void);
extern void test_USB_Reset_Handler(void);
extern void test_usb_isr(void);
extern void test_rand_num(void);
extern void test_d_atoc(void);
extern void test_EP_IN_Transfer(void);
extern void test_dummy(void);


//=======Test Reset Option=====
void resetTest()
{
  tearDown();
  setUp();
}


//=======MAIN=====
int main(void)
{
  Unity.TestFile = "./test/Testusb.c";
  UnityBegin();
  RUN_TEST(test_poll_usb, 23);
  RUN_TEST(test_USB_Testing_Read_Registers, 28);
  RUN_TEST(test_USB_Testing_Write_Registers, 33);
  RUN_TEST(test_USB_Init, 38);
  RUN_TEST(test_EP_OUT_Transfer, 45);
  RUN_TEST(test_USB_EP_OUT_SizeCheck, 52);
  RUN_TEST(test_USB_Set_Interface, 58);
  RUN_TEST(test_USB_StdReq_Handler, 63);
  RUN_TEST(test_USB_StdReq_Handler_cases, 68);
  RUN_TEST(test_USB_Setup_Handler, 100);
  RUN_TEST(test_USB_Endpoint_Setup_Handler, 122);
  RUN_TEST(test_USB_Handler, 139);
  RUN_TEST(test_USB_EP0_IN_Handler, 153);
  RUN_TEST(test_USB_EP0_Stall, 158);
  RUN_TEST(test_USB_EP0_OUT_Handler, 163);
  RUN_TEST(test_USB_Stall_Handler, 168);
  RUN_TEST(test_USB_Reset_Handler, 173);
  RUN_TEST(test_usb_isr, 178);
  RUN_TEST(test_rand_num, 201);
  RUN_TEST(test_d_atoc, 207);
  RUN_TEST(test_EP_IN_Transfer, 215);
  RUN_TEST(test_dummy, 237);

  return (UnityEnd());
}
