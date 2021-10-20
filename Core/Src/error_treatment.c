/*
 * error_treatment.c
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */

#include "error_treatment.h"
#include "cmsis_os.h"
#include "global_instances.h"


void check_for_errors(bool (* areThereErrors)(), int flagError) {
    if (areThereErrors()) {
        osThreadFlagsSet(t_main_taskHandle, flagError);     // seta flag de thread da main task com a flag flagError
        osEventFlagsSet(ECU_control_event_id, flagError);   // seta flag de estado com a flag flagError
    } else {
        osEventFlagsClear(ECU_control_event_id, flagError); // limpa flag de estado flagError
    }
}