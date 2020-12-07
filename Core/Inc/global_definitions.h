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

#define R_MOTOR 0
#define L_MOTOR 1


//CONTROLE.c

typedef struct {
	uint8_t parameter_control;
	uint16_t ref_torque[2];
	uint16_t ref_torque_neg[2];
	uint16_t ref_veloc[2];
	bool regen_active;
} vehicle_state_parameters_t;

//----------

typedef struct {
	uint8_t parameters;
	uint32_t torque_ref[2];
	uint32_t neg_torque_ref[2];
	uint32_t speed_ref[2];
} torque_message_t;


#define ADC_LINES 6

typedef enum {
	 APPS1_E = 0,
	 APPS2_E = 5,
	 STEERING_WHEEL_E = 1,
	 BRAKE_E = 3,
	 ADC_E1_E = 4,
	 ADC_E2_E = 2
} ADC_DMA_position_e;

#define APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE 10

const uint32_t RTD_FLAG 				= 1 << 5;
const uint32_t RTD_BTN_PRESSED_FLAG  	= 1 << 6;



const uint32_t APPS_ERROR_FLAG 		 	= 1 << 16;
const uint32_t INVERTER_COMM_ERROR_FLAG = 1 << 15;

const uint32_t ALL_ERRORS_FLAG = APPS_ERROR_FLAG 			|
								 INVERTER_COMM_ERROR_FLAG;



//FUNCOES

//seta o bit na posicao pos do byte como state
__attribute__((always_inline)) static inline
void set_bit(uint32_t* byte, uint8_t pos, uint8_t state) {
	*byte ^= (-(!!((unsigned long)state)) ^ *byte) & (1UL << pos);
}


#endif /* INC_GLOBAL_DEFINITIONS_H_ */
