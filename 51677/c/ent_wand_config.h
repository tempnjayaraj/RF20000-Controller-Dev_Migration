#ifndef ENT_WAND_CONFIG_H
#define ENT_WAND_CONFIG_H

/* enum containing config file offsets for the ent wand parameters LSB/MSB=Least/Most Sig Byte */
enum Ent_Wand_Config_Offsets
{
	ENT_PARAM0_HEAD,
        ENT_PARAM0_TYPE,
        ENT_PARAM0_NUMBER,
        ENT_PARAM0_TEMP1,
        ENT_PARAM0_TEMP2,
        ENT_PARAM0_KEY,
        ENT_PARAM0_SIZE,
        ENT_PARAM0_CMD,
        ENT_PARAM0_IDX,
        /* low voltage impedance test parameters */
	ENT_PARAM_HIGH_Z_LIMIT_LSB,								/**< Enum value 0 */
	ENT_PARAM_HIGH_Z_LIMIT_MSB,								/**< Enum value 1 */
	ENT_PARAM_LOW_Z_LIMIT_LSB,								/**< Enum value 2 */
	ENT_PARAM_LOW_Z_LIMIT_MSB,								/**< Enum value 3 */
	ENT_PARAM_Z_VOLTAGE_LSB,								/**< Enum value 4 */
	ENT_PARAM_Z_VOLTAGE_MSB,								/**< Enum value 5 */
	ENT_PARAM_Z_TIME_LSB,									/**< Enum value 6 */
	ENT_PARAM_Z_TIME_MSB,									/**< Enum value 7 */
	ENT_PARAM_Z_GAIN_LSB,									/**< Enum value 8 */
	ENT_PARAM_Z_GAIN_MSB,									/**< Enum value 9 */
	/* inflow prime parameters */
	ENT_PARAM_PRE_PRIME_SPD_LSB,							/**< Enum value 10 */
	ENT_PARAM_PRE_PRIME_SPD_MSB,							/**< Enum value 11 */
	ENT_PARAM_PRE_PRIME_TIME_LSB,							/**< Enum value 12 */
	ENT_PARAM_PRE_PRIME_TIME_MSB,							/**< Enum value 13 */
	ENT_PARAM_PRE_PRIME_RF_DELAY_LSB,						/**< Enum value 14 */
	ENT_PARAM_PRE_PRIME_RF_DELAY_MSB,						/**< Enum value 15 */
	ENT_PARAM_AUTO_PRIME_SPD_LSB,							/**< Enum value 16 */
	ENT_PARAM_AUTO_PRIME_SPD_MSB,							/**< Enum value 17 */
	ENT_PARAM_AUTO_PRIME_TIME_LSB,							/**< Enum value 18 */
	ENT_PARAM_AUTO_PRIME_TIME_MSB,							/**< Enum value 19 */
	/* ablate lo mode parameters */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_LO_MINUS_LSB,			/**< Enum value 20 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_LO_MINUS_MSB,			/**< Enum value 21 */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_LO_MINUS_LSB,		/**< Enum value 22 */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_LO_MINUS_MSB,		/**< Enum value 23 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_LO_MINUS_LSB,		/**< Enum value 24 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_LO_MINUS_MSB,		/**< Enum value 25 */
	ENT_PARAM_ABLATE_BLEND_TIME_LO_MINUS_LSB,				/**< Enum value 26 */
	ENT_PARAM_ABLATE_BLEND_TIME_LO_MINUS_MSB,				/**< Enum value 27 */
	ENT_PARAM_ABLATE_FLOW_1_LO_MINUS_LSB,					/**< Enum value 28 */
	ENT_PARAM_ABLATE_FLOW_1_LO_MINUS_MSB,					/**< Enum value 29 */
	ENT_PARAM_ABLATE_FLOW_2_LO_MINUS_LSB,					/**< Enum value 30 */
	ENT_PARAM_ABLATE_FLOW_2_LO_MINUS_MSB,					/**< Enum value 31 */
	ENT_PARAM_ABLATE_FLOW_3_LO_MINUS_LSB,					/**< Enum value 32 */
	ENT_PARAM_ABLATE_FLOW_3_LO_MINUS_MSB,					/**< Enum value 33 */
	ENT_PARAM_ABLATE_FLOW_4_LO_MINUS_LSB,					/**< Enum value 34 */
	ENT_PARAM_ABLATE_FLOW_4_LO_MINUS_MSB,					/**< Enum value 35 */
	ENT_PARAM_ABLATE_FLOW_5_LO_MINUS_LSB,					/**< Enum value 36 */
	ENT_PARAM_ABLATE_FLOW_5_LO_MINUS_MSB,					/**< Enum value 37 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_LO_DEFAULT_LSB,		/**< Enum value 38 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_LO_DEFAULT_MSB,		/**< Enum value 39 */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_LO_DEFAULT_LSB,		/**< Enum value 40 */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_LO_DEFAULT_MSB,		/**< Enum value 41 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_LO_DEFAULT_LSB,	/**< Enum value 42 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_LO_DEFAULT_MSB,	/**< Enum value 43 */
	ENT_PARAM_ABLATE_BLEND_TIME_LO_DEFAULT_LSB,				/**< Enum value 44 */
	ENT_PARAM_ABLATE_BLEND_TIME_LO_DEFAULT_MSB,				/**< Enum value 45 */
	ENT_PARAM_ABLATE_FLOW_1_LO_DEFAULT_LSB,					/**< Enum value 46 */
	ENT_PARAM_ABLATE_FLOW_1_LO_DEFAULT_MSB,					/**< Enum value 47 */
	ENT_PARAM_ABLATE_FLOW_2_LO_DEFAULT_LSB,					/**< Enum value 48 */
	ENT_PARAM_ABLATE_FLOW_2_LO_DEFAULT_MSB,					/**< Enum value 49 */
	ENT_PARAM_ABLATE_FLOW_3_LO_DEFAULT_LSB,					/**< Enum value 50 */
	ENT_PARAM_ABLATE_FLOW_3_LO_DEFAULT_MSB,					/**< Enum value 51 */
	ENT_PARAM_ABLATE_FLOW_4_LO_DEFAULT_LSB,					/**< Enum value 52 */
	ENT_PARAM_ABLATE_FLOW_4_LO_DEFAULT_MSB,					/**< Enum value 53 */
	ENT_PARAM_ABLATE_FLOW_5_LO_DEFAULT_LSB,					/**< Enum value 54 */
	ENT_PARAM_ABLATE_FLOW_5_LO_DEFAULT_MSB,					/**< Enum value 55 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_LO_PLUS_LSB,			/**< Enum value 56 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_LO_PLUS_MSB,			/**< Enum value 57 */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_LO_PLUS_LSB,			/**< Enum value 58 */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_LO_PLUS_MSB,			/**< Enum value 59 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_LO_PLUS_LSB,		/**< Enum value 60 */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_LO_PLUS_MSB,		/**< Enum value 61 */
	ENT_PARAM_ABLATE_BLEND_TIME_LO_PLUS_LSB,				/**< Enum value 62 */
	ENT_PARAM_ABLATE_BLEND_TIME_LO_PLUS_MSB,				/**< Enum value 63 */
        ENT_PARAM0_CHECKSUM,
        ENT_PARAM1_HEAD,
        ENT_PARAM1_TYPE,
        ENT_PARAM1_NUMBER,
        ENT_PARAM1_TEMP1,
        ENT_PARAM1_TEMP2,
        ENT_PARAM1_KEY,
        ENT_PARAM1_SIZE,
        ENT_PARAM1_CMD,
        ENT_PARAM1_IDX,
        ENT_PARAM_ABLATE_FLOW_1_LO_PLUS_LSB,					/**< Enum value 64 */
	ENT_PARAM_ABLATE_FLOW_1_LO_PLUS_MSB,					/**< Enum value 65 */
	ENT_PARAM_ABLATE_FLOW_2_LO_PLUS_LSB,					/**< Enum value 66 */
	ENT_PARAM_ABLATE_FLOW_2_LO_PLUS_MSB,					/**< Enum value 67 */
	ENT_PARAM_ABLATE_FLOW_3_LO_PLUS_LSB,					/**< Enum value 68 */
	ENT_PARAM_ABLATE_FLOW_3_LO_PLUS_MSB,					/**< Enum value 69 */
	ENT_PARAM_ABLATE_FLOW_4_LO_PLUS_LSB,					/**< Enum value 70 */
	ENT_PARAM_ABLATE_FLOW_4_LO_PLUS_MSB,					/**< Enum value 71 */
	ENT_PARAM_ABLATE_FLOW_5_LO_PLUS_LSB,					/**< Enum value 72 */
	ENT_PARAM_ABLATE_FLOW_5_LO_PLUS_MSB,					/**< Enum value 73 */
	ENT_PARAM_ABLATE_RMS_LIMIT_LO_LSB,						/**< Enum value 74 */
	ENT_PARAM_ABLATE_RMS_LIMIT_LO_MSB,						/**< Enum value 75 */
	ENT_PARAM_ABLATE_PULSE_WINDOW_LO_LSB,					/**< Enum value 76 */
	ENT_PARAM_ABLATE_PULSE_WINDOW_LO_MSB,					/**< Enum value 77 */
	ENT_PARAM_ABLATE_PULSE_ENERGY_LO_LSB,					/**< Enum value 78 */
	ENT_PARAM_ABLATE_PULSE_ENERGY_LO_MSB,					/**< Enum value 79 */
	/* ablate med mode parameters */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_MED_MINUS_LSB,			/**< Enum value 80  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_MED_MINUS_MSB,			/**< Enum value 81  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_MED_MINUS_LSB,		/**< Enum value 82  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_MED_MINUS_MSB,		/**< Enum value 83  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_MED_MINUS_LSB,	/**< Enum value 84  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_MED_MINUS_MSB,	/**< Enum value 85  */
	ENT_PARAM_ABLATE_BLEND_TIME_MED_MINUS_LSB,				/**< Enum value 86  */
	ENT_PARAM_ABLATE_BLEND_TIME_MED_MINUS_MSB,				/**< Enum value 87  */
	ENT_PARAM_ABLATE_FLOW_1_MED_MINUS_LSB,					/**< Enum value 88  */
	ENT_PARAM_ABLATE_FLOW_1_MED_MINUS_MSB,					/**< Enum value 89  */
	ENT_PARAM_ABLATE_FLOW_2_MED_MINUS_LSB,					/**< Enum value 90  */
	ENT_PARAM_ABLATE_FLOW_2_MED_MINUS_MSB,					/**< Enum value 91  */
	ENT_PARAM_ABLATE_FLOW_3_MED_MINUS_LSB,					/**< Enum value 92  */
	ENT_PARAM_ABLATE_FLOW_3_MED_MINUS_MSB,					/**< Enum value 93  */
	ENT_PARAM_ABLATE_FLOW_4_MED_MINUS_LSB,					/**< Enum value 94  */
	ENT_PARAM_ABLATE_FLOW_4_MED_MINUS_MSB,					/**< Enum value 95  */
	ENT_PARAM_ABLATE_FLOW_5_MED_MINUS_LSB,					/**< Enum value 96  */
	ENT_PARAM_ABLATE_FLOW_5_MED_MINUS_MSB,					/**< Enum value 97  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_MED_DEFAULT_LSB,		/**< Enum value 98  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_MED_DEFAULT_MSB,		/**< Enum value 99  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_MED_DEFAULT_LSB,		/**< Enum value 100  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_MED_DEFAULT_MSB,		/**< Enum value 101  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_MED_DEFAULT_LSB,	/**< Enum value 102  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_MED_DEFAULT_MSB,	/**< Enum value 103  */
	ENT_PARAM_ABLATE_BLEND_TIME_MED_DEFAULT_LSB,			/**< Enum value 104  */
	ENT_PARAM_ABLATE_BLEND_TIME_MED_DEFAULT_MSB,			/**< Enum value 105  */
	ENT_PARAM_ABLATE_FLOW_1_MED_DEFAULT_LSB,				/**< Enum value 106  */
	ENT_PARAM_ABLATE_FLOW_1_MED_DEFAULT_MSB,				/**< Enum value 107  */
	ENT_PARAM_ABLATE_FLOW_2_MED_DEFAULT_LSB,				/**< Enum value 108  */
	ENT_PARAM_ABLATE_FLOW_2_MED_DEFAULT_MSB,				/**< Enum value 109  */
	ENT_PARAM_ABLATE_FLOW_3_MED_DEFAULT_LSB,				/**< Enum value 110  */
	ENT_PARAM_ABLATE_FLOW_3_MED_DEFAULT_MSB,				/**< Enum value 111  */
	ENT_PARAM_ABLATE_FLOW_4_MED_DEFAULT_LSB,				/**< Enum value 112  */
	ENT_PARAM_ABLATE_FLOW_4_MED_DEFAULT_MSB,				/**< Enum value 113  */
	ENT_PARAM_ABLATE_FLOW_5_MED_DEFAULT_LSB,				/**< Enum value 114  */
	ENT_PARAM_ABLATE_FLOW_5_MED_DEFAULT_MSB,				/**< Enum value 115  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_MED_PLUS_LSB,			/**< Enum value 116  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_MED_PLUS_MSB,			/**< Enum value 117  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_MED_PLUS_LSB,		/**< Enum value 118  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_MED_PLUS_MSB,		/**< Enum value 119  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_MED_PLUS_LSB,		/**< Enum value 120  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_MED_PLUS_MSB,		/**< Enum value 121  */
	ENT_PARAM_ABLATE_BLEND_TIME_MED_PLUS_LSB,				/**< Enum value 122  */
	ENT_PARAM_ABLATE_BLEND_TIME_MED_PLUS_MSB,				/**< Enum value 123  */
	ENT_PARAM_ABLATE_FLOW_1_MED_PLUS_LSB,					/**< Enum value 124  */
	ENT_PARAM_ABLATE_FLOW_1_MED_PLUS_MSB,					/**< Enum value 125  */
	ENT_PARAM_ABLATE_FLOW_2_MED_PLUS_LSB,					/**< Enum value 126  */
	ENT_PARAM_ABLATE_FLOW_2_MED_PLUS_MSB,					/**< Enum value 127  */
	ENT_PARAM1_CHECKSUM,
        ENT_PARAM2_HEAD,
        ENT_PARAM2_TYPE,
        ENT_PARAM2_NUMBER,
        ENT_PARAM2_TEMP1,
        ENT_PARAM2_TEMP2,
        ENT_PARAM2_KEY,
        ENT_PARAM2_SIZE,
        ENT_PARAM2_CMD,
        ENT_PARAM2_IDX,
        ENT_PARAM_ABLATE_FLOW_3_MED_PLUS_LSB,					/**< Enum value 128  */
	ENT_PARAM_ABLATE_FLOW_3_MED_PLUS_MSB,					/**< Enum value 129  */
	ENT_PARAM_ABLATE_FLOW_4_MED_PLUS_LSB,					/**< Enum value 130  */
	ENT_PARAM_ABLATE_FLOW_4_MED_PLUS_MSB,					/**< Enum value 131  */
	ENT_PARAM_ABLATE_FLOW_5_MED_PLUS_LSB,					/**< Enum value 132  */
	ENT_PARAM_ABLATE_FLOW_5_MED_PLUS_MSB,					/**< Enum value 133  */
	ENT_PARAM_ABLATE_RMS_LIMIT_MED_LSB,						/**< Enum value 134  */
	ENT_PARAM_ABLATE_RMS_LIMIT_MED_MSB,						/**< Enum value 135  */
	ENT_PARAM_ABLATE_PULSE_WINDOW_MED_LSB,					/**< Enum value 136  */
	ENT_PARAM_ABLATE_PULSE_WINDOW_MED_MSB,					/**< Enum value 137  */
	ENT_PARAM_ABLATE_PULSE_ENERGY_MED_LSB,					/**< Enum value 138  */
	ENT_PARAM_ABLATE_PULSE_ENERGY_MED_MSB,					/**< Enum value 139  */
	/* ablate hi mode parameters */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_HI_MINUS_LSB,			/**< Enum value 140  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_HI_MINUS_MSB,			/**< Enum value 141  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_HI_MINUS_LSB,		/**< Enum value 142  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_HI_MINUS_MSB,		/**< Enum value 143  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_HI_MINUS_LSB,		/**< Enum value 144  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_HI_MINUS_MSB,		/**< Enum value 145  */
	ENT_PARAM_ABLATE_BLEND_TIME_HI_MINUS_LSB,				/**< Enum value 146  */
	ENT_PARAM_ABLATE_BLEND_TIME_HI_MINUS_MSB,				/**< Enum value 147  */
	ENT_PARAM_ABLATE_FLOW_1_HI_MINUS_LSB,					/**< Enum value 148  */
	ENT_PARAM_ABLATE_FLOW_1_HI_MINUS_MSB,					/**< Enum value 149  */
	ENT_PARAM_ABLATE_FLOW_2_HI_MINUS_LSB,					/**< Enum value 150  */
	ENT_PARAM_ABLATE_FLOW_2_HI_MINUS_MSB,					/**< Enum value 151  */
	ENT_PARAM_ABLATE_FLOW_3_HI_MINUS_LSB,					/**< Enum value 152  */
	ENT_PARAM_ABLATE_FLOW_3_HI_MINUS_MSB,					/**< Enum value 153  */
	ENT_PARAM_ABLATE_FLOW_4_HI_MINUS_LSB,					/**< Enum value 154  */
	ENT_PARAM_ABLATE_FLOW_4_HI_MINUS_MSB,					/**< Enum value 155  */
	ENT_PARAM_ABLATE_FLOW_5_HI_MINUS_LSB,					/**< Enum value 156  */
	ENT_PARAM_ABLATE_FLOW_5_HI_MINUS_MSB,					/**< Enum value 157  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_HI_DEFAULT_LSB,		/**< Enum value 158  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_HI_DEFAULT_MSB,		/**< Enum value 159  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_HI_DEFAULT_LSB,		/**< Enum value 160  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_HI_DEFAULT_MSB,		/**< Enum value 161  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_HI_DEFAULT_LSB,	/**< Enum value 162  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_HI_DEFAULT_MSB,	/**< Enum value 163  */
	ENT_PARAM_ABLATE_BLEND_TIME_HI_DEFAULT_LSB,				/**< Enum value 164  */
	ENT_PARAM_ABLATE_BLEND_TIME_HI_DEFAULT_MSB,				/**< Enum value 165  */
	ENT_PARAM_ABLATE_FLOW_1_HI_DEFAULT_LSB,					/**< Enum value 166  */
	ENT_PARAM_ABLATE_FLOW_1_HI_DEFAULT_MSB,					/**< Enum value 167  */
	ENT_PARAM_ABLATE_FLOW_2_HI_DEFAULT_LSB,					/**< Enum value 168  */
	ENT_PARAM_ABLATE_FLOW_2_HI_DEFAULT_MSB,					/**< Enum value 169  */
	ENT_PARAM_ABLATE_FLOW_3_HI_DEFAULT_LSB,					/**< Enum value 170  */
	ENT_PARAM_ABLATE_FLOW_3_HI_DEFAULT_MSB,					/**< Enum value 171  */
	ENT_PARAM_ABLATE_FLOW_4_HI_DEFAULT_LSB,					/**< Enum value 172  */
	ENT_PARAM_ABLATE_FLOW_4_HI_DEFAULT_MSB,					/**< Enum value 173  */
	ENT_PARAM_ABLATE_FLOW_5_HI_DEFAULT_LSB,					/**< Enum value 174  */
	ENT_PARAM_ABLATE_FLOW_5_HI_DEFAULT_MSB,					/**< Enum value 175  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_HI_PLUS_LSB,			/**< Enum value 176  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_HI_PLUS_MSB,			/**< Enum value 177  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_HI_PLUS_LSB,			/**< Enum value 178  */
	ENT_PARAM_ABLATE_SECONDARY_VOLTAGE_HI_PLUS_MSB,			/**< Enum value 179  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_HI_PLUS_LSB,		/**< Enum value 180  */
	ENT_PARAM_ABLATE_PRIMARY_VOLTAGE_TIME_HI_PLUS_MSB,		/**< Enum value 181  */
	ENT_PARAM_ABLATE_BLEND_TIME_HI_PLUS_LSB,				/**< Enum value 182  */
	ENT_PARAM_ABLATE_BLEND_TIME_HI_PLUS_MSB,				/**< Enum value 183  */
	ENT_PARAM_ABLATE_FLOW_1_HI_PLUS_LSB,					/**< Enum value 184  */
	ENT_PARAM_ABLATE_FLOW_1_HI_PLUS_MSB,					/**< Enum value 185  */
	ENT_PARAM_ABLATE_FLOW_2_HI_PLUS_LSB,					/**< Enum value 186  */
	ENT_PARAM_ABLATE_FLOW_2_HI_PLUS_MSB,					/**< Enum value 187  */
	ENT_PARAM_ABLATE_FLOW_3_HI_PLUS_LSB,					/**< Enum value 188  */
	ENT_PARAM_ABLATE_FLOW_3_HI_PLUS_MSB,					/**< Enum value 189  */
	ENT_PARAM_ABLATE_FLOW_4_HI_PLUS_LSB,					/**< Enum value 190  */
	ENT_PARAM_ABLATE_FLOW_4_HI_PLUS_MSB,					/**< Enum value 191  */
	ENT_PARAM2_CHECKSUM,
        ENT_PARAM3_HEAD,
        ENT_PARAM3_TYPE,
        ENT_PARAM3_NUMBER,
        ENT_PARAM3_TEMP1,
        ENT_PARAM3_TEMP2,
        ENT_PARAM3_KEY,
        ENT_PARAM3_SIZE,
        ENT_PARAM3_CMD,
        ENT_PARAM3_IDX,
        ENT_PARAM_ABLATE_FLOW_5_HI_PLUS_LSB,					/**< Enum value 192  */
	ENT_PARAM_ABLATE_FLOW_5_HI_PLUS_MSB,					/**< Enum value 193  */
	ENT_PARAM_ABLATE_RMS_LIMIT_HI_LSB,						/**< Enum value 194  */
	ENT_PARAM_ABLATE_RMS_LIMIT_HI_MSB,						/**< Enum value 195  */
	ENT_PARAM_ABLATE_PULSE_WINDOW_HI_LSB,					/**< Enum value 196  */
	ENT_PARAM_ABLATE_PULSE_WINDOW_HI_MSB,					/**< Enum value 197  */
	ENT_PARAM_ABLATE_PULSE_ENERGY_HI_LSB,					/**< Enum value 198  */
	ENT_PARAM_ABLATE_PULSE_ENERGY_HI_MSB,					/**< Enum value 199  */
	/* coag parameters */
	ENT_PARAM_COAG_1_PRIMARY_PWR_LSB,						/**< Enum value 200  */
	ENT_PARAM_COAG_1_PRIMARY_PWR_MSB,						/**< Enum value 201  */
	ENT_PARAM_COAG_1_SECONDARY_PWR_LSB,						/**< Enum value 202  */
	ENT_PARAM_COAG_1_SECONDARY_PWR_MSB,						/**< Enum value 203  */
	ENT_PARAM_COAG_1_PRIMARY_TIME_LSB,						/**< Enum value 204  */
	ENT_PARAM_COAG_1_PRIMARY_TIME_MSB,						/**< Enum value 205  */
	ENT_PARAM_COAG_1_BLEND_TIME_LSB,						/**< Enum value 206  */
	ENT_PARAM_COAG_1_BLEND_TIME_MSB,						/**< Enum value 207  */
	ENT_PARAM_COAG_1_PRIMARY_VOLTAGE_LIMIT_LSB,				/**< Enum value 208  */
	ENT_PARAM_COAG_1_PRIMARY_VOLTAGE_LIMIT_MSB,				/**< Enum value 209  */
	ENT_PARAM_COAG_1_SECONDARY_VOLTAGE_LIMIT_LSB,			/**< Enum value 210  */
	ENT_PARAM_COAG_1_SECONDARY_VOLTAGE_LIMIT_MSB,			/**< Enum value 211  */
	ENT_PARAM_COAG_1_SPD_LSB,								/**< Enum value 212  */
	ENT_PARAM_COAG_1_SPD_MSB,								/**< Enum value 213  */
	ENT_PARAM_COAG_2_PRIMARY_PWR_LSB,						/**< Enum value 214  */
	ENT_PARAM_COAG_2_PRIMARY_PWR_MSB,						/**< Enum value 215  */
	ENT_PARAM_COAG_2_SECONDARY_PWR_LSB,						/**< Enum value 216  */
	ENT_PARAM_COAG_2_SECONDARY_PWR_MSB,						/**< Enum value 217  */
	ENT_PARAM_COAG_2_PRIMARY_TIME_LSB,						/**< Enum value 218  */
	ENT_PARAM_COAG_2_PRIMARY_TIME_MSB,						/**< Enum value 219  */
	ENT_PARAM_COAG_2_BLEND_TIME_LSB,						/**< Enum value 220  */
	ENT_PARAM_COAG_2_BLEND_TIME_MSB,						/**< Enum value 221  */
	ENT_PARAM_COAG_2_PRIMARY_VOLTAGE_LIMIT_LSB,				/**< Enum value 222  */
	ENT_PARAM_COAG_2_PRIMARY_VOLTAGE_LIMIT_MSB,				/**< Enum value 223  */
	ENT_PARAM_COAG_2_SECONDARY_VOLTAGE_LIMIT_LSB,			/**< Enum value 224  */
	ENT_PARAM_COAG_2_SECONDARY_VOLTAGE_LIMIT_MSB,			/**< Enum value 225  */
	ENT_PARAM_COAG_2_SPD_LSB,								/**< Enum value 226  */
	ENT_PARAM_COAG_2_SPD_MSB,								/**< Enum value 227  */
	ENT_PARAM_COAG_3_PRIMARY_PWR_LSB,						/**< Enum value 228  */
	ENT_PARAM_COAG_3_PRIMARY_PWR_MSB,						/**< Enum value 229  */
	ENT_PARAM_COAG_3_SECONDARY_PWR_LSB,						/**< Enum value 230  */
	ENT_PARAM_COAG_3_SECONDARY_PWR_MSB,						/**< Enum value 231  */
	ENT_PARAM_COAG_3_PRIMARY_TIME_LSB,						/**< Enum value 232  */
	ENT_PARAM_COAG_3_PRIMARY_TIME_MSB,						/**< Enum value 233  */
	ENT_PARAM_COAG_3_BLEND_TIME_LSB,						/**< Enum value 234  */
	ENT_PARAM_COAG_3_BLEND_TIME_MSB,						/**< Enum value 235  */
	ENT_PARAM_COAG_3_PRIMARY_VOLTAGE_LIMIT_LSB,				/**< Enum value 236  */
	ENT_PARAM_COAG_3_PRIMARY_VOLTAGE_LIMIT_MSB,				/**< Enum value 237  */
	ENT_PARAM_COAG_3_SECONDARY_VOLTAGE_LIMIT_LSB,			/**< Enum value 238  */
	ENT_PARAM_COAG_3_SECONDARY_VOLTAGE_LIMIT_MSB,			/**< Enum value 239  */
	ENT_PARAM_COAG_3_SPD_LSB,								/**< Enum value 240  */
	ENT_PARAM_COAG_3_SPD_MSB,								/**< Enum value 241  */
	/* over current parameters (ipeak) */
	ENT_PARAM_IPEAK_VOLTAGE_LSB,							/**< Enum value 242  */
	ENT_PARAM_IPEAK_VOLTAGE_MSB,							/**< Enum value 243  */
	ENT_PARAM_IPEAK_LOAD_LSB,								/**< Enum value 244  */
	ENT_PARAM_IPEAK_LOAD_MSB,								/**< Enum value 245  */
	/* ramp rate parameters */
	ENT_PARAM_ABLATE_RAMP_UP_RATE_LSB,						/**< Enum value 246  */
	ENT_PARAM_ABLATE_RAMP_UP_RATE_MSB,						/**< Enum value 247  */
	ENT_PARAM_ABLATE_RAMP_DOWN_RATE_LSB,					/**< Enum value 248  */
	ENT_PARAM_ABLATE_RAMP_DOWN_RATE_MSB,					/**< Enum value 249  */
        /* param data ends */
        ENT_PARAM_EMPTY00_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY00_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY01_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY01_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY02_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY02_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY03_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY03_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY04_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY04_MSB,								/**< Enum value 244  */
        ENT_PARAM3_CHECKSUM,
        ENT_PARAM4_HEAD,
        ENT_PARAM4_TYPE,
        ENT_PARAM4_NUMBER,
        ENT_PARAM4_TEMP1,
        ENT_PARAM4_TEMP2,
        ENT_PARAM4_KEY,
        ENT_PARAM4_SIZE,
        ENT_PARAM4_CMD,
        ENT_PARAM4_IDX,
        ENT_PARAM_EMPTY05_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY05_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY06_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY06_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY07_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY07_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY08_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY08_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY09_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY09_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY10_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY10_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY11_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY11_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY12_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY12_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY13_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY14_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY14_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY15_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY16_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY16_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY17_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY17_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY18_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY18_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY19_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY19_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY20_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY20_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY21_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY21_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY22_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY22_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY23_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY23_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY24_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY24_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY25_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY25_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY26_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY26_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY27_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY27_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY28_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY28_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY29_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY29_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY30_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY30_MSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY31_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY31_MSB,								/**< Enum value 244  */
        ENT_PARAM_REVISION_LSB,
        ENT_PARAM_REVISION_MSB,
        ENT_PARAM_EMPTY32_LSB,								/**< Enum value 244  */
        ENT_PARAM_EMPTY32_MSB,								/**< Enum value 244  */
        ENT_PARAM_CHECKSUM_LSB,
        ENT_PARAM_CHECKSUM_MSB,   
        ENT_PARAM4_CHECKSUM,
};

#endif