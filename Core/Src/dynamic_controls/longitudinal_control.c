/*
 * longitudinal_control.c
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */

#include "dynamic_controls/longitudinal_control.h"

#include "DynamicControls/constants_control.h"
#include "cmsis_os.h"
#include "dynamic_controls/PID.h"
#include "sensors/wheel_speed.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_variables.h"
#include "util/util.h"

#include <math.h>

static longitudinal_t controlled_wheels[2] = {
    [L_MOTOR].wheel = REAR_LEFT, [R_MOTOR].wheel = REAR_RIGHT};

void init_longitudinal_control() {
    PID_init(&controlled_wheels[L_MOTOR].pid_longitudinal, 1, KP_LONGITUDINAL,
             TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
    PID_set_setpoint(&controlled_wheels[L_MOTOR].pid_longitudinal, IDEAL_SLIP_DRY);
    PID_init(&controlled_wheels[R_MOTOR].pid_longitudinal, 1, KP_LONGITUDINAL,
             TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
    PID_set_setpoint(&controlled_wheels[R_MOTOR].pid_longitudinal, IDEAL_SLIP_DRY);
    // TODO(Giovanni): fazer logica de selecao pista seca/molhada
}

double wheel_control(uint8_t wheel_motor, SPEEDS_t speeds) {
    float cm_speed;
    double slip;

    // speed and slip ratios
    // speed of the car's center of mass
    cm_speed = (float)get_global_var_value(REAR_AVG_SPEED);
    // slip ratio of the selected wheel
    slip = ((speeds.wheels[controlled_wheels[wheel_motor].wheel] - cm_speed) / cm_speed)
           * 100;
    // PID
    return (
        uint32_t)(PID_compute(&(controlled_wheels[wheel_motor].pid_longitudinal), slip));
}

longitudinal_control_result_t longitudinal_control() {
    longitudinal_control_result_t result;
    SPEEDS_t speeds = get_global_var_value(SPEEDS);

    result.torque_decrease[R_MOTOR] = wheel_control(R_MOTOR, speeds);
    result.torque_decrease[L_MOTOR] = wheel_control(L_MOTOR, speeds);

    return result;
}
