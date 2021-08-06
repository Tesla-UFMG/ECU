/*
 * modo.c
 *
 *  Created on: Jul 13, 2021
 *      Author: Felipe Telles
 */


#include "modo.h"
#include "cmsis_os.h"
#include "global_variables.h"
#include "global_instances.h"



void seleciona_modo(void *argument) {

	for(;;) {

		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		//espera um semáforo liberado por interrupção e espera está autorizado a mudar de modo
		osSemaphoreAcquire(s_mode_buttonHandle, osWaitForever);
		osSemaphoreAcquire(s_Allowed_change_modeHandle, osWaitForever);

			if (g_race_mode > AUTOX)
				g_race_mode = ENDURO;

			if(g_race_mode == ENDURO) {
				modo_selecionado = enduro;
			}
			else if(g_race_mode == ACELERACAO) {
				modo_selecionado = aceleracao;
			}
			else if(g_race_mode == SKIDPAD) {
				modo_selecionado = skidpad;
			}
			else if(g_race_mode == AUTOX) {
				modo_selecionado = autox;
			}
			else
				modo_selecionado = erro;

		osSemaphoreRelease(s_Allowed_change_modeHandle);
	}
}
