/*
 * leds.c
 *  Cores:
 *      Preto: -
 *      Vermelho:   Erro
 *      Verde:      Enduro
 *      Azul:       Autox
 *      Amarelo:    Erro leve (APPS ou BSE Plausability)
 *      Roxo:       Aceleracao
 *      Ciano:      Skidpad
 *      Branco:     Aviso (REGEN ou Controle dinamicos)
 *
 *
 *  Created on: May 12, 2021
 *      Author: Felipe Telles
 */

#include "leds/rgb_led_handler.h"

#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

osStatus_t resultado;

void write_rgb_color(rgb_t rgb_gpio);
void write_debug_color(rgb_t rgb_gpio);
rgb_t get_rgb_color(cores_t color);
void blink_rgb(uint32_t delay);

osStatus_t set_rgb_led(cores_t* pattern, control_rgb_led_e control,
                       uint8_t sizeOfPattern) {
    rgb_led_message_t message = {pattern, control, sizeOfPattern};
    resultado = osMessageQueuePut(q_rgb_led_messageHandle, &message, 0, 0U);
}

void write_pattern(rgb_led_message_t message) {
    for (int i = 0; i < message.sizeOfPattern; ++i) {
        write_rgb_color(get_rgb_color(message.pattern[i]));
        if (message.sizeOfPattern > 1) {
            blink_rgb(RGB_BLINK500_DELAY);
        }
    }
}

void rgb_led(void* argument) {
    UNUSED(argument);

    rgb_led_message_t message;

    for (;;) {
       // ECU_ENABLE_BREAKPOINT_DEBUG();

        // espera RTD ser setado ou timeout estourar
        switch (
            osMessageQueueGet(q_rgb_led_messageHandle, &message, NULL, RGB_BLINK_DELAY)) {

            // caso timeout estore vai piscar o led, indicando que ta fora do RTD
            case osErrorTimeout: write_pattern(message); break;

            default:
                switch (message.control) {
                    case FIXED:
                        for (;;) {
//                            const uint32_t message_count =
//                                osMessageQueueGetCount(q_rgb_led_messageHandle);
                            write_pattern(message);
                            //if (message_count > 0) {
                                osMessageQueueGet(q_rgb_led_messageHandle, &message, NULL,
                                                  osWaitForever);
                                if (message.control == BLINK200
                                    || message.control == NO_CHANGE) {
                                    break;
                                }
                           // }
                        }
                        break;
                    default: write_pattern(message); break;
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
    // write_debug_color(rgb_gpio); //apenas para debug
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
