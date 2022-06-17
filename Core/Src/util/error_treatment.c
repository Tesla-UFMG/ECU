/*
 * error_treatment.c
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */

#include "util/error_treatment.h"

#include "util/global_instances.h"

void check_for_errors(bool (*areThereErrors)(), uint32_t flagError) {
    if (areThereErrors()) {
        // sets the thread flag and the event flag with flagError
        issue_error(flagError, /*should_set_control_event_flag=*/true);
    } else {
        clear_error(flagError);
    }
}

void check_for_errors_with_timeout(bool (*areThereErrors)(), uint32_t flagError,
                                   osTimerId_t timerHandler, uint16_t timerAmount) {
    if (areThereErrors()) {
        // if the timer is not running it is initialized. the if protects against a timer
        // reset
        if (!osTimerIsRunning(timerHandler)) {
            osTimerStart(timerHandler, timerAmount / portTICK_PERIOD_MS);
        }
    } else {
        // stops the timer
        osTimerStop(timerHandler);
        clear_error(flagError);
    }
}

void errors_with_timer_callback(void* argument) {
    // get the flag from callback argument
    uint32_t flagError = (uint32_t)argument;
    // sets the thread flag and the event flag with flagError
    issue_error(flagError, /*should_set_control_event_flag=*/true);
}

void issue_error(uint32_t flagError, bool should_set_control_event_flag) {
    // sets main_task.c thread flag with flagError
    osThreadFlagsSet(t_main_taskHandle, flagError);
    if (should_set_control_event_flag) {
        // sets the event flag with flagError
        osEventFlagsSet(e_ECU_control_flagsHandle, flagError);
    }
}

void clear_error(uint32_t flagError) {
    osEventFlagsClear(e_ECU_control_flagsHandle, flagError);
}
