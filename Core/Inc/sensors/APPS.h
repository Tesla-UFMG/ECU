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
#define BRAKE_ACTIVE     1200 // valor enviado pela SU-F: 2000
#define SU_F_ERROR       3000 // valor enviado pela SU-F: 3500
#define APPS_25_PERCENT  250
#define APPS_05_PERCENT  50
#define SU_F_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulamento: T.4.3.3 (2022)
#define APPS_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulamento: T.4.2.5 (2022)

// Valores abaixo obtidos durante a calibracao do APPS
// Explicacoes e mais detalhes disponiveis na planilha:
// https://docs.google.com/spreadsheets/d/1oN8rV0IG5MuoPPMHFQrklaCpWQLI31Kn/edit?usp=sharing&ouid=111087741893616112745&rtpof=true&sd=true

#define APPS_MATRIX_LENGTH 2

// APPS 1
#define APPS1_DEADZONE                                                                   \
    { 380, 1730 }
#define APPS1_ADJUST                                                                     \
    { 0.662, -165.404 }
#define APPS1_MAX 1800
#define APPS1_MIN 200

// APPS 2
#define APPS2_DEADZONE                                                                   \
    { 750, 3580 }
#define APPS2_ADJUST                                                                     \
    { 0.321, -148.386 }
#define APPS2_MIN 3650
#define APPS2_MAX 500

typedef enum { SLOPE = 0, INTERCEPT } linear_adjust_e;

typedef enum { LOWER = 0, UPPER } deadzone_limits_e;

#endif /* INC_THROTTLE_H_ */
