/*
 * eeprom.h
 *
 *  Created on: 27 de mar de 2018
 *      Author: User
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "flash.h"
#include "stm32f1xx_hal.h"

/* EEPROM start address in Flash */

#define PAGE_SIZE  (uint16_t)0x400  /* Page size = 1KByte */


#define EEPROM_START_ADDRESS    ((uint32_t)0x08010000) /* EEPROM emulation start address:
                                                  after 64KByte of used Flash memory */

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + 0x000))
#define PAGE0_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

/* Used Flash pages for EEPROM emulation */
#define PAGE0                   ((uint16_t)0x0000)
#define PAGE1                   ((uint16_t)0x0001)

/* No valid page define */
#define NO_VALID_PAGE           ((uint16_t)0x00AB)

/* Page status definitions */
#define ERASED                  ((uint16_t)0xFFFF)     /* PAGE is empty */
#define RECEIVE_DATA            ((uint16_t)0xEEEE)     /* PAGE is marked to receive data */
#define VALID_PAGE              ((uint16_t)0x0000)     /* PAGE containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE    ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE     ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL               ((uint8_t)0x80)

/* Variables' number */
#define NumbOfVar               ((uint8_t)0x03)

enum error_type {TxError = 0, RxError, Overflow};
extern uint16_t write_status;

uint16_t EE_Init(void);
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data);

void Error_Diagnostics_Read();							// reads and copies to transmittable vector
void Error_Diagnostics_Write(CAN_HandleTypeDef* hcan);	// detects and writes errors in both current and permanent addr tables
void Error_Diagnostics_Clear();							// resets counters and storage
void Update_Test_Counter();								// updates counter after RTD enabled
uint16_t Get_Traveled_Distance();						// gets current traveled distance
void Record_Distance(uint16_t dist_log[]);				// records in test odometer and overall odometer
void inline Clear_Test_Distance() { write_status = EE_WriteVariable(0x7777, 0); };

#endif /* EEPROM_H_ */
