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

uint16_t meters = 0;
uint16_t vel;


void odometer_calc() {

    //Stores the distance traveled in micrometers (mm * 1000)
    uint64_t dist_traveled_um = 0; 
    
    uint32_t instant_dist_um = 0;

    FRONT_AVG_SPEED_t avg_speed = 0;

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        if (!is_RTD_active()) {
            dist_traveled_um = 0;
            log_data(ID_DISTANCE_P_ODOM, (uint16_t)0); 
        }

        wait_for_rtd();

        get_global_var(FRONT_AVG_SPEED, &avg_speed);

        vel = avg_speed;

        /* Calculates the distance traveled in micrometers (mm * 1000).
         * avg_speed is in (km/h * 10) and ODOM_DELAY_MS is in milliseconds.
         * Formula: (avg_speed * ODOM_DELAY_MS * 1000) / 36
         */
        instant_dist_um = ((uint32_t)avg_speed * ODOM_DELAY_MS * 1000) / 36;        
        dist_traveled_um += instant_dist_um;

        //Data logger only accepts 16 bits, so we need to convert the distance traveled to meters
        uint16_t dist_traveled_m = (uint16_t)(dist_traveled_um / 1000000);
        meters = dist_traveled_m;
        log_data(ID_DISTANCE_P_ODOM, dist_traveled_m);
        
        osDelay(ODOM_DELAY_MS);
    }
}
