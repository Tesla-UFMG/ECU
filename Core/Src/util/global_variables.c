/*
 * global_variables.c
 *
 *  Created on: May 29, 2020
 *      Author: renanmoreira
 */

#include "util/global_variables.h"

#include "util/constants.h"

volatile uint16_t ADC_DMA_buffer[ADC_LINES];

volatile const modos enduro     = {.tor_max          = 1000,
                                   .vel_max          = vel_max_rpm,
                                   .freio_regen      = frenagem_regenerativa,
                                   .dif_elt          = 0,
                                   .traction_control = 0,
                                   .bat_safe         = 1,
                                   .torq_gain        = 10,
                                   .mode             = ENDURO,
                                   .cor              = VERDE};
volatile const modos aceleracao = {.tor_max          = 1500,
                                   .vel_max          = vel_max_rpm,
                                   .freio_regen      = 0,
                                   .dif_elt          = 0,
                                   .traction_control = 1,
                                   .bat_safe         = 0,
                                   .torq_gain        = 25,
                                   .mode             = ACELERACAO,
                                   .cor              = ROXO};
volatile const modos skidpad    = {.tor_max          = 2000,
                                   .vel_max          = vel_max_rpm,
                                   .freio_regen      = 0,
                                   .dif_elt          = 0,
                                   .traction_control = 0,
                                   .bat_safe         = 0,
                                   .torq_gain        = 15,
                                   .mode             = SKIDPAD,
                                   .cor              = CIANO};
volatile const modos autox      = {.tor_max          = 2500,
                                   .vel_max          = vel_max_rpm,
                                   .freio_regen      = 0,
                                   .dif_elt          = 0,
                                   .traction_control = 0,
                                   .bat_safe         = 0,
                                   .torq_gain        = 20,
                                   .mode             = AUTOX,
                                   .cor              = AZUL};
volatile const modos erro       = {.tor_max          = 0,
                                   .vel_max          = 0,
                                   .freio_regen      = 0,
                                   .dif_elt          = 0,
                                   .traction_control = 0,
                                   .bat_safe         = 0,
                                   .torq_gain        = 0,
                                   .mode             = ERRO,
                                   .cor              = VERMELHO};
