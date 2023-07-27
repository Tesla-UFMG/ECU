/*
 * inverter_can_error_manager.h
 *
 *  Created on: Jan 5, 2022
 *      Author: Felipe Telles
 */

#ifndef INC_CAN_INVERTER_CAN_MONITOR_H_
#define INC_CAN_INVERTER_CAN_MONITOR_H_

#define INVERTER_NO_MESSAGE_ERROR_TIME 500
#define INVERTER_PRECHARGE_TIME        (500 / portTICK_PERIOD_MS)

#define LEFT_INV_SMALLEST_ID  0x100
#define LEFT_INV_BIGGEST_ID   0x103
#define RIGHT_INV_SMALLEST_ID 0x200
#define RIGHT_INV_BIGGEST_ID  0x203

#endif /* INC_CAN_INVERTER_CAN_MONITOR_H_ */
