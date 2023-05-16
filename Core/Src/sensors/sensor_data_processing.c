/*
 * sensor_data_processing.c
 *
 *  Created on: 1 de mar de 2023
 *      Author: caius
 */

#include "sensors/sensor_data_processing.h"

float get_add_moving_average(moving_average_t* moving_average, float const data) {
    float buffer_sum                              = 0;
    moving_average->buffer[moving_average->index] = data;

    if (++moving_average->index == moving_average->size) {
        moving_average->index              = 0;
        moving_average->max_points_reached = true;
    }
    const uint8_t current_size =
        moving_average->max_points_reached ? moving_average->size : moving_average->index;

    for (uint8_t i = 0; i < current_size; ++i) {
        buffer_sum += moving_average->buffer[i];
    }
    return (buffer_sum / current_size);
}
