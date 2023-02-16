/*
 * util.h
 *
 *  Created on: 15 de ago de 2021
 *      Author: Felipe Telles
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "cmsis_os.h"
#include "global_definitions.h"
#include "stdbool.h"
#include "util/CMSIS_extra/global_variables_handler.h"

speed_pin_e get_speed_pin(uint16_t pin);
uint32_t get_flag_MSB(uint32_t value);
uint32_t get_most_significant_thread_flag();
bool get_individual_flag(osEventFlagsId_t ef_id, uint32_t flag);
uint16_t concatenate_two_uint8_to_uint16(const uint8_t* data);
bool is_the_car_stationary();
bool is_RTD_active();
void wait_for_rtd();

#define sign(x)    (((x) >= 0) - ((x) < 0))
#define max(x, y)  ((x) > (y) ? (x) : (y))
#define min(x, y)  (((x) < (y)) ? (x) : (y))
#define abs(x)     ((x) < 0 ? -(x) : (x))
#define avg(x, y)  (((x) + (y)) / 2)
#define cm_to_m(x) ((x) / 100)
#define m_to_cm(x) ((x)*100)
#define UNUSED(x)  ((void)(x))

#ifdef DEBUG_ECU
#define ECU_ENABLE_BREAKPOINT_DEBUG()                                                    \
    {                                                                                    \
        extern void brkpt();                                                             \
        brkpt();                                                                         \
    }
#else
#define ECU_ENABLE_BREAKPOINT_DEBUG()
#endif

#endif /* INC_UTIL_H_ */
