/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_command/torque_manager.h"

#include "cmsis_os.h"
#include "dynamic_controls/lateral_control.h"
#include "dynamic_controls/longitudinal_control.h"
#include "stdint.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"
#include "main.h"
#include "string.h"

void rampa_torque(uint32_t * ref_torque, const double* ref_torque_decrease);
void send_ref_torque_message(const uint32_t* ref_torque);
void select_dynamic_control(bool is_DYNAMIC_CONTROL_active);
extern UART_HandleTypeDef huart1;
extern osMessageQueueId_t q_ref_torque_messageHandle;
E_CONTROL_TYPE g_control_type = NO_CONTROL;

// normal ramp
void rampa_torque(uint32_t* ref_torque, const double* ref_torque_decrease) {
    static uint32_t ref_torque_ant[2] = {0, 0};
    double desired_torque[2];
    bool should_decrease = (ref_torque_decrease != NULL);

    //Torque requested by the pilot.
    //double torque = ((double)(get_global_var_value(SELECTED_MODE).torq_gain
                            //  * get_global_var_value(THROTTLE_PERCENT))
                   //  / 10);

    // Maps throttle_percent [0,1] to torque range [0, tor_max].
    double torque = (double) (get_global_var_value(SELECTED_MODE).tor_max
                             *get_global_var_value(THROTTLE_PERCENT)) /1000;



    //Torque after the decrease of the dynamic control, if it is active. If the control is not active, the torque is not decreased.
    desired_torque[R_MOTOR] = (uint32_t)(max(
        0, (float)(torque - (should_decrease ? ref_torque_decrease[R_MOTOR] : 0))));
    desired_torque[L_MOTOR] = (uint32_t)(max(
        0, (float)(torque - (should_decrease ? ref_torque_decrease[L_MOTOR] : 0))));

    for (int i = 0; i < 2; i++) {
    	/*checks whether the reference passed the inflection point and applies the
    	 * aggressive increment(INC_TORQUE). Otherwise, uses the initial
    	 * ramp increment(INC_TORQUE_INIT)*/
        uint32_t torque_increment = (ref_torque_ant[i] > TORQUE_INIT_LIMITE) ? INC_TORQUE : INC_TORQUE_INIT;
        ref_torque[i] = min(desired_torque[i], (ref_torque_ant[i] + torque_increment));
        ref_torque_ant[i] = ref_torque[i];
    }
}


// sends the torque message
void send_ref_torque_message(const uint32_t* ref_torque) {
    ref_torque_t ref_torque_message;
    ref_torque_message.ref_torque[R_MOTOR] = ref_torque[R_MOTOR];
    ref_torque_message.ref_torque[L_MOTOR] = ref_torque[L_MOTOR];

    osMessageQueuePut(q_ref_torque_messageHandle, &ref_torque_message, 0, 0U);
}


void select_dynamic_control(bool is_DYNAMIC_CONTROL_active) {

    if (is_DYNAMIC_CONTROL_active) {
        if (get_global_var_value(SELECTED_MODE).dif_elt == 1
            && get_global_var_value(SELECTED_MODE).traction_control == 0) {
            g_control_type = LATERAL;
        }
        if (get_global_var_value(SELECTED_MODE).dif_elt == 0
            && get_global_var_value(SELECTED_MODE).traction_control == 1) {
            g_control_type = LONGITUDINAL;
        }
        //TODO (Guilherme): Abaixo está uma das possíveis alterações a serem feitas para a integração dos controles.
        if (get_global_var_value(SELECTED_MODE).dif_elt == 1
            && get_global_var_value(SELECTED_MODE).traction_control == 1) {
            g_control_type = BOTH_CONTROLS;
        }
    }
}
PacoteHIL_t pac_teste;
PacoteHILSend_t pac_teste2;
void torque_manager(void* argument) {
    UNUSED(argument);
    uint32_t ref_torque[2] = {0, 0};
    //tamanho do pacoteHILSend + 2 para o header e o terminator
    uint8_t tx_data[sizeof(PacoteHILSend_t)+2];   
    PacoteHILSend_t pacote_mandado; //dado que eu vou mandar
    PacoteHIL_t pacote_recebido; //dado que eu vou receber
    tx_data[0] = 'S';
    tx_data[sizeof(PacoteHILSend_t)+1] = '\n';
    for (;;) {
        // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
        uint32_t tick = osKernelGetTickCount();
        ECU_ENABLE_BREAKPOINT_DEBUG();
        //essa parte aqui eu só vou usar no debug por enquanto, se tudo der certo, implementar em outros lugares não deve ser dificil
        pacote_recebido = dados_planta;
        pac_teste = pacote_recebido;
        const bool is_DYNAMIC_CONTROL_active =
            get_individual_flag(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);
        select_dynamic_control(is_DYNAMIC_CONTROL_active);

        // todo (João Pedro): add new "case's" when the integration of controls is
        // implemented
        // PacoteHIL_t dados_planta;
        // // Após receber os 7 bytes úteis entre 'S' e '\n' no buffer rx_buffer:
        // memcpy(&dados_planta, &rx_buffer[1], sizeof(PacoteHIL_t));
        switch (g_control_type) {
            case LATERAL: // TODO(giovanni): do the integration of the two controllers
                tick += LATERAL_DELAY;
                lateral_result_t result_lateral = lateral_control();
                // TODO(giovanni): use rampa_torque while longitudinal control is not defined
                rampa_torque(ref_torque, result_lateral.torque_decrease);

                // sends the reference torque
                send_ref_torque_message(ref_torque);

                osDelayUntil(tick);

                break;

            case LONGITUDINAL:
                tick += LONGITUDINAL_DELAY;
                longitudinal_control_result_t result_longitudinal = longitudinal_control();
                // TODO(giovanni): remove ramp with bench tests
                rampa_torque(ref_torque, result_longitudinal.torque_decrease);
                // sends the torque command to the inverter
                send_ref_torque_message(ref_torque);

                osDelayUntil(tick);

                break;
            
            //TODO (Guilherme): Abaixo está mais uma das possíveis alterações a serem feitas para a integração dos controles. 
            //                  Essa parte pode se tornar mais complexa caso o delay dos controles seja diferente então é necessário ter mais cuidado.
            //                  Nesse sentido, a minha sugestão é apenas uma possibilidade simples de implementação que só funciona caso os delays sejam iguais.
            
            case BOTH_CONTROLS:
                tick += LATERAL_DELAY;//Could be longitudinal as well (since they are equal)
                lateral_result_t lateral_result = lateral_control();
                longitudinal_control_result_t longitudinal_result = longitudinal_control();
                // TODO (Guilherme): Pode ser interressante criar um tipo para armazenar os resultados dos dois controles, isso é puramente estético mas tornaria o código mais intuitivo.
                lateral_result_t result;
                result.torque_decrease[R_MOTOR] = lateral_result.torque_decrease[R_MOTOR] + longitudinal_result.torque_decrease[R_MOTOR];
                result.torque_decrease[L_MOTOR] = lateral_result.torque_decrease[L_MOTOR] + longitudinal_result.torque_decrease[L_MOTOR];

                rampa_torque(ref_torque, result.torque_decrease);

                // sends the reference torque
                send_ref_torque_message(ref_torque);

                osDelayUntil(tick);

                break;

            default: // torque ramp
                rampa_torque(ref_torque, NULL);

                // sends the reference torque
                send_ref_torque_message(ref_torque);
                //a ideia é que eu vou mandar os dados para cá e daqui mandar para o simulink
                STEERING_WHEEL_t steering_wheel = get_global_var_value(STEERING_WHEEL);
                pacote_mandado.steering_angle = steering_wheel;
                pacote_mandado.torque = 1000;
                pac_teste2 = pacote_mandado;
                memcpy(&tx_data[1],&pacote_mandado, sizeof(PacoteHILSend_t));
                HAL_UART_Transmit_DMA(&huart1, tx_data, sizeof(tx_data));
                osDelay(RAMPA_DELAY);

                break;
        }
    }
}


