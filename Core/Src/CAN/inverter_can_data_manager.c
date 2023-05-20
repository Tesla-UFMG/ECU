
/*
 * inverter_can_id.c
 *
 *  Created on: Oct 13, 2021
 *      Author: joaovictorchaves
 */

#include "CAN/inverter_can_data_manager.h"

#include "util/util.h"

static uint32_t dataInverter[NUM_VARIABLES_INVERTER];

void inverter_store_value(can_vars_inverter_e var_name, uint16_t inverter_value) {
    dataInverter[var_name] = inverter_value;
}

uint16_t inverter_get_value(can_vars_inverter_e var_name) {
    return dataInverter[var_name];
}

can_vars_inverter_e inverter_get_var_name_from_id_and_pos(uint32_t id, int pos) {
    // Unused because compiler may think this is unused and throw a warning as it does not
    // implement X-MACRO
    UNUSED(id);
    UNUSED(pos);
#define ENTRY(a, b, c)                                                                   \
    if (id == (b) && pos == (c))                                                         \
        return a;                                                                        \
    else {                                                                               \
        VARIABLES_INVERTER_CAN_RX;                                                       \
    }
#undef ENTRY
    return INVALID_VARIABLE_INVERTER;
}
