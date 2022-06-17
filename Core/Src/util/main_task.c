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
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

void main_task(void* argument) {
    UNUSED(argument);

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        wait_for_rtd();

        // wait for any error
        osThreadFlagsWait(ALL_ERRORS_FLAG, osFlagsWaitAny | osFlagsNoClear,
                          osWaitForever);
        // get the thread and event flags
        uint32_t error_flags = osThreadFlagsGet();
        uint32_t event_flags = osEventFlagsGet(e_ECU_control_flagsHandle);
        // get the most significant thread flag. then treats it
        uint32_t most_significant_error_flags =
            get_flag_MSB(error_flags & ALL_ERRORS_FLAG);
        bool isErrorPresent;
        switch (most_significant_error_flags) {

                // the inverter can busoff error will be treated if it happens more than
                // once in a short period of time (period defined by: BUS_OFF_ERROR_TIME),
                // this way the car leaves RTD mode only if there is a frequent CAN error.

            case INVERTER_BUS_OFF_ERROR_FLAG:
                // verifies if the error is set(exists) in the event flag. if it is the
                // car leaves RTD mode
                isErrorPresent = event_flags & INVERTER_BUS_OFF_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                } else {
                    // if the error is not set(doesnt exist) in the event flag a timer
                    // will be initialized. the error is checked again with the timer so
                    // the car leaves RTD mode if the error becomes present again.
                    osEventFlagsSet(e_ECU_control_flagsHandle,
                                    INVERTER_BUS_OFF_ERROR_FLAG);
                    osThreadFlagsClear(INVERTER_BUS_OFF_ERROR_FLAG);
                    osTimerStart(tim_inverter_BUS_OFF_errorHandle, BUS_OFF_ERROR_TIME);
                }
                break;

            case INVERTER_CAN_TRANSMIT_ERROR_FLAG:

                // verifies if the error is set(exists) in the event flag. if it is the
                // car leaves RTD mode a timer of 75 ms is used to check constant errors
                // so the car only leaves RTD mode after multiple occurrences
                isErrorPresent = event_flags & INVERTER_CAN_TRANSMIT_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                } else {
                    // if the error is not set(doesnt exist) in the event flag a timer
                    // will be initialized. the error is checked again with the timer so
                    // the car leaves RTD mode if the error becomes present again.
                    osThreadFlagsClear(INVERTER_CAN_TRANSMIT_ERROR_FLAG);
                }
                break;

            case INVERTER_COMM_ERROR_FLAG:
                // verifies if the error is still set in the event flag. if it is
                // the car leaves RTD mode .if it is not the thread flag is cleared

                isErrorPresent = event_flags & INVERTER_COMM_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                } else {
                    osThreadFlagsClear(INVERTER_COMM_ERROR_FLAG);
                }
                break;

            case SU_F_ERROR_FLAG:
                // verifies if the error is still set in the event flag. if it is
                // the car leaves RTD mode. if it is not the thread flag is cleared and
                //  the led returns to normal
                isErrorPresent = event_flags & SU_F_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                } else {
                    osThreadFlagsClear(SU_F_ERROR_FLAG);
                }
                break;

            case APPS_ERROR_FLAG: // Regulamento: T.4.2 (2021)

                isErrorPresent = event_flags & APPS_ERROR_FLAG;
                if (isErrorPresent) {
                    set_rgb_led(AMARELO, NO_CHANGE);
                    osDelay(20);
                } else {
                    osThreadFlagsClear(APPS_ERROR_FLAG);
                    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, NO_CHANGE);
                }
                break;

            case BSE_ERROR_FLAG: // Regulamento: EV.5.7 (2021)
                // verifies if the error is still set in the event flag. if it is
                // sets the led to yellow. if it is not the thread flag is cleared
                // and the led returns to normal
                isErrorPresent = event_flags & BSE_ERROR_FLAG;
                if (isErrorPresent) {
                    set_rgb_led(AMARELO, NO_CHANGE);
                    osDelay(20);
                } else {
                    osThreadFlagsClear(BSE_ERROR_FLAG);
                    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, NO_CHANGE);
                }
                break;

            default: osDelay(100); break;
        }
    }
}
