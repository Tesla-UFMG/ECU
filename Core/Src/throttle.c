/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "throttle.h"
#include "CMSIS_extra/global_variables_handler.h"
#include "cmsis_os.h"
#include "datalog_handler.h"
#include "error_treatment.h"
#include "global_instances.h"
#include "global_variables.h"
#include "util.h"

typedef struct {
    uint16_t value[APPS_MATRIX_LENGTH];
    float fix_mul[APPS_MATRIX_LENGTH];
    float fix_add[APPS_MATRIX_LENGTH];
} apps_ref;

uint16_t throttle_calc(uint16_t APPS_VALUE, const apps_ref* ref);
bool is_there_APPS_error();
bool is_there_BSE_error();
bool is_there_SU_F_error();

static uint16_t apps1_value;
static uint16_t apps2_value;
static uint16_t bse;
static uint16_t apps1_throttle_percent = 0;
static uint16_t apps2_throttle_percent = 0;
static uint16_t throttle_percent = 0;

void throttle_read(void* argument) {
    UNUSED(argument);

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        apps1_value = ADC_DMA_buffer[APPS1_E];
        apps2_value = ADC_DMA_buffer[APPS2_E];
        bse = ADC_DMA_buffer[BRAKE_E];

        const static apps_ref apps1_ref = {APPS1_REF, APPS1_FIX_MUL,
                                           APPS1_FIX_ADD}; // valores de referencia e parametros
        const static apps_ref apps2_ref = {APPS2_REF, APPS2_FIX_MUL,
                                           APPS2_FIX_ADD}; // para o calculo da porcentagem

        apps1_throttle_percent = throttle_calc(
            apps1_value, &apps1_ref); // calcula a porcentagem do pedal a partir do APPS1
        apps2_throttle_percent = throttle_calc(
            apps2_value, &apps2_ref); // calcula a porcentagem do pedal a partir do APPS2
        throttle_percent =
            avg(apps1_throttle_percent,
                apps2_throttle_percent); // a porcentagem final do pedal eh a media dos dois apps

        set_global_var_value(BRAKE_STATUS, (bse > BRAKE_ACTIVE));
        set_global_var_value(THROTTLE_STATUS, (throttle_percent > 0));

        log_data(ID_BRAKE, get_global_var_value(BRAKE_STATUS));

        check_for_errors(is_there_APPS_error,
                         APPS_ERROR_FLAG); // verifica a plausabilidade dos APPSs
        check_for_errors_with_timeout(is_there_APPS_error, APPS_ERROR_FLAG, tim_APPS_errorHandle,
                                      APPS_ERROR_TIMER); // verifica a plausabilidade do BSE
        check_for_errors_with_timeout(
            is_there_SU_F_error, SU_F_ERROR_FLAG, tim_SU_F_errorHandle,
            SU_F_ERROR_TIMER); // verifica se a placa de freio esta em curto

        uint16_t message = throttle_percent;
        osMessageQueuePut(q_throttle_controlHandle, &message, 0, 0U);

        osDelay(THROTTLE_DELAY);
    }
}

uint16_t throttle_calc(uint16_t apps_value, const apps_ref* ref) {
    if (apps_value < 0) {
        return 0;
    }
    if (apps_value >= ref->value[APPS_MATRIX_LENGTH - 1]) {
        return 1000;
    }

    for (int i = 0; i < APPS_MATRIX_LENGTH;
         i++) {                           // compara o valor do APPS com as faixas de acionamento
        if (apps_value < ref->value[i]) { // para escolher quais parametros utilizar durante o
            return (uint16_t)(ref->fix_mul[i] * (float)apps_value +
                              ref->fix_add[i]); // calculo da porcentagem
        }
    }

    return 0;
}

bool is_there_APPS_error() {       // Regulamento: T.4.2 (2021)
    if (apps2_value > APPS2_MAX    // Se o valor de APPS2 for acima do seu maximo
        || apps2_value < APPS2_MIN // ou abaixo do seu minimo
        || apps1_value > APPS1_MAX // Se o valor de APPS1 for acima do seu maximo
        || apps1_value < APPS1_MIN // ou abaixo do seu minimo
        || abs(apps1_throttle_percent - apps2_throttle_percent) / 10 >
               APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE) { // Se os APPS1 e APPS2 discordarem em mais
                                                         // de 10%
        return true;
    }
    return false;
}

bool is_there_BSE_error() {
    bool is_BSE_error_active = get_individual_flag(ECU_control_event_id, BSE_ERROR_FLAG);
    if (is_BSE_error_active) {
        return (throttle_percent >= APPS_05_PERCENT); // Regulamento: EV.5.7.2 (2021)
    }
    return (throttle_percent > APPS_25_PERCENT &&
            bse > BRAKE_ACTIVE); // Regulamento: EV.5.7.1 (2021)
}

bool is_there_SU_F_error() { return (bse > SU_F_ERROR); }
