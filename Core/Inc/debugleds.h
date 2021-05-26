/*
 * leds.h
 *
 *  Created on: May 13, 2021
 *      Author: Felipe Telles
 */

#ifndef INC_DEBUGLEDS_H_
#define INC_DEBUGLEDS_H_

#include "global_variables.h"
#include "global_definitions.h"
#include "global_instances.h"
#include "stdint.h"
#include "main.h"

typedef enum {
	ON,
	OFF,
	TOGGLE,
	BLINK,
} ControlDebugLED_e;


typedef struct {
	uint16_t lednumber;
	ControlDebugLED_e control;
	uint8_t amount;
} debugled_message_t;


#endif /* INC_DEBUGLEDS_H_ */
