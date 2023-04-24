/*
 * controle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "torque_command/torque_parameters.h"

#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"

void update_regen_state(vehicle_state_e vehicle_state);

extern osMessageQueueId_t q_ref_torque_messageHandle;

volatile vehicle_state_parameters_t g_vehicle_state_parameters;

volatile vehicle_state_e vehicle_state;
bool teste_velocidade = false;
void update_state(bool disable) {
    if (disable == true) {
        vehicle_state = S_DISABLE_E;
    } else if ((get_global_var_value(THROTTLE_PERCENT) < 100)
               && (frenagem_regenerativa == true)
               && get_global_var_value(REAR_AVG_SPEED) > RPM_KMPH_5) {
        vehicle_state = S_BRAKE_E;
        teste_velocidade = true; //teste pra ver se ta entrando

    } else if (get_global_var_value(THROTTLE_PERCENT) > 100) {
        vehicle_state = S_ACCELERATE_E;
    } else {
        vehicle_state = S_NEUTER_E;
    }

    update_regen_state(vehicle_state);
}

void update_state_parameters(torque_message_t* torque_message) {

    SELECTED_MODE_t selected_mode = get_global_var_value(SELECTED_MODE);
    switch (vehicle_state) {
        case S_NEUTER_E:
            set_bit8(&torque_message->parameters, P_ENABLE, true);
            set_bit8(&torque_message->parameters, P_BRAKE, false);
            SPEEDS_t speeds = get_global_var_value(SPEEDS);
            set_bit8(&torque_message->parameters, P_RUNSTOP,
                     (speeds.wheels[R_MOTOR] > RPM_KMPH_5
                      || speeds.wheels[L_MOTOR] > RPM_KMPH_5));
            torque_message->torque_ref[R_MOTOR]     = 0;
            torque_message->torque_ref[L_MOTOR]     = 0;
            torque_message->neg_torque_ref[R_MOTOR] = 0;
            torque_message->neg_torque_ref[L_MOTOR] = 0;
            torque_message->speed_ref[R_MOTOR]      = selected_mode.vel_max;
            torque_message->speed_ref[L_MOTOR]      = selected_mode.vel_max;
            break;
        case S_BRAKE_E:
            set_bit8(&torque_message->parameters, P_ENABLE, true);
            set_bit8(&torque_message->parameters, P_BRAKE, selected_mode.freio_regen);
            set_bit8(&torque_message->parameters, P_RUNSTOP, true);
            torque_message->torque_ref[R_MOTOR]     = 0;
            torque_message->torque_ref[L_MOTOR]     = 0;
            torque_message->neg_torque_ref[R_MOTOR] = REG_BRAKING_TORQUE;
            torque_message->neg_torque_ref[L_MOTOR] = REG_BRAKING_TORQUE;
            torque_message->speed_ref[R_MOTOR]      = 0;
            torque_message->speed_ref[L_MOTOR]      = 0;
            break;
        case S_ACCELERATE_E:
            set_bit8(&torque_message->parameters, P_ENABLE, true);
            set_bit8(&torque_message->parameters, P_BRAKE, false);
            set_bit8(&torque_message->parameters, P_RUNSTOP, true);
            torque_message->neg_torque_ref[R_MOTOR] = 0;
            torque_message->neg_torque_ref[L_MOTOR] = 0;
            torque_message->speed_ref[R_MOTOR]      = selected_mode.vel_max;
            torque_message->speed_ref[L_MOTOR]      = selected_mode.vel_max;
            break;

        case S_DISABLE_E:
            set_bit8(&torque_message->parameters, P_ENABLE, false);
            set_bit8(&torque_message->parameters, P_BRAKE, false);
            set_bit8(&torque_message->parameters, P_RUNSTOP, false);
            torque_message->torque_ref[R_MOTOR]     = 0;
            torque_message->torque_ref[L_MOTOR]     = 0;
            torque_message->neg_torque_ref[R_MOTOR] = 0;
            torque_message->neg_torque_ref[L_MOTOR] = 0;
            torque_message->speed_ref[R_MOTOR]      = 0;
            torque_message->speed_ref[L_MOTOR]      = 0;
            break;
    }
}

void torque_parameters(void* argument) {
    UNUSED(argument);

    // torque reference message. Includes the desired torque value and if a disable is
    // intended
    ref_torque_t ref_torque_message;
    // complete message to be sent to inverter
    torque_message_t torque_message = {.parameters = 0};

    for (;;) {

#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        bool disable;
        // disable will only be FALSE when RTD_FLAG is setted
        disable = !is_RTD_active();

        switch (osMessageQueueGet(q_ref_torque_messageHandle, &ref_torque_message, 0,
                                  TORQUE_PARAMETERS_DELAY)) {

            case osOK:

                torque_message.torque_ref[R_MOTOR] =
                    ref_torque_message.ref_torque[R_MOTOR];
                torque_message.torque_ref[L_MOTOR] =
                    ref_torque_message.ref_torque[L_MOTOR];

                update_state(disable);
                update_state_parameters(&torque_message);

                osMessageQueuePut(q_torque_messageHandle, &torque_message, 0, 0U);

                log_data(ID_REF_TORQUE_R_MOTOR, torque_message.torque_ref[R_MOTOR]);
                log_data(ID_REF_TORQUE_L_MOTOR, torque_message.torque_ref[L_MOTOR]);

                break;
            case osErrorTimeout:
                update_state(disable);
                update_state_parameters(&torque_message);

                osMessageQueuePut(q_torque_messageHandle, &torque_message, 0, 0U);
                break;
            default: break;
        }
    }
}

/**
 * @brief Set regenerative braking warning flag
 *
 * @param vehicle_state
 */
void update_regen_state(vehicle_state_e vehicle_state) {
    if (vehicle_state == S_BRAKE_E) {
        // se frenagem ativa, seta flag de aviso
        osEventFlagsSet(e_ECU_control_flagsHandle, REGEN_WARN_FLAG);
    } else {
        // se frenagem ativa, limpa flag de aviso
        osEventFlagsClear(e_ECU_control_flagsHandle, REGEN_WARN_FLAG);
    }
}
