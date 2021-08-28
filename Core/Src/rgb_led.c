/*
 * leds.c
 *  Cores:
 *      Preto: -
 *      Vermelho:   Erro
 *      Verde:      Enduro
 *      Azul:       Autox
 *      Amarelo:    Erro leve (APPS ou BSE Plausability)
 *      Roxo:       Aceleração
 *      Ciano:      Skidpad
 *      Branco:     Aviso (REGEN ou Controle dinâmicos)
 *
 *
 *  Created on: May 12, 2021
 *      Author: Felipe Telles
 */


#include "rgb_led.h"
#include "cmsis_os.h"
#include "global_variables.h"
#include "global_instances.h"

void write_rgb_color(rgb rgb_gpio);
void write_debug_color(rgb rgb_gpio);
rgb get_rgb_color(cores_t color);

extern osMessageQueueId_t q_rgb_led_messageHandle;

osStatus_t set_rgb_led(cores_t color, control_rgb_led_e control) {
    rgb_led_message_t message = {color, control};
    return osMessageQueuePut(q_rgb_led_messageHandle, &message, 0, 0U);
}

void rgb_led(void *argument) {

    rgb_led_message_t message;

    for(;;) {
        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        switch(osMessageQueueGet(q_rgb_led_messageHandle, &message, NULL, RGB_BLINK_DELAY)){  //espera RTD ser setado ou timeout estourar

        case osErrorTimeout:                                                                   //caso timeout estore vai piscar o led, indicando que tá fora do RTD
            write_rgb_color(get_rgb_color(message.color));
            blink_rgb(RGB_BLINK_DELAY);
            break;

        default:
            switch (message.control){
                case FIXED:
                    for(;;){
                    write_rgb_color(get_rgb_color(message.color));
                    osMessageQueueGet(q_rgb_led_messageHandle, &message, NULL, osWaitForever);
                    if (message.control == BLINK200)
                        break;
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

void blink_rgb(uint32_t delay){
    osDelay(delay);
    write_rgb_color(get_rgb_color(PRETO));
}

void write_rgb_color(rgb rgb_gpio){
    HAL_GPIO_WritePin(C_LED_RED_GPIO_Port, C_LED_RED_Pin, rgb_gpio.red);
    HAL_GPIO_WritePin(C_LED_GREEN_GPIO_Port, C_LED_GREEN_Pin, rgb_gpio.green);
    HAL_GPIO_WritePin(C_LED_BLUE_GPIO_Port, C_LED_BLUE_Pin, rgb_gpio.blue);
    //write_debug_color(rgb_gpio); //apenas para debug
}

void write_debug_color(rgb rgb_gpio){
    HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, !rgb_gpio.red);
    HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, !rgb_gpio.green);
    HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, !rgb_gpio.blue);
}

rgb get_rgb_color(cores_t color){
    switch (color){
        case PRETO:
            return RGB_BLACK;
        case VERMELHO:
            return RGB_RED;
        case VERDE:
            return RGB_GREEN;
        case AZUL:
            return RGB_BLUE;
        case AMARELO:
            return RGB_YELLOW;
        case ROXO:
            return RGB_PURBLE;
        case CIANO:
            return RGB_CYAN;
        case BRANCO:
        default:
            return RGB_WHITE;
    }
}
