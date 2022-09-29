/**
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************
  File:       FLASH_SECTOR_H7.c
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

#include "util/flash_sector_h7.h"

#include "stdio.h"
#include "stm32h7xx_hal.h"
#include "string.h"

// The default flash word is 32 bytes length or 8 MCU words length
// This means the memory can save only a 256 bit number at a time, not more nor less

// There are 2 BANKS available for H745, BANK 1 (0x0800 0000 - 0x080F FFFF) and BANK 2
// (0x0810 0000 - 0x080F FFFF) Both of them have Sectors 0 to 7.

// The DATA to be written here MUST be according to the List Shown Below
// For EXAMPLE:- For H74x/5x, a single data must be 8 numbers of 32 bits word
// If you try to write a single 32 bit word, it will automatically write 0's for the rest
// 7
//    - 256 bits for STM32H74x/5X devices (8x 32bits words)
//    - 128 bits for STM32H7Ax/BX devices (4x 32bits words)
//    - 256 bits for STM32H72x/3X devices (8x 32bits words)
//

// Function to determine the sector number of a user given address
static uint32_t GetSector(uint32_t Address) {
    uint32_t sector = 0;

    // Checking only if the address is in the last sector
    if ((Address >= 0x080E0000) && (Address < 0x08100000)) {
        sector = FLASH_SECTOR_7;
    }

    return sector;
}

// Function to write data in the flash
uint32_t Flash_Write_Data(uint32_t StartSectorAddress, uint32_t* data,
                          uint16_t numberofwords) {

    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTORError;
    int sofar = 0;

    // Unlock the Flash to enable the flash control register access
    HAL_FLASH_Unlock();

    // Erase the user Flash area

    // Get the number of sector to erase from 1st sector
    uint32_t StartSector      = GetSector(StartSectorAddress);
    uint32_t EndSectorAddress = StartSectorAddress + numberofwords * 4;
    uint32_t EndSector        = GetSector(EndSectorAddress);

    // Fill EraseInit structure
    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector       = StartSector;

    // The the proper BANK to erase the Sector
    if (StartSectorAddress < 0x08100000) {
        EraseInitStruct.Banks = FLASH_BANK_1;
    } else {
        EraseInitStruct.Banks = FLASH_BANK_2;
    }

    EraseInitStruct.NbSectors = (EndSector - StartSector) + 1;

    // Erase the sector entirely
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
        return HAL_FLASH_GetError();
    }

    // Program the user Flash area 8 WORDS at a time
    // (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR)

    // Saving the data
    while (sofar < numberofwords) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, StartSectorAddress,
                              (uint32_t)&data[sofar])
            == HAL_OK) {
            // Sector address goes from 32 to 32 positions, as each saved number is 256
            // bits length
            StartSectorAddress += 4 * FLASH_WORD_SIZE;

            // As it saves 256 bits at a time, jumps from 8 positions of 32 bit words
            sofar += FLASH_WORD_SIZE;
        }

        else {
            // Error occurred while writing data in Flash memory
            return HAL_FLASH_GetError();
        }
    }

    // Lock the Flash to disable the flash control register access (recommended
    // to protect the FLASH memory against possible unwanted operation)
    HAL_FLASH_Lock();

    return 0;
}

// Function to read a MCU word
void Flash_Read_Data(uint32_t StartSectorAddress, uint32_t* data,
                     uint16_t numberofwords) {
    while (1) {

        *data = *(__IO uint32_t*)StartSectorAddress;

        // Jumps 32 bits
        StartSectorAddress += 4;

        //data++;

        if (!(numberofwords--)) {
            break;
        }
    }
}
