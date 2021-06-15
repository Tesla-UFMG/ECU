/*
 * torque_manager.h
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#ifndef INC_TORQUE_MANAGER_H_
#define INC_TORQUE_MANAGER_H_

#include "stdint.h"

typedef enum {
	NO_CONTROL = 0,
	LONGITUDINAL = 1,
	LATERAL = 3
} E_CONTROL_TYPE;

const E_CONTROL_TYPE g_control_type = NO_CONTROL;


// defines rampa de torque
#define INC_TORQUE 300						// inclinacao 2a parte da rampa
#define INC_TORQUE_INIT 200					// inclinacao 1a parte
#define TORQUE_INIT_LIMITE 500				// ponto de transicao entre as rampas

#define RAMPA_DELAY 60

#endif /* INC_TORQUE_MANAGER_H_ */
