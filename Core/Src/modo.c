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
        osThreadFlagsWait(MODE_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);
		osSemaphoreAcquire(s_allowed_change_modeHandle, osWaitForever);

        if (g_race_mode > AUTOX)
            g_race_mode = ENDURO;

		switch(g_race_mode){
		case ENDURO:
            modo_selecionado = enduro;
            break;
		case ACELERACAO:
		    modo_selecionado = aceleracao;
		    break;
        case SKIDPAD:
            modo_selecionado = skidpad;
            break;
        case AUTOX:
            modo_selecionado = autox;
            break;
        default:
            modo_selecionado = erro;
            break;
		}



		osSemaphoreRelease(s_allowed_change_modeHandle);
	}
}
