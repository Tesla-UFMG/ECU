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
static longitudinal_t controlled_wheels[2]; //TODO: acrescentar outras rodas quando tivermos 4 motores.

void init_controls() {

	//Lateral Control
	PID_init(&pid_lateral, 1, KP_LATERAL, TI_LATERAL, 0, 4000, -4000, LATERAL_DELAY);
	//Longitudinal Control
	controlled_wheels[L_MOTOR].wheel = REAR_LEFT;
	controlled_wheels[R_MOTOR].wheel = REAR_RIGHT;
	PID_init(&controlled_wheels[L_MOTOR].pid_longitudinal, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
	PID_set_setpoint(&controlled_wheels[L_MOTOR].pid_longitudinal, IDEAL_SLIP_DRY);                  //TODO: fazer logica de seleção pista seca/molhada
	PID_init(&controlled_wheels[R_MOTOR].pid_longitudinal, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
	PID_set_setpoint(&controlled_wheels[R_MOTOR].pid_longitudinal, IDEAL_SLIP_DRY);

}
