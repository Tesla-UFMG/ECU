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
#include "util/util.h"

volatile uint16_t datalog_data_holder[CAN_ID_QUAN];

extern osMessageQueueId_t q_datalog_messageHandle;

void datalogger(void* argument) {
    UNUSED(argument);

    datalog_message_t message;

    uint16_t vet_tx[4];

    for (;;) {

        ECU_ENABLE_BREAKPOINT_DEBUG();

        // as long as you can extract item from the message queue
        while (osMessageQueueGet(q_datalog_messageHandle, &message, 0, 0) == osOK) {
            datalog_data_holder[message.id] = message.data;
        }

        const uint16_t WRITE_ITERATION_LIMIT = ECU_CAN_LAST_POPULATED_ID
                                               + ECU_CAN_LAST_DEBUG_ID
                                               - ECU_CAN_FIRST_DEBUG_ID + 1;
        for (uint16_t id = ECU_CAN_INITIAL_ID; id < WRITE_ITERATION_LIMIT; id++) {
            for (uint16_t pos = 0; pos < 4; pos++) {
                uint16_t internal_index = get_internal_from_id_pos(id, pos);
                // if it passes a non-existent id and position combination, internal will
                // be 0. position 0 is always empty to fill in gaps
                vet_tx[pos] = datalog_data_holder[internal_index];
            }
            CAN_ID_t can_id = get_CAN_ID_from_internal(get_internal_from_id_pos(id, 0));
            // convey the message
            general_can_transmit(can_id.id, vet_tx);
        }

        // when extracting all queued items and submitting, it waits a certain amount of
        // time to extract again
        osDelay(DATALOGGER_DELAY);
    }
}
