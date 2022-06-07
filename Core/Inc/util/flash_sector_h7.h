/**
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************
  File:       FLASH_SECTOR_H7.h
  Modifier:   ControllersTech.com
  Updated:    27th MAY 2021
  ***************************************************************************************************************
  Copyright (C) 2017 ControllersTech.com
  This is a free software under the GNU license, you can redistribute it and/or modify it
  under the terms of the GNU General Public License version 3 as published by the Free
  Software Foundation. This software library is shared with public for educational
  purposes, without WARRANTY and Author is not liable for any damages caused directly or
  indirectly by this software, read more about this on the GNU General Public License.
  ***************************************************************************************************************
*/

#ifndef INC_FLASH_SECTOR_H7_H_
#define INC_FLASH_SECTOR_H7_H_

#include "stm32h7xx_hal.h"

// If using any other STM32H7 Series
#define FLASH_WORD_SIZE 8

uint32_t Flash_Write_Data(uint32_t StartSectorAddress, uint32_t* data,
                          uint16_t numberofwords);
void Flash_Read_Data(uint32_t StartSectorAddress, uint32_t* data, uint16_t numberofwords);

#endif /* INC_FLASH_SECTOR_H7_H_ */
