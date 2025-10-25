/*
 * choose_dynamic_controls.c
 *
 *  Created on: 7 de jul de 2022
 *      Author: JoaoPedroNunes
 */

#include "driver_settings/dynamic_controls_choice.h"

#include "cmsis_os.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

void dynamic_controls_choice(void* argument) {
    UNUSED(argument);

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        osThreadFlagsWait(DYNAMIC_CONTROLS_CHOICE_BTN_PRESSED_THREAD_FLAG, osFlagsWaitAny,
                          osWaitForever);

        if (is_RTD_active()) {
            continue;
        }
        const bool is_DYNAMIC_CONTROL_active =
            get_individual_flag(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);

        if (!is_DYNAMIC_CONTROL_active) {
            osEventFlagsSet(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);
        } else {
            osEventFlagsClear(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);
        }
    }
}
