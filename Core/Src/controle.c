/*
 * controle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "controle.h"

void update_state(vehicle_state_e* vehicle_state) {
	if (desabilitar == true) {
		*vehicle_state = S_DISABLE_E;
	} else if ((throttle_percent < 100) && (frenagem_regenerativa == true)
				&& vel_motor[MOTOR_ESQ] > _5_kmph_rpm) {
		seta_leds(AZUL);	// se frenagem ativa, led da ecu indica BRANCO
		*vehicle_state = S_BRAKE_E;
	} else if(acelerador > 100) {
		seta_leds(modo_selecionado.cor); // quando desligada, volta a cor do modo
		*vehicle_state = S_ACCELERATE_E;
	} else if (botao_habilita_acionado() && mode_slc == 4) {
		*vehicle_state = S_REVERSE_E;
	} else {
		seta_leds(modo_selecionado.cor);	// quando desligada, volta a cor do modo
		*vehicle_state = S_NEUTER_E;
	}
}

void controle(void *argument) {

	//veloc_total = (speed_t_total[0] + speed_t_total[1] + speed_t_total[2] + speed_t_total[3]) / 4;
	vehicle_state_e vehicle_state;

	for (;;) {

		update_state(&vehicle_state);

		switch(vehicle_state){
			case S_NEUTER_E:
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

			case S_BRAKE_E:
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

			case S_ACCELERATE_E:
				habilita = true;
				freiar = false;
				runstop = true;
				rev = false;
				refTorque[MOTOR_DIR] = (uint16_t) (modo_selecionado.torq_gain * acelerador) / 10;
				refTorque[MOTOR_ESQ] = (uint16_t) (modo_selecionado.torq_gain * acelerador) / 10;
				refTorqueNeg[MOTOR_DIR] =  0;
				refTorqueNeg[MOTOR_ESQ] =  0;
				refVeloc[MOTOR_DIR] = modo_selecionado.vel_max;
				refVeloc[MOTOR_ESQ] = modo_selecionado.vel_max;

				rampa_torque();
				//if (modo_selecionado.traction_control == true) tc_system();
				//else torque_vectoring();
				regen_active = false;
			break;

			case S_REVERSE_E:
				habilita = true;
				freiar = false;
				runstop = true;
				rev = true;
				refTorque[MOTOR_DIR] = 0;
				refTorque[MOTOR_ESQ] = 0;
				refTorqueNeg[MOTOR_DIR] =  300; // 30%
				refTorqueNeg[MOTOR_ESQ] =  300;
				refVeloc[MOTOR_DIR] = 500;
				refVeloc[MOTOR_ESQ] = 500;
				regen_active = false;
			break;

			case S_DISABLE_E:
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

}
