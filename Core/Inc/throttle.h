/*
 * throttle.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_THROTTLE_H_
#define INC_THROTTLE_H_

#include "stdint.h"

#define THROTTLE_DELAY 25
#define BRAKE_ACTIVE 1500   //valor enviado pela SU-F: 2000
#define SU_F_ERROR 3000     //valor enviado pela SU-F: 3500
#define APPS_25_PERCENT 250
#define APPS_05_PERCENT 50
#define SU_F_ERROR_TIMER (100 - THROTTLE_DELAY)   //Regulamento: T.4.3.3 (2022)
#define APPS_ERROR_TIMER (100 - THROTTLE_DELAY)   //Regulamento: T.4.2.5 (2022)

// Valores abaixo obtidos durante a calibração do APPS
// Explicações e mais detalhes disponíveis na planilha:
// https://docs.google.com/spreadsheets/d/1oN8rV0IG5MuoPPMHFQrklaCpWQLI31Kn/edit?usp=sharing&ouid=111087741893616112745&rtpof=true&sd=true

#define APPS_MATRIX_LENGTH 9
// APPS 1
#define APPS1_REF 2065, 2260, 2465, 2640, 2835, 3020, 3205, 3400, 3575
#define APPS1_FIX_MUL 0, 1.5385, 0.4878, 0.5714, 0.5128, 0.5405, 0.5405, 0.5128, 0.5714
#define APPS1_FIX_ADD 0, -3177, -802.4, -1008, -853.8, -932.4, -932.4, -843.6, -1043
#define APPS1_MAX 3700
#define APPS1_MIN 1900
// APPS 2
#define APPS2_REF 280, 470, 860, 1220, 1570, 1933, 2280, 2630, 2970
#define APPS2_FIX_MUL 0, 1.5789, 0.2564, 0.2778, 0.2857, 0.2754, 0.2882, 0.2857, 0.2941
#define APPS2_FIX_ADD 0, -442.1, 179.5, 161.1, 151.4, 167.5, 142.9, 148.6, 126.5
#define APPS2_MAX 3100
#define APPS2_MIN 0

typedef struct {
    uint16_t value[APPS_MATRIX_LENGTH];
    float fix_mul[APPS_MATRIX_LENGTH];
    float fix_add[APPS_MATRIX_LENGTH];
} apps_ref;

#endif /* INC_THROTTLE_H_ */
