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
//TODO (Guilherme): Verificar o tempo de amostragem. MUITO IMPORTANTE!
#define LONGITUDINAL_DELAY 30

//TODO (Guilherme): Pode ser interessante pensar em alterar o valor do setpoint. Durante o processo de validação viu-se que o valor ótimo de slip varia entre 0,1 e 0,2. Nesse sentido, pode ser interessante tentar definir qual seria o valor do slip ratio ideal do nosso pneu.
//All the slip related variables are defined with a negative sign. This is essential to make the code work correctly and in the most simple way possible. 
/* 
First it is important to understand that the idea of the longitudinal control is to reduce the torque of the motor when the slip is bigger than the ideal. 
Therefore, we expect the controller to act when the slip is bigger than the ideal, that is, when the error (defined as "ideal slip - real slip") is negative.
The problem is that if the error is negative the PID output will be negative as well, fact that would make the torque reference increase instead of decrease.

The solution for this problem is to define the slip and the setpoint with a negative sign, so when the absolute value of the slip is bigger than the ideal, the error will be positive and the PID output will be negative.
With this, the code will behave as expected, reducing the torque reference when the slip is bigger than the ideal and not changing it when the slip is smaller than the ideal.
*/
#define IDEAL_SLIP_DRY     -13
#define IDEAL_SLIP_WET     -30

//Defines the longitudinal PID's parameters
//TODO (Guilherme): Ainda precisamos definir os parâmetros do controlador.
#define KP_LONGITUDINAL 6.51864262048678 
#define KI_LONGITUDINAL 0.14843404179579
#define TI_LONGITUDINAL (KP_LONGITUDINAL / KI_LONGITUDINAL)

//Creates a struct that will be used to store the longitudinal PID's outputs
typedef struct {
    double torque_decrease[2];
} longitudinal_control_result_t;

//Initializes some functions that will be developed in the file "longitudinal_control.c"
void init_longitudinal_control();
double wheel_control();
longitudinal_control_result_t longitudinal_control();



#endif /* INC_DYNAMICCONTROLS_LONGITUDINAL_CONTROL_H_ */

