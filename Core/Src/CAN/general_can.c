/*
 * general_can.c
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#include "CAN/general_can.h"
#include "CAN/CAN_handler.h"
#include "global_definitions.h"
#include "global_instances.h"
#include "debugleds.h"

static FDCAN_HandleTypeDef* can_ptr;;

static FDCAN_TxHeaderTypeDef TxHeader;

static uint8_t RxData[8];
static FDCAN_RxHeaderTypeDef RxHeader;
int16_t datageneral[4];
uint32_t idgeneral;



//função que inicializa a can geral, chamada em initializer.c
void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref) {
    can_ptr = can_ref;
    void CAN_general_receive_callback(FDCAN_HandleTypeDef*, uint32_t);
    void CAN_general_error_callback(FDCAN_HandleTypeDef*, uint32_t);
    initialize_CAN(can_ptr, CAN_general_receive_callback, CAN_general_error_callback, &TxHeader);
}



//função usada para transmitir alguma mensagem
void general_can_transmit(uint32_t id, uint16_t* data) {
	can_transmit(can_ptr, &TxHeader, id, data);
	osDelay(CAN_DELAY);
}



//função de callback, chamada quando chega qualquer mensagem, de qualquer ID
void CAN_general_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)  {
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}

		set_debugleds(DEBUGLED3,TOGGLE,0);

		idgeneral = RxHeader.Identifier;
		for(int i = 0; i < 8; i += 2){
			datageneral[i/2] = (RxData[i+1] << 8) | RxData[i];
		}
		//TODO: implementar lógica de colocar as mensagens nas variáveis certas

		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			/* Notification Error */
			Error_Handler();
		}
	}
}



void CAN_general_error_callback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs){
    if(ErrorStatusITs |= FDCAN_IT_BUS_OFF){
        osEventFlagsSet(ECU_control_event_id, GENERAL_BUS_OFF_ERROR_FLAG);
        CLEAR_BIT(hfdcan->Instance->CCCR, FDCAN_CCCR_INIT);
    }
}
