/*      Formula Tesla UFMG
 *     ECU_2018_V_2_00
 *     Marco 2019
 */

#include "main.h"
#include "ecu.h"
#include "sys.h"
#include "eeprom.h"
#include "pid.h"

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
extern uint16_t dist_log[2];
extern uint16_t mode_slc;

modos modo_selecionado;
uint32_t tempo_main_inicial, tempo_final; //variaveis auxiliares para calcular o tempo do loop da main
uint16_t recorded_dist = 0;
PID_t* launch_control;

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

	// if selected mode is acceleration, initializes PID
	if (mode_slc == 1) {
		double pid_max=1, pid_min=0, pid_sample=0.002, Kp=10, Ti=1, Td=0, pid_setpoint;
		PID_init(launch_control,1,Kp,Ti,Td,pid_max,pid_min,pid_sample);

		uint8_t launch_slc=0; // launch selection - switches between dry and wet parameters

		switch(launch_slc) {  // transfer this variable to the interruptions section, write
							  // the logic to change with button input an then import it as
							  // extern once everything is defined (pinout etc)
		case DRY:
			standard:;
			pid_setpoint=12;
			PID_set_setpoint(launch_control,pid_setpoint); // 12% slip
			break;
		case WET:
			pid_setpoint=8;
			PID_set_setpoint(launch_control,pid_setpoint); // 8% slip not yet validated
			break;
		default:
			goto standard;
		}

	}
	//init_datalogger();

	// updates test counter
	Update_Test_Counter();
	// clears current counters for the test
	Error_Diagnostics_Clear();
	// clears previous test distance
	Clear_Test_Distance();
	// gets travelled distance for odometer
	recorded_dist = Get_Traveled_Distance();
	dist_log[1] = recorded_dist;

	//      Loop
	while (true)
	{
		tempo_main_inicial = HAL_GetTick(); // usado para calcular o tempo do loop da main
		//atualiza_watchdog();
		le_acelerador(&flag_erro_apps);
		le_volante();
		Vel_Calc();
		Odometer();
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
