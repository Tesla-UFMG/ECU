/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_manager.h"
#include "cmsis_os.h"
#include "DynamicControls/lateral_control.h"
#include "util.h"
#include "global_definitions.h"
#include "stdint.h"
#include "CMSIS_extra/global_variables_handler.h"

extern osMessageQueueId_t q_ref_torque_messageHandle;
extern osMutexId_t m_state_parameter_mutexHandle;

void torque_manager(void* argument) {
    (void) argument;

    for (;;) {
        uint32_t tick = osKernelGetTickCount(); // NOLINT(clang-analyzer-deadcode.DeadStores)

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

		//osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, osWaitForever);

        void send_ref_torque_message (const uint32_t *ref_torque);

        switch (g_control_type) {
        case LONGITUDINAL:
            // TODO(renanmoreira): implementar controle longitudinal

            break;
        case LATERAL:
            tick += LATERAL_DELAY;
            // controle lateral
            lateral_t ref_torque_decrease = lateral_control();
            // controle longitudinal
            uint32_t ref_torque_lateral[2] = {0, 0};
            void rampa_torque_lateral(lateral_t *ref_torque_decrease, uint32_t *ref_torque); // TODO(renanmoreira): utilizar rampa_torque enquanto controle longitudinal nao estiver definido
            rampa_torque_lateral(&ref_torque_decrease, ref_torque_lateral);
            // enviar referencia de torque
            send_ref_torque_message (ref_torque_lateral);

            osDelayUntil(tick);

            break;

        default: ;// rampa de torque
            uint32_t rampa_torque();
            uint32_t ref_torque = rampa_torque();
            uint32_t ref_torque_long[2] = {ref_torque, ref_torque};

            // enviar referencia de torque
            send_ref_torque_message (ref_torque_long);

            osDelay(RAMPA_DELAY);

            break;
        }

    }

}

// Rampa normal
uint32_t rampa_torque() {
    static uint32_t ref_torque_ant = 0;
    uint32_t ref_torque = (uint32_t)((float)(get_global_var_value(SELECTED_MODE).torq_gain * get_global_var_value(THROTTLE_PERCENT)) / 10);

    if (ref_torque_ant > TORQUE_INIT_LIMITE) {					    // verifica se a referencia
        if (ref_torque > ref_torque_ant + INC_TORQUE) {				// ja passou do ponto de inflexao
            ref_torque = ref_torque_ant + INC_TORQUE;				// e aplica o incremento mais agressivo
        }
    } else {
        if (ref_torque > ref_torque_ant + INC_TORQUE_INIT) {		// caso contrario usa o
            ref_torque = ref_torque_ant + INC_TORQUE_INIT;			// incremento da primeira rampa
        }
    }
    ref_torque_ant = ref_torque;									// aplica a referencia calculada

    return ref_torque;
}

// Rampa vetorizacao
void rampa_torque_lateral(lateral_t *ref_torque_decrease, uint32_t *ref_torque) {
    static uint32_t ref_torque_ant[2] = {0, 0};
    ref_torque[R_MOTOR] = (uint32_t)((float)(get_global_var_value(SELECTED_MODE).torq_gain * get_global_var_value(THROTTLE_PERCENT)) / 10);
    ref_torque[L_MOTOR] = (uint32_t)((float)(get_global_var_value(SELECTED_MODE).torq_gain * get_global_var_value(THROTTLE_PERCENT)) / 10);

    // A referencia de torque sera a referencia acima menos a referencia de decrescimento.
    // Se chegar a valores negativos, entao sera 0.
    ref_torque[ref_torque_decrease->ref_wheel] = (uint32_t) max((int32_t) (ref_torque[ref_torque_decrease->ref_wheel] - ref_torque_decrease->ref_decrease), 0);

    for (int i=0;i<2;i++) {
        if (ref_torque_ant[i] > TORQUE_INIT_LIMITE) {                       // verifica se a referencia
            if (ref_torque[i] > ref_torque_ant[i] + INC_TORQUE) {           // ja passou do ponto de inflexao
                ref_torque[i] = ref_torque_ant[i] + INC_TORQUE;             // e aplica o incremento mais agressivo
            }
        } else {
            if (ref_torque[i] > ref_torque_ant[i] + INC_TORQUE_INIT) {      // caso contrario usa o
                ref_torque[i] = ref_torque_ant[i] + INC_TORQUE_INIT;        // incremento da primeira rampa
            }
        }
        ref_torque_ant[i] = ref_torque[i];                                  // aplica a referencia calculada
    }
}

// Enviar mensagem de torque
void send_ref_torque_message (const uint32_t *ref_torque) {
    ref_torque_t ref_torque_message;
    ref_torque_message.ref_torque[R_MOTOR] = ref_torque[R_MOTOR];
    ref_torque_message.ref_torque[L_MOTOR] = ref_torque[L_MOTOR];

    osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);
}
