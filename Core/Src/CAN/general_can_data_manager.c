/*
 * general_can_data_manager.c
 *
 *  Created on: Dec 9, 2020
 *      Author: renanmoreira
 */
#include "CAN/general_can_data_manager.h"

#include "CAN/CAN_IDs.h"

static uint32_t general_data_can[NUM_STATES_GENERAL];

void general_store_value(general_can_vars_e var_name, uint16_t general_value_can) {
    general_data_can[var_name] = general_value_can;
}

uint16_t general_get_value(general_can_vars_e var_name) {
    return general_data_can[var_name];
}

// NOLINTNEXTLINE
general_can_vars_e general_get_var_name_from_id_and_pos(uint32_t id, int pos) {
#define ENTRY(a, b, c)                                                                   \
    if (id == (b) && pos == (c))                                                         \
        return a;                                                                        \
    else
    // NOLINTNEXTLINE
    VARIABLES_GENERAL_CAN_RX_IDS;
#undef ENTRY
    return -1;
}
