/*
 * initializer_controls.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "DynamicControls/initializer_controls.h"
#include "DynamicControls/lateral_control.h"
#include "DynamicControls/PID.h"

const float wheelbase = FRONT_DISTANCE + REAR_DISTANCE;
const float ku = (REAR_DISTANCE * MASS / (FRONT_CORNER_STIFF * wheelbase)) - (FRONT_DISTANCE * MASS / (REAR_CORNER_STIFF * wheelbase));
PID_t pid_lateral;

void init_controls() {

	//Lateral Control
	PID_init(&pid_lateral, 1, KP, TI, 0, 4000, -4000, LATERAL_DELAY);
}
