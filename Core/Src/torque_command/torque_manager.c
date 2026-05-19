/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_command/torque_manager.h"

#include "cmsis_os.h"
#include "dynamic_controls/lateral_control.h"
#include "dynamic_controls/longitudinal_control.h"
#include "stdint.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

extern osMessageQueueId_t q_ref_torque_messageHandle;

void torque_manager(void* argument) {
    UNUSED(argument);

    uint32_t ref_torque[2] = {0, 0};

    for (;;) {
        // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
        uint32_t tick = osKernelGetTickCount();

        ECU_ENABLE_BREAKPOINT_DEBUG();

        const bool is_DYNAMIC_CONTROL_active =
        get_individual_flag(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);

        select_dynamic_control(is_DYNAMIC_CONTROL_active);

        // todo (João Pedro): add new "case's" when the integration of controls is
        // implemented

        switch (g_control_type) {
            case LATERAL: // TODO(giovanni): do the integration of the two controllers
                tick += LATERAL_DELAY;
                lateral_result_t result_lateral = lateral_control();
                // TODO(giovanni): use torque_ramp while longitudinal control is not defined
                torque_ramp(ref_torque, result_lateral.torque_decrease);

                // sends the reference torque
                send_ref_torque_message(ref_torque);
            
                osDelayUntil(tick);

                break;

            case LONGITUDINAL:
                tick += LONGITUDINAL_DELAY;
                longitudinal_control_result_t result_longitudinal = longitudinal_control();
                // TODO(giovanni): remove ramp with bench tests
                torque_ramp(ref_torque, result_longitudinal.torque_decrease);
                // sends the torque command to the inverter
                send_ref_torque_message(ref_torque);

                osDelayUntil(tick);

                break;

                //TODO (Guilherme): Abaixo está mais uma das possíveis alterações a serem feitas para a integração dos controles.
                //                  Essa parte pode se tornar mais complexa caso o delay dos controles seja diferente então é necessário ter mais cuidado.
                //                  Nesse sentido, a minha sugestão é apenas uma possibilidade simples de implementação que só funciona caso os delays sejam iguais.

            case BOTH_CONTROLS:
                tick += LATERAL_DELAY;//Could be longitudinal as well (since they are equal)
                lateral_result_t lateral_result = lateral_control();
                longitudinal_control_result_t longitudinal_result = longitudinal_control();
                // TODO (Guilherme): Pode ser interressante criar um tipo para armazenar os resultados dos dois controles, isso é puramente estético mas tornaria o código mais intuitivo.
                lateral_result_t result;
                result.torque_decrease[R_MOTOR] = lateral_result.torque_decrease[R_MOTOR] + longitudinal_result.torque_decrease[R_MOTOR];
                result.torque_decrease[L_MOTOR] = lateral_result.torque_decrease[L_MOTOR] + longitudinal_result.torque_decrease[L_MOTOR];

                torque_ramp(ref_torque, result.torque_decrease);

                // sends the reference torque
                send_ref_torque_message(ref_torque);

                osDelayUntil(tick);

                break;

            default: // torque ramp
                torque_ramp(ref_torque, NULL);

                // sends the reference torque
                send_ref_torque_message(ref_torque);

                osDelay(RAMP_DELAY);

                break;
        }
    }
}

// normal ramp
static void torque_ramp(uint32_t* ref_torque, const double* ref_torque_decrease) {
    static uint32_t ref_torque_ant[2] = {0, 0};
    double desired_torque[2];
    bool should_decrease = (ref_torque_decrease != NULL);

    //Torque requested by the pilot.
    //double torque = ((double)(get_global_var_value(SELECTED_MODE).torq_gain
    //  * get_global_var_value(THROTTLE_PERCENT))
    //  / 10);

    // Maps throttle_percent [0,1] to torque range [0, tor_max].
    double torque = (double) (get_global_var_value(SELECTED_MODE).tor_max
    *get_global_var_value(THROTTLE_PERCENT)) /1000;



    //Torque after the decrease of the dynamic control, if it is active. If the control is not active, the torque is not decreased.
    desired_torque[R_MOTOR] = (uint32_t)(max(
        0, (float)(torque - (should_decrease ? ref_torque_decrease[R_MOTOR] : 0))));
    desired_torque[L_MOTOR] = (uint32_t)(max(
        0, (float)(torque - (should_decrease ? ref_torque_decrease[L_MOTOR] : 0))));

    for (int i = 0; i < 2; i++) {
        /*checks whether the reference passed the inflection point and applies the
         * aggressive increment(INC_TORQUE). Otherwise, uses the initial
         * ramp increment(INC_TORQUE_INIT)*/
        uint32_t torque_increment = (ref_torque_ant[i] > TORQUE_INIT_LIMIT) ? INC_TORQUE : INC_TORQUE_INIT;
        ref_torque[i] = min(desired_torque[i], (ref_torque_ant[i] + torque_increment));
        ref_torque_ant[i] = ref_torque[i];
    }
}


// sends the torque message
static void send_ref_torque_message(const uint32_t* ref_torque) {
    ref_torque_t ref_torque_message;
    ref_torque_message.ref_torque[R_MOTOR] = ref_torque[R_MOTOR];
    ref_torque_message.ref_torque[L_MOTOR] = ref_torque[L_MOTOR];

    osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);
}


static void select_dynamic_control(bool is_DYNAMIC_CONTROL_active) {

    if (is_DYNAMIC_CONTROL_active) {
        if (get_global_var_value(SELECTED_MODE).dif_elt == 1
            && get_global_var_value(SELECTED_MODE).traction_control == 0) {
            g_control_type = LATERAL;
            }
            if (get_global_var_value(SELECTED_MODE).dif_elt == 0
                && get_global_var_value(SELECTED_MODE).traction_control == 1) {
                g_control_type = LONGITUDINAL;
                }
                //TODO (Guilherme): Abaixo está uma das possíveis alterações a serem feitas para a integração dos controles.
                if (get_global_var_value(SELECTED_MODE).dif_elt == 1
                    && get_global_var_value(SELECTED_MODE).traction_control == 1) {
                    g_control_type = BOTH_CONTROLS;
                    }
    } else {
        g_control_type = NO_CONTROL;
    }
}


