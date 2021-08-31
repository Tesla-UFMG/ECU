/*
 * main_task.c
 *
 *  Created on: Jun 3, 2020
 *      Author: renanmoreira
 */

#include "main_task.h"
#include "global_variables.h"
#include "global_instances.h"
#include "cmsis_os.h"
#include "RTD.h"
#include "debugleds.h"
#include "rgb_led.h"
#include "throttle.h"


void main_task(void *argument) {

	for(;;) {

        #ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

        bool is_RTD_active = (osEventFlagsGet(ECU_control_event_id) & RTD_FLAG);

        if (!is_RTD_active){
            set_rgb_led(modo_selecionado.cor, NO_CHANGE);
            osSemaphoreRelease(s_allowed_change_modeHandle);                                         //libera semáforo que permite a mudança de modos
	        for(;;) {
	            osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);              //espera receber flag q o botão de RTD foi pressionado
	            if(is_RTD_available())
	                break;                                                                           //sai do for infinito caso tudo esteja certo para acionar RTD
	            else
	                set_debugleds(DEBUGLED1,BLINK,2);                                                //envia uma mensagem de alerta caso n seja possível acionar RTD
	           }
	           set_RTD();//seta RTD
		}


		//espera por qualquer erro relatado pela ECU
		osEventFlagsWait(ECU_control_event_id, ALL_ERRORS_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
		uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);
		error_flags &= ALL_ERRORS_FLAG;                              //filtra apenas flags de erros severos, que precisam sair de RTD, ignorando as outras
		switch (error_flags) {
		    case INVERTER_COMM_ERROR_FLAG:
		        exit_RTD();
		        //check_comm();
		        break;

            case SU_F_ERROR_FLAG:
                exit_RTD();
                if (check_SU_F())
                    osDelay(20);
                else {
                    osEventFlagsClear(ECU_control_event_id, SU_F_ERROR_FLAG);
                }
                break;

		    case APPS_ERROR_FLAG:
                osEventFlagsClear(ECU_control_event_id, THROTTLE_AVAILABLE_FLAG);
                set_rgb_led(AMARELO, NO_CHANGE);
                if (check_apps())
                    osDelay(20);
                else {
                    osEventFlagsClear(ECU_control_event_id, APPS_ERROR_FLAG);
                    set_rgb_led(modo_selecionado.cor, NO_CHANGE);
                    osEventFlagsSet(ECU_control_event_id, THROTTLE_AVAILABLE_FLAG);
                }
                break;

		    case BSE_ERROR_FLAG:
		        osEventFlagsClear(ECU_control_event_id, THROTTLE_AVAILABLE_FLAG);
                set_rgb_led(AMARELO, NO_CHANGE);
                if (check_bse())
                    osDelay(20);
                else {
                    osEventFlagsClear(ECU_control_event_id, BSE_ERROR_FLAG);
                    set_rgb_led(modo_selecionado.cor, NO_CHANGE);
                    osEventFlagsSet(ECU_control_event_id, THROTTLE_AVAILABLE_FLAG);
	                }

            default:
                osDelay(100);
                break;
		}
	}
}


