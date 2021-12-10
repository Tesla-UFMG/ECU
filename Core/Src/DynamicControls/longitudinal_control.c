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

extern longitudinal_t controlled_wheels[2];

uint32_t longitudinal_control(uint8_t wheel_motor){
    float cm_speed;
    double slip;

    // speed and slip ratios
    WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
    cm_speed = ((wheel_speeds.speed[FRONT_RIGHT] + wheel_speeds.speed[FRONT_LEFT])/2); // speed of the car's center of mass
    slip = ((wheel_speeds.speed[controlled_wheels[wheel_motor].wheel] - cm_speed) / cm_speed) * 100;    // slip ratio of the selected wheel
    // PID
    return (uint32_t)(PID_compute(&(controlled_wheels[wheel_motor].pid_longitudinal), slip));


};
