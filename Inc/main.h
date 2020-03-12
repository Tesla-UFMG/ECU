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
#include "stm32f1xx_hal.h"

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
#define __DLC__ 8
#define Led_green_Pin GPIO_PIN_13
#define Led_green_GPIO_Port GPIOC
#define Led_blue_Pin GPIO_PIN_14
#define Led_blue_GPIO_Port GPIOC
#define Led_red_Pin GPIO_PIN_15
#define Led_red_GPIO_Port GPIOC
#define APPS1_Pin GPIO_PIN_0
#define APPS1_GPIO_Port GPIOA
#define FREIO_Pin GPIO_PIN_1
#define FREIO_GPIO_Port GPIOA
#define VOLANTE_Pin GPIO_PIN_2
#define VOLANTE_GPIO_Port GPIOA
#define APPS2_Pin GPIO_PIN_3
#define APPS2_GPIO_Port GPIOA
#define SVO_1_Pin GPIO_PIN_7
#define SVO_1_GPIO_Port GPIOA
#define SVO_1_EXTI_IRQn EXTI9_5_IRQn
#define SVO_2_Pin GPIO_PIN_0
#define SVO_2_GPIO_Port GPIOB
#define SVO_2_EXTI_IRQn EXTI0_IRQn
#define SVO_3_Pin GPIO_PIN_1
#define SVO_3_GPIO_Port GPIOB
#define SVO_3_EXTI_IRQn EXTI1_IRQn
#define BOOT_1_Pin GPIO_PIN_2
#define BOOT_1_GPIO_Port GPIOB
#define SVO_4_Pin GPIO_PIN_10
#define SVO_4_GPIO_Port GPIOB
#define SVO_4_EXTI_IRQn EXTI15_10_IRQn
#define SPI2_INT_Pin GPIO_PIN_12
#define SPI2_INT_GPIO_Port GPIOB
#define SPI2_INT_EXTI_IRQn EXTI15_10_IRQn
#define CS_CAN_Pin GPIO_PIN_8
#define CS_CAN_GPIO_Port GPIOA
#define RTDS_Pin GPIO_PIN_11
#define RTDS_GPIO_Port GPIOA
#define TSMS_Pin GPIO_PIN_12
#define TSMS_GPIO_Port GPIOA
#define HABILITA_Pin GPIO_PIN_15
#define HABILITA_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
