/*
 * general_can.h
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GENERAL_CAN_H_
#define INC_GENERAL_CAN_H_

#include "stm32h7xx.h"

void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref);
void general_can_transmit(uint32_t id, uint16_t* data);
void CAN_general_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs);

#endif /* INC_GENERAL_CAN_H_ */
