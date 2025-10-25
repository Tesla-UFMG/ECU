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
#define VARIABLES_INVERTER_CAN_RX                                                        \
    ENTRY(speed_m_l, 0x100, 0)                                                           \
    ENTRY(torque_m_l, 0x100, 1)                                                          \
    ENTRY(power_m_l, 0x100, 2)                                                           \
    ENTRY(current_m_l, 0x100, 3)                                                         \
    ENTRY(speed_m_r, 0x200, 0)                                                           \
    ENTRY(torque_m_r, 0x200, 1)                                                          \
    ENTRY(power_m_r, 0x200, 2)                                                           \
    ENTRY(current_m_r, 0x200, 3)                                                         \
    ENTRY(energy_m_l, 0x101, 0)                                                          \
    ENTRY(overload_m_l, 0x101, 1)                                                        \
    ENTRY(temp_mosf1_m_l, 0x101, 2)                                                      \
    ENTRY(temp_mosf2_m_l, 0x101, 3)                                                      \
    ENTRY(energy_m_r, 0x201, 0)                                                          \
    ENTRY(overload_m_r, 0x201, 1)                                                        \
    ENTRY(temp_mosf1_m_r, 0x201, 2)                                                      \
    ENTRY(temp_mosf2_m_r, 0x201, 3)                                                      \
    ENTRY(lost_msg_m_l, 0x102, 0)                                                        \
    ENTRY(bus_off_count_m_l, 0x102, 1)                                                   \
    ENTRY(can_state_m_l, 0x102, 2)                                                       \
    ENTRY(lost_msg_m_r, 0x202, 0)                                                        \
    ENTRY(bus_off_count_m_r, 0x202, 1)                                                   \
    ENTRY(can_state_m_r, 0x202, 2)                                                       \
    ENTRY(inv_state_m_l, 0x103, 0)                                                       \
    ENTRY(failure_m_l, 0x103, 1)                                                         \
    ENTRY(alarm_m_l, 0x103, 2)                                                           \
    ENTRY(inv_state_m_r, 0x203, 0)                                                       \
    ENTRY(failure_m_r, 0x203, 1)                                                         \
    ENTRY(alarm_m_r, 0x203, 2)

typedef enum {
    INVALID_VARIABLE_INVERTER = -1,
#define ENTRY(a, b, c) a,
    VARIABLES_INVERTER_CAN_RX
#undef ENTRY
        NUM_VARIABLES_INVERTER
} can_vars_inverter_e;

void inverter_store_value(can_vars_inverter_e var_name, uint16_t inverter_value);
uint16_t inverter_get_value(can_vars_inverter_e var_name);
can_vars_inverter_e inverter_get_var_name_from_id_and_pos(uint32_t id, int pos);

#endif /* INC_INVERTER_CAN_IDS_H_ */
