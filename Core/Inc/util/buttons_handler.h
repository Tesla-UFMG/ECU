/*
 * buttons_handler.h
 *
 *  Created on: 20 de mar de 2022
 *      Author: Felipe Telles
 */

#ifndef INC_UTIL_BUTTON_HANDLER_H_
#define INC_UTIL_BUTTON_HANDLER_H_

#include "cmsis_os.h"
#include "main.h"

#define POLLING_TIME    (10)
#define DEBOUNCE_TIME   (50 / POLLING_TIME)   // Debounce time in ms
#define LONG_PRESS_TIME (2500 / POLLING_TIME) // long press time in ms

/**
 * @brief enum with all possible buttons states
 *
 */
typedef enum {
    BUTTON_NOT_PRESSED = 0,
    BUTTON_PRESSED,
    BUTTON_LONG_PRESSED
} button_state_e;

/**
 * @brief enum with all buttons available on the ECU
 *
 */
typedef enum {
    B_RTD = 0,
    B_MODE,
    B_DYNAMICS_CONTROLS,

    BUTTONS_QUAN
} available_buttons_e;

/**
 * @brief struct with all buttons parameters.
 *
 */
typedef struct {
    uint16_t pin;
    GPIO_TypeDef* port;
    uint16_t setCounter;
    button_state_e state;
    uint8_t enableLongPress;
} buttons_parameters_t;

#endif /* INC_UTIL_BUTTON_HANDLER_H_ */
