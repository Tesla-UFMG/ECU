/*
 * util.c
 *
 *  Created on: 15 de ago de 2021
 *      Author: Felipe Telles
 */

#include "util/util.h"

#include "main.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_instances.h"

speed_pin_e get_speed_pin(uint16_t pin) {
    switch (pin) {
        case S_VEL1_Pin: return FRONT_RIGHT;
        case S_VEL2_Pin: return FRONT_LEFT;
        case S_VEL3_Pin: return REAR_RIGHT;
        default: return REAR_LEFT;
    }
}

/**
 * @brief Get the flag most significant bit
 *
 * @param value flag to get MSB
 * @return uint32_t a variable with the same size as value, but only with value's most
 * significant set
 */

uint32_t get_most_significant_thread_flag() {
    uint32_t thread_flag           = osThreadFlagsGet();
    uint32_t most_significant_flag = 1 << 31;
    while (thread_flag != 0) {
        if (most_significant_flag & thread_flag) {
            return most_significant_flag;
        }
        most_significant_flag >>= 1;
    }
    return 0;
}

bool get_individual_flag(osEventFlagsId_t ef_id, uint32_t flag) {
    return (osEventFlagsGet(ef_id) & flag);
}

uint16_t concatenate_two_uint8_to_uint16(const uint8_t* data) {
    return (data[1] << 8 | data[0]);
}

bool is_the_car_stationary() {
    FRONT_AVG_SPEED_t front_avg_speed = get_global_var_value(FRONT_AVG_SPEED);

    return (front_avg_speed == 0);
}

void wait_for_rtd() {
    osEventFlagsWait(e_ECU_control_flagsHandle, RTD_FLAG, osFlagsNoClear, osWaitForever);
}
