/*
 * RTD_handler.c
 *
 *  Created on: Aug 15, 2021
 *      Author: Felipe Telles
 */

#include "RTD.h"
#include "global_variables.h"
#include "debugleds.h"
#include "main.h"
#include "global_instances.h"

void RTD_handler(void *argument) {

    for(;;) {

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        osSemaphoreRelease(s_allowed_change_modeHandle);                                //libera semáforo que permite a mudança de modos

           for(;;) {
               osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);     //espera receber flag q o botão de RTD foi pressionado
               uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);               //obtem todas as flags
               error_flags &= ALL_SEVERE_ERROR_FLAG;                          //filtra apenas flags de erros severos, ignorando as outras
               if(is_brake_active && !is_throttle_active && !error_flags && (g_race_mode != ERRO))
                   break;                                                                  //sai do for infinito caso tudo esteja certo para acionar RTD
               else
                   set_debugleds(DEBUGLED1,BLINK,2);                                       //envia uma mensagem de alerta caso n seja possível acionar RTD
           }

           //seta a flag de RTD
           osEventFlagsSet(ECU_control_event_id, RTD_FLAG);                                //Seta flag de RTD
           osSemaphoreAcquire(s_allowed_change_modeHandle, osWaitForever);                 //Bloqueia mudança de modo
           aciona_sirene();

    }
}

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

