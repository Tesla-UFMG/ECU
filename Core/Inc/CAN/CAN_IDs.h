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
#define VARIABLES_GENERAL_CAN_TX\
/*Driver Inputs*/\
    CAN_GENERAL_LIST_DATA(ID_PILOT_REF_TORQUE, 16, 0) /*Driver requested torque reference (before torque ramp and dynamic controllers). Ranges from 0 to 2500 depending on the selected mode (no scaling required).*/ \
    CAN_GENERAL_LIST_DATA(ID_THROTTLE, 16, 1) /*APPS travel percentage. Ranges from 0 to 1000 (Value in % * 10, i.e., divide by 10 to obtain percentage).*/ \
    CAN_GENERAL_LIST_DATA(ID_STEERING_WHEEL, 16, 2) /*Average front wheel steering angle in radians. To obtain radians, divide the raw value by 1000 and subtract (2*PI).*/ \
    CAN_GENERAL_LIST_DATA(ID_INTERNAL_WHEEL, 16, 3) /*Inside cornering wheel. 3 possible states: 0 is right; 1 is left; 2 is center.*/ \
    /*Torque References*/\
    CAN_GENERAL_LIST_DATA(ID_REF_TORQUE_R, 17, 0) /*Actual torque reference command sent to the right inverter. Ranges from 0 to 2500 depending on mode (Value from 0 to 1000 in % * 10, i.e., divide by 10).*/ \
    CAN_GENERAL_LIST_DATA(ID_REF_TORQUE_L, 17, 1) /*Actual torque reference command sent to the left inverter. Ranges from 0 to 2500 depending on mode (Value from 0 to 1000 in % * 10, i.e., divide by 10).*/ \
    CAN_GENERAL_LIST_DATA(ID_DC_REF_TORQUE_R, 17, 2) /*Dynamic control offset on the right inverter torque reference. To obtain torque reference (%), subtract 2500 and divide by 10.*/ \
    CAN_GENERAL_LIST_DATA(ID_DC_REF_TORQUE_L, 17, 3) /*Dynamic control offset on the left inverter torque reference. To obtain torque reference (%), subtract 2500 and divide by 10.*/ \
    /*Speeds*/\
    CAN_GENERAL_LIST_DATA(ID_SPEED_FR, 18, 0) /*Front right wheel speed in km/h * 10. Divide by 10 to obtain km/h.*/ \
    CAN_GENERAL_LIST_DATA(ID_SPEED_FL, 18, 1) /*Front left wheel speed in km/h * 10. Divide by 10 to obtain km/h.*/ \
    CAN_GENERAL_LIST_DATA(ID_SPEED_RR, 18, 2) /*Rear right wheel speed in km/h * 10. Divide by 10 to obtain km/h.*/ \
    CAN_GENERAL_LIST_DATA(ID_SPEED_RL, 18, 3) /*Rear left wheel speed in km/h * 10. Divide by 10 to obtain km/h.*/ \
    /*Accelerations*/\
    CAN_GENERAL_LIST_DATA(ID_ACCEL_FR, 19, 0) /*Front right wheel acceleration in m/s² * 100. Divide by 100 to obtain m/s².*/ \
    CAN_GENERAL_LIST_DATA(ID_ACCEL_FL, 19, 1) /*Front left wheel acceleration in m/s² * 100. Divide by 100 to obtain m/s².*/ \
    CAN_GENERAL_LIST_DATA(ID_ACCEL_RR, 19, 2) /*Rear right wheel acceleration in m/s² * 100. Divide by 100 to obtain m/s².*/ \
    CAN_GENERAL_LIST_DATA(ID_ACCEL_RL, 19, 3) /*Rear left wheel acceleration in m/s² * 100. Divide by 100 to obtain m/s².*/ \
    /*Dynamics Control 1*/\
    CAN_GENERAL_LIST_DATA(ID_SET_POINT_LATERAL, 20, 0) /*Lateral control setpoint multiplied by 1000. Divide by 1000 to obtain rad/s.*/ \
    CAN_GENERAL_LIST_DATA(ID_KP, 20, 1) /*Lateral controller proportional gain (Kp) multiplied by 100. Divide by 100 to obtain original value.*/ \
    CAN_GENERAL_LIST_DATA(ID_TI, 20, 2) /*Lateral controller integral time constant Ti (Kp/Ki) multiplied by 100. Divide by 100 to obtain original value.*/ \
    /*Dynamics Control 2*/\
    CAN_GENERAL_LIST_DATA(ID_MAX_YAW, 21, 0) /*Maximum yaw rate limit from lateral controller in rad/s * 1000. Divide by 1000 to obtain rad/s. Note: stored as magnitude (>= 0); sign depends on steering direction.*/ \
    CAN_GENERAL_LIST_DATA(ID_DESIRED_YAW, 21, 1) /*Target yaw rate from lateral controller in rad/s * 1000. Divide by 1000 to obtain rad/s. Note: stored as magnitude (>= 0); sign depends on steering direction.*/ \
    CAN_GENERAL_LIST_DATA(ID_SLIP_R, 21, 2) /*Rear right wheel slip ratio multiplied by 10000. Divide by 10000 to obtain actual value.*/ \
    CAN_GENERAL_LIST_DATA(ID_SLIP_L, 21, 3) /*Rear left wheel slip ratio multiplied by 10000. Divide by 10000 to obtain actual value.*/ \
    /*Inverters State*/\
    CAN_GENERAL_LIST_DATA(ID_INV_STATE_L_MOTOR, 22, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_INV_STATE_R_MOTOR, 22, 1) /**/ \
    /*Motors Torque and Speed*/\
    CAN_GENERAL_LIST_DATA(ID_TORQUE_L_MOTOR, 23, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_SPEED_L_MOTOR, 23, 1) /**/ \
    CAN_GENERAL_LIST_DATA(ID_TORQUE_R_MOTOR, 23, 2) /**/ \
    CAN_GENERAL_LIST_DATA(ID_SPEED_R_MOTOR, 23, 3) /**/ \
    /*Mosfets Temperature*/\
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE1_L, 24, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE2_L, 24, 1) /**/ \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE1_R, 24, 2) /**/ \
    CAN_GENERAL_LIST_DATA(ID_TEMPERATURE2_R, 24, 3) /**/ \
    /*Motors Power and Energy*/\
    CAN_GENERAL_LIST_DATA(ID_ENERGY_R_MOTOR, 25, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_POWER_R_MOTOR, 25, 1) /**/ \
    CAN_GENERAL_LIST_DATA(ID_ENERGY_L_MOTOR, 25, 2) /**/ \
    CAN_GENERAL_LIST_DATA(ID_POWER_L_MOTOR, 25, 3) /**/ \
    /*Motors Info*/\
    CAN_GENERAL_LIST_DATA(ID_CURRENT_L_MOTOR, 26, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_CURRENT_R_MOTOR, 26, 1) /**/ \
    /*Inverters Communication Info*/\
    CAN_GENERAL_LIST_DATA(ID_LOST_MSG_L_MOTOR, 27, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_LOST_MSG_R_MOTOR, 27, 1) /**/ \
    /*Inverter CAN States*/\
    CAN_GENERAL_LIST_DATA(ID_CAN_STATE_L_MOTOR, 28, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_CAN_STATE_R_MOTOR, 28, 1) /**/ \
    /*Inverter Bus OFF Status*/\
    CAN_GENERAL_LIST_DATA(ID_BUS_OFF_L_MOTOR, 29, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_BUS_OFF_R_MOTOR, 29, 1) /**/ \
    /*Motors Alarms*/\
    CAN_GENERAL_LIST_DATA(ID_FAILURE_R_MOTOR, 66, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_ALARM_L_MOTOR, 66, 1) /**/ \
    CAN_GENERAL_LIST_DATA(ID_FAILURE_L_MOTOR, 66, 2) /**/ \
    CAN_GENERAL_LIST_DATA(ID_ALARM_R_MOTOR, 66, 3) /**/ \
    /*Active Dynamic Control*/\
    CAN_GENERAL_LIST_DATA(ID_G_CONTROL_TYPE, 67, 0) /*Active dynamic control mode applied to the vehicle. 3 possible values: 0 is NO_CONTROL; 1 is LATERAL; 2 is LONGITUDINAL.*/ \
    /*Current Driving Mode*/\
    CAN_GENERAL_LIST_DATA(ID_MODE, 68, 0) /*Current vehicle driving mode. 5 possible values: 0 is ERROR; 1 is ENDURO (100%); 2 is ACCELERATION (150%); 3 is SKIDPAD (200%); 4 is AUTOCROSS (250%).*/ \
    /*Control Event Flags*/\
    CAN_GENERAL_LIST_DATA(ID_CONTROL_EVENT_FLAG_1, 69, 0) /*Most significant 16 bits (MSB) of the 32-bit control flags. Must be concatenated with ID_CONTROL_EVENT_FLAG_2 to obtain full bitmask.*/ \
    CAN_GENERAL_LIST_DATA(ID_CONTROL_EVENT_FLAG_2, 69, 1) /*Least significant 16 bits (LSB) of the 32-bit control flags. Must be concatenated with ID_CONTROL_EVENT_FLAG_1 to obtain full bitmask.*/ \
    /*Odometer Info*/\
    CAN_GENERAL_LIST_DATA(ID_DISTANCE_P_ODOM, 70, 0) /*Distance traveled by the vehicle in meters (trip odometer). Resets every time the car exits RTD (Ready To Drive). No scaling required.*/ \
    /*Motors Info*/\
    CAN_GENERAL_LIST_DATA(ID_OVERLOAD_R_MOTOR, 71, 0) /**/ \
    CAN_GENERAL_LIST_DATA(ID_OVERLOAD_L_MOTOR, 71, 1) /**/ \
    /*Brake Status*/\
    CAN_GENERAL_LIST_DATA(ID_BRAKE, 72, 0) /*Brake switch state (boolean). 1 (true) = brake pedal pressed; 0 (false) = brake pedal released.*/ \
    /*Regenerative Braking Info*/\
    CAN_GENERAL_LIST_DATA(ID_REGEN_BRAKE_STATE, 84, 0) /*Regenerative braking state (boolean). Follows the brake state logic; expected value is always 0 as regenerative braking is currently unused.*/ \
    CAN_GENERAL_LIST_DATA(ID_DEBUG_1, 84, 1) /**/ \
    CAN_GENERAL_LIST_DATA(ID_DEBUG_2, 84, 2) /**/ \
    CAN_GENERAL_LIST_DATA(ID_DEBUG_3, 84, 3) /**/ 
typedef enum {
    RESERVED_GENERAL_ID_0,
#define CAN_GENERAL_LIST_DATA(var_name, msg_id, msg_wrd) var_name,
    VARIABLES_GENERAL_CAN_TX
#undef CAN_GENERAL_LIST_DATA
        CAN_GENERAL_ID_QUAN
} CAN_general_list_e;

typedef struct {
    uint16_t id;
    uint16_t message_to_send;
    uint16_t original_enum; // Guarda a posição original
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
