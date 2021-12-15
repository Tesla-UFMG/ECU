/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "throttle.h"
#include "datalog_handler.h"
#include "error_treatment.h"
#include "math.h"
#include "CMSIS_extra/global_variables_handler.h"
#include "util.h"
#include "global_variables.h"
#include "global_instances.h"
#include "cmsis_os.h"

uint16_t throttle_calc(uint16_t APPS, uint16_t *value, float *fix_mul, float *fix_add, uint16_t MAX);
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

        static uint16_t apps1_value[APPS_MATRIX_LENGTH] = {APPS1_REF};      // valores de referencia e parametros para
        static float apps1_fix_mul[APPS_MATRIX_LENGTH] = {APPS1_FIX_MUL};   // o calculo da porcentagem.
        static float apps1_fix_add[APPS_MATRIX_LENGTH] = {APPS1_FIX_ADD};   // disponíveis na planilha:
        static uint16_t apps2_value[APPS_MATRIX_LENGTH] = {APPS2_REF};      // https://onedrive.live.com/view.aspx?resid=52B70A6379F6F8AB!115743&ithint=file%2cxlsx&authkey=!AJ8JGeSHTz3KdoQ
        static float apps2_fix_mul[APPS_MATRIX_LENGTH] = {APPS2_FIX_MUL};
        static float apps2_fix_add[APPS_MATRIX_LENGTH] = {APPS2_FIX_ADD};

        apps2_throttle_percent = throttle_calc(APPS2,apps2_value,apps2_fix_mul,apps2_fix_add,APPS2_MAX_PERCENT);    //calcula a porcentagem do pedal a partir do APPS2
        apps1_calc = throttle_calc(apps2_throttle_percent,apps1_value,apps1_fix_mul,apps1_fix_add,APPS1_MAX);       //calcula a porcentagem esperada do APPS1 a partir da porcentagem calculada do APPS2

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

uint16_t throttle_calc(uint16_t APPS, uint16_t *value, float *fix_mul, float *fix_add, uint16_t MAX) {
    if(APPS < 0)
        return 0;
    else                                                                    // compara o valor do APPS com as faixas de acionamento
        for (int i=0; i<APPS_MATRIX_LENGTH; i++) {                          // para escolher quais parametros utilizar durante o
            if(APPS < value[i]) {                                           // calculo da porcentagem.
                return fmin(MAX,(fix_mul[i] * APPS + fix_add[i]));          // Se for maior que o maximo permitido, retorna maximo.
        }
    }
    return MAX;
}

bool is_there_APPS_error() {            //Regulamento: T.4.2 (2021)
    if (    APPS2 >= APPS2_MAX_VALUE    //Se o valor de APPS2 for acima do seu máximo
         || APPS1 < APPS1_MIN           //Se o valor de APPS1 for abaixo do seu mínimo
         || APPS1 > APPS1_MAX           //Se o valor de APPS1 for acima do seu máximo
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
