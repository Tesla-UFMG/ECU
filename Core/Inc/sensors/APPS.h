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

#define APPS_MATRIX_LENGTH 8
// APPS 1
#define APPS1_REF                                                                        \
    { 2200, 2284, 2502, 2587, 2824, 2955, 3186, 3424 }
#define APPS1_FIX_MUL                                                                    \
    { 0, 1.1070, 0.6550, 1.68, 0.6025, 1.0901, 0.6182, 0.6 }
#define APPS1_FIX_ADD                                                                    \
    { 0, -2385.5, -1353.3, -3917.8, -1130.3, -2507.2, -1112.7, -1054.4 }
#define APPS1_MAX 3600
#define APPS1_MIN 2000
// APPS 2
#define APPS2_REF                                                                        \
    { 508, 799, 1256, 1466, 1901, 2185, 2663, 3138 }
#define APPS2_FIX_MUL                                                                    \
    { 0, 0.4907, 0.3125, 0.68, 0.3283, 0.5028, 0.2987, 0.3006 }
#define APPS2_FIX_ADD                                                                    \
    { 0, -249.3, -106.9, -568.5, -52.8, -384.6, 61.2, 56.6 }
#define APPS2_MAX 3300
#define APPS2_MIN 0

#endif /* INC_THROTTLE_H_ */
