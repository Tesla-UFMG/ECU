/*
 * calc.c
 *
 *  Created on: May 02, 2022
 *      Author: Felipe Telles
 */

#include "CAN/inverter_can_data_manager.h"
#include "datalogging/datalog_handler.h"
#include "sensors/steering.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_definitions.h"
#include "util/util.h"

void log_speed(SPEEDS_t* speed);

void speed_datalog(void* argument) {
    UNUSED(argument);

    for (;;) {
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif
        SPEEDS_t speed;
        WHEEL_ENCODER_SPEEDS_t wheel_sensor_speed =
            get_global_var_value(WHEEL_ENCODER_SPEEDS);
        uint16_t motor_speed_right = get_value(speed_m_r);
        uint16_t motor_speed_left  = get_value(speed_m_r);

        speed.wheels[FRONT_RIGHT] = wheel_sensor_speed.wheel_encoder_speed[FRONT_RIGHT];
        speed.wheels[FRONT_LEFT]  = wheel_sensor_speed.wheel_encoder_speed[FRONT_LEFT];
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
        osDelay(10);
    }
}

void log_speed(SPEEDS_t* speed) {
    // todo: (Felipe) log avg speed after updated datalogging
}
