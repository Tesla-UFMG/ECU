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


speed_pin_e get_speed_pin(uint16_t pin);
uint32_t get_flag_MSB(uint32_t value);

#endif /* INC_UTIL_H_ */
