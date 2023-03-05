/*
 * security_architecture.c
 *
 *  Created on: 2 de mar de 2023
 *      Author: caius
 */

//steps : cross validation functions with boolean return
//monitoring task
#include "security_architecture.h"

#include "global_variables_handler.h"
#include "sensor_data_processing.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_variables.h"
#include "util/global_definitions.h"


static uint16_t speed_filtered;
static int16_t IMU_long_accel_filtered;
static bool bse_active = get_global_var_value(BRAKE_STATUS);

static uint16_t raw_speed_data = (uint16_t)get_global_var_value(REAR_AVG_SPEED);
static uint16_t raw_IMU_long_accel_data = (int16_t)general_get_value(accelerometer_y);


void init_moving_average(){ //colocar essa e a init_cross_val dentro da initializer_controls.c
//	moving_average(&apps1_filtered /*, apps1 nao filtrado*/);
//	moving_average(&apps2_filtered /*, apps2 nao filtrado*/);
	moving_average(&IMU_long_accel_filtered, raw_IMU_long_accel_data);
	moving_average(&speed_filtered, raw_speed_data);
}

uint8_t is_imu_bse_ok(){
	if(bse_active && (IMU_long_accel_filtered > IMU_MAX_LONG_ACCEL_THRESHOLD))
		return 0;
	return 1;
}

uint8_t is_imu_speed_ok(){
	if((speed < SPEED_MIN_THRESHOLD) && (IMU_long_accel_filtered > IMU_MAX_LONG_ACCEL_THRESHOLD))
		return 0;
	return 1;
}


