/*
 * RTD.h
 *
 *  Created on: Aug 15, 2021
 *      Author: Felipe Telles
 */

#ifndef INC_RTD_H_
#define INC_RTD_H_

// FSAE Rules: EV.10.5.2.a (2023)
// Note : Judge specifically requested to use maximum time during Electrical Inspection
#define RTDS_TIME_MS 3000
#include "cmsis_os.h"
#include "util/util.h"


void exit_RTD();
void activate_RTDS();
bool can_RTD_be_enabled();
void set_RTD();

#endif /* INC_RTD_H_ */
