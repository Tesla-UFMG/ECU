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

extern volatile float g_wheel_speed[4];
extern volatile uint16_t steering_wheel;
extern volatile uint16_t gyro_yaw; // ainda nao existe
extern const uint32_t ku;
extern const uint32_t wheelbase;
extern PID_t pid_lateral;

lateral_t lateral_control() {

	uint32_t cg_speed, desired_yaw, max_yaw, setpoint;
	lateral_t ref_torque;

	cg_speed = (g_wheel_speed[FRONT_RIGHT] + g_wheel_speed[FRONT_LEFT])/2;
	// defines desired yaw (setpoint)
	desired_yaw = cg_speed * steering_wheel / (wheelbase + ku * cg_speed * cg_speed);
	max_yaw = FRICTION_COEFFICIENT * GRAVITY / cg_speed;
	if (desired_yaw > max_yaw)
		setpoint = max_yaw;
	else
		setpoint = desired_yaw;

	// PID
	PID_set_setpoint (&pid_lateral, setpoint);
	ref_torque.ref_decrease = PID_compute(&pid_lateral, gyro_yaw);

	if (ref_torque.ref_decrease > 0)      // se o sinal for positivo, a reducao sera
		ref_torque.wheel = REAR_RIGHT;    // na roda direita
	else                                   // caso contrario, sera
		ref_torque.wheel = REAR_LEFT;     // na roda esquerda

	ref_torque.ref_decrease = abs(ref_torque.ref_decrease);

	return ref_torque;
}
