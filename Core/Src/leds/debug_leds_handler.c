/*
 * debug_leds_handler.c
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

        //wait for the mensagge arrive
        osMessageQueueGet(q_debug_leds_messageHandle, &message, NULL, osWaitForever);
        switch (message.control) {
            case ON:
            	// turn on the selected LED
                HAL_GPIO_WritePin(GPIOE, message.lednumber, GPIO_PIN_RESET);
                break;
            case OFF:
            	//turn off the selected LED
                HAL_GPIO_WritePin(GPIOE, message.lednumber, GPIO_PIN_SET);
                break;
            case TOGGLE:
            	// toggle the selected LED
                HAL_GPIO_TogglePin(GPIOE, message.lednumber);
                break;
            case BLINK:
            	//blinks the selected LED, the chosen number of times
            	//delays ensure it is possible to count how many times it blinked
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
