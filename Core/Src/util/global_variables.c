/*
 * global_variables.c
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */

#include "util/global_variables.h"

#include "util/constants.h"

volatile uint16_t ADC_DMA_buffer[ADC_LINES];

volatile const modes enduro     = {.tor_max          = 1000,
                                   .vel_max          = vel_max_rpm,
                                   .regen_brake      = regenerative_braking,
                                   .dif_elt          = 0,
                                   .traction_control = 0,
                                   .bat_safe         = 1,
                                   .torq_gain        = 10,
                                   .mode             = ENDURO,
                                   .color            = GREEN};
volatile const modes acceleration = {.tor_max        = 1500,
                                   .vel_max          = vel_max_rpm,
                                   .regen_brake      = 0,
                                   .dif_elt          = 0,
                                   .traction_control = 1,
                                   .bat_safe         = 0,
                                   .torq_gain        = 25,
                                   .mode             = ACCELERATION,
                                   .color            = PURPLE};
volatile const modes skidpad    = {.tor_max          = 2000,
                                   .vel_max          = vel_max_rpm,
                                   .regen_brake      = 0,
                                   .dif_elt          = 1,
                                   .traction_control = 0,
                                   .bat_safe         = 0,
                                   .torq_gain        = 15,
                                   .mode             = SKIDPAD,
                                   .color            = CYAN};
volatile const modes autox      = {.tor_max          = 2500,
                                   .vel_max          = vel_max_rpm,
                                   .regen_brake      = 0,
                                   .dif_elt          = 1,
                                   .traction_control = 0,
                                   .bat_safe         = 0,
                                   .torq_gain        = 20,
                                   .mode             = AUTOX,
                                   .color            = BLUE};
volatile const modes error       = {.tor_max         = 0,
                                   .vel_max          = 0,
                                   .regen_brake      = 0,
                                   .dif_elt          = 0,
                                   .traction_control = 0,
                                   .bat_safe         = 0,
                                   .torq_gain        = 0,
                                   .mode             = ERRO,
                                   .color            = RED};
