/*
 * general_can_handler.h
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GENERAL_CAN_HANDLER_H_
#define INC_GENERAL_CAN_HANDLER_H_

#include "CAN/CAN_IDs.h"

typedef enum {
#define ENTRY(a, b, c) a,
    VARIABLES_GENERAL_CAN_RX_IDS
#undef ENTRY
        NUM_STATES_GENERAL
} general_can_vars_e;

void general_store_value(general_can_vars_e var_name, uint16_t general_value_can);
uint16_t general_get_value(general_can_vars_e var_name);
general_can_vars_e general_get_var_name_from_id_and_pos(uint32_t id, int pos);

#endif /* INC_CAN_HANDLER_H_ */
