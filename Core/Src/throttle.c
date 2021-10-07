/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "throttle.h"
#include "datalog_handler.h"
#include "error_treatment.h"
#include "CMSIS_extra/global_variables_handler.h"
#include "util.h"
#include "global_variables.h"
#include "global_instances.h"
#include "cmsis_os.h"


uint16_t calculate_expected_apps1_from_apps2(uint16_t apps2_percentage);
uint16_t calculate_apps2(uint16_t APPS2);
bool is_there_APPS_error();
bool is_there_BSE_error();
bool is_there_SU_F_error();

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

        set_global_var_value(BRAKE_STATUS, (BSE > BRAKE_ACTIVE));
        set_global_var_value(THROTTLE_STATUS, (apps2_throttle_percent > 0));

        log_data(ID_BRAKE, get_global_var_value(BRAKE_STATUS));

        check_for_errors(is_there_APPS_error, APPS_ERROR_FLAG);           //verifica a plausabilidade dos APPSs
        check_for_errors(is_there_BSE_error, BSE_ERROR_FLAG);             //verifica a plausabilidade do BSE
        check_for_errors(is_there_SU_F_error, SU_F_ERROR_FLAG);           //verifica se a placa de freio está em curto

        uint16_t message = apps2_throttle_percent;
        osMessageQueuePut(q_throttle_controlHandle, &message, 0, 0U);

        osDelay(THROTTLE_DELAY);
    }
}

//calcula o valor do APPS2
uint16_t calculate_apps2(uint16_t APPS2) {
    uint16_t apps2_percentage = 0;
    if (APPS2 < 0)
        apps2_percentage = 0;
    else if (APPS2 >= 0 && APPS2 < 825)
        apps2_percentage = 0.3636 * APPS2;
    else if (APPS2 >= 825 && APPS2 < 1112)
        apps2_percentage = 0.3484 * APPS2 + 43.1;
    else if (APPS2 >= 1112 && APPS2 < 1530)
        apps2_percentage = 0.2392 * APPS2 + 149.1;
    else if (APPS2 >= 1530 && APPS2 < 1850)
        apps2_percentage = 0.3125 * APPS2 + 49.4;
    else if (APPS2 >= 1850 && APPS2 < 2230)
        apps2_percentage = 0.2631 * APPS2 + 132.2;
    else if (APPS2 >= 2230 && APPS2 < 2510)
        apps2_percentage = 0.3571 * APPS2 - 75.3;
    else if (APPS2 >= 2510 && APPS2 < 2850)
        apps2_percentage = 0.2941 * APPS2 + 79.6;
    else if (APPS2 >= 2850 && APPS2 < 3140)
        apps2_percentage = 0.3448 * APPS2 - 54.8;

    if (apps2_percentage > 1000)
        apps2_percentage = 1000;

    return apps2_percentage;
}

//calcula o valor teórico de APPS1 a partir do valor de APPS2
uint16_t calculate_expected_apps1_from_apps2(uint16_t apps2_percentage) {
    if (apps2_percentage >= 0 && apps2_percentage < 200)
        return (2000);
    else if (apps2_percentage >= 200 && apps2_percentage < 300)
        return ((uint16_t) (0.435 * apps2_percentage + 2235));
    else if (apps2_percentage >= 300 && apps2_percentage < 400)
        return ((uint16_t) (0.606 * apps2_percentage + 2288));
    else if (apps2_percentage >= 400 && apps2_percentage < 500)
        return ((uint16_t) (0.444 * apps2_percentage + 2532));
    else if (apps2_percentage >= 500 && apps2_percentage < 600)
        return ((uint16_t) (0.645 * apps2_percentage + 2523));
    else if (apps2_percentage >= 600 && apps2_percentage < 700)
        return ((uint16_t) (0.454 * apps2_percentage + 2811));
    else if (apps2_percentage >= 700 && apps2_percentage < 800)
        return ((uint16_t) (0.667 * apps2_percentage + 2747));
    else if (apps2_percentage >= 800 && apps2_percentage < 900)
        return ((uint16_t) (0.625 * apps2_percentage + 2877));
    else if (apps2_percentage >= 900 && apps2_percentage < 1050)
        return ((uint16_t) (0.909 * apps2_percentage + 2640));
    else
        return 0;
}

bool is_there_APPS_error() {      //Regulamento: T.4.2 (2021)
    if (    APPS2 >= 3140         //Se o valor de APPS2 for acima do seu máximo
         || APPS1 < 2000          //Se o valor de APPS1 for abaixo do seu mínimo
         || APPS1 > 3550          //Se o valor de APPS1 for acima do seu máximo
         || APPS1 < apps1_calc * (1-APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0)   //verifica se APPS1 está abaixo do valor teórico de APPS1, considerando a tolerância
         || APPS1 > apps1_calc * (1+APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0))  //verifica se APPS1 está acima do valor teórico de APPS1, considerando a tolerância
        return true;
    else
        return false;
}

bool is_there_BSE_error() {
    bool is_BSE_error_active = get_individual_flag(ECU_control_event_id, BSE_ERROR_FLAG);
    if (is_BSE_error_active)
        return (apps2_throttle_percent >= APPS_05_PERCENT);                         //Regulamento: EV.5.7.2 (2021)
    else
        return (apps2_throttle_percent > APPS_25_PERCENT && BSE > BRAKE_ACTIVE);    //Regulamento: EV.5.7.1 (2021)
}

bool is_there_SU_F_error() {
    return (BSE > SU_F_ERROR);
}
