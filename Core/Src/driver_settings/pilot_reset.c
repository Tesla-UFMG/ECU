/*
 * pilot_reset.c
 *
 *  Created on: Mar 15th  2022
 *      Author: GiovanniPasa
 */

#include "driver_settings/RTD.h"
#include "driver_settings/pilot_reset.h"

#include "leds/debug_leds_handler.h"
#include "leds/rgb_led_handler.h"
#include "main.h"
#include "stdbool.h"

#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"



void pilot_reset(void* argument) {
    UNUSED(argument);
    uint32_t returns[2];

    for (;;) {

        // Waits the mode button to be pressed initially, then waits until a timeout for the combination of buttons to be pressed
        //in the following order: mode, rtd,  mode again, but this time with the brake pressed and with no throttle input.
        //After that the system will be reseted
        osThreadFlagsWait(MODE_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);
        returns[0] =  osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, RESET_BUTTONS_TIMEOUT);
        returns[1] =  osThreadFlagsWait(MODE_BTN_PRESSED_FLAG, osFlagsWaitAny, RESET_BUTTONS_TIMEOUT);

        if(returns[0] == osFlagsErrorTimeout || returns[1] == osFlagsErrorTimeout) continue;

        BRAKE_STATUS_t is_brake_active  = get_global_var_value(BRAKE_STATUS);
        THROTTLE_STATUS_t is_throttle_active = get_global_var_value(THROTTLE_STATUS);
        if(is_brake_active && !is_throttle_active){
            HAL_NVIC_SystemReset();
        }


    }
}
