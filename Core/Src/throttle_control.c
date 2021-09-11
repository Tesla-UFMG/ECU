/*
 * throttle_control.c
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */


#include "throttle_control.h"
#include "datalog_handler.h"
#include "global_instances.h"
#include "datalog_handler.h"

extern volatile uint16_t throttle_percent;

void throttle_control(void *argument) {
    for(;;) {
        uint16_t message;

        osMessageQueueGet(q_throttle_controlHandle, &message, NULL, osWaitForever);                     //espera por uma mensagem com o valor de APPS

        bool is_throttle_available = (osEventFlagsGet(ECU_control_event_id) & THROTTLE_AVAILABLE_FLAG); //determina se o pedal está disponivel a partir da flag de evento

        //se o pedal está disponível atualiza o valor de throttle_percent para o da mensagem, caso não atualiza para 0
        if (is_throttle_available)
            throttle_percent = message;
        else
            throttle_percent = 0;

        log_data(ID_THROTTLE, throttle_percent);
    }
}
