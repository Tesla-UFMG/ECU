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

static moving_average_t speedFL_mov_avg;
static moving_average_t speedFR_mov_avg;
static moving_average_t speedRL_mov_avg;
static moving_average_t speedRR_mov_avg;
static moving_average_t IMU_long_accel_mov_avg;

static BRAKE_STATUS_t bse_active;

static float speedFL_mov_ave_value;
static float speedFR_mov_ave_value;
static float speedRL_mov_ave_value;
static float speedRR_mov_ave_value;
static float IMU_long_accel_mov_ave_value;

static bool imu_bse_error_status;
static bool imu_speed_error_status;

void cross_validation(void* argument) {
    UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        float IMU_long_accel_data = (float)general_get_value(accelerometer_z);

        SPEEDS_t speed_var = get_global_var_value(SPEEDS);

        float speedFL_data = (float)speed_var.wheels[FRONT_LEFT];
        float speedFR_data = (float)speed_var.wheels[FRONT_RIGHT];
        float speedRL_data = (float)speed_var.wheels[REAR_LEFT];
        float speedRR_data = (float)speed_var.wheels[REAR_RIGHT];

        bse_active = get_global_var_value(BRAKE_STATUS);

        IMU_long_accel_mov_ave_value =
            get_add_moving_average(&IMU_long_accel_mov_avg, IMU_long_accel_data);

        speedFL_mov_ave_value = get_add_moving_average(&speedFL_mov_avg, speedFL_data);
        speedFR_mov_ave_value = get_add_moving_average(&speedFR_mov_avg, speedFR_data);
        speedRL_mov_ave_value = get_add_moving_average(&speedRL_mov_avg, speedRL_data);
        speedRR_mov_ave_value = get_add_moving_average(&speedRR_mov_avg, speedRR_data);

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
    if (((speedFR_mov_ave_value < NULL_SPEED_MARGIN_ERROR)
         || (speedFL_mov_ave_value < NULL_SPEED_MARGIN_ERROR)
         || (speedRL_mov_ave_value < NULL_SPEED_MARGIN_ERROR)
         || (speedRR_mov_ave_value < NULL_SPEED_MARGIN_ERROR))
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
