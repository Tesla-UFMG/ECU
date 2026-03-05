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
#include "math.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_variables.h"
#include "util/util.h"

static PID_t pid_lateral;

void init_lateral_control() {
    PID_init(&pid_lateral, 1, KP_LATERAL, TI_LATERAL, 0, 4000, -4000, LATERAL_DELAY);
}

lateral_result_t lateral_control() {
    STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
    INTERNAL_WHEEL_t internal_wheel = get_global_var_value(INTERNAL_WHEEL);

    double cg_speed;
    double gyro_adjusted;    // entre -1.5 e 1.5
    double desired_yaw;
    double max_yaw;
    double setpoint;
    double pid_result;
    THROTTLE_STATUS_t is_throttle_active = get_global_var_value(THROTTLE_STATUS);
    lateral_result_t ref_torque_result = {.torque_decrease = {0, 0}};
    double calc_gyro(uint16_t gyro_yaw);

    int16_t gyro_yaw = (int16_t)general_get_value(gyroscope_y);

    // velocidade em m/s
    //cg_speed = ((double)get_global_var_value(REAR_AVG_SPEED)) / (10 * 3.6);
    cg_speed = ((double)get_global_var_value(FRONT_AVG_SPEED)) / (10 * 3.6);

    // yaw rate
    //TODO(JOÃO): Verificar com aquisição. Acredito que não será necessário
    gyro_adjusted = calc_gyro(gyro_yaw);

    //Using always absolute value
    desired_yaw   = (cg_speed * fabs(steering_wheel)) / (WHEELBASE + (KU * cg_speed * cg_speed));

    //This condition prevents division by zero.
    if (cg_speed < 1){
    	max_yaw = 0;
    } else{
    	max_yaw       = TUNABILITY_FACTOR * ((FRICTION_COEFFICIENT * GRAVITY) / cg_speed);
    }

    // max desired yaw (setpoint), o menor valor, em modulo
    setpoint = desired_yaw > max_yaw ? max_yaw : desired_yaw;

    // PID
    //TODO(JOÃO): Implementar lookup table
    PID_set_setpoint(&pid_lateral, setpoint);
    pid_result = PID_compute(&pid_lateral, gyro_adjusted);
    // variavel de retorno


    if(internal_wheel == DIREITA && cg_speed > 5 && is_throttle_active) {
        ref_torque_result.torque_decrease[R_MOTOR] = fabs(pid_result);
		ref_torque_result.torque_decrease[L_MOTOR] = 0;
    } else if(internal_wheel == ESQUERDA && cg_speed > 5 && is_throttle_active){
    	ref_torque_result.torque_decrease[R_MOTOR] = 0;
        ref_torque_result.torque_decrease[L_MOTOR] = fabs(pid_result);
    } else {
    	ref_torque_result.torque_decrease[R_MOTOR] = 0;
		ref_torque_result.torque_decrease[L_MOTOR] = 0;
    }
    return ref_torque_result;
}

// TODO(renanmoreira): verificar os calculos quando tivermos os valores reais de gyro e
// steering
//TODO(JOÃO): Verificar se é necessário

double calc_gyro(uint16_t gyro_yaw) {
    // ajusta o valor do yaw para aquele usado no pid
    double gyro_adjusted;
    // na primeira metade, virando a direita (valor positivo) e na segunda, a esquerda
    // (negativo)
    if (gyro_yaw < HALF_GYRO) {
        gyro_adjusted = (double)gyro_yaw / ADJUST_GYRO_R;
    } else {
        gyro_adjusted = -(double)gyro_yaw / ADJUST_GYRO_L;
    }

    return gyro_adjusted;
}
