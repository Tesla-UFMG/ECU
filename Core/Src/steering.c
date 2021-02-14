/*
 * steering.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "steering.h"
#include "constants.h"
#include "global_definitions.h"
#include "datalog_handler.h"

extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

volatile uint8_t internal_wheel;
volatile uint16_t steering_wheel;


void steering_read(void *argument) {
	uint16_t volante_cru;
	for(;;) {
		extern void brkpt();
		brkpt();

		volante_cru = ADC_DMA_buffer[ADC_E2_E];

		uint16_t volante_aux = volante_cru,
				 zero_aux = ZERO_VOLANTE;

		//Se o mínimo do volante for menor que 0, o sensor voltará no valor máximo do ADC
		//se isso acontecer, o valor do ADC voltará para 4095
		//então subtrai 4095 do valor lido, dando um valor negativo que pode ser aplicado na formula
		//o mesmo vale pro zero do volante
		if (VOLANTE_MIN > VOLANTE_MAX) {
			zero_aux -= 4095;
			if (volante_cru > VOLANTE_MAX)
				volante_cru -= 4095;
		}


		if (volante_cru < zero_aux) {
			steering_wheel = 0;
		}
		else{
			steering_wheel = volante_cru * GANHO_VOLANTE - ZERO_VOLANTE;
		}

		log_data(ID_STEERING_WHEEL, steering_wheel);

		//SPAN_ALINHAMENTO é apenas um span pra ainda considerar o volante no centro
		//até uma certa quantidade
		if(steering_wheel > VOLANTE_ALINHADO + SPAN_ALINHAMENTO){
			internal_wheel = ESQUERDA;
		}
		else if(steering_wheel < VOLANTE_ALINHADO - SPAN_ALINHAMENTO){
			internal_wheel = DIREITA;
		}
		else{
			internal_wheel = CENTRO;
		}

		osDelay(100);

	//	return (volante);

	}
}
