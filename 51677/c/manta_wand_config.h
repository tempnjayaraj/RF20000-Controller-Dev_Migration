#ifndef MANTA_WAND_CONFIG_H
#define MANTA_WAND_CONFIG_H

/* enum containing config file offsets for the MANTA wand parameters LSB/MSB=Least/Most Sig Byte */
enum MANTA_Wand_Config_Offsets
{
  MANTA_PARAM0_HEAD,					
  MANTA_PARAM0_TYPE,					
  MANTA_PARAM0_NUMBER,					
  MANTA_PARAM0_TEMP1,					
  MANTA_PARAM0_TEMP2,					
  MANTA_PARAM0_KEY,					
  MANTA_PARAM0_SIZE,					
  MANTA_PARAM0_CMD,					
  MANTA_PARAM0_IDX,					
  MANTA_PARAM_HIGH_Z_LIMIT_LSB,					/**< Enum value 0 */
  MANTA_PARAM_HIGH_Z_LIMIT_MSB,					
  MANTA_PARAM_LOW_Z_LIMIT_LSB,					
  MANTA_PARAM_LOW_Z_LIMIT_MSB,					
  MANTA_PARAM_Z_VOLTAGE_LSB,					
  MANTA_PARAM_Z_VOLTAGE_MSB,					
  MANTA_PARAM_Z_TIME_LSB,					
  MANTA_PARAM_Z_TIME_MSB,
  MANTA_PARAM_Z_GAIN_LSB,
  MANTA_PARAM_Z_GAIN_MSB,
  MANTA_PARAM_PRE_PRIME_SPD_LSB,				
  MANTA_PARAM_PRE_PRIME_SPD_MSB,			
  MANTA_PARAM_PRE_PRIME_TIME_LSB,				
  MANTA_PARAM_PRE_PRIME_TIME_MSB,				
  MANTA_PARAM_PRE_PRIME_RF_DELAY_LSB,				
  MANTA_PARAM_PRE_PRIME_RF_DELAY_MSB,
  MANTA_PARAM_MANUAL_PRIME_SPD_LSB,				
  MANTA_PARAM_MANUAL_PRIME_SPD_MSB,
  MANTA_PARAM_AUTO_PRIME_ADJ_LSB,				
  MANTA_PARAM_AUTO_PRIME_ADJ_MSB,
  MANTA_PARAM_AUTO_PRIME_INIT_SPD_LSB,
  MANTA_PARAM_AUTO_PRIME_INIT_SPD_MSB,
  MANTA_PARAM_AUTO_PRIME_FINAL_SPD_LSB,
  MANTA_PARAM_AUTO_PRIME_FINAL_SPD_MSB,
  MANTA_PARAM_AUTO_PRIME_RMS_THRESH_LSB,
  MANTA_PARAM_AUTO_PRIME_RMS_THRESH_MSB,
  MANTA_PARAM_AUTO_PRIME_TIMEOUT_LSB,
  MANTA_PARAM_AUTO_PRIME_TIMEOUT_MSB,
  MANTA_PARAM_FLOW_1_LSB,			                
  MANTA_PARAM_FLOW_1_MSB,			              
  MANTA_PARAM_FLOW_2_LSB,			                
  MANTA_PARAM_FLOW_2_MSB,			                
  MANTA_PARAM_FLOW_3_LSB,			                
  MANTA_PARAM_FLOW_3_MSB,			                
  MANTA_PARAM_FLOW_4_LSB,			                
  MANTA_PARAM_FLOW_4_MSB,			                
  MANTA_PARAM_FLOW_5_LSB,			                
  MANTA_PARAM_FLOW_5_MSB,			                
  MANTA_PARAM_COAG_1_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_1_PRIMARY_PWR_MSB,					
  MANTA_PARAM_COAG_2_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_2_PRIMARY_PWR_MSB,					
  MANTA_PARAM_COAG_3_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_3_PRIMARY_PWR_MSB,					
  MANTA_PARAM_COAG_4_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_4_PRIMARY_PWR_MSB,					
  MANTA_PARAM_COAG_5_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_5_PRIMARY_PWR_MSB,					
  MANTA_PARAM_COAG_6_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_6_PRIMARY_PWR_MSB,					
  MANTA_PARAM_COAG_7_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_7_PRIMARY_PWR_MSB,					
  MANTA_PARAM_COAG_8_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_8_PRIMARY_PWR_MSB,					
  MANTA_PARAM_COAG_9_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_9_PRIMARY_PWR_MSB,
  MANTA_PARAM_COAG_10_PRIMARY_PWR_LSB,					
  MANTA_PARAM_COAG_10_PRIMARY_PWR_MSB,
  MANTA_PARAM_IPEAK_VOLTAGE_LSB,					
  MANTA_PARAM_IPEAK_VOLTAGE_MSB,					
  MANTA_PARAM_IPEAK_LOAD_LSB,					
  MANTA_PARAM_IPEAK_LOAD_MSB,
  MANTA_PARAM_PWM_MOD_LSB,
  MANTA_PARAM_PWM_MOD_MSB,                                      /**< Enum value 63 */
  MANTA_PARAM0_CHECKSUM,
  
  MANTA_PARAM1_HEAD,					
  MANTA_PARAM1_TYPE,					
  MANTA_PARAM1_NUMBER,					
  MANTA_PARAM1_TEMP1,					
  MANTA_PARAM1_TEMP2,					
  MANTA_PARAM1_KEY,					
  MANTA_PARAM1_SIZE,					
  MANTA_PARAM1_CMD,					
  MANTA_PARAM1_IDX,	
  MANTA_PARAM_PWM_C0V_LSB,                                      /**< Enum value 0 */
  MANTA_PARAM_PWM_C0V_MSB,
  MANTA_PARAM_PWM_C1V_LSB,
  MANTA_PARAM_PWM_C1V_MSB, 
  MANTA_PARAM_PWM_C2V_LSB,
  MANTA_PARAM_PWM_C2V_MSB,
  MANTA_PARAM_PWM_C3V_LSB,
  MANTA_PARAM_PWM_C3V_MSB,
  MANTA_PARAM_PWM_C4V_LSB,
  MANTA_PARAM_PWM_C4V_MSB,
  MANTA_PARAM_PWM_C5V_LSB,
  MANTA_PARAM_PWM_C5V_MSB,
  MANTA_PARAM_PWM_C6V_LSB,
  MANTA_PARAM_PWM_C6V_MSB,
  MANTA_PARAM_PWM_C7V_LSB,
  MANTA_PARAM_PWM_C7V_MSB,
  MANTA_PARAM_WORN_DET_TIME_LSB,
  MANTA_PARAM_WORN_DET_TIME_MSB,
  MANTA_PARAM_FLOW_1_LLSB,			                
  MANTA_PARAM_FLOW_1_LMSB,
  MANTA_PARAM_FLOW_1_ULSB,			                
  MANTA_PARAM_FLOW_1_UMSB,
  MANTA_PARAM_FLOW_2_LLSB,			                
  MANTA_PARAM_FLOW_2_LMSB,
  MANTA_PARAM_FLOW_2_ULSB,			                
  MANTA_PARAM_FLOW_2_UMSB,
  MANTA_PARAM_FLOW_3_LLSB,			                
  MANTA_PARAM_FLOW_3_LMSB,
  MANTA_PARAM_FLOW_3_ULSB,			                
  MANTA_PARAM_FLOW_3_UMSB,
  MANTA_PARAM_FLOW_4_LLSB,			                
  MANTA_PARAM_FLOW_4_LMSB,
  MANTA_PARAM_FLOW_4_ULSB,			                
  MANTA_PARAM_FLOW_4_UMSB,
  MANTA_PARAM_FLOW_5_LLSB,			                
  MANTA_PARAM_FLOW_5_LMSB,
  MANTA_PARAM_FLOW_5_ULSB,			                
  MANTA_PARAM_FLOW_5_UMSB,
  MANTA_PARAM_RAMP_RATE_UP_LSB,
  MANTA_PARAM_RAMP_RATE_UP_MSB,
  MANTA_PARAM_RAMP_RATE_DOWN_LSB,
  MANTA_PARAM_RAMP_RATE_DOWN_MSB,
  MANTA_PARAM_COAG_RMS_CURRENT_LIMIT_LSB,					
  MANTA_PARAM_COAG_RMS_CURRENT_LIMIT_MSB,					
  MANTA_PARAM_COAG1_FLOW1_LSB,					
  MANTA_PARAM_COAG1_FLOW1_MSB,					
  MANTA_PARAM_COAG1_FLOW2_LSB,					
  MANTA_PARAM_COAG1_FLOW2_MSB,					
  MANTA_PARAM_COAG1_FLOW3_LSB,					
  MANTA_PARAM_COAG1_FLOW3_MSB,					
  MANTA_PARAM_COAG1_FLOW4_LSB,					
  MANTA_PARAM_COAG1_FLOW4_MSB,					
  MANTA_PARAM_COAG1_FLOW5_LSB,					
  MANTA_PARAM_COAG1_FLOW5_MSB,					
  MANTA_PARAM_COAG2_FLOW1_LSB,					
  MANTA_PARAM_COAG2_FLOW1_MSB,					
  MANTA_PARAM_COAG2_FLOW2_LSB,					
  MANTA_PARAM_COAG2_FLOW2_MSB,					
  MANTA_PARAM_COAG2_FLOW3_LSB,					
  MANTA_PARAM_COAG2_FLOW3_MSB,					
  MANTA_PARAM_COAG2_FLOW4_LSB,					
  MANTA_PARAM_COAG2_FLOW4_MSB,					
  MANTA_PARAM_COAG2_FLOW5_LSB,
  MANTA_PARAM_COAG2_FLOW5_MSB,                                  /**< Enum value 63 */					
  MANTA_PARAM1_CHECKSUM,
  
  MANTA_PARAM2_HEAD,					
  MANTA_PARAM2_TYPE,					
  MANTA_PARAM2_NUMBER,					
  MANTA_PARAM2_TEMP1,					
  MANTA_PARAM2_TEMP2,					
  MANTA_PARAM2_KEY,					
  MANTA_PARAM2_SIZE,					
  MANTA_PARAM2_CMD,					
  MANTA_PARAM2_IDX,
  MANTA_PARAM_COAG3_FLOW1_LSB,					
  MANTA_PARAM_COAG3_FLOW1_MSB,					
  MANTA_PARAM_COAG3_FLOW2_LSB,					
  MANTA_PARAM_COAG3_FLOW2_MSB,					
  MANTA_PARAM_COAG3_FLOW3_LSB,					
  MANTA_PARAM_COAG3_FLOW3_MSB,					
  MANTA_PARAM_COAG3_FLOW4_LSB,					
  MANTA_PARAM_COAG3_FLOW4_MSB,					
  MANTA_PARAM_COAG3_FLOW5_LSB,					
  MANTA_PARAM_COAG3_FLOW5_MSB,					
  MANTA_PARAM_COAG4_FLOW1_LSB,					
  MANTA_PARAM_COAG4_FLOW1_MSB,					
  MANTA_PARAM_COAG4_FLOW2_LSB,					
  MANTA_PARAM_COAG4_FLOW2_MSB,					
  MANTA_PARAM_COAG4_FLOW3_LSB,					
  MANTA_PARAM_COAG4_FLOW3_MSB,					
  MANTA_PARAM_COAG4_FLOW4_LSB,					
  MANTA_PARAM_COAG4_FLOW4_MSB,					
  MANTA_PARAM_COAG4_FLOW5_LSB,					
  MANTA_PARAM_COAG4_FLOW5_MSB,					
  MANTA_PARAM_COAG5_FLOW1_LSB,					
  MANTA_PARAM_COAG5_FLOW1_MSB,					
  MANTA_PARAM_COAG5_FLOW2_LSB,					
  MANTA_PARAM_COAG5_FLOW2_MSB,					
  MANTA_PARAM_COAG5_FLOW3_LSB,					
  MANTA_PARAM_COAG5_FLOW3_MSB,					
  MANTA_PARAM_COAG5_FLOW4_LSB,					
  MANTA_PARAM_COAG5_FLOW4_MSB,					
  MANTA_PARAM_COAG5_FLOW5_LSB,					
  MANTA_PARAM_COAG5_FLOW5_MSB,					
  MANTA_PARAM_COAG6_FLOW1_LSB,					
  MANTA_PARAM_COAG6_FLOW1_MSB,					
  MANTA_PARAM_COAG6_FLOW2_LSB,					
  MANTA_PARAM_COAG6_FLOW2_MSB,					
  MANTA_PARAM_COAG6_FLOW3_LSB,					
  MANTA_PARAM_COAG6_FLOW3_MSB,					
  MANTA_PARAM_COAG6_FLOW4_LSB,					
  MANTA_PARAM_COAG6_FLOW4_MSB,					
  MANTA_PARAM_COAG6_FLOW5_LSB,					
  MANTA_PARAM_COAG6_FLOW5_MSB,					
  MANTA_PARAM_COAG7_FLOW1_LSB,					
  MANTA_PARAM_COAG7_FLOW1_MSB,					
  MANTA_PARAM_COAG7_FLOW2_LSB,					
  MANTA_PARAM_COAG7_FLOW2_MSB,					
  MANTA_PARAM_COAG7_FLOW3_LSB,					
  MANTA_PARAM_COAG7_FLOW3_MSB,					
  MANTA_PARAM_COAG7_FLOW4_LSB,					
  MANTA_PARAM_COAG7_FLOW4_MSB,					
  MANTA_PARAM_COAG7_FLOW5_LSB,					
  MANTA_PARAM_COAG7_FLOW5_MSB,					
  MANTA_PARAM_COAG8_FLOW1_LSB,					
  MANTA_PARAM_COAG8_FLOW1_MSB,					
  MANTA_PARAM_COAG8_FLOW2_LSB,					
  MANTA_PARAM_COAG8_FLOW2_MSB,					
  MANTA_PARAM_COAG8_FLOW3_LSB,					
  MANTA_PARAM_COAG8_FLOW3_MSB,					
  MANTA_PARAM_COAG8_FLOW4_LSB,					
  MANTA_PARAM_COAG8_FLOW4_MSB,					
  MANTA_PARAM_COAG8_FLOW5_LSB,					
  MANTA_PARAM_COAG8_FLOW5_MSB,					
  MANTA_PARAM_COAG9_FLOW1_LSB,					
  MANTA_PARAM_COAG9_FLOW1_MSB,					
  MANTA_PARAM_COAG9_FLOW2_LSB,					
  MANTA_PARAM_COAG9_FLOW2_MSB,			                /**< Enum value 63 */
  MANTA_PARAM2_CHECKSUM,
  
  MANTA_PARAM3_HEAD,					
  MANTA_PARAM3_TYPE,					
  MANTA_PARAM3_NUMBER,					
  MANTA_PARAM3_TEMP1,					
  MANTA_PARAM3_TEMP2,					
  MANTA_PARAM3_KEY,					
  MANTA_PARAM3_SIZE,					
  MANTA_PARAM3_CMD,					
  MANTA_PARAM3_IDX,                                    					
  MANTA_PARAM_COAG9_FLOW3_LSB,					
  MANTA_PARAM_COAG9_FLOW3_MSB,					
  MANTA_PARAM_COAG9_FLOW4_LSB,					
  MANTA_PARAM_COAG9_FLOW4_MSB,					
  MANTA_PARAM_COAG9_FLOW5_LSB,					
  MANTA_PARAM_COAG9_FLOW5_MSB,
  MANTA_PARAM_COAG10_FLOW1_LSB,					
  MANTA_PARAM_COAG10_FLOW1_MSB,					
  MANTA_PARAM_COAG10_FLOW2_LSB,					
  MANTA_PARAM_COAG10_FLOW2_MSB,					
  MANTA_PARAM_COAG10_FLOW3_LSB,					
  MANTA_PARAM_COAG10_FLOW3_MSB,					
  MANTA_PARAM_COAG10_FLOW4_LSB,					
  MANTA_PARAM_COAG10_FLOW4_MSB,					
  MANTA_PARAM_COAG10_FLOW5_LSB,					
  MANTA_PARAM_COAG10_FLOW5_MSB,					
  MANTA_PARAM_EMPTY51_LSB,					
  MANTA_PARAM_EMPTY51_MSB,					
  MANTA_PARAM_EMPTY52_LSB,					
  MANTA_PARAM_EMPTY52_MSB,					
  MANTA_PARAM_EMPTY53_LSB,					
  MANTA_PARAM_EMPTY53_MSB,					
  MANTA_PARAM_EMPTY54_LSB,					
  MANTA_PARAM_EMPTY54_MSB,					
  MANTA_PARAM_EMPTY55_LSB,					
  MANTA_PARAM_EMPTY55_MSB,					
  MANTA_PARAM_EMPTY56_LSB,					
  MANTA_PARAM_EMPTY56_MSB,					
  MANTA_PARAM_EMPTY57_LSB,					
  MANTA_PARAM_EMPTY57_MSB,					
  MANTA_PARAM_EMPTY58_LSB,					
  MANTA_PARAM_EMPTY58_MSB,					
  MANTA_PARAM_EMPTY59_LSB,					
  MANTA_PARAM_EMPTY59_MSB,					
  MANTA_PARAM_EMPTY60_LSB,					
  MANTA_PARAM_EMPTY60_MSB,					
  MANTA_PARAM_EMPTY61_LSB,					
  MANTA_PARAM_EMPTY61_MSB,					
  MANTA_PARAM_EMPTY62_LSB,					
  MANTA_PARAM_EMPTY62_MSB,					
  MANTA_PARAM_EMPTY63_LSB,					
  MANTA_PARAM_EMPTY63_MSB,					
  MANTA_PARAM_EMPTY64_LSB,					
  MANTA_PARAM_EMPTY64_MSB,					
  MANTA_PARAM_EMPTY65_LSB,					
  MANTA_PARAM_EMPTY65_MSB,					
  MANTA_PARAM_EMPTY66_LSB,					
  MANTA_PARAM_EMPTY66_MSB,					
  MANTA_PARAM_EMPTY67_LSB,					
  MANTA_PARAM_EMPTY67_MSB,					
  MANTA_PARAM_EMPTY68_LSB,					
  MANTA_PARAM_EMPTY68_MSB,					
  MANTA_PARAM_EMPTY69_LSB,					
  MANTA_PARAM_EMPTY69_MSB,					
  MANTA_PARAM_EMPTY70_LSB,					
  MANTA_PARAM_EMPTY70_MSB,					
  MANTA_PARAM_EMPTY71_LSB,					
  MANTA_PARAM_EMPTY71_MSB,										
  MANTA_PARAM_EMPTY72_LSB,					
  MANTA_PARAM_EMPTY72_MSB,					
  MANTA_PARAM_EMPTY73_LSB,					
  MANTA_PARAM_EMPTY73_MSB,					
  MANTA_PARAM_EMPTY74_LSB,					
  MANTA_PARAM_EMPTY74_MSB,                                      /**< Enum value 63 */					
  MANTA_PARAM3_CHECKSUM,					
  
  MANTA_PARAM4_HEAD,					
  MANTA_PARAM4_TYPE,					
  MANTA_PARAM4_NUMBER,					
  MANTA_PARAM4_TEMP1,					
  MANTA_PARAM4_TEMP2,					
  MANTA_PARAM4_KEY,					
  MANTA_PARAM4_SIZE,					
  MANTA_PARAM4_CMD,					
  MANTA_PARAM4_IDX,
  MANTA_PARAM_EMPTY75_LSB,                                      /**< Enum value 0 */					
  MANTA_PARAM_EMPTY75_MSB,					
  MANTA_PARAM_EMPTY76_LSB,					
  MANTA_PARAM_EMPTY76_MSB,
  MANTA_PARAM_EMPTY77_LSB,					
  MANTA_PARAM_EMPTY77_MSB,					
  MANTA_PARAM_EMPTY78_LSB,					
  MANTA_PARAM_EMPTY78_MSB,					
  MANTA_PARAM_EMPTY79_LSB,					
  MANTA_PARAM_EMPTY79_MSB,					
  MANTA_PARAM_EMPTY80_LSB,					
  MANTA_PARAM_EMPTY80_MSB,					
  MANTA_PARAM_EMPTY81_LSB,					
  MANTA_PARAM_EMPTY81_MSB,					
  MANTA_PARAM_EMPTY82_LSB,					
  MANTA_PARAM_EMPTY82_MSB,					
  MANTA_PARAM_EMPTY83_LSB,					
  MANTA_PARAM_EMPTY83_MSB,					
  MANTA_PARAM_EMPTY84_LSB,					
  MANTA_PARAM_EMPTY84_MSB,					
  MANTA_PARAM_EMPTY85_LSB,					
  MANTA_PARAM_EMPTY85_MSB,					
  MANTA_PARAM_EMPTY86_LSB,					
  MANTA_PARAM_EMPTY86_MSB,					
  MANTA_PARAM_EMPTY87_LSB,					
  MANTA_PARAM_EMPTY87_MSB,					
  MANTA_PARAM_EMPTY88_LSB,					
  MANTA_PARAM_EMPTY88_MSB,					
  MANTA_PARAM_EMPTY89_LSB,					
  MANTA_PARAM_EMPTY89_MSB,					
  MANTA_PARAM_EMPTY90_LSB,					
  MANTA_PARAM_EMPTY90_MSB,					
  MANTA_PARAM_EMPTY91_LSB,					
  MANTA_PARAM_EMPTY91_MSB,					
  MANTA_PARAM_EMPTY92_LSB,					
  MANTA_PARAM_EMPTY92_MSB,					
  MANTA_PARAM_EMPTY93_LSB,					
  MANTA_PARAM_EMPTY93_MSB,					
  MANTA_PARAM_EMPTY94_LSB,					
  MANTA_PARAM_EMPTY94_MSB,					
  MANTA_PARAM_EMPTY95_LSB,					
  MANTA_PARAM_EMPTY95_MSB,					
  MANTA_PARAM_EMPTY96_LSB,					
  MANTA_PARAM_EMPTY96_MSB,					
  MANTA_PARAM_EMPTY97_LSB,					
  MANTA_PARAM_EMPTY97_MSB,					
  MANTA_PARAM_EMPTY98_LSB,					
  MANTA_PARAM_EMPTY98_MSB,					
  MANTA_PARAM_EMPTY99_LSB,					
  MANTA_PARAM_EMPTY99_MSB,					
  MANTA_PARAM_EMPTY100_LSB,					
  MANTA_PARAM_EMPTY100_MSB,					
  MANTA_PARAM_EMPTY101_LSB,					
  MANTA_PARAM_EMPTY101_MSB,					
  MANTA_PARAM_EMPTY102_LSB,					
  MANTA_PARAM_EMPTY102_MSB,					
  MANTA_PARAM_EMPTY103_LSB,					
  MANTA_PARAM_EMPTY103_MSB,															
  MANTA_PARAM_REVISION_LSB,					
  MANTA_PARAM_REVISION_MSB,					
  MANTA_PARAM_EMPTY104_LSB,					
  MANTA_PARAM_EMPTY104_MSB,					
  MANTA_PARAM_CHECKSUM_LSB,					
  MANTA_PARAM_CHECKSUM_MSB,                                     /**< Enum value 63 */					
  MANTA_PARAM4_CHECKSUM,
  
};

#endif