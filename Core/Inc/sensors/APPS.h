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
    { 2240, 2320, 2430, 2680, 2860, 3030, 3120, 3440 }
#define APPS1_FIX_MUL                                                                    \
    { 0, 0.6896, 0.9091, 0.4, 0.5556, 0.5882, 1.1111, 0.3125 }
#define APPS1_FIX_ADD                                                                    \
    { 0, -1500, -1966.3, -686.4, -1060.5, -1111.1, -2652.7, -118.2}
#define APPS1_MAX 3700
#define APPS1_MIN 0
// APPS 2
#define APPS2_REF                                                                        \
    { 310, 815, 1140, 1460, 1800, 2070, 2540, 2930 }
#define APPS2_FIX_MUL                                                                    \
    { 0, 0.1905, 0.3077, 0.3125, 0.2941, 0.3704, 0.2128, 0.2564 }
#define APPS2_FIX_ADD                                                                    \
    { 0, -55.2, -107.9, -70.6, -1.01, -95.5, 273.6, 205.5 }
#define APPS2_MAX 3300
#define APPS2_MIN 0

#endif /* INC_THROTTLE_H_ */
