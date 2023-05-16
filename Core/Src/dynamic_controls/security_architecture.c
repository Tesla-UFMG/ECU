/*
 * security_architecture.c
 *
 *  Created on: 2 de mar de 2023
 *      Author: caius
 */

#include "dynamic_controls/security_architecture.h"

#include "CAN/general_can_data_manager.h"
#include "datalogging/datalog_handler.h"
#include "sensors/sensor_data_processing.h"
#include "util/global_instances.h"
#include "util/util.h"

static moving_average_t speed_mov_avg;
static moving_average_t IMU_long_accel_mov_avg;
static BRAKE_STATUS_t bse_active;

static float speed_mov_ave_value; // fazer pros 4 sensores
static float IMU_long_accel_mov_ave_value;

static bool imu_bse_error_status;
static bool imu_speed_error_status;

void cross_validation(void* argument) {
    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        float IMU_long_accel_data = (float)general_get_value(accelerometer_z);
        float speed_data          = (float)get_global_var_value(REAR_AVG_SPEED);
        bse_active                = get_global_var_value(BRAKE_STATUS);

        IMU_long_accel_mov_ave_value =
            get_add_moving_average(&IMU_long_accel_mov_avg, IMU_long_accel_data);
        speed_mov_ave_value = get_add_moving_average(&speed_mov_avg, speed_data);

        imu_bse_error_status   = is_there_imu_bse_error();
        imu_speed_error_status = is_there_imu_speed_error();

        if (imu_bse_error_status || imu_speed_error_status) {
            osTimerStart(tim_cross_validation_errorHandle, CROSS_VALIDATION_ERROR_TIME);
        }

        else {
            osTimerStop(tim_cross_validation_errorHandle);
        }
        cross_validation_status_datalog();
        osDelay(CROSS_VALIDATION_DELAY);
    }
}

bool is_there_imu_bse_error() {
    if (bse_active && (IMU_long_accel_mov_ave_value > IMU_NULL_ACCEL_MARGIN_ERROR)) {
        return true;
    }
    return false;
}

bool is_there_imu_speed_error() {
    if ((speed_mov_ave_value < NULL_SPEED_MARGIN_ERROR)
        && (IMU_long_accel_mov_ave_value > IMU_NULL_ACCEL_MARGIN_ERROR)) {
        return true;
    }
    return false;
}

void cross_validation_error_callback() {
    osEventFlagsSet(e_ECU_control_flagsHandle, CROSS_VALIDATION_ERROR_WARN_FLAG);
}

void cross_validation_status_datalog() {
    log_data(ID_IMU_BSE_ERROR, imu_bse_error_status);
    log_data(ID_IMU_SPEED_ERROR, imu_speed_error_status);
}
