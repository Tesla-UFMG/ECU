/*
 * odometer.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_ODOMETER_H_
#define INC_ODOMETER_H_


#include "stdint.h"
#include "util/global_definitions.h"

#define SPEED_SENSOR_TEETH_QUAN 16   // quantidade de dentes da roda fonica
#define WHEEL_RADIUS            0.26 // em m
#define MAX_SPEED               2000 // em km/h*10
#define MIN_SPEED               10   // em km/h*10

#define FIRST_WHEEL FRONT_RIGHT
#define LAST_WHEEL  FRONT_LEFT // ultima roda que possui sensor de roda fonica

typedef struct
{
    uint32_t tim_count;
    dist_pin_e pin;  //Da mesma maneira que para a velocidade, seria necessário adicionar um typedef enum para o pino do sensor da velocidade, porém com utilizando nomenclatura para distância?
} dist_message_t;


#endif /* INC_ODOMETER_H_ */
