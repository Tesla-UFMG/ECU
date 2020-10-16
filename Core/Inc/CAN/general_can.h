/*
 * general_can.h
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GENERAL_CAN_H_
#define INC_GENERAL_CAN_H_

void initialize_general_CAN();
void general_can_transmit(uin32_t id, uint16_t* data);

#endif /* INC_GENERAL_CAN_H_ */
