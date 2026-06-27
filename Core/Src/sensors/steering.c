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

double zero_aux;
double steering_scaled_bits;

uint16_t volante_cru;
uint16_t previous_value = 0;
int32_t delta;
int32_t steering_position = 0;
float steering_rad;
float steering_wheel_rad;
uint16_t primeiroValor = 0;
volatile int16_t valor_inicial = 0;
int16_t somador = 0;



extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

void steering_read(void* argument) {

	UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        volante_cru = ADC_DMA_buffer[STEERING_WHEEL_E];

        //apenas para inicializar: volante alinhado
        if(primeiroValor == 0){
        	valor_inicial = volante_cru;
        }

        delta = (int32_t)volante_cru - (int32_t)previous_value;

        /*if(fabs(delta) < 30){
        	delta = 0;
        }*/


        if(primeiroValor != 0){

        if(delta > ADC_MAX_VALUE/2){
        	delta -= ADC_MAX_VALUE;
        	//-1095
        }

        else if(delta < -ADC_MAX_VALUE/2){
        	delta += ADC_MAX_VALUE;
        }

        	steering_position += delta;
        }

        steering_rad = (steering_position * ((2.0*3.1415)/ADC_MAX_VALUE));

        steering_wheel_rad = steering_rad/RELATION_DIRECAO;

        set_global_var_value(STEERING_WHEEL, (STEERING_WHEEL_t)(steering_wheel_rad));
        //STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
        float steering_wheel = steering_wheel_rad*1000 + 645;
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

        previous_value = volante_cru;
    	primeiroValor = 1;
        osDelay(10);

        //	return (volante);
    }
}
