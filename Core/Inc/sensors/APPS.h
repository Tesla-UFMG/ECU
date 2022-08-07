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

#define APPS_MATRIX_LENGTH 10
// APPS 1
#define APPS1_REF                                                                        \
    { 2200, 2270, 2430, 2570, 2700, 2900, 3110, 3200, 3460, 3520 }
#define APPS1_FIX_MUL                                                                    \
    { 0, 1.0526, 0.625, 0.7143, 0.7692, 0.5, 0.4762, 1.1111, 0.3846, 1.6667 }
#define APPS1_FIX_ADD                                                                    \
    { 0, -2259.5, -1307.7, -1513.5, -1643.6, -905.6, -825.4, -2788.9, -453.1, -4877.9}
#define APPS1_MAX 3700
#define APPS1_MIN 2000
// APPS 2
#define APPS2_REF                                                                        \
    { 310, 600, 955, 1280, 1570, 2000, 2350, 2700, 3035, 3120 }
#define APPS2_FIX_MUL                                                                    \
    { 0, 0.3125, 0.2817, 0.3077, 0.3448, 0.2326, 0.2857, 0.2857, 0.2985, 1.1765 }
#define APPS2_FIX_ADD                                                                    \
    { 0, -87.5, -57.9, -71.6, -108.1, 79.3, -15.9, -4.8, -28.3, -2681.8 }
#define APPS2_MAX 3300
#define APPS2_MIN 100

#endif /* INC_THROTTLE_H_ */
