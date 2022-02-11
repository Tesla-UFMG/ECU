/*
 * main_task.c
 *
 *  Created on: Jun 3, 2020
 *      Author: renanmoreira
 */

#include "util/main_task.h"
#include "util/global_variables.h"
#include "util/global_instances.h"
#include "cmsis_os.h"
#include "driver_settings/RTD.h"
#include "leds/debug_leds_handler.h"
#include "leds/rgb_led_handler.h"
#include "sensors/APPS.h"
#include "util/util.h"
#include "util/CMSIS_extra/global_variables_handler.h"



void main_task(void *argument) {
	UNUSED(argument);

    for(;;) {

        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        // espera RTD ser acionado. Por meio da RTD_FLAG
        osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, osWaitForever);

        // espera por qualquer erro
        osThreadFlagsWait(ALL_ERRORS_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
        // obtem os valores de flag de thread e de evento
        uint32_t error_flags = osThreadFlagsGet();
        uint32_t event_flags = osEventFlagsGet(ECU_control_event_id);
        // obtem a flag de thread mais significativa, ela que sera tratada
        uint32_t most_significant_error_flags = get_flag_MSB(error_flags & ALL_ERRORS_FLAG);
        bool isErrorPresent;
        switch (most_significant_error_flags) {

            // O erro de bus off da can do inversor so sera tratado se ocorrer mais de uma vez em um
            // curto periodo de tempo (tempo definido por : BUS_OFF_ERROR_TIME), assim o RTD sera
            // desabilitado somente se tiver o erro frequente na CAN.
            case INVERTER_BUS_OFF_ERROR_FLAG:
                //verifica se o erro esta presente na flag de evento, caso esteja sai de RTD
                isErrorPresent = event_flags & INVERTER_BUS_OFF_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                } else {
                    // caso o erro nao esteja presente a flag de evento sera setada e um timer
                    // iniciado para que caso tenha o erro novamente saia de RTD
                    osEventFlagsSet(ECU_control_event_id, INVERTER_BUS_OFF_ERROR_FLAG);
                    osThreadFlagsClear(INVERTER_BUS_OFF_ERROR_FLAG);
                    osTimerStart(tim_inverter_BUS_OFF_errorHandle, BUS_OFF_ERROR_TIME);
                }
                break;

            case INVERTER_COMM_ERROR_FLAG:
                // verifica se o erro ainda esta presente na flag de evento, caso esteja sai de RTD,
                // caso não esteja a flag de thread é resetada
                isErrorPresent = event_flags & INVERTER_COMM_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                } else {
                    osThreadFlagsClear(INVERTER_COMM_ERROR_FLAG);
                }
                break;

            case SU_F_ERROR_FLAG:
                // verifica se o erro ainda esta presente na flag de evento, caso esteja sai de RTD,
                // caso não esteja a flag de thread é resetada e o led volta ao normal
                isErrorPresent = event_flags & SU_F_ERROR_FLAG;
                if (isErrorPresent) {
                    exit_RTD();
                } else {
                    osThreadFlagsClear(SU_F_ERROR_FLAG);
                }
                break;

            case APPS_ERROR_FLAG:       //Regulamento: T.4.2 (2021)

                isErrorPresent = event_flags & APPS_ERROR_FLAG;
                if (isErrorPresent) {
                    set_rgb_led(AMARELO, NO_CHANGE);
                    osDelay(20);
                } else {
                    osThreadFlagsClear(APPS_ERROR_FLAG);
                    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, NO_CHANGE);
                }
                break;

            case BSE_ERROR_FLAG:        //Regulamento: EV.5.7 (2021)
                // verifica se o erro ainda esta presente na flag de evento, caso esteja seta o led
                // como amarelo, caso não esteja a flag de thread é resetada e o led volta ao normal
                isErrorPresent = event_flags & BSE_ERROR_FLAG;
                if (isErrorPresent) {
                    set_rgb_led(AMARELO, NO_CHANGE);
                    osDelay(20);
                } else {
                    osThreadFlagsClear(BSE_ERROR_FLAG);
                    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, NO_CHANGE);
                }
                break;

            default:
                osDelay(100);
                break;
        }
    }
}

