/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include <stdbool.h>

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
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
#define RTDS_IN_Pin GPIO_PIN_11
#define RTDS_IN_GPIO_Port GPIOA
#define TSMS_Pin GPIO_PIN_12
#define TSMS_GPIO_Port GPIOA
#define HABILITA_Pin GPIO_PIN_15
#define HABILITA_GPIO_Port GPIOA

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
