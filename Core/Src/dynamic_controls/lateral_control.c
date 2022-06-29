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

    double cg_speed;
    double gyro_adjusted;    // entre -1.5 e 1.5
    float steering_adjusted; // entre -0.5 e 0.5
    double desired_yaw;
    double max_yaw;
    double setpoint;
    double pid_result;
    lateral_result_t ref_torque_result = {.torque_decrease = {0, 0}};
    double calc_gyro(uint16_t gyro_yaw);
    float calc_steering(int16_t steering_wheel);

    int16_t gyro_yaw = (int16_t)general_get_value(gyroscope_y);

    // velocidade em m/s
    cg_speed = ((double)get_global_var_value(REAR_AVG_SPEED)) / (10 * 3.6);
    // steering
    steering_adjusted = calc_steering(steering_wheel);
    // yaw rate
    gyro_adjusted = calc_gyro(gyro_yaw);
    desired_yaw   = cg_speed * steering_adjusted / (WHEELBASE + KU * cg_speed * cg_speed);
    max_yaw       = sign(steering_adjusted) * FRICTION_COEFFICIENT * GRAVITY / cg_speed;
    // max desired yaw (setpoint), o menor valor, em modulo
    setpoint = fabs(desired_yaw) > fabs(max_yaw) ? max_yaw : desired_yaw;
    // PID
    PID_set_setpoint(&pid_lateral, setpoint);
    pid_result = PID_compute(&pid_lateral, gyro_adjusted);
    // variavel de retorno

    // se o sinal for positivo, a reducao sera na roda direita caso contrario, sera na
    // roda esquerda
    if (pid_result > 0) {
        ref_torque_result.torque_decrease[R_MOTOR] = fabs(pid_result);
    } else {
        ref_torque_result.torque_decrease[L_MOTOR] = fabs(pid_result);
    }

    return ref_torque_result;
}

// TODO(renanmoreira): verificar os calculos quando tivermos os valores reais de gyro e
// steering

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

// TODO(Luiza): verificar valor do steering
float calc_steering(int16_t steering_wheel) {
    float steering_adjusted;
    int lat_ctrl_steering_tolerance = 50;
    if (steering_wheel
            < lat_ctrl_steering_tolerance // tolerance to consider the steering centered
        && steering_wheel > -lat_ctrl_steering_tolerance) {
        steering_adjusted = 0;
    } else {
        steering_adjusted = Y0 + ((Y1 - Y0) / (X1 - X0)) * ((steering_wheel / 10) - X0);
    }

    return steering_adjusted;
}
