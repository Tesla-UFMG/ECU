/*
 * encoder_speed.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "sensors/encoder_speed.h"

#include "datalogging/speed.h"
#include "stm32h7xx.h"
#include "string.h"
#include "util/CMSIS_extra/cmsis_extra.h"
#include "util/global_instances.h"
#include "util/util.h"


//Initialization of functions
static void reset_speed_all();

static void reset_speed_single(const encoder_int_message_t* message, const encoder_int_message_t* last_messages, uint32_t min_count_rear, uint32_t min_count_front);

static inline uint32_t get_tim2_freq();

static inline uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc);

static inline uint32_t calculate_speed_rear(uint32_t speed, uint32_t freq, uint32_t presc);

static inline uint32_t calculate_timeout(uint32_t speed, uint32_t freq, uint32_t presc);

static inline uint32_t calculate_timeout_rear(uint32_t speed, uint32_t freq, uint32_t presc);

static inline uint32_t calculate_timeout_RTOS(uint32_t speed);

//Variable that stores the speed of each wheel
static encoder_speeds_message_t speeds_message;

/* Functions that calculate wheel's speeds (rear and front axle)
 *
 * distance = circumference divided by the number of teeth
 *          = 2*pi*radius/teeth_number (m)
 * time     = timer value multiplied by the prescaler divided by frequency
 *          = timer*prescaler/frequency (s)
 * speed    = distance/time
 *          = (2*pi*radius/teeth_number)*freq/(presc*timer) (m/s)
 *          = (10*3.6*2*pi*radius/teeth_number)*freq/(presc*timer) (10*km/h)
 */
 static inline uint32_t calculate_speed(uint32_t time_between_messages, uint32_t freq, uint32_t presc) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER)* ((float)freq / ((float)presc)) / time_between_messages);
}

static inline uint32_t calculate_speed_rear(uint32_t time_between_messages, uint32_t freq, uint32_t presc) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER_REAR)* ((float)freq / ((float)presc)) / time_between_messages);
}

/*
//Functions that calculate the timeout for rear and front wheels, seperatly.
static inline uint32_t calculate_timeout(uint32_t speed) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER) * 1000 / speed);
}
static inline uint32_t calculate_timeout_rear(uint32_t speed) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER_REAR) * 1000 / speed);
}
*/

//Functions that calculate the timeout for rear and front wheels

//calculates time in ticks
static inline uint32_t calculate_timeout(uint32_t speed, uint32_t freq, uint32_t presc) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER) * (freq/presc) / speed);
}
//calculates time in ticks
static inline uint32_t calculate_timeout_rear(uint32_t speed, uint32_t freq, uint32_t presc) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER_REAR) * (freq/presc) / speed);
}

//calculates time in milisseconds
static inline uint32_t calculate_timeout_RTOS(uint32_t speed) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER) * 1000 / speed);
}

//Function that reset all wheel's speed.
static void reset_speed_all() {
    for (uint8_t i = 0; i < WHEEL_ENCODERS_AVAILABLE; i++) {
        speeds_message.wheels[i] = 0;
    }
}

//Function that reset the speed of a single wheel, if it is without an interruption for a long time.
static void reset_speed_single(const encoder_int_message_t* message, const encoder_int_message_t* last_messages, uint32_t max_count_rear, uint32_t max_count_front) {
    for (speed_pin_e i = FIRST_WHEEL; i <= WHEEL_ENCODERS_AVAILABLE; i++) {
    	uint32_t max_count;
    	if(i == REAR_RIGHT || i == REAR_LEFT){
    		max_count = max_count_rear;
    	}else{
    		max_count = max_count_front;
    	}
        if ((message->tim_count - last_messages[i].tim_count) > max_count) {
            speeds_message.wheels[i] = 0;
        }
    }
}

// gets tim2 frequency from the APB1 clock domain, considering that it might have an
// prescaler which doubles the frequency
static inline uint32_t get_tim2_freq() {
    // Get PCLK1 prescaler
    if (RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) {
        // PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1
        return 2 * HAL_RCC_GetPCLK1Freq();
    }
    // PCLK1 prescaler equal to 1 => TIMCLK = PCLK1
    return HAL_RCC_GetPCLK1Freq();
}


void encoder_speed_calc(void) {
    
    //Variable that stores the message received from the encoder interrupt, which contains the timer count and the wheel pin that caused the interruption
    encoder_int_message_t interrupt_message;

    //Variable that stores the last message received from each wheel
    encoder_int_message_t last_interrupt_messages[4];

    // initialize with zeros the last messages buffer
    // NOLINTNEXTLINE
    memset(&last_interrupt_messages, 0, sizeof(encoder_int_message_t) * 4);

    // gets tim2 frequency
    const uint32_t tim_freq = get_tim2_freq();

    //  tim2 prescaler
    const uint32_t tim_presc = htim2.Init.Prescaler + 1;

    // value in tim2 time of the minimum period between messages. This will be used to avoid mistakes caused by signal noise.
    const uint32_t min_count_rear = calculate_timeout_rear(MAX_SPEED, tim_freq, tim_presc);
    const uint32_t min_count = calculate_timeout(MAX_SPEED, tim_freq, tim_presc);
    // value in tim2 time of the minimum speed which will be calculated
    
    
    // value in timersys time of the maximum period between messages. This will be used as a value to comparison in "reset_speed_single"
    const uint32_t max_timeout_rear = calculate_timeout_rear(MIN_SPEED, tim_freq, tim_presc);
    const uint32_t max_timeout = calculate_timeout(MIN_SPEED, tim_freq, tim_presc);
    
    // value in milliseconds of the maximum period between messages to consider in the RTOS task. This will be the max time the function osMessageQueueGet will wait for a message before returning with timeout error.
    const uint32_t max_timeout_RTOS = calculate_timeout_RTOS(MIN_SPEED);

    //Variable that will store the difference between the current message timer count and the last message timer count
    uint32_t d_tim_count;
    //Variable that will store the speed calculated for a wheel, which will be sent to the datalogging task
    uint32_t speed; 

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        // waits until a message arrives or until timeout
        switch (osMessageQueueGet(q_encoder_int_messageHandle, &interrupt_message, NULL,
                                  max_timeout_RTOS)) {

            // case the task was called by timeout
            case osErrorTimeout:
                reset_speed_all(); // zeros all wheels speed
                break;

            // case the task was called by a new message arriving
            default:
                // verifies if any wheel is without an interruption for a long time,
                // if yes that wheel speed is zeroed
                reset_speed_single(&interrupt_message, last_interrupt_messages,
                                   max_timeout_rear, max_timeout);

                // difference between current message and last message timestamp
                d_tim_count = interrupt_message.tim_count
                              - last_interrupt_messages[interrupt_message.pin].tim_count;

                //discards value if d_tim_count results in a speed greater than the one configured as maximum
                if (interrupt_message.pin == REAR_RIGHT || interrupt_message.pin == REAR_LEFT) {
                	if (d_tim_count < min_count_rear) {
                		continue;
                	}
                } else {
                	if (d_tim_count < min_count) {
                		continue;
                	}
                }


                if(interrupt_message.pin == REAR_RIGHT || interrupt_message.pin == REAR_LEFT){
                	speed = calculate_speed_rear(d_tim_count, tim_freq, tim_presc);
                }else{
                	speed = calculate_speed(d_tim_count, tim_freq, tim_presc);
                }
                // saves the speed only of the wheel which speed was just calculated
                speeds_message.wheels[interrupt_message.pin] = speed;
                // store message to use in the next iteration
                last_interrupt_messages[interrupt_message.pin] = interrupt_message;
                break;
        }
        osMessageQueuePutOverwrite(q_encoder_speeds_messageHandle, &speeds_message, 0);
    }
}
