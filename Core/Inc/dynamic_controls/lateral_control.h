/*
 * lateral_control.h
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_
#define INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_

#include "stdint.h"


#define LATERAL_DELAY 0.014

//defines PID LATERAL
#define LUT_SIZE 10
static const double VX_REF[LUT_SIZE] = {5,7,9,11,13,15,17,19,21,23};
//static const double P_REF[LUT_SIZE]  = {11.85,49.26,64.61,93.15,111.82,128.26,158.12,344.21,353.61,363.16};
//static const double I_REF[LUT_SIZE]  = {415.39,69.16,42.06,24.40,16.47,12.61,9.08,15.82,16.97,17.87};

static const double P_REF[LUT_SIZE]  = {159.54, 177.67, 189.37, 197.74, 220.20, 212.14, 179.10, 175.82, 408.69, 413.96};
static const double I_REF[LUT_SIZE]  = {69.34, 44.03, 31.78, 24.63, 20.04, 15.63, 14.57, 12.62, 21.69, 20.18};

typedef struct {
    double torque_decrease[2];
} lateral_result_t;

lateral_result_t lateral_control();
void init_lateral_control();
void pi_lookup_table(double Vx, double *Pout, double *TIout);

#endif /* INC_DYNAMICCONTROLS_LATERAL_CONTROL_H_ */
