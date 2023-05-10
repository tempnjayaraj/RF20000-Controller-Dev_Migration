/******************************************************************************/
//                   ArthroCare Proprietary and Confidential
//                   (c) ArthroCare Corporation 2012
//                           All Rights Reserved
/******************************************************************************/
/*******************************************************************************

file:              esdhc.c
Author:            Chris Wallis
Date:              05-APR-2013
Modified:          
CPU:               Freescale Kinetis P/N MK60FN1M0VLQ15 (144-Pin), REVID ?
Compiler:          IAR ANSI C/C++ Compiler for ARM 6.51
*******************************************************************************/

/*******************************************************************************
Comments:
Programmable interrupt timer(s).

NOTE: N/A

*******************************************************************************/

#include "MK60F15.h"
#include "esdhc.h"
#include "esdhc_def.h"
//#include "system.h"
#include "cpu.h"
#include "ftfe.h"
#include "system.h"

/******************************************************************************
* Function: ESDHC_Init
*
* Purpose: Initialize .
******************************************************************************/

//__root uint16_t sound_file[341];
//__root const uint8_t foo @ 0x30000 = 'A';
//__root const uint8_t boo[2] @ 0x30FFF = {'B','C'};

static uint8_t eeram[4096] @ 0x14000000;

uint8_t esdhc_init(void){
  
//  uint16_t		blk_len	=0;
//  uint16_t		blk_cnt 	=0;
//  uint32_t		blk_size	=0;
//  uint32_t		card_addr = 0x0;
//
//  uint32_t 		loop_cnt =4096;
//  uint32_t 		loop_div =512;
//  uint32_t 		count;
//  uint32_t 		no_of_error;							//No. of mismatch during verification
//  uint32_t		mass_size;
//  
//  uint32_t flash_idx = 0x30000;
//  uint32_t app_size = 0;
//  uint8_t eeram_idx = 0;
//  uint32_t app_starting_sector = 0;
//  uint32_t app_ending_sector = 0;
//    
//  uint8_t card_detected = 0;
//  
//  int 		card_type; 
//  
//  ropi_rwpi_header_layout *p_ropi_rwpi_header;  
//  
//  uint8_t app_present;
//  
//  uint32_t partition_size = 0, 
//           starting_sector = 0,
//           ending_sector = 0,
//           x = 0;
//  
//  const uint8_t header_signature[] = "RF20000P";
//  uint8_t              Mmc2SdramAddr[512];
//     
//  blk_cnt = 1;
//  blk_len = 0x200; // 512 Byte	
//  blk_size= blk_cnt * blk_len;	// byte base
//  
///* Enable gate clock */
//SIM_SCGC3 |= SIM_SCGC3_ESDHC_MASK;
//
//MPU_CESR &= 0xFFFFFFFE; 
//
//
////PORTE_PCR4 = PORT_PCR_PE_MASK | PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; /* SDHC0_DAT3 */
//PORTE_PCR4 = PORT_PCR_MUX(4);// | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK; /* SDHC0_DAT3 */
//PORTE_PCR0 = PORT_PCR_MUX(4) | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PS_MASK; /* SDHC0_DAT3 */
//PORTE_PCR1 = PORT_PCR_MUX(4)| PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK |  PORT_PCR_PS_MASK; /* SDHC0_DAT3 */
//PORTE_PCR2 = PORT_PCR_MUX(4)| PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PS_MASK; /* SDHC0_DAT3 */
//PORTE_PCR3 = PORT_PCR_MUX(4)| PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PS_MASK; /* SDHC0_DAT3 */
//PORTE_PCR5 = PORT_PCR_MUX(4)| PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PS_MASK; /* SDHC0_DAT3 */
//
///* Soft reset */
//SDHC_SYSCTL |= SDHC_SYSCTL_RSTA_MASK;
//
//while((SDHC_SYSCTL&SDHC_SYSCTL_RSTA_MASK)!=0); // wait sdhc to clear this bit automatically..
//
//SDHC_SYSCTL = (SDHC_SYSCTL_SDCLKFS(8) | SDHC_SYSCTL_DVS(0));
//SDHC_SYSCTL  |= SDHC_SYSCTL_DTOCV(0xE);
//SDHC_SYSCTL  |= SDHC_SYSCTL_SDCLKEN_MASK 
//                                |SDHC_SYSCTL_PEREN_MASK
//                                |SDHC_SYSCTL_HCKEN_MASK
//                                |SDHC_SYSCTL_IPGEN_MASK;
//
//while((SDHC_PRSSTAT& SDHC_PRSSTAT_SDSTB_MASK)!=SDHC_PRSSTAT_SDSTB_MASK); //waiting sd clock stable
//
//SDHC_IRQSTATEN = SDHC_IRQSTATEN_CINSEN_MASK|SDHC_IRQSTATEN_CRMSEN_MASK ; 
//
////Clear IRQSTAT
//SDHC_IRQSTAT=0xffffffff; 
//
//// Using DAT3 to detect Card insertion
////PORTE_PCR4 |= (1<<6)  ;	//	SDHC0_DAT3
//SDHC_PROCTL = SDHC_PROCTL_EMODE(2) |SDHC_PROCTL_D3CD_MASK;
//
////printf("Please insert Card!\n");
//
////Waiting inserting Card
////while((SDHC_IRQSTAT & SDHC_IRQSTAT_CINS_MASK) != (SDHC_IRQSTAT_CINS_MASK)){}
//if((SDHC_IRQSTAT & SDHC_IRQSTAT_CINS_MASK) == (SDHC_IRQSTAT_CINS_MASK))
//{
//card_detected = 1;
////printf("Card had been inserted!\n");
//
//SDHC_PROCTL &= ~SDHC_PROCTL_D3CD_MASK;
//PORTE_PCR4 = PORT_PCR_MUX(4) | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PS_MASK; /* SDHC0_DAT3 */
//SDHC_IRQSTATEN = ~(SDHC_IRQSTATEN_CINSEN_MASK|SDHC_IRQSTATEN_CRMSEN_MASK) ; 
//
///* Initialize card */
//card_type = label_card_type();
//
//if(card_type ==SD_CARD)
//{
//        Init_SD_Card(BIT_MODE);
//}
//
//
//
//SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1)| SDHC_BLKATTR_BLKSIZE(0x200);
//SDHC_PROCTL |= SDHC_PROCTL_DTW(DTW_4BIT);
//
//SDHC_WML = 	((WML_WML4) <<SDHC_WML_WRWML_SHIFT)
//		|((WML_WML4) <<SDHC_WML_RDWML_SHIFT) ; 
//
//
//
////for(count=520;count<520+2;count++)
////{
//    //Show checking progress on terminal 
////    if((count%512) ==0)
////    {
////      printf("\t\tchecking %4d to %4d bytes data...\n", (count/loop_div)*loop_div*blk_size, (count/loop_div +1)*loop_div*blk_size);
////    }
//    
//    //Clear_Memory((uint32_t*)&Mmc2SdramAddr,blk_size);
//    //Clear_Memory((uint32_t*)Sdram2MmcAddr,blk_size);
//    //Set_Memory_Pattern2((uint32_t*)Sdram2MmcAddr,blk_size,count);  //
//
//    //esdhc_set_dma_addr(Sdram2MmcAddr);	
//
//    //Write data to MMC Card .....		
//    //CMD24_Write_Block2(count*512);
//    memset(&Mmc2SdramAddr[0],0,512); 
//    esdhc_set_dma_addr((uint32_t)&Mmc2SdramAddr[0]);	
//    CMD17_Read_Block2(0);
//    
//    starting_sector = (
//     (Mmc2SdramAddr[473] << 24) |
//     (Mmc2SdramAddr[472] << 16) |
//     (Mmc2SdramAddr[471] << 8) |
//     (Mmc2SdramAddr[470])
//     );
//    
//    partition_size = (
//     (Mmc2SdramAddr[477] << 24) |
//     (Mmc2SdramAddr[476] << 16) |
//     (Mmc2SdramAddr[475] << 8) |
//     (Mmc2SdramAddr[474])
//     );
//    
//    ending_sector = starting_sector + partition_size;
//                     
////    SDHC_IRQSTATEN |= SDHC_IRQSTATEN_CCSEN_MASK;
////    // Clear Command Complete bit
////    SDHC_IRQSTAT = 0xFFFFFFFF;
////    SDHC_IRQSIGEN = SDHC_IRQSIGEN_CCIEN_MASK;
//    
//    
////Read data from MMC Card .....
//    for (x = starting_sector; x < ending_sector; x++)
//    {
//    memset(&Mmc2SdramAddr[0],0,512); 
//    esdhc_set_dma_addr((uint32_t)&Mmc2SdramAddr[0]);	
//    CMD17_Read_Block2(x);
//    
//    app_present = memcmp(&Mmc2SdramAddr[16],header_signature,8);
//    
//    if (app_present == 0)
//    {
//      p_ropi_rwpi_header = (ropi_rwpi_header_layout*)&Mmc2SdramAddr[0];
//      app_starting_sector = x;
//      app_size = (p_ropi_rwpi_header->ropi_bytes / 512) + 1;
//      app_ending_sector = app_starting_sector + app_size;
//      memcpy(&eeram[0],&Mmc2SdramAddr[0],512);
//      break;
//      
//    }
//     
//    //esdhc_set_dma_addr((uint32_t)&Mmc2SdramAddr[512]);	
//    //CMD17_Read_Block2(521*512);
//    
////    memcpy((uint8_t *)&current_therapy_settings->log.idx,&Mmc2SdramAddr[0],5);
////    
////    current_therapy_settings->log.idx &= 0x1FF;
////    
////    esdhc_set_dma_addr((uint32_t)&Mmc2SdramAddr[0]);	
////    CMD17_Read_Block2((521+current_therapy_settings->log.block)*512);
////    
////    //esdhc_set_dma_addr((uint32_t)&Mmc2SdramAddr[512]);	
////    //CMD17_Read_Block2(521*512);
////    
////    memcpy((uint8_t *)&current_therapy_settings->log.data[0],&Mmc2SdramAddr[0],512);
//    
//    }
//    
//    if (app_present == 0)
//    { 
//
//      for(x = 1; x < app_size; x++)
//      {
//      
//        if (x == 190)
//        {
//          asm("nop");
//        }
//          
//        eeram_idx = x % 8;
//        memset(&Mmc2SdramAddr[0],0,512); 
//        esdhc_set_dma_addr((uint32_t)&Mmc2SdramAddr[0]);	
//        CMD17_Read_Block2(x+app_starting_sector);
//        memcpy(&eeram[eeram_idx*512],&Mmc2SdramAddr[0],512);
//               
//        if (eeram_idx == 7)
//        {
//          
//          erase_ftfe_sector(flash_idx);
//          write_ftfe_section(flash_idx);
//          memset(&eeram[0],0xFF,4096); 
//          flash_idx += 4096;
//            
//        }
//            
//      }
//      
//      if (eeram_idx != 7)
//      {
//        
//        erase_ftfe_sector(flash_idx);
//        write_ftfe_section(flash_idx);
//                        
//      }
//      
//      
//      
//      
//    }
//    
//    SDHC_IRQSTATEN |= SDHC_IRQSTATEN_CCSEN_MASK;
//    // Clear Command Complete bit
//    SDHC_IRQSTAT = 0xFFFFFFFF;
//    SDHC_IRQSIGEN = SDHC_IRQSIGEN_CCIEN_MASK;
//    enable_irq(80);
//    //Enable_IRQ(17);
//    
//    //    
////    if (count == 520)
////    {
////    memcpy ((uint8_t *) &sound_file[0], &Mmc2SdramAddr[0], 512);
////    current_therapy_settings->audio.p_sound_file = (uint16_t*)&sound_file[0];
////    
////    }
////    else if (count == 521)
////    {
////    memcpy ((uint8_t *) (&sound_file[0]+256), &Mmc2SdramAddr[0], 170); 
////    }
////    //no_of_error=Check_Data((uint32_t*)Sdram2MmcAddr,(uint32_t*)Mmc2SdramAddr,blk_len);	
////    if(no_of_error>0) 	
////    {
////            printf("Mismatch %d found in access %d\r\n",no_of_error,count);
////            //block_error++;
////            //SDHC_error();
////    }	
//
//  //}
//
//}

return 0;


}

void Clear_Memory(uint32_t* addr,uint32_t Size)
{	
	uint32_t x;
	uint32_t * WriteMemory;
	
	WriteMemory=(uint32_t*)addr;
	
	for (x=0;x<Size/4;x++)
	{
		*WriteMemory = 0;
		WriteMemory++;
	}	

	WriteMemory=(uint32_t*)addr;
}

//************************************************
//************************************************
void Set_Memory_Pattern2(uint32_t* addr,uint32_t Size, uint32_t data_offset)
{	
	uint32_t x;
	uint32_t *WriteMemory;
			
	WriteMemory=(uint32_t*)addr;
	
	for(x=0; x<Size/4; x++)
	{
			*WriteMemory = (0x12345678 + x*0x00010001 +data_offset);
			//*WriteMemory = 0x00000000;
			//*WriteMemory = 0xaaaaaaaa;
			WriteMemory++;	
	}	
}

//************************************************
// Set internal DMA System Address
//
//************************************************
void esdhc_set_dma_addr(uint32_t dma_addr)
{
	while((SDHC_PRSSTAT&SDHC_PRSSTAT_DLA_MASK)!=0); //Waiting for DLA was cleared by sdhc
	if((SDHC_IRQSTAT & SDHC_IRQSTAT_TC_MASK)!=0)
		SDHC_IRQSTAT = SDHC_IRQSTAT_TC_MASK; // Clear TC before setting DMA Address
		
	SDHC_DSADDR = dma_addr;
}

//************************************************
//************************************************
void CMD24_Write_Block2(uint32_t card_addr)
{

//  while(esdhc_get_cihb() != 0);  // waiting command line is available	
//  while(esdhc_get_cdihb()!=0);  // waiting data line is available, it doesn't work for waitting card out of busy.
  //wait_card_inTran(card_addr); // wait card in trans state!

  // Clear Command Complete bit
  SDHC_IRQSTAT = 0xFFFFFFFF;

  //
  SDHC_CMDARG =  card_addr;
  SDHC_XFERTYP =  MMC_CMD24 
                                          |SDHC_XFERTYP_DPSEL_MASK
                                          |SDHC_XFERTYP_AC12EN_MASK
                                          |SDHC_XFERTYP_BCEN_MASK
                                  |SDHC_XFERTYP_DMAEN_MASK;

//  wait_wdata_tc();	//waiting write transfer complete

  if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) 
  {
  //	printf("Error: CMD24_Write_Block2 SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);
  CMD12_STOP_TRANS();
  }
  //Rd_Response48();
}

//************************************************
//************************************************
void CMD12_STOP_TRANS(void)
{
  
  
  // Clear Command Complete bit
  SDHC_IRQSTAT = 0xFFFFFFFF;

  //
  SDHC_CMDARG	=  0;
  SDHC_XFERTYP 	=  MMC_CMD12;
  #if (defined(DEBUG_INFO))	
  while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

  
          if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
                  printf("\tstop transmission done(12) \n");
          else 
                  printf("\tstop transmission(12) timeout \n");
  

  if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) printf("Error: CMD12_STOP_TRANS SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);
  #endif
  Rd_Response128();

}

//************************************************
//************************************************
void CMD17_Read_Block2(uint32_t card_addr)
{


        
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	while(esdhc_get_cdihb()!=0);   // waiting data line is available, it doesn't work for waitting card out of busy.
	//wait_card_inTran(card_addr); // wait card in trans state!
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	SDHC_CMDARG =  card_addr;
 	while (SDHC_PRSSTAT & SDHC_PRSSTAT_DLA_MASK) {};
	SDHC_XFERTYP =  	MMC_CMD17
						|SDHC_XFERTYP_DPSEL_MASK
						|SDHC_XFERTYP_DTDSEL_MASK
						|SDHC_XFERTYP_AC12EN_MASK
						|SDHC_XFERTYP_BCEN_MASK
						|SDHC_XFERTYP_DMAEN_MASK;
 	
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
 	
 	while((SDHC_PRSSTAT & SDHC_PRSSTAT_RTA_MASK));	//read transfer active
 	
        while(!(SDHC_IRQSTAT & SDHC_IRQSTAT_TC_MASK)); // transfer completed!
 	
 	#if (defined(DEBUG_INFO))
		printf("\tinterrupt state: %x \n",SDHC_IRQSTAT);
 		printf("\tpresent state: %x \n",SDHC_PRSSTAT);
 		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tread  blocks cmd done(17)\n");
 		else 
 			printf("\tread  blocks cmd timeout\n");
	#endif
	
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) 
	{
		//	printf("Error: CMD24_Write_Block2 SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);
		CMD12_STOP_TRANS();
	}		
	Rd_Response48();	
}


//Check the data of the card
//Parameter
//   uint32_t* sdram2mmcaddr = Reference data pointer 
//   uint32_t* mmc2sdramaddr = Target data pointer
//   size: byte number

//************************************************
//uint32_t Check_Data(uint32_t* sdram2mmcaddr,uint32_t* mmc2sdramaddr,uint32_t Size)
//{
//	uint32_t x;
//	uint32_t *Data_Out	=(uint32_t*)mmc2sdramaddr;
//	uint32_t *Write_Data	=(uint32_t*)sdram2mmcaddr;
//	uint32_t error		=0;
//	
//	//printf("mmc2sdramaddr =");Print32(mmc2sdramaddr);
//	//printf("sdram2mmcaddr =");Print32(sdram2mmcaddr);
//	
//	for(x=0;x<Size/4;x++)
//	{
//	
//		if (Data_Out[x]!=Write_Data[x])
//		{
//			////printf("0x%X = 0x%X\n",&Data_Out[x],Data_Out[x]);
//			//#if (defined(DEBUG_INFO))
//				printf("Sou %1d = %x \n", x, Write_Data[x]);
//				printf("Des %1d = %x \n", x, Data_Out[x]); 
//				printf("Error Address = %x \n", x*4);
//			//#endif
//			error++;
//		}
//	}
//	
//	return error;
//}

//************************************************
//obtain the 48 bits responses
//************************************************
uint32_t Rd_Response48(void)
{

  	uint32_t		ResponseNormal;
	
  //
	// bit 47:47 - 0,Start
	// bit 46:46 - 0,Tx bit
	// bit 45:40 - Command index
	// bit 39:08 - Card Status / OCR / [RCA,Card Status]
	// bit 07:01 - CRC7
	// bit 00:00 - 1,Stop
	//
	// 47.........32 31..24 23..21  20..17       16 15.14   13      12     11   10.8 7....0
	// <....> <....> <....> <.....  ........     .> <...     .       .      .   ...> <....>
	// < CMD> <   Error   > <Erase> <State> <Ready> <Rsv> <AppCMD> <Rsv> <Err> <Rsv> <CRC7>

	ResponseNormal= SDHC_CMDRSP0;
        #if (defined(DEBUG_INFO))
        printf("\t\tResponse48 = %x \n",ResponseNormal); 
        #endif
	
	return ResponseNormal;

}


//************************************************
// Command Line is inhibit or not
//		CIHB Low:  Cannot issue command
//		CIHIB High:  Can issue command using only CMD line
//************************************************
int esdhc_get_cihb(void)
{
	if((SDHC_PRSSTAT & SDHC_PRSSTAT_CIHB_MASK)!=0)
		return(1);
	else 
		return(0);
}

//************************************************
// DATA line was inhibit or not
// 		cdihb Low: Can issue command which uses the DAT line
// 		cdihb High: Cannot issue command which uses the DAT line
//************************************************
int esdhc_get_cdihb(void)
{
	if((SDHC_PRSSTAT &  SDHC_PRSSTAT_CDIHB_MASK)!=0)
		return(1);
	else 
		return(0);
}


//************************************************
//obtain the 128 bits responses
//************************************************
void Rd_Response128(void)
{
	//
	// bit 135:135 - 0,Start
	// bit 134:134 - 0,Tx bit
	// bit 133:128 - 111111,Rsv
	// bit 127:001 - CID/CSD incl. CRC7
	// bit 000:000 - 1,Stop
	//

	uint32_t		Response128HH;
	uint32_t		Response128HL;
	uint32_t		Response128LH;
	uint32_t		Response128LL;
	Response128HH = SDHC_CMDRSP3;
	Response128HL = SDHC_CMDRSP2;
	Response128LH = SDHC_CMDRSP1;
	Response128LL = SDHC_CMDRSP0;

   	#if (defined(DEBUG_INFO))
  		printf("\t\tResponse128HH = %x\n ",Response128HH); 
		printf("\t\tResponse128HL = %x\n ",Response128HL); 
		printf("\t\tResponse128LH = %x\n ",Response128LH); 
		printf("\t\tResponse128LL = %x\n ",Response128LL); 
	#endif
	
	////printf("Response128: %x %x %x %x \n",Response128HH,Response128HL,Response128LH,Response128LL);
}

void wait_wdata_tc(void)
{
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
 	//while((SDHC_PRSSTAT & SDHC_PRSSTAT_WTA_MASK));	//Write transfer active
 	while(!(SDHC_IRQSTAT & SDHC_IRQSTAT_TC_MASK));
}

//************************************************
// Lable the Card Type 
//************************************************
int label_card_type(void)
{
    	int HCS =0;
		
    	// sending Go_idle_cmd
	CMD0_GO_IDLE_STATE();
	
	/****************/
	//Label MMC Card
	/****************/
   	#if (defined(DEBUG_INFO))	
		printf("\tMMC SEND_OP_COND (1) \n");
	#endif
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

	//
    	SDHC_CMDARG 	=  0x00ff8000;
 	SDHC_XFERTYP 	=  MMC_CMD1;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
 	if((SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)==0x0)
	{
		//printf("\tMMC Card is accepted\n");
		return(MMC_CARD);
	}


	/****************/
	//Label SD Card
	/****************/
    	#if (defined(DEBUG_INFO))
		printf("\tSD send SEND_IF_COND (8)");
	#endif
	
	if(CMD8_SEND_IFCOND0()==0)
		HCS=0;
	else 
		HCS =1;


	CMD55_APP_CMD(0x0);
	
   	#if (defined(DEBUG_INFO))
		printf("\n\tSD_SEND_OPCON(A41)");
	#endif
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	//sending send_rca
 	SDHC_CMDARG =  (HCS<<30);
 	SDHC_XFERTYP =  SD_ACMD41;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
 	if((SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)==0x0)
	{
		//printf("\tSD Card is accepted\n");
		return(SD_CARD);
	}


	/****************/
	//Label SDIO Card
	/****************/
	#if (defined(DEBUG_INFO))
		printf("\n\tSent IO_SEND_OP_COND(5)");
	#endif

	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	//sending send_rca
 	SDHC_CMDARG =  0x0;
 	SDHC_XFERTYP =  SDIO_CMD5;

	#if (defined(DEBUG_INFO))
	
        while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));	
 	if((SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)==0x0)
	{
		printf("\n\tSDIO Card is accepted\n");
		return(SDIO_CARD);
	}
        
        #endif

}

//************************************************
// MMC Command
//************************************************
void CMD0_GO_IDLE_STATE(void)
{
   	#if (defined(DEBUG_INFO))
		printf("\n\tGo_idle_cmd Start(0)\n");
	#endif

	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;
	
	// sending Go_idle_cmd
    	SDHC_CMDARG =  0x0;
	SDHC_XFERTYP =  MMC_CMD0;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

   	#if (defined(DEBUG_INFO))
    		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tGo_idle_cmd done (0) \n");
 		else
			printf("\tGo_idle_cmd timeout \n");
	
	
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) printf("Error: CMD0_GO_IDLE_STATE SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);

        #endif
}

//************************************************
// To confirm SD card version
//************************************************
uint32_t CMD8_SEND_IFCOND0(void)
{
   	#if (defined(DEBUG_INFO))
		printf("\n\tSEND_IF_COND (8)");
	#endif

	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	//sending send_rca
 	SDHC_CMDARG 	=  CHECK_PATTEN | VHS;
 	SDHC_XFERTYP 	=  SD_CMD8;
 	while (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK));
 	
   	#if (defined(DEBUG_INFO))
		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tSEND_IF_COND (8)\n");
 		else 
			printf("\tSEND_IF_COND (8) timeout \n");	
	

	if((SDHC_IRQSTAT&SDHC_IRQSTAT_CTOE_MASK)== SDHC_IRQSTAT_CTOE_MASK)
		printf("\tVer1.X SD Memory Card or not SD Card\n");
	else
		printf("\tVer2.00 or later SD Memory Card\n");

        #endif
    	return Rd_Response48();

}

//************************************************
//************************************************
void CMD55_APP_CMD (uint32_t card_rca)
{
   	#if (defined(DEBUG_INFO))
		printf("\n\tAPP CMD (55)");
	#endif

	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	//sending send_rca
 	SDHC_CMDARG 	=  card_rca;
 	SDHC_XFERTYP 	=  SD_CMD55;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
 	
   	#if (defined(DEBUG_INFO))
		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tAPP CMD (55) \n");
 		else 
			printf("\tAPP CMD (55) timeout \n");
	
	
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) printf("Error: CMD55_APP_CMD SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);

        #endif
        
    	//Rd_Response48();

}


//************************************************
//************************************************
void Init_SD_Card(uint8_t dat_width)
{

	int card_num	 	= 0;
        uint32_t base_rca=0, SD_OCR = 0;
	card_registry	= CARD_REGISTRY_ON;
	#if (defined(DEBUG_INFO))
	printf("\tStart to initialize SD Card.... \n");
	#endif
    	// sending Go_idle_cmd
	//CMD0_GO_IDLE_STATE();
	        
       SD_OCR= ACMD41_SEND_OPCON((base_rca|HCS<<30));
        
	while(1)
	{
		if((ACMD41_SEND_OPCON((SD_OCR|HCS<<30))& SD_BUSY)!=0x0) //SD stay at busy state when =0x0
			break;
	}


	// Card Registry (Don't support on Palladium MMC model)
	while(card_registry == CARD_REGISTRY_ON)
	{
		//all send CID 
		CMD2_ALL_SEND_CID();

		if((SDHC_IRQSTAT& SDHC_IRQSTAT_CTOE_MASK) == SDHC_IRQSTAT_CTOE_MASK)
		{
			card_registry = CARD_REGISTRY_OFF;
			
                        #if (defined(DEBUG_INFO))
			printf("\tThere are %1d Cards on the SD bus!\n\n",card_num+1);
			printf("\tCard Registry is Done!\n");
                        #endif
		}
		else
		{
			SD_rca = (CMD3_SD_GET_RCA()&0xFFFF0000);
			#if (defined(DEBUG_INFO))
                        printf("\t\tCard RCA =%x \n",SD_rca); 
                        #endif
			
			// Check CSD & CID
			CMD9_SEND_CSD(SD_rca);

			//card_num ++;
		}
	}
	
	base_rca = SD_rca;
	
	//send card status register
	CMD13_SEND_STATUS(base_rca);
	#if (defined(DEBUG_INFO))
        if(Current_State == Stby || Ready_For_Data == Ready)
	{
		printf("\tSD card is in standby state now!\n");
	}
        #endif
	//card select
	CMD7_CARD_SELECT(base_rca);

	//Set bit mode
 	if (dat_width ==DTW_4BIT)
	{
		// Set 4bit Mode
		ACMD6_SWITCH(base_rca, DTW_4BIT);
	}
	else if(dat_width ==DTW_1BIT)
	{
		// Set 1bit Mode
		ACMD6_SWITCH(base_rca, DTW_1BIT);
		//printf("Default 1 bit bus width!!\n\n");
		//ACMD13_SD_STATUS(base_rca, DTW_1BIT);
	}
	else
		
        //printf("Error: Please set the data bus width!!\n\n");

	ACMD13_SD_STATUS(base_rca, dat_width);

	#if (defined(DEBUG_INFO))
        printf("\tSD card initialization done!\n\n");
        #endif

}


//************************************************
//************************************************
uint32_t ACMD41_SEND_OPCON(uint32_t args)
{
	CMD55_APP_CMD(0x0);
	
   	#if (defined(DEBUG_INFO))
		printf("\n\tSD_SEND_OPCON(A41)");
	#endif

	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	//sending send_rca
 	SDHC_CMDARG =  args;
 	SDHC_XFERTYP =  SD_ACMD41;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
 	
   	#if (defined(DEBUG_INFO))
		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tSD_SEND_OPCON(A41) Done \n");
 		else 
			printf("\tSD_SEND_OPCON(A41) Timeout \n");
	

	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) printf("Error: ACMD41_SEND_OPCON SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);
        #endif
    	return Rd_Response48();

}


//************************************************
//  Get to know card was busy or not
// 		Data[0] Low  : sdhc card is busy
// 		Data[0] High : sdhc card is availbale.
//************************************************
int esdhc_get_cardbusy(void)
{
	if((SDHC_PRSSTAT & SDHC_PRSSTAT_DLSL(1))==0)  // Check DLSL = 0x1 or 0x0 (busy)...
		return(1); // Card is Busy
	else 
		return(0); // Card is Free
}


//************************************************
//************************************************
void CMD2_ALL_SEND_CID(void)
{
   	#if (defined(DEBUG_INFO))
		printf("\n\tall_send_CID cmd start (2)\n");
	#endif

	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	// sending all_send_CID  
    	SDHC_CMDARG =  0x0;
	SDHC_XFERTYP =  MMC_CMD2;
 	while (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK));
 	
   	#if (defined(DEBUG_INFO))
		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tall_send_CID cmd done (2) \n");
 		else 
			printf("\tall_send_CID cmd timeout \n");
	

	if(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK) printf("\t\tAll send CID command Done!\n");

        #endif
	Rd_Response128();

}

//************************************************
// SD Command
//************************************************
uint32_t CMD3_SD_GET_RCA()
{
   	#if (defined(DEBUG_INFO))
		printf("\n\tsend_rca cmd start (3)");
	#endif
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	//sending send_rca
 	SDHC_CMDARG =  0x0;
 	SDHC_XFERTYP =  MMC_CMD3;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

   	#if (defined(DEBUG_INFO)) 	
	 	if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tsend_rca cmd done (3) \n");
	 	else 
		 	printf("\tsend_rca cmd timeout \n");
 	
	
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0)printf("CMD3_SD_GET_RCA SDHC_IRQSTAT =%x \n",SDHC_IRQSTAT);
        #endif
        
    	return Rd_Response48();

}

//************************************************
//************************************************
void CMD9_SEND_CSD(uint32_t card_rca)
{
   	#if (defined(DEBUG_INFO))
		printf("\n\tsend CSD cmd start (9)\n");
	#endif

	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

	//
    	SDHC_CMDARG =  card_rca;
 	SDHC_XFERTYP =  MMC_CMD9;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

   	#if (defined(DEBUG_INFO))	
 		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tsend CSD cmd done (9)\n");
 		else 
			printf("\tsend CSD cmd timeout \n");
	
	
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) printf("Error: CMD9_SEND_CSD SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);
        #endif
	Rd_Response128();

	CSD_Structure		= (uint8_t) ((Response128HH&0xC0000000) >> 30);
	SpecVer 	      		= (uint8_t) ((Response128HH&0x3C000000) >> 26); 
	TAAC				= (uint8_t) ((Response128HH&0x00FF0000) >> 16);
	NSAC				= (uint8_t) ((Response128HH&0x0000FF00) >> 8);
	TranSpeed			= (uint8_t) ((Response128HH&0x000000FF) >> 0);
	CCC					= (uint16_t)((Response128HL&0xFFF00000) >> 20);
	ReadBlLen			= (uint8_t) ((Response128HL&0x000F0000) >> 16);
	ReadBlPartial		= (uint8_t) ((Response128HL&0x00008000) >> 15);
	WriteBlockMisalign	= (uint8_t) ((Response128HL&0x00004000) >> 14);
	ReadBlockMisalign	= (uint8_t) ((Response128HL&0x00002000) >> 13);
	DsrImp				= (uint8_t) ((Response128HH&0x00001000) >> 12);
	CSize				= (uint16_t)(((Response128LH&0xC0000000) >> 30)|((Response128HL&0x000003FF) << 2));
	VddRdCurrMin		= (uint8_t) ((Response128LH&0x38000000) >> 27);
	VddRdCurrMax		= (uint8_t) ((Response128LH&0x07000000) >> 24);
	VddWrCurrMin		= (uint8_t) ((Response128LH&0x00E00000) >> 21);
	VddWrCurrMax		= (uint8_t) ((Response128LH&0x001C0000) >> 18);
	CSizeMult			= (uint8_t) ((Response128LH&0x00038000) >> 15);
	EraseGrpSize		= (uint8_t) ((Response128LH&0x00007C00) >> 10);
	EraseGrpMult		= (uint8_t) ((Response128LH&0x000003E0) >> 5);
	WpGrpSize			= (uint8_t) ((Response128LH&0x0000001F) >> 0);
	WpGrpEnable		= (uint8_t) ((Response128LL&0x80000000) >> 31);
	DefaultECC			= (uint8_t) ((Response128LL&0x60000000) >> 29);
	R2WFactor			= (uint8_t) ((Response128LL&0x1C000000) >> 26);
	WrBlLen				= (uint8_t) ((Response128LL&0x03C00000) >> 22);
	WriteBlPartial		= (uint8_t) ((Response128LL&0x00200000) >> 21);
	ContentProtApp		= (uint8_t) ((Response128LL&0x00010000) >> 16);
	FileFormatGrp		= (uint8_t) ((Response128LL&0x00008000) >> 15);
	Copy				= (uint8_t) ((Response128LL&0x00004000) >> 14);
	PermWriteProtect	= (uint8_t) ((Response128LL&0x00002000) >> 13);
  	TempWriteProtect	= (uint8_t) ((Response128LL&0x00001000) >> 12);
	FileFormat			= (uint8_t) ((Response128LL&0x00000C00) >> 10);
	ECC					= (uint8_t) ((Response128LL&0x00000300) >> 8);
 	CRC					= (uint8_t) ((Response128LL&0x000000FE) >> 1);		

}

//************************************************
//************************************************
uint32_t CMD13_SEND_STATUS(uint32_t card_rca)
{
	uint32_t status_reg;
	
     	#if (defined(DEBUG_INFO))
		printf("\n\tcard status cmd start (13) \n");
	#endif

	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	//
 	SDHC_CMDARG =  card_rca;
 	SDHC_XFERTYP =  MMC_CMD13
					|SDHC_XFERTYP_CICEN_MASK
 					|SDHC_XFERTYP_CCCEN_MASK;
 	while ((SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)!=SDHC_IRQSTAT_CC_MASK) ; 
	
	while((SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)==SDHC_IRQSTAT_CTOE_MASK)
	{
		SDHC_IRQSTAT = 0xFFFFFFFF;

 		//
 		SDHC_CMDARG =  card_rca;
 		SDHC_XFERTYP =  MMC_CMD13
						|SDHC_XFERTYP_CICEN_MASK
 						|SDHC_XFERTYP_CCCEN_MASK;
	}

 	#if (defined(DEBUG_INFO))
 		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tcard status cmd done (13) \n");
 		else 
			printf("\tcard status cmd timeout \n");
	
	
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) 
          printf("Error: CMD13_SEND_STATUS SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);
        #endif
	status_reg = Rd_Response48();
	
	Add_OutOfRange 	= (uint8_t)(((status_reg)>>31)&0x01);
	Add_Misalign 		= (uint8_t)(((status_reg)>>30)&0x01);
	Blk_Len_Err 			= (uint8_t)(((status_reg)>>29)&0x01);
	Erase_Seq_Err 		= (uint8_t)(((status_reg)>>28)&0x01);
	Erase_Param 		= (uint8_t)(((status_reg)>>27)&0x01);
	Wp_Violation 		= (uint8_t)(((status_reg)>>26)&0x01);
	Card_Is_Locked 		= (uint8_t)(((status_reg)>>25)&0x01);
	Lock_Unlock_Failed 	= (uint8_t)(((status_reg)>>24)&0x01);
	Com_CRC_Err 		= (uint8_t)(((status_reg)>>23)&0x01);
	Illegal_Cmd 			= (uint8_t)(((status_reg)>>22)&0x01);
	Card_Ecc_Fail 		= (uint8_t)(((status_reg)>>21)&0x01);
	CC_Err 				= (uint8_t)(((status_reg)>>20)&0x01);
	Err 					= (uint8_t)(((status_reg)>>19)&0x01);
	UndErrun 			= (uint8_t)(((status_reg)>>18)&0x01);
	OverRun			= (uint8_t)(((status_reg)>>17)&0x01);
	CidCsd_Overwrite 	= (uint8_t)(((status_reg)>>16)&0x01);
	WP_Erase_Skip 		= (uint8_t)(((status_reg)>>15)&0x01);
	Erase_Reset 		= (uint8_t)(((status_reg)>>13)&0x01);
	Current_State 		= (uint8_t)(((status_reg)>>9)&0x0f);
	Ready_For_Data 	= (uint8_t)(((status_reg)>>8)&0x01);
	Switch_Err 			= (uint8_t)(((status_reg)>>7)&0x01);
	App_Cmd 			= (uint8_t)(((status_reg)>>5)&0x01);	

	return status_reg;
	
}


//************************************************
//************************************************
void CMD7_CARD_SELECT(uint32_t card_rca)
{
     	#if (defined(DEBUG_INFO))
		printf("\n\tCard Select cmd (7)!\n");
	#endif	

	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb() != 0);  // waiting command line is available	
	
	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

 	//sending card select
 	SDHC_CMDARG =  card_rca;
 	SDHC_XFERTYP =  MMC_CMD7;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

     	#if (defined(DEBUG_INFO)) 	
 		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tCard Select cmd (7) done \n");
 		else 
			printf("\tCard Select cmd (7) timeout \n");
	
	
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) printf("Error: CMD7_CARD_SELECT SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);

        #endif
    	Rd_Response48();

}

//************************************************
//************************************************
void ACMD6_SWITCH(uint32_t card_rca, uint8_t bus_width)
{
   	#if (defined(DEBUG_INFO))
		printf("\n\tSwitch setting command (6)\n");
	#endif

	CMD55_APP_CMD(card_rca);
	
	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb()); // Wait   CMD line free

	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

  	//switch setting bus width
  	if(bus_width == DTW_4BIT)
 		SDHC_CMDARG =  0x2;//bus_width;
 	else 
		SDHC_CMDARG =  0x0;//bus_width;
		
 	SDHC_XFERTYP =  SD_ACMD6; 	
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
 	
   	#if (defined(DEBUG_INFO))
		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tAcmd done (6)\n");
 		else 
 			printf("\tAcmd timeout\n");
	
		
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) printf("Error: ACMD6_SWITCH  SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);
        #endif
	Rd_Response48();

}


//************************************************
//************************************************
void ACMD13_SD_STATUS(uint32_t card_rca, uint8_t bus_width)
{

	uint16_t		blk_len	=0;
	uint16_t		blk_cnt 	=0;
	uint32_t		blk_size	=0;
	uint8_t* 	sd_status;

	sd_status= ((uint8_t* )Store_Card_Reg); 
	
	blk_cnt = 1;
	blk_len = 0x40; // 512 bit SD_Status data 
	blk_size= blk_cnt * blk_len;	// byte base

   	#if (defined(DEBUG_INFO))
		printf("\n\tACMD13 SD status (13)");
	#endif
	
        SDHC_WML = 	((WML_WML0) <<SDHC_WML_WRWML_SHIFT)
                        |((WML_WML0) <<SDHC_WML_RDWML_SHIFT) ; 
	SDHC_PROCTL |= SDHC_PROCTL_DTW(bus_width);
	SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1)| SDHC_BLKATTR_BLKSIZE(blk_len);
	esdhc_set_dma_addr(Store_Card_Reg);	

	Clear_Memory((uint32_t*)Store_Card_Reg, blk_size);
	
	CMD55_APP_CMD(card_rca);
	
	while(esdhc_get_cardbusy()); // Wait Card free
	while(esdhc_get_cihb()); // Wait   CMD line free

	// Clear Command Complete bit
	SDHC_IRQSTAT = 0xFFFFFFFF;

  	//switch setting bus width
 	SDHC_CMDARG =  0x00;//stuff bits
 	SDHC_XFERTYP =  SD_ACMD13
						|SDHC_XFERTYP_DPSEL_MASK
						|SDHC_XFERTYP_DTDSEL_MASK
						|SDHC_XFERTYP_AC12EN_MASK
						|SDHC_XFERTYP_BCEN_MASK
						|SDHC_XFERTYP_DMAEN_MASK;
 	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
 	
   	#if (defined(DEBUG_INFO))	
   		if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
 			printf("\tAcmd done (13)\n");
 		else 
 			printf("\tAcmd13 timeout\n");
	
	
	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0) printf("Error: ACMD13_SD_STATUS SDHC_IRQSTAT  =%x \n",SDHC_IRQSTAT);
        #endif
	Rd_Response48();

//	sdDat_Bus_Width 		= ((sd_status[0])>>6) & 0x3;
//	sdSecured_Mode 		= ((sd_status[1])>>5) & 0x1;
//	sdSd_Card_Type			= sd_status[60] & 0xf;
//	sdSize_Pro_EdArea		= sd_status[56] & 0x7; 
//	sdSpeed_Class			= sd_status[55] & 0x7;
//	sdPreFormance_Move	= sd_status[54] & 0x7;
//	sdAu_Size				= ((sd_status[53])>>2)&0x3;
//	sdErase_Size			= sd_status[51]&0xf;
//	sdErase_Timeout		= ((sd_status[50])>>2)&0x3; 
//	sdErase_Offset			= sd_status[50] & 0x3;
	
}
 