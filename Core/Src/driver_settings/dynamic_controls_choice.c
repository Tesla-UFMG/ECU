/*
 * choose_dynamic_controls.c
 *
 *  Created on: 7 de jul de 2022
 *      Author: JoaoPedroNunes
 */

#include "driver_settings/dynamic_controls_choice.h"

#include "cmsis_os.h"
#include "driver_settings/RTD.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/buttons_handler.h"
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

        osThreadFlagsWait(DYNAMIC_CONTROL_CHOICE_BTN_PRESSED_FLAG, osFlagsWaitAny,
                          osWaitForever);

        bool is_RTD_active = get_individual_flag(e_ECU_control_flagsHandle, RTD_FLAG);
        if (!is_RTD_active) {

            DYNAMIC_CONTROL_t selected_control = get_global_var_value(DYNAMIC_CONTROL);

            switch (get_global_var_value(SELECTED_MODE).traction_control) {

                case 1:
                    ++selected_control;

                    if (selected_control > 1) {
                        selected_control = 0;
                    }
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
            }
            switch (get_global_var_value(SELECTED_MODE).dif_elt) {

                case 1:
                    ++selected_control;

                    if (selected_control == 1) {
                        selected_control = 3;
                    }
                    if (selected_control > 3) {
                        selected_control = 0;
                    }
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
            }
        }
    }
}
