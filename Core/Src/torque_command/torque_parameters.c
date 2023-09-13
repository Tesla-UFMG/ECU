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
#include "CAN/inverter_can_data_manager.h"
#include "stdio.h"

void update_regen_state(vehicle_state_e vehicle_state);

extern osMessageQueueId_t q_ref_torque_messageHandle;
extern UART_HandleTypeDef hlpuart1;

volatile vehicle_state_parameters_t g_vehicle_state_parameters;

volatile vehicle_state_e vehicle_state;

// Regen brake tests variables
uint16_t MAX_REG_BRAKING_TORQUE = 1000;
uint16_t regen_power = 3000*10;
#define equation_constant 45.22
uint16_t REG_BRAKING_TORQUE = 0;
uint16_t max_speed = 0;
bool regenerating = false;

void constant_power_torque_calc()
{
	max_speed = max(inverter_get_value(speed_m_l), inverter_get_value(speed_m_r));
	REG_BRAKING_TORQUE = ((regen_power*equation_constant)/max_speed);
	REG_BRAKING_TORQUE = min(REG_BRAKING_TORQUE, MAX_REG_BRAKING_TORQUE);
}

//uint16_t regenerative_cc_current = 0;
//bool speed_condition = false;

int digit_counter (uint16_t number)
{
    int counter = 0;
    if (number == 0)
        return 1;
    while (number != 0)
    {
        number /= 10;
        counter++;
    }
    // printf("contdor parcial %d\n", contador);
    return counter;
}

//uint16_t two_complement_transform(uint16_t number)
//{
//	uint16_t bits_size = 16;
//	uint16_t max_pos_value = (1 << (bits_size-1)) -1;
//
//	if (number & (1<<(bits_size -1)))
//	{
//		uint16_t absolute_value = (~number+1)&max_pos_value;
//		return absolute_value;
//	}
//	else
//	{
//		return number;
//	}
//}

void update_state(bool disable) {
    if (disable == true) {
        vehicle_state = S_DISABLE_E;
        //speed_condition = false;
        REG_BRAKING_TORQUE = 0;
        max_speed = 0;
    } else if ((get_global_var_value(THROTTLE_PERCENT) < 100)
               && (frenagem_regenerativa == true)
               && get_global_var_value(REAR_AVG_SPEED) > 1200) {
        vehicle_state = S_BRAKE_E;
        constant_power_torque_calc();
        //speed_condition = true; //teste pra ver se ta entrando

    } else if (get_global_var_value(THROTTLE_PERCENT) > 100) {
        vehicle_state = S_ACCELERATE_E;
        //speed_condition = false;
        REG_BRAKING_TORQUE = 0;
        max_speed = 0;
    } else {
        vehicle_state = S_NEUTER_E;
        //speed_condition = false;
        REG_BRAKING_TORQUE = 0;
        max_speed = 0;
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
            torque_message->neg_torque_ref[R_MOTOR] = REG_BRAKING_TORQUE/*100*/;
            torque_message->neg_torque_ref[L_MOTOR] = REG_BRAKING_TORQUE/*100*/;
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
    char buffer [35] = {0};

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
                //regenerative_cc_current = calculate_inverter_cc_current();

                uint16_t int_left_motor_rpm = inverter_get_value(speed_m_l) /*1000*/;
                uint16_t int_right_motor_rpm = inverter_get_value(speed_m_r) /*2000*/;
                uint16_t int_left_motor_torque = inverter_get_value(torque_m_l) /*300*/;
                uint16_t int_right_motor_torque = inverter_get_value(torque_m_r) /*4000*/;
                uint16_t int_neg_ref = REG_BRAKING_TORQUE /*2000*/;
                int cont1 = digit_counter(int_left_motor_rpm);
                int cont2 = digit_counter(int_right_motor_rpm);
                int cont3 = digit_counter(int_left_motor_torque);
                int cont4 = digit_counter(int_right_motor_torque);
                int cont5 = digit_counter(int_neg_ref);
                int total_cont = cont1 + cont2 + cont3 + cont4+ cont5;
                snprintf(buffer, (total_cont +6), "%u,%u,%u,%u,%u-", int_left_motor_rpm, int_right_motor_rpm, int_left_motor_torque, int_right_motor_torque, int_neg_ref);
                HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, (total_cont+5), 50);

                if ((inverter_get_value(current_m_l) & (1<<(15))&& (inverter_get_value(current_m_r) & (1<<15))))
                {
                	regenerating = true;
                }
                else
                {
                	regenerating = false;
                }

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
