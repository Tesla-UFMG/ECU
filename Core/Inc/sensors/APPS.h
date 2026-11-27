/*
 * APPS.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_THROTTLE_H_
#define INC_THROTTLE_H_

#include "stdint.h"

#define THROTTLE_DELAY   25
#define BRAKE_ACTIVE     1500 // valor enviado pela SU-F: 2800
#define SU_F_ERROR       2900 // valor enviado pela SU-F: 3500
#define APPS_25_PERCENT  250
#define APPS_05_PERCENT  50
#define SU_F_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulamento: T.4.3.3 (2022)
#define APPS_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulamento: T.4.2.5 (2022)

// Valores abaixo obtidos durante a calibracao do APPS
// Explicacoes e mais detalhes disponiveis na planilha:
// https://docs.google.com/spreadsheets/d/1oN8rV0IG5MuoPPMHFQrklaCpWQLI31Kn/edit?usp=sharing&ouid=111087741893616112745&rtpof=true&sd=true

#define APPS_MATRIX_LENGTH 2

// APPS 1
#define APPS1_LOWER_DEADZONE   255
#define APPS1_UPPER_DEADZONE   1055
#define APPS1_ADJUST_SLOPE     1.25
#define APPS1_ADJUST_INTERCEPT (-300.0)
#define APPS1_MIN              240
#define APPS1_MAX              1035

// APPS 2
#define APPS2_LOWER_DEADZONE   505
#define APPS2_UPPER_DEADZONE   2140
#define APPS2_ADJUST_SLOPE     0.61
#define APPS2_ADJUST_INTERCEPT (-300.31)
#define APPS2_MIN              490
#define APPS2_MAX              2120

#endif /* INC_THROTTLE_H_ */
