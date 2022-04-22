
#ifndef INC_FLASH_SECTOR_H7_H_
#define INC_FLASH_SECTOR_H7_H_

#include "stm32h7xx_hal.h"

uint32_t Flash_Write_Data(uint32_t StartSectorAddress, uint32_t* data,
                          uint16_t numberofwords);
void Flash_Read_Data(uint32_t StartSectorAddress, uint32_t* data, uint16_t numberofwords);

#endif /* INC_FLASH_SECTOR_H7_H_ */
