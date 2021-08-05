/*
 * external_interrupt_handler.c
 *
 *  Created on: May 25, 2020
 *      Author: renanmoreira
 */

#include "external_interrupt_handler.h"

speed_message_t speed_message;

//implementa a funcao homonima da HAL, que trata interrupcao por pino
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//	__disable_irq(); //desabilita interrupcoes momentaneamente para nao haver nenhuma outra interrupcao antes do calculo da velocidade
	switch (GPIO_Pin) {
		case S_VEL1_Pin:
		case S_VEL2_Pin:
		case S_VEL3_Pin:
		case S_VEL4_Pin:
			speed_message.pin = GPIO_Pin;
			speed_message.tim_count = htim2.Instance->CNT;
			osMessageQueuePut(q_speed_messageHandle, &speed_message, 0U, 0);
			break;

		case B_RTD_Pin:
			osThreadFlagsSet(t_main_taskHandle, RTD_BTN_PRESSED_FLAG);
			break;

		case B_MODO_Pin:
			g_race_mode++;
			osSemaphoreRelease(s_mode_buttonHandle);
			break;

		default:
			break;
	}
//	__enable_irq(); //habilita interrupcoes
}


