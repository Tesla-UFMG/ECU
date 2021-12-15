/*
 * throttle.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_THROTTLE_H_
#define INC_THROTTLE_H_

#define THROTTLE_DELAY 25
#define BRAKE_ACTIVE 2000
#define SU_F_ERROR 3300
#define APPS_25_PERCENT 250
#define APPS_05_PERCENT 50

// Valores abaixo obtidos durante a calibração do APPS
// Explicações e mais detalhes disponíveis na planilha:
// https://onedrive.live.com/view.aspx?resid=52B70A6379F6F8AB!115743&ithint=file%2cxlsx&authkey=!AJ8JGeSHTz3KdoQ

#define APPS_MATRIX_LENGTH 9
// APPS 1
#define APPS1_REF 200, 300, 400, 500, 600, 700, 800, 900, 1135
#define APPS1_FIX_MUL 0, 1.75, 2.05, 1.75, 1.95, 1.85, 1.85, 1.95, 1.75
#define APPS1_FIX_ADD 2065, 1735, 1645, 1765, 1665, 1725, 1725, 1645, 1825
#define APPS1_MAX 3700
#define APPS1_MIN 1900
// APPS 2
#define APPS2_REF 280, 470, 860, 1220, 1570, 1933, 2280, 2630, 3400
#define APPS2_FIX_MUL 0, 1.5789, 0.2564, 0.2778, 0.2857, 0.2754, 0.2882, 0.2857, 0.2941
#define APPS2_FIX_ADD 0, -442.1, 179.5, 161.1, 151.4, 167.5, 142.9, 148.6, 126.5
#define APPS2_MAX_VALUE 3500
#define APPS2_MAX_PERCENT 1000

#endif /* INC_THROTTLE_H_ */
