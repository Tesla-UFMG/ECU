/*
 * global_variables.c
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */


#include "global_variables.h"

volatile uint16_t g_motor_speed[2];

volatile race_mode_t g_race_mode = ENDURO;

volatile uint16_t ADC_DMA_buffer[ADC_LINES];

volatile uint16_t throttle_percent = 0;
volatile uint8_t brake_status = 0;
volatile uint8_t apps_status = 0;

modos aceleracao, skidpad, autox, enduro, reverse, erro;

modos modo_selecionado;
