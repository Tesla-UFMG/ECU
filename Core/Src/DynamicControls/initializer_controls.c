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


void init_controls() {
	//Lateral Control
	PID_init(&pid_lateral, 1, KP_LATERAL, TI_LATERAL, 0, 4000, -4000, LATERAL_DELAY);
	//Longitudinal Control
	PID_init(&pid_longitudinal_L, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
	PID_init(&pid_longitudinal_R, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);

}
