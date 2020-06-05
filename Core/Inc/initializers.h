/*
 * initializers.h
 *
 *  Created on: 11 de mai de 2020
 *      Author: renanmoreira
 */

#ifndef INITIALIZERS_H_
#define INITIALIZERS_H_

#include "main.h"
#include "stm32h7xx_hal.h"
#include "global_definitions.h"
#include "global_variables.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal_adc.h"

void init_NVIC_priorities();
void init_ADC_DMA(ADC_HandleTypeDef* hadc);

#endif /* INITIALIZERS_H_ */
