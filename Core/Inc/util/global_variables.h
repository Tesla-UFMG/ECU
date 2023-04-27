/*
 * global_variables.h
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_VARIABLES_H_
#define INC_GLOBAL_VARIABLES_H_

#include "global_definitions.h"

// buffer updated by DMA with data read from ADC
extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

// control variable to disable torque reference
extern volatile uint8_t g_should_disable_engines;

// variable that stores parameters of the current state of the vehicle
extern volatile vehicle_state_parameters_t g_vehicle_state_parameters;

extern modes acceleration, skidpad, autox, enduro, reverse, error;

// store the current state of the vehicle (accelerate, neutral, brake)
extern volatile vehicle_state_e vehicle_state;

#endif /* INC_GLOBAL_VARIABLES_H_ */
