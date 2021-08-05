/*
 * leds.c
 * 	Cores:
 * 		Preto: -
 * 		Vermelho: 	Erro
 * 		Verde: 		Enduro
 * 		Azul: 		Autox
 * 		Amarelo: 	Erro leve (APPS ou BSE Plausability)
 * 		Roxo:		Aceleração
 * 		Ciano:		Skidpad
 * 		Branco:		Aviso (REGEN ou Controle dinâmicos)
 *
 *
 *  Created on: May 12, 2021
 *      Author: Felipe Telles
 */


#include "rgb_led.h"
#include "cmsis_os.h"
#include "global_variables.h"
#include "global_instances.h"

void write_rgb_color(rgb rgb_gpio);
void write_debug_color(rgb rgb_gpio);
rgb get_rgb_color(cores_t color);

void rgb_led(void *argument) {
	for(;;) {
		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		switch(osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, 200)){ //espera até alguma mensagem chegar ou timeout estourar

		case osFlagsErrorTimeout:
			write_rgb_color(get_rgb_color(modo_selecionado.cor));
			osDelay(200);
			write_rgb_color(get_rgb_color(PRETO));
			break;

		default:
			write_rgb_color(get_rgb_color(modo_ativado.cor));
			osEventFlagsWait(ECU_control_event_id, ALL_ERRORS_FLAG | ALL_WARN_FLAG, osFlagsNoClear, osWaitForever);
			uint32_t flags = osEventFlagsGet(ECU_control_event_id);
			flags = flags & (ALL_ERRORS_FLAG | ALL_WARN_FLAG);
				switch (flags) {
					case REGEN_WARN_FLAG:
					case DYNAMIC_CONTROL_WARN_FLAG:
						write_rgb_color(get_rgb_color(BRANCO));
						osDelay(10);
					break;
					case APPS_ERROR_FLAG:
					case BSE_ERROR_FLAG:
						write_rgb_color(get_rgb_color(AMARELO));
						osDelay(10);
					break;
				}
			break;
		}
	}
}

void write_rgb_color(rgb rgb_gpio){
	HAL_GPIO_WritePin(C_LED_RED_GPIO_Port, C_LED_RED_Pin, rgb_gpio.red);
	HAL_GPIO_WritePin(C_LED_GREEN_GPIO_Port, C_LED_GREEN_Pin, rgb_gpio.green);
	HAL_GPIO_WritePin(C_LED_BLUE_GPIO_Port, C_LED_BLUE_Pin, rgb_gpio.blue);
	//write_debug_color(rgb_gpio); //apenas para debug
}

void write_debug_color(rgb rgb_gpio){
	HAL_GPIO_WritePin(C_LED_DEBUG1_GPIO_Port, C_LED_DEBUG1_Pin, !rgb_gpio.red);
	HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, !rgb_gpio.green);
	HAL_GPIO_WritePin(C_LED_DEBUG3_GPIO_Port, C_LED_DEBUG3_Pin, !rgb_gpio.blue);
}

rgb get_rgb_color(cores_t color){
	rgb rgb_gpio;
	switch (color){
		case PRETO:
			rgb_gpio = RGB_BLACK;
			break;
		case VERMELHO:
			rgb_gpio = RGB_RED;
			break;
		case VERDE:
			rgb_gpio = RGB_GREEN;
			break;
		case AZUL:
			rgb_gpio = RGB_BLUE;
			break;
		case AMARELO:
			rgb_gpio = RGB_YELLOW;
			break;
		case ROXO:
			rgb_gpio = RGB_PURBLE;
			break;
		case CIANO:
			rgb_gpio = RGB_CYAN;
			break;
		case BRANCO:
			rgb_gpio = RGB_WHITE;
			break;
	}
	return rgb_gpio;
}
