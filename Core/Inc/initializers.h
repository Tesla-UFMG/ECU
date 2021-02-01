/*
 * initializers.h
 *
 *  Created on: 11 de mai de 2020
 *      Author: renanmoreira
 */

#ifndef INITIALIZERS_H_
#define INITIALIZERS_H_


#include "stm32h7xx.h"

void init_NVIC_priorities();
void init_ADC_DMA(ADC_HandleTypeDef* hadc);
void init_CAN();

#endif /* INITIALIZERS_H_ */
