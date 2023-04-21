/*
 * torque_manager.h
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#ifndef INC_TORQUE_MANAGER_H_
#define INC_TORQUE_MANAGER_H_

#include "stdint.h"

// todo: add a new type of control when the integration of two is implemented
// controls

typedef enum { LONGITUDINAL = 2, LATERAL = 1, NO_CONTROL = 0 } E_CONTROL_TYPE;

E_CONTROL_TYPE g_control_type = NO_CONTROL;

// defines torque ramp
#define INC_TORQUE         300 // slope of the ramp second part
#define INC_TORQUE_INIT    200 // slope of the ramp first part
#define TORQUE_INIT_LIMITE 500 // transition point between ramps

#define RAMPA_DELAY 30

#endif /* INC_TORQUE_MANAGER_H_ */
