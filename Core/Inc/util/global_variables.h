/*
 * global_variables.h
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_VARIABLES_H_
#define INC_GLOBAL_VARIABLES_H_

#include "global_definitions.h"

// updated buffer by the DMA with data read from the ADC
extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

//control variable for disable the torque reference
extern volatile uint8_t g_should_disable_engines;

// variable that holds the parameters of the vehicle's current state
extern volatile vehicle_state_parameters_t g_vehicle_state_parameters;

extern volatile const modos aceleracao;
extern volatile const modos skidpad;
extern volatile const modos autox;
extern volatile const modos enduro;
extern volatile const modos erro;

// stores the vehicle's current state (accelerate, neutral, brake)
extern volatile vehicle_state_e vehicle_state;

#endif /* INC_GLOBAL_VARIABLES_H_ */
