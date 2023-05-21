/*
 * general_can_data_manager.c
 *
 *  Created on: Jun 22, 2022
 *      Author: joaopedronunes
 */

#include "CAN/general_can_data_manager.h"

#include "util/util.h"

static uint32_t general_data_can[NUM_VARIABLE_GENERAL];

void general_store_value(general_can_vars_e var_name, uint16_t general_value_can) {
    general_data_can[var_name] = general_value_can;
}

uint16_t general_get_value(general_can_vars_e var_name) {
    return general_data_can[var_name];
}

general_can_vars_e general_get_var_name_from_id_and_pos(uint32_t id, int pos) {
#define ENTRY(a, b, c)                                                                   \
    if (id == (b) && pos == (c)) {                                                       \
        return a;                                                                        \
    }
    VARIABLES_GENERAL_CAN_RX_IDS
#undef ENTRY
    return INVALID_VARIABLE_GENERAL;
}
