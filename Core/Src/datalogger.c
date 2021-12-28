/*
 * datalogger.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */
#include "datalogger.h"
#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "cmsis_os.h"
#include "global_definitions.h"

volatile uint16_t datalog_data_holder[CAN_ID_QUAN];

extern osMessageQueueId_t q_datalog_messageHandle;

void datalogger(void *argument) {

    datalog_message_t message;

    uint16_t vet_tx[4];

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        // enquanto conseguir extrair item da fila de mensagens
        while (osMessageQueueGet(q_datalog_messageHandle, &message, 0, 0) == osOK) {
            datalog_data_holder[message.id] = message.data;
        }

        const uint16_t WRITE_ITERATION_LIMIT =
            ECU_CAN_LAST_POPULATED_ID + ECU_CAN_LAST_DEBUG_ID - ECU_CAN_FIRST_DEBUG_ID + 1;
        for (uint16_t id = ECU_CAN_INITIAL_ID; id < WRITE_ITERATION_LIMIT; id++) {
            for (uint16_t pos = 0; pos < 4; pos++) {
                uint16_t internal_index = get_internal_from_id_pos(id, pos);
                // caso passe por uma combinacao de id e posicao inexistente,
                // internal sera 0. a posicao 0 e sempre vazia para preencher
                // lacunas
                vet_tx[pos] = datalog_data_holder[internal_index];
            }
            CAN_ID_t can_id = get_CAN_ID_from_internal(get_internal_from_id_pos(id, 0));
            // transmite a mensagem
            general_can_transmit(can_id.id, vet_tx);
        }

        // quando extrair todos os itens enfileirados e enviar, espera uma certa
        // quantidade de tempo para extrair novamente
        osDelay(DATALOGGER_DELAY);
    }
}
