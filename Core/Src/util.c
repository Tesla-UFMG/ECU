/*
 * util.c
 *
 *  Created on: 15 de ago de 2021
 *      Author: Felipe Telles
 */

#include "util.h"

speed_pin_e get_speed_pin(uint16_t pin){
    switch(pin){
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
