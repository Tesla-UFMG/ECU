/*
 * inverter_can_error_manager.c
 *
 *  Created on: Jan 5, 2022
 *      Author: Felipe Telles
 */

#include "CAN/inverter_can_error_manager.h"
#include "global_definitions.h"
#include "global_instances.h"
#include "error_treatment.h"
#include "util.h"




extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

void inverter_no_comm_error(void *argument) {

    for(;;) {
        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif



        switch(osThreadFlagsWait(INVERTER_CAN_ACTIVE, osFlagsWaitAny, 500)){
            case osFlagsErrorTimeout :
                issue_error(INVERTER_COMM_ERROR_FLAG, /*should_set_control_event_flag=*/true);    // chama o erro para a main_task tratar
                osEventFlagsClear(ECU_control_event_id, INVERTER_READY);     // seta flag de thread da main task com a flag flagError
                osTimerStop(tim_inverter_readyHandle);      //interrompe o timer
                break;
            default:
                clear_error(INVERTER_COMM_ERROR_FLAG);
                if (!get_individual_flag(ECU_control_event_id, INVERTER_READY)){
                    if (!osTimerIsRunning(tim_inverter_readyHandle)){                               //se o timer não tiver rodando ele será iniciado. Esse if serve para evitar reiniciar o timer
                        osTimerStart(tim_inverter_readyHandle, 5000/portTICK_PERIOD_MS);
                    }
                }
                break;
        }
    }
}

void inverter_ready_callback(void *argument){
    osEventFlagsSet(ECU_control_event_id, INVERTER_READY);
}
