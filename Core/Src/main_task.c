/*
 * main_task.c
 *
 *  Created on: Jun 3, 2020
 *      Author: renanmoreira
 */

#include "main_task.h"

void main_task(void *argument) {
	for(;;) {
		//espera receber flag q o botão de RTD foi pressionado
		osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);

		//seta a flag de RTD
		osEventFlagsSet(ECU_control_event_id, RTD_FLAG);
	}
}
