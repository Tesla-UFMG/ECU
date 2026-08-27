/*
 * encoder_acceleration.h
 *
 *  Created on: August 26, 2026
 *      Author: Enzo Ribeiro
 */

#ifndef INC_ACCELERATION_CALC_H_
#define INC_ACCELERATION_CALC_H_
#include "util/global_definitions.h"
#include "datalogging/speed.h"
#include "sensors/encoder_speed.h"
speed_pin_e wheelCaught(encoder_speeds_message_t vel);
typedef struct{
    float wheels[WHEEL_ENCODERS_AVAILABLE];
}encoder_acceleration_message_t;
#endif