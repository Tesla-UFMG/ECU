/*
 * throttle_handler.c
 *
 *  Created on: Jun 30, 2020
 *      Author: renanmoreira
 */

#include "throttle_handler.h"

void throttle_handler(void *argument) {
	torque_message_t message;
	for (;;) {
		osMessageQueueGet(q_torque_messageHandle, &message, NULL, osWaitForever); //espera até alguma mensagem chegar

		#if (TORQUE_MESSAGE_RESEND_TIMES > 1)
		for (int i = 0; i < TORQUE_MESSAGE_RESEND_TIMES; i++) {
		#endif

			//TODO: LOGICA PARA COLOCAR MENSAGEM NA FILA DE ENVIO CAN

			#if (TORQUE_MESSAGE_DELAY > 0)
			osDelay(TORQUE_MESSAGE_DELAY);
			#endif
		#if (TORQUE_MESSAGE_RESEND_TIMES > 1)
		}
		#endif
	}
}
