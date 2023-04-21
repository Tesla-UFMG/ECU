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

        double zero_aux = STEERING_ZERO;

        // Se o minimo do volante for menor que 0, o sensor voltara no valor maximo do ADC
        // se isso acontecer, o valor do ADC voltara para 4095
        // entao subtrai 4095 do valor lido, dando um valor negativo que pode ser aplicado
        // na formula o mesmo vale pro zero do volante
        if (STEERING_MIN > STEERING_MAX) {
            zero_aux -= 4095;
            if (volante_cru > STEERING_MAX) {
                volante_cru -= 4095;
            }
        }

        if (volante_cru < zero_aux) {
            set_global_var_value(STEERING_WHEEL, 0);
        } else {
            set_global_var_value(STEERING_WHEEL,
                                 (uint16_t)volante_cru * STEERING_GAIN - STEERING_ZERO);
        }

        STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);

        log_data(ID_STEERING_WHEEL, steering_wheel);

        // SPAN_ALINHAMENTO eh apenas um span pra ainda considerar o volante no centro
        // ate uma certa quantidade
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
