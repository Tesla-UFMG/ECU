/*
 * leds.c
 *
 *  Created on: May 12, 2021
 *      Author: Felipe Telles
 */


#include "debugleds.h"
#include "cmsis_os.h"
#include "datalog_handler.h"


osStatus_t set_debugleds(uint16_t lednumber, ControlDebugLED_e control, uint8_t amount) {
	debugled_message_t message = {lednumber, control, amount};
	return osMessageQueuePut(q_debugleds_messageHandle, &message, 0, 0U);
}

void debugleds(void *argument) {

	debugled_message_t message;

	for(;;) {
		extern void brkpt();
		osMessageQueueGet(q_debugleds_messageHandle, &message, NULL, osWaitForever); //espera até alguma mensagem chegar
		switch (message.control){
			case ON:
				HAL_GPIO_WritePin(GPIOE, message.lednumber, 0); 				//acende o led escolhido
				break;
			case OFF:
				HAL_GPIO_WritePin(GPIOE, message.lednumber, 1); 				//apaga o led escolhido
				break;
			case TOGGLE:
				HAL_GPIO_TogglePin(GPIOE, message.lednumber); 					//toggle o led escolhido
				break;
			case BLINK:
				for(uint8_t i = 0; i < message.amount; i++){ 					//pisca o led escolhido por quantas vezes selecionadas
					HAL_GPIO_WritePin(GPIOE, message.lednumber, 0);				//delays garante que é possível contar quantas vezes piscou
					osDelay(300);
					HAL_GPIO_WritePin(GPIOE, message.lednumber, 1);
					osDelay(300);
				}
				osDelay(500);
				break;
		}
	}
}
