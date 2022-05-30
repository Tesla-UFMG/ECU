/*
 * constants.h
 *
 *  Created on: Dec 10, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CONSTANTS_H_
#define INC_CONSTANTS_H_

#define num_amostras_filtro    10   // Amostra media movel acelerador
#define TORQUE_MAX             3000 // em decimos de percentagem do torque nom
#define vel_max_rpm            9000
#define vetorizacao_torque     0
#define frenagem_regenerativa  0
#define REG_BRAKING_TORQUE     750
#define modo_aceleracao        0
#define modo_enduro            0
#define zona_morta_acelerador  10
#define TORQUE_GAIN            1
#define GANHO_VOLANTE          11.375 // 4096 do ADC dividido por 360 graus
#define ALIGNED_STEERING       1809
#define ALIGNED_STEERING_ANGLE 97
#define ANG_MAX_STEERING       207
#define SPAN_ALIGNMENT         70
#define MIN_STEERING           662
#define MAX_STEERING           3189
#define GAIN_ARRANC            1.1
#define INC_VELOC              50
#define DIREITA                0
#define ESQUERDA               1
#define CENTRO                 2

#endif /* INC_CONSTANTS_H_ */
