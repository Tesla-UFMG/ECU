/*
 * sensor_data_processing.c
 *
 *  Created on: 1 de mar de 2023
 *      Author: caius
 */

#include "sensors/sensor_data_processing.h"



// Calculate moving average for data logged from any sensor using a circular buffer

void moving_average(uint16_t* mov_ave, uint16_t data) {

	uint8_t buffer_index = 0;
	uint16_t movave_buffer[BUFFER_SIZE];
	uint32_t buffer_sum = 0;

	mov_ave = 0;

	// Put the signal value read by sensor into each buffer position
	movave_buffer[buffer_index] = data;// Put the signal value read by sensor into each buffer position
	movave_buffer[buffer_index] = data;

	// circular buffer logic
	buffer_index = (buffer_index + 1) % BUFFER_SIZE;


	// Calculate the moving average based on a mutable value
	// It requires empirical tests for choosing that value
	for(uint8_t i = 0; i < BUFFER_SIZE; i++){
		buffer_sum += movave_buffer[i];
	}
	mov_ave = buffer_sum / BUFFER_SIZE;

	// Avoid variable type overflow
	buffer_sum = 0;
	if(buffer_index == 256)
		buffer_index = 0;

	// circular buffer logic
	buffer_index = (buffer_index + 1) % BUFFER_SIZE;


	// Calculate the moving average based on a mutable value
	// It requires empirical tests for choosing that value
	for(uint8_t i = 0; i < BUFFER_SIZE; i++){
		buffer_sum += movave_buffer[i];
	}
	mov_ave = buffer_sum / BUFFER_SIZE;

	// Avoid variable type overflow
	buffer_sum = 0;
	if(buffer_index == 256)
		buffer_index = 0;


}

