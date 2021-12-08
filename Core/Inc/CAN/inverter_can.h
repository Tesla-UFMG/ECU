/*
 * inverter_can.h
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#ifndef INC_INVERTER_CAN_H_
#define INC_INVERTER_CAN_H_

#include "stm32h7xx.h"

//Nome variável, id da mensagem, posicao na mensagem
#define VARIABLES \
    ENTRY(speed_m_l, 100, 0)  \
    ENTRY(torque_m_l, 100, 1)  \
    ENTRY(power_m_l, 100, 2)  \
    ENTRY(current_m_l, 100, 3)  \
    ENTRY(speed_m_r, 200, 0)  \
    ENTRY(torque_m_r, 200, 1)  \
    ENTRY(power_m_r, 200, 2)  \
    ENTRY(current_m_r, 200, 3)  \
    ENTRY(energy_m_l, 101, 0)  \
    ENTRY(overload_m_l, 101, 1)  \
    ENTRY(temp_mosf1_m_l, 101, 2)  \
    ENTRY(temp_mosf2_m_l, 101, 3)  \
    ENTRY(energy_m_r, 201, 0)  \
    ENTRY(overload_m_r, 201, 1)  \
    ENTRY(temp_mosf1_m_r, 201, 2)  \
    ENTRY(temp_mosf2_m_r, 201, 3)  \
    ENTRY(lost_msg_m_l, 102, 0)  \
    ENTRY(bus_off_count_m_l, 102, 1)  \
    ENTRY(can_state_m_l, 102, 2)  \
    ENTRY(lost_msg_m_r, 202, 0)  \
    ENTRY(bus_off_count_m_r, 202, 1)  \
    ENTRY(can_state_m_r, 202, 2)  \
    ENTRY(inv_state_m_l, 103, 0)  \
    ENTRY(failure_m_l, 103, 1)  \
    ENTRY(alarm_m_l, 103, 2)  \
    ENTRY(inv_state_m_r, 203, 0)  \
    ENTRY(failure_m_r, 203, 1)  \
    ENTRY(alarm_m_r, 203, 2)
    

typedef enum
{
#define ENTRY(a,b,c) a,
    VARIABLES
#undef ENTRY
    NUM_STATES
} can_vars_e;

int16_t dataInverter[NUM_STATES];

void initialize_inverter_CAN(FDCAN_HandleTypeDef* can_ref);
void inverter_can_transmit(uint32_t id, uint16_t* data);
void check_inverter_comm_error(void *argument);
void check_inverter_comm_error_callback(void *argument);

#endif /* INC_INVERTER_CAN_H_ */
