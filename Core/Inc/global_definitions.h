/*
 * global_definitions.h
 *
 *  Created on: 11 de mai de 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_DEFINITIONS_H_
#define INC_GLOBAL_DEFINITIONS_H_

#include "stdbool.h"
#include "stdint.h"

// cores do LED da ECU
typedef enum cores { PRETO = 0,VERDE, AZUL, CIANO, VERMELHO, AMARELO,ROXO, BRANCO } cores_t;

typedef enum modos_desempenho { ERRO = 0, ACELERACAO, SKIDPAD, AUTOX, ENDURO } race_mode_t;

typedef struct //struct de modo
{
	int tor_max; //torque maximo (de 0 a 4000)
	int vel_max; //velocidade maxima (de 0 a 9000)
	bool freio_regen; //frenagem regenerativa (1 para ativada, 0 para desativada)
	bool dif_elt; //diferencial eletronico (1 ativo, 0 desat)

	//bool arranc_control;

	bool traction_control; //controle de tracao (1 ativo, 0 desat)
	bool bat_safe; //reducao de consumo de bateria se em niveis criticos (1 ativo, 0 desat)
	int torq_gain; //ganho de torque, aconselhavel q seja proporcional ao torque maximo ( de 0 a 40)
	race_mode_t mode; // 1 acel, 2 skid, 3 autox, 4 enduro
	cores_t cor;
} modos;



const uint32_t RTD_FLAG 			 = 1 << 5;
const uint32_t RTD_BTN_PRESSED_FLAG  = 1 << 6;

#endif /* INC_GLOBAL_DEFINITIONS_H_ */
