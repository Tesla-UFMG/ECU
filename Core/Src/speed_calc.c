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

//velocidade m/s = omega*circunferencia = delta_theta/delta_tempo * 2*pi*raio
//              = (raio*2*pi/16)/delta_t (m/s)
//              = (raio*2*pi/16)/(n*25u) (m/s)
//              = 40khz*(raio*2*pi/16)/n (m/s)
//              = 3.6*40khz*(raio*2*pi/16)/n (km/h)
//              = 10*3.6*40khz*(raio*2*pi/16)/n (0.1km/h)
//              = 10*3.6*40khz*(0.26*2*pi/16)/n (0.1km/h) = 147026/n

volatile float g_wheel_speed[4];

extern TIM_HandleTypeDef htim2;
extern osMessageQueueId_t q_speed_messageHandle;

void speed_calc(void *argument) {
	speed_message_t message;
	speed_message_t last_messages[4];
	memset(&last_messages, 0, sizeof(speed_message_t)*4); //inicializa com 0 buffer de ultimas mensagens


	const uint32_t tim_freq  = HAL_RCC_GetPCLK1Freq(), //pega frequência do APB1, q está conectado ao tim2
				   tim_presc = htim2.Init.Prescaler+1;   //pescaler do tim2

	uint32_t d_tim_count, speed;

	for(;;) {
		osMessageQueueGet(q_speed_messageHandle, &message, NULL, osWaitForever); //espera até alguma mensagem chegar

		d_tim_count = message.tim_count - last_messages[message.pin].tim_count; //diferenca entre timestamp da mensagem atual e da anterior
		speed = (10*3.6*2*M_PI / SPEED_SENSOR_TEETH_QUAN) * (tim_freq/(d_tim_count*tim_presc));
		g_wheel_speed[message.pin] = speed; //seta velocidade especifica da roda recebida
		last_messages[message.pin] = message; //guarda mensagem até a próxima interacão

		uint16_t datalog_id = message.pin == FRONT_RIGHT ? ID_SPEED_FR :
							  message.pin == FRONT_LEFT  ? ID_SPEED_FL :
							  message.pin == BACK_RIGHT  ? ID_SPEED_RR :
									  	  	  	  	  	   ID_SPEED_RL;
		log_data(datalog_id, (uint16_t) speed);
	}

	//TODO: lógica para zerar velocidade de uma roda se n receber por certo período de tempo
}



