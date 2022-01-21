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
#include "DynamicControls/PID.h"
#include "speed_calc.h"
#include "global_variables.h"
#include "constants.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"

static longitudinal_t controlled_wheels[2] = { [L_MOTOR].wheel = REAR_LEFT, [R_MOTOR].wheel = REAR_RIGHT};

void init_longitudinal_control(){
    PID_init(&controlled_wheels[L_MOTOR].pid_longitudinal, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
    PID_set_setpoint(&controlled_wheels[L_MOTOR].pid_longitudinal, IDEAL_SLIP_DRY);                  //TODO: fazer logica de selecao pista seca/molhada
    PID_init(&controlled_wheels[R_MOTOR].pid_longitudinal, 1, KP_LONGITUDINAL, TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
    PID_set_setpoint(&controlled_wheels[R_MOTOR].pid_longitudinal, IDEAL_SLIP_DRY);
}

double wheel_control(uint8_t wheel_motor, WHEEL_SPEEDS_t wheel_speeds){
    float cm_speed;
    double slip;

    // speed and slip ratios
    cm_speed = ((wheel_speeds.speed[FRONT_RIGHT] + wheel_speeds.speed[FRONT_LEFT])/2); // speed of the car's center of mass
    slip = ((wheel_speeds.speed[controlled_wheels[wheel_motor].wheel] - cm_speed) / cm_speed) * 100;    // slip ratio of the selected wheel
    // PID
    return (uint32_t)(PID_compute(&(controlled_wheels[wheel_motor].pid_longitudinal), slip));
}
longitudinal_control_result_t  longitudinal_control(){
    longitudinal_control_result_t result;
    WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);

    result.torque_decrease[R_MOTOR] = wheel_control(R_MOTOR, wheel_speeds);
    result.torque_decrease[L_MOTOR] = wheel_control(L_MOTOR, wheel_speeds);

    return result;
}
