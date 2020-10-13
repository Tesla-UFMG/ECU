/*
 * inverter_can.c
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#include "stm32h7xx_hal_fdcan.h"
#include "inverter_can.h"

//defined in main.c
extern FDCAN_HandleTypeDef hfdcan1;

FDCAN_TxHeaderTypeDef TxHeader;



void initialize_inverter_CAN() {

	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
		/* Start Error */
		Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
		/* Notification Error */
		Error_Handler();
	}

	hfdcan1.RxBufferNewMessageCallback = CAN1_receive_callback;


//	TxHeader.Identifier = 0x321;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

}



void inverter_can_transmit(uin32_t id, uint16_t* data) {
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, (uint8_t*)data) != HAL_OK) {
		//deu ruim
		//TODO: tratar quando falhar envio de mensagem de can ao inversor
	}
}


void CAN1_receive_callback(FDCAN_HandleTypeDef* hfdcan) {
	//TODO: tratar resposta do inversor
}




