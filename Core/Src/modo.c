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
	race_mode_t g_race_mode = 0;

	for(;;) {
		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		osSemaphoreAcquire(s_mode_buttonHandle, osWaitForever);

		g_race_mode++;
		if (g_race_mode > 4)
			g_race_mode = 1;

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
	}
}