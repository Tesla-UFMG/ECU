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
#include "CAN/inverter_can.h"

void precharge_monitor();
void left_inv_error_callback();
void right_inv_error_callback();
void inverter_can_diff(uint32_t id);

//variaveis provisórias para teste do arduino
//inicializadas com valor '2' para ver se passaram pela lógica de diferenciação
uint8_t left_inv_sent = 2;
uint8_t right_inv_sent = 2;
uint8_t both_invs_sent = 2;





void inverter_comm_error(void* argument) {
    UNUSED(argument);


    uint32_t id;

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();


        switch (osMessageQueueGet(q_ids_can_inverterHandle, &id, NULL, osWaitForever)) {

            default:
            	// differentiates CAN signals from both inverters
            	// and indicates if those are ready
            	inverter_can_diff(id);

                break;
        }
    }
}

void inverter_can_diff(uint32_t id){

	// Restart the timer and clear the error if any message on each inverter
	// gets received
	if (id  >= 100 && id <= 103)	{
		osTimerStart(tim_left_inv_errorHandle, INV_COMM_ERROR_TIME);
		clear_error(LEFT_INVERTER_COMM_ERROR_FLAG);
		//teste arduino
		left_inv_sent = 1;
	}
	if (id  >= 200 && id <= 203)	{
		osTimerStart(tim_right_inv_errorHandle, INV_COMM_ERROR_TIME);
		clear_error(RIGHT_INVERTER_COMM_ERROR_FLAG);
		//teste arduino
		right_inv_sent = 1;
	}

	// Calls the precharge_monitor() if there is no error flags
	if	(!get_individual_flag(e_ECU_control_flagsHandle, LEFT_INVERTER_COMM_ERROR_FLAG)
	        && !get_individual_flag(e_ECU_control_flagsHandle,
	                               RIGHT_INVERTER_COMM_ERROR_FLAG))	{
		//teste arduino
		both_invs_sent = 1;
		precharge_monitor();
	}
}

void left_inv_error_callback()	{

		issue_error(LEFT_INVERTER_COMM_ERROR_FLAG, /*should_set_control_event_flag=*/true);

	    osEventFlagsClear(e_ECU_control_flagsHandle, INVERTER_READY);

	    osTimerStop(tim_inverter_readyHandle);
	    //teste
	    left_inv_sent = 0;
	    both_invs_sent = 0;
}

void right_inv_error_callback()	{

		issue_error(RIGHT_INVERTER_COMM_ERROR_FLAG, /*should_set_control_event_flag=*/true);

	    osEventFlagsClear(e_ECU_control_flagsHandle, INVERTER_READY);

	    osTimerStop(tim_inverter_readyHandle);
	    //teste
	    right_inv_sent = 0;
	    both_invs_sent = 0;

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
