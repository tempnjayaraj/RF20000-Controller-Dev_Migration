/*
 * File:		USB_device.c
 * Purpose:		Main process
 *
 */

#include "usb_cdc.h"    /* USB DCD support */
#include "usb_reg.h"    /* USB regulator */
#include "Settings.h"
#include "mcg.h"

/* Extern Variables */
extern uint8_t gu8USB_Flags; 
extern uint8_t gu8EP3_OUT_ODD_Buffer[];
extern tBDT tBDTtable[];


/********************************************************************/
/*
void main (void)
{
    printf("\n********** USB Device Module Testing **********\n");
    
  SIM_SOPT1CFG |= SIM_SOPT1CFG_URWE_MASK |
                  SIM_SOPT1CFG_USSWE_MASK |
                  SIM_SOPT1CFG_UVSWE_MASK;
  
    // USB CDC Initialization
    CDC_Init();

    while(1)
    {
       // USB CDC service routine
          CDC_Engine();

        // If data transfer arrives
        if(FLAG_CHK(EP_OUT,gu8USB_Flags))
        {
            (void)USB_EP_OUT_SizeCheck(EP_OUT);         
            usbEP_Reset(EP_OUT);
            usbSIE_CONTROL(EP_OUT);
            FLAG_CLR(EP_OUT,gu8USB_Flags);

            // Send it back to the PC
            EP_IN_Transfer(EP2,CDC_OUTPointer,1);

        }             

    }
}
*/

