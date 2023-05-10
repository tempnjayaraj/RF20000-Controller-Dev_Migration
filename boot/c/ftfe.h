#ifndef FTFE_H
#define FTFE_H

#include <stdint.h>
#include "MK26F18.h"

#define FTFE_PROGRAM_PHRASE     0x07
#define FTFE_ERASE_SECTOR       0x09
#define FTFE_PROGRAM_ONCE_CMD   0x43
#define FTFE_READ_ONCE_CMD      0x41
#define FTFE_PROGRAM_SECTION    0x0B
#define FTFE_ERASE_BLOCK        0x08
#define FTFE_ERR_FLAGS_MASK     (FTFE_FSTAT_MGSTAT0_MASK    \
                                 | FTFE_FSTAT_FPVIOL_MASK   \
                                 | FTFE_FSTAT_ACCERR_MASK   \
                                 | FTFE_FSTAT_RDCOLERR_MASK)
                                 
uint8_t program_ftfe_once(uint8_t*, uint8_t, uint8_t); 
uint8_t read_ftfe_once(uint8_t*, uint8_t, uint8_t);
uint8_t erase_ftfe_sector(uint32_t);
uint8_t write_ftfe_phrase(uint32_t,uint8_t *);   
uint8_t save_params2flash(void);
uint8_t write_ftfe_section(uint32_t,uint16_t); 
uint8_t save_jump_address(uint32_t);

#endif