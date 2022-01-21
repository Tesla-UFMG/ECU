/*
 * longitudinal_control.h
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */
#ifndef INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_

#include "CMSIS_extra/global_variables_handler.h"
#include "PID.h"
#include <stdint.h>

#define LONGITUDINAL_DELAY 30
#define IDEAL_SLIP_DRY     13
#define IDEAL_SLIP_WET     30

// defines PID LONGITUDINAL
#define KP_LONGITUDINAL 6.51864262048678 // TODO(renanmoreira): check values
#define KI_LONGITUDINAL 0.14843404179579
#define TI_LONGITUDINAL (KP_LONGITUDINAL / KI_LONGITUDINAL)

typedef struct {
    double ref_decrease;
    uint8_t wheel;
    PID_t pid_longitudinal;
} longitudinal_t;

typedef struct {
    double torque_decrease[2];
} longitudinal_control_result_t;

longitudinal_control_result_t longitudinal_control();
double wheel_control(uint8_t wheel_motor, WHEEL_SPEEDS_t wheel_speeds);
void init_longitudinal_control();

#endif /* INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_ */
