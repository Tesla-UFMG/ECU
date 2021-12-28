
/*
 * inverter_can_id.c
 *
 *  Created on: Oct 13, 2021
 *      Author: joaovictorchaves
 */

#include "CAN/inverter_can_data_manager.h"

static uint32_t dataInverter[NUM_STATES];

void store_value(can_vars_e var_name, uint16_t value) { dataInverter[var_name] = value; }

uint16_t get_value(can_vars_e var_name) { return dataInverter[var_name]; }

// NOLINTNEXTLINE
can_vars_e get_var_name_from_id_and_pos(uint32_t id, int pos) {
#define ENTRY(a, b, c)                                                                             \
    if (id == (b) && pos == (c))                                                                   \
        return a;                                                                                  \
    else
    // NOLINTNEXTLINE
    VARIABLES;
#undef ENTRY
    return -1;
}
