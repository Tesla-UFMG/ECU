/*
 * CAN_IDs.h
 *
 *  Created on: Dec 8, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CAN_CAN_IDS_H_
#define INC_CAN_CAN_IDS_H_

#include "stdint.h"

// INVERTER

#define ID_RIGHT_INVERTER 0x301
#define ID_LEFT_INVERTER  0x300
#define ID_COMM_FLAG      0x302 // flag de comunicacao ecu-inv ok

//----------
// DATALOGGER
// LEGENDA:  LABEL    	    	 INTERNAL_ID
// 0 E RESERVADO PARA LACUNAS NO PACOTE
#define ID_ECU_TIMER            1
#define ID_STEERING_WHEEL       2
#define ID_THROTTLE             3
#define ID_BRAKE                4
#define ID_MODE                 5
#define ID_DISTANCE_ODOM        6
#define ID_TORQUE_R_MOTOR       7
#define ID_TORQUE_L_MOTOR       8
#define ID_REF_TORQUE_R_MOTOR   9
#define ID_REF_TORQUE_L_MOTOR   10
#define ID_REF_SPEED_R          11
#define ID_REF_SPEED_L          12
#define ID_CONTROL_EVENT_FLAG_1 13
#define ID_CONTROL_EVENT_FLAG_2 14
#define ID_SPEED_FL             15
#define ID_SPEED_FR             16
#define ID_SPEED_RR             17
#define ID_SPEED_RL             18
#define ID_TORQUE_GAIN          19
#define ID_CURRENT_R_MOTOR      20
#define ID_CURRENT_L_MOTOR      21
#define ID_TEMPERATURE1_R       22
#define ID_TEMPERATURE2_R       23
#define ID_TEMPERATURE1_L       24
#define ID_TEMPERATURE2_L       25
#define ID_STATUS_DATALOG       26
#define ID_CURRENT_FLAG         27
#define ID_PANEL_DEBUG_1        28
#define ID_PANEL_DEBUG_2        29
#define ID_PANEL_DEBUG_3        30
#define ID_PANEL_DEBUG_4        31
#define ID_REGEN_BRAKE_STATE    32
#define ID_SPEED_L_MOTOR        33
#define ID_SPEED_R_MOTOR        34
#define ID_POWER_L_MOTOR        35
#define ID_POWER_R_MOTOR        36
#define ID_ENERGY_L_MOTOR       37
#define ID_ENERGY_R_MOTOR       38
#define ID_OVERLOAD_L_MOTOR     39
#define ID_OVERLOAD_R_MOTOR     40
#define ID_LOST_MSG_L_MOTOR     41
#define ID_LOST_MSG_R_MOTOR     42
#define ID_BUS_OFF_L_MOTOR      43
#define ID_BUS_OFF_R_MOTOR      44
#define ID_CAN_STATE_L_MOTOR    45
#define ID_CAN_STATE_R_MOTOR    46
#define ID_INV_STATE_L_MOTOR    47
#define ID_INV_STATE_R_MOTOR    48
#define ID_FAILURE_L_MOTOR      49
#define ID_FAILURE_R_MOTOR      50
#define ID_ALARM_L_MOTOR        51
#define ID_ALARM_R_MOTOR        52

#define CAN_ID_QUAN 53

typedef struct {
    uint16_t id;
    uint8_t pos;
} CAN_ID_t;

extern CAN_ID_t CAN_ID_map[CAN_ID_QUAN];
// TODO(renanmoreira): IMPLEMENT X-MACROS FOR CAN_ID'S
#define ECU_CAN_INITIAL_ID        100
#define ECU_CAN_FINAL_ID          149
#define ECU_CAN_LAST_POPULATED_ID 113
#define ECU_CAN_FIRST_DEBUG_ID    351
#define ECU_CAN_LAST_DEBUG_ID     351

#define INTERNAL_ID_ECU_TIMER                                                            \
    { 101, 0 }
#define INTERNAL_ID_STEERING_WHEEL                                                       \
    { 101, 1 }
#define INTERNAL_ID_THROTTLE                                                             \
    { 101, 2 }
#define INTERNAL_ID_BRAKE                                                                \
    { 101, 3 }
#define INTERNAL_ID_MODE                                                                 \
    { 102, 0 }
#define INTERNAL_ID_DISTANCE_ODOM                                                        \
    { 102, 2 }
#define INTERNAL_ID_TORQUE_R_MOTOR                                                       \
    { 103, 0 }
#define INTERNAL_ID_TORQUE_L_MOTOR                                                       \
    { 103, 1 }
#define INTERNAL_ID_REF_TORQUE_R_MOTOR                                                   \
    { 103, 2 }
#define INTERNAL_ID_REF_TORQUE_L_MOTOR                                                   \
    { 103, 3 }
#define INTERNAL_ID_REF_SPEED_R_MOTOR                                                    \
    { 104, 0 }
#define INTERNAL_ID_REF_SPEED_L_MOTOR                                                    \
    { 104, 1 }
#define INTERNAL_ID_CONTROL_EVENT_FLAG_1                                                 \
    { 104, 2 }
#define INTERNAL_ID_CONTROL_EVENT_FLAG_2                                                 \
    { 104, 3 }
#define INTERNAL_ID_SPEED_FL                                                             \
    { 105, 0 }
#define INTERNAL_ID_SPEED_FR                                                             \
    { 105, 1 }
#define INTERNAL_ID_SPEED_RR                                                             \
    { 105, 2 }
#define INTERNAL_ID_SPEED_RL                                                             \
    { 105, 3 }
#define INTERNAL_ID_TORQUE_GAIN                                                          \
    { 106, 0 }
#define INTERNAL_ID_CURRENT_R_MOTOR                                                      \
    { 106, 2 }
#define INTERNAL_ID_CURRENT_L_MOTOR                                                      \
    { 106, 3 }
#define INTERNAL_ID_TEMPERATURE1_R                                                       \
    { 107, 0 }
#define INTERNAL_ID_TEMPERATURE2_R                                                       \
    { 107, 1 }
#define INTERNAL_ID_TEMPERATURE1_L                                                       \
    { 107, 2 }
#define INTERNAL_ID_TEMPERATURE2_L                                                       \
    { 107, 3 }
#define INTERNAL_ID_STATUS_DATALOG                                                       \
    { 108, 1 }
#define INTERNAL_ID_CURRENT_FLAG                                                         \
    { 108, 3 }
#define INTERNAL_ID_PANEL_DEBUG_1                                                        \
    { 351, 0 }
#define INTERNAL_ID_PANEL_DEBUG_2                                                        \
    { 351, 1 }
#define INTERNAL_ID_PANEL_DEBUG_3                                                        \
    { 351, 2 }
#define INTERNAL_ID_PANEL_DEBUG_4                                                        \
    { 351, 3 }
#define INTERNAL_ID_REGEN_BRAKE_STATE                                                    \
    { 102, 1 }
#define INTERNAL_ID_SPEED_L_MOTOR                                                        \
    { 109, 0 }
#define INTERNAL_ID_SPEED_R_MOTOR                                                        \
    { 109, 1 }
#define INTERNAL_ID_POWER_L_MOTOR                                                        \
    { 109, 2 }
#define INTERNAL_ID_POWER_R_MOTOR                                                        \
    { 109, 3 }
#define INTERNAL_ID_ENERGY_L_MOTOR                                                       \
    { 110, 0 }
#define INTERNAL_ID_ENERGY_R_MOTOR                                                       \
    { 110, 1 }
#define INTERNAL_ID_OVERLOAD_L_MOTOR                                                     \
    { 110, 2 }
#define INTERNAL_ID_OVERLOAD_R_MOTOR                                                     \
    { 110, 3 }
#define INTERNAL_ID_LOST_MSG_L_MOTOR                                                     \
    { 111, 0 }
#define INTERNAL_ID_LOST_MSG_R_MOTOR                                                     \
    { 111, 1 }
#define INTERNAL_ID_BUS_OFF_L_MOTOR                                                      \
    { 111, 2 }
#define INTERNAL_ID_BUS_OFF_R_MOTOR                                                      \
    { 111, 3 }
#define INTERNAL_ID_CAN_STATE_L_MOTOR                                                    \
    { 112, 0 }
#define INTERNAL_ID_CAN_STATE_R_MOTOR                                                    \
    { 112, 1 }
#define INTERNAL_ID_INV_STATE_L_MOTOR                                                    \
    { 112, 2 }
#define INTERNAL_ID_INV_STATE_R_MOTOR                                                    \
    { 112, 3 }
#define INTERNAL_ID_FAILURE_L_MOTOR                                                      \
    { 113, 0 }
#define INTERNAL_ID_FAILURE_R_MOTOR                                                      \
    { 113, 1 }
#define INTERNAL_ID_ALARM_L_MOTOR                                                        \
    { 113, 2 }
#define INTERNAL_ID_ALARM_R_MOTOR                                                        \
    { 113, 3 }

//----------
#define VARIABLES_GENERAL                                                                       \
    ENTRY(AccelX, 130, 0)                                                                \
    ENTRY(AccelY, 130, 1)                                                                \
    ENTRY(AccelZ, 130, 2)                                                                \
    ENTRY(ErrorIMUAccel, 130, 3)                                                         \
    ENTRY(GyroX, 131, 0)                                                                 \
    ENTRY(Gyroy, 131, 1)                                                                 \
    ENTRY(GyroZ, 131, 2)                                                                 \
    ENTRY(ERRORIMUGyro, 131, 3)                                                          \

typedef enum {
#define ENTRY(a,b,c) a,
    VARIABLES_GENERAL
#undef ENTRY
        NUM_STATES_GENERAL
} can_vars_e_general;

void initialize_CAN_IDs();
uint16_t convert_id_pos_to_index(uint16_t id, uint16_t pos);
CAN_ID_t get_CAN_ID_from_internal(uint16_t internal);
uint16_t get_internal_from_id_pos(uint16_t id, uint16_t pos);

void store_value_general(can_vars_e_general var_name, uint16_t value_can_general);
uint16_t get_value_general(can_vars_e_general var_name);
can_vars_e_general get_var_name_from_id_and_pos_general(uint32_t id, int pos);


#endif /* INC_CAN_CAN_IDS_H_ */
