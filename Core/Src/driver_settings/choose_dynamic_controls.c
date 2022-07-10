/*
 * choose_dynamic_controls.c
 *
 *  Created on: 7 de jul de 2022
 *      Author: JoaoPedroNunes
 */

#include "cmsis_os.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/buttons_handler.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

#include <driver_settings/choose_dynamic_controls.h>

void enable_dynamic_controls(void* argument) {
    UNUSED(argument);

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        osThreadFlagsWait(CHOOSE_DYNAMIC_CONTROL_BTN_PRESSED_FLAG, osFlagsWaitAny,
                          osWaitForever);

        DYNAMIC_CONTROL_t type_control = get_global_var_value(DYNAMIC_CONTROL);

        if (get_global_var_value(SELECTED_MODE).traction_control == 1
            && get_global_var_value(SELECTED_MODE).dif_elt == 0) {
            if (type_control == LONGITUDINAL) {
                type_control = NO_CONTROL;
                break;
            }
            if (type_control == NO_CONTROL) {
                type_control = LONGITUDINAL;
                break;
            }
        }
        if (get_global_var_value(SELECTED_MODE).dif_elt == 1
            && get_global_var_value(SELECTED_MODE).traction_control == 0) {
            if (type_control == LATERAL) {
                type_control = NO_CONTROL;
                break;
            }
            if (type_control == NO_CONTROL) {
                type_control = LATERAL;
                break;
            }
        }
        if (get_global_var_value(SELECTED_MODE).dif_elt == 1
            && get_global_var_value(SELECTED_MODE).traction_control == 1) {
            if (type_control == LONGITUDINAL) {
                type_control = NO_CONTROL;
                break;
            }
            if (type_control == NO_CONTROL) {
                type_control = LATERAL;
                break;
            }
            if (type_control == LATERAL) {
                type_control = LONGITUDINAL;
                break;
            }
        }
        if (get_global_var_value(SELECTED_MODE).dif_elt == 0
            && get_global_var_value(SELECTED_MODE).traction_control == 0) {
            type_control = NO_CONTROL;
            break;
        }
        set_global_var_value(DYNAMIC_CONTROL, type_control);
    }
}
