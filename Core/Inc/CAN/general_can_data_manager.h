/*
 * general_can_data_manager.h
 *
 *  Created on: Jun 22, 2022
 *      Author: joaopedronunes
 */

#ifndef INC_GENERAL_CAN_DATA_MANAGER_H_
#define INC_GENERAL_CAN_DATA_MANAGER_H_

#include "CAN/CAN_IDs.h"

#define VARIABLES_GENERAL_CAN_RX_IDS                                                     \
    ENTRY(accelerometer_x, 291, 0)                                                       \
    ENTRY(accelerometer_y, 291, 1)                                                       \
    ENTRY(accelerometer_z, 291, 2)                                                       \
    ENTRY(error_imu_accelerometer, 291, 3)                                               \
    ENTRY(gyroscope_x, 292, 0)                                                           \
    ENTRY(gyroscope_y, 292, 1)                                                           \
    ENTRY(gyroscope_z, 292, 2)                                                           \
    ENTRY(error_imu_gyroscope, 292, 3)

typedef enum {
#define ENTRY(a, b, c) a,
    VARIABLES_GENERAL_CAN_RX_IDS
#undef ENTRY
        NUM_STATES_GENERAL
} general_can_vars_e;

void general_store_value(general_can_vars_e var_name, uint16_t general_value_can);
uint16_t general_get_value(general_can_vars_e var_name);
general_can_vars_e general_get_var_name_from_id_and_pos(uint32_t id, int pos);

#endif /* INC_GENERAL_CAN_DATA_MANAGER_H_ */
