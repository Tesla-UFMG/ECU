/*
 * lateral_control.h
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_


#include "stdint.h"


#define LATERAL_DELAY 30

// defines calculos
#define HALF_GYRO     2000
#define ADJUST_GYRO_R 1000
#define ADJUST_GYRO_L 2000
//#define ADJUST_STEERING 2000
// lookup table
#define Y0            ((-0.523598776 - 0.599520598) / 2)
#define Y1            ((0.599520598 + 0.523598776) / 2)
#define X0            (-1.941983885)
#define X1            (1.941983885)
// defines PID LATERAL
#define KP_LATERAL    6.51864262048678
#define KI_LATERAL    43.9160892044863
#define TI_LATERAL    (KP_LATERAL / KI_LATERAL) // 0.14843404179579


typedef struct {
    double torque_decrease[2];
} lateral_result_t;

lateral_result_t lateral_control();
void init_lateral_control();

#endif /* INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_ */
