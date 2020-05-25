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
