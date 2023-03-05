/*
 * sensor_data_processing.c
 *
 *  Created on: 1 de mar de 2023
 *      Author: caius
 */

#include "sensors/sensor_data_processing.h"



// moving average genérico

void moving_average(uint16_t* movave, uint16_t data) {

	uint8_t buffer_index = 0;
	uint16_t movave_buffer[BUFFER_SIZE];
	uint32_t buffer_sum = 0;

	movave = 0;

	// circular buffer for calculating the moving average of APPS signals

	for(;;){
		// Put the APPS value read by ADC into each buffer position
		movave_buffer[buffer_index] = data;

		// circular buffer logic
		if(buffer_index == 256)
			buffer_index = 0;
		buffer_index = (buffer_index + 1) % BUFFER_SIZE;


		// Calculates the moving average based on a mutable value
		// It requires empirical tests for choosing that value
		for(uint8_t i = 0; i < BUFFER_SIZE; i++){
			buffer_sum += movave_buffer[i];
		}
		movave = buffer_sum / BUFFER_SIZE;

		// avoiding variable type overflow
		buffer_sum = 0;
	}

}

