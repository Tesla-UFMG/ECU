/*      Formula Tesla UFMG
 *     ECU_2018_V_2_00
 *     Marco 2019
 */

#include "main.h"
#include "ecu.h"
#include "sys.h"
#include "eeprom.h"

//    Variaveis globais
/*
 //Variaveis opcionais
extern uint16_t velocEncoder;
extern bool habilita;
extern uint8_t speed_t_flag[4];
extern uint32_t speed_t_total[4];
extern uint8_t apps_t_flag;
extern uint8_t flag_rtds;
extern uint16_t ADC_DMA[6];
extern uint8_t refresh_speed;
extern uint16_t vel_roda[4];
extern uint8_t roda_interna;
*/
extern uint8_t flag_dtl;
extern uint8_t flag_can;
extern uint8_t conta_t_dtl;
extern int16_t dist_calc, tempo_teste;
extern uint8_t dist_pr;
extern uint16_t acelerador;
extern uint16_t funct_flags;
extern uint32_t tempo_final;
modos modo_selecionado;

uint32_t tempo_main_inicial, tempo_final; //variaveis auxiliares para calcular o tempo do loop da main
uint16_t recorded_dist = 0;

int main(void)
{
	//      Init
	uint8_t flag_erro_apps = false, RTD_OK = false, toggle_led = false;
	inicializa_perifericos();
	//Ready to drive
	inicializa_modos();
	inicializa_tab_dif();

	while(RTD_OK == 0){

		modo_selecionado = le_chave_modo();
		if(toggle_led)
			seta_leds(modo_selecionado.cor);
		else
			seta_leds(PRETO);
		toggle_led ^= 1;
		delay_ms(200);
		if(pedal_freio_acionado() && botao_habilita_acionado() && contator_fechado()){
			aciona_sirene(200);
			RTD_OK = true;
			funct_flags |= RTD;
		}
		actual_datalogger();
		le_acelerador(&flag_erro_apps);
		controle();
		le_volante();

	}
	modo_selecionado = le_chave_modo();
	seta_leds(modo_selecionado.cor);
	seta_flags(modo_selecionado);
	//init_datalogger();

	// updates test counter
	Update_Test_Counter();
	// clears current counters for the test
	Error_Diagnostics_Clear();
	// clears previous test distance
	Clear_Test_Distance();
	// gets travelled distance for odometer
	recorded_dist = Get_Traveled_Distance();

	//      Loop
	while (true)
	{
		tempo_main_inicial = HAL_GetTick(); // usado para calcular o tempo do loop da main
		//atualiza_watchdog();
		le_acelerador(&flag_erro_apps);
		le_volante();
		Vel_Calc();
		// Dist_Calc();								// substituida por Odometro -- eh chamada dentro dela
		Odometer(recorded_dist);
		//ERS_control();							// em fase de implementacao
		controle();
		comando_inversor();
		//transmite_ecu_datalogger();				//Transmite dados correspondente a cada modo
		actual_datalogger();
		if(flag_dtl == 0)
			if(acelerador > 300)
			{
				start_datalogger();
				flag_dtl = 2;
				dist_calc = 0;
				tempo_teste = 0;
				dist_pr = 0;
			}
		if(conta_t_dtl > 10 && modo_selecionado.mode != ENDURO)
				{
					stop_datalogger();
					flag_dtl = 1;
					conta_t_dtl = 0;
				}

		tempo_final = HAL_GetTick() - tempo_main_inicial; // calcula o tempo de um loop da main
		// msg de debug com arduino na rede do tc

		// reads and records errors
		Error_Diagnostics_Read();
		//testa_comunic_inv(500);
//		atualiza_watchdog();
	}

}
