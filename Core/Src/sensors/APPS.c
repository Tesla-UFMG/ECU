/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "sensors/APPS.h"

#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/error_treatment.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

extern IWDG_HandleTypeDef hiwdg1;

typedef struct {
    uint16_t deadzone_lower_limit;
    uint16_t deadzone_upper_limit;
    float adjust_parameters_slope;
    float adjust_parameters_intercept;
} apps_ref;

static uint16_t throttle_calc(uint16_t APPS_VALUE, const apps_ref* ref);
static bool is_there_APPS_error();
static bool is_there_BSE_error();
static bool is_there_SU_F_error();

static uint16_t apps1_value;
static uint16_t apps2_value;
static uint16_t bse;
static uint16_t apps1_throttle_percent = 0;
static uint16_t apps2_throttle_percent = 0;
static uint16_t throttle_percent       = 0;

uint16_t apps1 = 0;
uint16_t apps2 = 0;
bool err;
bool err2;
bool err3;
uint16_t diff = 0;


void APPS_read(void* argument) {
    UNUSED(argument);

    for (;;) {

        ECU_ENABLE_BREAKPOINT_DEBUG();

        apps1_value = ADC_DMA_buffer[APPS1_E];
        apps2_value = ADC_DMA_buffer[APPS2_E];
        bse         = ADC_DMA_buffer[BRAKE_E];

        // valores de referencia e parametros para o calculo da porcentagem
        static const apps_ref apps1_ref = {.deadzone_lower_limit = APPS1_LOWER_DEADZONE,
                                           .deadzone_upper_limit = APPS1_UPPER_DEADZONE,
                                           .adjust_parameters_slope = APPS1_ADJUST_SLOPE,
                                           .adjust_parameters_intercept =
                                               APPS1_ADJUST_INTERCEPT};
        static const apps_ref apps2_ref = {.deadzone_lower_limit = APPS2_LOWER_DEADZONE,
                                           .deadzone_upper_limit = APPS2_UPPER_DEADZONE,
                                           .adjust_parameters_slope = APPS2_ADJUST_SLOPE,
                                           .adjust_parameters_intercept =
                                               APPS2_ADJUST_INTERCEPT};

        // calcula a porcentagem do pedal a partir do APPS1 e APPS2 e faz a media
        apps1_throttle_percent = throttle_calc(apps1_value, &apps1_ref);
        apps2_throttle_percent = throttle_calc(apps2_value, &apps2_ref);
        throttle_percent       = avg(apps1_throttle_percent, apps2_throttle_percent);

        set_global_var_value(BRAKE_STATUS, (BRAKE_STATUS_t)(bse > BRAKE_ACTIVE));
        set_global_var_value(THROTTLE_STATUS, (THROTTLE_STATUS_t)(throttle_percent > 0));

        log_data(ID_BRAKE, get_global_var_value(BRAKE_STATUS));

        // verifica a plausabilidade do APPS e BSE e plausabilidade dos APPSs
        check_for_errors(is_there_BSE_error, BSE_ERROR_FLAG);
        check_for_errors_with_timeout(is_there_APPS_error, APPS_ERROR_FLAG,
                                      tim_APPS_errorHandle, APPS_ERROR_TIMER);
        // verifica se a placa de freio esta enviando sinal de curto
        check_for_errors_with_timeout(is_there_SU_F_error, SU_F_ERROR_FLAG,
                                      tim_SU_F_errorHandle, SU_F_ERROR_TIMER);

        apps1 = apps1_throttle_percent;
        apps2 = apps2_throttle_percent;
        diff = abs(apps1_throttle_percent - apps2_throttle_percent) / 10;
        err = is_there_APPS_error();
        err2 = is_there_BSE_error();
        err3 = is_there_SU_F_error();
        uint16_t message = throttle_percent;
        osMessageQueuePut(q_throttle_controlHandle, &message, 0, 0U);

        HAL_IWDG_Refresh(&hiwdg1);
        osDelay(THROTTLE_DELAY);
    }
}

static uint16_t throttle_calc(uint16_t apps_value, const apps_ref* ref) {
    if (apps_value > ref->deadzone_upper_limit) {
        return 1000;
    }
    if (apps_value < ref->deadzone_lower_limit) {
        return 0;
    }
    return (uint16_t)(ref->adjust_parameters_slope * (float)apps_value
                      + ref->adjust_parameters_intercept);
}

static bool is_there_APPS_error() { // Regulamento: T.4.2 (2021)
    if (apps2_value > APPS2_MAX     // Se o valor de APPS2 for acima do seu maximo
        || apps2_value < APPS2_MIN  // ou abaixo do seu minimo
        || apps1_value > APPS1_MAX  // Se o valor de APPS1 for acima do seu maximo
        || apps1_value < APPS1_MIN  // ou abaixo do seu minimo
        // Se os APPS1 e APPS2 discordarem em mais de 10%
        || abs(apps1_throttle_percent - apps2_throttle_percent) / 10
               > APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE) {
        return true;
    }
    return false;
}

static bool is_there_BSE_error() {
    const bool is_BSE_error_active =
        get_individual_flag(e_ECU_control_flagsHandle, BSE_ERROR_FLAG);
    if (is_BSE_error_active) {
        // Regulamento: EV.5.7.2 (2021)
        return (throttle_percent >= APPS_05_PERCENT);
    }
    // Regulamento: EV.5.7.1 (2021)
    return (throttle_percent > APPS_25_PERCENT && bse > BRAKE_ACTIVE);
}

static bool is_there_SU_F_error() {
    return (bse > SU_F_ERROR);
}
