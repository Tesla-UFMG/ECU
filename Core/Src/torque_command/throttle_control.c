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
        //waits for an message with the APPS value
        osMessageQueueGet(q_throttle_controlHandle, &message, NULL, osWaitForever);
        /*checks whether the RTD flag is active and the BSE and APPS flags
         are cleaned, if true, updates the throttle_percent with the received message value.
         .Else its updates to 0*/
        bool is_apps_error_present =
            get_individual_flag(e_ECU_control_flagsHandle, APPS_ERROR_FLAG);
        bool is_bse_error_present =
            get_individual_flag(e_ECU_control_flagsHandle, BSE_ERROR_FLAG);

        if (is_RTD_active() && !is_apps_error_present && !is_bse_error_present) {
            set_global_var_value(THROTTLE_PERCENT, (THROTTLE_PERCENT_t)message);
        } else {
            set_global_var_value(THROTTLE_PERCENT, (THROTTLE_PERCENT_t)0);
        }

        log_data(ID_THROTTLE, get_global_var_value(THROTTLE_PERCENT));
    }
}