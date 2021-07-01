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
void CAN_inverter_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs);

#endif /* INC_INVERTER_CAN_H_ */
