/*
 * datalogger.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */
#include "datalogging/datalogger.h"

#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "cmsis_os.h"
#include "util/global_definitions.h"

volatile uint16_t datalog_data_holder[CAN_ID_QUAN];

extern osMessageQueueId_t q_datalog_messageHandle;


void datalogger(void* argument) {
    UNUSED(argument);

    datalog_message_t message;

    uint16_t vet_tx[4];

    int16_t internal_id;

    uint16_t external_id;

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        // as long as you can extract item from the message queue
        while (osMessageQueueGet(q_datalog_messageHandle, &message, 0, 0) == osOK) {
            datalog_data_holder[message.id] = message.data;
        }

        // does the for using the size of the structure
        for (uint16_t struct_pos = 1; struct_pos < get_amount_ext_id(); struct_pos++) {
            for (uint16_t word = 0; word < 4; word++) {
                internal_id = get_internal_id_from_pos_and_word(struct_pos, word);
                // if internal id does not exist
                if (internal_id == -1) {
                    vet_tx[word] = 0;
                } else {
                    vet_tx[word] = datalog_data_holder[internal_id];
                }
            }
            // transmit data via CAN
            external_id = get_external_id_from_struct_pos(struct_pos);
            general_can_transmit(external_id, vet_tx);
        }

        // when extracting all queued items and sending, wait a certain amount of time to
        // extract again
        osDelay(DATALOGGER_DELAY);
    }
}
