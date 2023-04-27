/*
 * datalog_acquisition.c
 *
 *  Created on: Oct 18th 2021
 *      Author: GiovanniPasa
 */
#include "datalogging/datalog_acquisition.h"

#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "datalogging/datalogger.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

void datalog_acquisition() {
    modes modo_dl;
    uint16_t flag_ecu_1;
    uint16_t flag_ecu_2;
    for (;;) {

        modo_dl = get_global_var_value(SELECTED_MODE);
        // division of the 32-bit flag into two 16-bit messages
        // for this the value of flag is passed and shifted to the right 16x
        // after being shifted what remained in the most significant 16 bits is ignored
        // from the 'and' operation
        flag_ecu_1 = (osEventFlagsGet(e_ECU_control_flagsHandle) >> 16);
        flag_ecu_1 = flag_ecu_1 & 0xffff;
        // the same occurs in the second part of the msg but without the shift.
        flag_ecu_2 = (osEventFlagsGet(e_ECU_control_flagsHandle) & 0xffff);

        log_data(ID_MODE, modo_dl.mode);
        log_data(ID_TORQUE_GAIN, modo_dl.torq_gain);
        log_data(ID_REGEN_BRAKE_STATE, modo_dl.brake_regen);
        log_data(ID_CONTROL_EVENT_FLAG_1, flag_ecu_1);
        log_data(ID_CONTROL_EVENT_FLAG_2, flag_ecu_2);

        osDelay(DATALOGGER_DELAY);
    }
}
