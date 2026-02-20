/*
 * constants.h
 *
 *  Created on: Dec 10, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CONSTANTS_H_
#define INC_CONSTANTS_H_

#define num_amostras_filtro   10		   // Moving average sample of the accelerator.
#define TORQUE_MAX            3000 		   // In tenth of a percent of the nominal torque.
#define vel_max_rpm           9000
#define vetorizacao_torque    0
#define frenagem_regenerativa 0
#define REG_BRAKING_TORQUE    750
#define modo_aceleracao       0
#define modo_enduro           0
#define zona_morta_acelerador 10
#define TORQUE_GAIN           1
#define ZERO_VOLANTE          662.9894	   // Bit value corresponding to 0 V at the sensor output; used to calibrate the measurement scaling.
#define GANHO_VOLANTE         0.9722419	   // Controls the sensor gain; used to calibrate the sensitivity and measurement scaling.
#define VOLANTE_ALINHADO      1030		   // ADC value when the steering wheel is in neutral position.
#define SPAN_ALINHAMENTO      70           // Sensor Dead zone.
#define VOLANTE_MAX           3189		   // Maximum value that the sensor can achieve once installed.
#define VOLANTE_MIN           700          // Minimum value that the sensor can achieve once installed.
#define GAIN_ARRANC           1.1
#define INC_VELOC             50
#define DIREITA               0
#define ESQUERDA              1
#define CENTRO                2

#endif /* INC_CONSTANTS_H_ */
