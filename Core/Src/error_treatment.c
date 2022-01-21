/*
 * error_treatment.c
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */

#include "error_treatment.h"
#include "global_instances.h"

void check_for_errors(bool (*areThereErrors)(), uint32_t flagError) {
    if (areThereErrors()) {
        issue_error(flagError,
                    /*should_set_control_event_flag=*/true); // seta flag de thread e de estado com
                                                             // a flag flagError
    } else {
        clear_error(flagError); // limpa flag de estado flagError
    }
}

void check_for_errors_with_timeout(bool (*areThereErrors)(), uint32_t flagError,
                                   osTimerId_t timerHandler, uint16_t timerAmount) {
    if (areThereErrors()) {
        if (!osTimerIsRunning(timerHandler)) { // se o timer nao tiver rodando ele sera iniciado.
                                               // Esse if serve para evitar reiniciar o timer
            osTimerStart(timerHandler, timerAmount / portTICK_PERIOD_MS);
        }
    } else {
        osTimerStop(timerHandler); // interrompe o timer
        clear_error(flagError);    // limpa flag de estado flagError
    }
}

void errors_with_timer_callback(void* argument) {
    uint32_t flagError = (uint32_t)argument; // obtem a flag a partir do argumento do callback
    issue_error(flagError, /*should_set_control_event_flag=*/true); // seta flag de thread e de
                                                                    // estado com a flag flagError
}

void issue_error(uint32_t flagError, bool should_set_control_event_flag) {
    osThreadFlagsSet(t_main_taskHandle,
                     flagError); // seta flag de thread da main task com a flag flagError
    if (should_set_control_event_flag) {
        osEventFlagsSet(ECU_control_event_id,
                        flagError); // seta flag de thread da main task com a flag flagError
    }
}

void clear_error(uint32_t flagError) {
    osEventFlagsClear(ECU_control_event_id,
                      flagError); // seta flag de thread da main task com a flag flagError
}
