/*
 * inverter_diff.c
 *
 *  Created on: Jan 24, 2023
 *      Author: caio
 */


#include "CAN/inverter_can.h"

#include "CAN/CAN_handler.h"
#include "CAN/inverter_can_data_manager.h"
#include "leds/debug_leds_handler.h"
#include "util/error_treatment.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"
#include "cmsis_os.h"
#include "CAN/CAN_IDs.h"
#include "util/global_variables.h"


extern FDCAN_HandleTypeDef hfdcan1;

static FDCAN_RxHeaderTypeDef RxHeader;

void inverter_diff(void* argument) {
    UNUSED(argument);

    for (;;) {
        //ECU_ENABLE_BREAKPOINT_DEBUG();


    	uint32_t id = RxHeader.Identifier;		// a variável 'id' recebe o identificador da mensagem

    	osMessageQueueGet(q_ids_can_inverterHandle, &hfdcan1, NULL, osWaitForever); // pega a can dos dois inversores da fila de mensagem.
    																				// colocar timeout.


        bool right_inv_received = false;	// bools que indicam se chegou alguma mensagem
        bool left_inv_received = false;		// de cada inversor (vide estrutura condicional abaixo)

        if (id == ID_RIGHT_INVERTER) {
        	right_inv_received = true;
        }

        if (id == ID_LEFT_INVERTER) {
        	left_inv_received = true;
        }

        if ( (left_inv_received && right_inv_received) == true){			// se chegar mensagem dos dois inversores, a flag é setada
		osThreadFlagsSet(t_inverter_comm_errorHandle, INVERTER_CAN_ACTIVE);
        }


    }
}




