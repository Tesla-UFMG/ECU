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


	//calculo do angulo atual com base na leitura atual do adc, no angulo maximo do volante e da
    //leitura do min e do maximo na calibracao usando teorema de Tales
float calculo_steering (float volante_atual, float zero_volante, float volante_max, float ang_max)
{
        float steering_calc_a;
        steering_calc_a = ang_max * ((volante_atual - zero_volante)/(volante_max - zero_volante));
        return steering_calc_a;
}


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

        // Como o sensor nao possui batente, se o minimo do volante for menor que o 0 do ADC,
        // o sensor voltara no valor maximo do ADC (4095)
        // entao é subtraido 4095 do valor lido, dando um valor negativo que pode ser aplicado
        // na formula
        if (ZERO_VOLANTE > VOLANTE_MAX) {
            zero_aux -= 4095;
            if (volante_cru > VOLANTE_MAX) {
                volante_cru -= 4095;
            }
        }



        // A funcao converte o valor do ADC para o valor em graus do estercamento do volante
        // Tambem informa a direcao do volante
        // SPAN_ALINHAMENTO eh apenas um span pra ainda considerar o volante no centro
        // ate uma certa quantidade
        if (volante_cru > VOLANTE_ALINHADO + SPAN_ALINHAMENTO) {
            set_global_var_value(INTERNAL_WHEEL, ESQUERDA);
            set_global_var_value(STEERING_WHEEL,
                                       (uint16_t)(calculo_steering(volante_cru, ZERO_VOLANTE, VOLANTE_MAX, ANG_MAX_VOLANTE) - ANGULO_VOLANTE_ALINHADO));
        } else if (volante_cru < VOLANTE_ALINHADO - SPAN_ALINHAMENTO) {
            set_global_var_value(INTERNAL_WHEEL, DIREITA);
            set_global_var_value(STEERING_WHEEL,
            							(uint16_t)ANGULO_VOLANTE_ALINHADO - calculo_steering(volante_cru, ZERO_VOLANTE, VOLANTE_MAX, ANG_MAX_VOLANTE));
        } else {
            set_global_var_value(INTERNAL_WHEEL, CENTRO);
            set_global_var_value(STEERING_WHEEL, 0);
        }

        STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);

        log_data(ID_STEERING_WHEEL, steering_wheel);

        osDelay(100);

    }
}
