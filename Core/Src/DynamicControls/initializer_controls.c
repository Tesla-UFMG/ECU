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
<<<<<<< Updated upstream
static PID_t pid_longitudinal_L;
static PID_t pid_longitudinal_R;
=======
PID_t pid_longitudinal_L;
PID_t pid_longitudinal_R;
>>>>>>> Stashed changes


void init_controls() {
	//Lateral Control
	PID_init(&pid_lateral, 1, KP_LATERAL, TI_LATERAL, 0, 4000, -4000, LATERAL_DELAY);
	//Longitudinal Control
<<<<<<< Updated upstream
	PID_init(&pid_longitudinal_L, /*reset=*/true, KP_LONGITUDINAL, TI_LONGITUDINAL, /*Td=*/0, /*max_output=*/4000, /*min_output=*/0, LONGITUDINAL_DELAY);
	PID_init(&pid_longitudinal_L, /*reset=*/true, KP_LONGITUDINAL, TI_LONGITUDINAL, /*Td=*/0, /*max_output=*/4000, /*min_output=*/0, LONGITUDINAL_DELAY);
=======
	PID_init(&pid_longitudinal_L, 1, KP_LONGITUDINAL, TI_LONGITUDINAL,0, 4000, 0, LONGITUDINAL_DELAY);
	PID_init(&pid_longitudinal_L, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
>>>>>>> Stashed changes
}
