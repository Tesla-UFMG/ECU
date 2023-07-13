/*
 * CAN_IDs.h
 *
 *  Created on: Dec 8, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CAN_CAN_IDS_H_
#define INC_CAN_CAN_IDS_H_

#include "stdint.h"
#include "stdlib.h"

//----------
// DATALOGGER
// 0 E RESERVADO PARA LACUNAS NO PACOTE
// Variable name, Message Id, Message word
#define VARIABLES_GENERAL_CAN_TX                                                         \
    CAN_GENERAL_LIST_DATA(ID_SPEED_AVG, 76, 0)                                          \
    CAN_GENERAL_LIST_DATA(ID_STEERING_WHEEL, 76, 1)                                     \
    CAN_GENERAL_LIST_DATA(ID_THROTTLE, 76, 2)                                           \
    CAN_GENERAL_LIST_DATA(ID_BRAKE, 76, 3)                                              \
    CAN_GENERAL_LIST_DATA(ID_MODE, 77, 0)                                               \
    CAN_GENERAL_LIST_DATA(ID_TORQUE_GAIN, 77, 1)                                        \
    CAN_GENERAL_LIST_DATA(ID_DISTANCE_P_ODOM, 77, 2)                                    \
    CAN_GENERAL_LIST_DATA(ID_DISTANCE_T_ODOM, 77, 3)                                    \
    CAN_GENERAL_LIST_DATA(ID_CONTROL_EVENT_FLAG_1, 78, 0)                               \
    CAN_GENERAL_LIST_DATA(ID_CONTROL_EVENT_FLAG_2, 78, 1)                               \
    CAN_GENERAL_LIST_DATA(ID_REF_TORQUE_R_MOTOR, 78, 2)                                 \
    CAN_GENERAL_LIST_DATA(ID_REF_TORQUE_L_MOTOR, 78, 3)                                 \
    CAN_GENERAL_LIST_DATA(ID_SPEED_FL, 79, 0)                                           \
    CAN_GENERAL_LIST_DATA(ID_SPEED_FR, 79, 1)                                           \
    CAN_GENERAL_LIST_DATA(ID_SPEED_RL, 79, 2)                                           \
    CAN_GENERAL_LIST_DATA(ID_SPEED_RR, 79, 3)                                           \
    CAN_GENERAL_LIST_DATA(ID_REGEN_BRAKE_STATE, 81, 0)                                  \
    CAN_GENERAL_LIST_DATA(ID_SPEED_L_MOTOR, 85, 0)                                      \
    CAN_GENERAL_LIST_DATA(ID_TORQUE_L_MOTOR, 85, 1)                                     \
    CAN_GENERAL_LIST_DATA(ID_POWER_L_MOTOR, 85, 2)                                      \
    CAN_GENERAL_LIST_DATA(ID_CURRENT_L_MOTOR, 85, 3)                                    \
    CAN_GENERAL_LIST_DATA(ID_ENERGY_L_MOTOR, 86, 0)                                     \
    CAN_GENERAL_LIST_DATA(ID_OVERLOAD_L_MOTOR, 86, 1)                                   \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE1_L, 86, 2)                                     \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE2_L, 86, 3)                                     \
    CAN_GENERAL_LIST_DATA(ID_LOST_MSG_L_MOTOR, 87, 0)                                   \
    CAN_GENERAL_LIST_DATA(ID_BUS_OFF_L_MOTOR, 87, 1)                                    \
    CAN_GENERAL_LIST_DATA(ID_CAN_STATE_L_MOTOR, 87, 2)                                  \
    CAN_GENERAL_LIST_DATA(ID_INV_STATE_L_MOTOR, 88, 0)                                  \
    CAN_GENERAL_LIST_DATA(ID_FAILURE_L_MOTOR, 88, 1)                                    \
    CAN_GENERAL_LIST_DATA(ID_ALARM_L_MOTOR, 88, 2)                                      \
    CAN_GENERAL_LIST_DATA(ID_SPEED_R_MOTOR, 95, 0)                                      \
    CAN_GENERAL_LIST_DATA(ID_TORQUE_R_MOTOR, 95, 1)                                     \
    CAN_GENERAL_LIST_DATA(ID_POWER_R_MOTOR, 95, 2)                                      \
    CAN_GENERAL_LIST_DATA(ID_CURRENT_R_MOTOR, 95, 3)                                    \
    CAN_GENERAL_LIST_DATA(ID_ENERGY_R_MOTOR, 96, 0)                                     \
    CAN_GENERAL_LIST_DATA(ID_OVERLOAD_R_MOTOR, 96, 1)                                   \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE1_R, 96, 2)                                     \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE2_R, 96, 3)                                     \
    CAN_GENERAL_LIST_DATA(ID_LOST_MSG_R_MOTOR, 97, 0)                                   \
    CAN_GENERAL_LIST_DATA(ID_BUS_OFF_R_MOTOR, 97, 1)                                    \
    CAN_GENERAL_LIST_DATA(ID_CAN_STATE_R_MOTOR, 97, 2)                                  \
    CAN_GENERAL_LIST_DATA(ID_INV_STATE_R_MOTOR, 98, 0)                                  \
    CAN_GENERAL_LIST_DATA(ID_FAILURE_R_MOTOR, 98, 1)                                    \
    CAN_GENERAL_LIST_DATA(ID_ALARM_R_MOTOR, 98, 2)                                      \
    CAN_GENERAL_LIST_DATA(ID_PANEL_DEBUG_1, 351, 0)                                      \
    CAN_GENERAL_LIST_DATA(ID_PANEL_DEBUG_2, 351, 1)                                      \
    CAN_GENERAL_LIST_DATA(ID_PANEL_DEBUG_3, 351, 2)                                      \
    CAN_GENERAL_LIST_DATA(ID_PANEL_DEBUG_4, 351, 3)

typedef enum {
    RESERVED_GENERAL_ID_0,
#define CAN_GENERAL_LIST_DATA(var_name, msg_id, msg_wrd) var_name,
    VARIABLES_GENERAL_CAN_TX
#undef CAN_GENERAL_LIST_DATA
        CAN_GENERAL_ID_QUAN
} CAN_general_list_e;

typedef struct {
    uint16_t id;
    uint8_t message_to_send;
} CAN_var_inf;

typedef struct {
    uint16_t external_ID;
    CAN_general_list_e message_to_send[4];
} datalog_send_t;

#define QUANT_RESERVED_ID 50
#define WORDS_PER_ID      4

// INVERTER
#define ID_RIGHT_INVERTER 0x301
#define ID_LEFT_INVERTER  0x300
#define ID_COMM_FLAG      0x302 // flag de comunicacao ecu-inv ok

uint16_t get_amount_ext_id(void);
void initialize_CAN_IDs_struct(void);
int16_t get_internal_id_from_pos_and_word(uint16_t pos_struct, uint16_t pos_word);
uint16_t get_external_id_from_struct_pos(uint16_t struct_pos);

#endif /* INC_CAN_CAN_IDS_H_ */
