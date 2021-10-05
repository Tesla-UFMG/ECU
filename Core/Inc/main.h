/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define C_LED_DEBUG1_Pin GPIO_PIN_4
#define C_LED_DEBUG1_GPIO_Port GPIOE
#define C_LED_DEBUG2_Pin GPIO_PIN_5
#define C_LED_DEBUG2_GPIO_Port GPIOE
#define C_LED_DEBUG3_Pin GPIO_PIN_6
#define C_LED_DEBUG3_GPIO_Port GPIOE
#define S_APPS2_Pin GPIO_PIN_5
#define S_APPS2_GPIO_Port GPIOA
#define S_APPS1_Pin GPIO_PIN_6
#define S_APPS1_GPIO_Port GPIOA
#define S_FREIO_Pin GPIO_PIN_7
#define S_FREIO_GPIO_Port GPIOA
#define S_VOLANTE_Pin GPIO_PIN_4
#define S_VOLANTE_GPIO_Port GPIOC
#define S_ADC_E1_Pin GPIO_PIN_5
#define S_ADC_E1_GPIO_Port GPIOC
#define S_ADC_E2_Pin GPIO_PIN_1
#define S_ADC_E2_GPIO_Port GPIOB
#define S_VEL1_Pin GPIO_PIN_12
#define S_VEL1_GPIO_Port GPIOE
#define S_VEL1_EXTI_IRQn EXTI15_10_IRQn
#define S_VEL2_Pin GPIO_PIN_13
#define S_VEL2_GPIO_Port GPIOE
#define S_VEL2_EXTI_IRQn EXTI15_10_IRQn
#define S_VEL3_Pin GPIO_PIN_14
#define S_VEL3_GPIO_Port GPIOE
#define S_VEL3_EXTI_IRQn EXTI15_10_IRQn
#define S_VEL4_Pin GPIO_PIN_15
#define S_VEL4_GPIO_Port GPIOE
#define S_VEL4_EXTI_IRQn EXTI15_10_IRQn
#define C_LED_BLUE_Pin GPIO_PIN_12
#define C_LED_BLUE_GPIO_Port GPIOD
#define C_LED_GREEN_Pin GPIO_PIN_13
#define C_LED_GREEN_GPIO_Port GPIOD
#define C_LED_RED_Pin GPIO_PIN_14
#define C_LED_RED_GPIO_Port GPIOD
#define B_DEBUG2_Pin GPIO_PIN_3
#define B_DEBUG2_GPIO_Port GPIOD
#define B_RTD_Pin GPIO_PIN_4
#define B_RTD_GPIO_Port GPIOD
#define B_RTD_EXTI_IRQn EXTI4_IRQn
#define B_MODO_Pin GPIO_PIN_6
#define B_MODO_GPIO_Port GPIOD
#define B_MODO_EXTI_IRQn EXTI9_5_IRQn
#define C_RTDS_Pin GPIO_PIN_5
#define C_RTDS_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_8
#define BOOT1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
