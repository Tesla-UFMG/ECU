/*
 * util.h
 *
 *  Created on: 15 de ago de 2021
 *      Author: Felipe Telles
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "main.h"
#include "speed_calc.h"
#include "stdbool.h"

speed_pin_e get_speed_pin(uint16_t pin);
void update_regen_state(bool is_status_regen);

#endif /* INC_UTIL_H_ */
