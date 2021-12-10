/*
 * inverter_can.c
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#include "CAN/inverter_can.h"
#include "CAN/CAN_handler.h"
#include "debugleds.h"
#include "global_definitions.h"
#include "global_instances.h"
#include "error_treatment.h"

static FDCAN_HandleTypeDef* can_ptr;

static FDCAN_TxHeaderTypeDef TxHeader;

static uint8_t RxData[8];
static FDCAN_RxHeaderTypeDef RxHeader;
int16_t datainverter[4];
uint32_t idinverter;



//função que inicializa a can do inversor, chamada em initializer.c.
void initialize_inverter_CAN(FDCAN_HandleTypeDef* can_ref) {
    can_ptr = can_ref;
    void CAN_inverter_receive_callback(FDCAN_HandleTypeDef*, uint32_t);
    void CAN_inverter_error_callback(FDCAN_HandleTypeDef*, uint32_t);
    initialize_CAN(can_ptr, CAN_inverter_receive_callback, CAN_inverter_error_callback, &TxHeader);
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

//callback que será chamado quando ouver erro de BUSOFF da CAN
void CAN_inverter_error_callback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs){
    if(ErrorStatusITs | FDCAN_IT_BUS_OFF){
        issue_error(INVERTER_BUS_OFF_ERROR_FLAG);               // chama o erro para a main_task tratar
        CLEAR_BIT(hfdcan->Instance->CCCR, FDCAN_CCCR_INIT);     // limpa o bit de INIT da CAN, voltando a receber mensagem
    }
}

void inverter_BUS_OFF_error_callback(void *argument){
    osEventFlagsClear(ECU_control_event_id, INVERTER_BUS_OFF_ERROR_FLAG); // limpa flag de estado do erro
}

