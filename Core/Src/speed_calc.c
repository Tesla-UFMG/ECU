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

//distancia     = circuferencia dividido pela quantidade de dentes
//              = 2*pi*raio/quant.dentes (m)
//tempo         = valor do timer multiplicado pelo prescaler dividido pela frequencia
//              = timer*prescaler/frequencia (s)
//velocidade    = distancia/tempo
//              = (2*pi*raio/quant.dentes)*freq/(presc*timer) (m/s)
//              = (10*3.6*2*pi*raio/quant.dentes)*freq/(presc*timer) (10*km/s)

void reset_speed_all();
void reset_speed_single(speed_message_t message, speed_message_t last_messages[4], uint32_t min_count);

volatile float g_wheel_speed[4];

extern TIM_HandleTypeDef htim2;
extern osMessageQueueId_t q_speed_messageHandle;

uint32_t d_tim_countx;

void speed_calc(void *argument) {
	speed_message_t message;
	speed_message_t last_messages[4];
	memset(&last_messages, 0, sizeof(speed_message_t)*4);   //inicializa com 0 buffer de ultimas mensagens


	const uint32_t	tim_freq  = HAL_RCC_GetPCLK1Freq(),     //pega frequência do APB1, q está conectado ao tim2
	        tim_presc = htim2.Init.Prescaler+1,             //prescaler do tim2
			//valor em tempo do timer2 da velocidade máxima a ser calculada
			max_count = (10*3.6*2*M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN) * ((float)tim_freq/((float)tim_presc)) / MAX_SPEED,
			//valor em tempo do timer2 da velocidade mínima a ser calculada
			min_count = (10*3.6*2*M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN) * ((float)tim_freq/((float)tim_presc)) / MIN_SPEED,
			//valor em tempo do timersys da velocidade mínima a ser calculada
			min_timeout = (10*3.6*2*M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN) * 1000 / MIN_SPEED;


	uint32_t d_tim_count, speed;

	for(;;) {
		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		switch(osMessageQueueGet(q_speed_messageHandle, &message, NULL,  min_timeout)){     //espera até alguma mensagem chegar ou timeout estourar

		//caso a tarefa tenha sido chamada por timeout
		case osErrorTimeout:
			reset_speed_all();  //zera a velocidade de todas as rodas
			break;

		//caso a tarefa tenha sido chamada pela queue
		case osOK:
			//verifica se alguma roda está a muito tempo sem receber interrupção, caso sim a velocidade dessa roda é zerada
			reset_speed_single(message, last_messages, min_count);

			//diferenca entre timestamp da mensagem atual e da anterior
			d_tim_count = message.tim_count - last_messages[message.pin].tim_count;

			//caso d_tim_count calcule uma velocidade maior do que é possível o valor será descartado
			if(d_tim_count < max_count)
				continue;

			d_tim_countx=d_tim_count;
			speed = (10*3.6*2*M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN) * ((float)tim_freq/((float)d_tim_count*tim_presc)); //calcula a velocidade
			g_wheel_speed[message.pin] = speed;     //seta velocidade especifica da roda recebida
			last_messages[message.pin] = message;   //guarda mensagem até a próxima interacão

			uint16_t datalog_id =   message.pin == FRONT_RIGHT ?    ID_SPEED_FR :
			                        message.pin == FRONT_LEFT  ?    ID_SPEED_FL :
			                        message.pin == REAR_RIGHT  ?    ID_SPEED_RR :
			                                                        ID_SPEED_RL;
			log_data(datalog_id, (uint16_t) speed);
		break;
		}
	}
}

void reset_speed_all(){
	for(speed_pin_e i = FIRST_WHEEL; i <= LAST_WHEEL; i++)
		g_wheel_speed[i] = 0;
}

void reset_speed_single(speed_message_t message, speed_message_t last_messages[4], uint32_t min_count){
	for(speed_pin_e i = FIRST_WHEEL; i <= LAST_WHEEL; i++)
		if((message.tim_count - last_messages[i].tim_count) > min_count)
			g_wheel_speed[i] = 0;
}











