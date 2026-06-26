/*
 * steering.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_STEERING_H_
#define INC_STEERING_H_

#define ADC_MAX_VALUE		  4095
#define ZERO_VOLANTE          220	   // Bit value corresponding to 0 V at the sensor output; used to calibrate the measurement scaling.
#define GANHO_VOLANTE         1.02	   // Controls the sensor gain; used to calibrate the sensitivity and measurement scaling.
#define VOLANTE_ALINHADO      1325	   // ADC value when the steering wheel is in neutral position.
#define SPAN_ALINHAMENTO      150      // Sensor Dead zone.
#define VOLANTE_MAX           3990	   // Maximum value that the sensor can achieve once installed.
#define VOLANTE_MIN           340      // Minimum value that the sensor can achieve once installed.

//TODO (João): Atualizar esses valores com os dados do carro. Lembrar de atualizar o cálculo no steering.c
//  lookup table
#define STEERING_RAD_LEFT_WHEEL          - ( (0.558505 + 0.733038) / 2)
#define STEERING_RAD_RIGHT_WHEEL           ( (0.733038 + 0.558505) / 2)
#define STEERING_RAD_LEFT              (-2.26893)
#define STEERING_RAD_RIGHT             (2.26893)
#define RELATION_DIRECAO				4.4


#endif /* INC_STEERING_H_ */
