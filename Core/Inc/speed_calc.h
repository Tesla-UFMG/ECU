/*
 * speed_calc.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_SPEED_CALC_H_
#define INC_SPEED_CALC_H_

#include "stdint.h"

#define SPEED_SENSOR_TEETH_QUAN 16  // quantidade de dentes da roda fônica
#define WHEEL_RADIUS 0.26           // em m
#define MAX_SPEED 2000              // em km/h*10
#define MIN_SPEED 5	                // em km/h*10

#define FIRST_WHEEL FRONT_RIGHT
#define LAST_WHEEL FRONT_LEFT       //ultima roda que possui sensor de roda fônica

typedef enum {
	FRONT_RIGHT,
	FRONT_LEFT,
	REAR_RIGHT,
	REAR_LEFT
} speed_pin_e;

typedef struct {
	uint32_t tim_count;
	speed_pin_e pin;
} speed_message_t;



#endif /* INC_SPEED_CALC_H_ */
