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

void check_for_errors(bool (* areThereErrors)(), int flagError);
void issue_error(uint32_t flagError);

#endif /* SRC_ERROR_TREATMENT_H_ */
