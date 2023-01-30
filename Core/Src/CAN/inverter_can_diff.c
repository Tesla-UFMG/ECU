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

#define POS_LEFT_INV_ERROR 1
#define POS_RIGHT_INV_ERROR 2


static FDCAN_RxHeaderTypeDef RxHeader;

// Function that gets ID's from CAN messages received from inverters and sets the flag if both are active
void inverter_diff(void* argument) {

    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        uint32_t id;	//pq foi inicializada com esse valor?
        const bool left_inv_received;
        const bool right_inv_received;
        const bool left_inv_error;
        const bool right_inv_error;
        uint8_t inv_error_type;

        //while(timer aleatorio){  <- COLOCO ESSE WHILE AQ?

        	id = RxHeader.Identifier;

        	osMessageQueueGet(q_ids_can_inverterHandle, &id, NULL, osWaitForever);

        	left_inv_received = (id  >= 100 && id <= 103);
        	right_inv_received = (id  >= 200 && id <= 203);

        }

        left_inv_error = (!left_inv_received);
        right_inv_error = (!right_inv_received);

        if	(left_inv_error) {
        	inv_error_type = POS_LEFT_INV_ERROR;
        }

        if	(right_inv_error) {
        	inv_error_type = POS_RIGHT_INV_ERROR;
        }

        //while(timer aleatorio){  <- OU COLOCO ESSE WHILE AQ?
        switch(inv_error_type){
        	case POS_LEFT_INV_ERROR:
				//manda esse erro pra can monitor sla?
				break;
        	case POS_RIGHT_INV_ERROR:
				//rt
				break;
        	default:
        		osThreadFlagsSet(t_inverter_comm_errorHandle, INVERTER_CAN_ACTIVE);

        }
    }
}
