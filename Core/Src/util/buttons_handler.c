/*
 * buttons_handler.c
 *
 *  Created on: 20 de mar de 2022
 *      Author: Felipe Telles
 */

#include "util/buttons_handler.h"

#include "leds/rgb_led_handler.h"
#include "main.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

void long_button_press_handler(available_buttons_e button);
void button_release_handler(available_buttons_e button);
void initialize_buttons();

static buttons_parameters_t buttons[BUTTONS_QUAN];

void button_release_handler(available_buttons_e button) {
    switch (button) {
        case B_RTD:
            osThreadFlagsSet(t_RTDHandle, RTD_BTN_PRESSED_THREAD_FLAG);
            osThreadFlagsSet(t_pilot_resetHandle, RTD_BTN_PRESSED_THREAD_FLAG);
            break;

        case B_MODE:;
            RACE_MODE_t race_mode = get_global_var_value(RACE_MODE);
            set_global_var_value(RACE_MODE, (RACE_MODE_t)(race_mode + 1));
            osThreadFlagsSet(t_seleciona_modoHandle, MODE_BTN_PRESSED_THREAD_FLAG);
            osThreadFlagsSet(t_pilot_resetHandle, MODE_BTN_PRESSED_THREAD_FLAG);
            break;

        case B_DYNAMICS_CONTROLS:
            osThreadFlagsSet(t_dynamic_controls_choiceHandle,
                             DYNAMIC_CONTROLS_CHOICE_BTN_PRESSED_THREAD_FLAG);
            break;

        default: break;
    }
}

void long_button_press_handler(available_buttons_e button) {
    uint32_t error_flags = osEventFlagsGet(e_ECU_control_flagsHandle);
    error_flags &= ALL_SEVERE_ERROR_FLAG;
    RACE_MODE_t race_mode = get_global_var_value(RACE_MODE);
    bool is_inverter_ready =
        get_individual_flag(e_ECU_control_flagsHandle, INVERTER_READY_FLAG);
    THROTTLE_STATUS_t is_throttle_active = get_global_var_value(THROTTLE_STATUS);
    switch (button) {
        case B_RTD:
            if (!error_flags && (race_mode != ERRO) && is_inverter_ready
                && !is_throttle_active) {
                osEventFlagsSet(e_ECU_control_flagsHandle, RTD_FLAG);
                set_rgb_led(get_global_var_value(SELECTED_MODE).cor, FIXED);
            }
            break;
            // case B_MODE: break;
            // case B_DYNAMICS_CONTROLS: break;
        default: break;
    }
}

void buttons_handler(void* argument) {
    UNUSED(argument);

    // initialize the buttons with the desired configuration
    initialize_buttons();

    available_buttons_e current;

    for (;;) {

        // loops to read the state of every button initialized
        for (current = 0; current < BUTTONS_QUAN; current++) {
            // if the pin is set increases the count by one
            if (HAL_GPIO_ReadPin(buttons[current].port, buttons[current].pin)
                == GPIO_PIN_SET) {
                buttons[current].setCounter++;
                // to prevent bounce the button is only considered set after the
                // debounce time has passed
                if (buttons[current].setCounter == DEBOUNCE_TIME) {
                    buttons[current].state = BUTTON_PRESSED;
                    // handles a long press if its enabled on the button
                    // initialization and after the configured timer has passed
                } else if (buttons[current].setCounter == LONG_PRESS_TIME
                           && buttons[current].enableLongPress) {
                    buttons[current].state = BUTTON_LONG_PRESSED;
                    long_button_press_handler(current);
                }
                // if the pin is not set the counter will be reseted
            } else {
                buttons[current].setCounter = 0;
                // if the previous button state was pressed or long pressed the state
                // will be updated to no pressed
                if (buttons[current].state != BUTTON_NOT_PRESSED) {
                    // handles a release if its not a long press
                    if (buttons[current].state == BUTTON_PRESSED) {
                        button_release_handler(current);
                    }
                    buttons[current].state = BUTTON_NOT_PRESSED;
                }
            }
        }
        osDelay(POLLING_TIME);
    }
}

/**
 * @brief initialize the ECU buttons parameters. .pin and .port are specific to the pin
 * and defined in main.h. .setCounter and .state must be initialized as zero.
 * .enableLongPress must be 0 if no long press action is configured or 1 if action is
 * configured
 *
 */
void initialize_buttons() {
    buttons[B_RTD] = (buttons_parameters_t){.pin             = B_RTD_Pin,
                                            .port            = B_RTD_GPIO_Port,
                                            .setCounter      = 0,
                                            .state           = BUTTON_NOT_PRESSED,
                                            .enableLongPress = 1};

    buttons[B_MODE] = (buttons_parameters_t){.pin             = B_MODE_Pin,
                                             .port            = B_MODE_GPIO_Port,
                                             .setCounter      = 0,
                                             .state           = BUTTON_NOT_PRESSED,
                                             .enableLongPress = 0};

    buttons[B_DYNAMICS_CONTROLS] =
        (buttons_parameters_t){.pin             = B_DYNAMICS_CONTROLS_Pin,
                               .port            = B_DYNAMICS_CONTROLS_GPIO_Port,
                               .setCounter      = 0,
                               .state           = BUTTON_NOT_PRESSED,
                               .enableLongPress = 0};
}
