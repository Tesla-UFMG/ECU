/*
 * throttle_control.c
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */


#include "throttle_control.h"
#include "cmsis_os.h"
#include "datalog_handler.h"
#include "global_instances.h"
#include "datalog_handler.h"
#include "util.h"

extern volatile uint16_t throttle_percent;

void throttle_control(void *argument) {
    for(;;) {
        uint16_t message;

        osMessageQueueGet(q_throttle_controlHandle, &message, NULL, osWaitForever);                     //espera por uma mensagem com o valor de APPS

        //se o pedal está disponível atualiza o valor de throttle_percent para o da mensagem, caso não atualiza para 0
        if (get_individual_flag(ECU_control_event_id, THROTTLE_AVAILABLE_FLAG) && get_individual_flag(ECU_control_event_id, RTD_FLAG))
            throttle_percent = message;
        else
            throttle_percent = 0;

        log_data(ID_THROTTLE, throttle_percent);
    }
}
