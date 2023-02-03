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
void left_inv_error_callback();
void right_inv_error_callback();
void inverter_can_diff(uint32_t id);

static bool left_inv_received = 0;
static bool right_inv_received = 0;
static FDCAN_RxHeaderTypeDef RxHeader;


void inverter_comm_error(void* argument) {
    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        uint32_t id = RxHeader.Identifier; //iniciei com esse valor pois está dando o erro " 'id' undeclared "

        switch (osMessageQueueGet(q_ids_can_inverterHandle, &id, NULL, osWaitForever)) {

            default:
                clear_error(INVERTER_COMM_ERROR_FLAG); //manter?
                precharge_monitor();				   //manter?

            	// differentiates CAN signals from both inverters
            	// and indicates if those are ready
            	inverter_can_diff(id);

                break;
        }
    }
}

void set_inverter_communication_error() {
    // alert the main_task that the error is present
    issue_error(INVERTER_COMM_ERROR_FLAG, /*should_set_control_event_flag=*/true);
    // reset the flag that indicates if the inverter is ready
    osEventFlagsClear(e_ECU_control_flagsHandle, INVERTER_READY);
    // stop the timer that sets the flag
    osTimerStop(tim_inverter_readyHandle);
}

void inverter_can_diff(uint32_t id){
	// redundancy for setting the flags
	if (id  >= 100 && id <= 103)	{
		left_inv_received = 1;
	}
	if (id  >= 200 && id <= 203)	{
		right_inv_received = 1;
	}

	// indicates whether the car is ready
	if	((!LEFT_INVERTER_CAN_ERROR) && (!RIGHT_INVERTER_CAN_ERROR) &&
	   (right_inv_received == 1) && (left_inv_received == 1))	{
		// set the flag indicating the inverter is ready
		osEventFlagsSet(e_ECU_control_flagsHandle, INVERTER_READY);
		// set the flag indicating the inverter CAN is active
		osThreadFlagsSet(t_inverter_comm_errorHandle, INVERTER_CAN_ACTIVE);
		// stop the timer that sets both flags
		osTimerStop(tim_left_inv_errorHandle);
		osTimerStop(tim_right_inv_errorHandle);
	}
}

void left_inv_error_callback()	{
		// alert the main_task that the error is present
	    issue_error(LEFT_INVERTER_CAN_ERROR, /*should_set_control_event_flag=*/true);
	    // reset the flag that indicates if the inverter is ready
	    osEventFlagsClear(e_ECU_control_flagsHandle, INVERTER_READY);
}

void right_inv_error_callback()	{
	// alert the main_task that the error is present
		    issue_error(RIGHT_INVERTER_CAN_ERROR, /*should_set_control_event_flag=*/true);
		    // reset the flag that indicates if the inverter is ready
		    osEventFlagsClear(e_ECU_control_flagsHandle, INVERTER_READY);
}


void precharge_monitor() {
    // start the timer only when the flag is reseted and the timer is not alredy
    // running to avoid restarting the timer
    if (!get_individual_flag(e_ECU_control_flagsHandle, INVERTER_READY)) {
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
    osEventFlagsSet(e_ECU_control_flagsHandle, INVERTER_READY);
}
