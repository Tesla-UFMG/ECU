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
#define WHEEL_RADIUS              0.26f // in m

#define HIST_SPEED_SIZE 3

#define MIN_SPEED_FOR_COMPENSATION_M_S 5.55f // in m/s

#define MAX_SPEED_VARIATION_M_S        5.55f // in m/s

#define MAX_SPEED_KM_H                  800 // in km/h*10
#define MIN_SPEED_KM_H                  100  // in km/h*10

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
