/*
 * odometer_calc.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "datalogging/odometer_calc.h"

#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "datalogging/odometer_save.h"
#include "main.h"
#include "util/CMSIS_extra/cmsis_extra.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/flash_sector_h7.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

static inline uint32_t calculate_distance(uint32_t speed_avg);
static void log_distance(uint32_t total_dist, uint32_t partial_dist);

void odometer_calc() {

    uint32_t partial_dist_traveled = 0;
    odometer_message_t total_dist_traveled;
    uint16_t odometer_speed_data = 0;

    // Read distance from flash just once
    Flash_Read_Data(ODOMETER_DATA_FLASH_ADDR, &(total_dist_traveled), WORDS_READ_ONE);

    // In case the memory position has never been used/saved
    if ((total_dist_traveled == 0x00000000) || (total_dist_traveled == 0xFFFFFFFF)) {
        total_dist_traveled = 0;
    }
    total_dist_traveled = m_to_cm(total_dist_traveled);
    osMessageQueuePutOverwrite(q_odometer_calc_save_messageHandle, &total_dist_traveled,
                               0);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();
        
        wait_for_rtd();
        // Calculate and log distance traveled
        const SPEEDS_t speed_var = get_global_var_value(SPEEDS);

        if (speed_var.wheels[FRONT_RIGHT] != 0 && speed_var.wheels[FRONT_LEFT] != 0) {
            get_global_var(FRONT_AVG_SPEED, &odometer_speed_data);
        } else {
            if (speed_var.wheels[FRONT_LEFT] == 0) {
                odometer_speed_data = speed_var.wheels[FRONT_RIGHT];
            } else {
                odometer_speed_data = speed_var.wheels[FRONT_LEFT];
            }
        }

        const uint16_t instant_distant_traveled = calculate_distance(odometer_speed_data);
        partial_dist_traveled += instant_distant_traveled;
        total_dist_traveled += instant_distant_traveled;
        osMessageQueuePutOverwrite(q_odometer_calc_save_messageHandle,
                                   &total_dist_traveled, 0);
        log_distance(total_dist_traveled, partial_dist_traveled);
        osDelay(CALC_DELAY);
    }
}

static void log_distance(uint32_t total_dist, uint32_t partial_dist) {
    log_data(ID_DISTANCE_T_ODOM, (uint16_t)cm_to_m(total_dist));
    log_data(ID_DISTANCE_P_ODOM, (uint16_t)cm_to_m(partial_dist));
}

static inline uint32_t calculate_distance(uint32_t speed_avg) {
    /*
     * AVG_TIME (ms)
     * speed_avg (10*km/h)
     * 1/360 is a correction factor to make distance in centimeters (cm)
     */
    return (uint32_t)(((1.0 / 360) * speed_avg) * (CALC_DELAY));
}
