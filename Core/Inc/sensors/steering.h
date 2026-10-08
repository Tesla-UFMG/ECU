/*
 * steering.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_STEERING_H_
#define INC_STEERING_H_

#define ADC_MAX_VALUE		  		4095
#define STEERING_RATIO				4.4
#define SENSOR_DEAD_ZONE      		100
#define PI							3.1415
#define WINDOW						32
#include "datalogging/datalog_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_definitions.h"
#include "util/util.h"

uint16_t moving_average(uint16_t new_sample);
#endif /* INC_STEERING_H_ */
