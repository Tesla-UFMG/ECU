/*
 * steering.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_STEERING_H_
#define INC_STEERING_H_


#define ZERO_VOLANTE          662.9894	   // Bit value corresponding to 0 V at the sensor output; used to calibrate the measurement scaling.
#define GANHO_VOLANTE         0.9722419	   // Controls the sensor gain; used to calibrate the sensitivity and measurement scaling.
#define VOLANTE_ALINHADO      1030		   // ADC value when the steering wheel is in neutral position.
#define SPAN_ALINHAMENTO      70           // Sensor Dead zone.
#define VOLANTE_MAX           3189		   // Maximum value that the sensor can achieve once installed.
#define VOLANTE_MIN           700          // Minimum value that the sensor can achieve once installed.


#endif /* INC_STEERING_H_ */
