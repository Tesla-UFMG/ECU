/*
 * global_variables.c
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */


#include "util/global_variables.h"

volatile uint16_t ADC_DMA_buffer[ADC_LINES];

modos aceleracao, skidpad, autox, enduro, reverse, erro;