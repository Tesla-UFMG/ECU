/*
 * inverter_diff.c
 *
 *  Created on: Jan 24, 2023
 *      Author: caio
 */

#include "CAN/inverter_can_diff.h"

#include "CAN/CAN_IDs.h"
#include "cmsis_os.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

extern FDCAN_HandleTypeDef hfdcan1;

static FDCAN_RxHeaderTypeDef RxHeader;

void inverter_diff(void* argument) {

    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();
        // a variável 'id' recebe o identificador da mensagem
        uint32_t id = RxHeader.Identifier;

        // pega a can dos dois inversores da fila de mensagem. colocar timeout.
        osMessageQueueGet(q_ids_can_inverterHandle, &hfdcan1, NULL, osWaitForever);

        // bools que indicam se chegou alguma mensagem de cada inversor (vide estrutura
        // condicional abaixo) (static, volatile??)
        bool right_inv_received = false;
        bool left_inv_received  = false;

        if (id == ID_RIGHT_INVERTER) {
            right_inv_received = true;
        }

        if (id == ID_LEFT_INVERTER) {
            left_inv_received = true;
        }

        // se chegar mensagem dos dois inversores, a flag é setada
        if ((left_inv_received && right_inv_received) == true) {
            osThreadFlagsSet(t_inverter_comm_errorHandle, INVERTER_CAN_ACTIVE);
        }
    }
}
