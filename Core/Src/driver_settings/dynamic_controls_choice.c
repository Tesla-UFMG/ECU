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

void enable_dynamic_controls(void* argument) {
    UNUSED(argument);

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        osThreadFlagsWait(CHOOSE_DYNAMIC_CONTROL_BTN_PRESSED_FLAG, osFlagsWaitAny,
                          osWaitForever);

        bool is_RTD_active = get_individual_flag(e_ECU_control_flagsHandle, RTD_FLAG);
        if (!is_RTD_active) {

            DYNAMIC_CONTROL_t selected_control = get_global_var_value(DYNAMIC_CONTROL);

            if (get_global_var_value(SELECTED_MODE).traction_control == 1
                && get_global_var_value(SELECTED_MODE).dif_elt == 0) {
                if (selected_control == LONGITUDINAL) {
                    selected_control = NO_CONTROL;
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
                }
                if (selected_control == NO_CONTROL) {
                    selected_control = LONGITUDINAL;
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
                }
            }
            if (get_global_var_value(SELECTED_MODE).dif_elt == 1
                && get_global_var_value(SELECTED_MODE).traction_control == 0) {
                if (selected_control == LATERAL) {
                    selected_control = NO_CONTROL;
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
                }
                if (selected_control == NO_CONTROL) {
                    selected_control = LATERAL;
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
                }
            }
            if (get_global_var_value(SELECTED_MODE).dif_elt == 1
                && get_global_var_value(SELECTED_MODE).traction_control == 1) {
                if (selected_control == LONGITUDINAL) {
                    selected_control = NO_CONTROL;
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
                }
                if (selected_control == NO_CONTROL) {
                    selected_control = LATERAL;
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
                }
                if (selected_control == LATERAL) {
                    selected_control = LONGITUDINAL;
                    set_global_var_value(DYNAMIC_CONTROL, selected_control);
                    break;
                }
            }
            if (get_global_var_value(SELECTED_MODE).dif_elt == 0
                && get_global_var_value(SELECTED_MODE).traction_control == 0) {
                selected_control = NO_CONTROL;
                set_global_var_value(DYNAMIC_CONTROL, selected_control);
                break;
            }
        }
    }
}
