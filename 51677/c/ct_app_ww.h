/*CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.50
*******************************************************************************/
/***************************************************************************//**
\file:     ct_app_ww.h
\brief     App for Connected Network Werewolf
\details   
*******************************************************************************/

#ifndef CT_APP_WW_H
#define CT_APP_WW_H

#include <stdint.h>
//#include "commonTypes.h"

#define PROT_VERSION     0x21
#define discProtocol     0x31
#define heartBeat        0x32
#define COBLATION        0x03
#define WEREWOLF         0x01      


#define HEADER_SIZE_WCD_MSGS          4
#define READ_SIZE_REMAINING_HEADER    2


//typedef struct bdm_serial_msg_container {
//   uint8_t  protocol_id; 
//   uint8_t  command_id;
//   uint8_t  cmd_sequence_number;
//   uint8_t  cmd_data_len;
//   uint8_t *cmd_data;
//} bdm_serial_msg_container_t, *pMsgSerial;

typedef union {
  uint8_t val;
  struct{
    uint8_t request_num:7;
    uint8_t req_num_msb:1;
  } bits;
} req_num;

typedef struct bdm_serial_msg_container1 {
   uint8_t  protocol_id; 
   uint8_t  command_id;
   req_num  cmd_sequence_number;
   uint8_t  cmd_data_len;
   uint8_t  cmd_data[50];
} bdm_serial_msg_container_t;


//void display_msg(pMsgSerial msg);

#define RESET_MSG(msg) (reset_array(msg,sizeof(bdm_serial_msg_container_t)))

/**
 * more prototypes for utility commands
 */
//void freeMsgContainerData(pMsgSerial msg); 
//int  read_serial_msg(int *fd, pMsgSerial *rxMsg, pExpCmds expCmds, uint8_t numCmds, bool *stopFlag);
//int  build_send_serial_msg(int socket_fd, pMsgSerial txMsg, pthread_mutex_t *pLock);
//void display_blob(pBlobDataHdr pHeader, uint8_t *blobData, uint8_t count);

///////////////////////////////////////
///////////////////////////////////////
// serial rx thread 
///////////////////////////////////////
///////////////////////////////////////
int  init_serial_connection(int *serial_fd, char *comport);  /**< inits the serial port */
void *serial_rx(void *args);                  /**< main serial rx task   */

/**
 * dispatch function called
 */
//typedef int (*dispatch_function_t2)(pMsgSerial rxMsg);  /**< function prototype for dispatch function */

typedef struct bdm_scd_exp_cmds {
   uint8_t expCmd;
   uint8_t expCmdLen; 
} bdm_scd_exp_cmds_t, *pExpCmds;

/**
 * serial RX args for serial rx thread
 */
//typedef struct serial_rx_args 
//{
//   dispatch_function_t2 serialProcessData;
//   int     *serial_fd;
//   uint8_t       numCmds;
//   pExpCmds pCmds;
//   bool    *stopFlag;
//} serial_rx_args_t, *pSerialRxArgs;


#define BDM_TO_SCD_GENERIC_RESPONSE CAPDEV_GENERIC_REPLY
#define SCD_TO_BDM_GENERIC_RESPONSE CAPDEV_GENERIC_REPLY
#define GENERIC_REPLY_DATA_LENGTH (sizeof(capdev_generic_reply_t))
#define ONE_BYTE_ACK_RESPONSE_DATA_LEN 1

/******************************
///////////////////////////////
/// Discovery
///////////////////////////////
*******************************/
typedef struct bdm_to_capdev_discover {
   uint8_t version_number;
} bdm_to_capdev_discovery_t, *pBdm2CapdevDisc;

#define BDM_TO_CAPDEV_DISCOVERY_REQ_LEN (sizeof(bdm_to_capdev_discovery_t))

typedef struct capdev_to_bdm_discovery {
   uint8_t version_number;
   uint8_t device_state;
   uint8_t devType;
   uint8_t devSubType;
} capdev_to_bdm_discovery_t, *pCapdev2BdmDisc;

#define CAPDEV_TO_BDM_DISCOVERY_RESP_LENGTH (sizeof(capdev_to_bdm_discovery_t))

typedef struct common_blob_top_msg {
   uint8_t  blobNum;
   uint8_t  totalBlobNum;
} common_blob_top_msg_t, *pBlobTop;

#define BLOB_DATA_OFFSET (sizeof(common_blob_top_msg_t))
#define MAX_OPAQUE_DATA_PACKET  (48)

typedef struct blob_data_ack {
   uint8_t packetNumber;
   uint8_t totalPackets;
   uint8_t status; // ack/nack error
} blob_data_ack_t, *pBlobDataAck;

#define GET_BLOB_DATA_REQUEST_DATA_LEN      (0)
#define GET_BLOB_DATA_RESPONSE_ACK_DATA_LEN (sizeof(blob_data_ack_t))
#define SET_BLOB_DATA_RESPONSE_DATA_LEN     (sizeof(blob_data_ack_t))

#endif