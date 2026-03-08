/*
 * lateral_control.h
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_

#include "stdint.h"


//TODO (João): Verificar o tempo de amostragem. MUITO IMPORTANTE!
#define LATERAL_DELAY 30

//TODO (João): Verificar se isso é necessário. Acredito que não.
// defines calculos
#define HALF_GYRO     2000
#define ADJUST_GYRO_R 1000
#define ADJUST_GYRO_L 2000

// defines PID LATERAL
#define LUT_SIZE 10
static const double VX_REF[LUT_SIZE] = {5,7,9,11,13,15,17,19,21,23};
static const double P_REF[LUT_SIZE]  = {1,2,3,4,5,6,7,8,9,10};
static const double I_REF[LUT_SIZE]  = {1,2,3,4,5,6,7,8,9,10};

typedef struct {
    double torque_decrease[2];
} lateral_result_t;

lateral_result_t lateral_control();
void init_lateral_control();
void pi_lookup_table(double Vx, double *Pout, double *TIout);


#endif /* INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_ */
