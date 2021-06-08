/*
 * general_can.c
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#include "CAN/general_can.h"
#include "CAN/CAN_handler.h"

//defined in main.c
FDCAN_HandleTypeDef* can2;

FDCAN_TxHeaderTypeDef TxHeader;



void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref) {
	can2 = can_ref;
	void CAN_general_receive_callback(FDCAN_HandleTypeDef* can2);
	initialize_CAN(can2, CAN_general_receive_callback, &TxHeader);
}



void general_can_transmit(uint32_t id, uint16_t* data) {
	can_transmit(can2, &TxHeader, id, data);
}


void CAN_general_receive_callback(FDCAN_HandleTypeDef* hfdcan) {
	//TODO: tratar resposta da can geral
}



