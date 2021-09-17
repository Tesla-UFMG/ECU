/*
 * initializer_controls.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "DynamicControls/initializer_controls.h"
#include "DynamicControls/lateral_control.h"
#include "DynamicControls/longitudinal_control.h"
#include "DynamicControls/PID.h"

PID_t pid_lateral;
PID_t pid_longitudinal_L;
PID_t pid_longitudinal_R;

double ti_calculator(double kp, double ki){
	double ti;
	ti = kp / ki;
	return ti;
}

void init_controls() {
	double ti_lateral;
	double ti_longitudinal;

	//Lateral Control
	ti_lateral = ti_calculator(KP_LATERAL, KI_LATERAL);
	PID_init(&pid_lateral, 1, KP_LATERAL, ti_lateral, 0, 4000, -4000, LATERAL_DELAY);
	//Longitudinal Control
	ti_longitudinal = ti_calculator(KP_LONGITUDINAL, KI_LONGITUDINAL); // TODO: CHECK COMPILATION ERRORS
	PID_init(&pid_longitudinal_L, 1, KP_LONGITUDINAL, ti_longitudinal, 0, 4000, 0, LONGITUDINAL_DELAY);
	PID_init(&pid_longitudinal_R, 1, KP_LONGITUDINAL, ti_longitudinal, 0, 4000, 0, LONGITUDINAL_DELAY);

}
