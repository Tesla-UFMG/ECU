/*
 * speed_calc.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "speed_calc.h"
#include "cmsis_os.h"
#include "stm32h7xx.h"
#include "math.h"
#include "string.h"
#include "datalog_handler.h"
#include "main.h"

//distancia 	= circuferencia dividido pela quantidade de dentes
//				= 2*pi*raio/quant.dentes (m)
//tempo 		= valor do timer multiplicado pelo prescaler dividido pela frequencia
//				= timer*prescaler/frequencia (s)
//velocidade	= distancia/tempo
//              = (2*pi*raio/quant.dentes)*freq/(presc*timer) (m/s)
//              = (10*3.6*2*pi*raio/quant.dentes)*freq/(presc*timer) (10*km/s)

speed_pin_e get_speed_pin(uint16_t pin);

volatile float g_wheel_speed[4];

extern TIM_HandleTypeDef htim2;
extern osMessageQueueId_t q_speed_messageHandle;


void speed_calc(void *argument) {
	speed_message_t message;
	speed_message_t last_messages[4];
	memset(&last_messages, 0, sizeof(speed_message_t)*4); //inicializa com 0 buffer de ultimas mensagens


	const uint32_t	tim_freq  = HAL_RCC_GetPCLK1Freq(), 	//pega frequência do APB1, q está conectado ao tim2
				   	tim_presc = htim2.Init.Prescaler+1,   	//pescaler do tim2
					max_count = (10*3.6*2*M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN) * ((float)tim_freq/((float)tim_presc)) / MAX_SPEED;


	uint32_t d_tim_count, speed;

	for(;;) {
		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		switch(osMessageQueueGet(q_speed_messageHandle, &message, NULL,  1000)){ //espera até alguma mensagem chegar


		case osErrorTimeout:
			g_wheel_speed[FRONT_RIGHT] = 0;
			g_wheel_speed[FRONT_LEFT] = 0;
			g_wheel_speed[REAR_RIGHT] = 0;
			g_wheel_speed[REAR_LEFT] = 0;
			break;


		case osOK: ;

		speed_pin_e wheel_pin = get_speed_pin(message.pin);
		d_tim_count = message.tim_count - last_messages[wheel_pin].tim_count; //diferenca entre timestamp da mensagem atual e da anterior


		if(d_tim_count < max_count) { //caso d_tim_count calcule uma velocidade maior do que é possível o valor será descartado
			Error_Handler();
			continue;
		}

		speed = (10*3.6*2*M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN) * ((float)tim_freq/((float)d_tim_count*tim_presc));
		g_wheel_speed[wheel_pin] = speed; 	//seta velocidade especifica da roda recebida
		last_messages[wheel_pin] = message; 	//guarda mensagem até a próxima interacão

		uint16_t datalog_id = 	wheel_pin == FRONT_RIGHT ? 	ID_SPEED_FR :
								wheel_pin == FRONT_LEFT  ? 	ID_SPEED_FL :
								wheel_pin == REAR_RIGHT  ? 	ID_SPEED_RR :
															ID_SPEED_RL;
		log_data(datalog_id, (uint16_t) speed);
			break;
	}

	//TODO: lógica para zerar velocidade de uma roda se n receber por certo período de tempo
}
}



speed_pin_e get_speed_pin(uint16_t pin){
	switch(pin){
	case S_VEL1_Pin:
		return FRONT_RIGHT;
		break;
	case S_VEL2_Pin:
		return FRONT_LEFT;
		break;
	case S_VEL3_Pin:
		return REAR_RIGHT;
		break;
	default:
		return REAR_LEFT;
		break;
	}
}




