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

//retorna uma variável do tamanho de value, porém apenas com o bit mais significativo setado em value
uint32_t get_flag_MSB(uint32_t value) {
    uint32_t flag = 1 << 31; // flag = 2^32
    while (flag != 0) {
        if (value & flag)
            return flag;
        flag >>= 1;
    }
    return 0;
}

bool get_individual_flag(osEventFlagsId_t ef_id, uint32_t flag){
    return (osEventFlagsGet(ef_id) & flag);
}

bool get_RTD_status(){
    return get_individual_flag(ECU_control_event_id, RTD_FLAG);
}



