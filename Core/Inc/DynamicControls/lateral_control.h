/*
 * lateral_control.h
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_

#include "stdint.h"

//defines PID
#define KP_LATERAL 6.51864262048678
#define KI_LATERAL 0.14843404179579// TODO: check values
#define LATERAL_DELAY 30
//defines calculos
#define HALF_GYRO 2000
#define ADJUST_GYRO_R 1000
#define ADJUST_GYRO_L 2000
#define ADJUST_STEERING 2000

typedef struct {
	double ref_decrease;
	uint8_t ref_wheel;
} lateral_t;

lateral_t lateral_control(volatile float g_wheel_speed[4], volatile uint16_t *steering_wheel, volatile uint8_t *internal_wheel, volatile uint16_t *gyro_yaw);

#define sign(x) (x >= 0) - (x < 0)

#endif /* INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_ */
