/*
 * steering.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "sensors/steering.h"

extern volatile uint16_t ADC_DMA_buffer[ADC_LINES];

uint16_t steering_adc_raw;
uint16_t previous_adc_raw = 0;
int32_t delta = 0;
int32_t steering_position_adc = 0;
float steering_angle;
float steering_rad=0;
float steering_wheel_rad=0;
uint16_t first_value = 0;
INTERNAL_WHEEL_t internal_wheel;
bool is_initialized = false;

uint16_t buf[WINDOW];     // circular buffer of last WINDOW raw samples
uint8_t  idx = 0;
uint32_t sum = 0;
bool     primed = false;
void steering_read(void* argument) {

	UNUSED(argument);

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();
        //Read the ADC value from the steering sensor.
        wait_for_rtd();
        steering_adc_raw = moving_average(ADC_DMA_buffer[STEERING_WHEEL_E]);

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
        steering_angle = steering_rad*180/PI;
        set_global_var_value(STEERING_WHEEL, (STEERING_WHEEL_t)(steering_wheel_rad));
        //STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
        float steering_wheel = steering_wheel_rad*1000 + 645;
        log_data(ID_STEERING_WHEEL, steering_wheel);


        if (steering_position_adc > SENSOR_DEAD_ZONE) {
            set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)RIGHT);
        } else if (steering_position_adc < -SENSOR_DEAD_ZONE ) {
              set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)LEFT);
        } else {
              set_global_var_value(INTERNAL_WHEEL, (INTERNAL_WHEEL_t)CENTER);
        }

        internal_wheel =  get_global_var_value(INTERNAL_WHEEL);
        log_data(ID_INTERNAL_WHEEL, get_global_var_value(INTERNAL_WHEEL));

        previous_adc_raw = steering_adc_raw;

        is_initialized = true;

    	osDelay(10);

        //return(volante);
    }
}

//algoritmo á ser testado para tirar ruído do ADC apps ?? --> embora eu não vá usar ele, eu posso usar
//algo do genêro para ver qual tá sendo a flutuação nos picos
uint16_t moving_average(uint16_t new_sample) {
		sum -= buf[idx];               // drop the oldest contribution
		buf[idx] = new_sample;         // overwrite oldest slot
		sum += new_sample;             // add the newest
		idx = (idx + 1) % WINDOW;
		if (!primed && idx == 0) primed = true;
		return primed ? (sum / WINDOW) : (sum / (idx ? idx : 1));
}
