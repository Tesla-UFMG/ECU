/*
 * speed_calc.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */


//velocidade m/s = omega*circunferencia = delta_theta/delta_tempo * 2*pi*raio
//              = (raio*2*pi/16)/delta_t (m/s)
//              = (raio*2*pi/16)/(n*25u) (m/s)
//              = 40khz*(raio*2*pi/16)/n (m/s)
//              = 3.6*40khz*(raio*2*pi/16)/n (km/h)
//              = 10*3.6*40khz*(raio*2*pi/16)/n (0.1km/h)
//              = 10*3.6*40khz*(0.26*2*pi/16)/n (0.1km/h) = 147026/n
void speed_calc(void *argument) {
	speed_message_t message;
	speed_message_t last_messages[4];
	memset(&last_messages, 0, sizeof(speed_message_t)*4); //inicializa com 0 buffer de ultimas mensagens


	const uint32_t tim_freq  = HAL_RCC_GetPCLK1Freq(), //pega frequência do APB1, q está conectado ao tim2
				   tim_presc = htim2.Init.Prescaler;   //pescaler do tim2

	uint32_t d_tim_count, speed;

	for(;;) {
		osMessageQueueGet(q_speed_messageHandle, &message, NULL, osWaitForever);

		d_tim_count = message.tim_count - last_messages[message.pin].tim_count;

		speed = (10*3.6*2*M_PI / SPEED_SENSOR_TEETH_QUAN) * (tim_freq/(d_tim_count*tim_presc));

		wheel_speed[message.pin] = speed;

		last_messages[message.pin] = message;
		osDelay(1);
	}



	if (speed_t_total[0] == 0)
		vel_roda[0] = 0;
	else
		vel_roda[0] = (uint16_t) (147026 / speed_t_total[0]);

	if (speed_t_total[1] == 0)
		vel_roda[1] = 0;
	else
		vel_roda[1] = (uint16_t) (147026 / speed_t_total[1]);

	if (speed_t_total[2] == 0)
		vel_roda[2] = 0;
	else
		vel_roda[2] = (uint16_t) (147026 / speed_t_total[2]);

	if (speed_t_total[3] == 0)
		vel_roda[3] = 0;
	else
		vel_roda[3] = (uint16_t) (147026 / speed_t_total[3]);

	media_diant = (vel_roda[0] + vel_roda[1]) >> 1;
}



