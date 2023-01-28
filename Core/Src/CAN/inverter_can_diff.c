/*
 * inverter_diff.c
 *
 *  Created on: Jan 24, 2023
 *      Author: caio
 */

#include "CAN/CAN_IDs.h"
#include "cmsis_os.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"


static FDCAN_RxHeaderTypeDef RxHeader;

// Function that gets ID's from CAN messages received from inverters and sets the flag if both are active
void inverter_diff(void* argument) {

    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        uint32_t id = RxHeader.Identifier;

        osMessageQueueGet(q_ids_can_inverterHandle, &id, NULL, osWaitForever);

        bool right_inv_received = false;
        bool left_inv_received  = false;

        if (id == ID_RIGHT_INVERTER) {
            right_inv_received = true;
        }

        if (id == ID_LEFT_INVERTER) {
            left_inv_received = true;
        }

        if ((left_inv_received && right_inv_received) == true) {
            osThreadFlagsSet(t_inverter_comm_errorHandle, INVERTER_CAN_ACTIVE);
        }
    }
}
