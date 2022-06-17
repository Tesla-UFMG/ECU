/*
 * steering.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "sensors/steering.h"

#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_definitions.h"
#include "util/util.h"

extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

// calculation of current angle based on current adc reading, maximum steering wheel angle
// and from the min and max reading in the calibration using Thales' theorem. The function
// returns a value between 0 and 207 with 0 being all the way to the right and 207 being
// all the way to the left
uint16_t calculate_steering(double current_read) {
    double steering_calc;
    steering_calc = ANG_MAX_STEERING
                    * ((current_read - MIN_STEERING) / (MAX_STEERING - MIN_STEERING));
    return (uint16_t)steering_calc;
}

void steering_read(void* argument) {
    UNUSED(argument);

    double current_read;
    for (;;) {
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        current_read = ADC_DMA_buffer[STEERING_WHEEL_E];

        double zero_aux = MIN_STEERING;

        // As the sensor does not have a stop, if the steering minimum is lower than the 0
        // of the ADC, the sensor will return at the maximum value of the ADC (4095) so
        // 4095 is subtracted from the value read, giving a negative value that can be
        // applied to the formula
        if (MIN_STEERING > MAX_STEERING) {
            zero_aux -= 4095;
            if (current_read > MAX_STEERING) {
                current_read -= 4095;
            }
        }

        STEERING_WHEEL_t steering_wheel = (int16_t)calculate_steering(current_read);
        set_global_var_value(STEERING_WHEEL, steering_wheel);
        log_data(ID_STEERING_WHEEL, (uint16_t)(steering_wheel + DATALOG_STEERING_OFFSET));

        osDelay(STEERING_CALC_DELAY);
    }
}
