/*
 * leds.h
 *
 *  Created on: May 21, 2023
 *      Author: Samuel Dantas
 */

#ifndef INC_DEBUGLEDS_H_
#define INC_DEBUGLEDS_H_

#include "cmsis_os.h"
#include "main.h"
#include "stdint.h"

#define DEBUGLED1 C_LED_DEBUG1_Pin
#define DEBUGLED2 C_LED_DEBUG2_Pin
#define DEBUGLED3 C_LED_DEBUG3_Pin

typedef enum { ON, OFF, TOGGLE, BLINK, FASTBLINK } ControlDebugLED_e;

typedef struct {
    uint16_t led_number;
    ControlDebugLED_e control;
    uint8_t amount;
} debug_led_message_t;

osStatus_t set_debugleds(uint16_t lednumber, ControlDebugLED_e control, uint8_t amount);

#endif /* INC_DEBUGLEDS_H_ */
