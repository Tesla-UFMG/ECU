/*
 * leds.c
 *
 *  Created on: May 12, 2021
 *      Author: Felipe Telles
 */

#include "leds/debug_leds_handler.h"

#include "cmsis_os.h"
#include "util/global_definitions.h"
#include "util/global_variables.h"
#include "util/util.h"

extern osMessageQueueId_t q_debug_leds_messageHandle;

osStatus_t set_debugleds(uint16_t lednumber, ControlDebugLED_e control, uint8_t amount) {
    debug_led_message_t message = {lednumber, control, amount};
    return osMessageQueuePut(q_debug_leds_messageHandle, &message, 0, 0U);
}

void debug_leds(void* argument) {
    UNUSED(argument);

    debug_led_message_t message;

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        // espera ate alguma mensagem chegar
        osMessageQueueGet(q_debug_leds_messageHandle, &message, NULL, osWaitForever);
        switch (message.control) {
            case ON:
                // acende o led escolhido
                HAL_GPIO_WritePin(GPIOE, message.lednumber, GPIO_PIN_RESET);
                break;
            case OFF:
                // apaga o led escolhido
                HAL_GPIO_WritePin(GPIOE, message.lednumber, GPIO_PIN_SET);
                break;
            case TOGGLE:
                // toggle o led escolhido
                HAL_GPIO_TogglePin(GPIOE, message.lednumber);
                break;
            case BLINK:
                // pisca o led escolhido, pelo valor de vezes escolhido
                // delays garante que eh possivel contar quantas vezes piscou
                for (uint8_t i = 0; i < message.amount; i++) {
                    HAL_GPIO_WritePin(GPIOE, message.lednumber, GPIO_PIN_RESET);
                    osDelay(300);
                    HAL_GPIO_WritePin(GPIOE, message.lednumber, GPIO_PIN_SET);
                    osDelay(300);
                }
                osDelay(500);
                break;
            case FASTBLINK:
                HAL_GPIO_WritePin(GPIOE, message.lednumber, GPIO_PIN_RESET);
                osDelay(message.amount);
                HAL_GPIO_WritePin(GPIOE, message.lednumber, GPIO_PIN_SET);
                osDelay(message.amount / 2);
                break;
        }
    }
}
