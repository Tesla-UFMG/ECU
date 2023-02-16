/*
 * inverter_can_error_manager.c
 *
 *  Created on: Jan 5, 2022
 *      Author: Felipe Telles
 */

#include "CAN/inverter_can_monitor.h"

#include "util/error_treatment.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

void set_inverter_communication_error();
void precharge_monitor();

void inverter_comm_error(void* argument) {
    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        switch (osThreadFlagsWait(INVERTER_CAN_ACTIVE_THREAD_FLAG, osFlagsWaitAny,
                                  INVERTER_NO_MESSAGE_ERROR_TIME)) {

            // when the task has been called due to timeout it means the ECU is not
            // receiving messages from the inverter
            case osFlagsErrorTimeout: set_inverter_communication_error(); break;

            // when the task has been called by the thread flag it means the ECU is
            // communicating with the inverter
            default:
                clear_error(INVERTER_COMM_ERROR_FLAG);
                precharge_monitor();
                break;
        }
    }
}

void set_inverter_communication_error() {
    // alert the main_task that the error is present
    issue_error(INVERTER_COMM_ERROR_FLAG, /*should_set_control_event_flag=*/true);
    // reset the flag that indicates if the inverter is ready
    osEventFlagsClear(e_ECU_control_flagsHandle, INVERTER_READY_THREAD_FLAG);
    // stop the timer that sets the flag
    osTimerStop(tim_inverter_readyHandle);
}

void precharge_monitor() {
    // start the timer only when the flag is reseted and the timer is not alredy
    // running to avoid restarting the timer
    if (!get_individual_flag(e_ECU_control_flagsHandle, INVERTER_READY_THREAD_FLAG)) {
        if (!osTimerIsRunning(tim_inverter_readyHandle)) {
            // timer to set a flag to indicate when the inverter is ready after the
            // precharge period
            osTimerStart(tim_inverter_readyHandle, INVERTER_PRECHARGE_TIME);
        }
    }
}

// if no more errors are present the event flag will be cleared
void inverter_BUS_OFF_error_callback(void* argument) {
    UNUSED(argument);
    clear_error(INVERTER_BUS_OFF_ERROR_FLAG);
}

// the flag wil be setted after the precharge time has passed
void inverter_ready_callback(void* argument) {
    UNUSED(argument);
    osEventFlagsSet(e_ECU_control_flagsHandle, INVERTER_READY_THREAD_FLAG);
}
