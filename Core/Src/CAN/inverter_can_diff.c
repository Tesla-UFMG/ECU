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

void inverter_diff(void* argument) {
    UNUSED(argument);

    for (;;) {
        //ECU_ENABLE_BREAKPOINT_DEBUG();


        osMessageQueueGet(q_ids_can_inverterHandle, &hfdcan1, NULL, osWaitForever); //colocar timeout
        //ideia : task que pega os primeiros elementos a sairem dessa queue até
        //que receba um id do inversor esquerdo (range 100-103) e do direito (range 200 a 203)
        //sinal = RxHeader.Identifier;
        // id inversor esquerdo 0x300 e id inversor direito 0x301;
    }
}




