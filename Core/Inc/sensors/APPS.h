/*
 * APPS.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_THROTTLE_H_
#define INC_THROTTLE_H_

#include "stdint.h"
#include "stdbool.h"

// Initialization of functions
typedef struct {
    uint16_t deadzone_lower_limit;
    uint16_t deadzone_upper_limit;
    float adjust_parameters_slope;
    float adjust_parameters_intercept;
} apps_ref;
static uint16_t throttle_calc(uint16_t APPS_VALUE, const apps_ref* ref);
static bool is_there_APPS_error();
static bool is_there_BSE_error();
static bool is_there_SU_F_error();

#define THROTTLE_DELAY   25
#define BRAKE_ACTIVE     1800 // value send by SU-F: 2800
#define SU_F_ERROR       3200 // value send by SU-F: 3500
#define APPS_25_PERCENT  250
#define APPS_05_PERCENT  50
#define SU_F_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulation: T.4.3.3 (2022)
#define APPS_ERROR_TIMER (100 - THROTTLE_DELAY) // Regulation: T.4.2.5 (2022)

// Values bellow obtained during the APPS calibration
// Explanations and more details available in the spreadsheet:
// https://docs.google.com/spreadsheets/d/1oN8rV0IG5MuoPPMHFQrklaCpWQLI31Kn/edit?usp=sharing&ouid=111087741893616112745&rtpof=true&sd=true

#define APPS_MATRIX_LENGTH 2

// APPS 1
#define APPS1_LOWER_DEADZONE   370
#define APPS1_UPPER_DEADZONE   1530
#define APPS1_ADJUST_SLOPE     0.7843
#define APPS1_ADJUST_INTERCEPT (-270.58)
#define APPS1_MIN              345
#define APPS1_MAX              1620

// APPS 2
#define APPS2_LOWER_DEADZONE   500
#define APPS2_UPPER_DEADZONE   2100
#define APPS2_ADJUST_SLOPE     0.5714
#define APPS2_ADJUST_INTERCEPT (-268.57)
#define APPS2_MIN              470
#define APPS2_MAX              2220

#endif /* INC_THROTTLE_H_ */
