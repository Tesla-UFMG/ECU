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
             TI_LONGITUDINAL, 0, NOMINAL_TORQUE, 0, LONGITUDINAL_DELAY);
    //Defines the rear left PID's setpoint
    PID_set_setpoint(&pid_longitudinal, IDEAL_SLIP_DRY);
}

//Function that returns the result of an individual longitudinal controller. First it calculates the controller input and than it calls the "PID_compute" function (that gives us the controller's output)
double wheel_control() {
    //Calculus variables
    uint16_t cg_speed;
    uint16_t rear_avg_speed;
    double slip;

    // speed of the car's center of mass
    cg_speed = (uint16_t)get_global_var_value(FRONT_AVG_SPEED);
    rear_avg_speed = (uint16_t)get_global_var_value(REAR_AVG_SPEED);

    // slip ratio of the selected wheel
    slip = ((rear_avg_speed - cg_speed)
            / cg_speed)
           * 100;


    return (double)(PID_compute(&(pid_longitudinal), slip));
}

    //TODO (Guilherme): Verificar qual das implementações é melhor
    /*
    double wheel_control(SPEEDS_t speeds) {

        double slip_right;
        uint16_t front_right_speed = speeds.wheels[FRONT_RIGHT];
        uint16_t rear_right_speed = speeds.wheels[REAR_RIGHT];

        slip_right = ((rear_right_speed - front_right_speed)
                / front_right_speed)
                * 100;


        double slip_left;
        uint16_t front_left_speed = speeds.wheels[FRONT_LEFT];
        uint16_t rear_left_speed = speeds.wheels[REAR_LEFT];

        slip_left = ((rear_left_speed - front_left_speed)
                / front_left_speed)
                * 100;

        slip = avg(slip_right, slip_left);


        return (uint32_t)(PID_compute(&(pid_longitudinal), slip));
    }

    */


    //Returns the individual output


longitudinal_control_result_t longitudinal_control() {

    //Variable that will store the result of both right and left wheel
    longitudinal_control_result_t ref_torque_result;
    int ref_torque;

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
//TODO (Guilherme): Verificar qual das implementações é melhor

/*
//Function that returns the longitudinal control's result (it contains the result of each wheel controller).
longitudinal_control_result_t longitudinal_control() {
    //Variable that will store the result of both right and left wheel
    longitudinal_control_result_t ref_torque_result;
    int ref_torque;

    //Gets the speed of the wheels
    SPEEDS_t speeds = get_global_var_value(SPEEDS);

    //TODO (Guilherme): verificar qual o tipo da variável do torque e substituir no PID_output
    double pid_result = wheel_control(speeds);

    //pid_result: delta torque 0 - 13 [N.m]
    //ref_torque: 0 to torq.max [%]
    modos mode = get_global_var_value(SELECTED_MODE);
    ref_torque = (fabs(pid_result)/NOMINAL_TORQUE) * mode.tor_max;

    //Store the right and left motor torque reference that must be subtracted
    ref_torque_result.torque_decrease[R_MOTOR] = ref_torque;
    ref_torque_result.torque_decrease[L_MOTOR] = ref_torque;

    return ref_torque_result;
}
*/




