/*
 * constants.h
 *
 *  Created on: Dec 10, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CONSTANTS_H_
#define INC_CONSTANTS_H_


#define num_filter_samples   10		   // Moving average sample of the accelerator.
#define TORQUE_MAX            3000 		   // In tenth of a percent of the nominal torque.
#define vel_max_rpm           7500
#define torque_vectoring    0
#define regenerative_braking 0
#define REG_BRAKING_TORQUE    750
#define mode_acceleration       0
#define mode_enduro           0
#define deadzone_accelerator 10
#define TORQUE_GAIN           1
#define BURST_GAIN           1.1
#define INC_VELOC             50
#define RIGHT               0
#define LEFT                1
#define CENTER                2


#endif /* INC_CONSTANTS_H_ */
