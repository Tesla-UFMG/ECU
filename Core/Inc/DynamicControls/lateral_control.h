/*
 * lateral_control.h
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_

#include "speed_calc.h"
#include "global_definitions.h"
#include "global_variables.h"
#include "constants.h"

//defines PID
#define KP 6.51864262048678
#define TI 0.14843404179579
#define LATERAL_DELAY 30

typedef struct {
	double ref_decrease;
	uint8_t ref_wheel;
} lateral_t;

#endif /* INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_ */
