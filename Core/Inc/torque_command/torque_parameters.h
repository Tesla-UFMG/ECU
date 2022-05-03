/*
 * controle.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CONTROLE_H_
#define INC_CONTROLE_H_

#include "util/global_variables.h"

#define P_ENABLE   0
#define P_RUNSTOP  1
#define P_BRAKE    2
#define P_THROTTLE 3

// TODO(renanmoreira): reavaliar quantidade certa de delay
#define CONTROLE_DELAY 50
// TODO(Felipe): verificar se esse define tá certo
#define D_5_kmph_rpm   459

#endif /* INC_CONTROLE_H_ */
