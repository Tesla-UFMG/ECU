/*
 * longitudinal_control.c
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */

#include "DynamicControls/longitudinal_control.h"
#include "DynamicControls/initializer_controls.h"
#include "DynamicControls/PID.h"
#include "cmsis_os.h"
#include <math.h>
#include "speed_calc.h"
#include "global_variables.h"
#include "constants.h"

extern PID_t pid_longitudinal_L;
extern PID_t pid_longitudinal_R;

longitudinal_t longitudinal_control(float g_wheel_speed[4]) {
    float cm_speed;
    double left_slip, right_slip, setpoint;
    longitudinal_t ref_torque;

    // speed and slip ratios
    cm_speed = ((g_wheel_speed[FRONT_RIGHT] + g_wheel_speed[FRONT_LEFT])/2);  // speed of the car's center of mass
    left_slip = ((g_wheel_speed[REAR_LEFT] - cm_speed) / cm_speed) * 100;   // slip ratio of the rear left wheel
    right_slip = ((g_wheel_speed[REAR_RIGHT] - cm_speed) / cm_speed) * 100;  // slip ratio of the rear right wheel
    // PID
    setpoint = IDEAL_SLIP_DRY;
    PID_set_setpoint(&pid_longitudinal_L, setpoint);
    PID_set_setpoint(&pid_longitudinal_R, setpoint);
    ref_torque.ref_decrease_L = PID_compute(&pid_longitudinal_L, left_slip);
    ref_torque.ref_decrease_R = PID_compute(&pid_longitudinal_R, right_slip);


    return ref_torque;
};


