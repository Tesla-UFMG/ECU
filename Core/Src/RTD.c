/*
 * RTD_handler.c
 *
 *  Created on: Aug 15, 2021
 *      Author: Felipe Telles
 */

#include "RTD.h"
#include "global_variables.h"
#include "main.h"
#include "global_instances.h"


void exit_RTD(){
    modo_selecionado = erro;                            //seta modo_selecionado como erro
    g_race_mode = ERRO;
    osEventFlagsClear(ECU_control_event_id, RTD_FLAG);  //limpa flag de RTD
}

void aciona_sirene(){
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_SET);
    osDelay(tempo_sirene);
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_RESET);
}

bool is_RTD_available(){
    uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);                           //obtem todas as flags
    error_flags &= ALL_SEVERE_ERROR_FLAG;                                                   //filtra apenas flags de erros severos, ignorando as outras
    if(is_brake_active && !is_throttle_active && !error_flags && (g_race_mode != ERRO))
        return true;
    else
        return false;
}

void set_RTD(){
    osEventFlagsSet(ECU_control_event_id, RTD_FLAG);                                         //Seta flag de RTD
    osSemaphoreAcquire(s_allowed_change_modeHandle, osWaitForever);                          //Bloqueia mudança de modo
    aciona_sirene();
}

