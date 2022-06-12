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
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        volante_cru = ADC_DMA_buffer[STEERING_WHEEL_E];

        double zero_aux = ZERO_VOLANTE;

        // Se o minimo do volante for menor que 0, o sensor voltara no valor maximo do ADC
        // se isso acontecer, o valor do ADC voltara para 4095
        // entao subtrai 4095 do valor lido, dando um valor negativo que pode ser aplicado
        // na formula o mesmo vale pro zero do volante
        if (VOLANTE_MIN > VOLANTE_MAX) {
            zero_aux -= 4095;
            if (volante_cru > VOLANTE_MAX) {
                volante_cru -= 4095;
            }
        }

        if (volante_cru < zero_aux) {
            set_global_var_value(STEERING_WHEEL, 0);
        } else {
            set_global_var_value(STEERING_WHEEL,
                                 (uint16_t)volante_cru * GANHO_VOLANTE - ZERO_VOLANTE);
        }

        STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);

        log_data(ID_STEERING_WHEEL, steering_wheel);

        // SPAN_ALINHAMENTO eh apenas um span pra ainda considerar o volante no centro
        // ate uma certa quantidade
        if (steering_wheel > VOLANTE_ALINHADO + SPAN_ALINHAMENTO) {
            set_global_var_value(INTERNAL_WHEEL, ESQUERDA);
        } else if (steering_wheel < VOLANTE_ALINHADO - SPAN_ALINHAMENTO) {
            set_global_var_value(INTERNAL_WHEEL, DIREITA);
        } else {
            set_global_var_value(INTERNAL_WHEEL, CENTRO)
        }

        osDelay(100);

        //	return (volante);
    }
}
