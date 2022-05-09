/*
 * odometer_save.c
 *
 *  Created on: May 8, 2022
 *      Author: dmroh
 */
#include "datalogging/odometer_save.h"
#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "main.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/flash_sector_h7.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

void odometer_save() {

    ODOMETER_TOTAL_t total_distance_traveled_g = 0;
    uint32_t save_counter                      = 0;
    uint32_t flash_read_data[2]                = {0, 0};
    uint32_t flash_distance[8]                 = {0, 0, 0, 0, 0, 0, 0, 0};

    for (;;) {

        // Wait the flag to be set so the task runs. The flag is set when RTD is exited
        osThreadFlagsWait(ODOMETER_SAVE_FLAG, osFlagsWaitAny, osWaitForever);

        // Reads Odometer global variable
        total_distance_traveled_g = get_global_var_value(ODOMETER_TOTAL);

        // Read distance and number of saves in flash
        Flash_Read_Data(FLASH_ADDR, flash_read_data, TWO_WORDS);

        // If distance traveled is greater than 10 meters
        if ((total_distance_traveled_g - flash_read_data[TOTAL_DISTANCE]) >= METERS_100) {

            // Edit flash distance array to save in the memory
            flash_distance[TOTAL_DISTANCE] = total_distance_traveled_g;
            flash_distance[FLASH_WEAR]     = (++flash_read_data[FLASH_WEAR]);

            // In order to avoid saving multiple times and wearing the embedded flash,
            // save up to 20 times
            if (save_counter < MAX_SAVE_TIMES) {

                // Function call to save data
                Flash_Write_Data(FLASH_ADDR, flash_distance, FLASHWORD);

                // Increment of counter
                save_counter++;
            } else {

                // Set a flag to warn if the saving limit was overcome
                osEventFlagsSet(ECU_control_event_id, FLASH_SAVE_LIMIT_FLAG);
            }
        }
        osDelay(SAVE_DELAY);
    }
}
