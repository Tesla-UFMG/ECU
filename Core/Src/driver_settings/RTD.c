/*
 * RTD.c
 *
 *  Created on: Aug 15, 2021
 *      Author: Felipe Telles
 */

#include "driver_settings/RTD.h"

#include "leds/debug_leds_handler.h"
#include "leds/rgb_led_handler.h"
#include "main.h"
#include "stdbool.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_instances.h"
#include "util/global_variables.h"
#include "util/util.h"

static void activate_RTDS();
static bool can_RTD_be_enabled();
static void set_RTD();

void RTD(void* argument) {
    UNUSED(argument);

    // seta o led rgb no primeira execucao do codigo
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, BLINK200);

    for (;;) {

        // espera receber flag q o botao de RTD foi pressionado
        //osThreadFlagsWait(RTD_BTN_PRESSED_THREAD_FLAG, osFlagsWaitAny, osWaitForever);

        if (!is_RTD_active()) {
            if (can_RTD_be_enabled()) {
                set_RTD();
            } else {
                // envia uma mensagem de alerta caso n seja possivel acionar RTD
                set_debugleds(DEBUGLED1, BLINK, 2);
            }
        }
    }
}

void exit_RTD() {
    // seta modo_selecionado como erro
    set_global_var_value(SELECTED_MODE, erro);
    set_global_var_value(RACE_MODE, ERRO);
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, BLINK200);
    // limpa flag de RTD
    osEventFlagsClear(e_ECU_control_flagsHandle, RTD_FLAG);
    osThreadFlagsSet(t_odometer_saveHandle, ODOMETER_SAVE_THREAD_FLAG);
}

/*
 * According to the rules EV.10.4.3 (2022) the only verifications needed to enable RTD are
 * if the brake is active and TS is active. As an added safety, others verifications are
 * made:
 *
 *      - is_brake_active: Allow RTD activation when the brake pedal is being pressed.
 *
 *      - !is_throttle_active: Allow RTD activation only if the throttle pedal is not
 * being pressed. Prevents an unexpected accelaration of the vehicle, as the driver should
 * not be pressing the pedal before RTD activation
 *
 *      - !error_flags &= ALL_SEVERE_ERROR_FLAG: Allow RTD activation if no error that
 * disables RTD is present
 *
 *      - race_mode != ERRO: Allow RTD activation after the driver has made an action to
 * ignore the error. The vehicle will be on race_mode ERRO after a severe error has
 * disabled RTD, the driver must then switch modes again to enable RTD.
 *
 *      - is_inverter_ready: Allow RTD activation after the precharge period is over and
 * both AIRs have been closed. According to the rules RTD must only be possible after TS
 * is active (which only means the shutdown circuit is closed) It also defines RTD as when
 * "the Motors will respond to the input of the APPS". So by the definition the RTD cannot
 * be enabled until both AIRs have been closed. This verification should be made by
 * reading the status of the AIRs sended by the vehicle BMS, which is not currently
 * working. While the BMS cannot make this reading, this verification will enable RTD
 * activation after a timer which is started when the inverter sends its first message.
 *      TODO: Allow RTD activation from the status check of the AIRs.
 */
static bool can_RTD_be_enabled() {
    // obtem todas as flags e filtra apenas flags de erros severos, ignorando as outras
    uint32_t error_flags = osEventFlagsGet(e_ECU_control_flagsHandle);
    error_flags &= ALL_SEVERE_ERROR_FLAG;
    BRAKE_STATUS_t is_brake_active       = get_global_var_value(BRAKE_STATUS);
    THROTTLE_STATUS_t is_throttle_active = get_global_var_value(THROTTLE_STATUS);
    RACE_MODE_t race_mode                = get_global_var_value(RACE_MODE);
    // flag that indicates when the inverter precharge time has passed and the inverter is
    // ready
    bool is_inverter_ready =
        get_individual_flag(e_ECU_control_flagsHandle, INVERTER_READY_FLAG);
    if (/*is_brake_active &&*/ !is_throttle_active /*&& !error_flags*/ && (race_mode != ERRO)
        && is_inverter_ready) {
        return true;
    }
    return false;
}

static void set_RTD() {
    osEventFlagsSet(e_ECU_control_flagsHandle, RTD_FLAG);
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, FIXED);
    activate_RTDS();
}

// Ready to drive sound. As defined by FSAE Rules: EV.10.5 (2023)
static void activate_RTDS() {
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_SET);
    osDelay(RTDS_TIME_MS);
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_RESET);
}
