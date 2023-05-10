/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              can.c
Author:            Chris Wallis
Date:              10-SEPT-2012
Modified:          
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID 3
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.30.4.3295
*******************************************************************************/

/*******************************************************************************
Comments:
FlexCAN driver.

NOTE: Baud rate set to 1Mbps.

*******************************************************************************/

#ifdef DEBUG_CAN

  #include "can_debug.h"
  #include "wand.h"
  #include "system.h"
  #include <string.h>

  const static CAN_MemMapPtr pFlexCAN = CAN0_BASE_PTR; 
  DEBUG CANdebug;
  //volatile DEBUG *pCANdebug = &CANdebug;

/******************************************************************************
* Function: FlexCAN_Write
*
* Purpose: Write to FlexCAN.
******************************************************************************/
uint8_t flexCAN_write_pumpdebug(void)
{

  uint32_t word0, word1;
  __root static uint32_t tx_count = 0;
  uint8_t *pword0 = (uint8_t*) &word0;
  uint8_t *pword1 = (uint8_t*) &word1;
  
  DEBUG TxCANdebug;
  
  TxCANdebug = CANdebug;
 
  PWORD_CLEAR(pword0, pword1);

 /******************************************************************************
  * Message Name: GEN Debug Message #1
 * Arbitration ID: 0x10
 ******************************************************************************/
  
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(0)) == CAN_IFLAG1_BUF31TO8I(0))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(0);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(0);
       
    //(*(uint32_t *)pword0) = ++tx_count;
    (*(uint32_t *)pword0) = (CANdebug.adc.vmod1 << 0)|
                            (CANdebug.adc.vmod2 << 16);

    swap_4bytes(pword0);  
    
    (*(uint32_t *)pword1) =  
              (CANdebug.adc.irms1 << 0) |
              (CANdebug.adc.irms2 << 16);
    swap_4bytes(pword1);
          
    /* GEN Debug Message 1 (0x10) */
    pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(GEN_DEBUG_1_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    pFlexCAN->MB[GEN_DEBUG_1_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
  
  }

 /******************************************************************************
 * Message Name: GEN Debug Message #2
 * Arbitration ID: 0x11
 ******************************************************************************/  
  
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(1)) == CAN_IFLAG1_BUF31TO8I(1))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(1);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(1);
  
    (*(uint32_t *)pword0) =  
      (*(uint32_t*)&TxCANdebug.pump.error);
    swap_4bytes(pword0);
    
    (*(uint32_t *)pword1) =  
      (*(uint32_t*)&TxCANdebug.pump.pid_output);
    swap_4bytes(pword1);
    
    /* GEN Debug Message 2 (0x11) */
    pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].ID = CAN_ID_PRIO(1) | CAN_ID_STD(GEN_DEBUG_2_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    //pFlexCAN->MB[GEN_DEBUG_2_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);  
    
  }

 /******************************************************************************
 * Message Name: GEN Debug Message #3
 * Arbitration ID: 0x12
 ******************************************************************************/  
  
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(2)) == CAN_IFLAG1_BUF31TO8I(2))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(2);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(2);
  
    (*(uint32_t *)pword0) = (uint32_t) (TxCANdebug.pump.pv);
    swap_4bytes(pword0);
    (*(uint32_t *)pword1) = (*(uint32_t*)(&TxCANdebug.pump.filt_pv));
    swap_4bytes(pword1);
    
    /* GEN Debug Message 2 (0x11) */
    pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].ID = CAN_ID_PRIO(2) | CAN_ID_STD(GEN_DEBUG_3_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    //pFlexCAN->MB[GEN_DEBUG_3_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);  
    
  }
  
 /******************************************************************************
 * Message Name: GEN Debug Message #4
 * Arbitration ID: 0x15
 ******************************************************************************/
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(5)) == CAN_IFLAG1_BUF31TO8I(5))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(5);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(5);
  
    (*(uint32_t *)pword0) =  
     ((TxCANdebug.wand.handle << CAN_GEN_TX_HANDLE_SHIFT) |
      (TxCANdebug.wand.ambient << CAN_GEN_TX_AMBIENT_SHIFT));
                              
    swap_4bytes(pword0);
    
    (*(uint32_t *)pword1) = TxCANdebug.wand.z_high_mod;
    swap_4bytes(pword1);
        
    /* GEN Debug Message 2 (0x11) */
    pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].ID = CAN_ID_PRIO(3) | CAN_ID_STD(GEN_DEBUG_6_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    //pFlexCAN->MB[GEN_DEBUG_6_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);  
    
  }
   return 0;
}


/******************************************************************************
* Function: FlexCAN_Write
*
* Purpose: Write to FlexCAN.
******************************************************************************/
void FlexCAN_Write_PWMDebug(void) {
 
  uint32_t word0, word1;
  __root static uint32_t tx_count = 0;
  uint8_t *pword0 = (uint8_t*) &word0;
  uint8_t *pword1 = (uint8_t*) &word1;
  
  DEBUG TxCANdebug;
  
  TxCANdebug = CANdebug;
 
  PWORD_CLEAR(pword0, pword1);
  
 /******************************************************************************
 * Message Name: GEN Debug Message #5 (not in use currently)
 * Arbitration ID: 0x13
 ******************************************************************************/
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(3)) == CAN_IFLAG1_BUF31TO8I(3))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);
       
    (*(uint32_t *)pword0) =  
     ((TxCANdebug.pwm.c0v << CAN_GEN_TX_PWM_C0V_SHIFT) |
      (TxCANdebug.pwm.c1v << CAN_GEN_TX_PWM_C1V_SHIFT));
    swap_4bytes(pword0);
    
    (*(uint32_t *)pword1) =  
     ((TxCANdebug.pwm.c2v << CAN_GEN_TX_PWM_C2V_SHIFT) |
      (TxCANdebug.pwm.c3v << CAN_GEN_TX_PWM_C3V_SHIFT));
    swap_4bytes(pword1);
          
    /* GEN Debug Message 1 (0x10) */
    pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(GEN_DEBUG_4_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    pFlexCAN->MB[GEN_DEBUG_4_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
  
  }

 /******************************************************************************
 * Message Name: GEN Debug Message #6 (not in use currently)
 * Arbitration ID: 0x14
 ******************************************************************************/ 
  
 /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(4)) == CAN_IFLAG1_BUF31TO8I(4))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(4);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(4);
       
    (*(uint32_t *)pword0) =  
     ((TxCANdebug.pwm.c4v << CAN_GEN_TX_PWM_C4V_SHIFT) |
      (TxCANdebug.pwm.c5v << CAN_GEN_TX_PWM_C5V_SHIFT));
    swap_4bytes(pword0);
    
    (*(uint32_t *)pword1) =  
     ((TxCANdebug.pwm.c6v << CAN_GEN_TX_PWM_C6V_SHIFT) |
      (TxCANdebug.pwm.c7v << CAN_GEN_TX_PWM_C7V_SHIFT));
    swap_4bytes(pword1);
          
    /* GEN Debug Message 1 (0x10) */
    pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(GEN_DEBUG_5_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    pFlexCAN->MB[GEN_DEBUG_5_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
  
  }
   
}

/******************************************************************************
* Function: GEN Debug Message #7
*
* Purpose: Write to FlexCAN.
******************************************************************************/
uint8_t flexCAN_write_wanddebug(void)
{

  uint32_t word0, word1;
  static uint16_t idx = 0;
  uint8_t *pword0 = (uint8_t*) &word0;
  uint8_t *pword1 = (uint8_t*) &word1;
  
  uint8_t txCANdebug[8] = {0, 0, 0, 0, 0, 0, 0, 0}, nbytes = 0;
   
  PWORD_CLEAR(pword0, pword1);
  
 /******************************************************************************
 * Message Name: GEN Debug Message #7 
 * Arbitration ID: 0x16
 ******************************************************************************/
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(3)) == CAN_IFLAG1_BUF31TO8I(3))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    if (idx < NUM_WAND_DATABASE_PAGES)
    {
      
      nbytes = 8;
      memcpy(&txCANdebug[0],(&db.passcode.head+idx),nbytes);
      idx += 8;
      
    }
    else 
    {
      
      nbytes = (sizeof(SMART_WAND_DATABASE) % 8);
      memcpy(&txCANdebug[0],(&db.passcode.head+idx),nbytes);
      idx = 0;
      
    }
             
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);
       
    (*(uint32_t *)pword0) = ((txCANdebug[0] << 0) |
                             (txCANdebug[1] << 8) |
                             (txCANdebug[2] << 16) |
                             (txCANdebug[3] << 24));
    swap_4bytes(pword0);
    
    (*(uint32_t *)pword1) = ((txCANdebug[4] << 0) |
                             (txCANdebug[5] << 8) |
                             (txCANdebug[6] << 16) |
                             (txCANdebug[7] << 24));
    swap_4bytes(pword1);
                     
    /* GEN Debug Message 7 (0x16) */
    pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(GEN_DEBUG_7_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    pFlexCAN->MB[GEN_DEBUG_7_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
  
  }
   return 0;
}
       
/******************************************************************************
* Function: FlexCAN_Write
*
* Purpose: Write to FlexCAN.
******************************************************************************/
uint8_t flexCAN_write_wireless(void)
{
 
  uint32_t word0, word1;
  uint8_t *pword0 = (uint8_t*) &word0;
  uint8_t *pword1 = (uint8_t*) &word1;
  
  PWORD_CLEAR(pword0, pword1);
  
 /******************************************************************************
 * Message Name: GEN Debug Message #8
 * Arbitration ID: 0x17
 ******************************************************************************/
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(7)) == CAN_IFLAG1_BUF31TO8I(7))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(7);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(7);
       
    (*(uint32_t *)pword0) = (CANdebug.adc.stator << 0) |
                            (CANdebug.adc.temperature << 16);
     

    swap_4bytes(pword0);
    
    (*(uint32_t *)pword1) = ((ADC0_CFG1 & ADC_CFG1_ADIV_MASK) >> ADC_CFG1_ADIV_SHIFT);
    swap_4bytes(pword1);
          
    /* GEN Debug Message 1 (0x10) */
    pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(GEN_DEBUG_8_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    pFlexCAN->MB[GEN_DEBUG_8_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
  
  }
  
  return 0;
   
}

/******************************************************************************
* Function: FlexCAN_Write
*
* Purpose: Write to FlexCAN.
******************************************************************************/
uint8_t flexCAN_write_wand_wear_debug(void)
{
 
  uint32_t word0, word1;
  uint8_t *pword0 = (uint8_t*) &word0;
  uint8_t *pword1 = (uint8_t*) &word1;
  
  PWORD_CLEAR(pword0, pword1);
  
 /******************************************************************************
 * Message Name: GEN Debug Message #9
 * Arbitration ID: 0x18
 ******************************************************************************/
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(3)) == CAN_IFLAG1_BUF31TO8I(3))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(3);
       
    (*(uint32_t *)pword0) = 0;
     
    swap_4bytes(pword0);
    
    (*(uint32_t *)pword1) = 0;
     
    swap_4bytes(pword1);
          
    /* GEN Debug Message 1 (0x10) */
    pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(GEN_DEBUG_9_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    pFlexCAN->MB[GEN_DEBUG_9_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
  
  }
  
 /******************************************************************************
 * Message Name: GEN Debug Message #10
 * Arbitration ID: 0x19
 ******************************************************************************/
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(4)) == CAN_IFLAG1_BUF31TO8I(4))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(4);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(4);
       
    //(*(uint32_t *)pword0) = CANdebug.wand.irms_wear_threshold_lo;
    (*(uint32_t *)pword0) = 0;
     
    swap_4bytes(pword0);
    
    //(*(uint32_t *)pword1) = CANdebug.wand.irms_wear_threshold_medhi;
     (*(uint32_t *)pword1) = ((CANdebug.wand.worn_count << 24) |
                              (CANdebug.wand.irms_worn << 16));
     
    swap_4bytes(pword1);
          
    /* GEN Debug Message 1 (0x10) */
    pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(GEN_DEBUG_10_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    //pFlexCAN->MB[GEN_DEBUG_10_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
  
  }
  
  /******************************************************************************
 * Message Name: GEN Debug Message #11
 * Arbitration ID: 0x1A
 ******************************************************************************/
  /* Step 1: Check & clear interrupt */
  if ((pFlexCAN->IFLAG1 & CAN_IFLAG1_BUF31TO8I(6)) == CAN_IFLAG1_BUF31TO8I(6))
  {
    
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(6);
    
  }
  
  /* Step 2: Abort pending transmission (if applicable) */
  if (pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_DATA) == CAN_CS_CODE(MB_CODE_TX_DATA))
  {  
    
    pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_ABORT);
    
  }

  /* Step 3: Verify transmissin complete or aborted */  
  if (((pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_ABORT)) == CAN_CS_CODE(MB_CODE_TX_ABORT)) ||
      ((pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].CS & CAN_CS_CODE(MB_CODE_TX_INACTIVE)) == CAN_CS_CODE(MB_CODE_TX_INACTIVE)) ||
      ((pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].CS) == CAN_CS_CODE(0)))
  {
    
    /* Step 3a: Clear interrupt */
    pFlexCAN->IFLAG1 |= CAN_IFLAG1_BUF31TO8I(6);
       
    (*(uint32_t *)pword0) = ((CANdebug.wand.thandle_wear[0] << 0) |
                             (CANdebug.wand.thandle_wear[1] << 8) |
                             (CANdebug.wand.thandle_wear[2] << 16) |
                             (CANdebug.wand.thandle_wear[3] << 24));
     
    swap_4bytes(pword0);
    
    (*(uint32_t *)pword1) = ((CANdebug.wand.thandle_wear[4] << 0) |
                             (CANdebug.wand.thandle_wear[5] << 8) |
                             (CANdebug.wand.thandle_wear[6] << 16) |
                             (CANdebug.wand.thandle_worn << 24));
     
    swap_4bytes(pword1);
          
    /* GEN Debug Message 1 (0x10) */
    pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_INACTIVE); 
    pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].ID = CAN_ID_PRIO(0) | CAN_ID_STD(GEN_DEBUG_11_MSG_ARB_ID);    
    pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].WORD0 = (*(uint32_t *)pword0);
    pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].WORD1 = (*(uint32_t *)pword1);

    /* Clear data words */
    PWORD_CLEAR(pword0, pword1);
    
    /* Transmit frame */
    //pFlexCAN->MB[GEN_DEBUG_11_MSG_MB].CS = CAN_CS_CODE(MB_CODE_TX_DATA) | CAN_CS_DLC(8);
  
  }
  
  return 0;
   
}



#endif