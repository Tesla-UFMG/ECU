/*
 * initializers.c
 *
 *  Created on: 11 de mai de 2020
 *      Author: renanmoreira
 */

#include "initializers.h"

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
