/*
 * torque_manager.h
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#ifndef INC_TORQUE_MANAGER_H_
#define INC_TORQUE_MANAGER_H_

#include "stdint.h"


//TODO (Guilherme): É necessário ter um "estado" em que ambos os controles estão ativos, senso a alteração abaixo uma das necessárias para a integração dos controles.
typedef enum {NO_CONTROL = 0, LATERAL = 1, LONGITUDINAL = 2, BOTH_CONTROLS = 3} E_CONTROL_TYPE;



E_CONTROL_TYPE g_control_type = NO_CONTROL;

// defines the torque ramp
#define INC_TORQUE         300 // inclination of the second part of the ramp
#define INC_TORQUE_INIT    200 // inclination of the first part
#define TORQUE_INIT_LIMITE 500 // transition point of the ramps

#define RAMPA_DELAY 30

#endif /* INC_TORQUE_MANAGER_H_ */
