/*
 * lateral_control.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "DynamicControls/lateral_control.h"
#include "DynamicControls/PID.h"
#include "DynamicControls/constants_control.h"
#include "cmsis_os.h"
#include "math.h"
#include "speed_calc.h"
#include "global_variables.h"
#include "constants.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"

static PID_t pid_lateral;

void init_lateral_control(){
    PID_init(&pid_lateral, 1, KP_LATERAL, TI_LATERAL, 0, 4000, -4000, LATERAL_DELAY);
}

lateral_result_t lateral_control() {
    WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
    STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
    INTERNAL_WHEEL_t internal_wheel = get_global_var_value(INTERNAL_WHEEL);
    GYRO_YAW_t gyro_yaw = get_global_var_value(GYRO_YAW); // TODO: receber GYRO_YAW em algum lugar

    float cg_speed;
    double gyro_adjusted;    // entre -1.5 e 1.5
    float steering_adjusted; // entre -0.5 e 0.5
    double desired_yaw, max_yaw, setpoint, pid_result;
    lateral_result_t ref_torque_result = { .torque_decrease = {0, 0} };
    double calc_gyro(uint16_t gyro_yaw);
    float calc_steering(uint16_t steering_wheel, uint8_t internal_wheel);

    // speed
    cg_speed = avg(wheel_speeds.speed[FRONT_RIGHT],wheel_speeds.speed[FRONT_LEFT]) / (10 * 3.6); // velocidade em m/s
    // steering
    steering_adjusted = calc_steering(steering_wheel, internal_wheel);
    // yaw rate
    gyro_adjusted = calc_gyro(gyro_yaw);
    desired_yaw = cg_speed * steering_adjusted / (WHEELBASE + KU * cg_speed * cg_speed);
    max_yaw = sign(steering_adjusted) * FRICTION_COEFFICIENT * GRAVITY / cg_speed;
    // max desired yaw (setpoint)
    setpoint = fabsf(desired_yaw) > fabsf(max_yaw) ? max_yaw : desired_yaw; // o menor valor, em modulo, sera o setpoint
    // PID
    PID_set_setpoint(&pid_lateral, setpoint);
    pid_result = PID_compute(&pid_lateral, gyro_adjusted);
    // variavel de retorno
    if (pid_result > 0)       // se o sinal for positivo, a reducao sera
    	ref_torque_result.torque_decrease[R_MOTOR] = fabsf(pid_result);    // na roda direita
    else                                   								// caso contrario, sera
    	ref_torque_result.torque_decrease[L_MOTOR] = fabsf(pid_result);    // na roda esquerda

    return ref_torque_result;
}

// TODO: verificar os calculos quando tivermos os valores reais de gyro e steering

double calc_gyro(uint16_t gyro_yaw) {
    // ajusta o valor do yaw para aquele usado no pid
    double gyro_adjusted;
    if (gyro_yaw < HALF_GYRO)                              // na primeira metade, esta virando
        gyro_adjusted = (double)gyro_yaw/ADJUST_GYRO_R;    // a direita (valor positivo)
    else                                                    // e na segunda, a esquerda (negativo)
        gyro_adjusted = - (double)gyro_yaw/ADJUST_GYRO_L;

    return gyro_adjusted;
}

float calc_steering(uint16_t steering_wheel, uint8_t internal_wheel){ // TODO: verificar valor do steering
    float steering_adjusted;
    if (internal_wheel == DIREITA)
        steering_adjusted = Y0 + ((Y1-Y0)/(X1-X0)) * ((float)steering_wheel - X0);
    else
        steering_adjusted = Y0 + ((Y1-Y0)/(X1-X0)) * (- (float)steering_wheel - X0);

    return steering_adjusted;
}
