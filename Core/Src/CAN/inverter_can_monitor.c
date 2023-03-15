/*
 * inverter_can_error_manager.c
 *
 *  Created on: Jan 5, 2022
 *      Author: Felipe Telles
 */

#include "CAN/inverter_can_monitor.h"

#include "CAN/inverter_can.h"
#include "util/error_treatment.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

void precharge_monitor();
void left_inv_error_callback();
void right_inv_error_callback();
void inverter_can_diff(uint32_t id);

void inverter_comm_error(void* argument) {
    UNUSED(argument);

    // Sets inverter communication error flags at first execution
    osEventFlagsSet(e_ECU_control_flagsHandle, LEFT_INVERTER_COMM_ERROR_FLAG);
    osEventFlagsSet(e_ECU_control_flagsHandle, RIGHT_INVERTER_COMM_ERROR_FLAG);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        uint32_t id;

        if (osMessageQueueGet(q_ids_can_inverterHandle, &id, NULL, osWaitForever)
            == osOK) {

            inverter_can_diff(id);
        }
    }
}

void inverter_can_diff(uint32_t id) {

    // Restart the timer and clear the error if any message on each inverter
    // gets received
    if (id >= LEFT_INV_SMALLEST_ID && id <= LEFT_INV_BIGGEST_ID) {
        osTimerStart(tim_left_inv_errorHandle, INV_COMM_ERROR_TIME);
        // clear_error(LEFT_INVERTER_COMM_ERROR_FLAG);
    }
    if (id >= RIGHT_INV_SMALLEST_ID && id <= RIGHT_INV_BIGGEST_ID) {
        osTimerStart(tim_right_inv_errorHandle, INV_COMM_ERROR_TIME);
        // clear_error(RIGHT_INVERTER_COMM_ERROR_FLAG);
    }

    // Calls the precharge_monitor() if there is no error flags
    if (!get_individual_flag(e_ECU_control_flagsHandle, LEFT_INVERTER_COMM_ERROR_FLAG)
        && !get_individual_flag(e_ECU_control_flagsHandle,
                                RIGHT_INVERTER_COMM_ERROR_FLAG)) {
        precharge_monitor();
    }
}

// Callback function set if ECU does not receive any message from the left inverter
// for 500ms
void left_inv_error_callback() {

    // issue_error(LEFT_INVERTER_COMM_ERROR_FLAG, /*should_set_control_event_flag=*/true);

    osEventFlagsClear(e_ECU_control_flagsHandle, INVERTER_READY_THREAD_FLAG);

    osTimerStop(tim_inverter_readyHandle);
}

// Callback function set if ECU does not receive any message from the right inverter
// for 500ms
void right_inv_error_callback() {

    // issue_error(RIGHT_INVERTER_COMM_ERROR_FLAG,
    // /*should_set_control_event_flag=*/true);

    osEventFlagsClear(e_ECU_control_flagsHandle, INVERTER_READY_THREAD_FLAG);

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
    // clear_error(INVERTER_BUS_OFF_ERROR_FLAG);
}

// the flag wil be setted after the precharge time has passed
void inverter_ready_callback(void* argument) {
    UNUSED(argument);
    osEventFlagsSet(e_ECU_control_flagsHandle, INVERTER_READY_THREAD_FLAG);
}
