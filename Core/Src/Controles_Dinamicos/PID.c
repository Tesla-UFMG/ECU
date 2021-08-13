/*
 * PID.c
 *
 *  Created on: Apr 14, 2020
 *      Author: renanmoreira
 */

#include "PID.h"


void PID_recalculate_constants(PID_t* pid) {
	pid->C0 = -pid->Kp;
	pid->C1 = pid->Kp + pid->Kp*pid->sample_period/pid->Ti;
	pid->C2 = pid->Kp*pid->Td/pid->sample_period;
}

void PID_init(PID_t* pid, uint8_t reset, double Kp, double Ti, double Td, double max_output, double min_output, double sample_period) {
	pid->Kp = Kp;
	pid->Ti = Ti;
	pid->Td = Td;
	pid->max_output = max_output;
	pid->min_output = min_output;
	pid->sample_period = sample_period;
	PID_recalculate_constants(pid);

	pid->output = 0;
	/* Verifica se precisa de reset */
	if (reset)
	{
		/* Reseta os estados, sempre de tamanho 2 */
		memset(pid->error_state, 0, 2U * sizeof(double));
		memset(pid->input_state, 0, 2U * sizeof(double));
	}

}

void PID_set_setpoint(PID_t* pid, double Setpoint) {
	pid->Setpoint = Setpoint;
}

void PID_set_parameters(PID_t* pid, double Kp, double Ti, double Td) {
	pid->Kp = Kp;
	pid->Ti = Ti;
	pid->Td = Td;
	PID_recalculate_constants(pid);
}

void PID_set_limits(PID_t* pid, double max_output, double min_output) {
	pid->max_output = max_output;
	pid->min_output = min_output;
}

void PID_set_sample_period(PID_t* pid, double sample_period) {
	pid->sample_period = sample_period;
	PID_recalculate_constants(pid);
}
