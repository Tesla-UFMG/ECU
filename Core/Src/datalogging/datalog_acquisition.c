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
    modos modo_dl;
    uint16_t flag_ecu_1;
    uint16_t flag_ecu_2;
    for (;;) {

        modo_dl = get_global_var_value(SELECTED_MODE);
        // divisao da flag de 32 bits em duas mensagens de 16bits, para isso o valor da
        // flag eh passado e shiftado para a direita 16x, apos ser shiftado o que
        // permaneceu nos 16 bits mais significativos eh ignorado a partir da operacao and
        flag_ecu_1 = (osEventFlagsGet(ECU_control_event_id) >> 16);
        flag_ecu_1 = flag_ecu_1 & 0xffff;
        // o mesmo ocorre na segunda parte da msg porem sem o shift.
        flag_ecu_2 = (osEventFlagsGet(ECU_control_event_id) & 0xffff);

        log_data(ID_MODE, modo_dl.mode);
        log_data(ID_TORQUE_GAIN, modo_dl.torq_gain);
        log_data(ID_REGEN_BRAKE_STATE, modo_dl.freio_regen);
        log_data(ID_CONTROL_EVENT_FLAG_1, flag_ecu_1);
        log_data(ID_CONTROL_EVENT_FLAG_2, flag_ecu_2);

        osDelay(DATALOGGER_DELAY);
    }
}
