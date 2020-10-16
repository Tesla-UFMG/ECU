/*
 * general_can.c
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#include "general_can.h"
#include "CAN_handler.h"

//defined in main.c
extern FDCAN_HandleTypeDef hfdcan2;

FDCAN_TxHeaderTypeDef TxHeader;



void initialize_general_CAN() {
	initialize_CAN(&hfdcan2, CAN2_receive_callback, &TxHeader);
}



void general_can_transmit(uin32_t id, uint16_t* data) {
	can_transmit(&hfdcan2, &TxHeader, id, data);
}


void CAN2_receive_callback(FDCAN_HandleTypeDef* hfdcan) {
	//TODO: tratar resposta da can geral
}



