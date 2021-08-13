/*
 * PID.h
 *
 *  Created on: Apr 14, 2020
 *      Author: renanmoreira
 */

#ifndef PID_H_
#define PID_H_

#include "stdint.h"

typedef struct {
	double Kp;
	double Ti;
	double Td;
	double C0;
	double C1;
	double C2;
	double Setpoint;
	double sample_period;
	double error_state[2];
	double input_state[2];
	double output;
	double max_output;
	double min_output;
} PID_t;

void PID_init(PID_t* pid, uint8_t reset, double Kp, double Ti, double Td, double max_output, double min_output, double sample_period);
void PID_set_setpoint(PID_t* pid, double Setpoint);
void PID_set_parameters(PID_t* pid, double Kp, double Ti, double Td);
void PID_set_limits(PID_t* pid, double max_output, double min_output);
void PID_set_sample_period(PID_t* pid, double sample_period);

__attribute__((always_inline)) static inline
double PID_compute(PID_t* pid, double input)
{
	double error = pid->Setpoint-input;
	pid->output += (pid->C1 * error) +
				   (pid->C0 * pid->error_state[0]) +
				   (pid->C2 * (2.0*pid->input_state[0] - input - pid->input_state[1]));

	//prevenir windup
	if (pid->output > pid->max_output) pid->output = pid->max_output;
	if (pid->output < pid->min_output) pid->output = pid->min_output;

	/* atualizar estados */
	pid->error_state[1] = pid->error_state[0];
	pid->error_state[0] = error;
	pid->input_state[1] = pid->input_state[0];
	pid->input_state[0] = input;

	/* retorna saída */
	return (pid->output);

}





#endif /* PID_H_ */
