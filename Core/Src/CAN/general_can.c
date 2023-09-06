/*
 * general_can.c
 *
 *  Created on: Oct 16, 2020
 *      Author: renanmoreira
 */

#include "CAN/general_can.h"

#include "CAN/CAN_IDs.h"
#include "CAN/CAN_handler.h"
#include "CAN/general_can_data_manager.h"
#include "leds/debug_leds_handler.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

static void CAN_general_receive_callback(FDCAN_HandleTypeDef* /*hfdcan*/,
                                         uint32_t /*RxFifo0ITs*/);
static void CAN_general_error_callback(FDCAN_HandleTypeDef* /*hfdcan*/,
                                       uint32_t /*ErrorStatusITs*/);

static FDCAN_HandleTypeDef* can_ptr;
static FDCAN_TxHeaderTypeDef TxHeader;
static FDCAN_RxHeaderTypeDef RxHeader;

// funcao que inicializa a can geral, chamada em initializer.c
void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref) {
    can_ptr = can_ref;
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
    uint8_t rx_data[8];

    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, rx_data)
            != HAL_OK) {
            /* Reception Error */
            Error_Handler();
        }
        uint32_t id     = RxHeader.Identifier;
        uint32_t length = RxHeader.DataLength;
        int32_t size;

        switch (length) {
            case FDCAN_DLC_BYTES_2: size = 1; break;
            case FDCAN_DLC_BYTES_4: size = 2; break;
            case FDCAN_DLC_BYTES_6: size = 3; break;
            case FDCAN_DLC_BYTES_8: size = 4; break;
            default: return;
        }
        for (int32_t i = 0; i < size; ++i) {
            general_can_vars_e var_name = general_get_var_name_from_id_and_pos(id, i);

            if (var_name > INVALID_VARIABLE_GENERAL && var_name < NUM_VARIABLE_GENERAL) {
                uint16_t data = concatenate_two_uint8_to_uint16(rx_data + i * 2);
                general_store_value(var_name, data);
            }
        }

        if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0)
            != HAL_OK) {
            /* Notification Error */
            Error_Handler();
        }
    }
}

// Callback for BUSOFF error in general can.
static void CAN_general_error_callback(FDCAN_HandleTypeDef* hfdcan,
                                       uint32_t ErrorStatusITs) {
    if (ErrorStatusITs | FDCAN_IT_BUS_OFF) {
        // Sets the flag for datalogging
        osEventFlagsSet(e_ECU_control_flagsHandle, GENERAL_BUS_OFF_ERROR_FLAG);
        // Clear INIT bit, so can communication is reestablished
        CLEAR_BIT(hfdcan->Instance->CCCR, FDCAN_CCCR_INIT);
    }
}
