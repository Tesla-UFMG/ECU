/*
 * RTD_handler.c
 *
 *  Created on: Aug 15, 2021
 *      Author: Felipe Telles
 */

#include "RTD.h"
#include "main.h"
#include "global_instances.h"
#include "rgb_led.h"
#include "debugleds.h"
#include "stdbool.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"

bool can_RTD_be_enabled();
void set_RTD();

void RTD(void *argument) {
    //seta o led rgb no primeira execução do código
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, BLINK200);

    for(;;) {

        osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);     //espera receber flag q o botão de RTD foi pressionado

        bool is_RTD_active = get_individual_flag(ECU_control_event_id, RTD_FLAG);

        if (!is_RTD_active) {
            if(can_RTD_be_enabled())
                set_RTD();                                                  //seta RTD
            else
                set_debugleds(DEBUGLED1,BLINK,2);                           //envia uma mensagem de alerta caso n seja possível acionar RTD
        }
    }
}


void exit_RTD() {
    set_global_var_value(SELECTED_MODE, erro);          //seta modo_selecionado como erro
    set_global_var_value(RACE_MODE, ERRO);
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, BLINK200);
    osEventFlagsClear(ECU_control_event_id, RTD_FLAG);  //limpa flag de RTD
}

bool can_RTD_be_enabled() {
    uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);                           //obtem todas as flags
    error_flags &= ALL_SEVERE_ERROR_FLAG;                                                   //filtra apenas flags de erros severos, ignorando as outras
    BRAKE_STATUS_t is_brake_active = get_global_var_value(BRAKE_STATUS);
    THROTTLE_STATUS_t is_throttle_active = get_global_var_value(THROTTLE_STATUS);
    RACE_MODE_t race_mode = get_global_var_value(RACE_MODE);
    if(/*is_brake_active && !is_throttle_active &&*/ !error_flags && (race_mode != ERRO))
        return true;
    else
        return false;
}

void set_RTD() {
    osEventFlagsSet(ECU_control_event_id, RTD_FLAG);                                         //Seta flag de RTD
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, FIXED);
    aciona_sirene(RTDS_TIME);
}

void aciona_sirene(uint32_t tempo_sirene) {
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_SET);
    osDelay(tempo_sirene);
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_RESET);
}
