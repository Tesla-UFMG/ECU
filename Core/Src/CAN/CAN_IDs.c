/*
 * CAN_IDs.c
 *
 *  Created on: Dec 9, 2020
 *      Author: renanmoreira
 */
#include "CAN/CAN_IDs.h"

CAN_ID_t CAN_ID_map[CAN_ID_QUAN];
// TODO(renanmoreira): talvez aumentar capacidade se precisar de mais ids de debug
uint16_t
    CAN_ID_POS_INTERNAL_ID_MAP[(ECU_CAN_LAST_POPULATED_ID - ECU_CAN_INITIAL_ID + 1) * 4
                               + 8];

uint16_t convert_id_pos_to_index(uint16_t id, uint16_t pos) {
    return (id - ECU_CAN_INITIAL_ID) * 4 + pos;
}

CAN_ID_t get_CAN_ID_from_internal(uint16_t internal) {
    return CAN_ID_map[internal];
}

uint16_t get_internal_from_id_pos(uint16_t id, uint16_t pos) {
    return CAN_ID_POS_INTERNAL_ID_MAP[convert_id_pos_to_index(id, pos)];
}

// PRIMEIRA POSICAO DA MENSAGEM CAN N PODE ESTAR VAZIA

void initialize_CAN_IDs() {
    CAN_ID_map[ID_ECU_TIMER]            = (CAN_ID_t)INTERNAL_ID_ECU_TIMER;
    CAN_ID_map[ID_STEERING_WHEEL]       = (CAN_ID_t)INTERNAL_ID_STEERING_WHEEL;
    CAN_ID_map[ID_THROTTLE]             = (CAN_ID_t)INTERNAL_ID_THROTTLE;
    CAN_ID_map[ID_BRAKE]                = (CAN_ID_t)INTERNAL_ID_BRAKE;
    CAN_ID_map[ID_MODE]                 = (CAN_ID_t)INTERNAL_ID_MODE;
    CAN_ID_map[ID_DISTANCE_P_ODOM]      = (CAN_ID_t)INTERNAL_ID_DISTANCE_P_ODOM;
    CAN_ID_map[ID_DISTANCE_T_ODOM]      = (CAN_ID_t)INTERNAL_ID_DISTANCE_T_ODOM;
    CAN_ID_map[ID_TORQUE_R_MOTOR]       = (CAN_ID_t)INTERNAL_ID_TORQUE_R_MOTOR;
    CAN_ID_map[ID_TORQUE_L_MOTOR]       = (CAN_ID_t)INTERNAL_ID_TORQUE_L_MOTOR;
    CAN_ID_map[ID_REF_TORQUE_R_MOTOR]   = (CAN_ID_t)INTERNAL_ID_REF_TORQUE_R_MOTOR;
    CAN_ID_map[ID_REF_TORQUE_L_MOTOR]   = (CAN_ID_t)INTERNAL_ID_REF_TORQUE_L_MOTOR;
    CAN_ID_map[ID_REF_SPEED_R]          = (CAN_ID_t)INTERNAL_ID_REF_SPEED_R_MOTOR;
    CAN_ID_map[ID_REF_SPEED_L]          = (CAN_ID_t)INTERNAL_ID_REF_SPEED_L_MOTOR;
    CAN_ID_map[ID_CONTROL_EVENT_FLAG_1] = (CAN_ID_t)INTERNAL_ID_CONTROL_EVENT_FLAG_1;
    CAN_ID_map[ID_CONTROL_EVENT_FLAG_2] = (CAN_ID_t)INTERNAL_ID_CONTROL_EVENT_FLAG_2;
    CAN_ID_map[ID_SPEED_FL]             = (CAN_ID_t)INTERNAL_ID_SPEED_FL;
    CAN_ID_map[ID_SPEED_FR]             = (CAN_ID_t)INTERNAL_ID_SPEED_FR;
    CAN_ID_map[ID_SPEED_RR]             = (CAN_ID_t)INTERNAL_ID_SPEED_RR;
    CAN_ID_map[ID_SPEED_RL]             = (CAN_ID_t)INTERNAL_ID_SPEED_RL;
    CAN_ID_map[ID_TORQUE_GAIN]          = (CAN_ID_t)INTERNAL_ID_TORQUE_GAIN;
    CAN_ID_map[ID_CURRENT_R_MOTOR]      = (CAN_ID_t)INTERNAL_ID_CURRENT_R_MOTOR;
    CAN_ID_map[ID_CURRENT_L_MOTOR]      = (CAN_ID_t)INTERNAL_ID_CURRENT_L_MOTOR;
    CAN_ID_map[ID_TEMPERATURE1_R]       = (CAN_ID_t)INTERNAL_ID_TEMPERATURE1_R;
    CAN_ID_map[ID_TEMPERATURE2_R]       = (CAN_ID_t)INTERNAL_ID_TEMPERATURE2_R;
    CAN_ID_map[ID_TEMPERATURE1_L]       = (CAN_ID_t)INTERNAL_ID_TEMPERATURE1_L;
    CAN_ID_map[ID_TEMPERATURE2_L]       = (CAN_ID_t)INTERNAL_ID_TEMPERATURE2_L;
    CAN_ID_map[ID_STATUS_DATALOG]       = (CAN_ID_t)INTERNAL_ID_STATUS_DATALOG;
    CAN_ID_map[ID_CURRENT_FLAG]         = (CAN_ID_t)INTERNAL_ID_CURRENT_FLAG;
    CAN_ID_map[ID_PANEL_DEBUG_1]        = (CAN_ID_t)INTERNAL_ID_PANEL_DEBUG_1;
    CAN_ID_map[ID_PANEL_DEBUG_2]        = (CAN_ID_t)INTERNAL_ID_PANEL_DEBUG_2;
    CAN_ID_map[ID_PANEL_DEBUG_3]        = (CAN_ID_t)INTERNAL_ID_PANEL_DEBUG_3;
    CAN_ID_map[ID_PANEL_DEBUG_4]        = (CAN_ID_t)INTERNAL_ID_PANEL_DEBUG_4;
    CAN_ID_map[ID_REGEN_BRAKE_STATE]    = (CAN_ID_t)INTERNAL_ID_REGEN_BRAKE_STATE;
    CAN_ID_map[ID_SPEED_L_MOTOR]        = (CAN_ID_t)INTERNAL_ID_SPEED_L_MOTOR;
    CAN_ID_map[ID_SPEED_R_MOTOR]        = (CAN_ID_t)INTERNAL_ID_SPEED_R_MOTOR;
    CAN_ID_map[ID_POWER_L_MOTOR]        = (CAN_ID_t)INTERNAL_ID_POWER_L_MOTOR;
    CAN_ID_map[ID_POWER_R_MOTOR]        = (CAN_ID_t)INTERNAL_ID_POWER_R_MOTOR;
    CAN_ID_map[ID_ENERGY_L_MOTOR]       = (CAN_ID_t)INTERNAL_ID_ENERGY_L_MOTOR;
    CAN_ID_map[ID_ENERGY_R_MOTOR]       = (CAN_ID_t)INTERNAL_ID_ENERGY_R_MOTOR;
    CAN_ID_map[ID_OVERLOAD_L_MOTOR]     = (CAN_ID_t)INTERNAL_ID_OVERLOAD_L_MOTOR;
    CAN_ID_map[ID_OVERLOAD_R_MOTOR]     = (CAN_ID_t)INTERNAL_ID_OVERLOAD_R_MOTOR;
    CAN_ID_map[ID_LOST_MSG_L_MOTOR]     = (CAN_ID_t)INTERNAL_ID_LOST_MSG_L_MOTOR;
    CAN_ID_map[ID_LOST_MSG_R_MOTOR]     = (CAN_ID_t)INTERNAL_ID_LOST_MSG_R_MOTOR;
    CAN_ID_map[ID_BUS_OFF_L_MOTOR]      = (CAN_ID_t)INTERNAL_ID_BUS_OFF_L_MOTOR;
    CAN_ID_map[ID_BUS_OFF_R_MOTOR]      = (CAN_ID_t)INTERNAL_ID_BUS_OFF_R_MOTOR;
    CAN_ID_map[ID_CAN_STATE_L_MOTOR]    = (CAN_ID_t)INTERNAL_ID_CAN_STATE_L_MOTOR;
    CAN_ID_map[ID_CAN_STATE_R_MOTOR]    = (CAN_ID_t)INTERNAL_ID_CAN_STATE_R_MOTOR;
    CAN_ID_map[ID_INV_STATE_L_MOTOR]    = (CAN_ID_t)INTERNAL_ID_INV_STATE_L_MOTOR;
    CAN_ID_map[ID_INV_STATE_R_MOTOR]    = (CAN_ID_t)INTERNAL_ID_INV_STATE_R_MOTOR;
    CAN_ID_map[ID_FAILURE_L_MOTOR]      = (CAN_ID_t)INTERNAL_ID_FAILURE_L_MOTOR;
    CAN_ID_map[ID_FAILURE_R_MOTOR]      = (CAN_ID_t)INTERNAL_ID_FAILURE_R_MOTOR;
    CAN_ID_map[ID_ALARM_L_MOTOR]        = (CAN_ID_t)INTERNAL_ID_ALARM_L_MOTOR;
    CAN_ID_map[ID_ALARM_R_MOTOR]        = (CAN_ID_t)INTERNAL_ID_ALARM_R_MOTOR;

    // define um identificador unico a cada conjunto id & pos. Mapeia esse identificador
    // em outro vetor, para facil acesso posterior no datalog.
    for (uint16_t i = 0; i < CAN_ID_QUAN; i++) {
        CAN_ID_t* current_ID = CAN_ID_map + i;
        uint16_t unique_index;
        // ids fora de range, de debug
        if (current_ID->id > ECU_CAN_LAST_POPULATED_ID) {
            unique_index = convert_id_pos_to_index(
                ECU_CAN_LAST_POPULATED_ID + 1 + current_ID->id - ECU_CAN_FIRST_DEBUG_ID,
                current_ID->pos);
        } else {
            unique_index = convert_id_pos_to_index(current_ID->id, current_ID->pos);
        }
        CAN_ID_POS_INTERNAL_ID_MAP[unique_index] = i;
    }
}
