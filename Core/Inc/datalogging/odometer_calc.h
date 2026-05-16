/*
 * odometer_calc.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_ODOMETER_CALC_H_
#define INC_ODOMETER_CALC_H_

#define CALC_DELAY 100

#define WORDS_READ_ONE 1
#include "cmsis_os.h"

static inline uint32_t calculate_distance(uint32_t speed_avg);
static void log_distance(uint32_t partial_dist);

#endif /* INC_ODOMETER_CALC_H_ */
