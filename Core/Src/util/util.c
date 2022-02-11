/*
 * util.c
 *
 *  Created on: 15 de ago de 2021
 *      Author: Felipe Telles
 */

#include "util.h"
#include "global_instances.h"
#include "main.h"

speed_pin_e get_speed_pin(uint16_t pin) {
    switch(pin) {
    case S_VEL1_Pin:
        return FRONT_RIGHT;
    case S_VEL2_Pin:
        return FRONT_LEFT;
    case S_VEL3_Pin:
        return REAR_RIGHT;
    default:
        return REAR_LEFT;
    }
}

//retorna uma variavel do tamanho de value, porem apenas com o bit mais significativo setado em value
uint32_t get_flag_MSB(uint32_t value) {
    uint32_t flag = 1 << 31; // flag = 2^32
    while (flag != 0) {
        if (value & flag) {
            return flag;
}
        flag >>= 1;
    }
    return 0;
}

bool get_individual_flag(osEventFlagsId_t ef_id, uint32_t flag){
    return (osEventFlagsGet(ef_id) & flag);
}

uint16_t concatenate_two_uint8_to_uint16(const uint8_t* data) {
    return (data[1] << 8 | data[0]);
}


