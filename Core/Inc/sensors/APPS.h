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
    { 2200, 2300, 2445, 2605, 2751, 2910, 3040, 3210, 3345 }
#define APPS1_FIX_MUL                                                                    \
    { 0, 1.0526, 0.6896, 0.625, 0.6849, 0.6289, 0.7692, 0.5882, 0.7407 }
#define APPS1_FIX_ADD                                                                    \
    { 0, -2321, -1461.2, -1278.1, -1409.2, -1230.2, -1613.5, -1038.2, -1502.8 }
#define APPS1_MAX 3700
#define APPS1_MIN 0
// APPS 2
#define APPS2_REF                                                                        \
    { 300, 574, 872, 1205, 1530, 1860, 2155, 2465, 2828 }
#define APPS2_FIX_MUL                                                                    \
    { 0, 0.3650, 0.3356, 0.3003, 0.3077, 0.3030, 0.3390, 0.3226, 0.2755 }
#define APPS2_FIX_ADD                                                                    \
    { 0, -109.5, -67.6, -11.9, 4.2, 36.4, -5.5, 54.8, 195.9 }
#define APPS2_MAX 3300
#define APPS2_MIN 0

#endif /* INC_THROTTLE_H_ */
