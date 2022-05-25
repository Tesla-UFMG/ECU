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
        log_data(ID_SPEED_L_MOTOR, get_value_inverter(speed_m_l));
        log_data(ID_SPEED_R_MOTOR, get_value_inverter(speed_m_r));
        log_data(ID_POWER_L_MOTOR, get_value_inverter(power_m_l));
        log_data(ID_POWER_R_MOTOR, get_value_inverter(power_m_r));
        log_data(ID_ENERGY_L_MOTOR, get_value_inverter(energy_m_l));
        log_data(ID_ENERGY_R_MOTOR, get_value_inverter(energy_m_r));
        log_data(ID_OVERLOAD_L_MOTOR, get_value_inverter(overload_m_l));
        log_data(ID_OVERLOAD_R_MOTOR, get_value_inverter(overload_m_r));
        log_data(ID_LOST_MSG_L_MOTOR, get_value_inverter(lost_msg_m_l));
        log_data(ID_LOST_MSG_R_MOTOR, get_value_inverter(lost_msg_m_r));
        log_data(ID_BUS_OFF_L_MOTOR, get_value_inverter(bus_off_count_m_l));
        log_data(ID_BUS_OFF_R_MOTOR, get_value_inverter(bus_off_count_m_r));
        log_data(ID_CAN_STATE_L_MOTOR, get_value_inverter(can_state_m_l));
        log_data(ID_CAN_STATE_R_MOTOR, get_value_inverter(can_state_m_r));
        log_data(ID_INV_STATE_L_MOTOR, get_value_inverter(inv_state_m_l));
        log_data(ID_INV_STATE_R_MOTOR, get_value_inverter(inv_state_m_r));
        log_data(ID_FAILURE_L_MOTOR, get_value_inverter(failure_m_l));
        log_data(ID_FAILURE_R_MOTOR, get_value_inverter(failure_m_l));
        log_data(ID_ALARM_L_MOTOR, get_value_inverter(alarm_m_l));
        log_data(ID_ALARM_R_MOTOR, get_value_inverter(alarm_m_l));
        log_data(ID_TORQUE_L_MOTOR, get_value_inverter(torque_m_l));
        log_data(ID_TORQUE_R_MOTOR, get_value_inverter(torque_m_r));
        log_data(ID_CURRENT_R_MOTOR, get_value_inverter(current_m_l));
        log_data(ID_CURRENT_L_MOTOR, get_value_inverter(current_m_r));
        log_data(ID_TEMPERATURE1_L, get_value_inverter(temp_mosf1_m_l));
        log_data(ID_TEMPERATURE1_R, get_value_inverter(temp_mosf1_m_r));
        log_data(ID_TEMPERATURE2_L, get_value_inverter(temp_mosf2_m_l));
        log_data(ID_TEMPERATURE2_R, get_value_inverter(temp_mosf2_m_r));
        osDelay(DATALOGGER_DELAY);
    }
}
