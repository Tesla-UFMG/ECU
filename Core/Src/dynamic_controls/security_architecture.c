/*
 * security_architecture.c
 *
 *  Created on: 2 de mar de 2023
 *      Author: caius
 */


#include "dynamic_controls/security_architecture.h"

#include "sensors/sensor_data_processing.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_variables.h"
#include "util/global_definitions.h"
#include "CAN/general_can_data_manager.h"
#include "cmsis_os2.h"
#include "cmsis_os.h"
#include "stm32h7xx.h"
#include "stdbool.h"

// Center of gravity speed used in dynamic controls functions
//static uint16_t raw_speed_data = get_global_var_value(REAR_AVG_SPEED);

//static int16_t raw_IMU_long_accel_data = (int16_t)general_get_value(accelerometer_y);

static uint16_t speed_filtered;
static int16_t IMU_long_accel_filtered;
static bool bse_active = get_global_var_value(BRAKE_STATUS);

void cross_validation(void* argument) {
    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        //osThreadFlagsWait();

        int16_t raw_IMU_long_accel_data = (int16_t)general_get_value(accelerometer_y);
        uint16_t raw_speed_data = get_global_var_value(REAR_AVG_SPEED);


        // TODO(caius): fazer uma média mais genérica usando struct e um serviço pro buffer circular
        moving_average(&IMU_long_accel_filtered, raw_IMU_long_accel_data);
        moving_average(&speed_filtered, raw_speed_data);

        if(!is_imu_bse_ok() || !is_imu_speed_ok()){
        	osTimerStart(tim_cross_validation_errorHandle, CROSS_VALIDATION_ERROR_TIME);
        }
        else{
        	osTimerStop(tim_cross_validation_errorHandle);
        	osEventFlagsClear(e_ECU_control_flagsHandle, CROSS_VALIDATION_ERROR_THREAD_FLAG);
        }
    }
}


uint8_t is_imu_bse_ok(){
	if(bse_active && (IMU_long_accel_filtered > IMU_MAX_LONG_ACCEL_THRESHOLD))
		return 0;
	return 1;
}

uint8_t is_imu_speed_ok(){
	if((speed_filtered < SPEED_MIN_THRESHOLD) && (IMU_long_accel_filtered > IMU_MAX_LONG_ACCEL_THRESHOLD))
		return 0;
	return 1;
}

void cross_validation_error_callback(){
	osEventFlagsSet(e_ECU_control_flagsHandle, CROSS_VALIDATION_ERROR_THREAD_FLAG);
}


