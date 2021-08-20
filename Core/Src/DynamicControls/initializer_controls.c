/*
 * initializer_controls.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "DynamicControls/initializer_controls.h"
#include "DynamicControls/lateral_control.h"
#include "DynamicControls/PID.h"
#include "stdint.h"

void init_controls() {
	const uint32_t wheelbase = FRONT_DISTANCE + REAR_DISTANCE;

	//Lateral Control
	const uint32_t ku = (REAR_DISTANCE * MASS / (FRONT_CORNER_STIFF * wheelbase)) - (FRONT_DISTANCE * MASS / (REAR_CORNER_STIFF * wheelbase));
	PID_t pid_lateral;
	PID_init(&pid_lateral, 1, KP, TI, 0, 4000, -4000, LATERAL_DELAY);
}
