/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

/* USER CODE BEGIN 0 */
#include "ecu.h"
#include "CANSPI.h"
#include "MCP2515.h"

	#define SPEED1_PIN_N 0b10000000
	#define SPEED2_PIN_N 0b1
	#define SPEED3_PIN_N 0b10
	#define SPEED4_PIN_N 0b10000000000
	#define SPEED1_PIN_INV 0b11111111111101111111
	#define SPEED2_PIN_INV 0b11111111111111111110
	#define SPEED3_PIN_INV 0b11111111111111111101
	#define SPEED4_PIN_INV 0b11111111101111111111
	#define TIMER_OVERFLOW 4000//valor maximo do timer (deve ser 400 no codigo final). O estouro ocorre a cada 100ms, logo o prescaler � 1799
	uint32_t speed_pin_n [4] = {SPEED1_PIN_N, SPEED2_PIN_N, SPEED3_PIN_N, SPEED4_PIN_N}; //pinos de interrup��o;
	uint32_t speed_pin_inv [4] = {SPEED1_PIN_INV, SPEED2_PIN_INV, SPEED3_PIN_INV, SPEED4_PIN_INV};
	uint16_t speed_deb_time [4] = {0,0,0,0};
	uint16_t speed_deb_flag [4] = {0,0,0,0};
	uint16_t speed_deb_cnt [4] = {0,0,0,0};
	uint8_t speed_t_flag[4], apps_t_flag;
	uint32_t speed_t_total[4], speed_t0[4], speed_t1[4], timer2_value;

	uint8_t refresh_speed;
	uint16_t timer_speed_ant=0, timer_speed_atual=0, time_speed_refresh=0;

	//variaveis usadas para adc, delaradas em ecu.c
	extern uint16_t buffer[6];
	extern uint16_t ADC_DMA[6];
	//variaveis usadas para receber mensagem por SPI, delaradas em CANSPI.h
	extern uCAN_MSG rxMessage;
	extern uint8_t roda_interna;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern CAN_HandleTypeDef hcan;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

	Speed_Interrupt_Handle();

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
* @brief This function handles EXTI line1 interrupt.
*/
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */

	Speed_Interrupt_Handle();

  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel1 global interrupt.
*/
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
* @brief This function handles ADC1 and ADC2 global interrupts.
*/
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
* @brief This function handles USB high priority or CAN TX interrupts.
*/
void USB_HP_CAN1_TX_IRQHandler(void)
{
  /* USER CODE BEGIN USB_HP_CAN1_TX_IRQn 0 */

  /* USER CODE END USB_HP_CAN1_TX_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan);
  /* USER CODE BEGIN USB_HP_CAN1_TX_IRQn 1 */

  /* USER CODE END USB_HP_CAN1_TX_IRQn 1 */
}

/**
* @brief This function handles USB low priority or CAN RX0 interrupts.
*/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 0 */

  /* USER CODE END USB_LP_CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan);
  /* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 1 */
  mensagem_inversor_recebida(hcan.pRxMsg->StdId,hcan.pRxMsg->Data);		//Armazena dados inversor
  CAN_Receive_Handler();		// Reabilita can_receive
  /* USER CODE END USB_LP_CAN1_RX0_IRQn 1 */
}

/**
* @brief This function handles EXTI line[9:5] interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

	Speed_Interrupt_Handle();

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  HAL_GPIO_EXTI_IRQHandler(MODE_Pin);
  //HAL_GPIO_EXTI_IRQHandler(REGEN_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
* @brief This function handles TIM3 global interrupt.
*/
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

	mensagem_CANSPI_recebida(rxMessage.frame.id, rxMessage.frame.data);
	//CANSPI_Receive(rxMessage);

	Speed_Interrupt_Handle();
	CAN_SPI_EXTI_Handler();

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */


/* ------------- Fun��o que passa os valor da conversao ao serem finalizadas ------------- */


/* ---------- Funcao APPS ---------- */


//velocidade m/s = omega*circunferencia = delta_theta/delta_tempo * 2*pi*raio
//              = (raio*2*pi/16)/delta_t (m/s)
//              = (raio*2*pi/16)/(n*25u) (m/s)
//              = 40khz*(raio*2*pi/16)/n (m/s)
//              = 3.6*40khz*(raio*2*pi/16)/n (km/h)
//              = 10*3.6*40khz*(raio*2*pi/16)/n (0.1km/h)
//              = 10*3.6*40khz*(0.26*2*pi/16)/n (0.1km/h) = 147026/n 
void Speed_Interrupt_Handle() //leitura de numero de timers
//funcao responsavel por fazer o calculo do tempo entre as interrupcoes externas vindas do sensor de velocidade
//assim que recebe uma interrupao externa do sensor de velocidade, o valor do timer eh armazenado.
//Quando recebe a segunda interrup��o do sensor de velocidade, ele armazena em outra variavel e faz o calculo do Delta T (diferen�a desse tempo)
//Sendo Delta T proporcional a 2.5x10e-5 segundo (valor obtido com base no inverso de 72MHz dividido pelo pre-scale + 1).
{
	uint8_t i;

	timer2_value= TIM2->CNT;

	for(i=0; i<4; i++) //verifica qual canal gerou a interrup��o
	{
		if((EXTI->PR & speed_pin_n [i]) == speed_pin_n [i]) //se o sensor i for a fonte da interrup��o
		{
			EXTI->FTSR &=  speed_pin_inv [i]; //desativa interrup��o

			if(speed_t_flag[i]==0) //se for a primeira interrup��o depois de 100ms ou mais de carro parado (> 1km/h)
			{
				if(i== roda_interna) //se os sensores da frente foram atualizados
				{
					refresh_speed = 1;
					timer_speed_ant = 0;
					timer_speed_atual = timer2_value;
					time_speed_refresh=0;
				}
				speed_t1 [i]= timer2_value; //pega o valor do timer nesse momento
				speed_t_flag[i]=1; //indica que um valor foi pego nesse ciclo de timer
				speed_deb_time[i] = 200;
			}
			else //se o carro n�o est� parado a mais de 100ms
			{

				speed_t0 [i]= speed_t1 [i]; //passa a leitura anterior da interrup��o para segunda posi��o
				speed_t1 [i]= timer2_value; //atualiza posi��o atual do timer

				if(speed_t_flag[i]==2) //se o timer sofreu overflow
				{
					if(i==roda_interna) //se os sensores da frente foram atualizados
								{
									refresh_speed = 1;
									timer_speed_ant = timer_speed_atual;
									timer_speed_atual = timer2_value;
									time_speed_refresh = (TIMER_OVERFLOW - timer_speed_ant) + timer_speed_atual;
								}
					if(speed_t0[i] >= speed_t1[i])
					{
						speed_t_total[i] = ( TIMER_OVERFLOW - (speed_t0[i] - speed_t1[i]) ); //define o tempo entre a borda anterior e a atual
					}
					else
					{
						speed_t_total[i] = ( TIMER_OVERFLOW + (speed_t1[i] - speed_t0[i]) ); //define o tempo entre a borda anterior e a atual
					}
				}
				else
				{
					if(i==roda_interna) //se os sensores da frente foram atualizados
					{
						refresh_speed = 1;
						timer_speed_ant = timer_speed_atual;
						timer_speed_atual = timer2_value;
						time_speed_refresh = (TIMER_OVERFLOW - timer_speed_ant) + timer_speed_atual;
					}
					speed_t_total[i]= (speed_t1[i] - speed_t0[i]); //define o tempo entre a borda anterior e a atual
				}
				speed_t_flag[i]=1; //atualiza flag de atualiza��o, indicando que variavel foi atualizada nesse ciclo de timer

				speed_deb_time[i] = speed_t_total[i] >>3;
			}

			speed_deb_flag[i] = 1; //seta flag d deboucing
			speed_deb_cnt[i] = 0;

		}
	//	EXTI->PR = EXTI->PR & (~speed_pin_n); //limpa indicador de interrup��o no pino
	}
}


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
