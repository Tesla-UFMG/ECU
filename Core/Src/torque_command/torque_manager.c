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
#include "util/util.h"
#include "driver_settings/dynamic_controls_choice.h"

extern osMessageQueueId_t q_ref_torque_messageHandle;
extern osMutexId_t m_state_parameter_mutexHandle;

void torque_manager(void* argument) {
    UNUSED(argument);

    uint32_t ref_torque[2] = {0, 0};

    for (;;) {
        // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
        uint32_t tick = osKernelGetTickCount();

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        void rampa_torque(uint32_t * ref_torque, const double* ref_torque_decrease);
        void send_ref_torque_message(const uint32_t* ref_torque);

        DYNAMIC_CONTROL_t g_control_type = get_global_var_value(DYNAMIC_CONTROL);

        switch (g_control_type) {
            case LONGITUDINAL:
                tick += LONGITUDINAL_DELAY;
                longitudinal_control_result_t result = longitudinal_control();
                // TODO(giovanni): remover rampa com testes de bancada
                rampa_torque(ref_torque, result.torque_decrease);
                // sends the torque command to the inverter
                send_ref_torque_message(ref_torque);

                osDelayUntil(tick);

                break;

            case LATERAL:
                // TODO(giovanni): fazer integracao dos dois controles
                tick += LATERAL_DELAY;
                lateral_result_t result_lateral = lateral_control();
                // TODO(giovanni): utilizar rampa_torque enquanto controle longitudinal
                // nao estiver definido
                rampa_torque(ref_torque, result_lateral.torque_decrease);

                // enviar referencia de torque
                send_ref_torque_message(ref_torque);

                osDelayUntil(tick);

                break;

            default:; // rampa de torque
                rampa_torque(ref_torque, NULL);

                // enviar referencia de torque
                send_ref_torque_message(ref_torque);

                osDelay(RAMPA_DELAY);

                break;
        }
    }
}

// Rampa normal
void rampa_torque(uint32_t* ref_torque, const double* ref_torque_decrease) {
    static uint32_t ref_torque_ant[2] = {0, 0};
    double desired_torque[2];
    bool should_decrease = (ref_torque_decrease != NULL);

    double torque = ((double)(get_global_var_value(SELECTED_MODE).torq_gain
                              * get_global_var_value(THROTTLE_PERCENT))
                     / 10);

    desired_torque[R_MOTOR] = (uint32_t)(max(
        0, (float)(torque - (should_decrease ? ref_torque_decrease[R_MOTOR] : 0))));
    desired_torque[L_MOTOR] = (uint32_t)(max(
        0, (float)(torque - (should_decrease ? ref_torque_decrease[L_MOTOR] : 0))));

    for (int i = 0; i < 2; i++) {
        // verifica se a referencia a passou do ponto de inflexao e aplica o incremento
        // mais agressivo (INC_TORQUE) caso contrario usa o incremento da primeira rampa
        // (INC_TORQUE_INIT)
        uint32_t torque_increment =
            (ref_torque_ant[i] > TORQUE_INIT_LIMITE) ? INC_TORQUE : INC_TORQUE_INIT;
        ref_torque[i] = min(desired_torque[i], (ref_torque_ant[i] + torque_increment));
        ref_torque_ant[i] = ref_torque[i];
    }
}

// Enviar mensagem de torque
void send_ref_torque_message(const uint32_t* ref_torque) {
    ref_torque_t ref_torque_message;
    ref_torque_message.ref_torque[R_MOTOR] = ref_torque[R_MOTOR];
    ref_torque_message.ref_torque[L_MOTOR] = ref_torque[L_MOTOR];

    osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);
}
