/*
 * inverter_can_error_manager.c
 *
 *  Created on: Jan 5, 2022
 *      Author: Felipe Telles
 */

#include <CAN/inverter_can_monitor.h>
#include "global_definitions.h"
#include "global_instances.h"
#include "error_treatment.h"
#include "util.h"

void set_inverter_communication_error();
void precharge_monitor();

void inverter_comm_error(void *argument) {

    for(;;) {
        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif


        switch(osThreadFlagsWait(INVERTER_CAN_ACTIVE, osFlagsWaitAny, INVERTER_NO_MESSAGE_ERROR_TIME)){

            //when the task has been called by the timeout it means the ECU is not receiving messages from the inverter
            case osFlagsErrorTimeout :
                set_inverter_communication_error();
                break;

            // when the task has been called by the thread flag it means the ECU is communicating with the inverter
            default:
                clear_error(INVERTER_COMM_ERROR_FLAG);
                precharge_monitor();
                break;
        }
    }
}

void set_inverter_communication_error() {
    issue_error(INVERTER_COMM_ERROR_FLAG, /*should_set_control_event_flag=*/true);      // alert the main_task that the error is present
    osEventFlagsClear(ECU_control_event_id, INVERTER_READY);                            // reset the flag that indicates if the inverter is ready
    osTimerStop(tim_inverter_readyHandle);                                              // stop the timer that sets the flag
}

void precharge_monitor() {
    if (!get_individual_flag(ECU_control_event_id, INVERTER_READY)) { // start the timer only when the flag is reseted and the timer is not running to avoid restarting the timer
        if (!osTimerIsRunning(tim_inverter_readyHandle)) {
            osTimerStart(tim_inverter_readyHandle, INVERTER_PRECHARGE_TIME); // timer to set a flag to indicate when the inverter is ready after the precharge period.
        }
    }
}

void inverter_BUS_OFF_error_callback(void *argument) {
    clear_error(INVERTER_BUS_OFF_ERROR_FLAG);                   // if no more errors are present the event flag will be cleared
}

void inverter_ready_callback(void *argument) {
    osEventFlagsSet(ECU_control_event_id, INVERTER_READY);      // the flag wil be setted after the precharge time has passed
}
