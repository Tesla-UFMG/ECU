/*
 * speed_calc.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "sensors/encoder_speed.h"

#include "datalogging/speed.h"
#include "math.h"
#include "stm32h7xx.h"
#include "string.h"
#include "util/CMSIS_extra/cmsis_extra.h"
#include "util/global_instances.h"
#include "util/util.h"

/*
 * distance = circumference divided by the number of teeth
 *          = 2*pi*radius/teeth_number (m)
 * time     = timer value multiplied by the prescaler divided by frequency
 *          = timer*prescaler/frequency (s)
 * speed    = distance/time
 *          = (2*pi*radius/teeth_number)*freq/(presc*timer) (m/s)
 *          = (10*3.6*2*pi*radius/teeth_number)*freq/(presc*timer) (10*km/h)
 */

static void reset_speed_all();
static void reset_speed_single(const encoder_int_message_t* message,
                               const encoder_int_message_t* last_messages,
                               uint32_t min_count);
static inline uint32_t get_tim2_freq();
static inline uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc);
static inline uint32_t calculate_timeout(uint32_t speed);

static encoder_speeds_message_t speeds_message;

void encoder_speed_calc(void) {
    encoder_int_message_t interrupt_message;
    encoder_int_message_t last_interrupt_messages[4];
    // initialize with zeros the last messages buffer
    // NOLINTNEXTLINE
    memset(&last_interrupt_messages, 0, sizeof(encoder_int_message_t) * 4);

    // gets tim2 frequency
    const uint32_t tim_freq = get_tim2_freq();
    //  tim2 prescaler
    const uint32_t tim_presc = htim2.Init.Prescaler + 1;
    // value in tim2 time of the maximum speed which will be calculated
    const uint32_t max_count = calculate_speed(MAX_SPEED, tim_freq, tim_presc);
    // value in tim2 time of the minimum speed which will be calculated
    const uint32_t min_count = calculate_speed(MIN_SPEED, tim_freq, tim_presc);
    // value in timersys time of the minimum speed which will be calculated
    const uint32_t min_timeout = calculate_timeout(MIN_SPEED);

    uint32_t d_tim_count;
    uint32_t speed;

    for (;;) {
        ECU_ENABLE_BREAKPOINT_DEBUG();

        // waits until a message arrives or until timeout
        switch (osMessageQueueGet(q_encoder_int_messageHandle, &interrupt_message, NULL,
                                  min_timeout)) {

            // case the task was called by timeout
            case osErrorTimeout:
                reset_speed_all(); // zeros all wheels speed
                break;

            // case the task was called by a new message arriving
            default:
                // verifies if any wheel is without an interruption for a long time,
                // if yes that wheel speed is zeroed
                reset_speed_single(&interrupt_message, last_interrupt_messages,
                                   min_count);

                // difference between current message and last message timestamp
                d_tim_count = interrupt_message.tim_count
                              - last_interrupt_messages[interrupt_message.pin].tim_count;

                // discards value if d_tim_count results in a speed greater than that
                // configured as maximum
                if (d_tim_count < max_count) {
                    continue;
                }

                speed = calculate_speed(d_tim_count, tim_freq, tim_presc);
                // saves the speed only of the wheel which speed was just calculated
                speeds_message.wheels[interrupt_message.pin] = speed;
                // store message to use in the next iteration
                last_interrupt_messages[interrupt_message.pin] = interrupt_message;
                break;
        }
        osMessageQueuePutOverwrite(q_encoder_speeds_messageHandle, &speeds_message, 0);
    }
}

static void reset_speed_all() {
    for (uint8_t i = 0; i < WHEEL_ENCODERS_AVAILABLE; i++) {
        speeds_message.wheels[i] = 0;
    }
}

static void reset_speed_single(const encoder_int_message_t* message,
                               const encoder_int_message_t* last_messages,
                               uint32_t min_count) {
    for (speed_pin_e i = FIRST_WHEEL; i <= WHEEL_ENCODERS_AVAILABLE; i++) {
        if ((message->tim_count - last_messages[i].tim_count) > min_count) {
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

static inline uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER)
                      * ((float)freq / ((float)presc)) / speed);
}

static inline uint32_t calculate_timeout(uint32_t speed) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_NUMBER)
                      * 1000 / speed);
}
