/*
 * torque_manager.c
 *
 *  Created on: Dec 7, 2020
 *      Author: renanmoreira
 */

#include "torque_command/torque_manager.h"

#include "CAN/general_can_data_manager.h"
#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "dynamic_controls/lateral_control.h"
#include "dynamic_controls/longitudinal_control.h"
#include "stdint.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/global_definitions.h"
#include "util/global_instances.h"
#include "util/util.h"
#include "util/constants.h"
#include "leds/rgb_led_handler.h"

//static longitudinal_result_t torque_decrease_teste;

void rampa_torque(uint32_t * ref_torque, lateral_result_t* lateral_result, longitudinal_result_t* longitudinal_result);
void send_ref_torque_message(const uint32_t* ref_torque);
void select_dynamic_control(bool is_DYNAMIC_CONTROL_active);

extern osMessageQueueId_t q_ref_torque_messageHandle;

void rampa_torque(uint32_t* ref_torque, lateral_result_t* lateral_result, longitudinal_result_t* longitudinal_result) {
    static uint32_t ref_torque_ant[2] = {0, 0};
    int desired_ref_torque[2];
    bool should_decrease = (lateral_result != NULL || longitudinal_result != NULL);
    uint16_t max_ref_torque = get_global_var_value(SELECTED_MODE).tor_max;

    // Maps throttle_percent [0,1] to torque range [0, tor_max].
    int pilot_ref_torque = ((int)get_global_var_value(SELECTED_MODE).tor_max * get_global_var_value(THROTTLE_PERCENT)) / 1000;
    log_data(ID_PILOT_REF_TORQUE, (uint16_t)pilot_ref_torque);

    //Torque after the decrease of the dynamics controls
    if (should_decrease) {
        if (g_control_type == LATERAL) {
            if (lateral_result != NULL) {
                if (lateral_result->distribute) {
                    if (lateral_result->decrease_wheel == CENTRO) {
                        desired_ref_torque[R_MOTOR] = pilot_ref_torque;
                        desired_ref_torque[L_MOTOR] = pilot_ref_torque;

                        log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500));
                        log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500));

                    } else if (lateral_result->decrease_wheel == ESQUERDA) {
                        //be carefull for not pass the max ref torque
                        desired_ref_torque[L_MOTOR] = pilot_ref_torque - (int)(lateral_result->ref_torque_result/2); 
                        desired_ref_torque[R_MOTOR] = pilot_ref_torque + (int)(lateral_result->ref_torque_result/2);

                        if (desired_ref_torque[R_MOTOR] > max_ref_torque) {
                            desired_ref_torque[L_MOTOR] = desired_ref_torque[L_MOTOR] - (desired_ref_torque[R_MOTOR] - max_ref_torque);
                            desired_ref_torque[R_MOTOR] = max_ref_torque;
                        }
                        if (desired_ref_torque[L_MOTOR] < 0) {
                            desired_ref_torque[L_MOTOR] = 0;
                        }

                        log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[R_MOTOR])));
                        log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[L_MOTOR])));
                        
                    } else if (lateral_result->decrease_wheel == DIREITA) {
                        desired_ref_torque[R_MOTOR] = pilot_ref_torque - (int)(lateral_result->ref_torque_result/2); 
                        desired_ref_torque[L_MOTOR] = pilot_ref_torque + (int)(lateral_result->ref_torque_result/2);

                        if (desired_ref_torque[L_MOTOR] > max_ref_torque) {
                            desired_ref_torque[R_MOTOR] = desired_ref_torque[R_MOTOR] - (desired_ref_torque[L_MOTOR] - max_ref_torque);
                            desired_ref_torque[L_MOTOR] = max_ref_torque;
                        }

                        if (desired_ref_torque[R_MOTOR] < 0) {
                            desired_ref_torque[R_MOTOR] = 0;
                        }

                        log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[R_MOTOR])));
                        log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[L_MOTOR])));

                    }
                } else {//lateral_result->distribute == false
                    if (lateral_result->decrease_wheel == CENTRO) {
                        desired_ref_torque[R_MOTOR] = pilot_ref_torque;
                        desired_ref_torque[L_MOTOR] = pilot_ref_torque;

                        log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500));
                        log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500));

                    } else if (lateral_result->decrease_wheel == ESQUERDA) {
                        desired_ref_torque[L_MOTOR] = pilot_ref_torque - (int)(lateral_result->ref_torque_result); 
                        desired_ref_torque[R_MOTOR] = pilot_ref_torque;

                        if (desired_ref_torque[L_MOTOR] < 0) {
                            desired_ref_torque[L_MOTOR] = 0;
                        }

                        log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[R_MOTOR])));
                        log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[L_MOTOR])));

                    } else if (lateral_result->decrease_wheel == DIREITA) {
                        desired_ref_torque[R_MOTOR] = pilot_ref_torque - (int)(lateral_result->ref_torque_result); 
                        desired_ref_torque[L_MOTOR] = pilot_ref_torque;

                        if (desired_ref_torque[R_MOTOR] < 0) {
                            desired_ref_torque[R_MOTOR] = 0;
                        }

                        log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[R_MOTOR])));
                        log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[L_MOTOR])));
                    }
                }
            } else {
                desired_ref_torque[R_MOTOR] = pilot_ref_torque;
                desired_ref_torque[L_MOTOR] = pilot_ref_torque; 

                log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500));
                log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500));

            }
        } else if (g_control_type == LONGITUDINAL) {
            if (longitudinal_result != NULL) {
                desired_ref_torque[R_MOTOR] = pilot_ref_torque - (int)(longitudinal_result->torque_decrease[R_MOTOR]);
                desired_ref_torque[L_MOTOR] = pilot_ref_torque - (int)(longitudinal_result->torque_decrease[L_MOTOR]);

                if (desired_ref_torque[R_MOTOR] < 0) {
                    desired_ref_torque[R_MOTOR] = 0;
                }

                if (desired_ref_torque[L_MOTOR] < 0) {
                    desired_ref_torque[L_MOTOR] = 0;
                }

                log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[R_MOTOR])));
                log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500 + (pilot_ref_torque - desired_ref_torque[L_MOTOR])));

            } else {
                desired_ref_torque[R_MOTOR] = pilot_ref_torque;
                desired_ref_torque[L_MOTOR] = pilot_ref_torque;

                log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500));
                log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500));

            }
        } else {    
            desired_ref_torque[R_MOTOR] = pilot_ref_torque;
            desired_ref_torque[L_MOTOR] = pilot_ref_torque;

            log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500));
            log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500));
        }
    } else {
        desired_ref_torque[R_MOTOR] = pilot_ref_torque;
        desired_ref_torque[L_MOTOR] = pilot_ref_torque;

        log_data(ID_DC_REF_TORQUE_R, (uint16_t)(2500));
        log_data(ID_DC_REF_TORQUE_L, (uint16_t)(2500));
    }

    for (int i = 0; i < 2; i++) {
        uint32_t torque_increment = (ref_torque_ant[i] > TORQUE_INIT_LIMITE) ? INC_TORQUE : INC_TORQUE_INIT;
        ref_torque[i] = min((uint32_t)desired_ref_torque[i], (ref_torque_ant[i] + torque_increment));
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
            set_rgb_led(BRANCO, FIXED);
        }
        if (get_global_var_value(SELECTED_MODE).dif_elt == 0
            && get_global_var_value(SELECTED_MODE).traction_control == 1) {
            g_control_type = LONGITUDINAL;
            set_rgb_led(BRANCO, FIXED);
        }
        if (get_global_var_value(SELECTED_MODE).dif_elt == 1
            && get_global_var_value(SELECTED_MODE).traction_control == 1) {
            g_control_type = NO_CONTROL;
        }
    } else {
        g_control_type = NO_CONTROL;
    }
    log_data(ID_G_CONTROL_TYPE, (uint16_t)(g_control_type));
}


void torque_manager(void* argument) {
    UNUSED(argument);

    uint32_t ref_torque[2] = {0, 0};

    lateral_result_t result_lateral = {.ref_torque_result = 0,
                                         .decrease_wheel = CENTRO,
                                         .distribute = DISTRIBUTE};
    longitudinal_result_t result_longitudinal = {.torque_decrease = {0, 0}};

    for (;;) {
        // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
        uint32_t tick = osKernelGetTickCount();

        ECU_ENABLE_BREAKPOINT_DEBUG();

        const bool is_DYNAMIC_CONTROL_active = get_individual_flag(e_ECU_control_flagsHandle, DYNAMIC_CONTROL_FLAG);

        select_dynamic_control(is_DYNAMIC_CONTROL_active);

        // TODO(giovanni): do the integration of the two controllers
        switch (g_control_type){
        
        case LATERAL:
            tick =+ LATERAL_DELAY;
            result_lateral = lateral_control();
            break;

        case LONGITUDINAL:
            tick =+ LONGITUDINAL_DELAY;
            result_longitudinal = longitudinal_control();
            break;

        default:
            tick =+ DEFAULT_DELAY;

            result_lateral.ref_torque_result = 0;
            result_lateral.decrease_wheel = CENTRO;

            result_longitudinal.torque_decrease[R_MOTOR] = 0;
            result_longitudinal.torque_decrease[L_MOTOR] = 0;
            break;

        }

        rampa_torque(ref_torque, &result_lateral, &result_longitudinal);

        log_data(ID_REF_TORQUE_L, (uint16_t)(ref_torque[L_MOTOR]));
        log_data(ID_REF_TORQUE_R, (uint16_t)(ref_torque[R_MOTOR]));

        // sends the reference torque
        send_ref_torque_message(ref_torque);

        osDelayUntil(tick);
    }
}
