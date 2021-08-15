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

		switch(osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, RGB_BLINK_DELAY)){ //espera RTD ser setado ou timeout estourar

		case osFlagsErrorTimeout:				//caso timeout estore vai piscar o led, indicando que tá fora do RTD
			write_rgb_color(get_rgb_color(modo_selecionado.cor));
			osDelay(RGB_BLINK_DELAY);
			write_rgb_color(get_rgb_color(PRETO));
			break;

		default:
			write_rgb_color(get_rgb_color(modo_selecionado.cor));			//caso RTD seja ativo o RGB ficará fixo
			osEventFlagsWait(ECU_control_event_id, ALL_ERRORS_FLAG | ALL_WARN_FLAG, osFlagsNoClear, osWaitForever);	//espera por uma flag de erro
			uint32_t flags = osEventFlagsGet(ECU_control_event_id);
			flags &= (ALL_ERRORS_FLAG | ALL_WARN_FLAG); 				//filtra apenas erros e avisos
				switch (flags) {											//caso seja um aviso a cor do led será branca, caso seja um erro leve será amarelo,
					case REGEN_WARN_FLAG:
					case DYNAMIC_CONTROL_WARN_FLAG:
						write_rgb_color(get_rgb_color(BRANCO));
					break;
					case APPS_ERROR_FLAG:
					case BSE_ERROR_FLAG:
						write_rgb_color(get_rgb_color(AMARELO));
					break;
				}
				osDelay(RGB_WARN_DELAY);
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
	switch (color){
		case PRETO:
			return RGB_BLACK;
		case VERMELHO:
			return RGB_RED;
		case VERDE:
			return RGB_GREEN;
		case AZUL:
			return RGB_BLUE;
		case AMARELO:
			return RGB_YELLOW;
		case ROXO:
			return RGB_PURBLE;
		case CIANO:
			return RGB_CYAN;
		case BRANCO:
			return RGB_WHITE;
		default:
			return RGB_WHITE;
	}
}
