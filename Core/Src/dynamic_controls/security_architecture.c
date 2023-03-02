/*
 * cross_validations.c
 *
 *  Created on: 2 de mar de 2023
 *      Author: caius
 */

//steps : cross validation functions with boolean return
//monitoring task



static uint16_t apps1_filtered;
static uint16_t apps2_filtered;
//extern STEERING_WHEEL_t steering_wheel;
static uint16_t speed_filtered;
//extern int16_t IMU_yaw;
static int16_t IMU_accel_filtered;

void init_moving_average(){ //colocar essa e a init_cross_val dentro da initializer_controls.c
	APPS_mov_average(&apps1_filtered, &apps2_filtered);
	IMU_accel_mov_average(&IMU_accel_filtered);
	speed_mov_average(&speed_filtered);
}

uint8_t imu_apps_cross_val(){
	uint16_t apps_mean = (apps1_filtered + apps2_filtered) / 2;
//	if((apps_mean > APPS_MAX_THRESHOLD) && (IMU_accel_filtered > IMU_ACCEL_MAX_THRESHOLD))...
//		return 0;
//	return 1;
}
uint8_t imu_bse(){
//	if(bse active && imu_accel > threshold)
//		return 0;
//	return 1;
}
uint8_t imu_speed(){
//	if(speed < threshold (parado) && imu > accel (detectando acel))
//		return 0;
//	return 1;
}
