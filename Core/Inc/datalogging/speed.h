/*
 * speed.h
 *
 *  Created on: 3 de mai de 2022
 *      Author: Felipe Telles
 */

#ifndef INC_DATALOGGING_SPEED_H_
#define INC_DATALOGGING_SPEED_H_

#include "util/global_definitions.h"

typedef struct {
    uint16_t wheels[WHEEL_ENCODERS_AVAILABLE];
} encoder_speeds_message_t;

#endif /* INC_DATALOGGING_SPEED_H_ */
