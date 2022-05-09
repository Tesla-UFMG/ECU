/*
 * odometer_calc.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "datalogging/odometer_calc.h"
#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "main.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/flash_sector_h7.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

// Function to calculate distance traveled
static inline uint32_t calculate_distance(uint32_t speed_avg);

// Function to send data via CAN
void log_distance(ODOMETER_TOTAL_t total_dist, uint32_t partial_dist);

// Thread
void odometer_calc()

{
    // Useful variables declaration
    uint32_t front_speed_avg       = 0;
    uint32_t partial_dist_traveled = 0;

    // Odometer variable created
    ODOMETER_TOTAL_t total_dist_traveled;

    // Read distance from flash just once
    Flash_Read_Data(FLASH_ADDR, &(total_dist_traveled), ONE_WORD);

    // In case the memory position has never been used/saved
    if ((total_dist_traveled == 0x00000000) || (total_dist_traveled == 0xFFFFFFFF)) {
        total_dist_traveled = 0;
    }

    // Store in the global variable
    set_global_var(ODOMETER_TOTAL, &total_dist_traveled);

    for (;;) {
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif
        // Wait event flag to be set so the task runs
        osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, osWaitForever);

        // Use global defined struct to read wheel speeds
        WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);

        // Calculate the front wheel speed average due to reduced change of locking up
        // compared to rear
        front_speed_avg =
            (uint32_t)((wheel_speeds.speed[FRONT_RIGHT] + wheel_speeds.speed[FRONT_LEFT])
                       / 2);

        // Calculate the partial distance traveled from the front tires average speed
        partial_dist_traveled = calculate_distance(front_speed_avg);

        // Read the global variable
        total_dist_traveled = get_global_var_value(ODOMETER_TOTAL);

        // Add the delay distance traveled to the total
        total_dist_traveled += partial_dist_traveled;

        // Write in the global variable
        set_global_var(ODOMETER_TOTAL, &total_dist_traveled);

        // Area for CAN

        // Send using CAN
        log_distance(total_dist_traveled, partial_dist_traveled);

        // Delay in thread execution. 100 ms
        osDelay(AVG_TIME);
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
    return (((1 / 36000) * speed_avg) * (AVG_TIME));
}
