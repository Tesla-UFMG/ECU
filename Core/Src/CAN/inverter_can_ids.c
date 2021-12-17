/*
 * inverter_can_id.c
 *
 *  Created on: Oct 13, 2021
 *      Author: joaovictorchaves
 */

#include "CAN/inverter_can_ids.h"

can_vars_e get_var_name_from_id_and_pos(int id, int pos)
{
    #define ENTRY(a,b,c) \
    if (id == b && pos == c) return a; else
    VARIABLES;
    #undef ENTRY
    return -1;
}
