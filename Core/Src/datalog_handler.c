/*
 * datalog_handler.c
 *
 *  Created on: Dec 14, 2020
 *      Author: renanmoreira
 */

#include "datalog_handler.h"
#include "cmsis_os.h"
#include "global_definitions.h"
#include "CAN/CAN_IDs.h"

extern osMessageQueueId_t q_datalog_messageHandle;

osStatus_t log_data(uint16_t id, uint16_t data) {
	datalog_message_t message = {id, data};
	return osMessageQueuePut(q_datalog_messageHandle, &message, 0, 0U);
}

