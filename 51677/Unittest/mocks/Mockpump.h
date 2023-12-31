/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKPUMP_H
#define _MOCKPUMP_H

#include "Types.h"
#include "pump.h"

void Mockpump_Init(void);
void Mockpump_Destroy(void);
void Mockpump_Verify(void);




#define PumpPID_IgnoreAndReturn(cmock_retval) PumpPID_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void PumpPID_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, float32_t cmock_to_return);
#define PumpPID_ExpectAndReturn(pv, cmock_arg1, cmock_retval) PumpPID_CMockExpectAndReturn(__LINE__, pv, cmock_arg1, cmock_retval)
void PumpPID_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, q15_t pv, bool cmock_arg1, float32_t cmock_to_return);
typedef float32_t (* CMOCK_PumpPID_CALLBACK)(q15_t pv, bool cmock_arg1, int cmock_num_calls);
void PumpPID_StubWithCallback(CMOCK_PumpPID_CALLBACK Callback);
#define pump_inactivity_monitor_IgnoreAndReturn(cmock_retval) pump_inactivity_monitor_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_inactivity_monitor_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_inactivity_monitor_ExpectAndReturn(cmock_retval) pump_inactivity_monitor_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_inactivity_monitor_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_inactivity_monitor_CALLBACK)(int cmock_num_calls);
void pump_inactivity_monitor_StubWithCallback(CMOCK_pump_inactivity_monitor_CALLBACK Callback);
#define pump_speed_ctrl_IgnoreAndReturn(cmock_retval) pump_speed_ctrl_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_speed_ctrl_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_speed_ctrl_ExpectAndReturn(cmock_retval) pump_speed_ctrl_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_speed_ctrl_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_speed_ctrl_CALLBACK)(int cmock_num_calls);
void pump_speed_ctrl_StubWithCallback(CMOCK_pump_speed_ctrl_CALLBACK Callback);
#define PumpFIR_Init_Ignore() PumpFIR_Init_CMockIgnore(__LINE__)
void PumpFIR_Init_CMockIgnore(UNITY_LINE_TYPE cmock_line);
#define PumpFIR_Init_Expect(cmock_arg1) PumpFIR_Init_CMockExpect(__LINE__, cmock_arg1)
void PumpFIR_Init_CMockExpect(UNITY_LINE_TYPE cmock_line, q15_t cmock_arg1);
typedef void (* CMOCK_PumpFIR_Init_CALLBACK)(q15_t cmock_arg1, int cmock_num_calls);
void PumpFIR_Init_StubWithCallback(CMOCK_PumpFIR_Init_CALLBACK Callback);
#define PumpUART4_Init_Ignore() PumpUART4_Init_CMockIgnore(__LINE__)
void PumpUART4_Init_CMockIgnore(UNITY_LINE_TYPE cmock_line);
#define PumpUART4_Init_Expect(cmock_arg1) PumpUART4_Init_CMockExpect(__LINE__, cmock_arg1)
void PumpUART4_Init_CMockExpect(UNITY_LINE_TYPE cmock_line, BAUD_RATE cmock_arg1);
typedef void (* CMOCK_PumpUART4_Init_CALLBACK)(BAUD_RATE cmock_arg1, int cmock_num_calls);
void PumpUART4_Init_StubWithCallback(CMOCK_PumpUART4_Init_CALLBACK Callback);
#define send_pump_cmd_Ignore() send_pump_cmd_CMockIgnore(__LINE__)
void send_pump_cmd_CMockIgnore(UNITY_LINE_TYPE cmock_line);
#define send_pump_cmd_Expect() send_pump_cmd_CMockExpect(__LINE__)
void send_pump_cmd_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_send_pump_cmd_CALLBACK)(int cmock_num_calls);
void send_pump_cmd_StubWithCallback(CMOCK_send_pump_cmd_CALLBACK Callback);
#define receive_pump_cmd_IgnoreAndReturn(cmock_retval) receive_pump_cmd_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void receive_pump_cmd_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define receive_pump_cmd_ExpectAndReturn(cmock_retval) receive_pump_cmd_CMockExpectAndReturn(__LINE__, cmock_retval)
void receive_pump_cmd_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_receive_pump_cmd_CALLBACK)(int cmock_num_calls);
void receive_pump_cmd_StubWithCallback(CMOCK_receive_pump_cmd_CALLBACK Callback);
#define pump_packetizer_IgnoreAndReturn(cmock_retval) pump_packetizer_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_packetizer_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_packetizer_ExpectAndReturn(cmock_retval) pump_packetizer_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_packetizer_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_packetizer_CALLBACK)(int cmock_num_calls);
void pump_packetizer_StubWithCallback(CMOCK_pump_packetizer_CALLBACK Callback);
#define pump_reset_IgnoreAndReturn(cmock_retval) pump_reset_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_reset_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_reset_ExpectAndReturn(cmock_retval) pump_reset_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_reset_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_reset_CALLBACK)(int cmock_num_calls);
void pump_reset_StubWithCallback(CMOCK_pump_reset_CALLBACK Callback);
#define pump_cmd_ack_IgnoreAndReturn(cmock_retval) pump_cmd_ack_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_cmd_ack_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_cmd_ack_ExpectAndReturn(cmock_retval) pump_cmd_ack_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_cmd_ack_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_cmd_ack_CALLBACK)(int cmock_num_calls);
void pump_cmd_ack_StubWithCallback(CMOCK_pump_cmd_ack_CALLBACK Callback);
#define drive_status_IgnoreAndReturn(cmock_retval) drive_status_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void drive_status_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define drive_status_ExpectAndReturn(cmock_retval) drive_status_CMockExpectAndReturn(__LINE__, cmock_retval)
void drive_status_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_drive_status_CALLBACK)(int cmock_num_calls);
void drive_status_StubWithCallback(CMOCK_drive_status_CALLBACK Callback);
#define pump_cmd_nack_IgnoreAndReturn(cmock_retval) pump_cmd_nack_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_cmd_nack_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_cmd_nack_ExpectAndReturn(cmock_retval) pump_cmd_nack_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_cmd_nack_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_cmd_nack_CALLBACK)(int cmock_num_calls);
void pump_cmd_nack_StubWithCallback(CMOCK_pump_cmd_nack_CALLBACK Callback);
#define drive_power_mode_IgnoreAndReturn(cmock_retval) drive_power_mode_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void drive_power_mode_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define drive_power_mode_ExpectAndReturn(cmock_retval) drive_power_mode_CMockExpectAndReturn(__LINE__, cmock_retval)
void drive_power_mode_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_drive_power_mode_CALLBACK)(int cmock_num_calls);
void drive_power_mode_StubWithCallback(CMOCK_drive_power_mode_CALLBACK Callback);
#define probe_test_complete_IgnoreAndReturn(cmock_retval) probe_test_complete_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void probe_test_complete_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define probe_test_complete_ExpectAndReturn(cmock_retval) probe_test_complete_CMockExpectAndReturn(__LINE__, cmock_retval)
void probe_test_complete_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_probe_test_complete_CALLBACK)(int cmock_num_calls);
void probe_test_complete_StubWithCallback(CMOCK_probe_test_complete_CALLBACK Callback);
#define over_current_IgnoreAndReturn(cmock_retval) over_current_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void over_current_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define over_current_ExpectAndReturn(cmock_retval) over_current_CMockExpectAndReturn(__LINE__, cmock_retval)
void over_current_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_over_current_CALLBACK)(int cmock_num_calls);
void over_current_StubWithCallback(CMOCK_over_current_CALLBACK Callback);
#define over_energy_IgnoreAndReturn(cmock_retval) over_energy_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void over_energy_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define over_energy_ExpectAndReturn(cmock_retval) over_energy_CMockExpectAndReturn(__LINE__, cmock_retval)
void over_energy_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_over_energy_CALLBACK)(int cmock_num_calls);
void over_energy_StubWithCallback(CMOCK_over_energy_CALLBACK Callback);
#define pump_uart_init_IgnoreAndReturn(cmock_retval) pump_uart_init_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_uart_init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_uart_init_ExpectAndReturn(cmock_retval) pump_uart_init_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_uart_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_uart_init_CALLBACK)(int cmock_num_calls);
void pump_uart_init_StubWithCallback(CMOCK_pump_uart_init_CALLBACK Callback);
#define pump_cmd_eack_IgnoreAndReturn(cmock_retval) pump_cmd_eack_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_cmd_eack_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_cmd_eack_ExpectAndReturn(cmock_retval) pump_cmd_eack_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_cmd_eack_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_cmd_eack_CALLBACK)(int cmock_num_calls);
void pump_cmd_eack_StubWithCallback(CMOCK_pump_cmd_eack_CALLBACK Callback);
#define pump_startup_delay_IgnoreAndReturn(cmock_retval) pump_startup_delay_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_startup_delay_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_startup_delay_ExpectAndReturn(cmock_retval) pump_startup_delay_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_startup_delay_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_startup_delay_CALLBACK)(int cmock_num_calls);
void pump_startup_delay_StubWithCallback(CMOCK_pump_startup_delay_CALLBACK Callback);
#define pump_set_speed_Ignore() pump_set_speed_CMockIgnore(__LINE__)
void pump_set_speed_CMockIgnore(UNITY_LINE_TYPE cmock_line);
#define pump_set_speed_Expect(Speed) pump_set_speed_CMockExpect(__LINE__, Speed)
void pump_set_speed_CMockExpect(UNITY_LINE_TYPE cmock_line, float Speed);
typedef void (* CMOCK_pump_set_speed_CALLBACK)(float Speed, int cmock_num_calls);
void pump_set_speed_StubWithCallback(CMOCK_pump_set_speed_CALLBACK Callback);

#endif
