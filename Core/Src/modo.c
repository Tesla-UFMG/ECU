/*
 * modo.c
 *
 *  Created on: Jul 13, 2021
 *      Author: Felipe Telles
 */


#include "modo.h"
#include "cmsis_os.h"
#include "global_variables.h"
#include "global_instances.h"
#include "rgb_led.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"


void seleciona_modo(void *argument) {
    for(;;) {

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        //espera um semáforo liberado por interrupção e espera está autorizado a mudar de modo
        osThreadFlagsWait(MODE_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);

        bool is_RTD_active = get_individual_flag(ECU_control_event_id, RTD_FLAG);
        if (!is_RTD_active) {
            RACE_MODE_t race_mode;
            get_global_var(RACE_MODE, &race_mode);
            if (race_mode > AUTOX)
            {
                race_mode = ENDURO;
                set_global_var(RACE_MODE, &race_mode);
            }

            SELECTED_MODE_t selected_mode;
            switch(race_mode) {
                case ENDURO:
                    set_global_var_value(SELECTED_MODE, enduro);
                    break;
                case ACELERACAO:
                    set_global_var_value(SELECTED_MODE, aceleracao);
                    break;
                case SKIDPAD:
                    set_global_var_value(SELECTED_MODE, skidpad);
                    break;
                case AUTOX:
                    set_global_var_value(SELECTED_MODE, autox);
                    break;
                default:
                    set_global_var_value(SELECTED_MODE, erro);
                    break;
                }

            set_rgb_led(selected_mode.cor, BLINK200);

        }
        //todo: dataloggar modos
    }
}
