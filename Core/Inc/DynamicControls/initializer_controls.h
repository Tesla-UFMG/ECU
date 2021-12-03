/*
 * initializer_controls.h
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_INITIALIZER_CONTROLS_H_
#define INC_DYNAMICCONTROLS_INITIALIZER_CONTROLS_H_

//vehicle parameters
#define FRONT_DISTANCE 0.775                   // distance between the CG and the front axle
#define REAR_DISTANCE 0.775                    // distance between the CG and the rear axle
#define MASS 357                               // vehicle + pilot
#define FRONT_CORNER_STIFF 50420.286           // cornering stiffness of the front axle
#define REAR_CORNER_STIFF 50420.286            // cornering stiffness of the rear axle
#define FRICTION_COEFFICIENT 1.3
#define GRAVITY 9.81
#define WHEELBASE FRONT_DISTANCE + REAR_DISTANCE
#define KU (REAR_DISTANCE * MASS / (FRONT_CORNER_STIFF * WHEELBASE)) - (FRONT_DISTANCE * MASS / (REAR_CORNER_STIFF * WHEELBASE))

//defines PID LONGITUDINAL
#define KP_LONGITUDINAL 6.51864262048678// TODO: check values
#define KI_LONGITUDINAL 0.14843404179579
#define TI_LONGITUDINAL KP_LONGITUDINAL / KI_LONGITUDINAL

void init_controls();


#endif /* INC_DYNAMICCONTROLS_INITIALIZER_CONTROLS_H_ */
