/*
 * global_variables.h
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_VARIABLES_H_
#define INC_GLOBAL_VARIABLES_H_

#include "global_definitions.h"

// buffer atualizado pelo DMA com dados de leitura do ADC
extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

// variavel de controle para desabilitar referencia de torque
extern volatile uint8_t g_should_disable_engines;

// variavel que guarda parametros do estado atual do veiculo
extern volatile vehicle_state_parameters_t g_vehicle_state_parameters;

extern modos aceleracao, skidpad, autox, enduro, reverse, erro;

// guarda o estado atual do veiculo (acelera, neutro, freia)
extern volatile vehicle_state_e vehicle_state;

#endif /* INC_GLOBAL_VARIABLES_H_ */
