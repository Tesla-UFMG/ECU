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

//Variable that contains important dada about the longitudinal control. The variable is defined as an array, so there is a separete controller and, therefor, a separete process for each rear wheel of the vehicle.
static longitudinal_t controlled_wheels[2] = {
    [L_MOTOR].wheel = REAR_LEFT, [R_MOTOR].wheel = REAR_RIGHT};

//TODO (Guilherme): Será que deixamos um controlador atuando em individualmente nas rodas? Durante o projeto de validação foi utilizado um controlador para as 2 rodas.
//Function that initializes the longitudinal_control
void init_longitudinal_control() {

    //Initializes the rear left PID
    PID_init(&controlled_wheels[L_MOTOR].pid_longitudinal, 1, KP_LONGITUDINAL,
             TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
    //Defines the rear left PID's setpoint
    PID_set_setpoint(&controlled_wheels[L_MOTOR].pid_longitudinal, IDEAL_SLIP_DRY);

    //Initializes the rear right PID
    PID_init(&controlled_wheels[R_MOTOR].pid_longitudinal, 1, KP_LONGITUDINAL,
             TI_LONGITUDINAL, 0, 4000, 0, LONGITUDINAL_DELAY);
             //Defines the rear right PID's setpoint

    PID_set_setpoint(&controlled_wheels[R_MOTOR].pid_longitudinal, IDEAL_SLIP_DRY);
    // TODO(Giovanni): fazer logica de selecao pista seca/molhada
}

//Function that returns the result of an individual longitudinal controller. First it calculates the controller input and than it calls the "PID_compute" function (that gives us the controller's output)
double wheel_control(uint8_t wheel_motor, SPEEDS_t speeds) {
    //Calculus variables
    float cm_speed;
    double slip;

    // speed of the car's center of mass
    //TODO (Guilherme): A velocidade do centro de massa não é a média das rodas dianteiras? 
    cm_speed = (float)get_global_var_value(REAR_AVG_SPEED);
    // slip ratio of the selected wheel
    slip = (((float)(speeds.wheels[controlled_wheels[wheel_motor].wheel]) - cm_speed)
            / cm_speed)
           * 100;

    //Returns the individual output
    return (
        uint32_t)(PID_compute(&(controlled_wheels[wheel_motor].pid_longitudinal), slip));
}

//Function that returns the longitudinal control's result (it contains the result of each wheel controller).
longitudinal_control_result_t longitudinal_control() {
    //Variable that will store the result of both right and left wheel
    longitudinal_control_result_t result;

    //Gets the speed of the wheels
    SPEEDS_t speeds = get_global_var_value(SPEEDS);

    //Store the right and left motor PID's result
    result.torque_decrease[R_MOTOR] = wheel_control(R_MOTOR, speeds);
    result.torque_decrease[L_MOTOR] = wheel_control(L_MOTOR, speeds);

    return result;
}
