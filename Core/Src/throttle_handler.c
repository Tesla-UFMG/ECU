/*
 * throttle_handler.c
 *
 *  Created on: Jun 30, 2020
 *      Author: renanmoreira
 */

#include "throttle_handler.h"
#include "CAN/inverter_can.h"

void throttle_handler(void *argument) {
	torque_message_t message;
	for (;;) {
		osMessageQueueGet(q_torque_messageHandle, &message, NULL, osWaitForever); //espera até alguma mensagem chegar


		for (int i = 0; i < TORQUE_MESSAGE_RESEND_TIMES; i++) {

			//TODO: TRANSCREVER DADO PARA PADRAO DA FUNCAO DE ENVIO DA CAN
			//inverter_can_transmit();

			#if (TORQUE_MESSAGE_DELAY > 0)
			osDelay(TORQUE_MESSAGE_DELAY);
			#endif
		}
	}
}
