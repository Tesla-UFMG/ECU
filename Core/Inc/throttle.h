/*
 * throttle.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_THROTTLE_H_
#define INC_THROTTLE_H_

#include "global_variables.h"
#include "global_definitions.h"
#include "global_instances.h"
#include "cmsis_os.h"

#define THROTTLE_DELAY 25

bool check_APPS_error (uint16_t APPS1, uint16_t APPS2, uint16_t apps1_calc);

#endif /* INC_THROTTLE_H_ */
