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
#include "global_definitions.h"
#include "global_instances.h"
#include "error_treatment.h"
#include "util.h"


static FDCAN_HandleTypeDef* can_ptr;

static FDCAN_TxHeaderTypeDef TxHeader;

static uint8_t RxData[8];
static FDCAN_RxHeaderTypeDef RxHeader;

//funcao que inicializa a can do inversor, chamada em initializer.c.
void initialize_inverter_CAN(FDCAN_HandleTypeDef* can_ref) {
    can_ptr = can_ref;
    void CAN_inverter_receive_callback(FDCAN_HandleTypeDef* /*hfdcan*/, uint32_t /*RxFifo0ITs*/);
    void CAN_inverter_error_callback(FDCAN_HandleTypeDef* /*hfdcan*/, uint32_t /*ErrorStatusITs*/);
    initialize_CAN(can_ptr, CAN_inverter_receive_callback, CAN_inverter_error_callback, &TxHeader);
}

//funcao usada para transmitir alguma mensagem
void inverter_can_transmit(uint32_t id, uint16_t* data) {
	can_transmit(can_ptr, &TxHeader, id, data);
	osDelay(CAN_DELAY);
}

//funcao de callback, chamada quando chega qualquer mensagem, de qualquer ID
void CAN_inverter_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)  {
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}

		set_debugleds(DEBUGLED3,TOGGLE,0);

		uint32_t idInverter = RxHeader.Identifier;
		for(int i = 0; i < 4; ++i){
			can_vars_e var_name = get_var_name_from_id_and_pos(idInverter, i);

			if (var_name != -1) {
				uint16_t data = concatenate_two_uint8_to_uint16(RxData + i*2);
				store_value(var_name, data);
			}

		}

		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			/* Notification Error */
			Error_Handler();
		}
	}
}

//callback que sera chamado quando ouver erro de BUSOFF da CAN
void CAN_inverter_error_callback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
    if (ErrorStatusITs | FDCAN_IT_BUS_OFF){
        issue_error(INVERTER_BUS_OFF_ERROR_FLAG, /*should_set_control_event_flag=*/false);    // chama o erro para a main_task tratar
        CLEAR_BIT(hfdcan->Instance->CCCR, FDCAN_CCCR_INIT);     // limpa o bit de INIT da CAN, voltando a receber mensagem
    }
}

void inverter_BUS_OFF_error_callback(void *argument) {
	(void) argument;
    clear_error(INVERTER_BUS_OFF_ERROR_FLAG);                   // limpa flag de estado do erro
}

