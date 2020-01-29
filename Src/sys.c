/*
 * sistema.c
 *
 *  Created on: 10 Aug 2018
 *      Author: r0t
 */
#include "ecu.h"
#include "sys.h"
#include "eeprom.h"
#include "stm32f1xx_hal.h"

extern uint8_t error_count;
cores_t led_conf;

uint8_t error_count = 0; // conta erros, quantas vezes o programa caiu no error handler
uint16_t debug_milis =0, debug_milis_ant = 0;

void inicializa_perifericos()
{
  SystemClock_Config();
  HAL_Init();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  Timer2_Config();
  CAN_ConfigFilter();
  CAN_ConfigFrames();
  CAN_Receive_Init();
  CANSPI_Initialize();
  inicializa_adc_dma();
  EE_Init();
  //inicializa_watchdog();
/*
  //Inicializa watchdog


*/
}

void inicializa_watchdog(){
	MX_WWDG_Init();

	IWDG->KR = 0x5555;
	IWDG->PR = PR_WDG;
	IWDG->RLR = RELOAD_WDG;
	IWDG->KR = SET_WDG;

}

void atualiza_watchdog(){
	IWDG->KR = REFRESH_WDG;
}



//funcao de debug temporizada
void debug_temp(uint16_t debug_periodo_ms){
	debug_milis = HAL_GetTick();
	if (debug_milis - debug_milis_ant >= debug_periodo_ms) {
		//insira aqui
//		uint8_t msg_debug[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
//		print_can(msg_debug, 0x10);
		// se deu algum erro, piscar led a cada tempo
		if (error_count > 0) {
			led_conf = ~led_conf;
			seta_leds(led_conf);
			aciona_sirene(5);
		}
		debug_milis_ant = debug_milis;
	}
}


void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	error_count++;
	//while(1) {


	//}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
