/*
 * util.h
 *
 *  Created on: 15 de ago de 2021
 *      Author: Felipe Telles
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "global_definitions.h"
#include "cmsis_os.h"
#include "stdbool.h"

speed_pin_e get_speed_pin(uint16_t pin);
uint32_t get_flag_MSB(uint32_t value);
bool get_individual_flag(osEventFlagsId_t ef_id, uint32_t flag);

#define sign(x) (x >= 0) - (x < 0)
#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) < (y)) ? (x) : (y))

#endif /* INC_UTIL_H_ */
