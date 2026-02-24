/*
 * PID.h
 *
 *  Created on: Apr 14, 2020
 *      Author: renanmoreira
 */

#ifndef PID_H_
#define PID_H_

#include "stdint.h"

//Creating the struct that will store the most important PID related variables 
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

//Function used to initialize a PID controller
void PID_init(PID_t* pid, uint8_t reset, double Kp, double Ti, double Td,
              double max_output, double min_output, double sample_period);

//Function that will set and/or update the PID's setpoint
void PID_set_setpoint(PID_t* pid, double Setpoint);

//Function that updates the PID's parameters (Kp, Ti and Td)
void PID_set_parameters(PID_t* pid, double Kp, double Ti, double Td);

//Function that updates the PID's output minimum and maximum limits
void PID_set_limits(PID_t* pid, double max_output, double min_output);

//Function that updates the PID's sample period
//TODO (Guilherme): Não vi essa função ser chamada em nenhum arquivo além do "PID.c". Não sei se isso é por que o "sample period" é constante, mas se for, qual seria a necessidade dessa função?
void PID_set_sample_period(PID_t* pid, double sample_period);

//Function that defines the PID's output
__attribute__((always_inline)) inline double PID_compute(PID_t* pid, double input) {
    //Defining the error
    double error = pid->Setpoint - input;

    //Defining the PID's output. This code defines the difference between the last output and the current one, that is why the "+=" operator is used and why it is necessary to have input and error states.
    pid->output +=
        (pid->C1 * error) + (pid->C0 * pid->error_state[0])
        + (pid->C2 * (2.0 * pid->input_state[0] - input - pid->input_state[1]));

    //Making sure the output defined previously is not bigger or lower than the maximum and minimum limits, respectively. If so, this part defines the output as the limit it exceds.
    if (pid->output > pid->max_output) {
        pid->output = pid->max_output;
    }
    if (pid->output < pid->min_output) {
        pid->output = pid->min_output;
    }

    //Uptading the error and input states
    //TODO (Guilherme): Acho que esse error_state[1] é inútil. No cálculo da saída só se usa o último erro (error_state[0]).
    pid->error_state[1] = pid->error_state[0];
    pid->error_state[0] = error;
    pid->input_state[1] = pid->input_state[0];
    pid->input_state[0] = input;

    //returns output
    return (pid->output);
}

#endif /* PID_H_ */
