#include "unity.h"
#include "MK26F18.h"
#include <string.h>
#include "esdhc.h"
#include "cpu.h"
#include "Mockftfe.h"
#include "Mocksystem.h"
#include "Mockwdog.h"
#include "Mockcrc.h"
#include "Mocksw_manager.h"
#include "Mocklegacy_wand.h"
#include "Mockevent_buffer.h"

#define MFG_SERIAL_NUM_CHARS            16

uint8_t wand_log_data_test[4];

GEN_SETTINGS current_therapy_settings;
SMART_WAND_PARAMETERS smart_wand_params;
uint8_t serial_number[MFG_SERIAL_NUM_CHARS];

event_table evt_tbl;
uint8_t eeram[4096];
legacy_wand_params legacy_wand_params_obj;

void setUp(void)
{
 }

void tearDown(void)
{
}

void test_download_sdhc_data (void)
{
   uint8_t rv = download_sdhc_data();
   TEST_ASSERT_EQUAL(0,rv);
}

void test_download_sdhc_data_1 (void)
{
   uint8_t rv = download_sdhc_data();
   TEST_ASSERT_EQUAL(0,rv);
}

void test_download_sdhc_data_2 (void)
{
   uint8_t rv = download_sdhc_data();
   TEST_ASSERT_EQUAL(0,rv);
}

void test_download_sdhc_data_3 (void)
{
   uint8_t rv = download_sdhc_data();
   TEST_ASSERT_EQUAL(0,rv);
}

void test_download_sdhc_data_4 (void)
{
   uint8_t rv = download_sdhc_data();
   TEST_ASSERT_EQUAL(0,rv);
}

void test_esdhc_init(void)
{
// Cannot run test on esdhc because of while loop
uint8_t rv = esdhc_init();

}

void test_sd_card_rw (void)
{
uint8_t rv = sd_card_rw();
}

void test_log2sdhc(void)
{
   uint8_t rv = log2sdhc();
}

void test_sd_card_init(void)
{
   uint8_t rv = sd_card_init(1);
}

void test_sd_card_init_2(void)
{
   uint8_t rv = sd_card_init(0);
}


void test_sd_card_log (void)
{
   uint8_t reset = 1;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_ERROR;
   uint8_t *p_data = &wand_log_data_test[0];
   uint8_t rv = sd_card_log(reset, cmd, p_data);
}

void test_sd_card_log_1 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = 0;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_sd_card_log_2 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = 0;
   uint8_t *p_data = &wand_log_data_test[0];
   //   put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_sd_card_log_3 (void)
{
   uint8_t reset = STATIC_LOG_ERROR;
   SDHC_LOGGING_CMDS cmd = 0;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_sd_card_log_4 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_ERROR;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_sd_card_log_5 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_ERROR;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_sd_card_log_6 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_ERROR;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_sd_card_log_7 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_ERROR;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}


void test_sd_card_log_8 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_ERROR;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}


void test_sd_card_log_9 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_ERROR;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
   TEST_ASSERT_EQUAL(0,rv);
}

void test_sd_card_log_10 (void)
{
   uint8_t reset = 1;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_THERAPY_DATA;
   uint8_t *p_data = &wand_log_data_test[0];
   uint8_t rv = sd_card_log(reset, cmd, p_data);
}

void test_sd_card_log_11 (void)
{
   uint8_t reset = 0;
   SDHC_LOGGING_CMDS cmd = STATIC_LOG_THERAPY_DATA;
   uint8_t *p_data = &wand_log_data_test[0];
   //put_event_ExpectAndReturn(&evt_tbl,MFG_DATE_TIME_FAULT,1);
   uint8_t rv = sd_card_log(reset, cmd, p_data);
}



// uint32_t download_sdhc_data2flash(uint32_t);
// void read_sdhc_mbr(mbr_sector*);
// 
// void Clear_Memory(uint32_t*,uint32_t);
// void Set_Memory_Pattern2(uint32_t*,uint32_t, uint32_t);
// void esdhc_set_dma_addr(uint32_t);
// void CMD24_Write_Block2(uint32_t);
// void CMD12_STOP_TRANS(void);
// void CMD17_Read_Block2(uint32_t card_addr);
// uint32_t Check_Data(uint32_t*,uint32_t*,uint32_t);
// uint32_t Rd_Response48(void);
// int esdhc_get_cihb(void);
// int esdhc_get_cdihb(void);
// void Rd_Response128(void);
// void wait_wdata_tc(void);
// int label_card_type(void);
// void CMD0_GO_IDLE_STATE(void);
// uint32_t CMD8_SEND_IFCOND0(void);
// void CMD55_APP_CMD (uint32_t);
// uint8_t sd_card_init(void);
// uint32_t ACMD41_SEND_OPCON(uint32_t);
// int esdhc_get_cardbusy(void);
// void CMD2_ALL_SEND_CID(void);
// uint32_t CMD3_SD_GET_RCA();
// void CMD9_SEND_CSD(uint32_t);
// uint32_t CMD13_SEND_STATUS(uint32_t);
// void CMD7_CARD_SELECT(uint32_t);
// void ACMD6_SWITCH(uint32_t, uint8_t);
// void ACMD13_SD_STATUS(uint32_t, uint8_t);
// uint8_t sd_card_write_circ_data(uint32_t , uint32_t );
// uint8_t sd_card_write_static_data(uint32_t, uint32_t);
// uint8_t sd_card_rw(void);
// uint8_t sd_card_log(uint8_t,SDHC_LOGGING_CMDS,uint8_t*);
// uint8_t log2sdhc(void);

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
