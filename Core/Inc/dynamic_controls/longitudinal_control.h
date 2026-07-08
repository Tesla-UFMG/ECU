/*
 * longitudinal_control.h
 *
 *  Created on: Sep 15th 2021
 *      Author: GiovanniPasa
 */
#ifndef INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_

#include "PID.h"
#include "util/CMSIS_extra/global_variables_handler.h"

#include <stdint.h>

//Defines the longitudinal control's sample time and setpoint
//TODO (Guilherme): check the sampling time. REALLY IMPORTANT!
#define LONGITUDINAL_DELAY 30

/*TODO (Guilherme): It might be interesting change the setpoint value.
During the validation process, we saw that the optimum value of slip goes from 0,1 to 0,2.
So, it might be intersting define what is the optimum slip ratio value for our tire*/
#define IDEAL_SLIP_DRY     13
#define IDEAL_SLIP_WET     30

//Defines the longitudinal PID's parameters
//TODO (Guilherme): Ainda precisamos definir os parâmetros do controlador.
#define KP_LONGITUDINAL 6.51
#define KI_LONGITUDINAL 0.14
#define TI_LONGITUDINAL (KP_LONGITUDINAL / KI_LONGITUDINAL)

//Creates a struct that will be used to store the longitudinal PID's outputs
typedef struct {
    double torque_decrease[2];
} longitudinal_control_result_t;

//Initializes some functions that will be developed in the file "longitudinal_control.c"
void init_longitudinal_control();
longitudinal_control_result_t longitudinal_control();



#endif /* INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_ */

