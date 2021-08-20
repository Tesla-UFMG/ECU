/*
 * lateral_control.h
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_

#include "speed_calc.h"

//defines PID
#define KP 6.51864262048678
#define TI 0.14843404179579
#define LATERAL_DELAY 30

typedef struct {
	uint32_t ref_decrease;
	speed_pin_e wheel;
} lateral_t;

#endif /* INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_ */
