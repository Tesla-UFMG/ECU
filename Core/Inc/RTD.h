/*
 * RTD_handler.h
 *
 *  Created on: Aug 15, 2021
 *      Author: Felipe Telles
 */

#ifndef INC_RTD_H_
#define INC_RTD_H_

#include "stdbool.h"

#define tempo_sirene 1000

void exit_RTD();
void aciona_sirene();
bool is_RTD_available();
void set_RTD();

#endif /* INC_RTD_H_ */
