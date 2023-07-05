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
#define BRAKE_ACTIVE     5000 // valor enviado pela SU-F: 2000
#define SU_F_ERROR       5000 // valor enviado pela SU-F: 3500
#define APPS_25_PERCENT  250
#define APPS_05_PERCENT  50
#define SU_F_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulamento: T.4.3.3 (2022)
#define APPS_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulamento: T.4.2.5 (2022)

// Valores abaixo obtidos durante a calibracao do APPS
// Explicacoes e mais detalhes disponiveis na planilha:
// https://docs.google.com/spreadsheets/d/1oN8rV0IG5MuoPPMHFQrklaCpWQLI31Kn/edit?usp=sharing&ouid=111087741893616112745&rtpof=true&sd=true

#define APPS_MATRIX_LENGTH 2

// APPS 1
#define APPS1_LOWER_DEADZONE   350
#define APPS1_UPPER_DEADZONE   1730
#define APPS1_ADJUST_SLOPE     0.662
#define APPS1_ADJUST_INTERCEPT (-165.404)
#define APPS1_MAX              1800
#define APPS1_MIN              200

// APPS 2
#define APPS2_LOWER_DEADZONE   750
#define APPS2_UPPER_DEADZONE   3580
#define APPS2_ADJUST_SLOPE     0.321
#define APPS2_ADJUST_INTERCEPT (-148.386)
#define APPS2_MIN              500
#define APPS2_MAX              3700

#endif /* INC_THROTTLE_H_ */
