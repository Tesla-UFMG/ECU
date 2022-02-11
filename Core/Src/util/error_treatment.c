/*
 * error_treatment.c
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */

#include "util/error_treatment.h"
#include "util/global_instances.h"


void check_for_errors(bool (* areThereErrors)(), uint32_t flagError) {
    if (areThereErrors()) {
        // seta flag de thread e de estado com a flag flagError
        issue_error(flagError, /*should_set_control_event_flag=*/true);
     } else {
        clear_error(flagError);
}
}

void check_for_errors_with_timeout(bool (* areThereErrors)(), uint32_t flagError, osTimerId_t timerHandler, uint16_t timerAmount) {
    if (areThereErrors()) {
        //se o timer nao tiver rodando ele sera iniciado. O if  evita o reinicio do timer
        if (!osTimerIsRunning(timerHandler)){
            osTimerStart(timerHandler, timerAmount/portTICK_PERIOD_MS);
        }
    } else {
        //interrompe o timer
        osTimerStop(timerHandler);
        clear_error(flagError);
    }
}

void errors_with_timer_callback(void *argument){
    //obtem a flag a partir do argumento do callback
    uint32_t flagError = (uint32_t)argument;
    // seta flag de thread e de estado com a flag flagError
    issue_error(flagError, /*should_set_control_event_flag=*/true);
}

void issue_error(uint32_t flagError, bool should_set_control_event_flag){
    // seta flag de thread da main task com a flag flagError
    osThreadFlagsSet(t_main_taskHandle, flagError);
    if (should_set_control_event_flag) {
        // seta flag de thread da main task com a flag flagError
        osEventFlagsSet(ECU_control_event_id, flagError);
}
}

void clear_error(uint32_t flagError){
    osEventFlagsClear(ECU_control_event_id, flagError);
}
