/*
 * util.c
 *
 *  Created on: 15 de ago de 2021
 *      Author: Felipe Telles
 */

#include "util.h"
#include "global_definitions.h"
#include "global_instances.h"

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

void update_regen_state(bool is_status_regen){
    if (is_status_regen)
        osEventFlagsSet(ECU_control_event_id, REGEN_WARN_FLAG);     // se frenagem ativa, seta flag de aviso
    else
        osEventFlagsClear(ECU_control_event_id, REGEN_WARN_FLAG);   // se frenagem ativa, limpa flag de aviso
}
