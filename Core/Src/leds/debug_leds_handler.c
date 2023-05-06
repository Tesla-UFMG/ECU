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
#include "util/CMSIS_extra/cmsis_extra.h"
#include "util/CMSIS_extra/global_variables_handler.h"


extern osMessageQueueId_t q_debug_leds_messageHandle;

osStatus_t set_debugleds(uint16_t lednumber, ControlDebugLED_e control, uint8_t amount) {
    debug_led_message_t message = {lednumber, control, amount};
    return osMessageQueuePutOverwrite(q_debug_leds_messageHandle, &message, 0);
}

void debug_leds(void* argument) {
    UNUSED(argument);

    debug_led_message_t message;

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        // wait until some message arrives
        osMessageQueueGet(q_debug_leds_messageHandle, &message, NULL, osWaitForever);
        switch (message.control) {
            case ON:
                // lights up the chosen led
                HAL_GPIO_WritePin(GPIOE, message.led_number, GPIO_PIN_RESET);
                break;
            case OFF:
                // turn off the chosen led
                HAL_GPIO_WritePin(GPIOE, message.led_number, GPIO_PIN_SET);
                break;
            case TOGGLE:
                // toggle the chosen led
                HAL_GPIO_TogglePin(GPIOE, message.led_number);
                break;
            case BLINK:
                // blink the chosen led, by the chosen value of times
                // delays ensures that it is possible to count how many times it blinked
                for (uint8_t i = 0; i < message.amount; i++) {
                    HAL_GPIO_WritePin(GPIOE, message.led_number, GPIO_PIN_RESET);
                    osDelay(300);
                    HAL_GPIO_WritePin(GPIOE, message.led_number, GPIO_PIN_SET);
                    osDelay(300);
                }
                osDelay(500);
                break;
            case FASTBLINK:
                HAL_GPIO_WritePin(GPIOE, message.led_number, GPIO_PIN_RESET);
                osDelay(message.amount);
                HAL_GPIO_WritePin(GPIOE, message.led_number, GPIO_PIN_SET);
                osDelay(message.amount / 2);
                break;
        }
    }
}
