/*
 * sensor_data_processing.h
 *
 *  Created on: Mar 2, 2023
 *      Author: caius
 */

#ifndef INC_SENSORS_SENSOR_DATA_PROCESSING_H_
#define INC_SENSORS_SENSOR_DATA_PROCESSING_H_

#define BUFFER_SIZE 100

void moving_average(uint16_t* movave, uint16_t data);

#endif /* INC_SENSORS_SENSOR_DATA_PROCESSING_H_ */
