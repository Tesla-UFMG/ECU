/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_manager.h"
#include "cmsis_os.h"
#include "global_definitions.h"

extern volatile uint16_t throttle_percent;
extern osMessageQueueId_t q_torque_messageHandle;
extern osMutexId_t m_state_parameter_mutexHandle;

void torque_manager(void *argument) {
	torque_message_t torque_message;

	switch (g_control_type) {
	case LONGITUDINAL:
		//TODO: implementar controle longitudinal

		break;
	case LATERAL:
		//TODO: implementar controle lateral

		break;

	default: // rampa de torque
		uint32_t rampa_torque();
		uint32_t ref_torque = rampa_torque();

		//TODO: pente fino nessa secao junto com a de controle para não haver inversao
		//de prioridade
		if (osMutexAcquire(m_state_parameter_mutexHandle, 0U) == osOK) {
			torque_message.neg_torque_ref[R_MOTOR] = g_vehicle_state_parameters.ref_torque_neg[R_MOTOR];
			torque_message.neg_torque_ref[L_MOTOR] = g_vehicle_state_parameters.ref_torque_neg[L_MOTOR];
			torque_message.speed_ref[R_MOTOR] = g_vehicle_state_parameters.ref_veloc[R_MOTOR];
			torque_message.speed_ref[L_MOTOR] = g_vehicle_state_parameters.ref_veloc[L_MOTOR];
			torque_message.parameters = g_vehicle_state_parameters.parameter_control;
		} else {
			//TODO: tratar falha em adquirir o mutex
		}
		torque_message.torque_ref[R_MOTOR] = ref_torque;
		torque_message.torque_ref[L_MOTOR] = ref_torque;

		osMessageQueuePut(q_torque_messageHandle, msg_ptr, 0, 0U);

		osDelay(30);

		break;
	}
}


uint32_t rampa_torque() {
	static uint32_t ref_torque_ant = 0;
	uint32_t ref_torque;

	if (refTorque_ant > TORQUE_INIT_LIMITE) {					    // verifica se a referencia
		if (ref_torque > refTorque_ant + INC_TORQUE) {				// ja passou do ponto de inflexao
			ref_torque = refTorque_ant + INC_TORQUE;				// e aplica o incremento mais agressivo
		}
	} else {
		if (ref_torque > ref_torque_ant + INC_TORQUE_INIT) {		// caso contrario usa o
			ref_torque = ref_torque_ant + INC_TORQUE_INIT;			// incremento da primeira rampa
		}
	}
	ref_torque_ant = ref_torque;									// aplica a referencia calculada

}
