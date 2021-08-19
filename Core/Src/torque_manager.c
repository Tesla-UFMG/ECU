/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_manager.h"
#include "cmsis_os.h"
#include "global_definitions.h"
#include "stdint.h"

extern volatile uint16_t throttle_percent;
extern modos modo_selecionado;
extern osMessageQueueId_t q_ref_torque_messageHandle;
extern osMutexId_t m_state_parameter_mutexHandle;

extern volatile uint16_t steering_wheel;

void torque_manager(void *argument) {

	ref_torque_t ref_torque_message;

	for (;;) {
		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		switch (g_control_type) {
		case LONGITUDINAL:
			//TODO: implementar controle longitudinal

			break;
		case LATERAL:
			//TODO: implementar controle lateral
			uint32_t rampa_torque();
			uint32_t ref_torque_init = rampa_torque(); // utilizar rampa_torque enquanto controle longitudinal nao estiver definido
			uint32_t lateral_control();
			uint32_t ref_torque_decrease = lateral_control();

			// se o sinal for positivo, a reducao sera na roda direita
			// caso contrario, sera na roda esquerda
			if (ref_torque_decrease >= 0) {
				ref_torque_message.ref_torque[R_MOTOR] = ref_torque - ref_torque_decrease;
				ref_torque_message.ref_torque[L_MOTOR] = ref_torque;
			} else {
				ref_torque_message.ref_torque[R_MOTOR] = ref_torque;
				ref_torque_message.ref_torque[L_MOTOR] = ref_torque + ref_torque_decrease;
			}

			osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);

			uint32_t tick;
			tick = osKernelGetTickCount();
			tick += LATERAL_DELAY;
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


uint32_t rampa_torque() {
	static uint32_t ref_torque_ant = 0;
	uint32_t ref_torque = (modo_selecionado.torq_gain * throttle_percent) / 10;

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
