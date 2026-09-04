/*
 * longitudinal_control.c
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */

/*
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
}



longitudinal_control_result_t longitudinal_control() {

    //Variables that will store the result 
    double pid_result;
    int ref_torque;
    longitudinal_control_result_t ref_torque_result = {.torque_decrease = {0, 0}};
    
    if (is_RTD_active() == false){

        ref_torque_result.torque_decrease[0] = 0;
        ref_torque_result.torque_decrease[1] = 0;
        
        return ref_torque_result;

    } 

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
    if (cg_speed < 30 ) {
        slip = 0;
    } else {
        //slip ratio calculation
        slip = ((rear_avg_speed - cg_speed)
                / cg_speed)
               * 100;
    }

    // double slip_data = (slip+1)*100;
    // log_data(ID_SLIP, slip_data);
    //This "if" is here to deactivate the controller when the slip is lower than the ideal or when the car is making a turn 

    if (slip <= IDEAL_SLIP_DRY || internal_wheel != CENTRO) {
        pid_result = 0;
        PID_reset(&pid_longitudinal);
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
    log_data(ID_TORQUE_DECREASE_L, ref_torque_result.torque_decrease[L_MOTOR]);
    log_data(ID_TORQUE_DECREASE_R, ref_torque_result.torque_decrease[R_MOTOR]);


    return ref_torque_result;
}

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
//double slip_teste[2] = {0, 0};

//Function that initializes the longitudinal_control
void init_longitudinal_control() {

    //Initializes the rear right PID
    PID_init(&pid_longitudinal[R_MOTOR], 1, KP_LONGITUDINAL,
             TI_LONGITUDINAL, 0, 0, -NOMINAL_TORQUE, LONGITUDINAL_DELAY);
    //Defines the rear right longitudinal PID's setpoint
    PID_set_setpoint(&pid_longitudinal[R_MOTOR], IDEAL_SLIP_DRY);

    //Initializes the rear left PID
    PID_init(&pid_longitudinal[L_MOTOR], 1, KP_LONGITUDINAL,
    TI_LONGITUDINAL, 0, 0, -NOMINAL_TORQUE, LONGITUDINAL_DELAY);
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

        return ref_torque_result;

    }

    modos mode = get_global_var_value(SELECTED_MODE);

    PID_set_limits(&pid_longitudinal[R_MOTOR], 0, -((mode.tor_max/1000)*NOMINAL_TORQUE));
    PID_set_limits(&pid_longitudinal[L_MOTOR], 0, -((mode.tor_max/1000)*NOMINAL_TORQUE));

    //Calculus variables
    double slip[2] = {0, 0};
    encoder_speeds_message_t speeds;
    osMessageQueueGet(q_encoder_speeds_messageHandle, &speeds, NULL, osWaitForever);

    //speeds.wheels[FRONT_RIGHT] = 10;
	//speeds.wheels[FRONT_LEFT] = 10;

    //SPEEDS_t *speed;
    //get_global_var(SPEEDS, &speed);

    INTERNAL_WHEEL_t internal_wheel = get_global_var_value(INTERNAL_WHEEL);

    // treatment made to avoid division by zero
    if (speeds.wheels[FRONT_RIGHT] < 10 || speeds.wheels[FRONT_LEFT] < 10) {
        ref_torque_result.torque_decrease[R_MOTOR] = 0;
        ref_torque_result.torque_decrease[L_MOTOR] = 0;
        for (int i = 0; i < 2; i++){
            PID_reset(&pid_longitudinal[i]);
        }
        return ref_torque_result;
    } else {
        //slip ratio calculation
        for (int i = 0; i < 2; i++){
            slip[i] = ((speeds.wheels[i+2] - speeds.wheels[i])/speeds.wheels[i])*100;
            //slip_teste[i]=slip[i];
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
        ref_torque[i] = (int)round((fabs(pid_result[i])/NOMINAL_TORQUE)*1000);
    }


    //Store the right and left motor torque reference that must be subtracted
    ref_torque_result.torque_decrease[R_MOTOR] = ref_torque[R_MOTOR];
    ref_torque_result.torque_decrease[L_MOTOR] = ref_torque[L_MOTOR];
    log_data(ID_TORQUE_DECREASE_L, ref_torque_result.torque_decrease[L_MOTOR]);
    log_data(ID_TORQUE_DECREASE_R, ref_torque_result.torque_decrease[R_MOTOR]);


    return ref_torque_result;
}

