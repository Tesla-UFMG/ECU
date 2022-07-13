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

extern datalog_send_t datalog_send_struct[QUANT_RESERVED_ID + 1];

void datalogger(void* argument) {
    UNUSED(argument);

    datalog_message_t message;

    uint16_t vet_tx[4];

    int16_t intern_id;

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
        for (int i = 1; i < get_quant_id(); i++) {
            for (int pos = 0; pos < 4; pos++) {
                intern_id = datalog_send_struct[i].pos[pos];
                // if internal id does not exist
                if (intern_id == -1) {
                    vet_tx[pos] = 0;
                } else {
                    vet_tx[pos] = datalog_data_holder[intern_id];
                }
            }
            // transmit data via CAN
            general_can_transmit(datalog_send_struct[i].extern_ID, vet_tx);
        }

        // when extracting all queued items and sending, wait a certain amount of time to
        // extract again
        osDelay(DATALOGGER_DELAY);
    }
}
