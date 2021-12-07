/*
 * initializer_controls.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "DynamicControls/initializer_controls.h"
#include "DynamicControls/lateral_control.h"
#include "global_definitions.h"
#include "DynamicControls/longitudinal_control.h"
#include "DynamicControls/PID.h"

PID_t pid_lateral;
longitudinal_t rear_left;
longitudinal_t rear_right;     //TODO: acrescentar outras rodas quando tivermos 4 motores.

void init_controls() {

	//Lateral Control
	PID_init(&pid_lateral, 1, KP_LATERAL, TI_LATERAL, 0, 4000, -4000, LATERAL_DELAY);
	//Longitudinal Control
	rear_left.wheel = REAR_LEFT;
	rear_right.wheel = REAR_RIGHT;
	PID_init(&rear_left.pid_longitudinal, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
	PID_init(&rear_right.pid_longitudinal, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);

}
