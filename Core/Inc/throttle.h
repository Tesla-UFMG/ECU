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
#define BRAKE_ACTIVE 2000
#define SU_F_ERROR 3500


uint16_t calculate_apps1_calc(uint16_t aux_throttle_percent);
uint16_t calculate_apps2(uint16_t APPS2);
bool check_for_APPS_errors();
bool check_for_BSE_errors();
bool check_for_SU_F_errors();


#endif /* INC_THROTTLE_H_ */
