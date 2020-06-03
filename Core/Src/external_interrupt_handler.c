/*
 * external_interrupt_handler.c
 *
 *  Created on: May 25, 2020
 *      Author: renanmoreira
 */

#include "external_interrupt_handler.h"

speed_message_t aux_message;

//implementa a funcao homonima da HAL, que trata interrupcao por pino
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	__disable_irq(); //desabilita interrupcoes momentaneamente para nao haver nenhuma outra interrupcao antes do calculo da velocidade
	switch (GPIO_Pin) {
		case S_VEL1_Pin:
		case S_VEL2_Pin:
		case S_VEL3_Pin:
		case S_VEL4_Pin:
			aux_message.pin = GPIO_Pin;
			aux_message.tim_count = htim2.Instance->CNT;
			osMessageQueuePut(q_speed_messageHandle, &aux_message, 0U, 0);
			break;

		case B_RTD_Pin:

			break;

		case B_MODO_Pin:
			g_race_mode++;
			if (g_race_mode > 4)
				g_race_mode = 0;
			break;

		default:
			break;
	}
	__enable_irq(); //habilita interrupcoes
}


