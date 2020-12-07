/*
 * controle.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CONTROLE_H_
#define INC_CONTROLE_H_

#include "global_variables.h"
#include "global_definitions.h"
#include "global_instances.h"
#include "cmsis_os.h"

typedef enum estado_veiculo {
	S_DISABLE_E = 0,
	S_BRAKE_E = 1,
	S_ACCELERATE_E = 2,
	S_NEUTER_E = 3
} vehicle_state_e;


const uint8_t P_ENABLE   = 1 << 0,
			  P_RUNSTOP  = 1 << 1,
			  P_BRAKE    = 1 << 2,
			  P_THROTTLE = 1 << 3;



#endif /* INC_CONTROLE_H_ */
