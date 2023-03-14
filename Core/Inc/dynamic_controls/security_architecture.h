/*
 * security_architecture.h
 *
 *  Created on: Mar 7, 2023
 *      Author: caius
 */

#ifndef INC_DYNAMIC_CONTROLS_SECURITY_ARCHITECTURE_H_
#define INC_DYNAMIC_CONTROLS_SECURITY_ARCHITECTURE_H_

// TODO(caius): add the threshold values and the timer time

#include "stdint.h"
#include "stdbool.h"

#define IMU_NULL_ACCEL_MARGIN_ERROR 1
#define NULL_SPEED_MARGIN_ERROR 1

#define CROSS_VALIDATION_ERROR_TIME 1

bool is_there_imu_bse_error();
bool is_there_imu_speed_error();
void cross_validation_error_callback();

#endif /* INC_DYNAMIC_CONTROLS_SECURITY_ARCHITECTURE_H_ */
