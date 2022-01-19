/*
 * RTD_handler.c
 *
 *  Created on: Aug 15, 2021
 *      Author: Felipe Telles
 */

#include "RTD.h"
#include "global_variables.h"
#include "main.h"
#include "global_instances.h"
#include "rgb_led.h"
#include "debugleds.h"
#include "stdbool.h"
#include "util.h"
#include "CMSIS_extra/global_variables_handler.h"

void aciona_sirene();
bool can_RTD_be_enabled();
void set_RTD();

void RTD(void *argument) {
    //seta o led rgb no primeira execução do código
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, BLINK200);

    for(;;) {

        osThreadFlagsWait(RTD_BTN_PRESSED_FLAG, osFlagsWaitAny, osWaitForever);     //espera receber flag q o botão de RTD foi pressionado

        bool is_RTD_active = get_individual_flag(ECU_control_event_id, RTD_FLAG);

        if (!is_RTD_active) {
            if(can_RTD_be_enabled())
                set_RTD();                                                  //seta RTD
            else
                set_debugleds(DEBUGLED1,BLINK,2);                           //envia uma mensagem de alerta caso n seja possível acionar RTD
        }
    }
}


void exit_RTD() {
    set_global_var_value(SELECTED_MODE, erro);          //seta modo_selecionado como erro
    set_global_var_value(RACE_MODE, ERRO);
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, BLINK200);
    osEventFlagsClear(ECU_control_event_id, RTD_FLAG);  //limpa flag de RTD
}

/*
 * According to the rules EV.10.4.3 (2022) the only verifications needed to enable RTD are if the brake is active and TS is active.
 * As an added safety, others verifications are made:
 *
 *      - is_brake_active: Allow RTD activation when the brake pedal is being pressed.
 *
 *      - !is_throttle_active: Allow RTD activation only if the throttle pedal is not being pressed. Prevents an unexpected
 *      accelaration of the vehicle, as the driver should not be pressing the pedal before RTD activation
 *
 *      - !error_flags &= ALL_SEVERE_ERROR_FLAG: Allow RTD activation if no error that disables RTD is present
 *
 *      - race_mode != ERRO: Allow RTD activation after the driver has made an action to ignore the error. The vehicle will
 *      be on race_mode ERRO after a severe error has disabled RTD, the driver must then switch modes again to enable RTD.
 *
 *      - is_inverter_ready: Allow RTD activation after the precharge period is over and both AIRs have been closed.
 *      According to the rules RTD must only be possible after TS is active (which only means the shutdown circuit is closed)
 *      It also defines RTD as when "the Motors will respond to the input of the APPS". So by the definition the RTD cannot
 *      be enabled until both AIRs have been closed.
 *      This verification should be made by reading the status of the AIRs sended by the vehicle BMS, which is not currently
 *      working. While the BMS cannot make this reading, this verification will enable RTD activation after a timer which
 *      is started when the inverter sends its first message.
 *      TODO: Allow RTD activation from the status check of the AIRs.
 */
bool can_RTD_be_enabled() {
    uint32_t error_flags = osEventFlagsGet(ECU_control_event_id);                           //obtem todas as flags
    error_flags &= ALL_SEVERE_ERROR_FLAG;                                                   //filtra apenas flags de erros severos, ignorando as outras
    BRAKE_STATUS_t is_brake_active = get_global_var_value(BRAKE_STATUS);
    THROTTLE_STATUS_t is_throttle_active = get_global_var_value(THROTTLE_STATUS);
    RACE_MODE_t race_mode = get_global_var_value(RACE_MODE);
    bool is_inverter_ready = get_individual_flag(ECU_control_event_id, INVERTER_READY);     // flag that indicates when the inverter precharge time has passed and the inverter is ready
    if(is_brake_active && !is_throttle_active &&  !error_flags && (race_mode != ERRO) && is_inverter_ready) {
        return true;
    }
    return false;
}

void set_RTD() {
    osEventFlagsSet(ECU_control_event_id, RTD_FLAG);                                         //Seta flag de RTD
    set_rgb_led(get_global_var_value(SELECTED_MODE).cor, FIXED);
    aciona_sirene();
}

void aciona_sirene() {
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_SET);
    osDelay(tempo_sirene);
    HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_RESET);
}
