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
        double steering_scaled_bits;
        double steering_rad;
        double steering_wheel_rad;

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
        	steering_scaled_bits = 0;
        } else {
        	steering_scaled_bits = (volante_cru * GANHO_VOLANTE) - ZERO_VOLANTE;
        }


        if (steering_scaled_bits < VOLANTE_MIN){
        	steering_scaled_bits = VOLANTE_MIN;
        } else if(steering_scaled_bits > VOLANTE_MAX){
        	steering_scaled_bits = VOLANTE_MAX;
        }


        //lookup table
        //y = y0 + (y1-y0)/(x1-x0) * (x-x0)
        //steering to right is positive, to left is negative.
        //negative sign compensates the sensor behavior, since its voltage decreases when steering to the right.
        steering_rad = - (STEERING_RAD_LEFT + ( (STEERING_RAD_RIGHT - STEERING_RAD_LEFT) / (VOLANTE_MAX - VOLANTE_MIN) ) * (steering_scaled_bits - VOLANTE_MIN));
        steering_wheel_rad =  (STEERING_RAD_LEFT_WHEEL + ( (STEERING_RAD_RIGHT_WHEEL - STEERING_RAD_LEFT_WHEEL) / (STEERING_RAD_RIGHT - STEERING_RAD_LEFT) ) * (steering_rad - STEERING_RAD_LEFT));


        set_global_var_value(STEERING_WHEEL, (STEERING_WHEEL_t)(steering_wheel_rad));
        STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
        log_data(ID_STEERING_WHEEL, steering_wheel);


        //SPAN_ALINHAMENTO, defines the tolerance rang used to determine whether the
        //steering wheel is considered to be in the center position
        if (steering_scaled_bits > VOLANTE_ALINHADO + SPAN_ALINHAMENTO) {
            set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)ESQUERDA);
        } else if (steering_scaled_bits < VOLANTE_ALINHADO - SPAN_ALINHAMENTO) {
            set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)DIREITA);
        } else {
            set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)CENTRO);
        }

        osDelay(100);

        //	return (volante);
    }
}
