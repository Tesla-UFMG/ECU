/*
 * initializer_controls.h
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_INITIALIZER_CONTROLS_H_
#define INC_DYNAMICCONTROLS_INITIALIZER_CONTROLS_H_

//defines PID LATERAL
#define KP_LATERAL 6.51864262048678
#define KI_LATERAL 43.9160892044863
#define TI_LATERAL KP_LATERAL / KI_LATERAL //0.14843404179579

void init_controls();

#endif /* INC_DYNAMICCONTROLS_INITIALIZER_CONTROLS_H_ */