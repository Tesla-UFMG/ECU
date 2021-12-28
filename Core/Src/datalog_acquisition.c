/*
 * datalog_acquisition.c
 *
 *  Created on: Oct 18th 2021
 *      Author: GiovanniPasa
 */
#include "datalogger.h"
#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "global_definitions.h"
#include "cmsis_os.h"
#include "datalog_acquisition.h"
#include "datalog_handler.h"
#include "global_variables.h"
#include "global_instances.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"


void datalog_acquisition(){
	modos modo_dl;
	uint16_t flag_ecu_1;
	uint16_t flag_ecu_2;
	for(;;){

		modo_dl = get_global_var_value(SELECTED_MODE);
		//divisao da flag de 32 bits em duas mensagens de 16bits
		flag_ecu_1 = (get_individual_flag(ECU_control_event_id, 0xffffffff) >> 16);     // valor da flag eh passado e shiftado para a direita 16x
		flag_ecu_1 = flag_ecu_1 & 0xffff;                                               // apos ser shiftado o que permaneceu nos 16 bits mais significativos eh
		flag_ecu_2 = (get_individual_flag(ECU_control_event_id, 0xffffffff) & 0xffff);  // ignorado a partir da operacao and com os 16 bits menos significativos
																					    // formando assim a primeira parte da msg de 16 bits
		log_data(ID_MODE, modo_dl.mode);                                                // o mesmo ocorre na segunda parte da msg porem sem o shift.
		log_data(ID_TORQUE_GAIN, modo_dl.torq_gain);
		log_data(ID_REGEN_BRAKE_STATE, modo_dl.freio_regen);
		log_data(ID_CONTROL_EVENT_FLAG_1, flag_ecu_1);
		log_data(ID_CONTROL_EVENT_FLAG_2, flag_ecu_2);

		osDelay(DATALOGGER_DELAY);
	}
}
