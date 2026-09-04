/*
 * encoder_acceleration.c
 *
 *  Created on: August 26, 2026
 *      Author: Enzo Ribeiro
 */
#include "sensors/encoder_acceleration.h"

#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "datalogging/datalog_handler.h"
#include "datalogging/speed.h"
#include "stm32h7xx.h"
#include "util/CMSIS_extra/cmsis_extra.h"
#include "util/global_instances.h"
#include "util/util.h"
#include "sensors/encoder_speed.h"

static encoder_acceleration_message_t acc;
static inline float calc_acceleration(encoder_speeds_message_t last_vel, encoder_speeds_message_t vel, speed_pin_e wheel_caught){
    if(vel.interrupt_message[wheel_caught]-last_vel.interrupt_message[wheel_caught] != 0){
        return (vel.wheels[wheel_caught]-last_vel.wheels[wheel_caught])/(vel.interrupt_message[wheel_caught]-last_vel.interrupt_message[wheel_caught]);
    }
    return -1;
}
void encoder_acceleration_calc(void* argument){
    UNUSED(argument);
    encoder_speeds_message_t last_vel = {.wheels = {0,0,0,0}, .interrupt_message = {0,0,0,0}};
    encoder_speeds_message_t vel;
    encoder_acceleration_message_t acceleration = {.wheels = {0,0,0,0}};
    for(;;){
        ECU_ENABLE_BREAKPOINT_DEBUG();
        osMessageQueueGet(q_encoder_speeds_messageHandle, &vel, NULL, osWaitForever);
        for(int i = FRONT_RIGHT; i<WHEEL_ENCODERS_AVAILABLE -1;i++){
            acceleration.wheels[i] = calc_acceleration(last_vel, vel, i);
        }
        last_vel.wheels[FRONT_RIGHT] =  vel.wheels[FRONT_RIGHT];
        last_vel.interrupt_message[FRONT_RIGHT] = vel.interrupt_message[FRONT_RIGHT];
        last_vel.wheels[FRONT_LEFT] = vel.wheels[FRONT_LEFT];
        last_vel.interrupt_message[FRONT_LEFT] = vel.interrupt_message[FRONT_LEFT];
        last_vel.wheels[REAR_RIGHT]  = vel.wheels[REAR_RIGHT];
        last_vel.interrupt_message[REAR_RIGHT]  = vel.interrupt_message[REAR_RIGHT];
        last_vel.wheels[REAR_LEFT]   = vel.wheels[REAR_LEFT];
        last_vel.interrupt_message[REAR_LEFT]   = vel.interrupt_message[REAR_LEFT];
    }
}

speed_pin_e wheelCaught(encoder_speeds_message_t vel){
    for(int i = FRONT_RIGHT; i<WHEEL_ENCODERS_AVAILABLE;i++){
        if(vel.wheels[i] != 0){
            return i;
        }
    }
    return WHEEL_ERROR;
}
