/*
 * throttle_handler.c
 *
 *  Created on: Jun 30, 2020
 *      Author: renanmoreira
 */

#include "throttle_handler.h"
#include "CAN/CAN_IDs.h"
#include "CAN/inverter_can.h"
#include "util.h"

void throttle_handler(void *argument) {
    UNUSED(argument);
    ;

    torque_message_t message;
    for (;;) {
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        // espera indefinidamente ate alguma mensagem de torque$ chegar
        osMessageQueueGet(q_torque_messageHandle, &message, NULL, osWaitForever);

        for (int i = 0; i < TORQUE_MESSAGE_RESEND_TIMES; i++) {
            void inverter_transmit(torque_message_t * message);
            inverter_transmit(&message);

#if (TORQUE_MESSAGE_DELAY > 0)
            osDelay(TORQUE_MESSAGE_DELAY);
#endif
        }
    }
}

void inverter_transmit(torque_message_t *message) {
    uint16_t vet_tx[4];

    vet_tx[0] = message->parameters;
    vet_tx[1] = message->torque_ref[R_MOTOR];
    vet_tx[2] = message->neg_torque_ref[R_MOTOR];
    vet_tx[3] = message->speed_ref[R_MOTOR];
    inverter_can_transmit(ID_RIGHT_INVERTER, vet_tx);

    vet_tx[1] = message->torque_ref[L_MOTOR];
    vet_tx[2] = message->neg_torque_ref[L_MOTOR];
    vet_tx[3] = message->speed_ref[L_MOTOR];
    inverter_can_transmit(ID_LEFT_INVERTER, vet_tx);

    vet_tx[0] = 1 << 8;
    vet_tx[1] = 0;
    vet_tx[2] = message->torque_ref[L_MOTOR];
    vet_tx[3] = message->torque_ref[R_MOTOR];
    inverter_can_transmit(ID_COMM_FLAG, vet_tx);
}
