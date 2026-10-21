/*
 * longitudinal_control.c
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */

#include "dynamic_controls/longitudinal_control.h"

#include "CAN/general_can_data_manager.h"
#include "cmsis_os.h"
#include "dynamic_controls/PID.h"
#include "datalogging/datalog_handler.h"
#include "dynamic_controls/constants_control.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_variables.h"
#include "util/util.h"
#include "sensors/encoder_speed.h"
#include "datalogging/speed.h"
#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "datalogging/datalog_handler.h"
#include "datalogging/speed.h"
#include "stm32h7xx.h"
#include "util/CMSIS_extra/cmsis_extra.h"
#include "util/global_instances.h"
#include "util/util.h"
#include "sensors/encoder_speed.h"

#include <math.h>

//Creates the longitudinal controller
static PID_t pid_longitudinal[2];
float slip_teste[2] = {0, 0};
SPEEDS_t speeds_teste = {.wheels = {0, 0, 0, 0}};
//Function that initializes the longitudinal_control
void init_longitudinal_control() {

    //Initializes the rear right PID
    PID_init(&pid_longitudinal[R_MOTOR], 1, KP_LONGITUDINAL,
             TI_LONGITUDINAL, 0, 0, -NOMINAL_TORQUE, LONGITUDINAL_DELAY_S);
    //Defines the rear right longitudinal PID's setpoint
    PID_set_setpoint(&pid_longitudinal[R_MOTOR], IDEAL_SLIP_DRY);

    //Initializes the rear left PID
    PID_init(&pid_longitudinal[L_MOTOR], 1, KP_LONGITUDINAL,
    TI_LONGITUDINAL, 0, 0, -NOMINAL_TORQUE, LONGITUDINAL_DELAY_S);
    //Defines the rear left longitudinal PID's setpoint
    PID_set_setpoint(&pid_longitudinal[L_MOTOR], IDEAL_SLIP_DRY);
}


longitudinal_result_t longitudinal_control() {

    //Variables that will store the result
    double pid_result[2] = {0, 0};
    int ref_torque[2] = {0, 0};
    longitudinal_result_t ref_torque_result = {.torque_decrease = {0, 0}};

    if (!is_RTD_active()){

        ref_torque_result.torque_decrease[R_MOTOR] = 0;
        ref_torque_result.torque_decrease[L_MOTOR] = 0;
        PID_reset(&pid_longitudinal[R_MOTOR]);
        PID_reset(&pid_longitudinal[L_MOTOR]);

        return ref_torque_result;

    }

    modos mode = get_global_var_value(SELECTED_MODE);

    PID_set_limits(&pid_longitudinal[R_MOTOR], 0, -((mode.tor_max/1000.0)*NOMINAL_TORQUE));
    PID_set_limits(&pid_longitudinal[L_MOTOR], 0, -((mode.tor_max/1000.0)*NOMINAL_TORQUE));

    //Calculus variables
    float slip[2] = {0, 0};
    SPEEDS_t speeds = get_global_var_value(SPEEDS);
    //speeds.wheels[FRONT_RIGHT] = 200;
	//speeds.wheels[FRONT_LEFT] = 200;
    speeds_teste = speeds;
    
    INTERNAL_WHEEL_t internal_wheel = get_global_var_value(INTERNAL_WHEEL);

    // treatment made to avoid division by zero
    if (speeds.wheels[FRONT_RIGHT] < 10 || speeds.wheels[FRONT_LEFT] < 10) {
        ref_torque_result.torque_decrease[R_MOTOR] = 0;
        ref_torque_result.torque_decrease[L_MOTOR] = 0;
        for (int i = 0; i < 2; i++){
            slip[i]=0;
            slip_teste[i]=0;
            PID_reset(&pid_longitudinal[i]);
        }
        return ref_torque_result;
    } else{
    
        if (speeds.wheels[FRONT_RIGHT] > speeds.wheels[REAR_RIGHT]){
            slip[R_MOTOR] = 0;
            slip_teste[R_MOTOR] = 0;
        } else {
            slip[R_MOTOR] = (float)(((float)(speeds.wheels[REAR_RIGHT]) - (float)(speeds.wheels[FRONT_RIGHT]))/(float)(speeds.wheels[FRONT_RIGHT]));
            slip_teste[R_MOTOR]=slip[R_MOTOR];
        }
        if (speeds.wheels[FRONT_LEFT] > speeds.wheels[REAR_LEFT]){
            slip[L_MOTOR] = 0;
            slip_teste[L_MOTOR] = 0;
        } else {
            slip[L_MOTOR] = (float)(((float)(speeds.wheels[REAR_LEFT]) - (float)(speeds.wheels[FRONT_LEFT]))/(float)(speeds.wheels[FRONT_LEFT]));
            slip_teste[L_MOTOR]=slip[L_MOTOR];
        }
    }

    //This "if" is here to deactivate the controller when the slip is lower than the ideal or when the car is making a turn
    for (int i = 0; i < 2; i++){
        if (slip[i] <= IDEAL_SLIP_DRY || internal_wheel != CENTRO) {
            PID_reset(&pid_longitudinal[i]);
            pid_result[i] = 0;
        }
        else {
            pid_result[i] = fabs((double)(PID_compute(&(pid_longitudinal[i]), slip[i])));
        }
    }

    //pid_result: delta torque 0 - mode max torque [N.m]
    //ref_torque: 0 to mode.torq_max [%]
    for (int i = 0; i < 2; i++){
        ref_torque[i] = (int)((fabs(pid_result[i])/NOMINAL_TORQUE)*(1000.0));
    }

    //Store the right and left motor torque reference that must be subtracted
    ref_torque_result.torque_decrease[R_MOTOR] = ref_torque[R_MOTOR];
    ref_torque_result.torque_decrease[L_MOTOR] = ref_torque[L_MOTOR];

    return ref_torque_result;
}