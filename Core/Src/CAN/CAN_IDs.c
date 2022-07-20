/*
 * CAN_IDs.c
 *
 *  Created on: Dec 9, 2020
 *      Author: renanmoreira
 */
#include "CAN/CAN_IDs.h"

static CAN_ID_t CAN_ID_map[CAN_ID_QUAN];

// TODO(renanmoreira): talvez aumentar capacidade se precisar de mais ids de debug

static CAN_ID_t CAN_ID_map_aux;

datalog_send_t datalog_send_struct[QUANT_RESERVED_ID];

void initialize_map_CAN_IDs() {
    // does the mapping of the ids in a structure that will be sorted
#define CAN_LIST_DATA(var_name, msg_id, msg_wrd)                                         \
    CAN_ID_map[var_name].var = var_name;                                                 \
    CAN_ID_map[var_name].id  = msg_id;                                                   \
    CAN_ID_map[var_name].pos = msg_wrd;
    VARIABLES_GENERAL_CAN_TX
#undef CAN_LIST_DATA
}

// Mapped structure ordering function
void sort_struct() {
    for (uint16_t i = 0; i < CAN_ID_QUAN; i++) {
        for (uint16_t j = i + 1; j < CAN_ID_QUAN; j++) {
            // Sort by ID
            if (CAN_ID_map[j].id < CAN_ID_map[i].id) {
                CAN_ID_map_aux = CAN_ID_map[i];
                CAN_ID_map[i]  = CAN_ID_map[j];
                CAN_ID_map[j]  = CAN_ID_map_aux;
            }
            // Sort by position if ID is the same
            if (CAN_ID_map[j].id == CAN_ID_map[i].id
                && CAN_ID_map[j].pos < CAN_ID_map[i].pos) {
                CAN_ID_map_aux = CAN_ID_map[i];
                CAN_ID_map[i]  = CAN_ID_map[j];
                CAN_ID_map[j]  = CAN_ID_map_aux;
            }
        }
    }
}

// function that gets the amount of external ids being used
int get_quant_id() {
    uint16_t quant = 1;
    for (uint16_t i = 0; i < CAN_ID_QUAN - 1; i++) {
        if (CAN_ID_map[i].id != CAN_ID_map[i + 1].id) {
            quant++;
        }
    }
    return quant;
}

// function that populates the datalogger sending structure
void initialize_CAN_IDs_struct() {
    initialize_map_CAN_IDs();
    sort_struct();
    // fills all spaces in the array of internal IDs with -1
    for (uint16_t i = 0; i < get_quant_id(); i++) {
        for (uint16_t j = 0; j < 4; j++) {
            datalog_send_struct[i].pos[j] = -1;
        }
    }

    // populate the struct with the external id and intern id in the array
    uint16_t i = 0;
    uint16_t j = 0;
    // puts the internal id "CAN_ID_map[j].var" in the vector using its corresponding
    // position "CAN_ID_map[j].pos"
    datalog_send_struct[i].pos[CAN_ID_map[j].pos] = CAN_ID_map[j].var;
    datalog_send_struct[i].extern_ID              = CAN_ID_map[j].id;
    for (j = 1; j < CAN_ID_QUAN; j++) {
        // do the same for the next positions
        if (CAN_ID_map[j].id != CAN_ID_map[j - 1].id) {
            i++;
            datalog_send_struct[i].extern_ID = CAN_ID_map[j].id;
        }
        datalog_send_struct[i].pos[CAN_ID_map[j].pos] = CAN_ID_map[j].var;
    }
}
