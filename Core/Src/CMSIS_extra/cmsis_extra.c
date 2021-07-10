/*
 * cmsis_extra.c
 *
 *  Created on: 10 de jul de 2021
 *      Author: renanmoreira
 */

#include "CMSIS_extra/cmsis_extra.h"

#include "FreeRTOS.h"
#include "queue.h"

osStatus_t osMessageQueuePutOverwrite(osMessageQueueId_t mq_id,
                                      const void *msg_ptr, uint8_t msg_prio) {
    QueueHandle_t hQueue = (QueueHandle_t)mq_id;
    osStatus_t stat;
    BaseType_t yield;

    (void)msg_prio; /* Message priority is ignored */

    stat = osOK;

    if (IS_IRQ()) {
        if ((hQueue == NULL) || (msg_ptr == NULL)) {
            stat = osErrorParameter;
        } else {
            yield = pdFALSE;
            if (xQueueOverwriteFromISR(hQueue, msg_ptr, &yield) != pdTRUE) {
                stat = osErrorResource;
            } else {
                portYIELD_FROM_ISR(yield);
            }
        }
    } else {
        if ((hQueue == NULL) || (msg_ptr == NULL)) {
            stat = osErrorParameter;
        } else {
            if (xQueueOverwrite(hQueue, msg_ptr) != pdPASS) {
                stat = osErrorResource;
            }
        }
    }

    return (stat);
}
