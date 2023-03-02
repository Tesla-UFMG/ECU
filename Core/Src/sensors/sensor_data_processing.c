/*
 * sensor_data_processing.c
 *
 *  Created on: 1 de mar de 2023
 *      Author: caius
 */

#include "sensors/sensor_data_processing.h"

#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_variables.h"
#include "util/global_definitions.h"


static uint16_t apps1 = ADC_DMA_buffer[APPS1_E];
static uint16_t apps2 = ADC_DMA_buffer[APPS2_E];
//static STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
static uint16_t speed = (get_global_var_value(REAR_AVG_SPEED)) / (10 * 3.6);
//static int16_t IMU_yaw = (int16_t)general_get_value(gyroscope_y);
static int16_t IMU_accel = (int16_t)general_get_value(accelerometer_z);

void APPS_mov_average(uint16_t* apps1_movave, uint16_t* apps2_movave) {
//  fazer um vetor e calcular media movel
//	a partir dele. comece colocando um valor de
//	[800] e testa empiricamente. teste visualizar a curva
//	comparando os dados crus e a média móvel

	uint16_t apps1_buffer[APPS_BUFFER_SIZE];
	uint16_t apps2_buffer[APPS_BUFFER_SIZE];
	uint8_t buffer_index = 0;
	uint32_t apps1_buffer_sum = 0;
	uint32_t apps2_buffer_sum = 0;

	apps1_movave = 0;
	apps2_movave = 0;

	// circular buffer for calculating the moving average of APPS signals

	for(;;){
		// Put the APPS value read by ADC into each buffer position
		apps1_buffer[buffer_index] = apps1;
		apps2_buffer[buffer_index] = apps2;

		// circular buffer logic
		buffer_index = (buffer_index + 1) % APPS_BUFFER_SIZE;

		// Calculates the moving average based on a mutable value
		// It requires empirical tests for choosing that value
		for(uint8_t i = 0; i < APPS_BUFFER_SIZE; i++){
			apps1_buffer_sum += apps1_buffer[i];
			apps2_buffer_sum += apps2_buffer[i];
		}
		apps1_movave = apps1_buffer_sum / APPS_BUFFER_SIZE;
		apps2_movave = apps2_buffer_sum / APPS_BUFFER_SIZE;
	}

}

//void steering_wheel_mov_average(uint16_t* steering_wheel_movave) {
//
//	uint16_t steering_wheel_buffer[STEERING_WHEEL_BUFFER_SIZE];
//	uint8_t buffer_index = 0;
//	uint32_t steering_wheel_buffer_sum = 0;
//
//	steering_wheel_movave = 0;
//
//
//	for(;;){
//
//		steering_wheel_buffer[buffer_index] = steering_wheel;
//
//		buffer_index = (buffer_index + 1) % STEERING_WHEEL_BUFFER_SIZE;
//
//		for(uint8_t i = 0; i < STEERING_WHEEL_BUFFER_SIZE; i++){
//			steering_wheel_buffer_sum += steering_wheel_buffer[i];
//		}
//
//		steering_wheel_movave = steering_wheel_buffer_sum / STEERING_WHEEL_BUFFER_SIZE;
//	}
//
//}

void speed_mov_average(uint16_t* speed_movave) {

	uint16_t speed_buffer[SPEED_BUFFER_SIZE];
	uint8_t buffer_index = 0;
	uint32_t speed_buffer_sum = 0;

	speed_movave = 0;

	for(;;){

		speed_buffer[buffer_index] = speed;

		buffer_index = (buffer_index + 1) % SPEED_BUFFER_SIZE;

		for(uint8_t i = 0; i < SPEED_BUFFER_SIZE; i++){
			speed_buffer_sum += speed_buffer[i];
		}
		speed_movave = speed_buffer_sum / SPEED_BUFFER_SIZE;
	}

}

//void IMU_yaw_mov_average(uint16_t* imu_yaw_movave) {
//
//	uint16_t imu_yaw_buffer[IMU_BUFFER_SIZE];
//	uint8_t buffer_index = 0;
//	uint32_t imu_yaw_buffer_sum = 0;
//
//	imu_yaw_movave = 0;
//
//	for(;;){
//
//		imu_yaw_buffer[buffer_index] = IMU_yaw;
//
//		buffer_index = (buffer_index + 1) % IMU_BUFFER_SIZE;
//
//		for(uint8_t i = 0; i < IMU_BUFFER_SIZE; i++){
//			imu_yaw_buffer_sum += imu_yaw_buffer[i];
//		}
//		imu_yaw_movave = imu_yaw_buffer_sum / IMU_BUFFER_SIZE;
//	}
//
//}

void IMU_accel_mov_average(uint16_t* imu_accel_movave) {

	uint16_t imu_accel_buffer[IMU_BUFFER_SIZE];
	uint8_t buffer_index = 0;
	uint32_t imu_accel_buffer_sum = 0;

	imu_accel_movave = 0;

	for(;;){

		imu_accel_buffer[buffer_index] = IMU_accel;

		buffer_index = (buffer_index + 1) % IMU_BUFFER_SIZE;

		for(uint8_t i = 0; i < IMU_BUFFER_SIZE; i++){
			imu_accel_buffer_sum += imu_accel_buffer[i];
		}
		imu_accel_movave = imu_accel_buffer_sum / IMU_BUFFER_SIZE;
	}

}

