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




const uint8_t P_ENABLE   = 1 << 0,
			  P_RUNSTOP  = 1 << 1,
			  P_BRAKE    = 1 << 2,
			  P_THROTTLE = 1 << 3;

//TODO: reavaliar quantidade certa de delay
#define CONTROLE_DELAY 10
#define		_5_kmph_rpm 			459

#endif /* INC_CONTROLE_H_ */
