/*
 * longitudinal_control.h
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */
#ifndef INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_

#include "stdint.h"

//defines PID
#define KP_LONGITUDINAL 6.51864262048678// TODO: check values
#define KI_LONGITUDINAL 0.14843404179579
#define TI_LONGITUDINAL KP_LONGITUDINAL / KI_LONGITUDINAL
#define LONGITUDINAL_DELAY 30
#define IDEAL_SLIP_DRY 13
#define IDEAL_SLIP_WET 30


typedef struct {
	double ref_decrease_L;
	double ref_decrease_R;

} longitudinal_t;

longitudinal_t longitudinal_control(volatile float g_wheel_speed[4]);


#endif /* INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_ */

