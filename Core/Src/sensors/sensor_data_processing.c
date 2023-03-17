/*
 * sensor_data_processing.c
 *
 *  Created on: 1 de mar de 2023
 *      Author: caius
 */

#include "sensors/sensor_data_processing.h"

// TODO(caius): Generalize moving average service implementing this sketch, allowing
// different types. Implement a general circular buffer as well It calculates moving
// average for data logged from any sensor using a circular buffer

static uint8_t buffer_index              = 0;
static float buffer_sum                  = 0;
static float mov_avg_buffer[BUFFER_SIZE] = {0.0};

void moving_average(float* mov_avg, float* data) {

    *mov_avg = 0;

    // Put the signal value read by sensor into each buffer position
    mov_avg_buffer[buffer_index] = *data;

    // circular buffer logic
    buffer_index = (buffer_index + 1) % BUFFER_SIZE;

    // Calculate the moving average based on a mutable value
    // It requires empirical tests for choosing that value
    for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
        buffer_sum += mov_avg_buffer[i];
    }
    *mov_avg = buffer_sum / (float)BUFFER_SIZE;

    // Avoid variable type overflow
    buffer_sum = 0;
    if (buffer_index == 256)
        buffer_index = 0;
}
