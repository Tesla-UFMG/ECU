/*
 * main_task.c
 *
 *  Created on: Jun 3, 2020
 *      Author: renanmoreira
 */

#include "util/main_task.h"

#include "cmsis_os.h"
#include "driver_settings/RTD.h"
#include "leds/debug_leds_handler.h"
#include "leds/rgb_led_handler.h"
#include "sensors/APPS.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

void led_color_response(uint32_t flag) {

    switch (flag) {
        // Soft error
        case BSE_ERROR_FLAG: {
            colors_t pattern[SOFT_ERROR_COLORS_PATTERN_SIZE] = {YELLOW, PURPLE};
            set_rgb_led(pattern, SOFT_ERROR_COLORS_PATTERN_SIZE, BLINK200);
            break;
        }
        case APPS_ERROR_FLAG: {
            colors_t pattern[SOFT_ERROR_COLORS_PATTERN_SIZE] = {YELLOW, BLUE};
            set_rgb_led(pattern, SOFT_ERROR_COLORS_PATTERN_SIZE, BLINK200);
            break;
        }
        // Hard error
        case INVERTER_CAN_TRANSMIT_ERROR_FLAG: {
            colors_t pattern[HARD_ERROR_COLORS_PATTERN_SIZE] = {RED, BLUE, YELLOW};
            set_rgb_led(pattern, HARD_ERROR_COLORS_PATTERN_SIZE, FIXED);
            break;
        }
        case SU_F_ERROR_FLAG: {
            colors_t pattern[HARD_ERROR_COLORS_PATTERN_SIZE] = {RED, WHITE, BLACK};
            set_rgb_led(pattern, HARD_ERROR_COLORS_PATTERN_SIZE, FIXED);
            break;
        }
        case INVERTER_BUS_OFF_ERROR_FLAG: {
            colors_t pattern[HARD_ERROR_COLORS_PATTERN_SIZE] = {RED, BLUE, GREEN};
            set_rgb_led(pattern, HARD_ERROR_COLORS_PATTERN_SIZE, FIXED);
            break;
        }
        case LEFT_INVERTER_COMM_ERROR_FLAG: {
            colors_t pattern[HARD_ERROR_COLORS_PATTERN_SIZE] = {RED, BLUE, PURPLE};
            set_rgb_led(pattern, HARD_ERROR_COLORS_PATTERN_SIZE, FIXED);
            break;
        }
        case RIGHT_INVERTER_COMM_ERROR_FLAG: {
            colors_t pattern[HARD_ERROR_COLORS_PATTERN_SIZE] = {RED, BLUE, CYAN};
            set_rgb_led(pattern, HARD_ERROR_COLORS_PATTERN_SIZE, FIXED);
            break;
        }
    }
}
int test_aux = 500;
uint32_t oldValue;

uint32_t test(int test_aux, uint32_t* oldValue) {
    uint32_t error_vec[3] = {BSE_ERROR_FLAG, INVERTER_CAN_TRANSMIT_ERROR_FLAG,
                             RIGHT_INVERTER_COMM_ERROR_FLAG};
    if (test_aux % 500 == 0 && test_aux < 1501) {
        *oldValue = error_vec[(test_aux / 500) - 1];
        return error_vec[(test_aux / 500) - 1];
    } else {
        return *oldValue;
    }
}

void main_task(void* argument) {

    UNUSED(argument);

    for (;;) {

        // ECU_ENABLE_BREAKPOINT_DEBUG();

        // wait_for_rtd();

        // Wait for any error
        osThreadFlagsWait(ALL_ERRORS_FLAG, osFlagsWaitAny | osFlagsNoClear,
                          osWaitForever);
        // Get the most significant thread flag
        uint32_t most_significant_error_flag =
            test(test_aux, &oldValue); // get_most_significant_thread_flag();
        // Get the event flag
        uint32_t event_flags =
            test(test_aux, &oldValue); // osEventFlagsGet(e_ECU_control_flagsHandle);
        test_aux++;
        bool isErrorPresent;
        switch (most_significant_error_flag) {

                // BUSOFF error is treated when it happens more than once in a short
                // period of time (period defined by: BUS_OFF_ERROR_TIME).The car leaves
                // RTD mode only if there is a frequent CAN error.
            case INVERTER_BUS_OFF_ERROR_FLAG:

                // If the event flag contains the error flag the car leaves RTD mode.
                isErrorPresent = event_flags & INVERTER_BUS_OFF_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                    led_color_response(most_significant_error_flag);
                } else {
                    // Starts a timer. If the error happens again before the timer expires
                    // the car leaves RTD mode.
                    osEventFlagsSet(e_ECU_control_flagsHandle,
                                    INVERTER_BUS_OFF_ERROR_FLAG);
                    osThreadFlagsClear(INVERTER_BUS_OFF_ERROR_FLAG);
                    osTimerStart(tim_inverter_BUS_OFF_errorHandle, BUS_OFF_ERROR_TIME);
                }
                break;

            case INVERTER_CAN_TRANSMIT_ERROR_FLAG:
            case LEFT_INVERTER_COMM_ERROR_FLAG:
            case RIGHT_INVERTER_COMM_ERROR_FLAG:
            case SU_F_ERROR_FLAG:
                // If the event flag contains the error flag the car leaves RTD mode.
                isErrorPresent = event_flags & most_significant_error_flag;
                if (isErrorPresent) {
                    exit_RTD();
                    led_color_response(most_significant_error_flag);
                } else {
                    // Clear the thread flag
                    osThreadFlagsClear(most_significant_error_flag);
                }
                break;

            case APPS_ERROR_FLAG: // FSAE Rules: T.4.2 (2021)
            case BSE_ERROR_FLAG:  // FSAE Rules: EV.5.7 (2021)

                // If the event flag contains the error flag ECU led is set to yellow
                isErrorPresent = event_flags & most_significant_error_flag;
                if (isErrorPresent) {
                    led_color_response(most_significant_error_flag);
                    osDelay(20);
                } else {
                    // Clear the thread flag and set ECU led to normal
                    osThreadFlagsClear(most_significant_error_flag);
                    set_rgb_led(get_global_var_value(SELECTED_MODE).rgb_colors, BLINK200,
                                ONE_COLOR_PATTERN_SIZE);
                }
                break;

            default: osDelay(100); break;
        }
    }
}
