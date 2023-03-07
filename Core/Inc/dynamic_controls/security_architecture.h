/*
 * security_architecture.h
 *
 *  Created on: 5 de mar de 2023
 *      Author: caius
 */

#ifndef INC_DYNAMIC_CONTROLS_SECURITY_ARCHITECTURE_H_
#define INC_DYNAMIC_CONTROLS_SECURITY_ARCHITECTURE_H_

// TODO(caius): add the threshold values and the timer time
#define IMU_LONG_ACCEL_THRESHOLD 1
#define SPEED_MIN_THRESHOLD 1
#define IMU_MAX_LONG_ACCEL_THRESHOLD 1

#define CROSS_VALIDATION_ERROR_TIME 1

uint8_t is_imu_bse_ok();
uint8_t is_imu_speed_ok();

#endif /* INC_DYNAMIC_CONTROLS_SECURITY_ARCHITECTURE_H_ */
