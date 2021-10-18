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
		flag_ecu_1 = (get_individual_flag(ECU_control_event_id, 0xffffffff) >> 16) & 0xffff;
		flag_ecu_2 = (get_individual_flag(ECU_control_event_id, 0xffffffff) & 0xffff);

		log_data(ID_MODE, modo_dl.mode);
		log_data(ID_TORQUE_GAIN, modo_dl.torq_gain);
		log_data(ID_REGEN_BRAKE_STATE, modo_dl.freio_regen);
		log_data(ID_CONTROL_EVENT_FLAG_1, flag_ecu_1);
		log_data(ID_CONTROL_EVENT_FLAG_2, flag_ecu_2);

		osDelay(DATALOGGER_DELAY);
	}
}
