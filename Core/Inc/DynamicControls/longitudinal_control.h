/*
 * longitudinal_control.h
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */
#ifndef INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_

#include "stdint.h"
#include "PID.h"

#define LONGITUDINAL_DELAY 30
#define IDEAL_SLIP_DRY 13
#define IDEAL_SLIP_WET 30


typedef struct {
	double ref_decrease;
	uint8_t wheel;
	PID_t pid_longitudinal;
} longitudinal_t;

void longitudinal_control(longitudinal_t *control_wheel);


#endif /* INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_ */
