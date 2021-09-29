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

extern PID_t pid_lateral;

lateral_t lateral_control(float g_wheel_speed[4], uint16_t *steering_wheel, uint8_t *internal_wheel, uint16_t *gyro_yaw) {
    float cg_speed;
    double gyro_adjusted;    // entre -1.5 e 1.5
    float steering_adjusted; // entre -0.5 e 0.5
    double desired_yaw, max_yaw, setpoint;
    lateral_t ref_torque;

    double calc_gyro(uint16_t *gyro_yaw);
    float calc_steering(uint16_t *steering_wheel, uint8_t *internal_wheel);

    // speed
    cg_speed = ((g_wheel_speed[FRONT_RIGHT] + g_wheel_speed[FRONT_LEFT])/2) / (10 * 3.6); // velocidade em m/s
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
    ref_torque.ref_decrease = PID_compute(&pid_lateral, gyro_adjusted);
    // variavel de retorno
    if (ref_torque.ref_decrease > 0)       // se o sinal for positivo, a reducao sera
        ref_torque.ref_wheel = R_MOTOR;    // na roda direita
    else                                   // caso contrario, sera
        ref_torque.ref_wheel = L_MOTOR;    // na roda esquerda

    ref_torque.ref_decrease = fabsf(ref_torque.ref_decrease);   // retorna o modulo da referencia
                                                                // de decrescimento
    return ref_torque;
}

// TODO: verificar os calculos quando tivermos os valores reais de gyro e steering

double calc_gyro(uint16_t *gyro_yaw) {
    // ajusta o valor do yaw para aquele usado no pid
    double gyro_adjusted;
    if (*gyro_yaw < HALF_GYRO)                              // na primeira metade, está virando
        gyro_adjusted = (double)*gyro_yaw/ADJUST_GYRO_R;    // à direita (valor positivo)
    else                                                    // e na segunda, à esquerda (negativo)
        gyro_adjusted = - (double)*gyro_yaw/ADJUST_GYRO_L;

    return gyro_adjusted;
}

float calc_steering(uint16_t *steering_wheel, uint8_t *internal_wheel) {
    float steering_adjusted;
    if (*internal_wheel == DIREITA)
        steering_adjusted = Y0 + ((Y1-Y0)/(X1-X0)) * ((float)*steering_wheel - X0);
    else
        steering_adjusted = Y0 + ((Y1-Y0)/(X1-X0)) * (- (float)*steering_wheel - X0);

    return steering_adjusted;
}
