
/*
 * inverter_can_id.c
 *
 *  Created on: Oct 13, 2021
 *      Author: joaovictorchaves
 */

#include "CAN/inverter_can_data_manager.h"

static uint32_t dataInverter[NUM_STATES_INVERTER];

void store_value_inverter(can_vars_e_inverter var_name, uint16_t value_inverter) {
    dataInverter[var_name] = value_inverter;
}

uint16_t get_value_inverter(can_vars_e_inverter var_name) {
    return dataInverter[var_name];
}

// NOLINTNEXTLINE
can_vars_e_inverter get_var_name_from_id_and_pos_inverter(uint32_t id, int pos) {
#define ENTRY(a, b, c)                                                                   \
    if (id == (b) && pos == (c))                                                         \
        return a;                                                                        \
    else
    // NOLINTNEXTLINE
    VARIABLES_INVERTER;
#undef ENTRY
    return -1;
}
