/*
 * rgb_led_handler.c
 *  Cores:
 *      Black: -
 *      Red:   	Error
 *      Green: 	Enduro
 *      Blue:  	Autox
 *      Yellow:	Slight error (APPS or BSE Plausability)
 *      Purple	Aceleration
 *      Cyan:	Skidpad
 *      White:  Warning(REGEN or Dynamic Controls)
 *
 *
 *  Created on: May 12, 2021
 *      Author: Felipe Telles
 */

#include "leds/rgb_led_handler.h"

#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

void write_rgb_color(rgb_t rgb_gpio);
void write_debug_color(rgb_t rgb_gpio);
rgb_t get_rgb_color(cores_t color);
void blink_rgb(uint32_t delay);

osStatus_t set_rgb_led(cores_t color, control_rgb_led_e control) {
    rgb_led_message_t message = {color, control};
    return osMessageQueuePut(q_rgb_led_messageHandle, &message, 0, 0U);
}

void rgb_led(void* argument) {
    UNUSED(argument);

    rgb_led_message_t message;

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        //waits for the RTD to be set or for the timeout to elapse
        switch (
            osMessageQueueGet(q_rgb_led_messageHandle, &message, NULL, RGB_BLINK_DELAY)) {
        	//if the timeout elapse it will blink an LED, indicating that is out of RTD mode
            case osErrorTimeout:
                write_rgb_color(get_rgb_color(message.color));
                blink_rgb(RGB_BLINK_DELAY);
                break;

            default:
                switch (message.control) {
                    case FIXED:
                        for (;;) {
                            write_rgb_color(get_rgb_color(message.color));
                            osMessageQueueGet(q_rgb_led_messageHandle, &message, NULL,
                                              osWaitForever);
                            if (message.control == BLINK200) {
                                break;
                            }
                        }
                        break;
                    default:
                        write_rgb_color(get_rgb_color(message.color));
                        blink_rgb(RGB_BLINK_DELAY);
                        break;
                }
                break;
        }
    }
}

void blink_rgb(uint32_t delay) {
    osDelay(delay);
    write_rgb_color(get_rgb_color(PRETO));
}

void write_rgb_color(rgb_t rgb_gpio) {
    HAL_GPIO_WritePin(C_LED_RED_GPIO_Port, C_LED_RED_Pin, rgb_gpio.red);
    HAL_GPIO_WritePin(C_LED_GREEN_GPIO_Port, C_LED_GREEN_Pin, rgb_gpio.green);
    HAL_GPIO_WritePin(C_LED_BLUE_GPIO_Port, C_LED_BLUE_Pin, rgb_gpio.blue);
    // write_debug_color(rgb_gpio); //only for debug purpose
}

void write_debug_color(rgb_t rgb_gpio) {
    HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, !rgb_gpio.red);
    HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, !rgb_gpio.green);
    HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, !rgb_gpio.blue);
}

rgb_t get_rgb_color(cores_t color) {
    switch (color) {
        case PRETO: return RGB_BLACK;
        case VERMELHO: return RGB_RED;
        case VERDE: return RGB_GREEN;
        case AZUL: return RGB_BLUE;
        case AMARELO: return RGB_YELLOW;
        case ROXO: return RGB_PURBLE;
        case CIANO: return RGB_CYAN;
        case BRANCO:
        default: return RGB_WHITE;
    }
}
