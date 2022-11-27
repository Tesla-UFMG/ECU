/*
 * external_interrupt_handler.c
 *
 *  Created on: May 25, 2020
 *      Author: renanmoreira
 */

#include "util/external_interrupt_handler.h"

#include "cmsis_os.h"
#include "main.h"
#include "sensors/encoder_speed.h"
#include "util/global_instances.h"
#include "util/util.h"

static encoder_int_message_t speed_message;

/**
 * @brief implements the default HAL function, which deals with per pin interruption
 *
 * @param GPIO_Pin Origin of the interruption
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    switch (GPIO_Pin) {
        case S_VEL1_Pin:
        case S_VEL2_Pin:
        case S_VEL3_Pin:
        case S_VEL4_Pin:
            speed_message.pin       = get_speed_pin(GPIO_Pin);
            speed_message.tim_count = htim2.Instance->CNT;
            osMessageQueuePut(q_encoder_int_messageHandle, &speed_message, 0U, 0);
            break;

        default: break;
    }
}
