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
#define ID_SPEED_AVG            1
#define ID_STEERING_WHEEL       2
#define ID_THROTTLE             3
#define ID_BRAKE                4
#define ID_MODE                 5
#define ID_TORQUE_GAIN          6
#define ID_DISTANCE_P_ODOM      7
#define ID_DISTANCE_T_ODOM      8
#define ID_CONTROL_EVENT_FLAG_1 9
#define ID_CONTROL_EVENT_FLAG_2 10
#define ID_REF_TORQUE_R_MOTOR   11
#define ID_REF_TORQUE_L_MOTOR   12
#define ID_SPEED_FL             13
#define ID_SPEED_FR             14
#define ID_SPEED_RL             15
#define ID_SPEED_RR             16
// #define /*RED_TORQUE_ROD_ESQ */ 17
// #define /*RED_TORQUE_ROD_DIR */ 18
// #define /*EXTRA_CONTROL_DIN  */ 19
// #define /*EXTRA_CONTROL_DIN  */ 20
#define ID_REGEN_BRAKE_STATE    21
#define ID_IMU_BSE_ERROR        22
#define ID_STATUS_DATALOG       23
#define ID_IMU_SPEED_ERROR      24
#define ID_CURRENT_FLAG         25
#define ID_PANEL_DEBUG_1        26
#define ID_PANEL_DEBUG_2        27
#define ID_PANEL_DEBUG_3        28
#define ID_PANEL_DEBUG_4        29
#define ID_SPEED_L_MOTOR        30
#define ID_TORQUE_L_MOTOR       31
#define ID_POWER_L_MOTOR        32
#define ID_CURRENT_L_MOTOR      33
#define ID_ENERGY_L_MOTOR       34
#define ID_OVERLOAD_L_MOTOR     35
#define ID_TEMPERATURE1_L       36
#define ID_TEMPERATURE2_L       37
#define ID_LOST_MSG_L_MOTOR     38
#define ID_BUS_OFF_L_MOTOR      39
#define ID_CAN_STATE_L_MOTOR    40
#define ID_INV_STATE_L_MOTOR    41
#define ID_FAILURE_L_MOTOR      42
#define ID_ALARM_L_MOTOR        43
#define ID_SPEED_R_MOTOR        44
#define ID_TORQUE_R_MOTOR       45
#define ID_POWER_R_MOTOR        46
#define ID_CURRENT_R_MOTOR      47
#define ID_ENERGY_R_MOTOR       48
#define ID_OVERLOAD_R_MOTOR     49
#define ID_TEMPERATURE1_R       50
#define ID_TEMPERATURE2_R       51
#define ID_LOST_MSG_R_MOTOR     52
#define ID_BUS_OFF_R_MOTOR      53
#define ID_CAN_STATE_R_MOTOR    54
#define ID_INV_STATE_R_MOTOR    55
#define ID_FAILURE_R_MOTOR      56
#define ID_ALARM_R_MOTOR        57
// #define /*ACCEL X*/ 58
// #define /*ACCEL Y*/ 59
// #define /*ACCEL Z*/ 60
// #define /*TEMP IMU*/ 61
// #define /*GYRO X*/ 62
// #define /*GYRO Y*/ 63
// #define /*GYRO Z*/ 64
// #define /*IMU ERROR*/ 65

#define CAN_ID_QUAN 65

typedef struct {
    uint16_t id;
    uint8_t pos;
} CAN_ID_t;

extern CAN_ID_t CAN_ID_map[CAN_ID_QUAN];
// TODO(renanmoreira): IMPLEMENT X-MACROS FOR CAN_ID'S
#define ECU_CAN_INITIAL_ID        100
#define ECU_CAN_FINAL_ID          149
#define ECU_CAN_LAST_POPULATED_ID 131
#define ECU_CAN_FIRST_DEBUG_ID    351
#define ECU_CAN_LAST_DEBUG_ID     351

#define INTERNAL_ID_SPEED_AVG                                                            \
    { 101, 0 }
#define INTERNAL_ID_STEERING_WHEEL                                                       \
    { 101, 1 }
#define INTERNAL_ID_THROTTLE                                                             \
    { 101, 2 }
#define INTERNAL_ID_BRAKE                                                                \
    { 101, 3 }
#define INTERNAL_ID_MODE                                                                 \
    { 102, 0 }
#define INTERNAL_ID_TORQUE_GAIN                                                          \
    { 102, 1 }
#define INTERNAL_ID_DISTANCE_P_ODOM                                                      \
    { 102, 2 }
#define INTERNAL_ID_DISTANCE_T_ODOM                                                      \
    { 102, 3 }
#define INTERNAL_ID_CONTROL_EVENT_FLAG_1                                                 \
    { 103, 0 }
#define INTERNAL_ID_CONTROL_EVENT_FLAG_2                                                 \
    { 103, 1 }
#define INTERNAL_ID_REF_TORQUE_R_MOTOR                                                   \
    { 103, 2 }
#define INTERNAL_ID_REF_TORQUE_L_MOTOR                                                   \
    { 103, 3 }
#define INTERNAL_ID_SPEED_FL                                                             \
    { 104, 0 }
#define INTERNAL_ID_SPEED_FR                                                             \
    { 104, 1 }
#define INTERNAL_ID_SPEED_RL                                                             \
    { 104, 2 }
#define INTERNAL_ID_SPEED_RR                                                             \
    { 104, 3 }
// #define REDUCAO TORQUE RODA ESQUERDA                                                   \
//     { 105, 0 }
// #define  || || || DIREITA                                                              \
//     { 105, 1 }
// #define  EXTRA CONTROLE DINAMICO   flags crossval?                                     \
//     { 105, 2 }
// #define  EXTRA CONTROLE DINAMICO                                                       \
//     { 105, 3 }
#define INTERNAL_ID_REGEN_BRAKE_STATE                                                    \
    { 106, 0 }
#define INTERNAL_ID_IMU_BSE_ERROR                                                        \
    { 108, 0 }
#define INTERNAL_ID_STATUS_DATALOG                                                       \
    { 108, 1 }
#define INTERNAL_ID_IMU_SPEED_ERROR                                                      \
    { 108, 2 }
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
#define INTERNAL_ID_SPEED_L_MOTOR                                                        \
    { 110, 0 }
#define INTERNAL_ID_TORQUE_L_MOTOR                                                       \
    { 110, 1 }
#define INTERNAL_ID_POWER_L_MOTOR                                                        \
    { 110, 2 }
#define INTERNAL_ID_CURRENT_L_MOTOR                                                      \
    { 110, 3 }
#define INTERNAL_ID_ENERGY_L_MOTOR                                                       \
    { 111, 0 }
#define INTERNAL_ID_OVERLOAD_L_MOTOR                                                     \
    { 111, 1 }
#define INTERNAL_ID_TEMPERATURE1_L                                                       \
    { 111, 2 }
#define INTERNAL_ID_TEMPERATURE2_L                                                       \
    { 111, 3 }
#define INTERNAL_ID_LOST_MSG_L_MOTOR                                                     \
    { 112, 0 }
#define INTERNAL_ID_BUS_OFF_L_MOTOR                                                      \
    { 112, 1 }
#define INTERNAL_ID_CAN_STATE_L_MOTOR                                                    \
    { 112, 2 }
#define INTERNAL_ID_INV_STATE_L_MOTOR                                                    \
    { 113, 0 }
#define INTERNAL_ID_FAILURE_L_MOTOR                                                      \
    { 113, 1 }
#define INTERNAL_ID_ALARM_L_MOTOR                                                        \
    { 113, 2 }
#define INTERNAL_ID_SPEED_R_MOTOR                                                        \
    { 120, 0 }
#define INTERNAL_ID_TORQUE_R_MOTOR                                                       \
    { 120, 1 }
#define INTERNAL_ID_POWER_R_MOTOR                                                        \
    { 120, 2 }
#define INTERNAL_ID_CURRENT_R_MOTOR                                                      \
    { 120, 3 }
#define INTERNAL_ID_ENERGY_R_MOTOR                                                       \
    { 121, 0 }
#define INTERNAL_ID_OVERLOAD_R_MOTOR                                                     \
    { 121, 1 }
#define INTERNAL_ID_TEMPERATURE1_R                                                       \
    { 121, 2 }
#define INTERNAL_ID_TEMPERATURE2_R                                                       \
    { 121, 3 }
#define INTERNAL_ID_LOST_MSG_R_MOTOR                                                     \
    { 122, 0 }
#define INTERNAL_ID_BUS_OFF_R_MOTOR                                                      \
    { 122, 1 }
#define INTERNAL_ID_CAN_STATE_R_MOTOR                                                    \
    { 122, 2 }
#define INTERNAL_ID_INV_STATE_R_MOTOR                                                    \
    { 123, 0 }
#define INTERNAL_ID_FAILURE_R_MOTOR                                                      \
    { 123, 1 }
#define INTERNAL_ID_ALARM_R_MOTOR                                                        \
    { 123, 2 }
// #define ACCEL X                                                                        \
//     { 130, 0 }
// #define ACCEL Y                                                                        \
//     { 130, 1 }
// #define ACCEL Z                                                                        \
//     { 130, 2 }
// #define TEMP IMU                                                                       \
//     { 130, 3 }
// #define GYRO X                                                                         \
//     { 131, 0 }
// #define GYRO Y                                                                         \
//     { 131, 1 }
// #define GYRO Z                                                                         \
//     { 131, 2 }
// #define IMU ERROR                                                                      \
//     { 131, 3 }

//----------

void initialize_CAN_IDs();
uint16_t convert_id_pos_to_index(uint16_t id, uint16_t pos);
CAN_ID_t get_CAN_ID_from_internal(uint16_t internal);
uint16_t get_internal_from_id_pos(uint16_t id, uint16_t pos);

#endif /* INC_CAN_CAN_IDS_H_ */
