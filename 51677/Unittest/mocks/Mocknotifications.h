/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKNOTIFICATIONS_H
#define _MOCKNOTIFICATIONS_H

#include "Types.h"
#include "notifications.h"

void Mocknotifications_Init(void);
void Mocknotifications_Destroy(void);
void Mocknotifications_Verify(void);




#define notification_manager_Ignore() notification_manager_CMockIgnore(__LINE__)
void notification_manager_CMockIgnore(UNITY_LINE_TYPE cmock_line);
#define notification_manager_Expect() notification_manager_CMockExpect(__LINE__)
void notification_manager_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_notification_manager_CALLBACK)(int cmock_num_calls);
void notification_manager_StubWithCallback(CMOCK_notification_manager_CALLBACK Callback);
#define gui_com_fault_IgnoreAndReturn(cmock_retval) gui_com_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void gui_com_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define gui_com_fault_ExpectAndReturn(cmock_retval) gui_com_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void gui_com_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_gui_com_fault_CALLBACK)(int cmock_num_calls);
void gui_com_fault_StubWithCallback(CMOCK_gui_com_fault_CALLBACK Callback);
#define peak_hit_cntr_IgnoreAndReturn(cmock_retval) peak_hit_cntr_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void peak_hit_cntr_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define peak_hit_cntr_ExpectAndReturn(cmock_retval) peak_hit_cntr_CMockExpectAndReturn(__LINE__, cmock_retval)
void peak_hit_cntr_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_peak_hit_cntr_CALLBACK)(int cmock_num_calls);
void peak_hit_cntr_StubWithCallback(CMOCK_peak_hit_cntr_CALLBACK Callback);
#define legacy_wand_short_error_IgnoreAndReturn(cmock_retval) legacy_wand_short_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void legacy_wand_short_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define legacy_wand_short_error_ExpectAndReturn(cmock_retval) legacy_wand_short_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void legacy_wand_short_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_legacy_wand_short_error_CALLBACK)(int cmock_num_calls);
void legacy_wand_short_error_StubWithCallback(CMOCK_legacy_wand_short_error_CALLBACK Callback);
#define reset_error_IgnoreAndReturn(cmock_retval) reset_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void reset_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define reset_error_ExpectAndReturn(cmock_retval) reset_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void reset_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_reset_error_CALLBACK)(int cmock_num_calls);
void reset_error_StubWithCallback(CMOCK_reset_error_CALLBACK Callback);
#define suction_tube_error_IgnoreAndReturn(cmock_retval) suction_tube_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void suction_tube_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define suction_tube_error_ExpectAndReturn(cmock_retval) suction_tube_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void suction_tube_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_suction_tube_error_CALLBACK)(int cmock_num_calls);
void suction_tube_error_StubWithCallback(CMOCK_suction_tube_error_CALLBACK Callback);
#define no_ambient_error_IgnoreAndReturn(cmock_retval) no_ambient_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void no_ambient_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define no_ambient_error_ExpectAndReturn(cmock_retval) no_ambient_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void no_ambient_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_no_ambient_error_CALLBACK)(int cmock_num_calls);
void no_ambient_error_StubWithCallback(CMOCK_no_ambient_error_CALLBACK Callback);
#define clear_internal_temp_error_IgnoreAndReturn(cmock_retval) clear_internal_temp_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void clear_internal_temp_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define clear_internal_temp_error_ExpectAndReturn(cmock_retval) clear_internal_temp_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void clear_internal_temp_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_clear_internal_temp_error_CALLBACK)(int cmock_num_calls);
void clear_internal_temp_error_StubWithCallback(CMOCK_clear_internal_temp_error_CALLBACK Callback);
#define internal_temp_error_IgnoreAndReturn(cmock_retval) internal_temp_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void internal_temp_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define internal_temp_error_ExpectAndReturn(cmock_retval) internal_temp_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void internal_temp_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_internal_temp_error_CALLBACK)(int cmock_num_calls);
void internal_temp_error_StubWithCallback(CMOCK_internal_temp_error_CALLBACK Callback);
#define adc_boundary_fault_IgnoreAndReturn(cmock_retval) adc_boundary_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void adc_boundary_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define adc_boundary_fault_ExpectAndReturn(cmock_retval) adc_boundary_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void adc_boundary_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_adc_boundary_fault_CALLBACK)(int cmock_num_calls);
void adc_boundary_fault_StubWithCallback(CMOCK_adc_boundary_fault_CALLBACK Callback);
#define crc_fault_IgnoreAndReturn(cmock_retval) crc_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void crc_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define crc_fault_ExpectAndReturn(cmock_retval) crc_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void crc_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_crc_fault_CALLBACK)(int cmock_num_calls);
void crc_fault_StubWithCallback(CMOCK_crc_fault_CALLBACK Callback);
#define wand_switch_stuck_fault_IgnoreAndReturn(cmock_retval) wand_switch_stuck_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void wand_switch_stuck_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define wand_switch_stuck_fault_ExpectAndReturn(cmock_retval) wand_switch_stuck_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void wand_switch_stuck_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_wand_switch_stuck_fault_CALLBACK)(int cmock_num_calls);
void wand_switch_stuck_fault_StubWithCallback(CMOCK_wand_switch_stuck_fault_CALLBACK Callback);
#define fp_switch_stuck_fault_IgnoreAndReturn(cmock_retval) fp_switch_stuck_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fp_switch_stuck_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define fp_switch_stuck_fault_ExpectAndReturn(cmock_retval) fp_switch_stuck_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void fp_switch_stuck_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_fp_switch_stuck_fault_CALLBACK)(int cmock_num_calls);
void fp_switch_stuck_fault_StubWithCallback(CMOCK_fp_switch_stuck_fault_CALLBACK Callback);
#define pump_fault_IgnoreAndReturn(cmock_retval) pump_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void pump_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define pump_fault_ExpectAndReturn(cmock_retval) pump_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void pump_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_pump_fault_CALLBACK)(int cmock_num_calls);
void pump_fault_StubWithCallback(CMOCK_pump_fault_CALLBACK Callback);
#define vmod_fault_IgnoreAndReturn(cmock_retval) vmod_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void vmod_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define vmod_fault_ExpectAndReturn(cmock_retval) vmod_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void vmod_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_vmod_fault_CALLBACK)(int cmock_num_calls);
void vmod_fault_StubWithCallback(CMOCK_vmod_fault_CALLBACK Callback);
#define can_timeout_IgnoreAndReturn(cmock_retval) can_timeout_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void can_timeout_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define can_timeout_ExpectAndReturn(cmock_retval) can_timeout_CMockExpectAndReturn(__LINE__, cmock_retval)
void can_timeout_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_can_timeout_CALLBACK)(int cmock_num_calls);
void can_timeout_StubWithCallback(CMOCK_can_timeout_CALLBACK Callback);
#define internal_temp_fault_IgnoreAndReturn(cmock_retval) internal_temp_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void internal_temp_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define internal_temp_fault_ExpectAndReturn(cmock_retval) internal_temp_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void internal_temp_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_internal_temp_fault_CALLBACK)(int cmock_num_calls);
void internal_temp_fault_StubWithCallback(CMOCK_internal_temp_fault_CALLBACK Callback);
#define wdog_fault_IgnoreAndReturn(cmock_retval) wdog_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void wdog_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define wdog_fault_ExpectAndReturn(cmock_retval) wdog_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void wdog_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_wdog_fault_CALLBACK)(int cmock_num_calls);
void wdog_fault_StubWithCallback(CMOCK_wdog_fault_CALLBACK Callback);
#define fault_init_IgnoreAndReturn(cmock_retval) fault_init_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fault_init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define fault_init_ExpectAndReturn(cmock_retval) fault_init_CMockExpectAndReturn(__LINE__, cmock_retval)
void fault_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_fault_init_CALLBACK)(int cmock_num_calls);
void fault_init_StubWithCallback(CMOCK_fault_init_CALLBACK Callback);
#define date_time_fault_IgnoreAndReturn(cmock_retval) date_time_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void date_time_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define date_time_fault_ExpectAndReturn(cmock_retval) date_time_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void date_time_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_date_time_fault_CALLBACK)(int cmock_num_calls);
void date_time_fault_StubWithCallback(CMOCK_date_time_fault_CALLBACK Callback);
#define serial_num_fault_IgnoreAndReturn(cmock_retval) serial_num_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void serial_num_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define serial_num_fault_ExpectAndReturn(cmock_retval) serial_num_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void serial_num_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_serial_num_fault_CALLBACK)(int cmock_num_calls);
void serial_num_fault_StubWithCallback(CMOCK_serial_num_fault_CALLBACK Callback);
#define wand_expired_error_IgnoreAndReturn(cmock_retval) wand_expired_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void wand_expired_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define wand_expired_error_ExpectAndReturn(cmock_retval) wand_expired_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void wand_expired_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_wand_expired_error_CALLBACK)(int cmock_num_calls);
void wand_expired_error_StubWithCallback(CMOCK_wand_expired_error_CALLBACK Callback);
#define wand_use_limit_error_IgnoreAndReturn(cmock_retval) wand_use_limit_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void wand_use_limit_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define wand_use_limit_error_ExpectAndReturn(cmock_retval) wand_use_limit_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void wand_use_limit_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_wand_use_limit_error_CALLBACK)(int cmock_num_calls);
void wand_use_limit_error_StubWithCallback(CMOCK_wand_use_limit_error_CALLBACK Callback);
#define clear_wand_expired_error_IgnoreAndReturn(cmock_retval) clear_wand_expired_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void clear_wand_expired_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define clear_wand_expired_error_ExpectAndReturn(cmock_retval) clear_wand_expired_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void clear_wand_expired_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_clear_wand_expired_error_CALLBACK)(int cmock_num_calls);
void clear_wand_expired_error_StubWithCallback(CMOCK_clear_wand_expired_error_CALLBACK Callback);
#define clear_wand_use_limit_error_IgnoreAndReturn(cmock_retval) clear_wand_use_limit_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void clear_wand_use_limit_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define clear_wand_use_limit_error_ExpectAndReturn(cmock_retval) clear_wand_use_limit_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void clear_wand_use_limit_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_clear_wand_use_limit_error_CALLBACK)(int cmock_num_calls);
void clear_wand_use_limit_error_StubWithCallback(CMOCK_clear_wand_use_limit_error_CALLBACK Callback);
#define wand_id_error_IgnoreAndReturn(cmock_retval) wand_id_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void wand_id_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define wand_id_error_ExpectAndReturn(cmock_retval) wand_id_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void wand_id_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_wand_id_error_CALLBACK)(int cmock_num_calls);
void wand_id_error_StubWithCallback(CMOCK_wand_id_error_CALLBACK Callback);
#define lw_id_error_IgnoreAndReturn(cmock_retval) lw_id_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void lw_id_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define lw_id_error_ExpectAndReturn(cmock_retval) lw_id_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void lw_id_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_lw_id_error_CALLBACK)(int cmock_num_calls);
void lw_id_error_StubWithCallback(CMOCK_lw_id_error_CALLBACK Callback);
#define wand_disconnected_error_IgnoreAndReturn(cmock_retval) wand_disconnected_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void wand_disconnected_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define wand_disconnected_error_ExpectAndReturn(cmock_retval) wand_disconnected_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void wand_disconnected_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_wand_disconnected_error_CALLBACK)(int cmock_num_calls);
void wand_disconnected_error_StubWithCallback(CMOCK_wand_disconnected_error_CALLBACK Callback);
#define check_electrode_error_IgnoreAndReturn(cmock_retval) check_electrode_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void check_electrode_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define check_electrode_error_ExpectAndReturn(cmock_retval) check_electrode_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void check_electrode_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_check_electrode_error_CALLBACK)(int cmock_num_calls);
void check_electrode_error_StubWithCallback(CMOCK_check_electrode_error_CALLBACK Callback);
#define wand_error_type_decipher_IgnoreAndReturn(cmock_retval) wand_error_type_decipher_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void wand_error_type_decipher_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define wand_error_type_decipher_ExpectAndReturn(cmock_arg1, cmock_retval) wand_error_type_decipher_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_retval)
void wand_error_type_decipher_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, NOTIFY_TXT cmock_arg1, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_wand_error_type_decipher_CALLBACK)(NOTIFY_TXT cmock_arg1, int cmock_num_calls);
void wand_error_type_decipher_StubWithCallback(CMOCK_wand_error_type_decipher_CALLBACK Callback);
#define clear_wand_errors_IgnoreAndReturn(cmock_retval) clear_wand_errors_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void clear_wand_errors_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
#define clear_wand_errors_ExpectAndReturn(cmock_retval) clear_wand_errors_CMockExpectAndReturn(__LINE__, cmock_retval)
void clear_wand_errors_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
typedef uint32_t (* CMOCK_clear_wand_errors_CALLBACK)(int cmock_num_calls);
void clear_wand_errors_StubWithCallback(CMOCK_clear_wand_errors_CALLBACK Callback);
#define lw_incompatible_error_IgnoreAndReturn(cmock_retval) lw_incompatible_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void lw_incompatible_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define lw_incompatible_error_ExpectAndReturn(cmock_retval) lw_incompatible_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void lw_incompatible_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_lw_incompatible_error_CALLBACK)(int cmock_num_calls);
void lw_incompatible_error_StubWithCallback(CMOCK_lw_incompatible_error_CALLBACK Callback);
#define legacy_wand_disconnected_error_IgnoreAndReturn(cmock_retval) legacy_wand_disconnected_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void legacy_wand_disconnected_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define legacy_wand_disconnected_error_ExpectAndReturn(cmock_retval) legacy_wand_disconnected_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void legacy_wand_disconnected_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_legacy_wand_disconnected_error_CALLBACK)(int cmock_num_calls);
void legacy_wand_disconnected_error_StubWithCallback(CMOCK_legacy_wand_disconnected_error_CALLBACK Callback);
#define gen_com_fault_IgnoreAndReturn(cmock_retval) gen_com_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void gen_com_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define gen_com_fault_ExpectAndReturn(cmock_retval) gen_com_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void gen_com_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_gen_com_fault_CALLBACK)(int cmock_num_calls);
void gen_com_fault_StubWithCallback(CMOCK_gen_com_fault_CALLBACK Callback);
#define two_wand_disconnected_error_IgnoreAndReturn(cmock_retval) two_wand_disconnected_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void two_wand_disconnected_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define two_wand_disconnected_error_ExpectAndReturn(cmock_retval) two_wand_disconnected_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void two_wand_disconnected_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_two_wand_disconnected_error_CALLBACK)(int cmock_num_calls);
void two_wand_disconnected_error_StubWithCallback(CMOCK_two_wand_disconnected_error_CALLBACK Callback);
#define multiple_button_pressed_test_IgnoreAndReturn(cmock_retval) multiple_button_pressed_test_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void multiple_button_pressed_test_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
#define multiple_button_pressed_test_ExpectAndReturn(cmock_arg1, cmock_retval) multiple_button_pressed_test_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_retval)
void multiple_button_pressed_test_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_arg1, uint32_t cmock_to_return);
typedef uint32_t (* CMOCK_multiple_button_pressed_test_CALLBACK)(uint32_t cmock_arg1, int cmock_num_calls);
void multiple_button_pressed_test_StubWithCallback(CMOCK_multiple_button_pressed_test_CALLBACK Callback);
#define ui_fault_IgnoreAndReturn(cmock_retval) ui_fault_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void ui_fault_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define ui_fault_ExpectAndReturn(cmock_retval) ui_fault_CMockExpectAndReturn(__LINE__, cmock_retval)
void ui_fault_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_ui_fault_CALLBACK)(int cmock_num_calls);
void ui_fault_StubWithCallback(CMOCK_ui_fault_CALLBACK Callback);
#define smart_wand_incompatible_error_IgnoreAndReturn(cmock_retval) smart_wand_incompatible_error_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void smart_wand_incompatible_error_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define smart_wand_incompatible_error_ExpectAndReturn(cmock_retval) smart_wand_incompatible_error_CMockExpectAndReturn(__LINE__, cmock_retval)
void smart_wand_incompatible_error_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_smart_wand_incompatible_error_CALLBACK)(int cmock_num_calls);
void smart_wand_incompatible_error_StubWithCallback(CMOCK_smart_wand_incompatible_error_CALLBACK Callback);

#endif
