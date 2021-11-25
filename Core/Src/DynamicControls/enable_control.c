/*
 * enable_control.c
 *
 *  Created on: Nov 25th 2021
 *      Author: GiovanniPasa
 */
#include "modo.h"
#include "cmsis_os.h"
#include "global_variables.h"
#include "global_instances.h"
#include "rgb_led.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"

void enable_dynamic_ctrl(){
	for(;;){

	osThreadFlagsWait(DYN_CTRL_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);

	bool is_RTD_active = get_individual_flag(ECU_control_event_id, RTD_FLAG);
	        if (is_RTD_active) {
	            if(get_global_var_value(DYNAMIC_CONTROL)){
	            	set_global_var_value(DYNAMIC_CONTROL, false);
	            }
	            else{
	            	set_global_var_value(DYNAMIC_CONTROL, true);
	            }
	        }
	}
}


