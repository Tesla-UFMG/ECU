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
    ENTRY(ACEL_X, 2, 0)                                                       		 \
    ENTRY(ACEL_Y, 2, 1)                                                       		 \
    ENTRY(ACEL_Z, 2, 2)                                                       		 \
    ENTRY(GYRO_X, 1, 0)                                                           	 \
    ENTRY(GYRO_Y, 1, 1)                                                           	 \
    ENTRY(GYRO_Z, 1, 2)                                                           	 \
	ENTRY(AIR_MINUS, 8, 0)

typedef enum {
    INVALID_VARIABLE_GENERAL = -1,
#define ENTRY(a, b, c) a,
    VARIABLES_GENERAL_CAN_RX_IDS
#undef ENTRY
        NUM_VARIABLE_GENERAL
} general_can_vars_e;

void general_store_value(general_can_vars_e var_name, uint16_t general_value_can);
uint16_t general_get_value(general_can_vars_e var_name);
general_can_vars_e general_get_var_name_from_id_and_pos(uint32_t id, int pos);

#endif /* INC_GENERAL_CAN_DATA_MANAGER_H_ */
