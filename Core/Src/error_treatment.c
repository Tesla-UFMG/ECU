/*
 * error_treatment.c
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */

#include "error_treatment.h"
#include "global_instances.h"


void check_for_errors(bool (* areThereErrors)(), uint32_t flagError) {
    if (areThereErrors())
        issue_error(flagError, true);   // seta flag de estado com a flag flagError
     else
        clear_error(flagError);         // limpa flag de estado flagError
}

void issue_error(uint32_t flagError, bool should_set_control_event_flag){
    osThreadFlagsSet(t_main_taskHandle, flagError);             // seta flag de thread da main task com a flag flagError
    if (should_set_control_event_flag)
        osEventFlagsSet(ECU_control_event_id, flagError);       // seta flag de thread da main task com a flag flagError
}

void clear_error(uint32_t flagError){
    osEventFlagsClear(ECU_control_event_id, flagError);     // seta flag de thread da main task com a flag flagError
}
