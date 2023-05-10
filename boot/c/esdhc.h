#ifndef ESDHC_H
#define ESDHC_H


#define BIT_MODE 			DTW_4BIT

uint8_t esdhc_init(void);
void Clear_Memory(uint32_t*,uint32_t);
void Set_Memory_Pattern2(uint32_t*,uint32_t, uint32_t);
void esdhc_set_dma_addr(uint32_t);
void CMD24_Write_Block2(uint32_t);
void CMD12_STOP_TRANS(void);
void CMD17_Read_Block2(uint32_t card_addr);
uint32_t Check_Data(uint32_t*,uint32_t*,uint32_t);
uint32_t Rd_Response48(void);
int esdhc_get_cihb(void);
int esdhc_get_cdihb(void);
void Rd_Response128(void);
void wait_wdata_tc(void);
int label_card_type(void);
void CMD0_GO_IDLE_STATE(void);
uint32_t CMD8_SEND_IFCOND0(void);
void CMD55_APP_CMD (uint32_t);
void Init_SD_Card(uint8_t);
uint32_t ACMD41_SEND_OPCON(uint32_t);
int esdhc_get_cardbusy(void);
void CMD2_ALL_SEND_CID(void);
uint32_t CMD3_SD_GET_RCA();
void CMD9_SEND_CSD(uint32_t);
uint32_t CMD13_SEND_STATUS(uint32_t);
void CMD7_CARD_SELECT(uint32_t);
void ACMD6_SWITCH(uint32_t, uint8_t);
void ACMD13_SD_STATUS(uint32_t, uint8_t);

#endif