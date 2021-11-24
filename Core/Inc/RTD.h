/*
 * RTD_handler.h
 *
 *  Created on: Aug 15, 2021
 *      Author: Felipe Telles
 */

#ifndef INC_RTD_H_
#define INC_RTD_H_

#include "global_variables.h"

#define RTDS_TIME 1000 //1 segundo: Regulamento: EV.10.5.2.a (2021)

void exit_RTD();
void aciona_sirene(uint32_t tempo_sirene);


#endif /* INC_RTD_H_ */
