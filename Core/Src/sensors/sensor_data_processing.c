/*
 * sensor_data_processing.c
 *
 *  Created on: 1 de mar de 2023
 *      Author: caius
 */

#include "sensors/sensor_data_processing.h"

// TODO(caius): It generalizes moving average service implementing this sketch, allowing
// different types. It implements a general circular buffer as well.

static uint8_t buffer_index              = 0;
static float buffer_sum                  = 0;
static float mov_avg_buffer[BUFFER_SIZE] = {0};

void moving_average(float* mov_avg, float const* data) {

    *mov_avg                     = 0;
    mov_avg_buffer[buffer_index] = *data;
    buffer_index                 = (buffer_index + 1) % BUFFER_SIZE;

    // TODO(caius): It requires empirical tests for choosing that value
    for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
        buffer_sum += mov_avg_buffer[i];
    }
    *mov_avg = buffer_sum / (float)BUFFER_SIZE;

    // Avoid variable type overflow
    buffer_sum = 0;
    if (buffer_index == 255) {
        buffer_index = 0;
    }
}
