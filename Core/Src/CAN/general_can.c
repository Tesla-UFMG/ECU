/*
 * general_can.c
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#include "CAN/general_can.h"

#include "CAN/CAN_handler.h"
#include "leds/debug_leds_handler.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

static FDCAN_HandleTypeDef* can_ptr;

static FDCAN_TxHeaderTypeDef TxHeader;

static uint8_t RxData[8];
static FDCAN_RxHeaderTypeDef RxHeader;
uint16_t datageneral[4];
uint32_t idgeneral;

// funcao que inicializa a can geral, chamada em initializer.c
void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref) {
    can_ptr = can_ref;
    void CAN_general_receive_callback(FDCAN_HandleTypeDef* /*hfdcan*/,
                                      uint32_t /*RxFifo0ITs*/);
    void CAN_general_error_callback(FDCAN_HandleTypeDef* /*hfdcan*/,
                                    uint32_t /*ErrorStatusITs*/);
    initialize_CAN(can_ptr, CAN_general_receive_callback, CAN_general_error_callback,
                   &TxHeader);
}

// funcao usada para transmitir alguma mensagem
void general_can_transmit(uint32_t id, uint16_t* data) {
    can_transmit(can_ptr, &TxHeader, id, data);
    osDelay(CAN_DELAY);
}

// funcao de callback, chamada quando chega qualquer mensagem, de qualquer ID
void CAN_general_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs) {
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
            /* Reception Error */
            Error_Handler();
        }

        set_debugleds(DEBUGLED3, TOGGLE, 0);

        idgeneral = RxHeader.Identifier;
        for (int i = 0; i < 4; ++i) {
            datageneral[i] = concatenate_two_uint8_to_uint16(RxData + 2 * i);
        }
        // TODO(renanmoreira): implementar logica de colocar as mensagens nas variaveis
        // certas

        if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0)
            != HAL_OK) {
            /* Notification Error */
            Error_Handler();
        }
    }
}

// callback que sera chamado quando ouver erro de BUSOFF da CAN
void CAN_general_error_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t ErrorStatusITs) {
    if (ErrorStatusITs | FDCAN_IT_BUS_OFF) {
        // seta a flag de evento para datalog
        osEventFlagsSet(e_ECU_control_flagsHandle, GENERAL_BUS_OFF_ERROR_FLAG);
        // limpa o bit de INIT da CAN, voltando a receber mensagem
        CLEAR_BIT(hfdcan->Instance->CCCR, FDCAN_CCCR_INIT);
    }
}
