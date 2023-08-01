/*
 * global_variables.c
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */

#include "util/global_variables.h"

volatile uint16_t ADC_DMA_buffer[ADC_LINES];

volatile modos aceleracao;
volatile modos skidpad;
volatile modos autox;
volatile modos enduro;
volatile modos reverse;
volatile modos erro;
