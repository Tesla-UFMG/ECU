/*
 * constants_control.h
 *
 *  Created on: 17 de set de 2021
 *      Author: Luiza
 */

#ifndef INC_DYNAMICCONTROLS_CONSTANTS_CONTROL_H_
#define INC_DYNAMICCONTROLS_CONSTANTS_CONTROL_H_

// vehicle parameters
#define NOMINAL_TORQUE       13
#define FRONT_DISTANCE       1.0     	// distance between the CG and the front axle
#define REAR_DISTANCE        0.549      // distance between the CG and the rear axle
#define MASS                 412        // vehicle (342kg) + driver (70kg)
#define FRONT_CORNER_STIFF   49870      // cornering stiffness of the front axle
#define REAR_CORNER_STIFF    55370      // cornering stiffness of the rear axle
#define FRICTION_COEFFICIENT 1.3
#define TUNABILITY_FACTOR    0.8
#define GRAVITY              9.81
#define WHEELBASE            (FRONT_DISTANCE + REAR_DISTANCE)
#define KU                                                                              \
   ( ( (REAR_DISTANCE * MASS) / (FRONT_CORNER_STIFF * WHEELBASE) )                       \
   - ( (FRONT_DISTANCE * MASS) / (REAR_CORNER_STIFF * WHEELBASE) ) )

#endif /* INC_DYNAMICCONTROLS_CONSTANTS_CONTROL_H_ */
