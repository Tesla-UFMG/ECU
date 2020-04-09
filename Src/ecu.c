#include "ecu.h"
#include "CANSPI.h"
#include "MCP2515.h"
#include "eeprom.h"
extern uint32_t tempo_final;

/*
 * To do:
 *
 */

//	Variaveis globais
uint8_t vetTx[8],
		flag_estado_comando_inversor=0;

//	Dados do inversor
uint16_t 			vel_motor[2] = {0, 0},
					torque[2],
					potencia[2],
					corr_torque[2],
					energia_consumida[2],
					valor_sobrecarga[2],
					temp1_mosf[2], temp2_mosf[2],
					temp1_motor[2], temp2_motor[2],
					msg_perdidas[2], cont_busoff[2], estado_can[2],
					estado_inv[2], falha_atual[2], alarme_atual[2];
uint16_t			torq_frenagem = TORQUE_FRENAGEM;

//	Variaveis ECU
uint16_t	acelerador = 0,
			vel_roda[4],
			vel_calculada[2],
			vel_calc_motor[2],
			volante = 0,
			volante_cru = 0,
			freio = 0,
			status_ecu = 0,
			veloc_total = 0,
			refVeloc[2] = {0, 0},
			refTorque[2] = {0, 0},
			refTorqueNeg[2] = {0, 0},
			valor_APPS[2] = {0, 0},
			refVeloc_ant[2] = {0, 0},
			refTorque_ant[2] = {0, 0},
			paramControl = 0,
			media_diant = 0,
			media_calc = 0,
			distancia = 0,
			dist_calc = 0 ,
			ADC_DMA[6] = {0, 0, 0, 0, 0, 0},		// [APPS0 ,freio, volante ,APPS1 , AD_extra ,seletora_de_modo]
			slip[2], slip_ratio[2],					// wheel slip de cada roda traseira e a razao entre o obtido e o desejado
			roda_interna,							// variavel de controle da vetorizacao
			dist_addr_table[] = {0x7777, 0x8888}; 	// enderecos usados em eeprom.c
uint8_t 	regen_bk_selection = 0;					// selecao da frenagem no volante, indica qual intensidade setar

//	Condicionais booleanas
bool 	habilita = false,
		desabilitar = false,
		runstop = false,
		freiar = false,
		acelerar = false,
		rev = false,
		check_error = true,
		erro_com = false,
		flag_msg_nova_inversor[2] = {false, false},
		flag_msg_ant_inversor[2] = {false, false},
		button_regen_bk = false,					// botao para ligar/desligar frenagem -> atualmente RTD
		regen_active = false;						// flag para rede CAN

// Timers
uint32_t time_init = 0,
		 time_actual = 0; // inicializacao do timer e tempo real depois de subtrair o inicial do atual
uint32_t com_inv_milis=0, com_inv_milis_ant=0;
extern int16_t tempo_teste;

//	Modos
estado_veiculo_t estado = NEUTRO;
extern modos modo_selecionado;
modos aceleracao, skidpad, autox, enduro, reverse, erro;

// Controle dos motores
motores_t selected_motors = MOTORESQ_DIR;	// variavel para escolher quais motores acionar
											// MOTOR_DIR -> apenas motor direito
											// MOTOR_ESQ -> apenas motor esquerdo
											// MOTORESQ_DIR -> ambos

//	Erros, buffers e flags
uint8_t flag_rtds = 0,
		integral_ant = 0,
		flag_dtl = 0,
		status_datalog = 0,
		flag_can=1;
extern uint8_t speed_t_flag[4],
			   apps_t_flag;
int16_t integral_error = 0, error_area = 0,
		integral_buffer[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		last_error = 0,
		acel_ant = 0,
		prop_dif[25],
		funct_flags = 0;
uint32_t buffer[6];
uint32_t msgs_recebidas_inversor_atual[2] = { 0, 0};
uint32_t msgs_recebidas_inversor_ant[2] = { 0, 0};

// Outros
extern uint8_t dist_pr;
extern uint16_t time_speed_refresh;
extern uint32_t speed_t_total[4];
extern uint16_t dist_log[2];
extern uint16_t regen_slc;
extern uint16_t mode_slc;

void seta_leds(uint8_t led_config) //bit 0 azul, bit 1 verde, bit 2 vermelho
{
	if(0b100 == (led_config & 0b100))
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);

	if(0b010 == (led_config & 0b010))
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);

	if(0b001 == (led_config & 0b001))
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void seta_flags(modos mode) {
	if (mode.freio_regen == true) {
		funct_flags |= REGEN;
	}
	else {
		funct_flags &= ~REGEN;
	}
	if (mode.traction_control == true) {
		funct_flags |= TC;
	}
	else {
		funct_flags &= ~TC;
	}
}

void inicializa_modos() {
	aceleracao.tor_max = 3500;
	aceleracao.vel_max = vel_max_rpm;
	aceleracao.freio_regen = 0;
	aceleracao.dif_elt = 0;
	aceleracao.traction_control = 1;
	aceleracao.bat_safe = 0;
	aceleracao.torq_gain = 30;
	aceleracao.mode = ACELERACAO;
	aceleracao.cor = ROXO;

	skidpad.tor_max = 3000;
	skidpad.vel_max = vel_max_rpm;
	skidpad.freio_regen = 0;
	skidpad.dif_elt = 0;
	skidpad.traction_control = 0;
	skidpad.bat_safe = 0;
	skidpad.torq_gain = 20;
	skidpad.mode = SKIDPAD;
	skidpad.cor = CIANO;

	autox.tor_max = 3000;
	autox.vel_max = vel_max_rpm;
	autox.freio_regen = 0;
	autox.dif_elt = 0;
	autox.traction_control = 1;
	autox.bat_safe = 0;
	autox.torq_gain = 25;
	autox.mode = AUTOX;
	autox.cor = AZUL;

	enduro.tor_max = 2500;
	enduro.vel_max = vel_max_rpm;
	enduro.freio_regen = frenagem_regenerativa;
	enduro.dif_elt = 0;
	enduro.traction_control = 0;
	enduro.bat_safe = 1;
	enduro.torq_gain = 25; //ERA 15
	enduro.mode = ENDURO;
	enduro.cor = VERDE;

	reverse.tor_max = 500;
	reverse.vel_max = 450;
	reverse.freio_regen = 0;
	reverse.dif_elt = 0;
	reverse.traction_control = 0;
	reverse.bat_safe = 1;
	reverse.torq_gain = 10;
	reverse.cor = AMARELO;

	erro.tor_max = 0;
	erro.vel_max = 0;
	erro.freio_regen = 0;
	erro.dif_elt = 0;
	erro.traction_control = 0;
	erro.bat_safe = 0;
	erro.torq_gain = 0;
	erro.mode = ERRO;
	erro.cor = VERMELHO;
}

bool pedal_freio_acionado(){
	return ADC_DMA[1] > 2200;
}

bool botao_habilita_acionado(){
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 1;
}

// a implementar
bool contator_fechado(){
	return true;
}

modos le_chave_modo(){
	if(mode_slc == 0) {
		return enduro;
	}
	else if(mode_slc == 1) {
		return aceleracao;
	}
	else if(mode_slc == 2) {
		return autox;
	}
	else if(mode_slc == 3) {
		return skidpad;
	}
	else if(mode_slc == 4) {
		return reverse;
	}
	else return erro;
}

//Aciona Sirene por 1s
void aciona_sirene(uint16_t tempo_ms){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	delay_ms_ecu(tempo_ms);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
}

void testa_comunic_inv(uint16_t com_inv_periodo_ms){
	com_inv_milis = HAL_GetTick();
	if (com_inv_milis - com_inv_milis_ant >= com_inv_periodo_ms) {
		//insira aqui
		flag_msg_ant_inversor[MOTOR_DIR] = flag_msg_nova_inversor[MOTOR_DIR];
		flag_msg_ant_inversor[MOTOR_ESQ] = flag_msg_nova_inversor[MOTOR_ESQ];
		flag_msg_nova_inversor[MOTOR_DIR] = false;
		flag_msg_nova_inversor[MOTOR_ESQ] = false;
		if(flag_msg_ant_inversor[MOTOR_ESQ] == false || flag_msg_ant_inversor[MOTOR_ESQ] == false) {
			erro_com = true;
			desabilitar = true;
		} else {
			desabilitar = false;
			erro_com = false;
		}
//		uint8_t msg_debug[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
//		print_can(msg_debug, 0x10);
		// se deu algum erro, piscar led a cada tempo
		//if (erro_com > 0) {
			//aciona_sirene(5);

//		}
		com_inv_milis_ant = com_inv_milis;
	}
}


void mensagem_CANSPI_recebida(uint16_t id, uint8_t* data) {
	if (id == 0x000) {
		flag_rtds = 1;
	}
}

//Converte resultado do ADC de int32 para int16
//Retorna em ADC_DMA
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	for (int i = 0; i < 6; i++) {
		buffer[i] &= 0x000FFFF;			// mascara para filtrar os dois primeiros bytes
		ADC_DMA[i] = (uint16_t) buffer[i];
	}
}

void inicializa_adc_dma() {
	HAL_ADC_Start_DMA(&hadc1, buffer, 6);
}

//	Le acelerador: adc le os dois sensores por n = num_am ostras vezes,
//	faz uma filtragem de media, verifica plausabilidade,
//	calcula valor do acelerador.
//	Retorna acelerador
uint16_t le_acelerador(uint8_t *flag_error) {
	uint16_t APPS1 = ADC_DMA[0];
	uint16_t APPS2 = ADC_DMA[3];
	uint16_t apps1_calc = 0;
	uint16_t BSE = ADC_DMA[1];
	if (APPS2 < 260) {
		acelerador = 0;
	}

	if (APPS2 >= 260 && APPS2 < 467) {
		acelerador = (uint16_t) (1.162 * APPS2 - 342.9);
	}

	if (APPS2 >= 467 && APPS2 < 1065) {
		acelerador = (uint16_t) (0.3344 * APPS2 + 43.8);
	}

	if (APPS2 >= 1065 && APPS2 < 2253) {
		acelerador = (uint16_t) (0.1684 * APPS2 + 220.7);
	}

	if (APPS2 >= 2253 && APPS2 < 3211) {
		acelerador = (uint16_t) (0.2087 * APPS2 + 129.9);
	}

	if (APPS2 >= 3211 && APPS2 < 3720) {
		acelerador = (uint16_t) (0.6598 * APPS2 - 1319);
	}
	if (APPS2 >= 3720)
		acelerador = 0;

	if (acelerador >= 0 && acelerador < 200)
		apps1_calc = 2212;
	if (acelerador >= 200 && acelerador < 400)
		apps1_calc = (uint16_t) (1.679 * acelerador + 1876);
	if (acelerador >= 400 && acelerador < 600)
		apps1_calc = (uint16_t) (2.621 * acelerador + 1499);
	if (acelerador >= 600 && acelerador < 800)
		apps1_calc = (uint16_t) (2.212 * acelerador + 1745);
	if (acelerador >= 800 && acelerador < 1135)
		apps1_calc = (uint16_t) (1.515 * acelerador + 2302);

	if (APPS1 > 3900 || APPS1 < 1802.24)
		acelerador = 0;
	if (APPS1 < apps1_calc * 0.9 || APPS1 > apps1_calc * 1.1)
		acelerador = 0;
	if (acelerador > 1000)
		acelerador = 1000;

	if (acelerador < 250 && BSE < 2200) {
		*flag_error = 0;
		apps_t_flag = 1;
	}

	if (BSE > 2200)
		freio = 1;
	else
		freio = 0;

	if (((acelerador > 300 && BSE > 2200) )) {
		*flag_error = 1;
		acelerador = 0;
	}
	if (*flag_error == 1) {
		acelerador = 0;
		//Error_Handler();
	}

	return (acelerador);
}

uint16_t le_volante() {

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

	return (volante);
}

//Calcula referencia de torque e velocidade baseado na dinamica e sensores do carro
//Parametros:	acelerador, speed_t_total[4], sens_freio, frenagem_regenerativa
//Retorna:		habilita, refTorque[2], refTorque_neg[2], refveloc
void controle() {
	toggle_regen();

	//veloc_total = (speed_t_total[0] + speed_t_total[1] + speed_t_total[2] + speed_t_total[3]) / 4;
	veloc_total = (vel_motor[MOTOR_ESQ]+vel_motor[MOTOR_DIR])*0.5;
	if (desabilitar == true){
		estado = DESABILITA;
	}

		if ((acelerador < 100) && (frenagem_regenerativa == true)
				&& vel_motor[MOTOR_ESQ] > _5_kmph_rpm)
	{
		seta_leds(AZUL);	// se frenagem ativa, led da ecu indica BRANCO
		estado = FREIA;
	}
	else if(acelerador > 100)
	{
		seta_leds(modo_selecionado.cor); // quando desligada, volta a cor do modo
		estado = ACELERA;
	}
	else if (botao_habilita_acionado() && mode_slc == 4)
	{
		estado = REVERSE;
	}
	else
	{
		seta_leds(modo_selecionado.cor);	// quando desligada, volta a cor do modo
		estado = NEUTRO;
	}

	switch(estado){
		case NEUTRO:
			habilita = true;
			freiar = false;
			//Comando de magnetizacao dependente da velocidade:
			runstop = (vel_motor[MOTOR_DIR] > _5_kmph_rpm || vel_motor[MOTOR_ESQ] > _5_kmph_rpm);
			refTorque[MOTOR_DIR] =  0;
			refTorque[MOTOR_ESQ] =  0;
			refTorqueNeg[MOTOR_DIR] =  0;
			refTorqueNeg[MOTOR_ESQ] =  0;
			refVeloc[MOTOR_DIR] = modo_selecionado.vel_max;
			refVeloc[MOTOR_ESQ] = modo_selecionado.vel_max;
			regen_active = false;
		break;

		case FREIA:
			habilita = true;
			freiar = modo_selecionado.freio_regen;
			runstop = true;
			rev = false;
			refTorque[MOTOR_DIR] =  0;
			refTorque[MOTOR_ESQ] =  0;
			refTorqueNeg[MOTOR_DIR] =  torq_frenagem;
			refTorqueNeg[MOTOR_ESQ] =  torq_frenagem;
			refVeloc[MOTOR_DIR] = 0;
			refVeloc[MOTOR_ESQ] = 0;
			regen_active = true;
			funct_flags |= REGEN;
		break;

		case ACELERA:
			habilita = true;
			freiar = false;
			runstop = true;
			rev = false;
			refTorque[MOTOR_DIR] = (uint16_t) (modo_selecionado.torq_gain * acelerador) / 10;
			refTorque[MOTOR_ESQ] = (uint16_t) (modo_selecionado.torq_gain * acelerador) / 10;
			refVeloc[MOTOR_DIR] = modo_selecionado.vel_max;
			refVeloc[MOTOR_ESQ] = modo_selecionado.vel_max;
			refTorqueNeg[MOTOR_DIR] =  0;
			refTorqueNeg[MOTOR_ESQ] =  0;

			rampa_torque();
			//if (modo_selecionado.traction_control == true) tc_system();
			//else torque_vectoring();
			regen_active = false;
		break;

		case REVERSE:
			habilita = true;
			freiar = false;
			runstop = true;
			rev = true;
			refTorque[MOTOR_DIR] = 0;
			refTorque[MOTOR_ESQ] = 0;
			refVeloc[MOTOR_DIR] = 500;
			refVeloc[MOTOR_ESQ] = 500;
			refTorqueNeg[MOTOR_DIR] =  300; // 30%
			refTorqueNeg[MOTOR_ESQ] =  300;
			regen_active = false;
		break;

		case DESABILITA:
			habilita = false;
			freiar = false;
			runstop = false;
			rev = false;
			refTorque[MOTOR_DIR] =  0;
			refTorque[MOTOR_ESQ] =  0;
			refTorqueNeg[MOTOR_DIR] =  0;
			refTorqueNeg[MOTOR_ESQ] =  0;
			refVeloc[MOTOR_DIR] = 0;
			refVeloc[MOTOR_ESQ] = 0;
			regen_active = false;
		break;
	}
}

// funcao de teste para enviar todos os dados de uma vez para sicronizar os IDs com o TC
// as flags determinam a frequencia de envio dos dados

void actual_datalogger() {
	if (flag_can == 1) //variï¿½vel "flag_dtl" assume o valor "2" no final da main
	{
		time_actual = (HAL_GetTick() - time_init) / 100; //Calcula a diferenca de tempo e transforma para decimos de segundo

		vetTx[0] = time_actual;
		vetTx[1] = time_actual >> 8;
		vetTx[2] = volante;
		vetTx[3] = volante >> 8;
		vetTx[4] = acelerador;
		vetTx[5] = acelerador >> 8;
		vetTx[6] = freio;
		vetTx[7] = freio >> 8;

		CANSPI_Transmit(101, 8, vetTx);
		CANSPI_Transmit(351, 8, vetTx);


		flag_can = 2;
	}

	else if(flag_can == 2)
	{
		//ALARMES A SEREM IMPLEMENTADOS
		vetTx[0] = modo_selecionado.mode;
		vetTx[1] = modo_selecionado.mode >> 8;
		vetTx[2] = 0;
		vetTx[3] = 0;
		vetTx[4] = dist_calc;
		vetTx[5] = dist_calc >>8;
		vetTx[6] = 0;
		vetTx[7] = 0;

		CANSPI_Transmit(102, 8, vetTx);

		flag_can = 3;

	}

	else if (flag_can == 3)
	{

		vetTx[0] = torque[MOTOR_DIR];
		vetTx[1] = torque[MOTOR_DIR] >> 8;
		vetTx[2] = torque[MOTOR_ESQ];
		vetTx[3] = torque[MOTOR_ESQ] >> 8;
		vetTx[4] = refTorque[MOTOR_DIR];
		vetTx[5] = refTorque[MOTOR_DIR] >> 8;
		vetTx[6] = refTorque[MOTOR_ESQ];
		vetTx[7] = refTorque[MOTOR_ESQ] >> 8;
		CANSPI_Transmit(103, 8, vetTx);


		flag_can = 4;
	}

	else if (flag_can == 4)
	{
		vetTx[0] = vel_motor[MOTOR_DIR];
		vetTx[1] = vel_motor[MOTOR_DIR] >> 8;
		vetTx[2] = vel_motor[MOTOR_ESQ];
		vetTx[3] = vel_motor[MOTOR_ESQ] >> 8;
		vetTx[4] = torq_frenagem; //frenagem
		vetTx[5] = torq_frenagem >> 8; //frenagem
		vetTx[6] = (uint16_t) regen_bk_selection;
		vetTx[7] = ((uint16_t) regen_bk_selection) >> 8;

		CANSPI_Transmit(104, 8, vetTx);
		CANSPI_Transmit(350, 8, vetTx);

		flag_can = 5;
	}

	else if (flag_can == 5)
	{
		vetTx[0] = vel_roda[RODA_DIANT_ESQ];
		vetTx[1] = vel_roda[RODA_DIANT_ESQ] >> 8;
		vetTx[2] = vel_roda[RODA_DIANT_DIR];
		vetTx[3] = vel_roda[RODA_DIANT_DIR] >> 8;
		vetTx[4] = vel_roda[RODA_TRAS_DIR];
		vetTx[5] = vel_roda[RODA_TRAS_DIR] >> 8;
		vetTx[6] = vel_roda[RODA_TRAS_ESQ];
		vetTx[7] = vel_roda[RODA_TRAS_ESQ] >> 8;
	//	CANSPI_Transmit(352, 8, vetTx);

		CANSPI_Transmit(105, 8, vetTx);
		flag_can = 6;
	}

	else if (flag_can == 6)
	{

		vetTx[0] = (uint16_t) modo_selecionado.torq_gain;
		vetTx[1] = ((uint16_t) modo_selecionado.torq_gain) >> 8;
		vetTx[2] = 0;
		vetTx[3] = 0;
		vetTx[4] = corr_torque[MOTOR_DIR];
		vetTx[5] = corr_torque[MOTOR_DIR] >> 8;
		vetTx[6] = corr_torque[MOTOR_ESQ];
		vetTx[7] = corr_torque[MOTOR_ESQ] >> 8;

		CANSPI_Transmit(106, 8, vetTx);

		flag_can = 7;
	}
	else if (flag_can == 7){

		vetTx[0] = temp1_mosf[MOTOR_DIR];
		vetTx[1] = temp1_mosf[MOTOR_DIR] >> 8;
		vetTx[2] = temp2_mosf[MOTOR_DIR];
		vetTx[3] = temp2_mosf[MOTOR_DIR] >> 8;
		vetTx[4] = temp1_mosf[MOTOR_ESQ];
		vetTx[5] = temp1_mosf[MOTOR_ESQ] >> 8;
		vetTx[6] = temp2_mosf[MOTOR_ESQ];
		vetTx[7] = temp2_mosf[MOTOR_ESQ] >> 8;

		CANSPI_Transmit(107, 8, vetTx);

		flag_can = 8;
	}
	else if (flag_can == 8){
		uint8_t flag_corrente=0;
		if(estado == ACELERA || estado == FREIA)
		{
			flag_corrente=1;
		}
		vetTx[0] = modo_selecionado.mode;
		vetTx[1] = 0;
		vetTx[2] = status_datalog;
		vetTx[3] = 0;
		vetTx[4] = 0;
		vetTx[5] = 0;
		vetTx[6] = flag_corrente;
		vetTx[7] = 0;
		check_error = CANSPI_Transmit(1	, 8, vetTx);
		flag_can = 1;
	}
	// IDs para debug usando painel
	else if (flag_can == 9) {
		vetTx[0] = acelerador;
		vetTx[1] = acelerador >> 8;
		vetTx[2] = erro_com;
		vetTx[3] = 0;
		vetTx[4] = volante_cru;
		vetTx[5] = volante_cru >> 8;
		vetTx[6] = 0;
		vetTx[7] = 0;
		check_error = CANSPI_Transmit(351, 8, vetTx);
		flag_can = 1;
	}
}


void init_datalogger() {
	vetTx[0] = modo_selecionado.mode;
	vetTx[1] = 0;
	vetTx[2] = 0;
	vetTx[3] = 0;
	vetTx[4] = 0;
	vetTx[5] = 0;
	vetTx[6] = 0;
	vetTx[7] = 0;
	check_error = CANSPI_Transmit(0x710, 8, vetTx);
}

// Mensagem para o TC iniciar datalogger
// Envia modo e flag 0b100 para id 0x310
void start_datalogger() {

	status_datalog = 1;
	time_init = HAL_GetTick();
}

// Mensagem para o TC parar datalogger
// Envia modo e flag 0b10000 para id 0x310
void stop_datalogger() {
	vetTx[0] = modo_selecionado.mode;
	vetTx[1] = 0;
	vetTx[2] = 0;
	vetTx[3] = 0;
	vetTx[4] = 1;
	vetTx[5] = 0;
	vetTx[6] = 0;
	vetTx[7] = 0;
	check_error = CANSPI_Transmit(0x710, 8, vetTx);
}

//Envia a mesnagem de controle para o barramento can2, para controlar os inversores
//Parametros: hailita, refTorque, refTorq_neg, refVeloc
void comando_inversor() {
	paramControl = (habilita) + (runstop << 1) + (freiar << 2) + (acelerar << 3) + (rev << 4);

	if (VETORIZACAO_TORQUE == 1) {
		if (roda_interna == DIREITA) {
			if (flag_estado_comando_inversor == 0 && (selected_motors == MOTOR_DIR || selected_motors == MOTORESQ_DIR)) {
				vetTx[0] = paramControl;
				vetTx[1] = paramControl >> 8;
				vetTx[2] = 0; // data 2 e 3 para velocidade
				vetTx[3] = 0;
				vetTx[4] = refTorqueNeg[MOTOR_DIR] & 0xff;
				vetTx[5] = refTorqueNeg[MOTOR_DIR] >> 8 & 0xff;
				vetTx[6] = refVeloc[MOTOR_DIR] & 0xff;
				vetTx[7] = refVeloc[MOTOR_DIR] >> 8 & 0xff;
				CAN_Transmit(vetTx, id_msg_controle_dir);
				delay_ms_ecu(10);
				flag_estado_comando_inversor = 1;
			}

			if (flag_estado_comando_inversor == 1 && (selected_motors == MOTOR_ESQ || selected_motors == MOTORESQ_DIR)) {
				vetTx[0] = paramControl;
				vetTx[1] = paramControl >> 8;;
				vetTx[2] = refTorque[MOTOR_ESQ] & 0xff; // data 2 e 3 para velocidade
				vetTx[3] = refTorque[MOTOR_ESQ] >> 8 & 0xff;
				vetTx[4] = refTorqueNeg[MOTOR_ESQ] & 0xff;
				vetTx[5] = refTorqueNeg[MOTOR_ESQ]>>8 & 0xff;
				vetTx[6] = refVeloc[MOTOR_ESQ] & 0xff;
				vetTx[7] = refVeloc[MOTOR_ESQ] >> 8 & 0xff;
				CAN_Transmit(vetTx, id_msg_controle_esq);
				delay_ms_ecu(10);
				flag_estado_comando_inversor = 2;
			}
		}

		if (roda_interna == ESQUERDA) {
			if (flag_estado_comando_inversor == 0 && (selected_motors == MOTOR_DIR || selected_motors == MOTORESQ_DIR)) {
				vetTx[0] = paramControl;
				vetTx[1] = paramControl >> 8;
				vetTx[2] = refTorque[MOTOR_DIR] & 0xff; // data 2 e 3 para velocidade
				vetTx[3] = refTorque[MOTOR_DIR] >> 8 & 0xff;
				vetTx[4] = refTorqueNeg[MOTOR_DIR] & 0xff;
				vetTx[5] = refTorqueNeg[MOTOR_DIR] >> 8 & 0xff;
				vetTx[6] = refVeloc[MOTOR_DIR] & 0xff;
				vetTx[7] = refVeloc[MOTOR_DIR] >> 8 & 0xff;
				CAN_Transmit(vetTx, id_msg_controle_dir);
				delay_ms_ecu(10);
				flag_estado_comando_inversor = 1;
			}

			if (flag_estado_comando_inversor == 1 && (selected_motors == MOTOR_ESQ || selected_motors == MOTORESQ_DIR)) {
				vetTx[0] = paramControl;
				vetTx[1] = paramControl >> 8;;
				vetTx[2] = 0; // data 2 e 3 para velocidade
				vetTx[3] = 0;
				vetTx[4] = refTorqueNeg[MOTOR_ESQ] & 0xff;
				vetTx[5] = refTorqueNeg[MOTOR_ESQ]>>8 & 0xff;
				vetTx[6] = refVeloc[MOTOR_ESQ] & 0xff;
				vetTx[7] = refVeloc[MOTOR_ESQ] >> 8 & 0xff;
				CAN_Transmit(vetTx, id_msg_controle_esq);
				delay_ms_ecu(10);
				flag_estado_comando_inversor = 2;
			}
		}
	}

	if (VETORIZACAO_TORQUE == 2) {

		float proporcionalAux;
		uint16_t torqueAux_direito;
		uint16_t torqueAux_esquerdo;
		uint16_t direcao_volante;

		direcao_volante = volante - VOLANTE_ALINHADO;									//verifica o lado de viragem (positivo:esquerdo, negativo:direito)

		if (volante < 0) {
			proporcionalAux = volante / (VOLANTE_MIN * GANHO_VOLANTE - ZERO_VOLANTE);	//calcula a porcentagem de estercamento pro lado direito
		} else {
			proporcionalAux = volante / (VOLANTE_MAX * GANHO_VOLANTE - ZERO_VOLANTE);	//calcula a porcentagem de estercamento pro lado esquerdo
		}

		proporcionalAux = 1 - proporcionalAux;
		torqueAux_direito = proporcionalAux * refTorque[MOTOR_DIR];
		torqueAux_esquerdo = proporcionalAux * refTorque[MOTOR_ESQ];

		if (roda_interna == DIREITA) {
			if (flag_estado_comando_inversor == 0 && (selected_motors == MOTOR_DIR || selected_motors == MOTORESQ_DIR)) {
				vetTx[0] = paramControl;
				vetTx[1] = paramControl >> 8;
				vetTx[2] = torqueAux_direito & 0xff; // data 2 e 3 para velocidade
				vetTx[3] = torqueAux_direito >> 8 & 0xff;
				vetTx[4] = refTorqueNeg[MOTOR_DIR] & 0xff;
				vetTx[5] = refTorqueNeg[MOTOR_DIR] >> 8 & 0xff;
				vetTx[6] = refVeloc[MOTOR_DIR] & 0xff;
				vetTx[7] = refVeloc[MOTOR_DIR] >> 8 & 0xff;
				CAN_Transmit(vetTx, id_msg_controle_dir);
				delay_ms_ecu(10);
				flag_estado_comando_inversor = 1;
			}

			if (flag_estado_comando_inversor == 1 && (selected_motors == MOTOR_ESQ || selected_motors == MOTORESQ_DIR)) {
				vetTx[0] = paramControl;
				vetTx[1] = paramControl >> 8;;
				vetTx[2] = refTorque[MOTOR_ESQ] & 0xff; // data 2 e 3 para velocidade
				vetTx[3] = refTorque[MOTOR_ESQ] >> 8 & 0xff;
				vetTx[4] = refTorqueNeg[MOTOR_ESQ] & 0xff;
				vetTx[5] = refTorqueNeg[MOTOR_ESQ]>>8 & 0xff;
				vetTx[6] = refVeloc[MOTOR_ESQ] & 0xff;
				vetTx[7] = refVeloc[MOTOR_ESQ] >> 8 & 0xff;
				CAN_Transmit(vetTx, id_msg_controle_esq);
				delay_ms_ecu(10);
				flag_estado_comando_inversor = 2;
			}
		}
		if (roda_interna == ESQUERDA) {
			if (flag_estado_comando_inversor == 0 && (selected_motors == MOTOR_DIR || selected_motors == MOTORESQ_DIR)) {
				vetTx[0] = paramControl;
				vetTx[1] = paramControl >> 8;
				vetTx[2] = refTorque[MOTOR_DIR] & 0xff; // data 2 e 3 para velocidade
				vetTx[3] = refTorque[MOTOR_DIR] >> 8 & 0xff;
				vetTx[4] = refTorqueNeg[MOTOR_DIR] & 0xff;
				vetTx[5] = refTorqueNeg[MOTOR_DIR] >> 8 & 0xff;
				vetTx[6] = refVeloc[MOTOR_DIR] & 0xff;
				vetTx[7] = refVeloc[MOTOR_DIR] >> 8 & 0xff;
				CAN_Transmit(vetTx, id_msg_controle_dir);
				delay_ms_ecu(10);
				flag_estado_comando_inversor = 1;
			}

			if (flag_estado_comando_inversor == 1 && (selected_motors == MOTOR_ESQ || selected_motors == MOTORESQ_DIR)) {
				vetTx[0] = paramControl;
				vetTx[1] = paramControl >> 8;;
				vetTx[2] = torqueAux_esquerdo & 0xff; // data 2 e 3 para velocidade
				vetTx[3] = torqueAux_esquerdo >> 8 & 0xff;
				vetTx[4] = refTorqueNeg[MOTOR_ESQ] & 0xff;
				vetTx[5] = refTorqueNeg[MOTOR_ESQ]>>8 & 0xff;
				vetTx[6] = refVeloc[MOTOR_ESQ] & 0xff;
				vetTx[7] = refVeloc[MOTOR_ESQ] >> 8 & 0xff;
				CAN_Transmit(vetTx, id_msg_controle_esq);
				delay_ms_ecu(10);
				flag_estado_comando_inversor = 2;
			}
		}
	}

	if(VETORIZACAO_TORQUE == 0){
		if (flag_estado_comando_inversor == 0 && (selected_motors == MOTOR_DIR || selected_motors == MOTORESQ_DIR)) {
			vetTx[0] = paramControl;
			vetTx[1] = paramControl >> 8;
			vetTx[2] = refTorque[MOTOR_DIR] & 0xff; // data 2 e 3 para velocidade
			vetTx[3] = refTorque[MOTOR_DIR] >> 8 & 0xff;
			vetTx[4] = refTorqueNeg[MOTOR_DIR] & 0xff;
			vetTx[5] = refTorqueNeg[MOTOR_DIR] >> 8 & 0xff;
			vetTx[6] = refVeloc[MOTOR_DIR] & 0xff;
			vetTx[7] = refVeloc[MOTOR_DIR] >> 8 & 0xff;
			CAN_Transmit(vetTx, id_msg_controle_dir);
			delay_ms_ecu(10);
			flag_estado_comando_inversor = 1;
		}

		if (flag_estado_comando_inversor == 1 && (selected_motors == MOTOR_ESQ || selected_motors == MOTORESQ_DIR)) {
			vetTx[0] = paramControl;
			vetTx[1] = paramControl >> 8;;
			vetTx[2] = refTorque[MOTOR_ESQ] & 0xff; // data 2 e 3 para velocidade
			vetTx[3] = refTorque[MOTOR_ESQ] >> 8 & 0xff;
			vetTx[4] = refTorqueNeg[MOTOR_ESQ] & 0xff;
			vetTx[5] = refTorqueNeg[MOTOR_ESQ]>>8 & 0xff;
			vetTx[6] = refVeloc[MOTOR_ESQ] & 0xff;
			vetTx[7] = refVeloc[MOTOR_ESQ] >> 8 & 0xff;
			CAN_Transmit(vetTx, id_msg_controle_esq);
			delay_ms_ecu(10);
			flag_estado_comando_inversor = 2;
		}

		//MSG para verificar a integridade da comunicacao
		if (flag_estado_comando_inversor == 2){

		vetTx[0] = 1;
		vetTx[1] = 0;
		vetTx[2] = 0;
		vetTx[3] = 0;
		vetTx[4] = vel_motor[MOTOR_ESQ] & 0xff;
		vetTx[5] = vel_motor[MOTOR_ESQ] >> 8 & 0xff;
		vetTx[6] = vel_motor[MOTOR_DIR] & 0xff;
		vetTx[7] = vel_motor[MOTOR_DIR] >> 8 & 0xff;
		CAN_Transmit(vetTx, id_flag_comunic);
		delay_ms_ecu(10);
		flag_estado_comando_inversor = 0;
		}
	}
}

//Retransmite as mensagens do barramento can2 para o barramento can1
//Chamada no can recieve handler
//Parametros: data, vetor de dados da ultima mensagem recebida
//Retorna: nada
void transmite_inversores_datalogger(uint16_t id, uint8_t* data) {
	check_error = CANSPI_Transmit(id, 8, data);
}

// Armazena dados da ultima mensagem recebida dos inversores nas respectivas variaveis e retransmite para can1
void mensagem_inversor_recebida(uint16_t id, uint8_t* data) {

	if(id > 0x99 && id < 0x150){
		flag_msg_nova_inversor[MOTOR_ESQ] = true;
	} else if(id > 0x199 && id < 0x250){
		flag_msg_nova_inversor[MOTOR_DIR] = true;
	}
	uint16_t* data_word = (uint16_t*) data;
	switch (id) {
	//INV_ME
	case 0x100:
		vel_motor[MOTOR_ESQ] = 				data_word[0];
		torque[MOTOR_ESQ] = 				data_word[1];
		potencia[MOTOR_ESQ] =		 		data_word[2];
		corr_torque[MOTOR_ESQ] =			data_word[3];
		break;
	case 0x101:
		energia_consumida[MOTOR_ESQ] = 		data_word[0];
		valor_sobrecarga[MOTOR_ESQ] = 		data_word[1];
		temp1_mosf[MOTOR_ESQ] = 			data_word[2];
		temp2_mosf[MOTOR_ESQ] = 			data_word[3];
		break;
	case 0x102:
		msg_perdidas[MOTOR_ESQ] =			data_word[0];
		cont_busoff[MOTOR_ESQ] =			data_word[1];
		estado_can[MOTOR_ESQ] = 			data_word[2];
		break;
	case 0x103:
		estado_inv[MOTOR_ESQ] = 			data_word[0];
		falha_atual[MOTOR_ESQ] = 			data_word[1];
		alarme_atual[MOTOR_ESQ] = 			data_word[2];
		break;

		//INV_MD
	case 0x200:
		vel_motor[MOTOR_DIR] = 				data_word[0];
		torque[MOTOR_DIR] = 				data_word[1];
		potencia[MOTOR_DIR] = 				data_word[2];
		corr_torque[MOTOR_DIR] = 			data_word[3];
		break;
	case 0x201:
		energia_consumida[MOTOR_DIR] = 		data_word[0];
		valor_sobrecarga[MOTOR_DIR] = 		data_word[1];
		temp1_mosf[MOTOR_DIR] =				data_word[2];
		temp2_mosf[MOTOR_DIR] = 			data_word[3];
		break;
	case 0x202:
		msg_perdidas[MOTOR_DIR] = 			data_word[0];
		cont_busoff[MOTOR_DIR] = 			data_word[1];
		estado_can[MOTOR_DIR] = 			data_word[2];
		break;
	case 0x203:
		estado_inv[MOTOR_DIR] = 			data_word[0];
		falha_atual[MOTOR_DIR] = 			data_word[1];
		alarme_atual[MOTOR_DIR] = 			data_word[2];
		break;
	}
}

//envia um vetor data[4] para o barramento can1, pra debug
void print_can(uint8_t id, uint16_t* data) {
	check_error = CANSPI_Transmit(id, 8, (uint8_t*) data);
}

int16_t PID_control(int16_t setpoint, int16_t medida) {
	uint16_t error, dif_error, out;

	error = setpoint - medida;

	error_area = error * time_speed_refresh / 40; // time_speed_refrash dividido por 40 para dar valor em ms
	if (integral_ant == 9)
		integral_ant = 0;
	else
		integral_ant = integral_ant + 1;
	integral_error = integral_error - integral_buffer[integral_ant]
			+ error_area;
	integral_buffer[integral_ant] = error_area;
	if (integral_error > MAX_INTEGRAL_ERROR)
		integral_error = MAX_INTEGRAL_ERROR;

	if (time_speed_refresh == 0)
		dif_error = 0;
	else
		dif_error = (error - last_error) * 40 / time_speed_refresh; //time_speed_refrash dividido por 40 para dar valor em ms
	last_error = error;

	out = GAIN_PID * (KP * error + KI * integral_error - KD * dif_error);

	return (out);
}

void Vel_Calc() //calcula velocidades em rpm, ou decimos de km/h

//velocidade m/s = omega*circunferencia = delta_theta/delta_tempo * 2*pi*raio
//              = (raio*2*pi/16)/delta_t (m/s)
//              = (raio*2*pi/16)/(n*25u) (m/s)
//              = 40khz*(raio*2*pi/16)/n (m/s)
//              = 3.6*40khz*(raio*2*pi/16)/n (km/h)
//              = 10*3.6*40khz*(raio*2*pi/16)/n (0.1km/h)
//              = 10*3.6*40khz*(0.26*2*pi/16)/n (0.1km/h) = 147026/n 
{
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

void Dist_Calc() //calcula o delta de distancia percorrida em 1 segundo e retorna o valor em decimetro
{
	if (dist_pr >= 10) {	// timer de 1 segundo
		dist_pr = 0;

		// transforma a velocidade para dm/s e multiplica pelo tempo de execu��o da main em ms
		// o tempo eh dividido por 1000 para a unidade de tempo ficar em segundos
		dist_calc = (media_diant/3.6) * (tempo_final/1000);
	}
}

void Odometer() {
	const static uint8_t TEST_DIST = 0;
	const static uint8_t OVERALL_DIST = 1;

	// calculates delta
	Dist_Calc();

	// verifica se o calculo da distancia ja deu 1 metro ou mais - valor retornado em dist_calc esta em decimetro

	if ((dist_calc / 10) >= 1) {
		dist_log[TEST_DIST] += (dist_calc / 10);		// updates test and overall distance
		dist_log[OVERALL_DIST] += (dist_calc / 10);		// updates distance already travelled

		// rounds to the nearest meter
		Record_Distance(dist_log);						// writes values in flash memory
	}
}

void Diferencial() {
	int16_t setpoint, medida, media_tras, pid_return;
	Vel_Calc();

	if (media_diant == 0)
		setpoint = 0;
	else
		setpoint = (int16_t) 100 * (vel_roda[RODA_DIANT_DIR] - vel_roda[RODA_DIANT_ESQ]) / media_diant; //se positivo roda direita mais rapida. Se negativo, roda esq mais rapida

	media_tras = (vel_roda[RODA_TRAS_DIR] + vel_roda[RODA_TRAS_ESQ]) >> 1;
	if (media_tras == 0)
		medida = 0;
	else
		medida = (int16_t) 100 * (vel_roda[RODA_TRAS_DIR] - vel_roda[RODA_TRAS_ESQ]) / media_tras; //se positivo roda direita mais rapida. Se negativo, roda esq mais rapida
	if (media_diant > 10)
		pid_return = PID_control(setpoint, medida);
	else {
		PID_control(setpoint, medida);
		pid_return = 0;
	}

	if (pid_return > TORQUE_GAIN * acelerador) {
		refTorque[MOTOR_DIR] = 0;
		refTorque[MOTOR_ESQ] = 2 * modo_selecionado.torq_gain * acelerador;
	} else if ((pid_return * (-1))
			> (int16_t) modo_selecionado.torq_gain * acelerador) {
		refTorque[MOTOR_DIR] = 2 * modo_selecionado.torq_gain * acelerador;
		refTorque[MOTOR_ESQ] = 0;
	} else {
		refTorque[MOTOR_DIR] = (uint16_t) (modo_selecionado.torq_gain
				* (acelerador - pid_return)) / 10;
		refTorque[MOTOR_ESQ] = (uint16_t) (modo_selecionado.torq_gain
				* (acelerador + pid_return)) / 10;
	}

	if (refTorque[MOTOR_DIR] > modo_selecionado.torq_gain * 1000)
		refTorque[MOTOR_DIR] = modo_selecionado.torq_gain * 1000;
	if (refTorque[MOTOR_ESQ] > modo_selecionado.torq_gain * 1000)
		refTorque[MOTOR_ESQ] = modo_selecionado.torq_gain * 1000;

	/*
 //diferencial
	if(selecionado.dif_elt == 1 && volante > 60)
		Diferencial_2();
	else if(selecionado.arranc_control == 1 && volante < 15)
		Controle_arrancada();
	else
	{
	refTorque[MOTOR_DIR] = (uint16_t) (selecionado.torq_gain * acelerador) / 10;
	refTorque[MOTOR_ESQ] = (uint16_t) (selecionado.torq_gain * acelerador) / 10;
	refVeloc[MOTOR_DIR] = selecionado.vel_max;
	refVeloc[MOTOR_ESQ] = selecionado.vel_max;
	}
*/


}

void Diferencial_2() {
	int16_t setpoint, medida, pid_return;
	Vel_Calc();
	Funcao_Dif();

	if (media_diant == 0)
		medida = 0;
	else
		medida = (int16_t) (1000 * (vel_roda[0] - vel_roda[1])) / media_diant; //se positivo roda direita mais rapida. Se negativo, roda esq mais rapida

	if (roda_interna == 1)
		setpoint = (-1) * Funcao_Dif();
	else
		setpoint = Funcao_Dif();

	if (media_diant > 10)
		pid_return = PID_control(setpoint, medida);
	else {
		PID_control(setpoint, medida);
		pid_return = 0;
	}

	if (pid_return > TORQUE_GAIN * acelerador) {
		refTorque[MOTOR_DIR] = 0;
		refTorque[MOTOR_ESQ] = 2 * modo_selecionado.torq_gain * acelerador;
	} else if ((pid_return * (-1))
			> (int16_t) modo_selecionado.torq_gain * acelerador) {
		refTorque[MOTOR_DIR] = 2 * modo_selecionado.torq_gain * acelerador;
		refTorque[MOTOR_ESQ] = 0;
	} else {
		refTorque[MOTOR_DIR] = (uint16_t) (modo_selecionado.torq_gain
				* (acelerador - pid_return)) / 10;
		refTorque[MOTOR_ESQ] = (uint16_t) (modo_selecionado.torq_gain
				* (acelerador + pid_return)) / 10;
	}

	if (refTorque[MOTOR_DIR] > modo_selecionado.torq_gain * 1000)
		refTorque[MOTOR_DIR] = modo_selecionado.torq_gain * 1000;
	if (refTorque[MOTOR_ESQ] > modo_selecionado.torq_gain * 1000)
		refTorque[MOTOR_ESQ] = modo_selecionado.torq_gain * 1000;

	refVeloc[MOTOR_DIR] = modo_selecionado.vel_max;
	refVeloc[MOTOR_ESQ] = modo_selecionado.vel_max;
}

void inicializa_tab_dif() {

	prop_dif[0] = 0;
	prop_dif[1] = 16;
	prop_dif[2] = 33;
	prop_dif[3] = 50;
	prop_dif[4] = 68;
	prop_dif[5] = 86;
	prop_dif[6] = 104;
	prop_dif[7] = 123;
	prop_dif[8] = 142;
	prop_dif[9] = 162;
	prop_dif[10] = 182;
	prop_dif[11] = 203;
	prop_dif[12] = 224;
	prop_dif[13] = 246;
	prop_dif[14] = 269;
	prop_dif[15] = 293;
	prop_dif[16] = 317;
	prop_dif[17] = 343;
	prop_dif[18] = 369;
	prop_dif[19] = 397;
	prop_dif[20] = 426;
	prop_dif[21] = 456;
	prop_dif[22] = 488;
	prop_dif[23] = 521;
	prop_dif[24] = 556;
}

int16_t Funcao_Dif() {
	int16_t proporcao, vol_norm;
	if (volante < 5) {
		vol_norm = 0;
		//		vel_calculada[0] = media_diant;
		//		vel_calculada[1] = media_diant;
	} else if (volante > 120) {
		proporcao = prop_dif[23];
		vol_norm = 120 / 5;
	} else
		vol_norm = volante / 5;

	proporcao = prop_dif[vol_norm];

	return (proporcao);
	//	media_calc = (vel_calculada[0] + vel_calculada[1])>>1;
}

void Controle_arrancada() {

// velocidade da roda da frente e' 9x a da velocidade do encoder
	if (vel_roda[1] < 50)
		vel_calc_motor[MOTOR_DIR] = 50 * 9 * GAIN_ARRANC;
	else
		vel_calc_motor[MOTOR_DIR] = vel_roda[1] * 9 * GAIN_ARRANC;
	if (vel_roda[0] < 50)
		vel_calc_motor[MOTOR_ESQ] = 50 * 9 * GAIN_ARRANC;
	else
		vel_calc_motor[MOTOR_ESQ] = vel_roda[0] * 9 * GAIN_ARRANC;

	if (vel_calc_motor[MOTOR_DIR] > modo_selecionado.vel_max)
		refVeloc[MOTOR_DIR] = modo_selecionado.vel_max;
	else
		refVeloc[MOTOR_DIR] = vel_calc_motor[MOTOR_DIR];

	if (vel_calc_motor[MOTOR_ESQ] > modo_selecionado.vel_max)
		refVeloc[MOTOR_ESQ] = modo_selecionado.vel_max;
	else
		refVeloc[MOTOR_ESQ] = vel_calc_motor[MOTOR_ESQ];


		/*

	 //Controle de arrancada
	 // Rampa de aceleracao maxima
	 // Nao permite que a velocidade aumente mais do que o incremento
	 	if(selecionado.arranc_control == 1)
	 {
	 if(refVeloc[MOTOR_DIR] > refVeloc_ant[MOTOR_DIR] + INC_VELOC)
	 {
	 refVeloc[MOTOR_DIR] = refVeloc_ant[MOTOR_DIR] + INC_VELOC;
	 }
	 refVeloc_ant[MOTOR_DIR]= refVeloc[MOTOR_DIR];

	 if(refVeloc[MOTOR_ESQ] > refVeloc_ant[MOTOR_ESQ] + INC_VELOC)
	 {
	 refVeloc[MOTOR_ESQ] = refVeloc_ant[MOTOR_ESQ] + INC_VELOC;
	 }
	 refVeloc_ant[MOTOR_ESQ]= refVeloc[MOTOR_ESQ];



	 if(refVeloc[MOTOR_DIR] < refVeloc_ant[MOTOR_DIR] - INC_VELOC)
	 {
	 refVeloc[MOTOR_DIR] = refVeloc_ant[MOTOR_DIR] - INC_VELOC;
	 }
	 refVeloc_ant[MOTOR_DIR]= refVeloc[MOTOR_DIR];

	 if(refVeloc[MOTOR_ESQ] < refVeloc_ant[MOTOR_ESQ] - INC_VELOC)
	 {
	 refVeloc[MOTOR_ESQ] = refVeloc_ant[MOTOR_ESQ] - INC_VELOC;
	 }
	 refVeloc_ant[MOTOR_ESQ]= refVeloc[MOTOR_ESQ];
	 }*/
}

void rampa_torque(){
	if (refTorque_ant[MOTOR_DIR] > TORQUE_INIT_LIMITE) {									// verifica se a referencia
			if (refTorque[MOTOR_DIR] > refTorque_ant[MOTOR_DIR] + INC_TORQUE) {				// ja passou do ponto de inflexao
				refTorque[MOTOR_DIR] = refTorque_ant[MOTOR_DIR] + INC_TORQUE;				// e aplica o incremento mais agressivo
			}
		} else {
			if (refTorque[MOTOR_DIR] > refTorque_ant[MOTOR_DIR] + INC_TORQUE_INIT) {		// caso contrario usa o
				refTorque[MOTOR_DIR] = refTorque_ant[MOTOR_DIR] + INC_TORQUE_INIT;			// incremento da primeira rampa
			}
		}
		refTorque_ant[MOTOR_DIR] = refTorque[MOTOR_DIR];									// aplica a referencia calculada

		// mesma logica para o motor esquerdo
		if (refTorque_ant[MOTOR_ESQ] > TORQUE_INIT_LIMITE) {
			if (refTorque[MOTOR_ESQ] > refTorque_ant[MOTOR_ESQ] + INC_TORQUE) {
				refTorque[MOTOR_ESQ] = refTorque_ant[MOTOR_ESQ] + INC_TORQUE;
			}
		} else {
			if (refTorque[MOTOR_ESQ] > refTorque_ant[MOTOR_ESQ] + INC_TORQUE_INIT) {
				refTorque[MOTOR_ESQ] = refTorque_ant[MOTOR_ESQ] + INC_TORQUE_INIT;
			}
		}
		refTorque_ant[MOTOR_ESQ] = refTorque[MOTOR_ESQ];

}

uint16_t wheel_slip(int selector) {
	if (media_diant == 0) {
		media_diant = 1; 	// para  nao dividir por 0 no inicio
	}
	// formula de wheel slip -> s = (Vw - Vveh)/Vveh
	// multiplica por 100 pra obter em porcentagem
	static uint16_t s;
	s = (uint8_t) (100*( vel_roda[selector] - media_diant)/ media_diant);
	return s;
}

void tc_system() {
	slip[0] = wheel_slip(RODA_TRAS_DIR);					// calcula wheel slip de cada roda trativa
	slip[1] = wheel_slip(RODA_TRAS_ESQ);
	slip_ratio[0] = (slip[0] / desired_slip_ratio) * 10;	// razao do deslizamento
	slip_ratio[1] = (slip[1] / desired_slip_ratio) * 10;	// multiplicado por 10 para ser inteiro

	if (slip[0] > UPPER_THRESHOLD || slip[1] > UPPER_THRESHOLD) {   			// se uma das duas for maior que
		if ((slip[0] / desired_slip_ratio) > 1) {								// 10% do desejado, verifica qual esta
			refTorque[MOTOR_DIR] = refTorque_ant[MOTOR_DIR] * DECR_OVER;		// acima e aplica um decrescimo na
		}																		// referencia de torque enviada aos motores
		if ((slip[1] / desired_slip_ratio) > 1) {
			refTorque[MOTOR_ESQ] = refTorque_ant[MOTOR_ESQ] * DECR_OVER;
		}
	} else {
		if (slip[0] > LOWER_THRESHOLD || slip[1] > LOWER_THRESHOLD) {						// um controle mais suave
			if (slip_ratio[0] > UNDER_MIN_LIMIT && slip_ratio[0] < UNDER_MAX_LIMIT) 	    // para reduzir a chance de
				refTorque[MOTOR_DIR] = refTorque_ant[MOTOR_DIR] * DECR_UNDER;				// overshoot do slip ratio
			if (slip_ratio[0] > UNDER_MAX_LIMIT)
				refTorque[MOTOR_DIR] = refTorque_ant[MOTOR_DIR] * DECR_MID;

			if (slip_ratio[1] > UNDER_MIN_LIMIT && slip_ratio[1] < UNDER_MAX_LIMIT)
				refTorque[MOTOR_ESQ] = refTorque_ant[MOTOR_ESQ] * DECR_UNDER;
			if (slip_ratio[1] > UNDER_MAX_LIMIT)
				refTorque[MOTOR_ESQ] = refTorque_ant[MOTOR_ESQ] * DECR_MID;
		}
	}
}

void torque_vectoring (void)
{
		uint16_t vol_gain_externo, vol_gain_interno, vel_diant_ext = 0 ;
			double const0=0.5, const1=-0.0005221, const2=0.000005772, const3= 0.00009419; //constatntes baseadas em simuala��o


			if(roda_interna == RODA_DIANT_DIR)	// curva para a direita
			{
				vel_diant_ext = vel_roda[RODA_DIANT_ESQ]  / 3.6 * 10; //transforma a velocidade de decimos de km/h em m/s
				vol_gain_externo = (const0 + const1*volante*vel_diant_ext + const2*volante*volante*vel_diant_ext + const3*volante*vel_diant_ext*vel_diant_ext) * 100;//equa��o baseada em simula��o de transferencia de peso
				vol_gain_interno = 100 - vol_gain_externo;//calcula percentual do ganho interno

				refTorque[MOTOR_DIR] = (uint16_t) (modo_selecionado.torq_gain * acelerador * vol_gain_interno ) / 500; //* 2) / 1000;
				refTorque[MOTOR_ESQ] = (uint16_t) (modo_selecionado.torq_gain * acelerador * vol_gain_externo ) / 500; //* 2) / 1000;	//faz a roda interna rodar mais devagar

			}
			if(roda_interna == RODA_DIANT_ESQ)	// curva para a esquerda
			{
				vel_diant_ext = vel_roda[RODA_DIANT_DIR] / 3.6 * 10; //transforma a velocidade de decimos de km/h em m/s
				vol_gain_externo = (const0 + const1*volante*vel_diant_ext + const2*volante*volante*vel_diant_ext + const3*volante*vel_diant_ext*vel_diant_ext) * 100;//equa��o baseada em simula��o de transferencia de peso
				vol_gain_interno = 100 - vol_gain_externo;//calcula percentual do ganho interno

				refTorque[MOTOR_DIR] = (uint16_t) (modo_selecionado.torq_gain * acelerador * vol_gain_externo ) / 500; //* 2) / 1000;	// envia o torque normalmente
				refTorque[MOTOR_ESQ] = (uint16_t) (modo_selecionado.torq_gain * acelerador * vol_gain_interno ) / 500; //*2) / 1000;	//faz a roda interna rodar mais devagar

			}
}

void toggle_regen() {
	if (botao_habilita_acionado()) {
		if (button_regen_bk == true) button_regen_bk = false;
		else button_regen_bk = true;
	}
}

// Energy Recovery System control
// driver chooses which intensity of regenerative braking on the steering wheel
// between LOW, MEDIUM, or HIGH
void ERS_control() {
	switch(regen_slc) {
	case 0:
		torq_frenagem = 0;
		break;
	case 1:
		torq_frenagem = 300;
		break;
	case 2:
		torq_frenagem = 500;
		break;
	case 3:
		torq_frenagem = 700;
		break;
	default:
		torq_frenagem = 0;
	}
}

/*
void efficiency_calc(){
	efficience = (vel_motor[MOTOR_DIR]*torque[MOTOR_DIR]+vel_motor[MOTOR_ESQ]*torque[MOTOR_ESQ])/(corrente_acum*tensao_acum);

}

*/

