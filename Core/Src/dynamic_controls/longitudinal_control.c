/*
 * longitudinal_control.c
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */

#include "dynamic_controls/longitudinal_control.h"

#include "cmsis_os.h"
#include "dynamic_controls/PID.h"
#include "dynamic_controls/constants_control.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_variables.h"
#include "util/util.h"

#include <math.h>

//Creates the longitudinal controller
PID_t pid_longitudinal;

//Function that initializes the longitudinal_control
void init_longitudinal_control() {

    //Initializes the rear left PID
    PID_init(&pid_longitudinal, 1, KP_LONGITUDINAL,
             TI_LONGITUDINAL, 0, 0, -NOMINAL_TORQUE, LONGITUDINAL_DELAY);
    //Defines the rear left PID's setpoint
    PID_set_setpoint(&pid_longitudinal, IDEAL_SLIP_DRY);
}

//Definition of the function that returns the result of an individual longitudinal controller. 
//First it calculates the controller input and than it calls the "PID_compute" function (that gives us the controller's output).
double wheel_control() {
    //Calculus variables
    double cg_speed;
    double rear_avg_speed;
    double slip;

    // speed of the car's center of mass
    cg_speed = (double)get_global_var_value(FRONT_AVG_SPEED);
    rear_avg_speed = (double)get_global_var_value(REAR_AVG_SPEED);

    // treatment made to avoid division by zero
    if (cg_speed < 1) {
        slip = 0;
    } else {
        //slip ratio calculation
        slip = ((rear_avg_speed - cg_speed)
                / cg_speed)
               * 100;
    }

    return fabs((double)(PID_compute(&(pid_longitudinal), slip)));
}



longitudinal_control_result_t longitudinal_control() {

    //Variable that will store the result of both right and left wheel
    longitudinal_control_result_t ref_torque_result;
    int ref_torque;

    //variable that stores the result of the longitudinal control
    double pid_result = wheel_control();

    //pid_result: delta torque 0 - 13 [N.m]
    //ref_torque: 0 to torq.max [%]
    modos mode = get_global_var_value(SELECTED_MODE);
    ref_torque = (fabs(pid_result)/NOMINAL_TORQUE) * mode.tor_max;

    //Store the right and left motor torque reference that must be subtracted
    ref_torque_result.torque_decrease[R_MOTOR] = ref_torque;
    ref_torque_result.torque_decrease[L_MOTOR] = ref_torque;


    return ref_torque_result;
}









