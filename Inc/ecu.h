/*
 * ecu.h
 *
 *  Created on: 10 de Ago de 2018
 *      Author: r0tc
 */

#ifndef ECU_H_
#define ECU_H_

#include "sys.h"
#include "ecu_defines.h"

#define		_5_kmph_rpm 			459
#define		id_msg_controle_dir 	0x301
#define		id_msg_controle_esq 	0x300
#define		id_flag_comunic			0x302		// flag de comunicacao ecu-inv ok
#define		pedal_acel_min 			5
#define     desired_slip_ratio		13  		// 13% de slip ratio
#define		DRY						13
#define		WET						8

//defines do watchdog
#define SET_WDG 0xCCCC
#define REFRESH_WDG 0xAAAA
#define PR_WDG 1 //na verdade 1
#define RELOAD_WDG 2500

enum chave_seletora_valores { SELECAO_1 = 410, SELECAO_2 = 1230, SELECAO_3 = 2050,
								SELECAO_4 = 2870, SELECAO_5=3250, SELECAO_6 = 3690 };

enum rodas { RODA_DIANT_ESQ = 0, RODA_DIANT_DIR, RODA_TRAS_DIR, RODA_TRAS_ESQ };
enum modos_desempenho { ERRO = 0, ACELERACAO, SKIDPAD, AUTOX, ENDURO };
enum cores { PRETO = 0,VERDE, AZUL, CIANO, VERMELHO, AMARELO,ROXO, BRANCO };	// cores do LED da ECU
typedef enum cores cores_t;
enum motores { MOTOR_ESQ = 0, MOTOR_DIR = 1, MOTORESQ_DIR = 2 };
typedef enum motores motores_t;
enum estado_veiculo { DESABILITA = 0, FREIA = 1, ACELERA = 2, NEUTRO = 3, REVERSE = 4 };
typedef enum estado_veiculo estado_veiculo_t;
enum regen_intesity { LOW = 300, MEDIUM = 450, HIGH = 600 };	// intensidades do torque de frenagem

typedef struct //struct de modo
{
	int tor_max; //torque maximo (de 0 a 4000)
	int vel_max; //velocidade maxima (de 0 a 9000)
	bool freio_regen; //frenagem regenerativa (1 para ativada, 0 para desativada)
	bool dif_elt; //diferencial eletronico (1 ativo, 0 desat)
	bool launch_control; //controle de arrancada (1 para ativado, 0 para desativado)
	bool traction_control; //controle de tracao (1 ativo, 0 desat)
	bool bat_safe; //reducao de consumo de bateria se em niveis criticos (1 ativo, 0 desat)
	int torq_gain; //ganho de torque, aconselhavel q seja proporcional ao torque maximo ( de 0 a 40)
	enum modos_desempenho mode; // 1 acel, 2 skid, 3 autox, 4 enduro
	cores_t cor;
} modos;

void inicializa_perifericos();
void seta_leds(uint8_t led_config);
void inicializa_modos();
void inicializa_adc_dma();
void testa_comunic_inv(uint16_t com_inv_periodo_ms);
uint16_t le_acelerador(uint8_t* flag_error);
uint16_t le_volante();
void controle();
void comando_inversor();
void transmite_inversores_datalogger(uint16_t, uint8_t*);
void mensagem_inversor_recebida(uint16_t id,uint8_t* data);
void actual_datalogger();
void transmite_ecu_datalogger();
void acel_datalogger();
void skid_datalogger();
void autox_datalogger();
void enduro_datalogger();
void init_datalogger();
void start_datalogger();
void stop_datalogger();
void print_can(uint8_t id, uint16_t* data);
void mensagem_CANSPI_recebida(uint16_t id, uint8_t* data);
int16_t PID_control(int16_t setpoint, int16_t medida);
void Vel_Calc();
void Dist_Calc();
void Odometer();
void Diferencial();
void Diferencial_2();
int16_t Funcao_Dif();
void inicializa_tab_dif();
void Controle_arrancada();
void rampa_torque();
bool pedal_freio_acionado();
bool botao_habilita_acionado();
bool contator_fechado();
modos le_chave_modo();
void aciona_sirene(uint16_t tempo_ms);
void wheel_slip();
void tc_system();
void torque_vectoring();
void toggle_regen();	// turns regenerative braking on/off
void ERS_control();		// Energy Recovery System -> regenerative braking
void seta_flags(modos mode);

#endif /* ECU_LIB_H_ */
