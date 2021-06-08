/*
 * inverter_can.c
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#include "CAN/inverter_can.h"
#include "CAN/CAN_handler.h"

//defined in main.c
FDCAN_HandleTypeDef* can1;

FDCAN_TxHeaderTypeDef TxHeader;



void initialize_inverter_CAN(FDCAN_HandleTypeDef* can_ref) {
	can1 = can_ref;
	void CAN_inverter_receive_callback(FDCAN_HandleTypeDef* can1);
	initialize_CAN(can1, CAN_inverter_receive_callback, &TxHeader);
}



void inverter_can_transmit(uint32_t id, uint16_t* data) {
	can_transmit(can1, &TxHeader, id, data);
}


void CAN_inverter_receive_callback(FDCAN_HandleTypeDef* hfdcan) {
	//TODO: tratar resposta do inversor
}




