/*
 * torque_manager.h
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#ifndef INC_TORQUE_MANAGER_H_
#define INC_TORQUE_MANAGER_H_

#include "stdint.h"
#include "cmsis_os.h"
#include "stdbool.h"

// Initialization os functions

static void torque_ramp(uint32_t * ref_torque, const double* ref_torque_decrease);
static void send_ref_torque_message(const uint32_t* ref_torque);
static void select_dynamic_control(bool is_DYNAMIC_CONTROL_active);


//TODO (Guilherme): It is necessary a type, where both controls is active,
//beeing the alteration bellow one of necessities of the integration of the controlers
typedef enum {NO_CONTROL = 0, LATERAL = 1, LONGITUDINAL = 2, BOTH_CONTROLS = 3} E_CONTROL_TYPE;



E_CONTROL_TYPE g_control_type = NO_CONTROL;

// defines the torque ramp
#define INC_TORQUE         300 // inclination of the second part of the ramp
#define INC_TORQUE_INIT    200 // inclination of the first part
#define TORQUE_INIT_LIMIT 500 // transition point of the ramps

#define RAMP_DELAY 30

#endif /* INC_TORQUE_MANAGER_H_ */
