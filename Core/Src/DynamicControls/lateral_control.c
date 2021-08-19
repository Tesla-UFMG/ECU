/*
 * lateral_control.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "lateral_control.h"
#include "initializer_controls.h"
#include "cmsis_os.h"
#include "PID.h"

extern volatile float g_wheel_speed[4];
extern volatile uint16_t steering_wheel;
extern volatile uint16_t gyro_yaw; // ainda nao existe

uint32_t cg_speed, desired_yaw, max_yaw, setpoint;
uint32_t ref_torque_decrease;


uint32_t lateral_control() {

	cg_speed = (g_wheel_speed[FRONT_RIGHT] + g_wheel_speed[FRONT_LEFT])/2;

	//defines desired yaw (setpoint)
	desired_yaw = cg_speed * steering_wheel / (wheelbase + ku * cg_speed * cg_speed);
	max_yaw = FRICTION_COEFFICIENT * GRAVITY / cg_speed;
	if (desired_yaw > max_yaw)
		setpoint = max_yaw;
	else
		setpoint = desired_yaw;

	PID_set_setpoint (&pid_lateral, setpoint);
	ref_torque_decrease = PID_compute(&pid_lateral, gyro_yaw);

	return ref_torque_decrease;
}
