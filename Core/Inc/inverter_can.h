/*
 * inverter_can.h
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#ifndef INC_INVERTER_CAN_H_
#define INC_INVERTER_CAN_H_

#include "stdint.h"

void initialize_inverter_CAN();
void inverter_can_transmit(uin32_t id, uint16_t* data);

#endif /* INC_INVERTER_CAN_H_ */
