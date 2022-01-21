/*
 * odometer.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */
#include "odometer.h"
#include "cmsis_os.h"
#include "util.h"

void odometer_calc(void* argument) {
    UNUSED(argument);

    for (;;) {
        osDelay(100);
    }
    /*
    const static uint8_t TEST_DIST = 0;
    const static uint8_t OVERALL_DIST = 1;

    // calculates delta
    Dist_Calc();

    // verifica se o calculo da distancia ja deu 1 metro ou mais - valor retornado em dist_calc esta
    em decimetro

    if ((dist_calc / 10) >= 1) {
            dist_log[TEST_DIST] += (dist_calc / 10);		// updates test and overall distance
            dist_log[OVERALL_DIST] += (dist_calc / 10);		// updates distance already
    travelled

            // rounds to the nearest meter
            Record_Distance(dist_log);						// writes values in flash
    memory
    }*/
}
