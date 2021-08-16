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

void main_task(void *argument) {

	for(;;) {

		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

        osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);

		//espera por qualquer erro relatado pela ECU
		osEventFlagsWait(ECU_control_event_id, ALL_ERRORS_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
		uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);
		error_flags &= ALL_ERRORS_FLAG;                              //filtra apenas flags de erros severos, que precisam sair de RTD, ignorando as outras
		switch (error_flags) {
		    case INVERTER_COMM_ERROR_FLAG:
		        exit_RTD();
		        break;
//            case APPS_ERROR_FLAG:
//                osDelay(100);
//                break;
//            case BSE_ERROR_FLAG:
//                osDelay(100);
//                break;
            default:
                osDelay(100);
                break;

		}
	}
}


