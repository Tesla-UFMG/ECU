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


        //verifica a plausabilidade do BSE
        check_for_errors(is_there_BSE_error, BSE_ERROR_FLAG);
        //verifica a plausabilidade dos APPSs
        check_for_errors_with_timeout(is_there_APPS_error, APPS_ERROR_FLAG, tim_APPS_errorHandle, APPS_ERROR_TIMER);
        //verifica se a placa de freio está em curto
        check_for_errors_with_timeout(is_there_SU_F_error, SU_F_ERROR_FLAG, tim_SU_F_errorHandle, SU_F_ERROR_TIMER);

        uint16_t message = apps2_throttle_percent;
        osMessageQueuePut(q_throttle_controlHandle, &message, 0, 0U);

        osDelay(THROTTLE_DELAY);
    }
}

//calcula o valor do APPS2
uint16_t calculate_apps2(uint16_t APPS2) {
    uint16_t apps2_percentage = 0;
    if (APPS2 < 277)
        apps2_percentage = 0;
    else if (APPS2 >= 277 && APPS2 < 560)
        apps2_percentage = 0.3534 * APPS2 - 97.88;
    else if (APPS2 >= 560 && APPS2 < 900)
        apps2_percentage = 0.2941 * APPS2 - 64.71;
    else if (APPS2 >= 900 && APPS2 < 1100)
        apps2_percentage = 0.5 * APPS2 - 250;
    else if (APPS2 >= 1100 && APPS2 < 1380)
        apps2_percentage = 0.3571 * APPS2 - 92.86;
    else if (APPS2 >= 1380 && APPS2 < 1610)
        apps2_percentage = 0.4348 * APPS2 - 200;
    else if (APPS2 >= 1610 && APPS2 < 1922)
        apps2_percentage = 0.3205 * APPS2 - 16.03;
    else if (APPS2 >= 1922 && APPS2 < 2140)
        apps2_percentage = 0.4587 * APPS2 - 281.65;
    else if (APPS2 >= 2140 && APPS2 < 2390)
        apps2_percentage = 0.4 * APPS2 - 156;
    else if (APPS2 >= 2390 && APPS2 < 2480)
            apps2_percentage = 1.1111 * APPS2 - 1855.56;
    else if (APPS2 >= 2480 && APPS2 < 2600)
            apps2_percentage = 0.8333 * APPS2 - 1166.67;

    if (apps2_percentage > 1000)
        apps2_percentage = 1000;

    return apps2_percentage;
}

//calcula o valor teórico de APPS1 a partir do valor de APPS2
uint16_t calculate_expected_apps1_from_apps2(uint16_t apps2_percentage) {
    if (apps2_percentage >= 0 && apps2_percentage < 100)
        return ((uint16_t) (2.08 * apps2_percentage + 2060));
    else if (apps2_percentage >= 100 && apps2_percentage < 200)
        return ((uint16_t) (1.42 * apps2_percentage + 2126));
    else if (apps2_percentage >= 200 && apps2_percentage < 300)
        return ((uint16_t) (1.2 * apps2_percentage + 2170));
    else if (apps2_percentage >= 300 && apps2_percentage < 400)
        return ((uint16_t) (1.5 * apps2_percentage + 2080));
    else if (apps2_percentage >= 400 && apps2_percentage < 500)
        return ((uint16_t) (1.2 * apps2_percentage + 2200));
    else if (apps2_percentage >= 500 && apps2_percentage < 600)
        return ((uint16_t) (1.65 * apps2_percentage + 1975));
    else if (apps2_percentage >= 600 && apps2_percentage < 700)
        return ((uint16_t) (1.3 * apps2_percentage + 2185));
    else if (apps2_percentage >= 700 && apps2_percentage < 800)
        return ((uint16_t) (1.31 * apps2_percentage + 2178));
    else if (apps2_percentage >= 800 && apps2_percentage < 900)
        return ((uint16_t) (1.04 * apps2_percentage + 2394));
    else if (apps2_percentage >= 900 && apps2_percentage < 1135)
        return ((uint16_t) (1 * apps2_percentage + 2430));
    else
        return 0;
}

bool is_there_APPS_error() {        //Regulamento: T.4.2 (2021)
    if (    APPS2 >= 3500           //Se o valor de APPS2 for acima do seu máximo
         || APPS1 < 1900            //Se o valor de APPS1 for abaixo do seu mínimo
         || APPS1 > 3700            //Se o valor de APPS1 for acima do seu máximo
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
