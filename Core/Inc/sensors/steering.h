/*
 * steering.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_STEERING_H_
#define INC_STEERING_H_


#define ZERO_VOLANTE          240	   // Bit value corresponding to 0 V at the sensor output; used to calibrate the measurement scaling.
#define GANHO_VOLANTE         1.11	   // Controls the sensor gain; used to calibrate the sensitivity and measurement scaling.
#define VOLANTE_ALINHADO      1944.5		   // ADC value when the steering wheel is in neutral position.
#define SPAN_ALINHAMENTO      200           // Sensor Dead zone.
#define VOLANTE_MAX           3189		   // Maximum value that the sensor can achieve once installed.
#define VOLANTE_MIN           700          // Minimum value that the sensor can achieve once installed.

//TODO (João): Atualizar esses valores com os dados do carro. Lembrar de atualizar o cálculo no steering.c
//  lookup table
#define STEERING_RAD_LEFT_WHEEL            ( (-0.523598776 - 0.599520598) / 2)
#define STEERING_RAD_RIGHT_WHEEL           ( (0.599520598 + 0.523598776) / 2)
#define STEERING_RAD_LEFT              (-1.941983885)
#define STEERING_RAD_RIGHT             (1.941983885)


#endif /* INC_STEERING_H_ */
