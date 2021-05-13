/*
 * leds.c
 *
 *  Created on: May 12, 2021
 *      Author: Felipe Telles
 */


#include "leds.h"
#include "cmsis_os.h"
#include "datalog_handler.h"




void leds(void *argument) {
	cores_t message;

	for(;;) {
		extern void brkpt();
		brkpt();
		osMessageQueueGet(q_leds_messageHandle, &message, NULL, osWaitForever); //espera até alguma mensagem chegar
		switch (message){
			case DEBUG000:
				HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, 0);
				HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, 0);
				HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, 0);
				break;
			case DEBUG001:
				HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, 0);
				HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, 0);
				HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, 1);
				break;
			case DEBUG010:
				HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, 0);
				HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, 1);
				HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, 0);
				break;
			case DEBUG011:
				HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, 0);
				HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, 1);
				HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, 1);
				break;
			case DEBUG100:
				HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, 1);
				HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, 0);
				HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, 0);
				break;
			case DEBUG101:
				HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, 1);
				HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, 0);
				HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, 1);
				break;
			case DEBUG110:
				HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, 1);
				HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, 1);
				HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, 0);
				break;
			case DEBUG111:
				HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, 1);
				HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, 1);
				HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, 1);
				break;

		}

	}

}
