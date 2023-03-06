/*
 * security_architecture.c
 *
 *  Created on: 2 de mar de 2023
 *      Author: caius
 */


#include "security_architecture.h"

#include "global_variables_handler.h"
#include "sensor_data_processing.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_variables.h"
#include "util/global_definitions.h"

// Center of gravity speed used in dynamic controls functions
static uint16_t raw_speed_data = (uint16_t)get_global_var_value(REAR_AVG_SPEED);

static uint16_t raw_IMU_long_accel_data = (int16_t)general_get_value(accelerometer_y);

static uint16_t speed_filtered;
static int16_t IMU_long_accel_filtered;
static bool bse_active = get_global_var_value(BRAKE_STATUS);

void cross_validation(void* argument) {
    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        moving_average(&IMU_long_accel_filtered, raw_IMU_long_accel_data);
        moving_average(&speed_filtered, raw_speed_data);

        if(!is_imu_bse_ok() || !is_imu_speed_ok())
        	osEventFlagsSet(e_ECU_control_flagsHandle, CROSS_VALIDATION_FLAG);
        else
        	osEventFlagsClear(e_ECU_control_flagsHandle, CROSS_VALIDATION_FLAG);

    }
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


