/*
 * global_variables.h
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_VARIABLES_H_
#define INC_GLOBAL_VARIABLES_H_

#include "global_definitions.h"

//velocidade de cada roda individualmente
extern volatile float g_wheel_speed[4];

//modo atual de corrida
extern volatile race_mode_t g_race_mode;

#endif /* INC_GLOBAL_VARIABLES_H_ */
