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

uint16_t steering_adc_raw;
uint16_t previous_adc_raw = 0;
int32_t delta = 0;
int32_t steering_position_adc = 0;
float steering_rad;
float steering_wheel_rad;
uint16_t first_value = 0;
INTERNAL_WHEEL_t internal_wheel;
bool is_initialized = false;
extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

void steering_read(void* argument) {

	UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        //Read the ADC value from the steering sensor.
        steering_adc_raw = ADC_DMA_buffer[STEERING_WHEEL_E];

        //Store the initial ADC steering sensor.
        if(!is_initialized){
        	first_value = steering_adc_raw;
        }

        //Difference between the current and previous ADC readings.
        //Handle ADC wraparound.
        delta = (int32_t)steering_adc_raw - (int32_t)previous_adc_raw;

        //Detect ADC wraparound using the difference (delta) between consecutive samples.
        //A large positive or negative delta indicates that the ADC reading crossed zero.
        //Starts after the first sample, when a previous reading is available.
        if(is_initialized){
        	if(delta > ADC_MAX_VALUE/2){
        		delta -= ADC_MAX_VALUE;
        	}
        	else if(delta < -ADC_MAX_VALUE/2){
        		delta += ADC_MAX_VALUE;
        	}
        	//Accumulate all ADC deltas using the first reading as the zero reference.
        	steering_position_adc += delta;
        }

        //Convert the accumulated ADC counts to an angle in radians.
        steering_rad = (steering_position_adc * ((2.0f * PI)/ADC_MAX_VALUE));
        steering_wheel_rad = steering_rad/STEERING_RATIO;

        set_global_var_value(STEERING_WHEEL, (STEERING_WHEEL_t)(steering_wheel_rad));
        //STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
        float steering_wheel = steering_wheel_rad*1000 + 645;
        log_data(ID_STEERING_WHEEL, steering_wheel);


        if (steering_position_adc > SENSOR_DEAD_ZONE) {
            set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)DIREITA);
        } else if (steering_position_adc < -SENSOR_DEAD_ZONE ) {
              set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)ESQUERDA);
        } else {
              set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)CENTRO);
        }

        internal_wheel =  get_global_var_value(INTERNAL_WHEEL);
        log_data(ID_INTERNAL_WHEEL, get_global_var_value(INTERNAL_WHEEL));

        previous_adc_raw = steering_adc_raw;

        is_initialized = true;

    	osDelay(10);

        //return(volante);
    }
}
