/*
 * error_treatment.h
 *
 *  Created on: Sep 10, 2021
 *      Author: Felipe Telles
 */

#ifndef SRC_ERROR_TREATMENT_H_
#define SRC_ERROR_TREATMENT_H_

#include "stdbool.h"
#include "stdint.h"

void check_for_errors(bool (* areThereErrors)(), uint32_t flagError);
void issue_error(uint32_t flagError, bool should_set_control_event_flag);
void clear_error(uint32_t flagError);

#endif /* SRC_ERROR_TREATMENT_H_ */
