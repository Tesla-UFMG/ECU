/*
 * initializers.c
 *
 *  Created on: 11 de mai de 2020
 *      Author: renanmoreira
 */

#include "util/initializers.h"

#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "CAN/inverter_can.h"
#include "cmsis_os.h"
#include "main.h"
#include "stm32h7xx.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/constants.h"
#include "util/global_definitions.h"
#include "util/global_variables.h"

// inicializa prioridade dos ISRs para permitir chamada da API do RTOS de dentro dos ISRs
//  mantendo a prioridade maxima de ISRs
void init_NVIC_priorities() {
    NVIC_SetPriority(S_VEL1_EXTI_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(S_VEL2_EXTI_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(S_VEL3_EXTI_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(S_VEL4_EXTI_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY);
}

void init_ADC_DMA(ADC_HandleTypeDef* hadc) {
    HAL_ADC_Start_DMA(hadc, (uint32_t*)ADC_DMA_buffer, ADC_LINES);
}

extern uint8_t error_count;
cores_t led_conf;

uint8_t error_count  = 0; // conta erros, quantas vezes o programa caiu no error handler
uint16_t debug_milis = 0, debug_milis_ant = 0;

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

// inicializar a CAN, chamada na main.c. Outra funcao que inicializa o periferico da CAN
// na main.c
void init_CAN() {
    initialize_inverter_CAN(&hfdcan1);
    initialize_general_CAN(&hfdcan2);
    initialize_CAN_IDs();
}

void inicializa_modos() {
    enduro.tor_max          = 2500;
    enduro.vel_max          = vel_max_rpm;
    enduro.freio_regen      = frenagem_regenerativa;
    enduro.dif_elt          = 0;
    enduro.traction_control = 0;
    enduro.bat_safe         = 1;
    enduro.torq_gain        = 25; // ERA 15
    enduro.mode             = ENDURO;
    enduro.cor              = VERDE;

    aceleracao.tor_max          = 3500;
    aceleracao.vel_max          = vel_max_rpm;
    aceleracao.freio_regen      = 0;
    aceleracao.dif_elt          = 0;
    aceleracao.traction_control = 1;
    aceleracao.bat_safe         = 0;
    aceleracao.torq_gain        = 30;
    aceleracao.mode             = ACELERACAO;
    aceleracao.cor              = ROXO;

    skidpad.tor_max          = 3000;
    skidpad.vel_max          = vel_max_rpm;
    skidpad.freio_regen      = 0;
    skidpad.dif_elt          = 0;
    skidpad.traction_control = 0;
    skidpad.bat_safe         = 0;
    skidpad.torq_gain        = 20;
    skidpad.mode             = SKIDPAD;
    skidpad.cor              = CIANO;

    autox.tor_max          = 3000;
    autox.vel_max          = vel_max_rpm;
    autox.freio_regen      = 0;
    autox.dif_elt          = 0;
    autox.traction_control = 1;
    autox.bat_safe         = 0;
    autox.torq_gain        = 25;
    autox.mode             = AUTOX;
    autox.cor              = AZUL;

    erro.tor_max          = 0;
    erro.vel_max          = 0;
    erro.freio_regen      = 0;
    erro.dif_elt          = 0;
    erro.traction_control = 0;
    erro.bat_safe         = 0;
    erro.torq_gain        = 0;
    erro.mode             = ERRO;
    erro.cor              = VERMELHO;

    set_global_var_value(SELECTED_MODE, enduro); // inicializa no modo enduro
}
