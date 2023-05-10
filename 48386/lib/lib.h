#ifndef __LIB_H
#define __LIB_H

#include <stdint.h>

////extern const uint16_t lib_lo_sound[17374];
//extern const uint16_t lib_lo_sound[134];
////extern const uint16_t lib_med_sound[14308]; 
////extern const uint16_t lib_hi_sound[14308]; 
//extern const uint16_t lib_med_sound[126];
//extern const uint16_t lib_hi_sound[113]; 
////extern const uint16_t lib_vac_sound[8687]; 
//extern const uint16_t lib_vac_sound[2994]; 
////extern const uint16_t lib_coag_sound[1533];
//extern const uint16_t lib_coag_sound[85];
//extern const uint16_t lib_click_sound[1022];
////extern const uint16_t lib_coag2_sound[1348];
////extern const uint16_t lib_ambient_alarm_sound[14819];
//extern const uint16_t lib_ambient_alarm_sound[9709];
////extern const uint16_t lib_splash_sound[182427];
//extern const uint16_t lib_error_sound[53655];
//extern const uint16_t lib_lo_mode_sound[22485];
//extern const uint16_t lib_med_mode_sound[15842];
//extern const uint16_t lib_hi_mode_sound[19419];
//extern const uint16_t lib_splash_sound[100668];

extern const uint16_t lib_lo_sound[43946];
extern const uint16_t lib_med_sound[43946];
extern const uint16_t lib_hi_sound[43946];
extern const uint16_t lib_vac_sound[2994];
extern const uint16_t lib_coag_sound[85];
extern const uint16_t lib_click_sound[1022];
extern const uint16_t lib_ambient_alarm_sound[6643];
extern const uint16_t lib_error_sound[53655];
extern const uint16_t lib_lo_mode_sound[22485];
extern const uint16_t lib_med_mode_sound[15842];
extern const uint16_t lib_hi_mode_sound[19419];
extern const uint16_t lib_splash_sound[38836];

//#define LIB_ENTRY_ADDR 0x000ee814
//
//typedef void (*lib_init_f)(void);
//
///* Library entry struct (collection of function pointers) */
//struct lib_func
//{
//    lib_init_f init;
//    
//};

//int lib_init(void);

#pragma pack(1)
typedef struct {
  uint16_t *p;
  uint16_t n;
  uint32_t a;
}SOUND_LIB;

//extern const SOUND_LIB lib_sounds[13];

#endif