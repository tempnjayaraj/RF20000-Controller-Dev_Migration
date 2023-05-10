/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKFTFE_H
#define _MOCKFTFE_H

#include "Types.h"
#include "ftfe.h"

void Mockftfe_Init(void);
void Mockftfe_Destroy(void);
void Mockftfe_Verify(void);




#define program_ftfe_once_IgnoreAndReturn(cmock_retval) program_ftfe_once_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void program_ftfe_once_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define program_ftfe_once_ExpectAndReturn(cmock_arg1, cmock_arg2, cmock_arg3, cmock_retval) program_ftfe_once_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_arg2, cmock_arg3, cmock_retval)
void program_ftfe_once_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* cmock_arg1, uint8_t cmock_arg2, uint8_t cmock_arg3, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_program_ftfe_once_CALLBACK)(uint8_t* cmock_arg1, uint8_t cmock_arg2, uint8_t cmock_arg3, int cmock_num_calls);
void program_ftfe_once_StubWithCallback(CMOCK_program_ftfe_once_CALLBACK Callback);
#define read_ftfe_once_IgnoreAndReturn(cmock_retval) read_ftfe_once_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void read_ftfe_once_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define read_ftfe_once_ExpectAndReturn(cmock_arg1, cmock_arg2, cmock_arg3, cmock_retval) read_ftfe_once_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_arg2, cmock_arg3, cmock_retval)
void read_ftfe_once_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* cmock_arg1, uint8_t cmock_arg2, uint8_t cmock_arg3, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_read_ftfe_once_CALLBACK)(uint8_t* cmock_arg1, uint8_t cmock_arg2, uint8_t cmock_arg3, int cmock_num_calls);
void read_ftfe_once_StubWithCallback(CMOCK_read_ftfe_once_CALLBACK Callback);
#define erase_ftfe_sector_IgnoreAndReturn(cmock_retval) erase_ftfe_sector_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void erase_ftfe_sector_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define erase_ftfe_sector_ExpectAndReturn(cmock_arg1, cmock_retval) erase_ftfe_sector_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_retval)
void erase_ftfe_sector_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_arg1, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_erase_ftfe_sector_CALLBACK)(uint32_t cmock_arg1, int cmock_num_calls);
void erase_ftfe_sector_StubWithCallback(CMOCK_erase_ftfe_sector_CALLBACK Callback);
#define write_ftfe_phrase_IgnoreAndReturn(cmock_retval) write_ftfe_phrase_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void write_ftfe_phrase_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define write_ftfe_phrase_ExpectAndReturn(cmock_retval) write_ftfe_phrase_CMockExpectAndReturn(__LINE__, cmock_retval)
void write_ftfe_phrase_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_write_ftfe_phrase_CALLBACK)(int cmock_num_calls);
void write_ftfe_phrase_StubWithCallback(CMOCK_write_ftfe_phrase_CALLBACK Callback);
#define save_params2flash_IgnoreAndReturn(cmock_retval) save_params2flash_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void save_params2flash_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define save_params2flash_ExpectAndReturn(cmock_retval) save_params2flash_CMockExpectAndReturn(__LINE__, cmock_retval)
void save_params2flash_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_save_params2flash_CALLBACK)(int cmock_num_calls);
void save_params2flash_StubWithCallback(CMOCK_save_params2flash_CALLBACK Callback);
#define write_ftfe_section_IgnoreAndReturn(cmock_retval) write_ftfe_section_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void write_ftfe_section_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define write_ftfe_section_ExpectAndReturn(cmock_arg1, cmock_arg2, cmock_retval) write_ftfe_section_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_arg2, cmock_retval)
void write_ftfe_section_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_arg1, uint16_t cmock_arg2, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_write_ftfe_section_CALLBACK)(uint32_t cmock_arg1, uint16_t cmock_arg2, int cmock_num_calls);
void write_ftfe_section_StubWithCallback(CMOCK_write_ftfe_section_CALLBACK Callback);
#define get_cached_nv_data_IgnoreAndReturn(cmock_retval) get_cached_nv_data_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void get_cached_nv_data_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* cmock_to_return);
#define get_cached_nv_data_ExpectAndReturn(cmock_retval) get_cached_nv_data_CMockExpectAndReturn(__LINE__, cmock_retval)
void get_cached_nv_data_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* cmock_to_return);
typedef uint8_t* (* CMOCK_get_cached_nv_data_CALLBACK)(int cmock_num_calls);
void get_cached_nv_data_StubWithCallback(CMOCK_get_cached_nv_data_CALLBACK Callback);
#define set_ftfe_function_IgnoreAndReturn(cmock_retval) set_ftfe_function_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void set_ftfe_function_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define set_ftfe_function_ExpectAndReturn(cmock_retval) set_ftfe_function_CMockExpectAndReturn(__LINE__, cmock_retval)
void set_ftfe_function_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_set_ftfe_function_CALLBACK)(int cmock_num_calls);
void set_ftfe_function_StubWithCallback(CMOCK_set_ftfe_function_CALLBACK Callback);
#define program_ftfe_partition_IgnoreAndReturn(cmock_retval) program_ftfe_partition_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void program_ftfe_partition_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define program_ftfe_partition_ExpectAndReturn(cmock_retval) program_ftfe_partition_CMockExpectAndReturn(__LINE__, cmock_retval)
void program_ftfe_partition_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_program_ftfe_partition_CALLBACK)(int cmock_num_calls);
void program_ftfe_partition_StubWithCallback(CMOCK_program_ftfe_partition_CALLBACK Callback);
#define read_ftfe_section_IgnoreAndReturn(cmock_retval) read_ftfe_section_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void read_ftfe_section_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define read_ftfe_section_ExpectAndReturn(destination, nwords, cmock_retval) read_ftfe_section_CMockExpectAndReturn(__LINE__, destination, nwords, cmock_retval)
void read_ftfe_section_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t destination, uint16_t nwords, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_read_ftfe_section_CALLBACK)(uint32_t destination, uint16_t nwords, int cmock_num_calls);
void read_ftfe_section_StubWithCallback(CMOCK_read_ftfe_section_CALLBACK Callback);

#endif
