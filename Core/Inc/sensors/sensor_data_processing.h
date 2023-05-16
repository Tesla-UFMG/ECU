/*
 * sensor_data_processing.h
 *
 *  Created on: Mar 2, 2023
 *      Author: caius
 */

#ifndef INC_SENSORS_SENSOR_DATA_PROCESSING_H_
#define INC_SENSORS_SENSOR_DATA_PROCESSING_H_

#include "stdbool.h"
#include "stdint.h"

#define MAX_BUFFER_SIZE 100

typedef struct {
    float buffer[MAX_BUFFER_SIZE];
    uint8_t index;
    uint8_t size;
    bool max_points_reached;
} moving_average_t;

// void moving_average(float* mov_avg, float const* data);
float get_add_moving_average(moving_average_t* moving_average, float const data);

#endif /* INC_SENSORS_SENSOR_DATA_PROCESSING_H_ */
