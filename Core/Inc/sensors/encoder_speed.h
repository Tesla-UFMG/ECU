/*
 * encoder_speed.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_SPEED_CALC_H_
#define INC_SPEED_CALC_H_

#include "stdint.h"
#include "util/global_definitions.h"

#define SPEED_SENSOR_TEETH_NUMBER 16   // number of teeth on the phonic wheel front wheel
#define SPEED_SENSOR_TEETH_NUMBER_REAR 24 //number of teeth on the phonic wheel rear wheel
#define WHEEL_RADIUS              0.26 // in m
#define MAX_SPEED                 2000 // in km/h*10
#define MIN_SPEED                 10   // in km/h*10

#define FIRST_WHEEL FRONT_RIGHT
#define SECOND_WHEEL REAR_RIGHT
#define THIRD_WHEEL REAR_LEFT
#define LAST_WHEEL  FRONT_LEFT // last wheel with an encoder

// Some compilers do not implement by default math defines, so it's better by implement by
// ourselves here
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    uint32_t tim_count;
    speed_pin_e pin;
} encoder_int_message_t;


#endif /* INC_SPEED_CALC_H_ */
