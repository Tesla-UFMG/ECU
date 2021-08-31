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


bool are_there_APPS_errors (uint16_t APPS1, uint16_t APPS2, uint16_t apps1_calc);
bool are_there_BSE_errors (uint16_t BSE, uint16_t aux_throttle_percent);
uint16_t calculate_apps1_calc(uint16_t aux_throttle_percent);
uint16_t calculate_apps2(uint16_t APPS2);
bool check_apps();
bool check_bse();
bool check_SU_F();


#endif /* INC_THROTTLE_H_ */
