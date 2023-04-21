/*
 * constants.h
 *
 *  Created on: Dec 10, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CONSTANTS_H_
#define INC_CONSTANTS_H_

#define NUM_SAMPLE_FILTER    10   // Accelerator moving average sample
#define TORQUE_MAX           3000 // Value in tenths of a percentage of the nominal torque
#define VEL_MAX_RPM          9000
#define TORQUE_VECTORING     0
#define REGENERATIVE_BRAKING 0
#define REG_BRAKING_TORQUE   750
#define ACCELERATION_MODE    0
#define ENDURO_MODE          0
#define THROTTLE_DEAD_ZONE   10
#define TORQUE_GAIN          1
#define STEERING_ZERO        662.9894
#define STEERING_GAIN        0.9722419
#define ALIGNED_STEERING     1030
#define SPAN_ALIGNED         70
#define STEERING_MAX         3189
#define STEERING_MIN         700
#define GAIN_ARRANC          1.1
#define INC_VELOC            50
#define RIGHT                0
#define LEFT                 1
#define CENTER               2

#endif /* INC_CONSTANTS_H_ */
