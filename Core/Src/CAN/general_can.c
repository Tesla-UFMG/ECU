/*
 * general_can.c
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#include "general_can.h"
#include "CAN_handler.h"

//defined in main.c
FDCAN_HandleTypeDef* hfdcan;

FDCAN_TxHeaderTypeDef TxHeader;



void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref) {
	hfdcan = can_ref;
	initialize_CAN(hfdcan, CAN_general_receive_callback, &TxHeader);
}



void general_can_transmit(uin32_t id, uint16_t* data) {
	can_transmit(hfdcan, &TxHeader, id, data);
}


void CAN_general_receive_callback(FDCAN_HandleTypeDef* hfdcan) {
	//TODO: tratar resposta da can geral
}



