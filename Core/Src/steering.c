/*
 * steering.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

void steering_read(void *argument) {
	volante_cru = ADC_DMA[2];

	uint16_t volante_aux = volante_cru,
			 zero_aux = ZERO_VOLANTE;

	//Se o mínimo do volante for menor que 0, o sensor voltará no valor máximo do ADC
	//se isso acontecer, o valor do ADC voltará para 4095
	//então subtrai 4095 do valor lido, dando um valor negativo que pode ser aplicado na formula
	//o mesmo vale pro zero do volante
	if (VOLANTE_MIN > VOLANTE_MAX) {
		zero_aux -= 4095;
		if (volante_cru > VOLANTE_MAX)
			volante_cru -= 4095;
	}


	if (volante_cru < zero_aux) {
		volante = 0;
	}
	else{
		volante = volante_cru * GANHO_VOLANTE - ZERO_VOLANTE;
	}

	//SPAN_ALINHAMENTO é apenas um span pra ainda considerar o volante no centro
	//até uma certa quantidade
	if(volante > VOLANTE_ALINHADO + SPAN_ALINHAMENTO){
		roda_interna = ESQUERDA;
	}
	else if(volante < VOLANTE_ALINHADO - SPAN_ALINHAMENTO){
		roda_interna = DIREITA;
	}
	else{
		roda_interna = CENTRO;
	}

//	return (volante);
}
