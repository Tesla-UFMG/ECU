/*
 * throttle.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_THROTTLE_H_
#define INC_THROTTLE_H_

#include "stdint.h"

#define THROTTLE_DELAY   25
#define BRAKE_ACTIVE     1500 // valor enviado pela SU-F: 2000
#define SU_F_ERROR       3000 // valor enviado pela SU-F: 3500
#define APPS_25_PERCENT  250
#define APPS_05_PERCENT  50
#define SU_F_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulamento: T.4.3.3 (2022)
#define APPS_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulamento: T.4.2.5 (2022)

// Valores abaixo obtidos durante a calibracao do APPS
// Explicacoes e mais detalhes disponiveis na planilha:
// https://docs.google.com/spreadsheets/d/1oN8rV0IG5MuoPPMHFQrklaCpWQLI31Kn/edit?usp=sharing&ouid=111087741893616112745&rtpof=true&sd=true

#define APPS_MATRIX_LENGTH 9
// APPS 1
#define APPS1_REF                                                                        \
    { 3345, 3210, 3040, 2910, 2751, 2605, 2445, 2300, 2200 }
#define APPS1_FIX_MUL                                                                    \
    { -0.7407407407, -0.5882352941, -0.7692307692, -0.6289308176, -0.6849315068, -0.625, -0.6896551724, -1, 0 }
#define APPS1_FIX_ADD                                                                    \
    { 2477.777778, 2013.235294, 2588.461538, 2205.188679, 2384.246575, 2253.125, 2436.206897, 3175, 0 }
#define APPS1_MAX 2000
#define APPS1_MIN 3700
// APPS 2
#define APPS2_REF                                                                        \
    { 2822, 2465, 2155, 1860, 1530, 1205, 872, 574, 300 }
#define APPS2_FIX_MUL                                                                    \
    { -0.2801120448, -0.3225806452, -0.3389830508, -0.303030303, -0.3076923077, -0.3003003003, -0.3355704698, -0.3649635036, 0  }
#define APPS2_FIX_ADD                                                                    \
    { 790.4761905, 920.1612903, 980.5084746, 938.6363636, 970.7692308, 986.8618619, 1042.61745, 1084.489051, 0 }
#define APPS2_MAX 200
#define APPS2_MIN 3400

#endif /* INC_THROTTLE_H_ */
