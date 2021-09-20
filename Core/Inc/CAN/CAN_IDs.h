/*
 * CAN_IDs.h
 *
 *  Created on: Dec 8, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CAN_CAN_IDS_H_
#define INC_CAN_CAN_IDS_H_

#include "stdint.h"

//INVERTER

#define	ID_RIGHT_INVERTER 	0x301
#define	ID_LEFT_INVERTER 	0x300
#define	ID_COMM_FLAG		0x302		// flag de comunicacao ecu-inv ok

//----------
//DATALOGGER
//LEGENDA:  LABEL    	    	 INTERNAL_ID
//0 E RESERVADO PARA LACUNAS NO PACOTE
#define ID_ECU_TIMER        	1
#define ID_STEERING_WHEEL		2
#define ID_THROTTLE				3
#define ID_BRAKE				4
#define ID_MODE					5
#define ID_DISTANCE_ODOM		6
#define ID_TORQUE_R_MOTOR		7
#define ID_TORQUE_L_MOTOR		8
#define ID_REF_TORQUE_R_MOTOR	9
#define ID_REF_TORQUE_L_MOTOR	10
#define ID_REF_SPEED_R			11
#define ID_REF_SPEED_L			12
#define ID_REF_TORQUE_NEG		13
#define ID_REGEN_BRAKE_QUAN		14
#define ID_SPEED_FL				15
#define ID_SPEED_FR				16
#define ID_SPEED_RR				17
#define ID_SPEED_RL				18
#define ID_TORQUE_GAIN			19
#define ID_CURRENT_R_MOTOR		20
#define ID_CURRENT_L_MOTOR		21
#define ID_TEMPERATURE1_R		22
#define ID_TEMPERATURE2_R		23
#define ID_TEMPERATURE1_L		24
#define ID_TEMPERATURE2_L		25
#define ID_STATUS_DATALOG		26
#define ID_CURRENT_FLAG			27
#define ID_PANEL_DEBUG_1		28
#define ID_PANEL_DEBUG_2		29
#define ID_PANEL_DEBUG_3		30
#define ID_PANEL_DEBUG_4		31

#define CAN_ID_QUAN 32

typedef struct {
	uint16_t id;
	uint8_t pos;
} CAN_ID_t;

extern CAN_ID_t CAN_ID_map[CAN_ID_QUAN];

#define ECU_CAN_INITIAL_ID 				100
#define ECU_CAN_FINAL_ID 				149
#define ECU_CAN_LAST_POPULATED_ID 		108
#define ECU_CAN_FIRST_DEBUG_ID			351
#define ECU_CAN_LAST_DEBUG_ID			351

#define INTERNAL_ID_ECU_TIMER        	{101, 0}
#define INTERNAL_ID_STEERING_WHEEL		{101, 1}
#define INTERNAL_ID_THROTTLE			{101, 2}
#define INTERNAL_ID_BRAKE				{101, 3}
#define INTERNAL_ID_MODE				{102, 0}
#define INTERNAL_ID_DISTANCE_ODOM		{102, 2}
#define INTERNAL_ID_TORQUE_R_MOTOR		{103, 0}
#define INTERNAL_ID_TORQUE_L_MOTOR		{103, 1}
#define INTERNAL_ID_REF_TORQUE_R_MOTOR	{103, 2}
#define INTERNAL_ID_REF_TORQUE_L_MOTOR	{103, 3}
#define INTERNAL_ID_REF_SPEED_R			{104, 0}
#define INTERNAL_ID_REF_SPEED_L			{104, 1}
#define INTERNAL_ID_REF_TORQUE_NEG		{104, 2}
#define INTERNAL_ID_REGEN_BRAKE_QUAN	{104, 3}
#define INTERNAL_ID_SPEED_FL			{105, 0}
#define INTERNAL_ID_SPEED_FR			{105, 1}
#define INTERNAL_ID_SPEED_RR			{105, 2}
#define INTERNAL_ID_SPEED_RL			{105, 3}
#define INTERNAL_ID_TORQUE_GAIN			{106, 0}
#define INTERNAL_ID_CURRENT_R_MOTOR		{106, 2}
#define INTERNAL_ID_CURRENT_L_MOTOR		{106, 3}
#define INTERNAL_ID_TEMPERATURE1_R		{107, 0}
#define INTERNAL_ID_TEMPERATURE2_R		{107, 1}
#define INTERNAL_ID_TEMPERATURE1_L		{107, 2}
#define INTERNAL_ID_TEMPERATURE2_L		{107, 3}
#define INTERNAL_ID_STATUS_DATALOG		{108, 1}
#define INTERNAL_ID_CURRENT_FLAG		{108, 3}
#define INTERNAL_ID_PANEL_DEBUG_1		{351, 0}
#define INTERNAL_ID_PANEL_DEBUG_2		{351, 1}
#define INTERNAL_ID_PANEL_DEBUG_3		{351, 2}
#define INTERNAL_ID_PANEL_DEBUG_4		{351, 3}


//----------

void initialize_CAN_IDs();
uint16_t convert_id_pos_to_index(uint16_t id, uint16_t pos);
CAN_ID_t get_CAN_ID_from_internal(uint16_t internal);
uint16_t get_internal_from_id_pos(uint16_t id, uint16_t pos);


#endif /* INC_CAN_CAN_IDS_H_ */
