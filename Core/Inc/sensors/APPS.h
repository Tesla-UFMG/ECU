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

#define APPS_MATRIX_LENGTH 2
// APPS 1
#define APPS1_REF                                                                        \
    { 3500, 1900 }
#define APPS1_FIX_MUL                                                                    \
    { -0.61 }
#define APPS1_FIX_ADD                                                                    \
    { 2190 }

/*
#define APPS1_REF                                                                        \
    { 3574, 3520, 3414, 3280, 3223, 3120, 2988, 2933, 2805, 2755, 2672, 2563, 2472, 2388,
2277, 2205, 2121, 2050, 2000 }
#define APPS1_FIX_MUL                                                                    \
    {0, -1.028, -0.524, -0.414, -0.974, -0.539, -0.420, -1.009, -0.434, -1.11, -0.669,
-0.509, -0.610, -0.661, -0.5, -0.771, -0.661, -0.782, -1.11 }
#define APPS1_FIX_ADD                                                                    \
    {0, 3895.3, 2120.5, 1747.0, 3582.2, 2180.7, 1811.3, 3570.2, 1882.2, 3779.5, 2563.7,
2137.5, 2395.6, 2521.3, 2137.5, 2754.2, 1456.9, 1658, 2275.5}
*/
#define APPS1_MAX 1900
#define APPS1_MIN 3700
// APPS 2
#define APPS2_REF                                                                        \
    { 3200, 250 }
#define APPS2_FIX_MUL                                                                    \
    { -0.322 }
#define APPS2_FIX_ADD                                                                    \
    { 1062 }
/*
#define APPS2_REF                                                                        \
    { 3276, 3192, 2990, 2735, 2630, 2445, 2196, 2095, 1858, 1753, 1600, 1400, 1225, 1055,
840, 694, 523, 375, 273 }
#define APPS2_FIX_MUL                                                                    \
    {0, -0.661, -0.275, -0.212, -0.529, -0.3, -0.223, -0.549, -0.234, -0.529, -0.363,
-0.278, -0.317, -0.326, -0.258, -0.380, -0.325, -0.375, -0.544 }
#define APPS2_FIX_ADD                                                                    \
    {0, 2386.5, 1154.5, 983.8, 1834.1, 1233, 1044.5, 1761.7, 1101.1, 1648.1, 1357.4, 1221,
1276.5, 1287.9, 1215.8, 1318.3, 225.2, 196.1, 204 }
*/
#define APPS2_MAX 200
#define APPS2_MIN 3400

#endif /* INC_THROTTLE_H_ */
