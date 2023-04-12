/*
 *  Created on: May 12, 2021
 *      Author: Felipe Telles
 */

#include "leds/rgb_led_handler.h"

#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

static void write_rgb_color(rgb_t rgb_gpio);
// static void write_debug_color(rgb_t rgb_gpio);
static void write_pattern(rgb_led_message_t message, int delay);
static int are_messages_available(osMessageQueueId_t q_id);
static void blink_rgb(uint32_t delay);
rgb_t get_rgb_color(colors_t color);

osStatus_t set_rgb_led(const colors_t* pattern, control_rgb_led_e control,
                       uint8_t sizeOfPattern) {
    rgb_led_message_t message;
    message.control       = control;
    message.sizeOfPattern = sizeOfPattern;
    message.pattern[0]    = pattern[0];
    message.pattern[1]    = sizeOfPattern > 1 ? pattern[1] : 0;
    message.pattern[2]    = sizeOfPattern > 2 ? pattern[2] : 0;
    return osMessageQueuePut(q_rgb_led_messageHandle, &message, 0, 0U);
}

void rgb_led(void* argument) {
    UNUSED(argument);

    rgb_led_message_t message;

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        // wait until the RTD is set or to timeout overflow

        switch (osMessageQueueGet(q_rgb_led_messageHandle, &message, NULL, RGB_TIMEOUT)) {

            // if the timeout overflows the led will blink to show the car its not in RTD
            case osErrorTimeout: write_pattern(message, RGB_BLINK_200_DELAY); break;

            default:
                if (message.control == FIXED) {
                    if (message.sizeOfPattern > 1) {
                        while (!are_messages_available(q_rgb_led_messageHandle)) {
                            write_pattern(message, RGB_BLINK_1000_DELAY);
                        }
                        break;
                    }
                    write_rgb_color(
                        get_rgb_color(message.pattern[ONE_COLOR_PATTERN_POS]));
                    while (!are_messages_available(q_rgb_led_messageHandle)) {
                        osDelay(RGB_BLINK_200_DELAY);
                    }
                }

                else if (message.control == BLINK200) {
                    write_pattern(message, RGB_BLINK_200_DELAY);
                }
                break;
        }
    }
}

void blink_rgb(uint32_t delay) {
    osDelay(delay);
    write_rgb_color(get_rgb_color(BLACK));
}

void write_pattern(rgb_led_message_t message, int delay) {
    for (int i = 0; i < message.sizeOfPattern; ++i) {
        write_rgb_color(get_rgb_color(message.pattern[i]));
        blink_rgb(delay);
    }
}

int are_messages_available(osMessageQueueId_t q_id) {
    return osMessageQueueGetCount(q_id);
}

void write_rgb_color(rgb_t rgb_gpio) {
    HAL_GPIO_WritePin(C_LED_RED_GPIO_Port, C_LED_RED_Pin, rgb_gpio.red);
    HAL_GPIO_WritePin(C_LED_GREEN_GPIO_Port, C_LED_GREEN_Pin, rgb_gpio.green);
    HAL_GPIO_WritePin(C_LED_BLUE_GPIO_Port, C_LED_BLUE_Pin, rgb_gpio.blue);
    // write_debug_color(rgb_gpio); //only for debug
}
/*
void write_debug_color(rgb_t rgb_gpio) {
    HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, !rgb_gpio.red);
    HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, !rgb_gpio.green);
    HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, !rgb_gpio.blue);
}*/

rgb_t get_rgb_color(colors_t color) {
    switch (color) {
        case BLACK: return RGB_BLACK;
        case RED: return RGB_RED;
        case GREEN: return RGB_GREEN;
        case BLUE: return RGB_BLUE;
        case YELLOW: return RGB_YELLOW;
        case PURPLE: return RGB_PURBLE;
        case CYAN: return RGB_CYAN;
        case WHITE:
        default: return RGB_WHITE;
    }
}
