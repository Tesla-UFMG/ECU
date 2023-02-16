/*
 * leds.h
 *
 *  Created on: May 13, 2021
 *      Author: Felipe Telles
 */

#ifndef INC_RGB_LED_H_
#define INC_RGB_LED_H_

#include "cmsis_os.h"
#include "main.h"
#include "stdint.h"
#include "util/global_definitions.h"

#define RGB_BLINK_DELAY 200

#define RGB_BLACK                                                                        \
    (rgb_t) {                                                                              \
        0, 0, 0                                                                          \
    }
#define RGB_RED                                                                          \
    (rgb_t) {                                                                              \
        1, 0, 0                                                                          \
    }
#define RGB_GREEN                                                                        \
    (rgb_t) {                                                                              \
        0, 1, 0                                                                          \
    }
#define RGB_BLUE                                                                         \
    (rgb_t) {                                                                              \
        0, 0, 1                                                                          \
    }
#define RGB_YELLOW                                                                       \
    (rgb_t) {                                                                              \
        1, 1, 0                                                                          \
    }
#define RGB_PURBLE                                                                       \
    (rgb_t) {                                                                              \
        1, 0, 1                                                                          \
    }
#define RGB_CYAN                                                                         \
    (rgb_t) {                                                                              \
        0, 1, 1                                                                          \
    }
#define RGB_WHITE                                                                        \
    (rgb_t) {                                                                              \
        1, 1, 1                                                                          \
    }

typedef enum { FIXED, BLINK200, NO_CHANGE, BLINK500 } control_rgb_led_e;

typedef struct {
    cores_t color;
    control_rgb_led_e control;
} rgb_led_message_t;

typedef struct {
    GPIO_PinState red;
    GPIO_PinState green;
    GPIO_PinState blue;
} rgb_t;

osStatus_t set_rgb_led(cores_t color, control_rgb_led_e control);

#endif /* INC_RGB_LED_H_ */
