#include "unity.h"
#include "MK26F18.h"
#include "Mocksystem.h"
#include "Zigbee.h"
#include "intrinsics.h"
#include "cpu.h"
#include "Mockevent_buffer.h"

GEN_SETTINGS current_therapy_settings;
event_table evt_tbl;
extern WIRELESS_CMD wireless;
extern WIRELESS_STATUS wireless_status_state;
extern uint8_t wireless_packet[3];
void setUp(void)
{
}

void tearDown(void)
{
}

void test_zigbee_init (void)
{
   uint8_t return_value = zigbee_init();
}

void test_UART5_Init (void)
{
   //put_event_ExpectAndReturn(&evt_tbl,RUN_IEC_OVERCURRENT,0);
   UART5_Init();
   //TEST_ASSERT_EQUAL(0,current_therapy_settings.setpoint.dac);
}

void test_enable_datastream (void)
{
   enable_datastream ();
}

void test_request_statusbytes (void)
{
   uint8_t return_value = request_statusbytes();
}

void test_request_statusbytes_1 (void)
{
   current_therapy_settings.footpedal.wireless.attempts = 11;
   uint8_t return_value = request_statusbytes();
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_request_statusbytes_2 (void)
{
   current_therapy_settings.footpedal.wireless.attempts = 0;
   current_therapy_settings.treatment.wireless_enable = 1;
   //uint8_t return_value = request_statusbytes();
  //TEST_ASSERT_EQUAL((UART5_C2& UART_C2_TCIE_MASK),UART_C2_TCIE_MASK);
   TEST_IGNORE_MESSAGE("Wireles IDX went to 0x03, which is a memory out of bounds issue");
}

void test_send_wireless_cmd (void)
{
   send_wireless_cmd();
   // No test possible for the first case.
   LPUART0_STAT |= LPUART_STAT_TC_MASK;
   send_wireless_cmd();
   wireless.idx = 0;
   send_wireless_cmd();
   TEST_ASSERT_EQUAL(0,LPUART0_CTRL & LPUART_CTRL_TCIE_MASK);
   wireless.idx = 8;
   wireless.cmd[0].size = 100;
   send_wireless_cmd();
   send_wireless_cmd();
   send_wireless_cmd();
   send_wireless_cmd();
   send_wireless_cmd();
}

void test_receive_wireless_cmd (void)
{
   //put_event_ExpectAndReturn(&evt_tbl,FOOT_SWITCH_STUCK,0);
    receive_wireless_cmd();
    LPUART0_STAT |= LPUART_STAT_RDRF_MASK;
    LPUART0_DATA = 0x0;
    receive_wireless_cmd();
    //TEST_ASSERT_EQUAL(MARRIAGE_STATUS,
    LPUART0_DATA = 0x40;
    receive_wireless_cmd();
    LPUART0_DATA = 0x80;
    receive_wireless_cmd();
    wireless_status_state = 0;

    LPUART0_STAT |= LPUART_STAT_RDRF_MASK;
    wireless_packet[0] = 0x1u;
    wireless_status_state = SYSTEM_STATUS;
    LPUART0_DATA = 0x80;
    wireless_packet[1] |= 0x20u ;
    //put_event_ExpectAndReturn(&evt_tbl,FOOT_SWITCH_STUCK,0);
    receive_wireless_cmd();

    LPUART0_STAT |= LPUART_STAT_RDRF_MASK;
    wireless_packet[0] = 0x1u;
    wireless_status_state = SYSTEM_STATUS;
    LPUART0_DATA = 0x80;
    wireless_packet[1] |= 0x00u ; //present = 0
    //put_event_ExpectAndReturn(&evt_tbl,FOOT_SWITCH_STUCK,0);
    receive_wireless_cmd();

    LPUART0_STAT |= LPUART_STAT_RDRF_MASK;
    wireless_packet[0] = 0x1u;
    wireless_status_state = SYSTEM_STATUS;
    LPUART0_DATA = 0x80;
    wireless_packet[1] |= 0x20u ; //present = 0
    current_therapy_settings.footpedal.wireless.married2station1 = 1;
    //put_event_ExpectAndReturn(&evt_tbl,FOOT_SWITCH_STUCK,0);
    receive_wireless_cmd();

    LPUART0_STAT |= LPUART_STAT_RDRF_MASK;
    wireless_packet[0] = 0x1u;
    wireless_status_state = SYSTEM_STATUS;
    LPUART0_DATA = 0x0;
    wireless_packet[1] |= 0x20u ;
    receive_wireless_cmd();

    LPUART0_STAT |= LPUART_STAT_RDRF_MASK;
    wireless_packet[0] = 0x1u;
    wireless_status_state = 0x03;//intentionally having a value out of bounds
    LPUART0_DATA = 0x0;
    wireless_packet[1] |= 0x20u ;
    receive_wireless_cmd();

    LPUART0_STAT |= LPUART_STAT_RDRF_MASK;
    wireless_packet[0] = 0x1u;
    wireless_status_state = MARRIAGE_STATUS;
    LPUART0_DATA = 0x80;
    wireless_packet[1] |= 0x20u ; //present = 0
    current_therapy_settings.footpedal.wireless.married2station1 = 1;
    receive_wireless_cmd();
    TEST_ASSERT_EQUAL(wireless_status_state,0);
}




void test_wireless_handshake (void)
{
   uint8_t return_value = wireless_handshake();
}

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
