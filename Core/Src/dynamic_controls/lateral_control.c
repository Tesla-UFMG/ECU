/*
 * lateral_control.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "dynamic_controls/lateral_control.h"

#include "CAN/general_can_data_manager.h"
#include "cmsis_os.h"
#include "dynamic_controls/PID.h"
#include "datalogging/datalog_handler.h"
#include "dynamic_controls/constants_control.h"
#include "math.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_variables.h"
#include "util/util.h"

static PID_t pid_lateral;
volatile int16_t yaw_RX_CAN;

void init_lateral_control() {
    PID_init(&pid_lateral, 1, P_REF[0], P_REF[0]/I_REF[0], 0, NOMINAL_TORQUE,
    		-NOMINAL_TORQUE, LATERAL_DELAY_S);
}

lateral_result_t lateral_control() {

    STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
    THROTTLE_STATUS_t is_throttle_active = get_global_var_value(THROTTLE_STATUS);
    INTERNAL_WHEEL_t internal_wheel = get_global_var_value(INTERNAL_WHEEL);
    INTERNAL_WHEEL_t external_wheel;

    if (internal_wheel == CENTRO) {
        external_wheel = CENTRO;
    } else if (internal_wheel == ESQUERDA) {
        external_wheel = DIREITA;
    } else {
        external_wheel = ESQUERDA;
    }

    double desired_yaw;
    double max_yaw;
    double setpoint;
    double kp;
    double ti;
    double pid_result;
    double yaw_rads;
    int ref_torque;
    
    //[m/s]
    double cg_speed = ((double)get_global_var_value(FRONT_AVG_SPEED)) / (10.0 * 3.6);

    lateral_result_t ref_torque_result = {.ref_torque_result = 0,
                                         .decrease_wheel = CENTRO,
                                         .distribute = DISTRIBUTE};
    
    if (!is_RTD_active() || internal_wheel == CENTRO || !is_throttle_active || cg_speed < 5.0) {
        PID_reset(&pid_lateral);
        ref_torque_result.ref_torque_result = 0;
        ref_torque_result.decrease_wheel = CENTRO;        
        return ref_torque_result;
    }

    yaw_RX_CAN = ((int16_t)(general_get_value(GYRO_Z)));

    if ((internal_wheel == DIREITA && yaw_RX_CAN < 0) || (internal_wheel == ESQUERDA && yaw_RX_CAN > 0)) {
        PID_reset(&pid_lateral);
        ref_torque_result.ref_torque_result = 0;
        ref_torque_result.decrease_wheel = CENTRO;
        return ref_torque_result;
    }

    yaw_rads = abs(yaw_RX_CAN) * LSM6DSR_TO_RADS;

    //Using always absolute value
    desired_yaw   = (cg_speed * fabs(steering_wheel)) / (WHEELBASE + (KU * cg_speed * cg_speed));
    log_data(ID_DESIRED_YAW, (uint16_t)(desired_yaw*1000));

    max_yaw = TUNABILITY_FACTOR * ((FRICTION_COEFFICIENT * GRAVITY) / cg_speed);
    

    log_data(ID_MAX_YAW, (uint16_t)(max_yaw*1000));

    //the smaller value in absolute magnitude
    setpoint = fmin(desired_yaw, max_yaw);
    log_data(ID_SET_POINT_LATERAL, (uint16_t)(setpoint*1000));

    modos mode = get_global_var_value(SELECTED_MODE);

    // PID
    PID_set_setpoint(&pid_lateral, setpoint);
    pi_lookup_table(cg_speed, &kp, &ti);
    PID_set_parameters(&pid_lateral, kp, ti, 0);

    log_data(ID_KP, (uint16_t)(kp*100));
    log_data(ID_TI, (uint16_t)(ti*100));

    PID_set_limits(&pid_lateral, ((mode.tor_max / 1000.0) * NOMINAL_TORQUE), -((mode.tor_max / 1000.0) * NOMINAL_TORQUE));

    pid_result = (double)PID_compute(&pid_lateral, yaw_rads); //Return variable


    //pid_result: delta torque 0 - 13 [N.m]
    //ref_torque: 0 to torq.max [%]
    ref_torque = (int)((fabs(pid_result)/NOMINAL_TORQUE) * mode.tor_max);

    

    if (pid_result > 0){
        //increase yaw rate -> decrease torque on internal wheel and increase torque on external wheel
        ref_torque_result.ref_torque_result = ref_torque;
        ref_torque_result.decrease_wheel = internal_wheel;
    } else if (pid_result < 0){
        //decrease yaw rate -> decrease torque on external wheel and increase torque on internal wheel
        ref_torque_result.ref_torque_result = ref_torque;
        ref_torque_result.decrease_wheel = external_wheel;
    } else{
        ref_torque_result.ref_torque_result = 0;
        ref_torque_result.decrease_wheel = CENTRO;
    }
    return ref_torque_result;
}

void pi_lookup_table(double Vx, double *Pout, double *TIout){

    int i;
    int idx = 0;
    double kp;
    double ki;

    if (Vx <= VX_REF[0]) {
        *Pout = P_REF[0];
        *TIout = P_REF[0]/I_REF[0];
        return;
    }

    if (Vx >= VX_REF[LUT_SIZE-1]) {
        *Pout = P_REF[LUT_SIZE-1];
        *TIout = P_REF[LUT_SIZE-1]/I_REF[LUT_SIZE-1];
        return;
    }

    for (i = 0; i < LUT_SIZE-1; i++) {
        if (Vx >= VX_REF[i] && Vx < VX_REF[i+1]) {
            idx = i;
            break;
        }
    }

    kp = 	P_REF[idx] +
           (P_REF[idx+1] - P_REF[idx]) *
           (Vx - VX_REF[idx]) /
           (VX_REF[idx+1] - VX_REF[idx]);

    ki =    I_REF[idx] +
           (I_REF[idx+1] - I_REF[idx]) *
           (Vx - VX_REF[idx]) /
           (VX_REF[idx+1] - VX_REF[idx]);

    *Pout = kp;
    *TIout = kp/ki;
}