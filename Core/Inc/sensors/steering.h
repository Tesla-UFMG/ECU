/*
 * steering.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "stdint.h"

#ifndef INC_STEERING_H_
#define INC_STEERING_H_

#define STEERING_CALC_DELAY     50
#define DATALOG_STEERING_OFFSET 10000

static int16_t calculate_steering(uint16_t current_read);

#endif /* INC_STEERING_H_ */
