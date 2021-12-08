/*
 * error_treatment.h
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */

#ifndef SRC_ERROR_TREATMENT_H_
#define SRC_ERROR_TREATMENT_H_

#include "stdbool.h"
#include "global_instances.h"

void check_for_errors(bool (* areThereErrors)(), uint32_t flagError);
void check_for_errors_with_timer(bool (* areThereErrors)(), uint32_t flagError, osTimerId_t timerHandler, uint16_t timerAmount);
void errors_with_timer_callback(void *argument);

#endif /* SRC_ERROR_TREATMENT_H_ */
