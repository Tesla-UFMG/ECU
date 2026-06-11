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
#include "dynamic_controls/constants_control.h"
#include "datalogging/datalog_handler.h"
#include "math.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_variables.h"
#include "util/util.h"

static PID_t pid_lateral;

void init_lateral_control() {
    PID_init(&pid_lateral, 1, P_REF[0], P_REF[0]/I_REF[0], 0, NOMINAL_TORQUE,
    		-NOMINAL_TORQUE, LATERAL_DELAY);
}

lateral_result_t lateral_control() {
    STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
    INTERNAL_WHEEL_t internal_wheel = get_global_var_value(INTERNAL_WHEEL);
    THROTTLE_STATUS_t is_throttle_active = get_global_var_value(THROTTLE_STATUS);

    double cg_speed;
    //double gyro_adjusted;    // entre -1.5 e 1.5
    double desired_yaw;
    double max_yaw;
    double setpoint;
    double kp;
    double ti;
    double pid_result;
    int ref_torque;
    lateral_result_t ref_torque_result = {.torque_decrease = {0, 0}};
    //double calc_gyro(uint16_t gyro_yaw);

    int16_t gyro_yaw = ((int16_t)fabs(general_get_value(gyroscope_y)));

    //[m/s]
    cg_speed = ((double)get_global_var_value(FRONT_AVG_SPEED)) / (10 * 3.6);

    // yaw rate
    //gyro_adjusted = calc_gyro(gyro_yaw);

    //Using always absolute value
    desired_yaw   = (cg_speed * fabs(steering_wheel)) / (WHEELBASE + (KU * cg_speed * cg_speed));
    log_data(ID_DESIRED_YAW, desired_yaw);

    //This condition prevents division by zero.
    if (cg_speed < 1){
    	max_yaw = 0;
    } else{
    	max_yaw = TUNABILITY_FACTOR * ((FRICTION_COEFFICIENT * GRAVITY) / cg_speed);
    }
    log_data(ID_MAX_YAW, max_yaw);
    //the smaller value in absolute magnitude
    setpoint = fmin(desired_yaw, max_yaw);
    log_data(ID_SET_POINT_LATERAL, setpoint);
    // PID
    PID_set_setpoint(&pid_lateral, setpoint);
    pi_lookup_table(cg_speed, &kp, &ti);
    log_data(ID_KP, kp);
    log_data(ID_TI, ti);
    PID_set_parameters(&pid_lateral, kp, ti, 0);
    pid_result = PID_compute(&pid_lateral, gyro_yaw); //Return variable

    //pid_result: delta torque 0 - 13 [N.m]
    //ref_torque: 0 to torq.max [%]
    modos mode = get_global_var_value(SELECTED_MODE);
    ref_torque = (fabs(pid_result)/NOMINAL_TORQUE) * mode.tor_max;

    if(cg_speed > 5 && is_throttle_active && internal_wheel != CENTRO){
    	if(pid_result > 0){
    		//increase yaw rate -> decrease torque on internal wheel
    		ref_torque_result.torque_decrease[R_MOTOR] =
    				(internal_wheel == DIREITA) ? ref_torque : 0;
    		ref_torque_result.torque_decrease[L_MOTOR] =
    		    	(internal_wheel == ESQUERDA) ? ref_torque : 0;
    	} else if(pid_result < 0){
    		//decrease yaw rate -> decrease torque on external wheel
    		ref_torque_result.torque_decrease[R_MOTOR] =
    		    	(internal_wheel == DIREITA) ? 0 : ref_torque;
    		ref_torque_result.torque_decrease[L_MOTOR] =
    		    	(internal_wheel == ESQUERDA) ? 0 : ref_torque;
    	}else{
    		ref_torque_result.torque_decrease[R_MOTOR] = 0;
    		ref_torque_result.torque_decrease[L_MOTOR] = 0;
    	}
    }
    else{
    	ref_torque_result.torque_decrease[R_MOTOR] = 0;
    	ref_torque_result.torque_decrease[L_MOTOR] = 0;
    }

    return ref_torque_result;
}

// TODO(renanmoreira): verificar os calculos quando tivermos os valores reais de gyro e
// steering
//TODO(JOÃO): Verificar se é necessário

//double calc_gyro(uint16_t gyro_yaw) {
    // ajusta o valor do yaw para aquele usado no pid
//    double gyro_adjusted;
    // na primeira metade, virando a direita (valor positivo) e na segunda, a esquerda
    // (negativo)
//    if (gyro_yaw < HALF_GYRO) {
//        gyro_adjusted = (double)gyro_yaw / ADJUST_GYRO_R;
//    } else {
//        gyro_adjusted = -(double)gyro_yaw / ADJUST_GYRO_L;
//    }

//    return gyro_adjusted;
//}


void pi_lookup_table(double Vx, double *Pout, double *TIout)
{
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
