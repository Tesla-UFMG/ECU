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
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/flash_sector_h7.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

static inline uint32_t calculate_distance(uint32_t speed_avg);
static void log_distance(ODOMETER_TOTAL_t total_dist, uint32_t partial_dist);

void odometer_calc() {

    uint32_t front_speed_avg       = 0;
    uint32_t partial_dist_traveled = 0;
    ODOMETER_TOTAL_t total_dist_traveled;
    // Read distance from flash just once
    Flash_Read_Data(ODOMETER_DATA_FLASH_ADDR, &(total_dist_traveled), WORDS_READ_ONE);

    // In case the memory position has never been used/saved
    if ((total_dist_traveled == 0x00000000) || (total_dist_traveled == 0xFFFFFFFF)) {
        total_dist_traveled = 0;
    }
    set_global_var(ODOMETER_TOTAL, &total_dist_traveled);

    for (;;) {
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif
        waitForRTD();
        // Calculate and log distance traveled
        WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
        front_speed_avg =
            (uint32_t)((wheel_speeds.speed[FRONT_RIGHT] + wheel_speeds.speed[FRONT_LEFT])
                       / 2);
        partial_dist_traveled = calculate_distance(front_speed_avg);
        total_dist_traveled   = get_global_var_value(ODOMETER_TOTAL);
        total_dist_traveled += partial_dist_traveled;
        set_global_var(ODOMETER_TOTAL, &total_dist_traveled);
        log_distance(total_dist_traveled, partial_dist_traveled);

        osDelay(CALC_DELAY);
    }
}

void log_distance(ODOMETER_TOTAL_t total_dist, uint32_t partial_dist) {
    log_data(ID_DISTANCE_T_ODOM, (uint16_t)total_dist);
    log_data(ID_DISTANCE_P_ODOM, (uint16_t)partial_dist);
}

static inline uint32_t calculate_distance(uint32_t speed_avg) {
    /*
     * AVG_TIME (ms)
     * speed_avg (10*km/h)
     * 1/36000 is a correction factor to make distance in meters (m)
     */
    return (uint32_t)(((1.0 / 36000) * speed_avg) * (CALC_DELAY));
}
