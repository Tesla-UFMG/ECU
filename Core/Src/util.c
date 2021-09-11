/*
 * util.c
 *
 *  Created on: 15 de ago de 2021
 *      Author: Felipe Telles
 */

#include "util.h"
#include "global_definitions.h"
#include "global_instances.h"

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

//retorna uma variável do tamanho de value, porém apenas com o bit mais significativo setado em value
uint32_t get_flag_MSB(uint32_t value) {
    int i = 0;
    while (value != 0) {
        ++i;
        value >>= 1;
    }
    return (1 << (i-1));
}



