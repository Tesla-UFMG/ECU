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

#define APPS_MATRIX_LENGTH 19
// APPS 1
#define APPS1_REF                                                                        \
    { 3574, 3520, 3414, 3280, 3223, 3120, 2988, 2933, 2805, 2755, 2672, 2563, 2472, 2388, 2277, 2205, 2121, 2050, 2000 }
#define APPS1_FIX_MUL                                                                    \
    { 0, -1.028, -0.524, -0.414, -0.974, -0.539, -0.420, -1.009, -0.434, -1.11, -0.669, -0.509, -0.610, -0.661, -0.5, -0.771, -0.661, -0.782, -1.11 }
#define APPS1_FIX_ADD                                                                    \
    { 0, 3895.3, 2120.5, 1747.0, 3582.2, 2180.7, 1811.3, 3570.2, 1882.2, 3779.5, 2563.7, 2137.5, 2395.6, 2521.3, 2137.5, 2754.2, 1456.9, 1658, 2275.5 }
#define APPS1_MAX 1900
#define APPS1_MIN 3700
// APPS 2
#define APPS2_REF                                                                        \
    { 3276, 3192, 2990, 2735, 2630, 2445, 2196, 2095, 1858, 1753, 1600, 1400, 1225, 1055, 840, 694, 523, 375, 273 }
#define APPS2_FIX_MUL                                                                    \
    { -0.2801120448, -0.3225806452, -0.3389830508, -0.303030303, -0.3076923077, -0.3003003003, -0.3355704698, -0.3649635036, 0  }
#define APPS2_FIX_ADD                                                                    \
    { 790.4761905, 920.1612903, 980.5084746, 938.6363636, 970.7692308, 986.8618619, 1042.61745, 1084.489051, 0 }
#define APPS2_MAX 200
#define APPS2_MIN 3400

#endif /* INC_THROTTLE_H_ */
