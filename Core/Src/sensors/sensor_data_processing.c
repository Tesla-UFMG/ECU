/*
 * sensor_data_processing.c
 *
 *  Created on: 1 de mar de 2023
 *      Author: caius
 */

//#include "sensors/sensor_data_processing.h"

// calcular a média móvel e enviá-la para as validações cruzadas

extern uint16_t apps1_value;
extern uint16_t apps2_value;

void APPS_moving_average() {
    //tempo pra média: 2s
}

void PID_init(PID_t* pid, uint8_t reset, double Kp, double Ti, double Td,
              double max_output, double min_output, double sample_period) {
    pid->Kp            = Kp;
    pid->Ti            = Ti;
    pid->Td            = Td;
    pid->max_output    = max_output;
    pid->min_output    = min_output;
    pid->sample_period = sample_period;
    PID_recalculate_constants(pid);

    pid->output = 0;
    /* Verifica se precisa de reset */
    if (reset) {
        /* Reseta os estados, sempre de tamanho 2 */
        memset(pid->error_state, 0, 2U * sizeof(double)); // NOLINT
        memset(pid->input_state, 0, 2U * sizeof(double)); // NOLINT
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
