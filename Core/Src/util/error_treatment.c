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
        // Sets the thread flag and the event flag with flagError
        issue_error(flagError, /*Should_set_control_event_flag=*/true);
    } else {
        clear_error(flagError);
    }
}

void check_for_errors_with_timeout(bool (*areThereErrors)(), uint32_t flagError,
                                   osTimerId_t timerHandler, uint16_t timerAmount) {
    if (areThereErrors()) {
        // Starts the timer only if it has not already been started, so that the timer
        // isn't restarted.
        if (!osTimerIsRunning(timerHandler)) {
            osTimerStart(timerHandler, timerAmount / portTICK_PERIOD_MS);
        }
    } else {
        // Stops the timer
        osTimerStop(timerHandler);
        clear_error(flagError);
    }
}

void errors_with_timer_callback(void* argument) {
    // Get the flag from callback argument
    uint32_t flagError = (uint32_t)argument;
    // Sets the thread flag and the event flag with flagError
    issue_error(flagError, /*should_set_control_event_flag=*/true);
}

void issue_error(uint32_t flagError, bool should_set_control_event_flag) {
    // Sets main_task.c thread flag with flagError
    osThreadFlagsSet(t_main_taskHandle, flagError);
    if (should_set_control_event_flag) {
        // Sets the event flag with flagError
        osEventFlagsSet(e_ECU_control_flagsHandle, flagError);
    }
}

void clear_error(uint32_t flagError) {
    osEventFlagsClear(e_ECU_control_flagsHandle, flagError);
}
