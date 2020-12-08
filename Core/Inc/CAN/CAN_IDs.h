/*
 * CAN_IDs.h
 *
 *  Created on: Dec 8, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CAN_CAN_IDS_H_
#define INC_CAN_CAN_IDS_H_


//INVERTER

#define	ID_RIGHT_INVERTER 	0x301
#define	ID_LEFT_INVERTER 	0x300
#define	ID_COMM_FLAG		0x302		// flag de comunicacao ecu-inv ok

//----------
//DATALOGGER
//LEGENDA:  LABEL    	    	 ID  POS
#define ID_ECU_TIMER        	{101, 0}
#define ID_STEERING_WHEEL		{101, 1}
#define ID_THROTTLE				{101, 2}
#define ID_BRAKE				{101, 3}
#define ID_MODE					{102, 0}
#define ID_DISTANCE_ODOM		{102, 2}
#define ID_TORQUE_R_MOTOR		{103, 0}
#define ID_TORQUE_L_MOTOR		{103, 1}
#define ID_REF_TORQUE_R_MOTOR	{103, 2}
#define ID_REF_TORQUE_L_MOTOR	{103, 3}
#define ID_REF_SPEED_R			{104, 0}
#define ID_REF_SPEED_L			{104, 1}
#define ID_REF_TORQUE_NEG		{104, 2}
#define ID_REGEN_BRAKE_QUAN		{104, 3}
#define ID_SPEED_FL				{105, 0}
#define ID_SPEED_FR				{105, 1}
#define ID_SPEED_RR				{105, 2}
#define ID_SPEED_RL				{105, 3}
#define ID_TORQUE_GAIN			{106, 0}
#define ID_CURRENT_R_MOTOR		{106, 2}
#define ID_CURRENT_L_MOTOR		{106, 3}
#define ID_TEMPERATURE1_R		{107, 0}
#define ID_TEMPERATURE2_R		{107, 1}
#define ID_TEMPERATURE1_L		{107, 2}
#define ID_TEMPERATURE2_L		{107, 3}
#define ID_STATUS_DATALOG		{108, 1}
#define ID_CURRENT_FLAG			{108, 3}
#define ID_PANEL_DEBUG_1		{351, 0}
#define ID_PANEL_DEBUG_2		{351, 1}
#define ID_PANEL_DEBUG_3		{351, 2}
#define ID_PANEL_DEBUG_4		{351, 3}
//----------


#endif /* INC_CAN_CAN_IDS_H_ */
