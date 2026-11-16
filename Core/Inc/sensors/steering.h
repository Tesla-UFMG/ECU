/*
 * steering.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */
#include "datalogging/datalog_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_definitions.h"
#include "util/util.h"
#include "FreeRTOSConfig.h"

#ifndef INC_STEERING_H_
#define INC_STEERING_H_

#define ADC_MAX_VALUE		  		4095
#define STEERING_RATIO				3.8
#define SENSOR_DEAD_ZONE      		100
#define PI							3.1415
#define WINDOW						32

#define STEERING_DELAY_S	        0.010

#define STEERING_DELAY	        (STEERING_DELAY_S*configTICK_RATE_HZ)

uint16_t moving_average(uint16_t new_sample);

#endif /* INC_STEERING_H_ */
