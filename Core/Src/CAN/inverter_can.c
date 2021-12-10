/*
 * inverter_can.c
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#include "CAN/inverter_can.h"
#include "CAN/CAN_handler.h"
#include "CAN/inverter_can_data_manager.h"
#include "debugleds.h"


static FDCAN_HandleTypeDef* can_ptr;

static FDCAN_TxHeaderTypeDef TxHeader;

static uint8_t RxData[8];
static FDCAN_RxHeaderTypeDef RxHeader;

//função que inicializa a can do inversor, chamada em initializer.c.
void initialize_inverter_CAN(FDCAN_HandleTypeDef* can_ref) {
	can_ptr = can_ref;
	void CAN_inverter_receive_callback(FDCAN_HandleTypeDef*, uint32_t);
	initialize_CAN(can_ptr, CAN_inverter_receive_callback, &TxHeader);
}

//função usada para transmitir alguma mensagem
void inverter_can_transmit(uint32_t id, uint16_t* data) {
	can_transmit(can_ptr, &TxHeader, id, data);
	osDelay(CAN_DELAY);
}

//função de callback, chamada quando chega qualquer mensagem, de qualquer ID
void CAN_inverter_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)  {
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}

		set_debugleds(DEBUGLED3,TOGGLE,0);

		idInverter = RxHeader.Identifier;
		for(int i = 0; i < 4; ++i){
			can_vars_e var_name = get_var_name_from_id_and_pos(idInverter, i);
			if (var_name != -1) {
				uint16_t data = (RxData[i*2 + 1] << 8) | RxData[i*2];
				store_value(var_name, data);
			}
		}

		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			/* Notification Error */
			Error_Handler();
		}
	}
}
