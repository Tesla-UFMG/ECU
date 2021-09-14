/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "throttle.h"
#include "datalog_handler.h"
#include "error_treatment.h"
#include "util.h"


uint16_t calculate_expected_apps1_from_apps2(uint16_t apps2_percentage);
uint16_t calculate_apps2(uint16_t APPS2);
bool check_for_APPS_errors();
bool check_for_BSE_errors();
bool check_for_SU_F_errors();

static uint16_t APPS1;
static uint16_t APPS2;
static uint16_t BSE;
static uint16_t apps2_throttle_percent = 0;
static uint16_t apps1_calc = 0;

void throttle_read(void *argument) {

    for (;;) {

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        APPS1 = ADC_DMA_buffer[APPS1_E];
        APPS2 = ADC_DMA_buffer[APPS2_E];
        BSE   = ADC_DMA_buffer[BRAKE_E];

        apps2_throttle_percent = calculate_apps2(APPS2);                      //calcula a porcentagem do pedal a partir do APPS2
        apps1_calc = calculate_expected_apps1_from_apps2(apps2_throttle_percent);

        is_brake_active = (BSE > BRAKE_ACTIVE);
        is_throttle_active = (apps2_throttle_percent > 0);

        log_data(ID_BRAKE, is_brake_active);

        check_for_errors(check_for_APPS_errors, APPS_ERROR_FLAG);           //verifica a plausabilidade dos APPSs
        check_for_errors(check_for_BSE_errors, BSE_ERROR_FLAG);             //verifica a plausabilidade do BSE
        check_for_errors(check_for_SU_F_errors, SU_F_ERROR_FLAG);           //verifica se a placa de freio está em curto

        uint16_t message = apps2_throttle_percent;
        osMessageQueuePut(q_throttle_controlHandle, &message, 0, 0U);

        osDelay(THROTTLE_DELAY);
    }
}

//calcula o valor do APPS2
uint16_t calculate_apps2(uint16_t APPS2) {
    uint16_t apps2_percentage = 0;
    if (APPS2 < 260)
        apps2_percentage = 0;
    else if (APPS2 >= 260 && APPS2 < 467)
        apps2_percentage = 1.162 * APPS2 - 342.9;
    else if (APPS2 >= 467 && APPS2 < 1065)
        apps2_percentage = 0.3344 * APPS2 + 43.8;
    else if (APPS2 >= 1065 && APPS2 < 2253)
        apps2_percentage = 0.1684 * APPS2 + 220.7;
    else if (APPS2 >= 2253 && APPS2 < 3211)
        apps2_percentage = 0.2087 * APPS2 + 129.9;
    else if (APPS2 >= 3211 && APPS2 < 3720)
        apps2_percentage = 0.6598 * APPS2 - 1319;

    if (apps2_percentage > 1000)
        apps2_percentage = 1000;

    return apps2_percentage;
}

//calcula o valor teórico de APPS1 a partir do valor de APPS2
uint16_t calculate_expected_apps1_from_apps2(uint16_t apps2_percentage) {
    uint16_t apps1_calc = 0;
    if (apps2_percentage >= 0 && apps2_percentage < 200)
        apps1_calc = 2212;
    else if (apps2_percentage >= 200 && apps2_percentage < 400)
        apps1_calc = (uint16_t) (1.679 * apps2_percentage + 1876);
    else if (apps2_percentage >= 400 && apps2_percentage < 600)
        apps1_calc = (uint16_t) (2.621 * apps2_percentage + 1499);
    else if (apps2_percentage >= 600 && apps2_percentage < 800)
        apps1_calc = (uint16_t) (2.212 * apps2_percentage + 1745);
    else if (apps2_percentage >= 800 && apps2_percentage < 1135)
        apps1_calc = (uint16_t) (1.515 * apps2_percentage + 2302);

    return apps1_calc;
}

bool check_for_APPS_errors() {      //Regulamento: T.4.2
    if (    APPS2 >= 3720           //Se o valor de APPS2 for acima do seu máximo
         || APPS1 < 1802.24         //Se o valor de APPS1 for abaixo do seu mínimo
         || APPS1 > 3900            //Se o valor de APPS1 for acima do seu máximo
         || APPS1 < apps1_calc * (1-APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0)   //verifica se APPS1 está abaixo do valor teórico de APPS1, considerando a tolerância
         || APPS1 > apps1_calc * (1+APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0))  //verifica se APPS1 está acima do valor teórico de APPS1, considerando a tolerância
        return true;
    else
        return false;
}

bool check_for_BSE_errors() {
    bool is_BSE_error_active = get_individual_flag(ECU_control_event_id, BSE_ERROR_FLAG);
    if (is_BSE_error_active)
        return (apps2_throttle_percent >= APPS_05_PERCENT);                         //Regulamento: EV.5.7.2
    else
        return (apps2_throttle_percent > APPS_25_PERCENT && BSE > BRAKE_ACTIVE);    //Regulamento: EV.5.7.1
}

bool check_for_SU_F_errors() {
    return (BSE > SU_F_ERROR);
}
