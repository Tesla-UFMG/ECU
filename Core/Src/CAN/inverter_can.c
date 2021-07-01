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

uint8_t RxData[8];
FDCAN_RxHeaderTypeDef RxHeader;
int16_t datainverter[4];
uint32_t idinverter;



void initialize_inverter_CAN(FDCAN_HandleTypeDef* can_ref) {
	can1 = can_ref;
	void CAN_inverter_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs);
	initialize_CAN(can1, CAN_inverter_receive_callback, &TxHeader);
}



void inverter_can_transmit(uint32_t id, uint16_t* data) {
	can_transmit(can1, &TxHeader, id, data);
}



void CAN_inverter_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)  {
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}

		idinverter = RxHeader.Identifier;
		for(int i = 0; i < 8; i += 2){
			datainverter[i/2] = (RxData[i+1] << 8) | RxData[i];
		}
		//TODO: implementar lógica de colocar as mensagens nas variáveis certas

		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			/* Notification Error */
			Error_Handler();
		}
	}
}


