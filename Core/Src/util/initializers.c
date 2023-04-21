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
#include "dynamic_controls/initializer_controls.h"
#include "main.h"
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
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
colors_t led_conf;

uint8_t error_count  = 0; // conta erros, quantas vezes o programa caiu no error handler
uint16_t debug_milis = 0, debug_milis_ant = 0;

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef hlpuart1;
extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c3;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

// inicializar a CAN, chamada na main.c. Outra funcao que inicializa o periferico da CAN
// na main.c
void init_CAN() {
    initialize_inverter_CAN(&hfdcan1);
    initialize_general_CAN(&hfdcan2);
    initialize_CAN_IDs();
}

void init_modes() {
    enduro.tor_max          = 1000;
    enduro.vel_max          = VEL_MAX_RPM;
    enduro.freio_regen      = REGENERATIVE_BRAKING;
    enduro.dif_elt          = 0;
    enduro.traction_control = 0;
    enduro.bat_safe         = 1;
    enduro.torq_gain        = 10; // ERA 15
    enduro.mode             = ENDURO;
    enduro.rgbColor[0]      = GREEN;

    aceleracao.tor_max          = 1500;
    aceleracao.vel_max          = VEL_MAX_RPM;
    aceleracao.freio_regen      = 0;
    aceleracao.dif_elt          = 0;
    aceleracao.traction_control = 1;
    aceleracao.bat_safe         = 0;
    aceleracao.torq_gain        = 15;
    aceleracao.mode             = ACELERACAO;
    aceleracao.rgbColor[0]      = PURPLE;

    skidpad.tor_max          = 2000;
    skidpad.vel_max          = VEL_MAX_RPM;
    skidpad.freio_regen      = 0;
    skidpad.dif_elt          = 0;
    skidpad.traction_control = 0;
    skidpad.bat_safe         = 0;
    skidpad.torq_gain        = 20;
    skidpad.mode             = SKIDPAD;
    skidpad.rgbColor[0]      = CYAN;

    autox.tor_max          = 2500;
    autox.vel_max          = VEL_MAX_RPM;
    autox.freio_regen      = 0;
    autox.dif_elt          = 0;
    autox.traction_control = 0;
    autox.bat_safe         = 0;
    autox.torq_gain        = 25;
    autox.mode             = AUTOX;
    autox.rgbColor[0]      = BLUE;

    erro.tor_max          = 0;
    erro.vel_max          = 0;
    erro.freio_regen      = 0;
    erro.dif_elt          = 0;
    erro.traction_control = 0;
    erro.bat_safe         = 0;
    erro.torq_gain        = 0;
    erro.mode             = ERRO;
    erro.rgbColor[0]      = RED;

    set_global_var_value(SELECTED_MODE, enduro); // inicializa no modo enduro
}

void deInit_all_peripherals() {
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_All);
    HAL_FDCAN_DeInit(&hfdcan1);
    HAL_FDCAN_DeInit(&hfdcan2);
    HAL_ADC_DeInit(&hadc1);
    HAL_UART_DeInit(&hlpuart1);
    HAL_SPI_DeInit(&hspi1);
    HAL_I2C_DeInit(&hi2c3);
    HAL_TIM_Base_DeInit(&htim1);
    HAL_TIM_Base_DeInit(&htim2);
}

void init_ECU() {
    /* ### - 2 - Start calibration ############################################ */
    if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED)
        != HAL_OK) {
        ;
    }
    HAL_TIM_Base_Start(&htim2);
    init_ADC_DMA(&hadc1);
    init_CAN();
    // init_global_variables must be before init_controls and init_modes
    // (i.e. before functions that use global variables)
    init_global_variables();
    init_controls();
    init_modes();
}
