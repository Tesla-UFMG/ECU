/*
 * odometer_save.h
 *
 *  Created on: May 8, 2022
 *      Author: dmroh
 */

#include "stdint.h"

#ifndef INC_DATALOGGING_ODOMETER_SAVE_H_
#define INC_DATALOGGING_ODOMETER_SAVE_H_

#define SAVE_DELAY 30000

#define ODOMETER_DATA_FLASH_ADDR 0x080E0000

#define WORDS_READ_TWO 2

#define MAXIMUM_SAVE_TIMES 20

#define MINIMUM_SAVE_DISTANCE 100

typedef uint32_t odometer_message_t;

enum { TOTAL_DISTANCE = 0, FLASH_WEAR };

#endif /* INC_DATALOGGING_ODOMETER_SAVE_H_ */
