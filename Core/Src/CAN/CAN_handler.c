/*
 * CAN_handler.c
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#include "CAN/CAN_handler.h"



//função para inicializar a CAN
void initialize_CAN(FDCAN_HandleTypeDef* hfdcan, void (* CAN_receive_callback)(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs), FDCAN_TxHeaderTypeDef* TxHeader) {

	if (HAL_FDCAN_RegisterRxFifo0Callback(hfdcan, CAN_receive_callback) != HAL_OK) { //função para registrar a função de callback
		/* Callback Register Error */
		Error_Handler();
	}

	if (HAL_FDCAN_Start(hfdcan) != HAL_OK) {
		/* Start Error */
		Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
		/* Notification Error */
		Error_Handler();
	}

//	TxHeader.Identifier = 0x321;
	TxHeader->IdType = FDCAN_STANDARD_ID;
	TxHeader->TxFrameType = FDCAN_DATA_FRAME;
	TxHeader->DataLength = FDCAN_DLC_BYTES_8;
	TxHeader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader->BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader->FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader->MessageMarker = 0;
}


//função que realiza a transmissão da mensagem
void can_transmit(FDCAN_HandleTypeDef* hfdcan, FDCAN_TxHeaderTypeDef* TxHeader, uint32_t id, uint16_t* data) {
	TxHeader->Identifier = id;
	if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, TxHeader, (uint8_t*)data) != HAL_OK) {

		//deu ruim
		//TODO: tratar quando falhar envio de mensagem de can ao inversor
	}
}




