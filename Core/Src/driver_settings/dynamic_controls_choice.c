/*
 * dynamic_controls_choice.c
 *
 *  Created on: 7 de jul de 2022
 *      Author: JoaoPedroNunes
 */

#include "driver_settings/dynamic_controls_choice.h"

#include "cmsis_os.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "leds/rgb_led_handler.h"
#include "util/util.h"

static bool teste;

void dynamic_controls_choice(void* argument) {
    UNUSED(argument);

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        if(osThreadFlagsWait(DYNAMIC_CONTROLS_CHOICE_BTN_PRESSED_THREAD_FLAG, osFlagsWaitAny,
                          osWaitForever)){

        	 if (is_RTD_active()) {
        	            continue;
        	 }
        	 const bool is_DYNAMIC_CONTROL_active =
        			 get_individual_flag(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);
        	 teste = is_DYNAMIC_CONTROL_active;

        	 if (!is_DYNAMIC_CONTROL_active) {
        		 osEventFlagsSet(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);
				 set_rgb_led(BRANCO, FIXED);
        	 } else {
        		 osEventFlagsClear(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);
        	 }
        }


    }
}
