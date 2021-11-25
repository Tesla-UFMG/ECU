/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_manager.h"
#include "cmsis_os.h"
#include "global_definitions.h"
#include "stdint.h"
#include "CMSIS_extra/global_variables_handler.h"


extern osMessageQueueId_t q_ref_torque_messageHandle;
extern osMutexId_t m_state_parameter_mutexHandle;

void torque_manager(void *argument) {

	ref_torque_t ref_torque_message;

	for (;;) {
		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		//osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, osWaitForever);

		if(get_global_var_value(DYNAMIC_CONTROL)){
			switch(get_global_var_value(SELECTED_MODE).dyn_control){
		 	 case LONGITUDINAL:
		 		 //TODO: implementar controle longitudinal

		 		 break;
		 	 case LATERAL:
		 		 //TODO: implementar controle lateral

		 		 break;
		 	 default: ;
		     // rampa de torque
				uint32_t rampa_torque();
				uint32_t ref_torque = rampa_torque();

				ref_torque_message.ref_torque[R_MOTOR] = ref_torque;
				ref_torque_message.ref_torque[L_MOTOR] = ref_torque;

				osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);

				osDelay(RAMPA_DELAY);
				break;
			}
		}
		else {
		     // rampa de torque
			uint32_t rampa_torque();
			uint32_t ref_torque = rampa_torque();

			ref_torque_message.ref_torque[R_MOTOR] = ref_torque;
			ref_torque_message.ref_torque[L_MOTOR] = ref_torque;

			osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);

			osDelay(RAMPA_DELAY);
		}



	}

}


uint32_t rampa_torque() {
	static uint32_t ref_torque_ant = 0;
	uint32_t ref_torque = (get_global_var_value(SELECTED_MODE).torq_gain * get_global_var_value(THROTTLE_PERCENT)) / 10;

	if (ref_torque_ant > TORQUE_INIT_LIMITE) {					    // verifica se a referencia
		if (ref_torque > ref_torque_ant + INC_TORQUE) {				// ja passou do ponto de inflexao
			ref_torque = ref_torque_ant + INC_TORQUE;				// e aplica o incremento mais agressivo
		}
	} else {
		if (ref_torque > ref_torque_ant + INC_TORQUE_INIT) {		// caso contrario usa o
			ref_torque = ref_torque_ant + INC_TORQUE_INIT;			// incremento da primeira rampa
		}
	}
	ref_torque_ant = ref_torque;									// aplica a referencia calculada

	return ref_torque;
}

