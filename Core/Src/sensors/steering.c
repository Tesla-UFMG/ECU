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

    double volante_cru;
    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        volante_cru = ADC_DMA_buffer[STEERING_WHEEL_E];

        double zero_aux = ZERO_VOLANTE;

        /*if the steering minimum value is below 0, the sensor wraps around the ADC maximum value
        In this case, the ADC reading returns 4095*/
        /*therefore, subtract 4095 from the measured value to obtaining a negative value
		that can be used in the calculation. The same applies to the steering zero position*/
        if (VOLANTE_MIN > VOLANTE_MAX) {
            zero_aux -= 4095;
            if (volante_cru > VOLANTE_MAX) {
                volante_cru -= 4095;
            }
        }

        if (volante_cru < zero_aux) {
            set_global_var_value(STEERING_WHEEL, (STEERING_WHEEL_t)0);
        } else {
            set_global_var_value(
                STEERING_WHEEL,
                (STEERING_WHEEL_t)(volante_cru * GANHO_VOLANTE - ZERO_VOLANTE));
        }

        STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);

        log_data(ID_STEERING_WHEEL, steering_wheel);

        //SPAN_ALINHAMENTO, defines the tolerance rang used to determine whether the
        //steering wheel is considered to be in the center position
        if (steering_wheel > VOLANTE_ALINHADO + SPAN_ALINHAMENTO) {
            set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)ESQUERDA);
        } else if (steering_wheel < VOLANTE_ALINHADO - SPAN_ALINHAMENTO) {
            set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)DIREITA);
        } else {
            set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)CENTRO);
        }

        osDelay(100);

        //	return (volante);
    }
}
