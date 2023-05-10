/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author:: Harshil.Shah      $:
$Rev:: 2806                 $:
$Date:: 2022-02-01 14:41:51#$:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file      gui.h
\brief     Header file for the GUI driver.
\details   
*******************************************************************************/

#ifndef GUI_H
#define GUI_H

#include "common.h"

typedef enum {
/*  0  */    NULL_GRAPHIC_ID,
/*  1  */    CUT_PLUS_BUTTON,
/*  2  */    CUT_MINUS_BUTTON,
/*  3  */    COAG_PLUS_BUTTON,
/*  4  */    COAG_MINUS_BUTTON,
/*  5  */    NOT_USED_0,
/*  6  */    HI_MODE_BUTTON,
/*  7  */    MED_MODE_BUTTON,
/*  8  */    LO_MODE_BUTTON,
/*  9  */    SETTINGS_BUTTON,
/*  10 */    NOT_USED_1,
/*  11 */    NOT_USED_2,
/*  12 */    NOT_USED_3,
/*  13 */    NOT_USED_4,
/*  14 */    WIRELESS_HANDSHAKE_BUTTON,
/*  15 */    NOT_USED_5,
/*  16 */    NOT_USED_6,
/*  17 */    NOT_USED_7,
/*  18 */    NOT_USED_8,
/*  19 */    ABLATE_DISABLE,
/*  20 */    NOT_USED_10,
/*  21 */    LANG_PREFERENCE_BUTTON,
/*  22 */    TRASH_BUTTON,
/*  23 */    LIST_LANGUAGES_BUTTON,
/*  24 */    NOT_USED_11,
/*  25 */    NOT_USED_12,
/*  26 */    NOT_USED_13,
/*  27 */    NOT_USED_14,
/*  28 */    NOT_USED_15,
/*  29 */    NOT_USED_16,
/*  30 */    NOT_USED_17,
/*  31 */    NOT_USED_18,
/*  32 */    NOT_USED_19,
/*  33 */    NOT_USED_20,
/*  34 */    NOT_USED_21,
/*  35 */    NOT_USED_22,
/*  36 */    NOT_USED_23,
/*  37 */    NOT_USED_24,
/*  38 */    EDIT_FP_FS_PREF_BUTTON,
/*  39 */    USER_SETTINGS_BUTTON,
/*  40 */    ENABLE_AMBIENT_BUTTON,
/*  41 */    DISABLE_AMBIENT_BUTTON,
/*  42 */    AMBIENT_THRESHOLD_PLUS,
/*  43 */    AMBIENT_THERSHOLD_MINUS,
/*  44 */    ENABLE_AMBIENT_ALARM,
/*  45 */    DISABLE_AMBIENT_ALARM,
/*  46 */    NOT_USED_25,
/*  47 */    NOT_USED_26,
/*  48 */    SET_LEVEL_BUTTON,
/*  49 */    NOT_USED_27,
/*  50 */    FLOW_UP,
/*  51 */    FLOW_DOWN,
/*  52 */    NOT_USED_28,
/*  53 */    NOT_USED_29,
/*  54 */    NOT_USED_30,
/*  55 */    NOT_USED_31,
/*  56 */    NOT_USED_32,
/*  57 */    SAVE_CURRENT_PROFILE_BUTTON,
/*  58 */    SAVE_NEW_PROFILE_BUTTON,
/*  59 */    NOT_USED_33,
/*  60 */    LIST_PROFILES,
/*  61 */    NOT_USED_34,
/*  62 */    NOT_USED_35,
/*  63 */    DELETE_PROFILE_BUTTON,
/*  64 */    IV_BAG,
/*  65 */    NEW_USER_PROFILE_BUTTON,
/*  66 */    IV_BAG_NOT_AUTO,
/*  67 */    IV_BAG_AUTO,
/*  68 */    ENABLE_WIRELESS_FP_BUTTON,
/*  69 */    DISALBLE_WIRELESS_FP_BUTTON,
/*  70 */    ENABLE_ICW_BUTTON,
/*  71 */    DISABLE_ICW_BUTTON,
/*  72 */    NOT_USED_36,
/*  73 */    SAVE_BUTTON,
/*  74 */    CANCEL_BUTTON,
/*  75 */    OK_BUTTON,
/*  76 */    BACK_BUTTON,
/*  77 */    NOT_USED_37,
/*  78 */    COAG_LEVEL_1_BUTTON,
/*  79 */    COAG_LEVEL_2_BUTTON,
/*  80 */    THERMOMETER_BUTTON,
/*  81 */    NOT_USED_38,
/*  82 */    NOT_USED_39,
/*  83 */    NOT_USED_40,
/*  84 */    NOT_USED_41,
/*  85 */    NOT_USED_42,
/*  86 */    NOT_USED_43,
/*  87 */    NOT_USED_44,
/*  88 */    WAND_NAME_BUTTON,
/*  89 */    NOT_USED_45,
/*  90 */    SPEAKER_BUTTON,
/*  91 */    PROFILE_BUTTON,
/*  92 */    USER_TONE_BUTTON,
/*  93 */    USER_TONE_PLUS_BUTTON,
/*  94 */    USER_TONE_MINUS_BUTTON,
/*  95 */    AMBIENT_ALARM_PLUS_BUTTON,
/*  96 */    AMBIENT_ALARM_MINUS_BUTTON,
/*  97 */    NOT_USED_46,
/*  98 */    NOT_USED_47,
/*  99 */    HI_LEVEL_MINUS,
/* 100 */    HI_LEVEL_DEFAULT,
/* 101 */    HI_LEVEL_PLUS,
/* 102 */    MED_LEVEL_MINUS,
/* 103 */    MED_LEVEL_DEFAULT,
/* 104 */    MED_LEVEL_PLUS,
/* 105 */    LO_LEVEL_MINUS,
/* 106 */    LO_LEVEL_DEFAULT,
/* 107 */    LO_LEVEL_PLUS,
/* 108 */    MAX_NUM_BUTTONS,
}GRAPHIC_ID;



typedef void (*GUI_Jump_Table) (uint8_t);

typedef enum
{
  PRIME_OFF    = 0,
  PRIME_MANUAL    ,
  PRIME_AUTO      ,
  PRIME_MODEn    //max number of prime modes
} prime_mode_t;

/* Screens */
#define MAX_NUM_SCREENS 54
#define SPLASH_SCREEN 0x1
#define FAULT_SCREEN 0x2
#define WAND_DETECTION_SCREEN 0x3
#define INSERT_TUBE_HELP_SCREEN 0x4
#define CLOSE_PUMP_DOOR_HELP_SCREEN 0x5
#define CONNECT_FOOTPEDAL_HELP_SCREEN 0x6
#define DELETE_INDIVIDUAL_USER_PROFILE_CHANCE_TO_BACK_OUT_SCREEN 0x7
#define SET_LEVEL_SCREEN 0x8
#define SET_DEFAULT_FINGERSWITCH_SETTINGS_SCREEN 0x9
#define MAIN_SETTINGS_SCREEN 0xA
#define PROFILE_SAVE_SCREEN 0xB
#define USER_NOTIFICATION_TONES_AND_AMBIENT_SETTINGS_SCREEN 0xC
#define USER_PROFILES_SCREEN 0xD
#define ENTER_NEW_USER_PROFILE_NAME_SCREEN 0xE
#define WIRELESS_FOOTPEDAL_HANDSHAKE_SCREEN 0xF
#define EDIT_USER_PROFILE_NO_WAND_ATTACHED_SCREEN 0x10
#define EDIT_USER_PROFILE_WAND_ATTACHED_SCREEN 0x11
#define SWITCHING_SETTINGS_SCREEN 0x12
#define LANGUAGES_SCREEN 0x13
#define WAND_INFO_SCREEN 0x14
#define FIRMWARE_UPDATE_SCREEN 0x15
#define ERROR_SCREEN 0x16
#define OPEN_PUMP_DOOR_HELP_SCREEN 0x18
#define SAVE_SETTINGS_SCREEN 0x1A
#define SAVE_SETTINGS_CONFIRMATION_SCREEN 0x1B
#define RF_2000_WAND_TREATMENT_SCREEN 0x28
#define LEGACY_6_8_WAND_TREATMENT_SCREEN 0x2A
#define TOPAZ_WAND_TREATMENT_SCREEN 0x2B
#define LEGACY_7_9_WAND_TREATMENT_SCREEN 0x2C
#define LEGACY_7_10_WAND_TREATMENT_SCREEN 0x2D
#define LEGACY_ENT_WAND_TREATMENT_SCREEN 0x33
#define ENT_SMART_WAND_TREATMENT_SCREEN 0x34
#define MANTA_SMART_WAND_TREATMENT_SCREEN 0x35

uint8_t update_gui(void);
uint8_t update_screen(void);
uint8_t load_screen01(void);
prime_mode_t get_priming_status(void);
void gui_stop_manual_prime(void);
uint8_t change_flow_manta(uint8_t coag_level, uint8_t new_flow);
void gui_stop_auto_prime(void);
void gui_clear_auto_prime_event(void);

#endif
