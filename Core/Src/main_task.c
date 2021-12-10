/*
 * main_task.c
 *
 *  Created on: Jun 3, 2020
 *      Author: renanmoreira
 */

#include "main_task.h"
#include "global_variables.h"
#include "global_instances.h"
#include "cmsis_os.h"
#include "RTD.h"
#include "debugleds.h"
#include "rgb_led.h"
#include "throttle.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"



void main_task(void *argument) {

    for(;;) {

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, osWaitForever);        //espera RTD ser acionado. Por meio da RTD_FLAG

        osThreadFlagsWait(ALL_ERRORS_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);     //espera por qualquer erro
        uint32_t error_flags = osThreadFlagsGet();                                              //obtem os valores de flag de thread
        uint32_t event_flags = osEventFlagsGet(ECU_control_event_id);                           //obtem os valores de flag de evento
        uint32_t most_significant_error_flags = get_flag_MSB(error_flags &= ALL_ERRORS_FLAG);   //obtem a flag de threa mais significativa
        bool isErrorPresent;
        switch (most_significant_error_flags) {

            case INVERTER_BUS_OFF_ERROR_FLAG:
                isErrorPresent = event_flags & INVERTER_BUS_OFF_ERROR_FLAG;                     //verifica se o erro está presente na flag de evento
                if (isErrorPresent) {
                exit_RTD();                                                                     //sai de RTD caso o erro esteja presente
                } else {                                                                        //caso o erro não esteja ainda:
                    osEventFlagsSet(ECU_control_event_id, INVERTER_BUS_OFF_ERROR_FLAG);         //seta a flag de evento para que caso tenha erro novamente saia de RTD
                    osThreadFlagsClear(INVERTER_BUS_OFF_ERROR_FLAG);                            //limpa flag de thread do erro
                    osTimerStart(tim_inverter_BUS_OFF_errorHandle, BUS_OFF_ERROR_TIME);    //inicia o timer para zerar a flag
                }
                break;

            case INVERTER_COMM_ERROR_FLAG:
                //todo: implementar erro de comunicação com inversor
                isErrorPresent = event_flags & INVERTER_COMM_ERROR_FLAG;    //verifica se o erro ainda está presente na flag de evento
                if (isErrorPresent) {
                    exit_RTD();                                             //sai de RTD caso o erro esteja presente
                } else {                                                    //caso o erro tenha sido resolvido:
                    osThreadFlagsClear(INVERTER_COMM_ERROR_FLAG);           //limpa flag de thread do erro
                }
                break;

            case SU_F_ERROR_FLAG:
                isErrorPresent = event_flags & SU_F_ERROR_FLAG;             //verifica se o erro ainda está presente na flag de evento
                if (isErrorPresent) {
                    exit_RTD();                                             //sai de RTD caso o erro esteja presente
                } else {                                                    //caso o erro tenha sido resolvido:
                    osThreadFlagsClear(SU_F_ERROR_FLAG);                    //limpa flag de thread do erro
                }
                break;

            case APPS_ERROR_FLAG:                                                       //Regulamento: T.4.2 (2021)
                isErrorPresent = event_flags & APPS_ERROR_FLAG;                         //verifica se o erro ainda está presente na flag de evento
                if (isErrorPresent) {                                                   //caso o erro esteja presente:
                    set_rgb_led(AMARELO, NO_CHANGE);                                    //seta o led rgb como amarelo
                    osDelay(20);
                } else {                                                                //caso o erro tenha sido resolvido:
                    osThreadFlagsClear(APPS_ERROR_FLAG);                                //limpa flag de thread do erro
                    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, NO_CHANGE);    //retorna o RGB ao funcionamento normal
                }
                break;

            case BSE_ERROR_FLAG:                                                        //Regulamento: EV.5.7 (2021)
                isErrorPresent = event_flags & BSE_ERROR_FLAG;                          //verifica se o erro ainda está presente na flag de evento
                if (isErrorPresent) {                                                   //caso o erro esteja presente:
                    set_rgb_led(AMARELO, NO_CHANGE);                                    //seta o led rgb como amarelo
                    osDelay(20);
                } else {                                                                //caso o erro tenha sido resolvido:
                    osThreadFlagsClear(BSE_ERROR_FLAG);                                 //limpa flag de thread do erro
                    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, NO_CHANGE);    //retorna o RGB ao funcionamento normal
                }
                break;

            default:
                osDelay(100);
                break;
        }
    }
}

