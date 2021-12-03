/*
 * longitudinal_control.c
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */

#include "DynamicControls/longitudinal_control.h"
#include "DynamicControls/PID.h"
#include "DynamicControls/constants_control.h"
#include "cmsis_os.h"
#include <math.h>
#include "speed_calc.h"
#include "global_variables.h"
#include "constants.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"

extern PID_t pid_longitudinal_L;
extern PID_t pid_longitudinal_R;

longitudinal_t longitudinal_control() {
    float cm_speed;
    double left_slip, right_slip;
    longitudinal_t ref_torque;

    // speed and slip ratios
    WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
    cm_speed = ((wheel_speeds.speed[FRONT_RIGHT] + wheel_speeds.speed[FRONT_LEFT])/2); // speed of the car's center of mass
    left_slip = ((wheel_speeds.speed[REAR_LEFT] - cm_speed) / cm_speed) * 100;   // slip ratio of the rear left wheel
    right_slip = ((wheel_speeds.speed[REAR_RIGHT] - cm_speed) / cm_speed) * 100;  // slip ratio of the rear right wheel
    // PID
    PID_set_setpoint(&pid_longitudinal_L, IDEAL_SLIP_DRY);
    PID_set_setpoint(&pid_longitudinal_R, IDEAL_SLIP_DRY);//TODO: fazer logica de seleção pista seca/molhada
    ref_torque.ref_decrease_L = PID_compute(&pid_longitudinal_L, left_slip);
    ref_torque.ref_decrease_R = PID_compute(&pid_longitudinal_R, right_slip);


    return ref_torque;
};
