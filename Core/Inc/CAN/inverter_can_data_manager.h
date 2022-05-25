/*
 * inverter_can_id.h
 *
 *  Created on: Oct 13, 2021
 *      Author: joaovictorchaves
 */

#ifndef INC_INVERTER_CAN_IDS_H_
#define INC_INVERTER_CAN_IDS_H_

#include "stdint.h"

// Nome variavel, id da mensagem, posicao na mensagem
#define VARIABLES_INVERTER                                                                      \
    ENTRY(speed_m_l, 100, 0)                                                             \
    ENTRY(torque_m_l, 100, 1)                                                            \
    ENTRY(power_m_l, 100, 2)                                                             \
    ENTRY(current_m_l, 100, 3)                                                           \
    ENTRY(speed_m_r, 200, 0)                                                             \
    ENTRY(torque_m_r, 200, 1)                                                            \
    ENTRY(power_m_r, 200, 2)                                                             \
    ENTRY(current_m_r, 200, 3)                                                           \
    ENTRY(energy_m_l, 101, 0)                                                            \
    ENTRY(overload_m_l, 101, 1)                                                          \
    ENTRY(temp_mosf1_m_l, 101, 2)                                                        \
    ENTRY(temp_mosf2_m_l, 101, 3)                                                        \
    ENTRY(energy_m_r, 201, 0)                                                            \
    ENTRY(overload_m_r, 201, 1)                                                          \
    ENTRY(temp_mosf1_m_r, 201, 2)                                                        \
    ENTRY(temp_mosf2_m_r, 201, 3)                                                        \
    ENTRY(lost_msg_m_l, 102, 0)                                                          \
    ENTRY(bus_off_count_m_l, 102, 1)                                                     \
    ENTRY(can_state_m_l, 102, 2)                                                         \
    ENTRY(lost_msg_m_r, 202, 0)                                                          \
    ENTRY(bus_off_count_m_r, 202, 1)                                                     \
    ENTRY(can_state_m_r, 202, 2)                                                         \
    ENTRY(inv_state_m_l, 103, 0)                                                         \
    ENTRY(failure_m_l, 103, 1)                                                           \
    ENTRY(alarm_m_l, 103, 2)                                                             \
    ENTRY(inv_state_m_r, 203, 0)                                                         \
    ENTRY(failure_m_r, 203, 1)                                                           \
    ENTRY(alarm_m_r, 203, 2)

typedef enum {
#define ENTRY(a, b, c) a,
    VARIABLES_INVERTER
#undef ENTRY
        NUM_STATES_INVERTER
} can_vars_e_inverter;

void store_value_inverter(can_vars_e_inverter var_name, uint16_t value_inverter);
uint16_t get_value_inverter(can_vars_e_inverter var_name);
can_vars_e_inverter get_var_name_from_id_and_pos_inverter(uint32_t id, int pos);

#endif /* INC_INVERTER_CAN_IDS_H_ */
