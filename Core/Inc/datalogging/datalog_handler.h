/*
 * datalog_handler.h
 *
 *  Created on: Dec 14, 2020
 *      Author: renanmoreira
 */

#ifndef INC_DATALOG_HANDLER_H_
#define INC_DATALOG_HANDLER_H_

#include "cmsis_os.h"
#include "util/global_definitions.h"
#include "CAN/CAN_IDs.h"

osStatus_t log_data(uint16_t id, uint16_t data);

#endif /* INC_DATALOG_HANDLER_H_ */
