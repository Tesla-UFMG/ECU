/*
 * main_task.c
 *
 *  Created on: Jun 3, 2020
 *      Author: renanmoreira
 */

#include "main_task.h"
#include "global_variables.h"
#include "debugleds.h"
#include "main.h"


void main_task(void *argument) {

	for(;;) {

		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		osSemaphoreRelease(s_Allowed_change_modeHandle);
		//espera receber flag q o botão de RTD foi pressionado
		for(;;) {
			osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);
			uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);
			error_flags = error_flags & ALL_SEVERE_ERROR_FLAG; //filtra apenas flags de erros, ignorando as outras
			if(brake_status && !error_flags && (g_race_mode != ERRO))
				break;
			else
				set_debugleds(DEBUGLED1, BLINK, 2);
		}

		//seta a flag de RTD
		osEventFlagsSet(ECU_control_event_id, RTD_FLAG);
		osSemaphoreAcquire(s_Allowed_change_modeHandle, osWaitForever);
		aciona_sirene();


		//espera por qualquer erro relatado pela ECU
		osEventFlagsWait(ECU_control_event_id, ALL_SEVERE_ERROR_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
		uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);
		error_flags = error_flags & ALL_SEVERE_ERROR_FLAG; //filtra apenas flags de erros severos, que precisam sair de RTD, ignorando as outras
		switch (error_flags) {
			case INVERTER_COMM_ERROR_FLAG:
				exit_RTD();
				break;
		}
	}
}

	void exit_RTD(){
		modo_selecionado = erro; //seta modo_selecionado como erro
		g_race_mode = ERRO;
		osEventFlagsClear(ECU_control_event_id, RTD_FLAG);
	}

	void aciona_sirene(){
		HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_SET);
		osDelay(200);
		HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_RESET);
	}
