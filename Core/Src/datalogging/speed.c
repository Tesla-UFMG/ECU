/*
 * speed.c
 *
 *  Created on: May 02, 2022
 *      Author: Felipe Telles
 */

#include "datalogging/speed.h"

#include "CAN/inverter_can_data_manager.h"
#include "datalogging/datalog_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_instances.h"
#include "util/util.h"

#define SPEED_LOG_DELAY 10

#if WHEEL_ENCODERS_AVAILABLE == 2
static encoder_speeds_message_t encoder_speeds_message = {.wheels = {0, 0}};
#else
static encoder_speeds_message_t encoder_speeds_message = {.wheels = {0, 0, 0, 0}};
#endif

/**
 * @brief Acquire speed values from wheel encoder and inverter speeds to creates global
 * variables with average front, average rear and all four wheels speeds
 *
 * @param argument
 */
void speed_datalog(void* argument) {
    UNUSED(argument);

    for (;;) {
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif
        SPEEDS_t speed;

        // Timeout to keep getting inverter speeds even if wheels encoder are stopped.
        // Note that if the task is released by a timeout the queue message pointer is not
        // overwritten
        osMessageQueueGet(q_encoder_speeds_messageHandle, &encoder_speeds_message, NULL,
                          SPEED_LOG_DELAY);

        uint16_t motor_speed_right = inverter_get_value(speed_m_r);
        uint16_t motor_speed_left  = inverter_get_value(speed_m_l);

        speed.wheels[FRONT_RIGHT] = encoder_speeds_message.wheels[FRONT_RIGHT];
        speed.wheels[FRONT_LEFT]  = encoder_speeds_message.wheels[FRONT_LEFT];
        speed.wheels[REAR_RIGHT]  = motor_speed_right;
        speed.wheels[REAR_LEFT]   = motor_speed_left;

        FRONT_AVG_SPEED_t avg_front_speed =
            avg(speed.wheels[FRONT_RIGHT], speed.wheels[FRONT_LEFT]);
        REAR_AVG_SPEED_t avg_rear_speed =
            avg(speed.wheels[REAR_RIGHT], speed.wheels[REAR_LEFT]);

        set_global_var(SPEEDS, &speed);
        set_global_var(FRONT_AVG_SPEED, &avg_front_speed);
        set_global_var(REAR_AVG_SPEED, &avg_rear_speed);

        log_data(ID_SPEED_FR, speed.wheels[FRONT_RIGHT]);
        log_data(ID_SPEED_FL, speed.wheels[FRONT_LEFT]);
        log_data(ID_SPEED_RR, speed.wheels[REAR_RIGHT]);
        log_data(ID_SPEED_RL, speed.wheels[REAR_LEFT]);
        // log_data(ID_SPEED_AVG, &avg_front_speed);
        // todo: (Felipe) log avg speed after updated datalogging task

        osDelay(SPEED_LOG_DELAY);
    }
}
