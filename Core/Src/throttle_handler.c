/*
 * throttle_handler.c
 *
 *  Created on: Jun 30, 2020
 *      Author: renanmoreira
 */

#include "throttle_handler.h"
#include "CAN/inverter_can.h"
#include "CAN/CAN_IDs.h"

void throttle_handler(void *argument) {
	torque_message_t message;
	for (;;) {
		osMessageQueueGet(q_torque_messageHandle, &message, NULL, osWaitForever); //espera até alguma mensagem chegar


		for (int i = 0; i < TORQUE_MESSAGE_RESEND_TIMES; i++) {

			comando_inversor(&message);

			#if (TORQUE_MESSAGE_DELAY > 0)
			osDelay(TORQUE_MESSAGE_DELAY);
			#endif
		}
	}
}



void comando_inversor(torque_message_t* message) {
	uint8_t vet_tx[8];

	vet_tx[0] = *message->parameters & 0xff;
	vet_tx[1] = *message->parameters >> 8;
	vet_tx[2] = *message->torque_ref[R_MOTOR] & 0xff;
	vet_tx[3] = *message->torque_ref[R_MOTOR] >> 8 & 0xff;
	vet_tx[4] = *message->neg_torque_ref[R_MOTOR] & 0xff;
	vet_tx[5] = *message->neg_torque_ref[R_MOTOR] >> 8 & 0xff;
	vet_tx[6] = *message->speed_ref[R_MOTOR] & 0xff;
	vet_tx[7] = *message->speed_ref[R_MOTOR] >> 8 & 0xff;
	inverter_can_transmit(ID_RIGHT_INVERTER, vet_tx);

	vet_tx[2] = *message->torque_ref[L_MOTOR] & 0xff;
	vet_tx[3] = *message->torque_ref[L_MOTOR] >> 8 & 0xff;
	vet_tx[4] = *message->neg_torque_ref[L_MOTOR] & 0xff;
	vet_tx[5] = *message->neg_torque_ref[L_MOTOR] >> 8 & 0xff;
	vet_tx[6] = *message->speed_ref[L_MOTOR] & 0xff;
	vet_tx[7] = *message->speed_ref[L_MOTOR] >> 8 & 0xff;
	inverter_can_transmit(ID_LEFT_INVERTER, vet_tx);

	vetTx[0] = 1;
	vetTx[1] = 0;
	vetTx[2] = 0;
	vetTx[3] = 0;
	vetTx[4] = vel_motor[L_MOTOR] & 0xff;
	vetTx[5] = vel_motor[L_MOTOR] >> 8 & 0xff;
	vetTx[6] = vel_motor[R_MOTOR] & 0xff;
	vetTx[7] = vel_motor[R_MOTOR] >> 8 & 0xff;
	inverter_can_transmit(ID_COMM_FLAG, vet_tx);

}

