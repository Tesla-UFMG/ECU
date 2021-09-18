/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_manager.h"
#include "cmsis_os.h"
#include "global_variables.h"
#include "DynamicControls/lateral_control.h"
#include "DynamicControls/longitudinal_control.h"

extern volatile float g_wheel_speed[4];
extern volatile uint16_t steering_wheel;
extern volatile uint8_t internal_wheel;
extern volatile uint16_t gyro_yaw; // ainda nao existe
extern volatile uint16_t throttle_percent;
extern modos modo_selecionado;
extern osMessageQueueId_t q_ref_torque_messageHandle;
extern osMutexId_t m_state_parameter_mutexHandle;

void torque_manager(void *argument) {

    ref_torque_t ref_torque_message;

    for (;;) {
        uint32_t tick;
        tick = osKernelGetTickCount();

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        switch (g_control_type) {
        case LONGITUDINAL:
        	tick += LONGITUDINAL_DELAY;
        	longitudinal_t torque_decrease_longitudinal = longitudinal_control(g_wheel_speed);
        	uint32_t ref_torque_longitudinal[2] = {0,0};
        	void rampa_torque_longitudinal(longitudinal_t *torque_decrease_longitudinal, uint32_t *ref_torque); // inicialmente usar rampa para evitar erros
        	rampa_torque_longitudinal(&torque_decrease_longitudinal, ref_torque_longitudinal);                  // catastroficos em testes de bancada

        	ref_torque_message.ref_torque[R_MOTOR] = ref_torque_longitudinal[R_MOTOR];
        	ref_torque_message.ref_torque[L_MOTOR] = ref_torque_longitudinal[L_MOTOR];

        	osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);

        	osDelayUntil(tick);

            break;
        case LATERAL: ;
            tick += LATERAL_DELAY;

            lateral_t torque_decrease_lateral = lateral_control(g_wheel_speed, &steering_wheel, &internal_wheel, &gyro_yaw);
            uint32_t ref_torque_lateral[2] = {0, 0};
            void rampa_torque_lateral(lateral_t *torque_decrease_lateral, uint32_t *ref_torque); // TODO: utilizar rampa_torque enquanto controle longitudinal nao estiver definido
            rampa_torque_lateral(&torque_decrease_lateral, ref_torque_lateral);

            ref_torque_message.ref_torque[R_MOTOR] = ref_torque_lateral[R_MOTOR];
            ref_torque_message.ref_torque[L_MOTOR] = ref_torque_lateral[L_MOTOR];

            osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);

            osDelayUntil(tick);

            break;

        default: ;// rampa de torque
            uint32_t rampa_torque();
            uint32_t ref_torque = rampa_torque();

            ref_torque_message.ref_torque[R_MOTOR] = ref_torque;
            ref_torque_message.ref_torque[L_MOTOR] = ref_torque;

            osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);

            osDelay(RAMPA_DELAY);

            break;
        }

    }

}

// Rampa normal
uint32_t rampa_torque() {
    static uint32_t ref_torque_ant = 0;
    uint32_t ref_torque = (uint32_t)((float)(modo_selecionado.torq_gain * throttle_percent) / 10);

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

// Rampa vetorização
void rampa_torque_lateral(lateral_t *ref_torque_decrease, uint32_t *ref_torque) {
    static uint32_t ref_torque_ant[2] = {0, 0};
    ref_torque[R_MOTOR] = (uint32_t)((float)(modo_selecionado.torq_gain * throttle_percent) / 10);
    ref_torque[L_MOTOR] = (uint32_t)((float)(modo_selecionado.torq_gain * throttle_percent) / 10);

    if (ref_torque_decrease->ref_wheel == R_MOTOR)
        ref_torque[R_MOTOR] = (uint32_t)((float)ref_torque[R_MOTOR] - ref_torque_decrease->ref_decrease);
    else
        ref_torque[L_MOTOR] = (uint32_t)((float)ref_torque[L_MOTOR] - ref_torque_decrease->ref_decrease);

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
// Rampa CA
void rampa_torque_longitudinal(longitudinal_t *ref_torque_decrease, uint32_t *ref_torque) {
    static uint32_t ref_torque_ant[2] = {0, 0};
    ref_torque[R_MOTOR] = (uint32_t)((float)(modo_selecionado.torq_gain * throttle_percent) / 10);
    ref_torque[L_MOTOR] = (uint32_t)((float)(modo_selecionado.torq_gain * throttle_percent) / 10);

    ref_torque[R_MOTOR] = (uint32_t)((float)ref_torque[R_MOTOR] - ref_torque_decrease->ref_decrease_R);
    ref_torque[L_MOTOR] = (uint32_t)((float)ref_torque[L_MOTOR] - ref_torque_decrease->ref_decrease_L);

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
