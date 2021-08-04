/*
 * main_task.c
 *
 *  Created on: Jun 3, 2020
 *      Author: renanmoreira
 */

#include "main_task.h"
#include "global_variables.h"
#include "main.h"

extern bool RTD;

void main_task(void *argument) {
	for(;;) {

		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		//espera receber flag q o botão de RTD foi pressionado
		for(;;) {
			osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);
			uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);
			error_flags = error_flags & ALL_SEVERE_ERROR_FLAG; //filtra apenas flags de erros, ignorando as outras
			if(/*brake_status && */!error_flags)
				break;
			else
				if(HAL_GPIO_ReadPin(B_MODO_GPIO_Port, B_MODO_Pin)){
					osEventFlagsClear(ECU_control_event_id, ALL_SEVERE_ERROR_FLAG);
					osDelay(1000);
					g_race_mode = ENDURO;
				}

		}

		//seta a flag de RTD
		osEventFlagsSet(ECU_control_event_id, RTD_FLAG);
		modo_ativado = modo_selecionado;

		//espera por qualquer erro relatado pela ECU
		osEventFlagsWait(ECU_control_event_id, ALL_SEVERE_ERROR_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
		uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);
		error_flags = error_flags & ALL_SEVERE_ERROR_FLAG; //filtra apenas flags de erros, ignorando as outras
		switch (error_flags) {
			case APPS_ERROR_FLAG:
				//TODO: tratar erro de APPS
				break;
			case INVERTER_COMM_ERROR_FLAG:
				g_race_mode = ERRO;
				modo_ativado = erro;
				osSemaphoreRelease(s_mode_buttonHandle); //seta modo_selecionado como erro
				osEventFlagsClear(ECU_control_event_id, RTD_FLAG);
				//TODO: tratar erro de comunicacao com o inversor
				break;
		}
		//TODO: revisar lógica de esperar por novo RTD. Talvez n seja necessário
	}
}
