

/*******************************************************************************
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
********************************************************************************
$Author::               $:
$Rev::                  $:
$Date::                 $:

CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
********************************************************************************
*******************************************************************************/


#include <stdint.h>
#include "commonTypes.h"
#include "usb.h"
#include "ct_app_ww.h"
#include "Settings.h"
#include "MK26F18.h"
#include "system.h"
#include "event_buffer.h"
#include "msgWerewolf.h"
#include "wand.h"

uint8_t bdm_poll_usb (void);
void in_bdm_wcd_msg (void);
bool prepare_therapy_message(void);
bool prepare_port_message(void);