/*
 * lateral_control.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "DynamicControls/lateral_control.h"
#include "DynamicControls/initializer_controls.h"
#include "DynamicControls/PID.h"
#include "cmsis_os.h"
#include "math.h"

extern volatile float g_wheel_speed[4];
extern volatile uint16_t steering_wheel;
extern volatile uint8_t internal_wheel;
extern volatile uint16_t gyro_yaw; // ainda nao existe
extern const float ku;
extern const float wheelbase;
extern PID_t pid_lateral;

lateral_t lateral_control() {

	float cg_speed;
	double gyro_adjusted;
	double desired_yaw, max_yaw, setpoint;
	lateral_t ref_torque;

	// yaw rate
	if (gyro_yaw < 2000)
		gyro_adjusted = (double)gyro_yaw / 1000;
	else
		gyro_adjusted = - (double)gyro_yaw / 2000;
	// speed
	cg_speed = ((g_wheel_speed[FRONT_RIGHT] + g_wheel_speed[FRONT_LEFT])/2) / (10 * 3.6); // m/s
	// steering
	float steering_adjusted; //entre -0.5 e 0.5
	if (internal_wheel == DIREITA) {
		steering_adjusted = (float)steering_wheel / 2000;
		max_yaw = FRICTION_COEFFICIENT * GRAVITY / cg_speed;
	} else {
		steering_adjusted = - ((float)steering_wheel - 1030) / 2000;
		max_yaw = - FRICTION_COEFFICIENT * GRAVITY / cg_speed;
	}
	// defines desired yaw (setpoint)
	desired_yaw = cg_speed * steering_adjusted / (wheelbase + ku * cg_speed * cg_speed);
	if (fabsf(desired_yaw) > fabsf(max_yaw))
		setpoint = max_yaw;
	else
		setpoint = desired_yaw;
	// PID
	PID_set_setpoint (&pid_lateral, setpoint);
	ref_torque.ref_decrease = PID_compute(&pid_lateral, gyro_adjusted);

	if (ref_torque.ref_decrease > 0)     // se o sinal for positivo, a reducao sera
		ref_torque.ref_wheel = R_MOTOR;    // na roda direita
	else                                 // caso contrario, sera
		ref_torque.ref_wheel = L_MOTOR;     // na roda esquerda

	ref_torque.ref_decrease = fabsf(ref_torque.ref_decrease);

	return ref_torque;
}
