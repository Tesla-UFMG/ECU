/*
 * inverter_can.c
 *
 *  Created on: Oct 13, 2020
 *      Author: renanmoreira
 */

#include "CAN/inverter_can.h"
#include "CAN/CAN_handler.h"
#include "global_variables.h"
#include "global_instances.h"
#include "error_treatment.h"
#include "debugleds.h"

static FDCAN_HandleTypeDef* can_ptr;

static FDCAN_TxHeaderTypeDef TxHeader;

static uint8_t RxData[8];
static FDCAN_RxHeaderTypeDef RxHeader;
uint32_t idInverter;
uint32_t last_message_arrival_time = 0;
bool is_there_inverter_comm_error();

void store_value(can_vars_e var_name, int value)
{
    dataInverter[var_name] = value;
}

int get_value(can_vars_e var_name)
{
    return dataInverter[var_name];
}

can_vars_e get_var_name_from_id_and_pos(int id, int pos)
{
    #define ENTRY(a,b,c) \
    if (id == b && pos == c) return a; else
    VARIABLES;
    #undef ENTRY
    return -1;
}

//função que inicializa a can do inversor, chamada em initializer.c.
void initialize_inverter_CAN(FDCAN_HandleTypeDef* can_ref) {
	can_ptr = can_ref;
	void CAN_inverter_receive_callback(FDCAN_HandleTypeDef*, uint32_t);
	initialize_CAN(can_ptr, CAN_inverter_receive_callback, &TxHeader);
	osTimerStart(inverter_can_timerHandle, 2000 / portTICK_PERIOD_MS);
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

		last_message_arrival_time = osKernelGetTickCount();

		set_debugleds(DEBUGLED3,TOGGLE,0);

		idInverter = RxHeader.Identifier;
		for(int i = 0; i < 8; i += 2){
			can_vars_e var_name = get_var_name_from_id_and_pos(idInverter, i/2);
			if (var_name != -1) {
				store_value(var_name, ((RxData[i+1] << 8) | RxData[i]));
			}
		}
		
		check_inverter_comm_error(NULL);

		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			/* Notification Error */
			Error_Handler();
		}
	}
}

bool is_there_inverter_comm_error() {
	uint32_t current_time = osKernelGetTickCount();

	return get_value(can_state_m_l) != 2 || get_value(can_state_m_r) != 2 || current_time - last_message_arrival_time > 2000;
}

void check_inverter_comm_error(void *argument) {
	check_for_errors(is_there_inverter_comm_error, INVERTER_COMM_ERROR_FLAG);
	osTimerStart(inverter_can_timerHandle, 2000 / portTICK_PERIOD_MS);
}
