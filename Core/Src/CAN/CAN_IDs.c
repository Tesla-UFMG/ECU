#include "CAN/CAN_IDs.h"

#include <stdio.h>
#include <string.h>

static CAN_var_inf CAN_ID_map[CAN_GENERAL_ID_QUAN];

static datalog_send_t datalog_send_struct[QUANT_RESERVED_ID];

static datalog_send_t aux_datalog_struct;

static uint16_t quant_of_ext_id;

static void populate_datalog_send_struct();
static void initialize_CAN_var_inf();
static int compare_function();
static void sort_struct();

static void initialize_CAN_var_inf() {
    // does the mapping of the ids in a structure that will be sorted
#define CAN_GENERAL_LIST_DATA(var_name, msg_id, msg_wrd)                                 \
    CAN_ID_map[var_name].var = var_name;                                                 \
    CAN_ID_map[var_name].id  = msg_id;                                                   \
    CAN_ID_map[var_name].pos = msg_wrd;
    VARIABLES_GENERAL_CAN_TX
#undef CAN_GENERAL_LIST_DATA
}

static int compare_function(const void* value1, const void* value2) {
    // Sort by ID
    if (((CAN_var_inf*)value1)->id < ((CAN_var_inf*)value2)->id) {
        return -1;
    }
    if (((CAN_var_inf*)value1)->id > ((CAN_var_inf*)value2)->id) {
        return +1;
    }

    // Sort by position if ID is the same
    if (((CAN_var_inf*)value1)->pos < ((CAN_var_inf*)value2)->pos) {
        return -1;
    }
    if (((CAN_var_inf*)value1)->pos > ((CAN_var_inf*)value2)->pos) {
        return +1;
    }

    // ID and pos are the same
    return 0;
}

// Mapped structure ordering function
static void sort_struct() {
    qsort(CAN_ID_map, CAN_GENERAL_ID_QUAN, sizeof(CAN_var_inf), compare_function);
}

void initialize_CAN_IDs_struct() {
    initialize_CAN_var_inf();
    sort_struct();
    // gets the amount of external id
    quant_of_ext_id = 1;
    for (uint16_t i = 0; i < (uint16_t)CAN_GENERAL_ID_QUAN - 1; i++) {
        if (CAN_ID_map[i].id != CAN_ID_map[i + 1].id) {
            quant_of_ext_id++;
        }
    }
    populate_datalog_send_struct();
}

static void populate_datalog_send_struct() {
    uint16_t j = 0;
    uint16_t i = 0;
    for (j = 0; j < quant_of_ext_id; j++) {
        for (uint16_t k = 0; k < 4; k++) {
            aux_datalog_struct.pos[k] = -1;
        }
        do {
            aux_datalog_struct.external_ID            = CAN_ID_map[i].id;
            aux_datalog_struct.pos[CAN_ID_map[i].pos] = CAN_ID_map[i].var;
            i++;
        } while (aux_datalog_struct.external_ID == CAN_ID_map[i].id);
        datalog_send_struct[j] = aux_datalog_struct;
    }
}

int16_t get_internal_id_from_pos_and_word(uint16_t pos_struct, uint16_t pos_word) {
    return datalog_send_struct[pos_struct].pos[pos_word];
}

uint16_t get_external_id_from_struct_pos(uint16_t struct_pos) {
    return datalog_send_struct[struct_pos].external_ID;
}

uint16_t get_amount_ext_id() {
    return quant_of_ext_id;
}
