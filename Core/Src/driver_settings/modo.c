/*
 * modo.c
 *
 *  Created on: Jul 13, 2021
 *      Author: Felipe Telles
 */

#include "driver_settings/modo.h"

#include "cmsis_os.h"
#include "leds/rgb_led_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

void seleciona_modo(void* argument) {
    UNUSED(argument);

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        // espera um semaforo liberado por interrupcao e espera esta autorizado a mudar de
        // modo
        osThreadFlagsWait(MODE_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);

        bool is_RTD_active = get_individual_flag(e_ECU_control_flagsHandle, RTD_FLAG);
        if (!is_RTD_active) {
            if (get_global_var_value(RACE_MODE) > AUTOX) {
                set_global_var_value(RACE_MODE, ENDURO);
            }

            switch (get_global_var_value(RACE_MODE)) {
                case ENDURO: set_global_var_value(SELECTED_MODE, enduro); break;
                case ACELERACAO: set_global_var_value(SELECTED_MODE, aceleracao); break;
                case SKIDPAD: set_global_var_value(SELECTED_MODE, skidpad); break;
                case AUTOX: set_global_var_value(SELECTED_MODE, autox); break;
                default: set_global_var_value(SELECTED_MODE, erro); break;
            }

            set_rgb_led(get_global_var_value(SELECTED_MODE).cor, BLINK200);
        }
        // todo: dataloggar modos
    }
}
