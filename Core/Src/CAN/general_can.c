/*
 * general_can.c
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#include "CAN/general_can.h"
#include "CAN/CAN_handler.h"
#include "debugleds.h"

//defined in main.c
FDCAN_HandleTypeDef* can2;

FDCAN_TxHeaderTypeDef TxHeader;



void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref) {
	can2 = can_ref;
	void CAN_general_receive_callback(FDCAN_HandleTypeDef* hfdcan);
	initialize_CAN(can2, CAN_general_receive_callback, &TxHeader);
}



void general_can_transmit(uint32_t id, uint16_t* data) {
	can_transmit(can2, &TxHeader, id, data);
	//set_debugleds(DEBUGLED2, TOGGLE,1);
}

uint8_t RxData[8];
FDCAN_RxHeaderTypeDef RxHeader;

uint16_t datageneral[4];
uint32_t idgeneral;

void CAN_general_receive_callback(FDCAN_HandleTypeDef* hfdcan) {
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
    	/* Reception Error */
    	Error_Handler();
    }

    set_debugleds(DEBUGLED2, FASTBLINK,15);
    idgeneral = RxHeader.Identifier;

    for(int i = 0; i < 8; i += 2){
        datageneral[i/2] = (RxData[i+1] << 8) | RxData[i];
    }


    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
    	/* Notification Error */
    	Error_Handler();
    }
}



