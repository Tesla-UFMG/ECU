/*
 * inverter_can.c
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#include "inverter_can.h"
#include "CAN/CAN_handler.h"

//defined in main.c
extern FDCAN_HandleTypeDef hfdcan1;

FDCAN_TxHeaderTypeDef TxHeader;



void initialize_inverter_CAN() {
	initialize_CAN(&hfdcan1, CAN1_receive_callback, &TxHeader);
}



void inverter_can_transmit(uin32_t id, uint16_t* data) {
	can_transmit(&hfdcan1, &TxHeader, id, data);
}


void CAN1_receive_callback(FDCAN_HandleTypeDef* hfdcan) {
	//TODO: tratar resposta do inversor
}




