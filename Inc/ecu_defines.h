/*
 * ecu_defines.h
 * Configura parametros gerais do ECU.
 *
 */

#define num_amostras_filtro 	            10			//Amostra media movel acelerador
#define TORQUE_MAX 			                3000		//em decimos de percentagem do torque nom
#define vel_max_rpm 		                9000
#define	vetorizacao_torque	             	0
#define	frenagem_regenerativa	            0
#define TORQUE_FRENAGEM 					750
#define	modo_aceleracao	              		0
#define	modo_enduro			                0
#define zona_morta_acelerador          		10
#define TORQUE_GAIN 						1
#define ZERO_VOLANTE 						786
#define GANHO_VOLANTE						9.07
#define GAIN_ARRANC 						1.1
#define INC_VELOC 							50
#define DIREITA 							0
#define ESQUERDA							1
#define CENTRO								2


// defines rampa de torque
#define INC_TORQUE 300						// inclinacao 2a parte da rampa
#define INC_TORQUE_INIT 200					// inclinacao 1a parte
#define TORQUE_INIT_LIMITE 500				// ponto de transicao entre as rampas

// defines controle de tracao
#define DECR_OVER 0.2						// decrescimo de torque em caso de overshoot de wheel slip
#define DECR_UNDER 0.05						// decrescimo de torque para reduzir overshoot de wheel slip
#define DECR_MID 0.125						// decrescimo de torque mediano
#define UPPER_THRESHOLD 14					// valor que determina overshoot
#define LOWER_THRESHOLD 10					// valor que determina undershoot
#define UNDER_MAX_LIMIT 9					// limite superior para undershoot
#define UNDER_MIN_LIMIT	6					// limite inferior para undershoot

/////////////////defines de pid//////////////////////////////////
#define KP 2
#define KI 0
#define KD 0
#define GAIN_PID 1
#define MAX_INTEGRAL_ERROR 10
#define SAMPLE_TIME 20 //em ms


// defines mascaras - flags das funcoes -> interface
#define RTD   0b1;
#define REGEN 0b10;
#define TC    0b100;
