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

		osSemaphoreRelease(s_Allowed_change_modeHandle); 							//libera semáforo que permite a mudança de modos

		for(;;) {
			osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever); //espera receber flag q o botão de RTD foi pressionado
			uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);			//obtem todas as flags
			error_flags = error_flags & ALL_SEVERE_ERROR_FLAG; 						//filtra apenas flags de erros severos, ignorando as outras
			if(brake_status && !error_flags && (g_race_mode != ERRO))
				break;																//sai do for infinito caso tudo esteja certo para acionar RTD
			else
				set_debugleds(DEBUGLED1, BLINK, 2);									//envia uma mensagem de alerta caso n seja possível acionar RTD
		}

		//seta a flag de RTD
		osEventFlagsSet(ECU_control_event_id, RTD_FLAG);					//Seta flag de RTD
		osSemaphoreAcquire(s_Allowed_change_modeHandle, osWaitForever);		//Bloqueia mudança de modo
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
		modo_selecionado = erro; 							//seta modo_selecionado como erro
		g_race_mode = ERRO;
		osEventFlagsClear(ECU_control_event_id, RTD_FLAG);	//limpa flag de RTD
	}

	void aciona_sirene(){
		HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_SET);
		osDelay(tempo_sirene);
		HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_RESET);
	}
