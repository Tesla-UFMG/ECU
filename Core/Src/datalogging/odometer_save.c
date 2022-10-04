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

    odometer_message_t total_distance_traveled = 0;
    uint32_t save_counter                      = 0;
    uint32_t flash_read_data[2]                = {0, 0};
    uint32_t flash_distance[8]                 = {0, 0, 0, 0, 0, 0, 0, 0};
    log_data(ID_PANEL_DEBUG_1, (uint16_t)save_counter);

    for (;;) {
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        // Wait the flag to be set so the task runs. The flag is set when RTD state is
        // left
        osThreadFlagsWait(ODOMETER_SAVE_FLAG, osFlagsWaitAny, osWaitForever);
        osMessageQueueGet(q_odometer_calc_save_messageHandle, &total_distance_traveled,
                          NULL, osWaitForever);

        // Read distance and number of saves in flash
        Flash_Read_Data(ODOMETER_DATA_FLASH_ADDR, flash_read_data, WORDS_READ_TWO);

        // Check if the distance traveled is enough to save. Fill the data array and save
        // in flash if the maximum save times have not been passed.
        if ((total_distance_traveled - flash_read_data[TOTAL_DISTANCE])
            >= MINIMUM_SAVE_DISTANCE) {
            flash_distance[TOTAL_DISTANCE] = total_distance_traveled;
            flash_distance[FLASH_WEAR]     = (++flash_read_data[FLASH_WEAR]);

            if (save_counter < MAXIMUM_SAVE_TIMES) {
                // Write data in the flash
                Flash_Write_Data(ODOMETER_DATA_FLASH_ADDR, flash_distance,
                                 FLASH_WORD_SIZE);
                save_counter++;
                log_data(ID_PANEL_DEBUG_1, (uint16_t)save_counter);
            } else {
                // Set a flag to warn if the saving limit was overcome
                osEventFlagsSet(e_ECU_control_flagsHandle, FLASH_SAVE_LIMIT_FLAG);
            }
        }
        osDelay(SAVE_DELAY);
    }
}
