#ifndef ESDCH_DEF_H
#define ESDCH_DEF_H

//CMD6 and EXT_CDS related definition
#define ACCESS_CMD_SET 				(0x0<<24)
#define ACCESS_SET_BIT					(0x1<<24)
#define ACCESS_CLEAR_BIT				(0x2<<24)
#define ACCESS_WRITE					(0x3<<24)

#define HC_ERASE_GRP_SIZE_EN			(224<<16)
#define HC_ERASE_GRP_SIZE(x)			((x)<<8)

#define HC_ERASE_TIMEOUT_MULT_EN		(223<<16)
#define HC_ERASE_TIMEOUT_MULT(x)		((x)<<8)

#define HS_TIMING 						(185<<16)
#define HS_YES							(1<<8)

#define BUS_WIDTH						(183<<16)
#define BUS_1BIT_MODE					(0<<8)
#define BUS_4BIT_MODE					(1<<8)
#define BUS_8BIT_MODE					(2<<8)

#define BOOT_CONFIG      					(179<<16)
#define BOOT_ACK_ON					(1<<6)
#define BOOT_PARTITION_ENABLE_OFF 	(0<<3)
#define BOOT_PARTITION1_ENABLE 		(1<<3)
#define BOOT_PARTITION2_ENABLE 		(2<<3)
#define BOOT_UAREA_ENABLE				(7<<3)
#define BOOT_PARTITION_ACCESS_OFF 	(0<<0)
#define BOOT_PARTITION1_ACCESS		1
#define BOOT_PARTITION2_ACCESS		2

#define BOOT_BUS_WIDTH       				(177<<16)
#define BOOT_BUS_WIDTH_1BIT 			0
#define BOOT_BUS_WIDTH_4BIT 			1
#define BOOT_BUS_WIDTH_8BIT 			2

#define ERASE_GROUP_DEF				(175<<16)
#define ERASE_GROUP_DEF_EN 			(1<<8)

//CMD16 related definition
#define BLOCK_LEN_512 0x200

// SD Card CMD8 Related definition
#define CHECK_PATTEN					0xAA
#define VHS								(1<<8)  //  2.7-3.6V 

#define KRAM_BASE_ADDR		0x1FFF2000

#define GPIO_PIN_FUN1 		(0x01<<8)
#define GPIO_PIN_FUN2 		(0x02<<8)
#define GPIO_PIN_FUN3 		(0x03<<8)
#define GPIO_PIN_FUN4 		(0x04<<8)

#define GPIO_PIN_PD			0x00
#define GPIO_PIN_PU			0x01
#define GPIO_PIN_PE			0x02
#define GPIO_PIN_SRE_FTST	(0x00<<2)
#define GPIO_PIN_SRE_FAST	(0x01<<2)
#define GPIO_PIN_SRE_SLOW	(0x02<<2)
#define GPIO_PIN_SRE_SLST	(0x03<<2)
#define GPIO_PIN_DSE_FTST	(0x00<<6)
#define GPIO_PIN_DSE_FAST	(0x01<<6)
#define GPIO_PIN_DSE_SLOW	(0x06<<6)
#define GPIO_PIN_DSE_SLST	(0x03<<6)

#define DATA_TYPE1 0x01
#define DATA_TYPE2 0x02

#define CARD_REGISTRY_ON		0x01
#define CARD_REGISTRY_OFF		0x00

#define SD_BUSY 				0x80000000


#define MMC_SET_R0		SDHC_XFERTYP_RSPTYP(0)  	// No Response
#define MMC_SET_R1		SDHC_XFERTYP_RSPTYP(2)  
#define MMC_SET_R1b	SDHC_XFERTYP_RSPTYP(3) 	// normal response command	;Response Length 48, check Busy after response
#define MMC_SET_R2		SDHC_XFERTYP_RSPTYP(1) 	// CID, CSD register		;Response Length 48
#define MMC_SET_R3		SDHC_XFERTYP_RSPTYP(2)  	// OCR register			;Response Length 136
#define MMC_SET_R4		SDHC_XFERTYP_RSPTYP(2) 	// OCR register			;Response Length 136
#define MMC_SET_R5		SDHC_XFERTYP_RSPTYP(2)   	// normal response command	; Response Length 48
#define MMC_SET_R5b	SDHC_XFERTYP_RSPTYP(3)   	// normal response command	;Response Length 48, check Busy after response
#define MMC_SET_R6		SDHC_XFERTYP_RSPTYP(2)   	// normal response command	; Response Length 48


#define MMC_CMD_R0		SDHC_CMDRSP0
#define MMC_CMD_R1		SDHC_CMDRSP0
#define MMC_CMD_R1b	SDHC_CMDRSP0
//define MMC_CMD_R2	 
#define MMC_CMD_R3		SDHC_CMDRSP0
#define MMC_CMD_R4		SDHC_CMDRSP0
#define MMC_CMD_R5		SDHC_CMDRSP0
#define MMC_CMD_R5b	SDHC_CMDRSP0
#define MMC_CMD_R6		SDHC_CMDRSP0

#define MMC_CMD0 		SDHC_XFERTYP_CMDINX(0)
#define MMC_CMD1		(SDHC_XFERTYP_CMDINX(1)  	|MMC_SET_R3)
#define MMC_CMD2		(SDHC_XFERTYP_CMDINX(2)  	|MMC_SET_R2)
#define MMC_CMD3		(SDHC_XFERTYP_CMDINX(3)  	|MMC_SET_R1)
#define MMC_CMD4		(SDHC_XFERTYP_CMDINX(4)  	|MMC_SET_R0)
#define MMC_CMD5		(SDHC_XFERTYP_CMDINX(5)  	|MMC_SET_R1b)
#define MMC_CMD6		(SDHC_XFERTYP_CMDINX(6)  	|MMC_SET_R1b)
#define MMC_CMD7		(SDHC_XFERTYP_CMDINX(7)  	|MMC_SET_R1b)
#define MMC_CMD8		(SDHC_XFERTYP_CMDINX(8)  	|MMC_SET_R1)
#define MMC_CMD9		(SDHC_XFERTYP_CMDINX(9)  	|MMC_SET_R2)
#define MMC_CMD10		(SDHC_XFERTYP_CMDINX(10)	|MMC_SET_R2)
#define MMC_CMD11		(SDHC_XFERTYP_CMDINX(11)	|MMC_SET_R1)
#define MMC_CMD12		(SDHC_XFERTYP_CMDINX(12)	|MMC_SET_R1b)
#define MMC_CMD13		(SDHC_XFERTYP_CMDINX(13)	|MMC_SET_R1)
#define MMC_CMD14		(SDHC_XFERTYP_CMDINX(14)	|MMC_SET_R1)
#define MMC_CMD15		(SDHC_XFERTYP_CMDINX(15)	|MMC_SET_R0)
#define MMC_CMD16		(SDHC_XFERTYP_CMDINX(16)	|MMC_SET_R1)
#define MMC_CMD17		(SDHC_XFERTYP_CMDINX(17)	|MMC_SET_R1)
#define MMC_CMD18		(SDHC_XFERTYP_CMDINX(18)	|MMC_SET_R1)
#define MMC_CMD19		(SDHC_XFERTYP_CMDINX(19)	|MMC_SET_R1)
#define MMC_CMD20		(SDHC_XFERTYP_CMDINX(20)  	|MMC_SET_R1)
#define MMC_CMD23		(SDHC_XFERTYP_CMDINX(23)  	|MMC_SET_R1)
#define MMC_CMD24		(SDHC_XFERTYP_CMDINX(24)	|MMC_SET_R1)
#define MMC_CMD25		(SDHC_XFERTYP_CMDINX(25)  	|MMC_SET_R1)
#define MMC_CMD26		(SDHC_XFERTYP_CMDINX(26)  	|MMC_SET_R1)
#define MMC_CMD27		(SDHC_XFERTYP_CMDINX(27)  	|MMC_SET_R1)
#define MMC_CMD28		(SDHC_XFERTYP_CMDINX(28)  	|MMC_SET_R1b)
#define MMC_CMD29		(SDHC_XFERTYP_CMDINX(29)  	|MMC_SET_R1b)
#define MMC_CMD30		(SDHC_XFERTYP_CMDINX(30)  	|MMC_SET_R1)
#define MMC_CMD35		(SDHC_XFERTYP_CMDINX(35)  	|MMC_SET_R1)
#define MMC_CMD36		(SDHC_XFERTYP_CMDINX(36)  	|MMC_SET_R1)
#define MMC_CMD38		(SDHC_XFERTYP_CMDINX(38)  	|MMC_SET_R1)
//#define MMC_CMD39		(SDHC_XFERTYP_CMDINX(39)  	|MMC_SET_R1)
//#define MMC_CMD40		(SDHC_XFERTYP_CMDINX(40)  	|MMC_SET_R1)

#define SD_CMD8 		(SDHC_XFERTYP_CMDINX(8)  	|MMC_SET_R1)
#define SD_CMD55 		(SDHC_XFERTYP_CMDINX(55)  	|MMC_SET_R1)
#define SD_ACMD6 		(SDHC_XFERTYP_CMDINX(6)  	|MMC_SET_R1)
#define SD_ACMD13 		(SDHC_XFERTYP_CMDINX(13)  	|MMC_SET_R1)
#define SD_ACMD22 		(SDHC_XFERTYP_CMDINX(22)  	|MMC_SET_R1)
#define SD_ACMD41		(SDHC_XFERTYP_CMDINX(41)  	|MMC_SET_R3)
#define SD_ACMD51		(SDHC_XFERTYP_CMDINX(51)  	|MMC_SET_R1)

#define SDIO_CMD5		(SDHC_XFERTYP_CMDINX(5)  	|MMC_SET_R1)
#define SDIO_CMD52		(SDHC_XFERTYP_CMDINX(52)  	|MMC_SET_R1)


#define IRQ_ERROR_MASK 		0xffff0000
#define DMA_TCD_END		0xffff

// ESDHC Clock Divder Setting
#define ESDHC_INIT_CLK_PRE 	0x80
#define ESDHC_INIT_CLK_DIV 	0x3

#define ESDHC_INIT_CLK_PRE1 	0x80
#define ESDHC_INIT_CLK_DIV1 	0x5

//12MHZ; 96/8=12MHz
#define ESDHC_CLK_PRE 		0x2
#define ESDHC_CLK_DIV 		0x3

//24MHZ; 96/4=24MHz
#define ESDHC_CLK_PRE1 		0x2
#define ESDHC_CLK_DIV1 		0x0

//0.5MHZ; 96/192 =0.5 MHz
#define ESDHC_CLK_PRE2 		0x10
#define ESDHC_CLK_DIV2 		0x5


//12MHZ; 96/16=6MHz
#define ESDHC_CLK_PRE3 		0x8
#define ESDHC_CLK_DIV3 		0x0

//96/32=3MHz
#define ESDHC_CLK_PRE4 		0x8
#define ESDHC_CLK_DIV4 		0x1

//25MHZ; 25MHZ: Bypass
#define ESDHC_CLK_PRE5 		0x0
#define ESDHC_CLK_DIV5 		0x0

#define ESDHC_RDTO			0xE


#define ESDHC_RCA 			(0x0<<16)
#define ESDHC_RCA0 			(0x03<<16)
#define IRQ_ERROR_MASK 		0xffff0000

// ESDHC Block Size and Count
#define ESDHC_BLKCNT 		0x05
#define ESDHC_BLKSIZE		0x200

#define WML_BRST_LEN0		0x4
#define WML_BRST_LEN1		0x8
#define WML_BRST_LEN2		0x10
#define WML_BRST_LEN4		0x0

#define WML_WML			0x04
#define WML_WML0			0x10
#define WML_WML1			0x20
#define WML_WML2			0x40
#define WML_WML3			0x01
#define WML_WML4			0x80

#define NO_INTERNAL_DMA 	0x0
#define SIMPLE_DMA 			0x0
#define ADMA1				0x01
#define ADMA2				0x02

#define DTW_1BIT			0x00
#define DTW_4BIT			0x01
#define DTW_8BIT			0x02

#define SD_1BIT				0x00
#define SD_4BIT				0x02

#define WR_BURST_LEN		40
#define RD_BURST_LEN		56
#define WML_WR				(WR_BURST_LEN/4)
#define WML_RD 				(RD_BURST_LEN/4)

#define BIT0			0x1
#define BIT1			0x2
#define BIT2			0x4
#define BIT3			0x8
#define BIT4			0x10		
#define BIT5			0x20
#define BIT6			0x40		
#define BIT7			0x80		
#define BIT8			0x100		
#define BIT9			0x200		
#define BIT10		0x400		
#define BIT11		0x800
#define BIT12		0x1000
#define BIT13		0x2000
#define BIT14		0x4000
#define BIT15		0x8000
#define BIT16		0x10000
#define BIT17		0x20000
#define BIT18		0x40000
#define BIT19		0x80000
#define BIT20		0x100000
#define BIT21		0x200000
#define BIT22		0x400000
#define BIT23		0x800000
#define BIT24		0x1000000
#define BIT25		0x2000000
#define BIT26		0x4000000
#define BIT27		0x8000000
#define BIT28		0x10000000
#define BIT29		0x20000000
#define BIT30		0x40000000
#define BIT31		0x80000000

 #define ESDHCV2_DSADDR_MASK			0xFFFFFFFC
 #define ESDHCV2_BLKATTR_MASK			0xFFFF0FFF
 #define ESDHCV2_CMDARG_MASK			0xFFFFFFFF
 #define ESDHCV2_XFERTYP_MASK			0x3FFB0037
 #define ESDHCV2_CMDRSP0_MASK			0x00000000
 #define ESDHCV2_CMDRSP1_MASK			0x00000000
 #define ESDHCV2_CMDRSP2_MASK			0x00000000
 #define ESDHCV2_CMDRSP3_MASK			0x00000000
 #define ESDHCV2_DATPORT_MASK			0x00000000
 #define ESDHCV2_PRSSTAT_MASK			0x00000000
 #define ESDHCV2_PROCTL_MASK			0x070D03FF
 #define ESDHCV2_SYSCTL_MASK			0x000FFFFF
 #define ESDHCV2_IRQSTAT_MASK			0x00000000
 #define ESDHCV2_IRQSTATEN_MASK 		0x117F01FF
 #define ESDHCV2_IRQSIGEN_MASK		0x117F01BF
 #define ESDHCV2_AUTOC12ERR_MASK		0x00000000
 #define ESDHCV2_HOSTCAPBLT_MASK		0x00000000
 #define ESDHCV2_WML_MASK				0x017F017F
 #define ESDHCV2_FEVT_MASK				0x00000000
 #define ESDHCV2_ADMAES_MASK			0x00000000
 #define ESDHCV2_ADSADDR_MASK			0xFFFFFFFC
 #define ESDHCV2_VENDOR_MASK			0x0F000003
 #define ESDHCV2_MMCBOOT_MASK			0x0F000003
 #define ESDHCV2_HOSTVER_MASK			0x00000000

 
extern int 		ESDHC_ERR_NUM;
extern volatile int 	ESDHC_INT;
volatile int 		card_registry;   //
volatile int 		card_type; 
volatile int 		HCS; 

#define MMC_CARD 	0x01
#define SD_CARD 		0x02
#define SDIO_CARD 	0x03

/********************************************************************/
//--------------------------------------
// Define the field which store Card register Value
//--------------------------------------
#define SRAM_L 			0x1FFFFFFF
#define Store_Card_Reg	0x1fffff00


	uint32_t		ResponseNormal;
	uint32_t		ResponseCMD12;
	uint32_t		Response128HH;
	uint32_t		Response128HL;
	uint32_t		Response128LH;
	uint32_t		Response128LL;

	uint32_t   	EPCsave, EPSRsave; 	
	uint32_t   	_gIRQ_status;
	uint32_t   	_gIRQ_status_DTD;
	uint32_t   	_gIRQ_status_AOD;
	uint32_t   	_gIRQ_status_count;			
	uint32_t 		_gDMA_status;
	uint32_t		_gStatus_data;
	uint32_t		_gResp_Time_out;
	uint32_t		_gRead_time_out;
	uint32_t		_gResponseCRCerror;
	uint32_t		_gWriteCRCerror;
	uint32_t		_gReadCRCerror;
	uint32_t		_gMMC_registrated;
	uint32_t		_gSD_registrated;
	uint32_t		_gSDIO_registrated;			
	uint32_t		_gMmc_size_1;
	uint32_t        	_gMmc_size_2;
	uint32_t		_gSd_size;
	uint32_t		_gSdio_size;

	uint32_t		SD_rca;
	uint32_t		MMC_rca;
	uint32_t		SDIO_rca;
	uint8_t 		No_mmccard;
	uint8_t 		No_sdcard;
	uint8_t		No_sdiocard;		

//--------------------------------------
// Card(MMC & SD) Card Status Register Related Definition
//--------------------------------------
#define Idle		0x00
#define Ready	0x01
#define Ident		0x02	
#define Stby		0x03
#define Tran		0x04
#define Data		0x05
#define Rcv		0x06
#define Prg		0x07
#define Dis		0x08
#define Btst		0x09
#define Slp		0x0a

	uint8_t		Add_OutOfRange;
	uint8_t		Add_Misalign;
	uint8_t		Blk_Len_Err;
	uint8_t		Erase_Seq_Err;
	uint8_t		Erase_Param;
	uint8_t		Wp_Violation;
	uint8_t		Card_Is_Locked;
	uint8_t		Lock_Unlock_Failed;
	uint8_t		Com_CRC_Err;
	uint8_t		Illegal_Cmd;
	uint8_t		Card_Ecc_Fail;
	uint8_t		CC_Err;
	uint8_t		Err;
	uint8_t		UndErrun;
	uint8_t		OverRun;
	uint8_t		CidCsd_Overwrite;
	uint8_t		WP_Erase_Skip;
	uint8_t		Erase_Reset;
	uint8_t		Current_State;
	uint8_t		Ready_For_Data;
	uint8_t		Switch_Err;
	uint8_t		App_Cmd;

//--------------------------------------
// SD Status Register Related Definition
//--------------------------------------
	uint8_t		sdDat_Bus_Width;
	uint8_t		sdSecured_Mode ;
	uint8_t		sdSd_Card_Type;
	uint8_t		sdSize_Pro_EdArea; 
	uint8_t		sdSpeed_Class;
	uint8_t		sdPreFormance_Move;
	uint8_t		sdAu_Size;
	uint8_t		sdErase_Size;
	uint8_t		sdErase_Timeout;
	uint8_t		sdErase_Offset;
		
//--------------------------------------
// MMC CSD Related Definition
//--------------------------------------
	uint8_t		CSD_Structure;
	uint8_t		SpecVer;
	uint8_t		TAAC;
	uint8_t		NSAC;
	uint8_t		TranSpeed;
	uint16_t		CCC;
	uint8_t		ReadBlLen;
	uint8_t		ReadBlPartial;
	uint8_t		WriteBlockMisalign;
	uint8_t		ReadBlockMisalign;
	uint8_t		DsrImp;
	uint16_t		CSize;
	uint8_t		VddRdCurrMin;
	uint8_t		VddRdCurrMax;
	uint8_t		VddWrCurrMin;
	uint8_t		VddWrCurrMax;
	uint8_t		CSizeMult;
	uint8_t		EraseGrpSize;
	uint8_t		EraseGrpMult;
	uint8_t		WpGrpSize;
	uint8_t		WpGrpEnable;
	uint8_t		DefaultECC;
	uint8_t		R2WFactor;
	uint8_t		WrBlLen;
	uint8_t		WriteBlPartial;
	uint8_t		ContentProtApp;
	uint8_t		FileFormatGrp;
	uint8_t		Copy;
	uint8_t		PermWriteProtect;
	uint8_t  		TempWriteProtect;
	uint8_t		FileFormat;
	uint8_t		ECC;
	uint8_t 		CRC;	

	// MMC EXT_CSD Related Definition
	uint8_t		mmcSCMD_SET;
	uint8_t		mmcBOOT_INFO;
	uint8_t		mmcBOOT_SIZE_MULTI;
	uint8_t		mmcACC_SIZE;
	uint8_t		mmcHC_ERASE_GRP_SIZE;
	uint8_t		mmcERASE_TIMEOUT_MULT;
	uint8_t		mmcREL_WR_SEC_C;
	uint8_t		mmcHC_WP_GRP_SIZE;
	uint8_t		mmcS_C_VCC;
	uint8_t		mmcS_C_VCCQ;
	uint8_t		mmcS_A_TIMEOUT;
	uint32_t		mmcSEC_COUNT;
	uint8_t		mmcMIN_PREF_W_8_52;
	uint8_t		mmcMIN_PREF_R_8_52;
	uint8_t		mmcMIN_PREF_W_8_26_4_52;
	uint8_t		mmcMIN_PREF_R_8_26_4_52;
	uint8_t		mmcMIN_PREF_W_4_26;
	uint8_t		mmcMIN_PREF_R_4_26;
	uint8_t		mmcPWR_CL_26_360;
	uint8_t		mmcPWR_CL_52_360;
	uint8_t		mmcPWR_CL_26_195;
	uint8_t		mmcPWR_CL_52_195;
	uint8_t		mmcCARD_TYPE;
	uint8_t		mmcCSD_STRUCTURE;
	uint8_t		mmcEXT_CSD_REV;
	uint8_t		mmcCMD_SET;
	uint8_t		mmcCMD_SET_REV;
	uint8_t		mmcPOWER_CLASS;
	uint8_t		mmcHS_TIMING;
	uint8_t		mmcBUS_WIDTH;
	uint8_t		mmcERASED_MEM_COUNT;
	uint8_t		mmcBOOT_CONFIG;
	uint8_t		mmcBOOT_BUS_WIDTH;
	uint8_t		mmcERASE_GROUP_DEF;
/********************************************************************/
#endif
