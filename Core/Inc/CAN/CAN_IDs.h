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
#include "CAN/CAN_IDs_identifiers.h"

//----------
// DATALOGGER
// 0 IS RESERVED FOR GAPS IN THE PACKAGE
// Variable name, Message Id, Message word
#define VARIABLES_GENERAL_CAN_TX                                                       \
    CAN_GENERAL_LIST_DATA(ID_SPEED_AVG, CanID_ECU_VELOCIDADE_MED, 0)                   \
    CAN_GENERAL_LIST_DATA(ID_STEERING_WHEEL, CanID_ECU_VOLANTE_ANG, 0)                 \
    CAN_GENERAL_LIST_DATA(ID_THROTTLE, CanID_ECU_ACELERADOR, 0)                        \
    CAN_GENERAL_LIST_DATA(ID_BRAKE, CanID_ECU_FREIO_STATUS, 0)                         \
    CAN_GENERAL_LIST_DATA(ID_MODE, CanID_ECU_MODO, 0)                                  \
    CAN_GENERAL_LIST_DATA(ID_TORQUE_GAIN, CanID_ECU_GANHO_TORQUE, 0)                   \
    CAN_GENERAL_LIST_DATA(ID_DISTANCE_P_ODOM, CanID_ECU_HODOM_PARCIAL, 0)              \
    CAN_GENERAL_LIST_DATA(ID_DISTANCE_T_ODOM, CanID_ECU_HODOM_TOTAL, 0)                \
    CAN_GENERAL_LIST_DATA(ID_CONTROL_EVENT_FLAG_1, CanID_ECU_FLAGS_ERRO, 0)            \
    CAN_GENERAL_LIST_DATA(ID_CONTROL_EVENT_FLAG_2, CanID_ECU_FLAGS_STATUS, 0)          \
    CAN_GENERAL_LIST_DATA(ID_REF_TORQUE_R_MOTOR, CanID_ECU_TORQUE_MD, 0)               \
    CAN_GENERAL_LIST_DATA(ID_REF_TORQUE_L_MOTOR, CanID_ECU_TORQUE_ME, 0)               \
    CAN_GENERAL_LIST_DATA(ID_SPEED_FL, CanID_ECU_VEL_RODA_DIANT_E, 0)                  \
    CAN_GENERAL_LIST_DATA(ID_SPEED_FR, CanID_ECU_VEL_RODA_DIANT_D, 0)                  \
    CAN_GENERAL_LIST_DATA(ID_SPEED_RL, CanID_ECU_VEL_RODA_TRASE_E, 0)                  \
    CAN_GENERAL_LIST_DATA(ID_SPEED_RR, CanID_ECU_VEL_RODA_TRASE_D, 0)                  \
    CAN_GENERAL_LIST_DATA(ID_REGEN_BRAKE_STATE, CanID_ECU_FRENAGEM_STATUS, 0)          \
    CAN_GENERAL_LIST_DATA(ID_SPEED_L_MOTOR, CanID_ECU_VELOCIDADE_ME, 0)                \
    CAN_GENERAL_LIST_DATA(ID_TORQUE_L_MOTOR, CanID_ECU_TORQUE_ME, 0)                   \
    CAN_GENERAL_LIST_DATA(ID_POWER_L_MOTOR, CanID_ECU_POTENCIA_ME, 0)                  \
    CAN_GENERAL_LIST_DATA(ID_CURRENT_L_MOTOR, CanID_ECU_CORR_TORQUE_ME, 0)             \
    CAN_GENERAL_LIST_DATA(ID_ENERGY_L_MOTOR, CanID_ECU_ENERGIA_CONS_ME, 0)             \
    CAN_GENERAL_LIST_DATA(ID_OVERLOAD_L_MOTOR, CanID_ECU_SOBRECARGA_ME, 0)             \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE1_L, CanID_ECU_TEMP_1_MOS_ME, 0)               \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE2_L, CanID_ECU_TEMP_2_MOS_ME, 0)               \
    CAN_GENERAL_LIST_DATA(ID_LOST_MSG_L_MOTOR, CanID_ECU_MSG_PERDIDA_ME, 0)            \
    CAN_GENERAL_LIST_DATA(ID_BUS_OFF_L_MOTOR, CanID_ECU_CONT_BUSOFF_ME, 0)             \
    CAN_GENERAL_LIST_DATA(ID_CAN_STATE_L_MOTOR, CanID_ECU_STATE_CAN_ME, 0)             \
    CAN_GENERAL_LIST_DATA(ID_INV_STATE_L_MOTOR, CanID_ECU_STATE_INV_ME, 0)             \
    CAN_GENERAL_LIST_DATA(ID_FAILURE_L_MOTOR, CanID_ECU_FALHA_ATUAL_ME, 0)             \
    CAN_GENERAL_LIST_DATA(ID_ALARM_L_MOTOR, CanID_ECU_ALARM_ATUAL_ME, 0)               \
    CAN_GENERAL_LIST_DATA(ID_SPEED_R_MOTOR, CanID_ECU_VELOCIDADE_MD, 0)                \
    CAN_GENERAL_LIST_DATA(ID_TORQUE_R_MOTOR, CanID_ECU_TORQUE_MD, 0)                   \
    CAN_GENERAL_LIST_DATA(ID_POWER_R_MOTOR, CanID_ECU_POTENCIA_MD, 0)                  \
    CAN_GENERAL_LIST_DATA(ID_CURRENT_R_MOTOR, CanID_ECU_CORR_TORQUE_MD, 0)             \
    CAN_GENERAL_LIST_DATA(ID_ENERGY_R_MOTOR, CanID_ECU_ENERGIA_CONS_MD, 0)             \
    CAN_GENERAL_LIST_DATA(ID_OVERLOAD_R_MOTOR, CanID_ECU_SOBRECARGA_MD, 0)             \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE1_R, CanID_ECU_TEMP_1_MOS_MD, 0)               \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE2_R, CanID_ECU_TEMP_2_MOS_MD, 0)               \
    CAN_GENERAL_LIST_DATA(ID_LOST_MSG_R_MOTOR, CanID_ECU_MSG_PERDIDA_MD, 0)            \
    CAN_GENERAL_LIST_DATA(ID_BUS_OFF_R_MOTOR, CanID_ECU_CONT_BUSOFF_MD, 0)             \
    CAN_GENERAL_LIST_DATA(ID_CAN_STATE_R_MOTOR, CanID_ECU_STATE_CAN_MD, 0)             \
    CAN_GENERAL_LIST_DATA(ID_INV_STATE_R_MOTOR, CanID_ECU_STATE_INV_MD, 0)             \
    CAN_GENERAL_LIST_DATA(ID_FAILURE_R_MOTOR, CanID_ECU_FALHA_ATUAL_MD, 0)             \
    CAN_GENERAL_LIST_DATA(ID_ALARM_R_MOTOR, CanID_ECU_ALARM_ATUAL_MD, 0)               \
    CAN_GENERAL_LIST_DATA(ID_PANEL_DEBUG_1, CanID_ECU_DEBUG_0, 0)                      \
    CAN_GENERAL_LIST_DATA(ID_PANEL_DEBUG_2, CanID_ECU_DEBUG_1, 0)                      \
    CAN_GENERAL_LIST_DATA(ID_PANEL_DEBUG_3, CanID_ECU_DEBUG_2, 0)                      \

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
int16_t get_internal_id_from_pos_and_word(uint16_t pos_struct,
		uint16_t pos_word);
uint16_t get_external_id_from_struct_pos(uint16_t struct_pos);

#endif /* INC_CAN_CAN_IDS_H_ */