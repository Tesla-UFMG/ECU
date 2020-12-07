/*
 * controle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "controle.h"

extern osMutexId_t m_state_parameter_mutexHandle;

volatile vehicle_state_parameters_t g_vehicle_state_parameters;


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

void update_state_parameters(vehicle_state_e* vehicle_state) {

	switch(*vehicle_state) {
		case S_NEUTER_E:
			set_bit(&g_vehicle_state_parameters.parameter_control, P_ENABLE, true);
			set_bit(&g_vehicle_state_parameters.parameter_control, P_BRAKE, false);
			//TODO: mudar velocidade do motor de acordo com nova logica
			set_bit(&g_vehicle_state_parameters.parameter_control, P_RUNSTOP, (vel_motor[MOTOR_DIR] > _5_kmph_rpm || vel_motor[MOTOR_ESQ] > _5_kmph_rpm));
			g_vehicle_state_parameters.ref_torque[MOTOR_DIR] = 0;
            g_vehicle_state_parameters.ref_torque[MOTOR_ESQ] = 0;
            g_vehicle_state_parameters.ref_torque_neg[MOTOR_DIR] = 0;
            g_vehicle_state_parameters.ref_torque_neg[MOTOR_ESQ] = 0;
            g_vehicle_state_parameters.ref_veloc[MOTOR_DIR] = modo_selecionado.vel_max;
            g_vehicle_state_parameters.ref_veloc[MOTOR_ESQ] = modo_selecionado.vel_max;
            g_vehicle_state_parameters.regen_active = false;
			break;
		case S_BRAKE_E:
			set_bit(&g_vehicle_state_parameters.parameter_control, P_ENABLE, true);
			set_bit(&g_vehicle_state_parameters.parameter_control, P_BRAKE, modo_selecionado.freio_regen);
			set_bit(&g_vehicle_state_parameters.parameter_control, P_RUNSTOP, true);
			g_vehicle_state_parameters.ref_torque[MOTOR_DIR] = 0;
			g_vehicle_state_parameters.ref_torque[MOTOR_ESQ] = 0;
			g_vehicle_state_parameters.ref_torque_neg[MOTOR_DIR] = torq_frenagem;
			g_vehicle_state_parameters.ref_torque_neg[MOTOR_ESQ] = torq_frenagem;
			g_vehicle_state_parameters.ref_veloc[MOTOR_DIR] = 0;
			g_vehicle_state_parameters.ref_veloc[MOTOR_ESQ] = 0;
			g_vehicle_state_parameters.regen_active = true;
			break;
		case S_ACCELERATE_E:
			set_bit(&g_vehicle_state_parameters.parameter_control, P_ENABLE, true);
			set_bit(&g_vehicle_state_parameters.parameter_control, P_BRAKE, false);
			set_bit(&g_vehicle_state_parameters.parameter_control, P_RUNSTOP, true);
			//TODO: Mudar para nova lógica de envio de mensagem de torque ao inversor
			g_vehicle_state_parameters.ref_torque[MOTOR_DIR] = (uint16_t) (modo_selecionado.torq_gain * acelerador) / 10;
			g_vehicle_state_parameters.ref_torque[MOTOR_ESQ] = (uint16_t) (modo_selecionado.torq_gain * acelerador) / 10;
			g_vehicle_state_parameters.ref_torque_neg[MOTOR_DIR] = 0;
			g_vehicle_state_parameters.ref_torque_neg[MOTOR_ESQ] = 0;
			g_vehicle_state_parameters.ref_veloc[MOTOR_DIR] = modo_selecionado.vel_max;
			g_vehicle_state_parameters.ref_veloc[MOTOR_ESQ] = modo_selecionado.vel_max;
			g_vehicle_state_parameters.regen_active = false;

			//TODO: readaptar para lógica de rampa de torque
			rampa_torque();
			//if (modo_selecionado.traction_control == true) tc_system();
			//else torque_vectoring();
			break;

		case S_DISABLE_E:
			set_bit(&g_vehicle_state_parameters.parameter_control, P_ENABLE, false);
			set_bit(&g_vehicle_state_parameters.parameter_control, P_BRAKE, false);
			set_bit(&g_vehicle_state_parameters.parameter_control, P_RUNSTOP, false);
			g_vehicle_state_parameters.ref_torque[MOTOR_DIR] = 0;
			g_vehicle_state_parameters.ref_torque[MOTOR_ESQ] = 0;
			g_vehicle_state_parameters.ref_torque_neg[MOTOR_DIR] = 0;
			g_vehicle_state_parameters.ref_torque_neg[MOTOR_ESQ] = 0;
			g_vehicle_state_parameters.ref_veloc[MOTOR_DIR] = 0;
			g_vehicle_state_parameters.ref_veloc[MOTOR_ESQ] = 0;
			g_vehicle_state_parameters.regen_active = false;
			regen_active = false;
			break;
	}
}

void controle(void *argument) {

	//veloc_total = (speed_t_total[0] + speed_t_total[1] + speed_t_total[2] + speed_t_total[3]) / 4;
	vehicle_state_e vehicle_state;

	uint8_t parameters = 0;


	for (;;) {

		osMutexAcquire(m_state_parameter_mutexHandle, osWaitForever);

		update_state(&vehicle_state);

		update_state_parameters(&vehicle_state);

		osMutexRelease(m_state_parameter_mutexHandle);

		//TODO: reavaliar quantidade certa de delay
		osDelay(100);


	}

}
