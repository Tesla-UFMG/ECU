/*
 * speed_calc.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_SPEED_CALC_H_
#define INC_SPEED_CALC_H_

#include "stdint.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal_rcc.h"
#include "math.h"

#define SPEED_SENSOR_TEETH_QUAN 16

typedef enum {
	FRONT_RIGHT,
	FRONT_LEFT,
	BACK_RIGHT,
	BACK_LEFT
} speed_pin_e;

typedef struct {
	uint32_t tim_count;
	speed_pin_e pin;
} speed_message_t;



#endif /* INC_SPEED_CALC_H_ */
