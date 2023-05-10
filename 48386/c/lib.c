#include <lib.h>

__root const SOUND_LIB lib_sounds[13] = {
  
  /* [0] Lo mode ablation tone */
  /* start addr */  {(uint16_t *)&lib_lo_sound[0],
  /* size */ (sizeof(lib_lo_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_lo_sound[43945] + 1)},
  
  /* [1] Lo mode ablation tone */
  /* start addr */ {(uint16_t *)&lib_lo_sound[0], 
  /* size */ (sizeof(lib_lo_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_lo_sound[43945] + 1)},
  
  /* [2] Med mode ablation tone */
  /* start addr */ {(uint16_t *)&lib_med_sound[0],
  /* size */ (sizeof(lib_med_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_med_sound[43945] + 1)},
  
  /* [3] Hi mode ablation tone */
  /* start addr */ {(uint16_t *)&lib_hi_sound[0],
  /* size */ (sizeof(lib_hi_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_hi_sound[43945] + 1)},
  
  /* [4] VAC mode ablation tone */
  /* start addr */ {(uint16_t *)&lib_vac_sound[0],
  /* size */ (sizeof(lib_vac_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_vac_sound[2993] + 1)},
  
  /* [5] Coag mode therapy tone */
  /* start addr */ {(uint16_t *)&lib_coag_sound[0],
  /* size */ (sizeof(lib_coag_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_coag_sound[84] + 1)},
  
  /* [6] Button click */
  /* start addr */ {(uint16_t *)&lib_click_sound[0],
  /* size */ (sizeof(lib_click_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_click_sound[1021] + 1)},
  
  /* [7] Ambient alarm audio */
  /* start addr */ {(uint16_t *)&lib_ambient_alarm_sound[0],
  /* size */ (sizeof(lib_ambient_alarm_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_ambient_alarm_sound[6642] + 1)},
  
  /* [8] Error audio */
  /* start addr */ {(uint16_t *)&lib_error_sound[0], 
  /* size */ (sizeof(lib_error_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_error_sound[53654] + 1)},
  
  /* [9] Hi mode change audio */
  /* start addr */ {(uint16_t *)&lib_hi_mode_sound[0], 
  /* size */ (sizeof(lib_hi_mode_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_hi_mode_sound[19417] + 1)},

  /* [10] Med mode change audio */
  /* start addr */ {(uint16_t *)&lib_med_mode_sound[0], 
  /* size */ (sizeof(lib_med_mode_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_med_mode_sound[15840] + 1)},  

  /* [11] Lo mode change audio */
  /* start addr */ {(uint16_t *)&lib_lo_mode_sound[0], 
  /* size */ (sizeof(lib_lo_mode_sound) / sizeof(uint16_t)),
  /* end addr */ (uint32_t) (&lib_lo_mode_sound[22483] + 1)},  
  
  /* [12] Splash audio */
  /* start addr */ {(uint16_t *)&lib_splash_sound[0],
  /* size */ 0,
  /* end addr */ (uint32_t) (&lib_splash_sound[38835] + 1)},
  
};

/* date of compliation "Mmm dd yyyy" in rom*/
__root const char date[11] = __DATE__;

/* time of compilation "hh:mm:ss" in rom*/
__root const char time[8] = __TIME__;
