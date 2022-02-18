/*
 * throttle_control.c
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */

#include "torque_command/throttle_control.h"

#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_instances.h"
#include "util/util.h"

void throttle_control(void* argument) {
    UNUSED(argument);

    for (;;) {
        uint16_t message;

        // espera por uma mensagem com o valor de APPS
        osMessageQueueGet(q_throttle_controlHandle, &message, NULL, osWaitForever);

        // verifica a se a flag de RTD esta ativada e se as flags de erro de BSE e APPS
        // estao desativadas, caso sim, atualiza o valor de throttle_percent para o da
        // mensagem, caso nao atualiza para 0
        bool is_RTD_active = get_individual_flag(ECU_control_event_id, RTD_FLAG);
        bool is_apps_error_present =
            get_individual_flag(ECU_control_event_id, APPS_ERROR_FLAG);
        bool is_bse_error_present =
            get_individual_flag(ECU_control_event_id, BSE_ERROR_FLAG);

        if (is_RTD_active && !is_apps_error_present && !is_bse_error_present) {
            set_global_var_value(THROTTLE_PERCENT, message);
        } else {
            set_global_var_value(THROTTLE_PERCENT, 0);
        }

        log_data(ID_THROTTLE, get_global_var_value(THROTTLE_PERCENT));
    }
}
