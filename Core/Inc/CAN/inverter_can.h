/*
 * inverter_can.h
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#ifndef INC_INVERTER_CAN_H_
#define INC_INVERTER_CAN_H_

#include "stm32h7xx.h"

void initialize_inverter_CAN(FDCAN_HandleTypeDef* can_ref);
void inverter_can_transmit(uint32_t id, uint16_t* data);

#define INVERTER_CAN_TRANSMIT_ERROR_TIMER 200

#endif /* INC_INVERTER_CAN_H_ */
