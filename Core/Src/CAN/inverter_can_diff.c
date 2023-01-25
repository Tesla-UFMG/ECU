/*
 * inverter_diff.c
 *
 *  Created on: Jan 24, 2023
 *      Author: caio
 */



void inverter_diff(void* argument) {
    UNUSED(argument);

    for (;;) {
        //ECU_ENABLE_BREAKPOINT_DEBUG();


        osMessageQueuePut(q_ids_can_inverterHandle, &hfdcan1, NULL,
                          osWaitForever);
        //ideia : task que pega os primeiros elementos a sairem dessa queue até
        //que receba um id do inversor esquerdo (range 100-103) e do direito (range 200 a 203)

    }
}




