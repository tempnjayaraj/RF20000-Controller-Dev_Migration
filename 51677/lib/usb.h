#ifndef __USB__
#define __USB__

#include "common.h"


/*********** HIL ***************/
#include "usb_cdc.h"
#define USB_InterfaceReq_Handler  CDC_InterfaceReq_Handler   
/********************************/

/* Macros */
#define DISABLE_USB (USBCTL0 = 0x00)    /* Disable USB module */
#define ENDPOINTS   3

/* EP0 settings */
#define EP0_SIZE            32


/********** Enpoint Value Options ***********/
/*                                          */
/* IN       Enabled for IN transactions     */
/* OUT      Enabled for OUT transactions    */
/* DISABLE  EndPoint Disabled               */
/*                                          */
/********************************************/


/* EP1 settings */
#define EP1_VALUE           _EP_IN
#define EP1_TYPE            INTERRUPT
#define EP1_SIZE            32
#define EP1_BUFF_OFFSET     0x18


/* EP2 settings */
#define EP2_VALUE           _EP_IN
#define EP2_TYPE            BULK
#define EP2_SIZE            64
#define EP2_BUFF_OFFSET     0x20

/* EP3 settings */
#define EP3_VALUE           _EP_OUT
#define EP3_TYPE            BULK
#define EP3_SIZE            64
#define EP3_BUFF_OFFSET     0x28

/* EP4 settings */
#define EP4_VALUE           DISABLE
#define EP4_SIZE            1
#define EP4_BUFF_OFFSET     0x08

/* EP5 settings */
#define EP5_VALUE           DISABLE
#define EP5_SIZE            1
#define EP5_BUFF_OFFSET     0x08

/* EP6 settings */
#define EP6_VALUE           DISABLE
#define EP6_SIZE            1
#define EP6_BUFF_OFFSET     0x08

/* Macros */
#define EP3_CTR   tBDTtable[4].Stat._byte= kSIE 


/* MACROS */
#define usbSIE_CONTROL(EP)   (tBDTtable[EP<<2].Stat._byte= kSIE)
#define usbMCU_CONTROL(EP)   (tBDTtable[EP<<2].Stat._byte= kMCU)
#define usbEP_Reset(EP)      (tBDTtable[EP<<2].Cnt=0x0040)



#define _EP_IN      USB_ENDPT_EPTXEN_MASK
#define _EP_OUT     USB_ENDPT_EPRXEN_MASK




//USB0_ENDPT_EPTXEN_MASK

//USB0_ENDPT_EPRXEN_MASK

#define DISABLE 0

#define INTERRUPT   0x03
#define BULK        0x05


// BDT status value
#define kMCU      0x00
#define kSIE      0x80

#define kUDATA0   0x88
#define kUDATA1   0xC8

                            

#define mSETUP_TOKEN    0x0D
#define mOUT_TOKEN      0x01
#define mIN_TOKEN       0x09




    // USB commands
#define mGET_STATUS           0
#define mCLR_FEATURE          1
#define mSET_FEATURE          3
#define mSET_ADDRESS          5
#define mGET_DESC             6
#define mSET_DESC             7
#define mGET_CONFIG           8
#define mSET_CONFIG           9
#define mGET_INTF             10
#define mSET_INTF             11
#define mSYNC_FRAME           12
#define	mGET_MAXLUN	          0xFE		// Mass Storage command
  
#define mDEVICE		            1
#define mCONFIGURATION	            2
#define mSTRING	        	    3
#define mINTERFACE	            4
#define mENDPOINT       	    5
#define	mDEVICE_QUALIFIE            6
#define mOTHER_SPEED_CONFIGURATION  7
#define mINTERFACE_POWER	    8


#define mREPORT                     0x22


/* Request Types */
#define STANDARD_REQ    0x00
#define SPECIFIC_REQ    0x20
#define VENDORSPEC_REQ  0x40
#define DEVICE_REQ      0x00
#define INTERFACE_REQ   0x01
#define ENDPOINT_REQ    0x02



enum
{
    uSETUP,
    uDATA
};

enum
{
    EP0,
    EP1,
    EP2,
    EP3,
    EP4,
    EP5,
    DUMMY,
    LOADER
    
};


enum
{
    uPOWER,
    uENUMERATED,
    uENABLED,
    uADDRESS,
    uREADY    
};
enum
{
    fIN,
    fOUT
};

enum
{
    bEP0OUT_ODD,
    bEP0OUT_EVEN,
    bEP0IN_ODD,
    bEP0IN_EVEN,
    bEP1OUT_ODD,
    bEP1OUT_EVEN,
    bEP1IN_ODD,
    bEP1IN_EVEN,
    bEP2OUT_ODD,
    bEP2OUT_EVEN,
    bEP2IN_ODD,
    bEP2IN_EVEN,
    bEP3OUT_ODD,
    bEP3OUT_EVEN,
    bEP3IN_ODD,
    bEP3IN_EVEN
};


/***** Data Types *****/
/*     */

typedef union _tBDT_STAT
{
    uint8_t _byte;
    struct
    {
        uint8_t :1;
        uint8_t :1;
        uint8_t BSTALL:1;              //Buffer Stall Enable
        uint8_t DTS:1;                 //Data Toggle Synch Enable
        uint8_t NINC:1;                //Address Increment Disable
        uint8_t KEEP:1;                //BD Keep Enable
        uint8_t DATA:1;                //Data Toggle Synch Value
        uint8_t UOWN:1;                //USB Ownership
    }McuCtlBit;
       
    struct
    {
        uint8_t    :2;
        uint8_t PID:4;                 //Packet Identifier
        uint8_t    :2;
    }RecPid;
} tBDT_STAT;                         //Buffer Descriptor Status Register



typedef struct _tBDT
{
    tBDT_STAT Stat;
    uint8_t  dummy;
    uint16_t Cnt;
    uint32_t Addr;             
} tBDT;                             





#define SWAP16(val)                                                 \
    (uint16_t)((((uint16_t)(val) >> 0x8) & 0xFF) |                    \
    (((uint16_t)(val) & 0xFF) << 0x8))

#define SWAP32(val)                                                 \
    (uint32_t)((SWAP16((uint32_t)(val) & (uint32_t)0xFFFF) << 0x10) |  \
    (SWAP16((uint32_t)((val) >> 0x10))))


typedef struct _tUSB_Setup 
{
       uint8_t bmRequestType;
       uint8_t bRequest;
       uint8_t wValue_l;
       uint8_t wValue_h;
       uint8_t wIndex_l;
       uint8_t wIndex_h;
       uint8_t wLength_l;
       uint8_t wLength_h;
}tUSB_Setup;



/* Prototypes */
void USB_Testing_Read_Registers(void);
void USB_Testing_Write_Registers(void);
void USB_Init(void);
void USB_EP0_IN_Handler(void);
void EP_IN_Transfer(uint8_t,uint8_t*,uint8_t);
uint8_t EP_OUT_Transfer(uint8_t,uint8_t*);
void USB_Stall_Handler(void);
void USB_Stall_Handler(void);
void USB_EP0_OUT_Handler(void);
void USB_EP0_Stall(void);
void USB_Set_Interface(void);
uint16_t USB_EP_OUT_SizeCheck(uint8_t u8EP);
void USB_Reset_Handler(void);
void Custom_Fields_Assignment(void);
void USB_Endpoint_Setup_Handler(void);
void usb_isr(void);
uint8_t poll_usb(void);



#endif /* __USB__*/