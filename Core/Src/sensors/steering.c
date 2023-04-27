/*
 * steering.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "sensors/steering.h"

#include "datalogging/datalog_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_definitions.h"
#include "util/util.h"

extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

void steering_read(void* argument) {
    UNUSED(argument);

    double steering_cru;
    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        steering_cru = ADC_DMA_buffer[STEERING_WHEEL_E];

        double zero_aux = STEERING_ZERO;

        // If the flywheel minimum is less than 0, the sensor will go back to the maximum
        // value of the ADC if this happens, the value of the ADC will go back to 4095
        // then subtract 4095 from the value read, giving a negative value that can be
        // applied in the formula the same goes for pro steering wheel zero
        if (STEERING_MIN > STEERING_MAX) {
            zero_aux -= 4095;
            if (steering_cru > STEERING_MAX) {
                steering_cru -= 4095;
            }
        }

        if (steering_cru < zero_aux) {
            set_global_var_value(STEERING_WHEEL, 0);
        } else {
            set_global_var_value(STEERING_WHEEL,
                                 (uint16_t)steering_cru * STEERING_GAIN - STEERING_ZERO);
        }

        STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);

        log_data(ID_STEERING_WHEEL, steering_wheel);

        // SPAN_ALIGNED is just a span to still consider the steering wheel in the center
        // up to a certain amount
        if (steering_wheel > ALIGNED_STEERING + SPAN_ALIGNED) {
            set_global_var_value(INTERNAL_WHEEL, LEFT);
        } else if (steering_wheel < ALIGNED_STEERING - SPAN_ALIGNED) {
            set_global_var_value(INTERNAL_WHEEL, RIGHT);
        } else {
            set_global_var_value(INTERNAL_WHEEL, CENTER);
        }

        osDelay(100);

        //	return (volante);
    }
}
