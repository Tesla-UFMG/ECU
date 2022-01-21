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

#define DEBUG_ECU 1

//pinos do sensores de velocidade
typedef enum {
    FRONT_RIGHT,
    FRONT_LEFT,
    REAR_RIGHT,
    REAR_LEFT
} speed_pin_e;

// cores do LED da ECU
typedef enum  { PRETO = 0, VERMELHO, VERDE, AZUL, AMARELO, ROXO, CIANO, BRANCO} cores_t;

typedef enum modos_desempenho {ERRO = 0, ENDURO, ACELERACAO, SKIDPAD, AUTOX} race_mode_t;

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
	race_mode_t mode; // 1 enduro, 2 aceleracao, 3 skidpad, 4 autox
	cores_t cor;
} modos;

#define R_MOTOR 0
#define L_MOTOR 1


//CONTROLE.c

typedef struct {
	uint8_t parameter_control;
	uint16_t ref_torque_neg[2];
	uint16_t ref_veloc[2];
	bool regen_active;
} vehicle_state_parameters_t;

typedef struct {
	uint16_t ref_torque[2];
	bool disable;
} ref_torque_t;

typedef enum estado_veiculo {
	S_DISABLE_E = 0,
	S_BRAKE_E = 1,
	S_ACCELERATE_E = 2,
	S_NEUTER_E = 3
} vehicle_state_e;

//----------

typedef struct {
	uint8_t parameters;
	uint32_t torque_ref[2];
	uint32_t neg_torque_ref[2];
	uint32_t speed_ref[2];
} torque_message_t;



//DATALOG

typedef struct {
	uint16_t id;
	uint16_t data;
} datalog_message_t;


//-----------


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


//General Flags
#define RTD_FLAG                    (1 << 1)
#define RTD_BTN_PRESSED_FLAG        (1 << 2)
#define MODE_BTN_PRESSED_FLAG       (1 << 3)
#define GENERAL_BUS_OFF_ERROR_FLAG  (1 << 4)
#define INVERTER_CAN_ACTIVE         (1 << 5)
#define INVERTER_READY              (1 << 6)

//Warning flags	(No actions necessary)
#define REGEN_WARN_FLAG             (1 << 10)
#define DYNAMIC_CONTROL_WARN_FLAG   (1 << 11)

//Soft error flags (RTD keeps on, torque ref to inverter is set to 0)

#define BSE_ERROR_FLAG              (1 << 16)     //Regulamento: EV.5.7 (2021)
#define APPS_ERROR_FLAG             (1 << 17)     //Regulamento: T.4.2 (2021)

//Hard error flags (RTD disable)
#define INVERTER_COMM_ERROR_FLAG    (1 << 20)
#define SU_F_ERROR_FLAG             (1 << 21)
#define INVERTER_BUS_OFF_ERROR_FLAG (1 << 22)


#define ALL_WARN_FLAG               REGEN_WARN_FLAG | DYNAMIC_CONTROL_WARN_FLAG
#define ALL_MINOR_ERROR_FLAG        APPS_ERROR_FLAG | BSE_ERROR_FLAG
#define ALL_SEVERE_ERROR_FLAG       INVERTER_COMM_ERROR_FLAG | SU_F_ERROR_FLAG
#define ALL_ERRORS_FLAG             APPS_ERROR_FLAG | BSE_ERROR_FLAG | INVERTER_COMM_ERROR_FLAG | SU_F_ERROR_FLAG | INVERTER_BUS_OFF_ERROR_FLAG

#define ALL_THROTTLE_ERROR_FLAG     APPS_ERROR_FLAG | BSE_ERROR_FLAG



//FUNCOES

//seta o bit na posicao pos do byte como state
__attribute__((always_inline)) static inline
void set_bit(uint32_t* byte, uint8_t pos, uint8_t state) {
	*byte ^= (-(!!((unsigned long)state)) ^ *byte) & (1UL << pos);
}

__attribute__((always_inline)) static inline
void set_bit8(uint8_t* byte, uint8_t pos, uint8_t state) {
	*byte ^= (-(!!((unsigned long)state)) ^ *byte) & (1UL << pos);
}


#endif /* INC_GLOBAL_DEFINITIONS_H_ */
