/*
 * odometer_save.h
 *
 *  Created on: May 8, 2022
 *      Author: dmroh
 */

#ifndef INC_DATALOGGING_ODOMETER_SAVE_H_
#define INC_DATALOGGING_ODOMETER_SAVE_H_

// Thread delay time
#define SAVE_DELAY 30000

// FLash saving memory address
#define FLASH_ADDR 0x080E0000

// Word to be read
#define TWO_WORDS 2

// Maximum saves each time ECU is turned on
#define MAX_SAVE_TIMES 20

// Minimum distance to save
#define METERS_100 100

enum { TOTAL_DISTANCE, FLASH_WEAR };

#endif /* INC_DATALOGGING_ODOMETER_SAVE_H_ */
