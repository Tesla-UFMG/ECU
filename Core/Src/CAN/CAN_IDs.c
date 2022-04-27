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

void initialize_CAN_IDs() {
#define CAN_LIST_DATA(var_name, msg_id, msg_wrd)                                         \
    CAN_ID_map[var_name].var = var_name;                                                 \
    CAN_ID_map[var_name].id  = msg_id;                                                   \
    CAN_ID_map[var_name].pos = msg_wrd;
    CAN_LIST
#undef CAN_LIST_DATA
}

// Algoritmo a ser utilizado
/*
estrutura ordenada
{
    id_externo, posicao, id_interno(enum)
            100,0,  0
            100,1,  1
            100,2,  2
            lacuna
            101,0,  3
            lacuna
            101,2,  4
            101,3   5
}

estrutura
{
    id_externo, vetor[id_interno(enum)]
             100, vetor= 0,1,2,-1
             101, vetor=3,-1,4,5
}
*/
uint16_t convert_id_pos_to_index(uint16_t id, uint16_t pos) {
#define CAN_LIST_DATA(var_name, msg_id, msg_wrd)                                         \
    if ((msg_id == id) && (msg_wrd == pos)) {                                            \
        return var_name;                                                                 \
    } else
    CAN_LIST;
#undef CAN_LIST_DATA
}

CAN_ID_t get_CAN_ID_from_internal(uint16_t internal) {
    return CAN_ID_map[internal];
}

uint16_t get_internal_from_id_pos(uint16_t id, uint16_t pos) {
    return CAN_ID_POS_INTERNAL_ID_MAP[convert_id_pos_to_index(id, pos)];
}
