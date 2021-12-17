/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_manager.h"
#include "cmsis_os.h"
#include "DynamicControls/lateral_control.h"
#include "DynamicControls/longitudinal_control.h"
#include "util.h"
#include "global_definitions.h"
#include "stdint.h"
#include "CMSIS_extra/global_variables_handler.h"

extern osMessageQueueId_t q_ref_torque_messageHandle;
extern osMutexId_t m_state_parameter_mutexHandle;

uint32_t ref_torque[2] = {0,0};


void torque_manager(void *argument) {

    for (;;) {
        uint32_t tick = osKernelGetTickCount();

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

		//osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, osWaitForever);

        void send_ref_torque_message (uint32_t *ref_torque);

        switch (g_control_type) {
        case LONGITUDINAL:
            tick += LONGITUDINAL_DELAY;
            longitudinal_control_result_t result = longitudinal_control();                           // Controller
            uint32_t torque_decrease_longitudinal[2] = {0, 0};
            torque_decrease_longitudinal[R_MOTOR] = result.torque_decrease[R_MOTOR];
            torque_decrease_longitudinal[L_MOTOR] = result.torque_decrease[L_MOTOR];
            void rampa_torque(uint32_t *ref_torque_decrease, uint32_t *ref_torque); // TODO: remover rampa com testes de bancada
            rampa_torque(torque_decrease_longitudinal, ref_torque);
            send_ref_torque_message(ref_torque);                                  //sends the torque command to the inverter

            osDelayUntil(tick);

            break;

        case LATERAL:
            tick += LATERAL_DELAY;
            lateral_t lateral_decrease = lateral_control(); //controller
            uint32_t torque_decrease_lateral[2] = {0, 0};       //value of torque to be decreased
            torque_decrease_lateral[lateral_decrease.ref_wheel] = lateral_decrease.ref_decrease;    // TODO: fazer integração dos dois controles
            void rampa_torque(uint32_t *ref_torque_decrease, uint32_t *ref_torque); // TODO: utilizar rampa_torque enquanto controle longitudinal nao estiver definido
            rampa_torque(torque_decrease_lateral, ref_torque);
            // enviar referencia de torque
            send_ref_torque_message(ref_torque);

            osDelayUntil(tick);

            break;

        default: ;// rampa de torque
            uint32_t null_decrease[2]= {0,0};
            void rampa_torque(uint32_t *ref_torque_decrease, uint32_t *ref_torque);

            rampa_torque(null_decrease, ref_torque);

            // enviar referencia de torque
            send_ref_torque_message(ref_torque);

            osDelay(RAMPA_DELAY);

            break;
        }

    }

}

// Rampa normal
void rampa_torque(uint32_t *ref_torque_decrease, uint32_t *ref_torque) {
    static uint32_t ref_torque_ant[2] = {0,0};

    float torque = ((get_global_var_value(SELECTED_MODE).torq_gain * get_global_var_value(THROTTLE_PERCENT)) / 10);
    ref_torque[R_MOTOR] = (uint32_t)(max(0, (float)(torque - ref_torque_decrease[R_MOTOR])));
    ref_torque[L_MOTOR] = (uint32_t)(max(0, (float)(torque - ref_torque_decrease[L_MOTOR])));


    for (int i=0;i<2;i++) {
        uint32_t torque_increment = (ref_torque_ant[i] > TORQUE_INIT_LIMITE) ? INC_TORQUE : INC_TORQUE_INIT; // verifica se a referencia a passou do ponto de inflexao
        ref_torque[i] = min(ref_torque[i], (ref_torque_ant[i] + torque_increment));                            // e aplica o incremento mais agressivo (INC_TORQUE)
        ref_torque_ant[i] = ref_torque[i];                                                                   // caso contrario usa o incremento da primeira rampa (INC_TORQUE_INIT)
    }
}

// Enviar mensagem de torque
void send_ref_torque_message (uint32_t *ref_torque) {
    ref_torque_t ref_torque_message;
    ref_torque_message.ref_torque[R_MOTOR] = ref_torque[R_MOTOR];
    ref_torque_message.ref_torque[L_MOTOR] = ref_torque[L_MOTOR];

    osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);
}


