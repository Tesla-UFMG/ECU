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
#include "rgb_led.h"
#include "debugleds.h"

void aciona_sirene();
bool can_RTD_be_enabled();
void set_RTD();

void RTD(void *argument) {

    set_rgb_led(modo_selecionado.cor, NO_CHANGE);

    for(;;) {

        osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);     //espera receber flag q o botão de RTD foi pressionado

        bool is_RTD_active = (osEventFlagsGet(ECU_control_event_id) & RTD_FLAG);
        if (!is_RTD_active){
            if(can_RTD_be_enabled())
                set_RTD();                                                  //seta RTD
            else
                set_debugleds(DEBUGLED1,BLINK,2);                           //envia uma mensagem de alerta caso n seja possível acionar RTD
        }
    }
}


void exit_RTD() {
    modo_selecionado = erro;                            //seta modo_selecionado como erro
    g_race_mode = ERRO;
    set_rgb_led(modo_selecionado.cor, BLINK200);
    osEventFlagsClear(ECU_control_event_id, RTD_FLAG);  //limpa flag de RTD
    osSemaphoreRelease(s_allowed_change_modeHandle);                            //libera semáforo que permite a mudança de modos
}

bool can_RTD_be_enabled() {
    uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);                           //obtem todas as flags
    error_flags &= ALL_SEVERE_ERROR_FLAG;                                                   //filtra apenas flags de erros severos, ignorando as outras
    if(is_brake_active && !is_throttle_active && !error_flags && (g_race_mode != ERRO))
        return true;
    else
        return false;
}

void set_RTD() {
    osEventFlagsSet(ECU_control_event_id, RTD_FLAG);                                         //Seta flag de RTD
    osSemaphoreAcquire(s_allowed_change_modeHandle, osWaitForever);                          //Bloqueia mudança de modo
    set_rgb_led(modo_selecionado.cor, FIXED);
    aciona_sirene();
}

void aciona_sirene() {
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_SET);
    osDelay(tempo_sirene);
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_RESET);
}
