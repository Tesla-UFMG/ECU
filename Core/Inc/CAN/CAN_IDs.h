/*
 * CAN_IDs.h
 *
 *  Created on: Dec 8, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CAN_CAN_IDS_H_
#define INC_CAN_CAN_IDS_H_

#include "stdint.h"

//----------
// DATALOGGER
// LEGENDA:  LABEL               INTERNAL_ID
// 0 E RESERVADO PARA LACUNAS NO PACOTE

// Variable name, Message Id, Message word
#define CAN_LIST                                                                         \
    CAN_LIST_DATA(ID_ECU_TIMER, 101, 0)                                                  \
    CAN_LIST_DATA(ID_STEERING_WHEEL, 101, 1)                                             \
    CAN_LIST_DATA(ID_THROTTLE, 101, 2)                                                   \
    CAN_LIST_DATA(ID_BRAKE, 101, 3)                                                      \
    CAN_LIST_DATA(ID_MODE, 102, 0)                                                       \
    CAN_LIST_DATA(ID_REGEN_BRAKE_STATE, 102, 1)                                          \
    CAN_LIST_DATA(ID_DISTANCE_T_ODOM, 102, 2)                                            \
    CAN_LIST_DATA(ID_DISTANCE_P_ODOM, 102, 3)                                            \
    CAN_LIST_DATA(ID_TORQUE_R_MOTOR, 103, 0)                                             \
    CAN_LIST_DATA(ID_TORQUE_L_MOTOR, 103, 1)                                             \
    CAN_LIST_DATA(ID_REF_TORQUE_R_MOTOR, 103, 2)                                         \
    CAN_LIST_DATA(ID_REF_TORQUE_L_MOTOR, 103, 3)                                         \
    CAN_LIST_DATA(ID_REF_SPEED_R, 104, 0)                                                \
    CAN_LIST_DATA(ID_REF_SPEED_L, 104, 1)                                                \
    CAN_LIST_DATA(ID_CONTROL_EVENT_FLAG_1, 104, 2)                                       \
    CAN_LIST_DATA(ID_CONTROL_EVENT_FLAG_2, 104, 3)                                       \
    CAN_LIST_DATA(ID_SPEED_FL, 105, 0)                                                   \
    CAN_LIST_DATA(ID_SPEED_FR, 105, 1)                                                   \
    CAN_LIST_DATA(ID_SPEED_RR, 105, 2)                                                   \
    CAN_LIST_DATA(ID_SPEED_RL, 105, 3)                                                   \
    CAN_LIST_DATA(ID_TORQUE_GAIN, 106, 0)                                                \
    CAN_LIST_DATA(ID_CURRENT_R_MOTOR, 106, 2)                                            \
    CAN_LIST_DATA(ID_CURRENT_L_MOTOR, 106, 3)                                            \
    CAN_LIST_DATA(ID_TEMPERATURE1_R, 107, 0)                                             \
    CAN_LIST_DATA(ID_TEMPERATURE2_R, 107, 1)                                             \
    CAN_LIST_DATA(ID_TEMPERATURE1_L, 107, 2)                                             \
    CAN_LIST_DATA(ID_TEMPERATURE2_L, 107, 3)                                             \
    CAN_LIST_DATA(ID_STATUS_DATALOG, 108, 1)                                             \
    CAN_LIST_DATA(ID_SPEED_L_MOTOR, 109, 0)                                              \
    CAN_LIST_DATA(ID_SPEED_R_MOTOR, 109, 1)                                              \
    CAN_LIST_DATA(ID_POWER_L_MOTOR, 109, 2)                                              \
    CAN_LIST_DATA(ID_POWER_R_MOTOR, 109, 3)                                              \
    CAN_LIST_DATA(ID_ENERGY_L_MOTOR, 110, 0)                                             \
    CAN_LIST_DATA(ID_ENERGY_R_MOTOR, 110, 1)                                             \
    CAN_LIST_DATA(ID_OVERLOAD_L_MOTOR, 110, 2)                                           \
    CAN_LIST_DATA(ID_OVERLOAD_R_MOTOR, 110, 3)                                           \
    CAN_LIST_DATA(ID_LOST_MSG_L_MOTOR, 111, 0)                                           \
    CAN_LIST_DATA(ID_LOST_MSG_R_MOTOR, 111, 1)                                           \
    CAN_LIST_DATA(ID_BUS_OFF_L_MOTOR, 111, 2)                                            \
    CAN_LIST_DATA(ID_BUS_OFF_R_MOTOR, 111, 3)                                            \
    CAN_LIST_DATA(ID_CAN_STATE_L_MOTOR, 112, 0)                                          \
    CAN_LIST_DATA(ID_CAN_STATE_R_MOTOR, 112, 1)                                          \
    CAN_LIST_DATA(ID_INV_STATE_L_MOTOR, 112, 2)                                          \
    CAN_LIST_DATA(ID_INV_STATE_R_MOTOR, 112, 3)                                          \
    CAN_LIST_DATA(ID_FAILURE_L_MOTOR, 113, 0)                                            \
    CAN_LIST_DATA(ID_FAILURE_R_MOTOR, 113, 1)                                            \
    CAN_LIST_DATA(ID_ALARM_L_MOTOR, 113, 2)                                              \
    CAN_LIST_DATA(ID_ALARM_R_MOTOR, 113, 3)                                              \
    CAN_LIST_DATA(ID_CURRENT_FLAG, 108, 3)                                               \
    CAN_LIST_DATA(ID_PANEL_DEBUG_1, 351, 0)                                              \
    CAN_LIST_DATA(ID_PANEL_DEBUG_2, 351, 1)                                              \
    CAN_LIST_DATA(ID_PANEL_DEBUG_3, 351, 2)                                              \
    CAN_LIST_DATA(ID_PANEL_DEBUG_4, 351, 3)

typedef enum {
#define CAN_LIST_DATA(var_name, msg_id, msg_wrd) var_name,
    CAN_LIST
#undef CAN_LIST_DATA
        CAN_ID_QUAN
} CAN_LIST_e;


typedef struct {
    CAN_LIST_e var;
    uint16_t id;
    uint8_t pos;
} CAN_ID_t;

typedef struct {
    uint16_t extern_ID;
    int16_t pos[4];
} datalog_send_t;

#define QUANT_RESERVED_ID 50

// INVERTER
#define ID_RIGHT_INVERTER 0x301
#define ID_LEFT_INVERTER  0x300
#define ID_COMM_FLAG      0x302 // flag de comunicacao ecu-inv ok

void sort_struct();
int get_quant_id();
void initialize_CAN_IDs_struct();
void initialize_map_CAN_IDs();

#endif /* INC_CAN_CAN_IDS_H_ */
