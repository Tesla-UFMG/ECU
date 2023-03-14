/*
 * security_architecture.c
 *
 *  Created on: 2 de mar de 2023
 *      Author: caius
 */

#include "dynamic_controls/security_architecture.h"

#include "CAN/general_can_data_manager.h"
#include "cmsis_os2.h"
#include "sensors/sensor_data_processing.h"
#include "util/util.h"
#include "util/global_instances.h"

static REAR_AVG_SPEED_t speed_mov_avg;
static IMU_ACCEL_t IMU_long_accel_mov_avg;
static BRAKE_STATUS_t bse_active;

void cross_validation(void* argument) {
    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        osThreadFlagsWait(DYNAMIC_CONTROLS_CHOICE_BTN_PRESSED_THREAD_FLAG, osFlagsWaitAny, osWaitForever);

        IMU_ACCEL_t IMU_long_accel_data = (int16_t)general_get_value(accelerometer_z);
        REAR_AVG_SPEED_t speed_data = get_global_var_value(REAR_AVG_SPEED);
        bse_active = get_global_var_value(BRAKE_STATUS);

        moving_average(&IMU_long_accel_mov_avg, &IMU_long_accel_data);
        moving_average(&speed_mov_avg, &speed_data);

        if(is_there_imu_bse_error() || is_there_imu_speed_error()){
        	osTimerStart(tim_cross_validation_errorHandle, CROSS_VALIDATION_ERROR_TIME);

        	if(is_there_imu_bse_error()){
        		osEventFlagsSet(e_ECU_control_flagsHandle, IMU_BSE_ERROR_THREAD_FLAG);
        	}
        	if(is_there_imu_speed_error()){
        		osEventFlagsSet(e_ECU_control_flagsHandle, IMU_SPEED_ERROR_THREAD_FLAG);
        	}
        }

        else{
        	osTimerStop(tim_cross_validation_errorHandle);
        	osEventFlagsClear(e_ECU_control_flagsHandle, CROSS_VALIDATION_ERROR_THREAD_FLAG);
        }
    }
}


bool is_there_imu_bse_error(){
	if(bse_active && (IMU_long_accel_mov_avg > IMU_NULL_ACCEL_MARGIN_ERROR))
		return true;
	else return false;
}

bool is_there_imu_speed_error(){
	if((speed_mov_avg < NULL_SPEED_MARGIN_ERROR) && (IMU_long_accel_mov_avg > IMU_NULL_ACCEL_MARGIN_ERROR))
		return true;
	else return false;
}

void cross_validation_error_callback(){
	osEventFlagsSet(e_ECU_control_flagsHandle, CROSS_VALIDATION_ERROR_THREAD_FLAG);
}
