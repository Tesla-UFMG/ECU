/*
 * flash.h
 *
 *  Created on: 27 de mar de 2018
 *      Author: User
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f1xx_hal.h"




#define IS_FLASH_ADDRESS(ADDRESS) (((ADDRESS) >= 0x08000000) && ((ADDRESS) < 0x0807FFFF))

HAL_StatusTypeDef FLASH_ErasePage(uint32_t Page_Address);

HAL_StatusTypeDef FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);






#endif /* FLASH_H_ */
