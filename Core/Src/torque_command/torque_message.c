/*
 * torque_message.c
 *
 *  Created on: Jun 30, 2020
 *      Author: renanmoreira
 */

#include "torque_command/torque_message.h"

#include "CAN/CAN_IDs.h"
#include "CAN/inverter_can.h"
#include "util/util.h"


void inverter_transmit(torque_message_t* message) {
    //The parameter in this function is a pointer to a struct that has the torque reference, negative torque reference and speed reference for both motors, as well as some other important parameters.

    //This variable will be used to receive and transmit the data to the inverters
    //TODO (Guilherme): Conferir se o tipo uint_16_t é o adequado, já que alguns dos dados são de 32 bits (olhar em global_definitions.h)
    uint16_t vet_tx[4];

    //Stores important parameters
    vet_tx[0] = message->parameters;

    //Stores data correspondent to the right inverter
    vet_tx[1] = message->torque_ref[R_MOTOR];
    vet_tx[2] = message->neg_torque_ref[R_MOTOR];
    vet_tx[3] = message->speed_ref[R_MOTOR];
    inverter_can_transmit(ID_RIGHT_INVERTER, vet_tx);

    //Stores data correspondent to the left inverter
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


void torque_message(void* argument) {
    UNUSED(argument);

    torque_message_t message;
    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        //waits forever until a torque message is received
        osMessageQueueGet(q_torque_messageHandle, &message, NULL, osWaitForever);

        for (int i = 0; i < TORQUE_MESSAGE_RESEND_TIMES; i++) {
            inverter_transmit(&message);

            #if (TORQUE_MESSAGE_DELAY > 0)
                        osDelay(TORQUE_MESSAGE_DELAY);
            #endif
        }
    }
}


