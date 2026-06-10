/*
 * longitudinal_control.c
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */

#include "dynamic_controls/longitudinal_control.h"

#include "cmsis_os.h"
#include "dynamic_controls/PID.h"
#include "datalogging/datalog_handler.h"
#include "dynamic_controls/constants_control.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_variables.h"
#include "util/util.h"

#include <math.h>

//Creates the longitudinal controller
static PID_t pid_longitudinal;

//Function that initializes the longitudinal_control
void init_longitudinal_control() {

    //Initializes the rear left PID
    PID_init(&pid_longitudinal, 1, KP_LONGITUDINAL,
             TI_LONGITUDINAL, 0, 0, -NOMINAL_TORQUE, LONGITUDINAL_DELAY);
    //Defines the longitudinal PID's setpoint
    PID_set_setpoint(&pid_longitudinal, IDEAL_SLIP_DRY);
    log_data(ID_SET_POINT_LONGITUDINAL, IDEAL_SLIP_DRY);
}



longitudinal_control_result_t longitudinal_control() {

    //Variables that will store the result 
    double pid_result;
    int ref_torque;
    longitudinal_control_result_t ref_torque_result;
    


    //Calculus variables
    double cg_speed;
    double rear_avg_speed;
    double slip;

    INTERNAL_WHEEL_t internal_wheel = get_global_var_value(INTERNAL_WHEEL);

    //speed of the car's center of mass
    cg_speed = (double)get_global_var_value(FRONT_AVG_SPEED);
    //avarage speed of the rear wheels
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
    log_data(ID_SLIP, slip);

    //This "if" is here to deactivate the controller when the slip is lower than the ideal or when the car is making a turn 
    if (slip <= 13 || internal_wheel != CENTRO) {
        pid_result = 0;
    } else {
        pid_result = fabs((double)(PID_compute(&(pid_longitudinal), slip)));
    }

    //pid_result: delta torque 0 - 13 [N.m]
    //ref_torque: 0 to torq.max [%]
    modos mode = get_global_var_value(SELECTED_MODE);
    ref_torque = (fabs(pid_result)/NOMINAL_TORQUE) * mode.tor_max;

    //Store the right and left motor torque reference that must be subtracted
    ref_torque_result.torque_decrease[R_MOTOR] = ref_torque;
    ref_torque_result.torque_decrease[L_MOTOR] = ref_torque;


    return ref_torque_result;
}









