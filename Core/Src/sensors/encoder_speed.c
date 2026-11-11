/*
 * encoder_speed.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "sensors/encoder_speed.h"
#include <stdlib.h>
#include "datalogging/speed.h"
#include "stm32h7xx.h"
#include "string.h"
#include "util/CMSIS_extra/cmsis_extra.h"
#include "util/global_instances.h"
#include "util/util.h"

#include <math.h>


//Initialization of functions
static void reset_data_all();

static void reset_data_single(const encoder_int_message_t* message, const encoder_int_message_t* last_messages, uint32_t min_count_rear, uint32_t min_count_front);

static inline uint32_t get_tim2_freq();

static inline float calculate_speed_m_s(uint32_t ticks_between_messages, uint32_t freq, uint32_t presc);
static inline float calculate_speed_m_s_rear(uint32_t ticks_between_messages, uint32_t freq, uint32_t presc);

static inline uint32_t calculate_timeout(uint32_t speed, uint32_t freq, uint32_t presc);
static inline uint32_t calculate_timeout_rear(uint32_t speed, uint32_t freq, uint32_t presc);
static inline uint32_t calculate_timeout_RTOS(uint32_t speed);

// Mudança: Aceleração recebe as velocidades não-truncadas e o delta de tempo exato
static inline float calc_acceleration(float speed_m_s, float last_speed_m_s, float delta_time_s);

//Variable that stores the speed of each wheel
static encoder_speeds_message_t speeds_message;
static encoder_accelerations_message_t accelerations_message;

// Variable that stores the last speed of each wheel, used to calculate the acceleration
static float last_speed_m_s[WHEEL_ENCODERS_AVAILABLE] = {0.0f, 0.0f, 0.0f, 0.0f};

/* Functions that calculate wheel's speeds (rear and front axle)
 *
 * distance = circumference divided by the number of teeth
 *          = 2*pi*radius/teeth_number (m)
 * time     = timer value multiplied by the prescaler divided by frequency
 *          = timer*prescaler/frequency (s)
 * speed    = distance/time (m/s)
 */
static float dif;
static inline float calculate_speed_m_s(uint32_t ticks_between_messages, uint32_t freq, uint32_t presc) {
    return (float)((2.0 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER) * ((float)freq / (float)presc) / (float)ticks_between_messages);
}

static inline float calculate_speed_m_s_rear(uint32_t ticks_between_messages, uint32_t freq, uint32_t presc) {
    return (float)((2.0 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER_REAR) * ((float)freq / (float)presc) / (float)ticks_between_messages);
}

//Function that calculates the acceleration of a wheel using pure floats and delta time
static inline float calc_acceleration(float speed_m_s, float last_speed_m_s, float delta_time_s) {
    if (delta_time_s > 0.0f) {
        return (speed_m_s - last_speed_m_s) / delta_time_s;
    }
    return 0.0f;
}

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
static void reset_data_all() {
    for (uint8_t i = 0; i < WHEEL_ENCODERS_AVAILABLE; i++) {
        speeds_message.wheels[i] = 0;
        accelerations_message.wheels[i] = 0;
        last_speed_m_s[i] = 0.0f;
    }
}

//Function that reset the speed of a single wheel, if it is without an interruption for a long time.
static void reset_data_single(const encoder_int_message_t* message, const encoder_int_message_t* last_messages, uint32_t max_count_rear, uint32_t max_count_front) {
    for (speed_pin_e i = 0; i <= WHEEL_ENCODERS_AVAILABLE-1; i++) {
        uint32_t max_count;
        if(i == REAR_RIGHT || i == REAR_LEFT){
            max_count = max_count_rear;
        }else{
            max_count = max_count_front;
        }
        if ((message->tim_count - last_messages[i].tim_count) > max_count) {
            speeds_message.wheels[i] = 0;
            accelerations_message.wheels[i] = 0;
        }
    }
}

// gets tim2 frequency from the APB1 clock domain, considering that it might have an prescaler (used to divide the frequency)
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
    
    encoder_int_message_t interrupt_message;
    encoder_int_message_t last_interrupt_messages[WHEEL_ENCODERS_AVAILABLE];

    memset(&last_interrupt_messages, 0, sizeof(encoder_int_message_t) * 4);

    const uint32_t tim_freq = get_tim2_freq();
    const uint32_t tim_presc = htim2.Init.Prescaler + 1;

    const uint32_t min_count_rear = calculate_timeout_rear(MAX_SPEED_KM_H, tim_freq, tim_presc);
    const uint32_t min_count = calculate_timeout(MAX_SPEED_KM_H, tim_freq, tim_presc);
    
    const uint32_t max_timeout_rear = calculate_timeout_rear(MIN_SPEED_KM_H, tim_freq, tim_presc);
    const uint32_t max_timeout = calculate_timeout(MIN_SPEED_KM_H, tim_freq, tim_presc);
    
    const uint32_t max_timeout_RTOS = calculate_timeout_RTOS(MIN_SPEED_KM_H);

    uint32_t d_tim_count;
    float speed_m_s;

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        switch (osMessageQueueGet(q_encoder_int_messageHandle, &interrupt_message, NULL,
                                  max_timeout_RTOS)) {

            case osErrorTimeout:

                reset_data_all();

                break;

            default:
                reset_data_single(&interrupt_message, last_interrupt_messages,
                                   max_timeout_rear, max_timeout);

                d_tim_count = interrupt_message.tim_count
                              - last_interrupt_messages[interrupt_message.pin].tim_count;

                if (interrupt_message.pin == REAR_RIGHT || interrupt_message.pin == REAR_LEFT) {
                    if (d_tim_count < min_count_rear) {
                        continue;
                    }
                } else {
                    if (d_tim_count < min_count) {
                        continue;
                    }
                }

                // 1. Cálculo da velocidade exata (float m/s)
                if(interrupt_message.pin == REAR_RIGHT || interrupt_message.pin == REAR_LEFT){
                    speed_m_s = calculate_speed_m_s_rear(d_tim_count, tim_freq, tim_presc);
                }else{
                    speed_m_s = calculate_speed_m_s(d_tim_count, tim_freq, tim_presc);
                }

                // Compensation logic aplicada em ambas as variáveis para manter coerência
                if (last_speed_m_s[interrupt_message.pin] > MIN_SPEED_FOR_COMPENSATION_M_S) {
                    
                    uint32_t percentage_ratio = (uint32_t)((speed_m_s * 100.0f) / last_speed_m_s[interrupt_message.pin]);

                    if (percentage_ratio >= 40 && percentage_ratio <= 60) {
                        speed_m_s = speed_m_s * 2.0f;
                    }
                    else if (percentage_ratio >= 25 && percentage_ratio < 40) {
                        speed_m_s = speed_m_s * 3.0f;
                    }
                }

                

                float speed_variation = speed_m_s - last_speed_m_s[interrupt_message.pin];
                dif = speed_variation;
                if (fabsf(speed_variation) > MAX_SPEED_VARIATION_M_S && last_speed_m_s[interrupt_message.pin] != 0.0f) {
                    
                    last_interrupt_messages[interrupt_message.pin] = interrupt_message;
                    continue;
                }

                float delta_time_s = (float)d_tim_count * (float)tim_presc / (float)tim_freq;

                accelerations_message.wheels[interrupt_message.pin] = calc_acceleration(speed_m_s, last_speed_m_s[interrupt_message.pin], delta_time_s);

                last_speed_m_s[interrupt_message.pin] = speed_m_s;
                last_interrupt_messages[interrupt_message.pin] = interrupt_message;

                speeds_message.wheels[interrupt_message.pin] = (uint32_t)(speed_m_s * 3.6f * 10.0f);

                break;
        }
        osMessageQueuePutOverwrite(q_encoder_speeds_messageHandle, &speeds_message, 0);
        osMessageQueuePutOverwrite(q_encoder_accelerations_messageHandle, &accelerations_message, 0);

    }
}
