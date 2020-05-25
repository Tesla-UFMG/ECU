/*
 * controle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

void controle(void *argument) {
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
