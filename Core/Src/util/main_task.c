/*
 * main_task.c
 *
 *  Created on: Jun 3, 2020
 *      Author: renanmoreira
 */
//#include <string.h>
#include "util/main_task.h"

#include "cmsis_os.h"
#include "driver_settings/RTD.h"
#include "leds/debug_leds_handler.h"
#include "leds/rgb_led_handler.h"
#include "sensors/APPS.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"
#include "util/global_definitions.h"


void set_the_pattern(cores_t *pattern){
		set_rgb_led(PRETO,FIXED, NULL);
		set_rgb_led(PRETO,BLINK500,pattern);
}

void myMemCpy(void *dest, void *src, size_t n)
{
char *csrc = (char *)src;
char *cdest = (char *)dest;

for (int i=0; i<n; i++)
    cdest[i] = csrc[i];
}

#define pattern_table(c1, c2, c3) ((cores_t[]){c1, c2, c3})

void led_color_response(uint32_t flag){

    cores_t pattern[3] = {};

    switch(flag) {
        // Hard error
        case INVERTER_BUS_OFF_ERROR_FLAG:
        	myMemCpy(pattern, pattern_table(VERMELHO, AZUL, BRANCO), sizeof(pattern));
            break;
        case INVERTER_CAN_TRANSMIT_ERROR_FLAG:
        	myMemCpy(pattern, pattern_table(VERMELHO, AZUL, AMARELO), sizeof(pattern));
            break;
        case INVERTER_COMM_ERROR_FLAG:
        	myMemCpy(pattern, pattern_table(VERMELHO, AZUL, VERDE), sizeof(pattern));
            break;
        case SU_F_ERROR_FLAG:
        	myMemCpy(pattern, pattern_table(VERMELHO, BRANCO, BRANCO), sizeof(pattern));
            break;
        // Soft error
        case APPS_ERROR_FLAG:
        	myMemCpy(pattern, pattern_table(AMARELO, VERDE, BRANCO), sizeof(pattern));
            break;
        case BSE_ERROR_FLAG:
        	myMemCpy(pattern, pattern_table(AMARELO, BRANCO, ROXO), sizeof(pattern));
            break;
    }
    set_the_pattern(pattern);
}

void main_task(void* argument) {

    UNUSED(argument);


    for (;;){


        ECU_ENABLE_BREAKPOINT_DEBUG();

        wait_for_rtd();

        // Wait for any error
        osThreadFlagsWait(ALL_ERRORS_FLAG, osFlagsWaitAny | osFlagsNoClear,
                          osWaitForever);
        // Get the most significant thread flag
        uint32_t most_significant_error_flag = get_most_significant_thread_flag();
        // Get the event flag
        uint32_t event_flags = osEventFlagsGet(e_ECU_control_flagsHandle);

        bool isErrorPresent;
        switch (most_significant_error_flag) {

                // BUSOFF error is treated when it happens more than once in a short
                // period of time (period defined by: BUS_OFF_ERROR_TIME).The car leaves
                // RTD mode only if there is a frequent CAN error.
            case INVERTER_BUS_OFF_ERROR_FLAG:

                // If the event flag contains the error flag the car leaves RTD mode.
                isErrorPresent = event_flags & INVERTER_BUS_OFF_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                    led_color_response(most_significant_error_flag);
                } else {
                    // Starts a timer. If the error happens again before the timer expires
                    // the car leaves RTD mode.
                    osEventFlagsSet(e_ECU_control_flagsHandle,
                                    INVERTER_BUS_OFF_ERROR_FLAG);
                    osThreadFlagsClear(INVERTER_BUS_OFF_ERROR_FLAG);
                    osTimerStart(tim_inverter_BUS_OFF_errorHandle, BUS_OFF_ERROR_TIME);
                }
                break;

            case INVERTER_CAN_TRANSMIT_ERROR_FLAG:
            case INVERTER_COMM_ERROR_FLAG:
            case SU_F_ERROR_FLAG:
                // If the event flag contains the error flag the car leaves RTD mode.
                isErrorPresent = event_flags & most_significant_error_flag;
                if (isErrorPresent) {
                    exit_RTD();
                    led_color_response(most_significant_error_flag);
                } else {
                    // Clear the thread flag
                    osThreadFlagsClear(most_significant_error_flag);
                }
                break;

            case APPS_ERROR_FLAG: // FSAE Rules: T.4.2 (2021)
            case BSE_ERROR_FLAG:  // FSAE Rules: EV.5.7 (2021)

                // If the event flag contains the error flag ECU led is set to yellow
                isErrorPresent = event_flags & most_significant_error_flag;
                if (isErrorPresent) {
                	led_color_response(most_significant_error_flag);
                    osDelay(20);
                } else {
                    // Clear the thread flag and set ECU led to normal
                    osThreadFlagsClear(most_significant_error_flag);
                    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, NO_CHANGE, NULL);
                }
                break;

            default: osDelay(100); break;
        }
    }
}
