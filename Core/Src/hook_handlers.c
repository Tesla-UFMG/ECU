/*
 * hook_handlers.c
 *
 *  Created on: Feb 1, 2021
 *      Author: renanmoreira
 */
#include "cmsis_os.h"
#include "stm32h7xx.h"

void vApplicationMallocFailedHook(void) {
	//erro de alocacao de memoria
}

void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName) {
	//stack overflow
}
