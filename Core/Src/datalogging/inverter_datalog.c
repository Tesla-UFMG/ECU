/*
 * inverter_datalog.c
 *
 *  Created on: Jan 15th 2022
 *      Author: GiovanniPasa
 */

#include "datalogging/inverter_datalog.h"

#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "CAN/inverter_can_data_manager.h"
#include "cmsis_os.h"
#include "datalogging/datalog_acquisition.h"
#include "datalogging/datalog_handler.h"
#include "datalogging/datalogger.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

void inverter_datalog() {
    for (;;) {
        log_data(ID_SPEED_L_MOTOR, get_value(speed_m_l));
        log_data(ID_SPEED_R_MOTOR, get_value(speed_m_r));
        log_data(ID_POWER_L_MOTOR, get_value(power_m_l));
        log_data(ID_POWER_R_MOTOR, get_value(power_m_r));
        log_data(ID_ENERGY_L_MOTOR, get_value(energy_m_l));
        log_data(ID_ENERGY_R_MOTOR, get_value(energy_m_r));
        log_data(ID_OVERLOAD_L_MOTOR, get_value(overload_m_l));
        log_data(ID_OVERLOAD_R_MOTOR, get_value(overload_m_r));
        log_data(ID_LOST_MSG_L_MOTOR, get_value(lost_msg_m_l));
        log_data(ID_LOST_MSG_R_MOTOR, get_value(lost_msg_m_r));
        log_data(ID_BUS_OFF_L_MOTOR, get_value(bus_off_count_m_l));
        log_data(ID_BUS_OFF_R_MOTOR, get_value(bus_off_count_m_r));
        log_data(ID_CAN_STATE_L_MOTOR, get_value(can_state_m_l));
        log_data(ID_CAN_STATE_R_MOTOR, get_value(can_state_m_r));
        log_data(ID_INV_STATE_L_MOTOR, get_value(inv_state_m_l));
        log_data(ID_INV_STATE_R_MOTOR, get_value(inv_state_m_r));
        log_data(ID_FAILURE_L_MOTOR, get_value(failure_m_l));
        log_data(ID_FAILURE_R_MOTOR, get_value(failure_m_l));
        log_data(ID_ALARM_L_MOTOR, get_value(alarm_m_l));
        log_data(ID_ALARM_R_MOTOR, get_value(alarm_m_l));
        log_data(ID_TORQUE_L_MOTOR, get_value(torque_m_l));
        log_data(ID_TORQUE_R_MOTOR, get_value(torque_m_r));
        log_data(ID_CURRENT_R_MOTOR, get_value(current_m_l));
        log_data(ID_CURRENT_L_MOTOR, get_value(current_m_r));
        log_data(ID_TEMPERATURE1_L, get_value(temp_mosf1_m_l));
        log_data(ID_TEMPERATURE1_R, get_value(temp_mosf1_m_r));
        log_data(ID_TEMPERATURE2_L, get_value(temp_mosf2_m_l));
        log_data(ID_TEMPERATURE2_R, get_value(temp_mosf2_m_r));
        osDelay(DATALOGGER_DELAY);
    }
}
