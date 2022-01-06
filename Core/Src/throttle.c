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

uint16_t throttle_calc(uint16_t APPS_VALUE, uint16_t *ref_value, float *fix_mul, float *fix_add);
bool is_there_APPS_error();
bool is_there_BSE_error();
bool is_there_SU_F_error();

static uint16_t apps1_value;
static uint16_t apps2_value;
static uint16_t bse;
static uint16_t apps1_throttle_percent = 0;
static uint16_t apps2_throttle_percent = 0;
static uint16_t throttle_percent = 0;

void throttle_read(void *argument) {

    for (;;) {

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        apps1_value = ADC_DMA_buffer[APPS1_E];
        apps2_value = ADC_DMA_buffer[APPS2_E];
        bse         = ADC_DMA_buffer[BRAKE_E];

        apps_ref apps1_ref = {APPS1_REF, APPS1_FIX_MUL, APPS1_FIX_ADD};     //valores de referencia e parametros
        apps_ref apps2_ref = {APPS2_REF, APPS2_FIX_MUL, APPS2_FIX_ADD};     //para o calculo da porcentagem.

        apps1_throttle_percent = throttle_calc(apps1_value,apps1_ref.value,apps1_ref.fix_mul,apps1_ref.fix_add);    //calcula a porcentagem do pedal a partir do APPS1
        apps2_throttle_percent = throttle_calc(apps2_value,apps2_ref.value,apps2_ref.fix_mul,apps2_ref.fix_add);    //calcula a porcentagem do pedal a partir do APPS2
        throttle_percent = (apps1_throttle_percent + apps2_throttle_percent)/2;                                     //a porcentagem final do pedal é a media dos dois apps

        set_global_var_value(BRAKE_STATUS, (bse > BRAKE_ACTIVE));
        set_global_var_value(THROTTLE_STATUS, (throttle_percent > 0));

        log_data(ID_BRAKE, get_global_var_value(BRAKE_STATUS));

        check_for_errors(is_there_APPS_error, APPS_ERROR_FLAG);                                                         //verifica a plausabilidade dos APPSs
        check_for_errors_with_timeout(is_there_APPS_error, APPS_ERROR_FLAG, tim_APPS_errorHandle, APPS_ERROR_TIMER);    //verifica a plausabilidade do BSE
        check_for_errors_with_timeout(is_there_SU_F_error, SU_F_ERROR_FLAG, tim_SU_F_errorHandle, SU_F_ERROR_TIMER);    //verifica se a placa de freio está em curto

        uint16_t message = throttle_percent;
        osMessageQueuePut(q_throttle_controlHandle, &message, 0, 0U);

        osDelay(THROTTLE_DELAY);
    }
}

uint16_t throttle_calc(uint16_t apps_value, uint16_t *ref_value, float *fix_mul, float *fix_add) {
    if(apps_value < 0)
        return 0;
    else if(apps_value >= ref_value[APPS_MATRIX_LENGTH-1])
        return 1000;
    else
        for (int i=0; i<APPS_MATRIX_LENGTH; i++) {                  // compara o valor do APPS com as faixas de acionamento
            if(apps_value < ref_value[i]) {                         // para escolher quais parametros utilizar durante o
                return (fix_mul[i] * apps_value + fix_add[i]);      // calculo da porcentagem.
        }
    }
    return 0;
}

bool is_there_APPS_error() {            //Regulamento: T.4.2 (2021)
    if (    apps2_value > APPS2_MAX     //Se o valor de APPS2 for acima do seu máximo
         || apps2_value < APPS2_MIN     //ou abaixo do seu mínimo
         || apps1_value > APPS1_MAX     //Se o valor de APPS1 for acima do seu máximo
         || apps1_value < APPS1_MIN     //ou abaixo do seu mínimo
         || fabs(apps1_throttle_percent - apps2_throttle_percent)/10 > APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE)    //Se os APPS1 e APPS2 discordarem em mais de 10%
        return true;
    else
        return false;
}

bool is_there_BSE_error() {
    bool is_BSE_error_active = get_individual_flag(ECU_control_event_id, BSE_ERROR_FLAG);
    if (is_BSE_error_active)
        return (throttle_percent >= APPS_05_PERCENT);                         //Regulamento: EV.5.7.2 (2021)
    else
        return (throttle_percent > APPS_25_PERCENT && bse > BRAKE_ACTIVE);    //Regulamento: EV.5.7.1 (2021)
}

bool is_there_SU_F_error() {
    return (bse > SU_F_ERROR);
}
