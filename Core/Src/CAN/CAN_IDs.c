/*
 * CAN_IDs.c
 *
 *  Created on: Dec 9, 2020
 *      Author: renanmoreira
 */
#include "CAN/CAN_IDs.h"

CAN_ID_t CAN_ID_map[CAN_ID_QUAN];
// TODO(renanmoreira): talvez aumentar capacidade se precisar de mais ids de debug
uint16_t
    CAN_ID_POS_INTERNAL_ID_MAP[(ECU_CAN_LAST_POPULATED_ID - ECU_CAN_INITIAL_ID + 1) * 4
                               + 8];

uint16_t convert_id_pos_to_index(uint16_t id, uint16_t pos) {
    return (id - ECU_CAN_INITIAL_ID) * 4 + pos;
}

CAN_ID_t get_CAN_ID_from_internal(uint16_t internal) {
    return CAN_ID_map[internal];
}

uint16_t get_internal_from_id_pos(uint16_t id, uint16_t pos) {
    return CAN_ID_POS_INTERNAL_ID_MAP[convert_id_pos_to_index(id, pos)];
}

// PRIMEIRA POSICAO DA MENSAGEM CAN N PODE ESTAR VAZIA


#define CAN_LIST_DATA (var_name, msg_id, msg_wrd)\
    CAN_ID_map[##var_name].id = ##msg_id;  \
    CAN_ID_map[##var_name].pos = ##msg_wrd; \
    CAN_LIST
#undef CAN_LIST_DATA


