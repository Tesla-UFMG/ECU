/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "throttle.h"
#include "datalog_handler.h"



void throttle_read(void *argument) {
    uint16_t APPS1;
    uint16_t APPS2;
    uint16_t BSE;
    uint16_t aux_throttle_percent;

    for (;;) {
        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        APPS1 = ADC_DMA_buffer[APPS1_E];
        APPS2 = ADC_DMA_buffer[APPS2_E];
        BSE   = ADC_DMA_buffer[BRAKE_E];

        //calcula a porcentagem do pedal a partir do APPS2
        if (APPS2 < 260) {
            aux_throttle_percent = 0;
        } else if (APPS2 >= 260 && APPS2 < 467) {
            aux_throttle_percent = 1.162 * APPS2 - 342.9;
        } else if (APPS2 >= 467 && APPS2 < 1065) {
            aux_throttle_percent = 0.3344 * APPS2 + 43.8;
        } else if (APPS2 >= 1065 && APPS2 < 2253) {
            aux_throttle_percent = 0.1684 * APPS2 + 220.7;
        } else if (APPS2 >= 2253 && APPS2 < 3211) {
            aux_throttle_percent = 0.2087 * APPS2 + 129.9;
        } else if (APPS2 >= 3211 && APPS2 < 3720) {
            aux_throttle_percent = 0.6598 * APPS2 - 1319;
        }

        if (aux_throttle_percent > 1000)
            aux_throttle_percent = 1000;


        brake_status = BSE > 2200;
        apps_status = aux_throttle_percent > 0;

        log_data(ID_BRAKE, brake_status);


        //verifica a plausabilidade dos APPSs
        if (check_APPS_error(APPS1, APPS2, aux_throttle_percent)){
            aux_throttle_percent = 0;                                   //zera a a porcentagem do pedal
            osEventFlagsSet(ECU_control_event_id, APPS_ERROR_FLAG);     //caso tenha algum erro de plausabilidade do apps a flag será setada
        }
        else
            osEventFlagsClear(ECU_control_event_id, APPS_ERROR_FLAG);   //limpa a flag se n houver erro

        //verifica a plausabilidade do BSE
        if (aux_throttle_percent > 300 && BSE > 2200) {
            aux_throttle_percent = 0;                                   //zera a a porcentagem do pedal
            osEventFlagsSet(ECU_control_event_id, BSE_ERROR_FLAG);      //caso tenha algum erro de plausabilidade do BSE a flag será setada
        }
        else
            osEventFlagsClear(ECU_control_event_id, BSE_ERROR_FLAG);    //limpa a flag se n houver erro


        throttle_percent = aux_throttle_percent;

        log_data(ID_THROTTLE, throttle_percent);

        osDelay(THROTTLE_DELAY);
    }
}

bool check_APPS_error (uint16_t APPS1, uint16_t APPS2, uint16_t aux_throttle_percent){
    uint16_t apps1_calc = 0;

    //calcula o valor teórico de APPS1 a partir do valor de APPS2
    if (aux_throttle_percent >= 0 && aux_throttle_percent < 200)
        apps1_calc = 2212;
    else if (aux_throttle_percent >= 200 && aux_throttle_percent < 400)
        apps1_calc = (uint16_t) (1.679 * aux_throttle_percent + 1876);
    else if (aux_throttle_percent >= 400 && aux_throttle_percent < 600)
        apps1_calc = (uint16_t) (2.621 * aux_throttle_percent + 1499);
    else if (aux_throttle_percent >= 600 && aux_throttle_percent < 800)
        apps1_calc = (uint16_t) (2.212 * aux_throttle_percent + 1745);
    else if (aux_throttle_percent >= 800 && aux_throttle_percent < 1135)
        apps1_calc = (uint16_t) (1.515 * aux_throttle_percent + 2302);


    if (APPS2 >= 3720)          //Se o valor de APPS2 for acima do seu máximo
        return true;
    else if (APPS1 < 1802.24)   //Se o valor de APPS1 for abaixo do seu mínimo
        return true;
    else if (APPS1 > 3900)      //Se o valor de APPS1 for acima do seu máximo
        return true;
    else if (APPS1 < apps1_calc * (1-APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0)) //verifica se APPS1 está abaixo do valor teórico de APPS1, considerando a tolerância
        return true;
    else if (APPS1 > apps1_calc * (1+APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0)) //verifica se APPS1 está acima do valor teórico de APPS1, considerando a tolerância
        return true;
    else
        return false;
}

