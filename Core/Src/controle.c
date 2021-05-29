/*
 * controle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "controle.h"
#include "cmsis_os.h"
#include "global_definitions.h"
#include "constants.h"
#include "datalog_handler.h"

extern osMutexId_t m_state_parameter_mutexHandle;
extern osMessageQueueId_t q_ref_torque_messageHandle;
extern volatile uint16_t g_motor_speed[2];
extern volatile uint16_t throttle_percent;

volatile vehicle_state_parameters_t g_vehicle_state_parameters;

volatile vehicle_state_e vehicle_state;

void update_state(bool disable) {
	//TODO: implementar funcao de setar leds
	if (disable == true) {
		vehicle_state = S_DISABLE_E;
	} else if ((throttle_percent < 100) && (frenagem_regenerativa == true)
				&& g_motor_speed[L_MOTOR] > _5_kmph_rpm) {
//		seta_leds(AZUL);	// se frenagem ativa, led da ecu indica BRANCO
		vehicle_state = S_BRAKE_E;
	} else if(throttle_percent > 100) {
//		seta_leds(modo_selecionado.cor); // quando desligada, volta a cor do modo
		vehicle_state = S_ACCELERATE_E;
	} else {
//		seta_leds(modo_selecionado.cor);	// quando desligada, volta a cor do modo
		vehicle_state = S_NEUTER_E;
	}
}

void update_state_parameters(torque_message_t* torque_message) {

	switch(vehicle_state) {
		case S_NEUTER_E:
			set_bit8(&torque_message->parameters, P_ENABLE, true);
			set_bit8(&torque_message->parameters, P_BRAKE, false);
			//TODO: mudar velocidade do motor de acordo com nova logica
			set_bit8(&torque_message->parameters, P_RUNSTOP, (g_motor_speed[R_MOTOR] > _5_kmph_rpm || g_motor_speed[L_MOTOR] > _5_kmph_rpm));
			torque_message->torque_ref[R_MOTOR] = 0;
			torque_message->torque_ref[L_MOTOR] = 0;
			torque_message->neg_torque_ref[R_MOTOR] = 0;
			torque_message->neg_torque_ref[L_MOTOR] = 0;
			torque_message->speed_ref[R_MOTOR] = modo_selecionado.vel_max;
			torque_message->speed_ref[L_MOTOR] = modo_selecionado.vel_max;
			break;
		case S_BRAKE_E:
			set_bit8(&torque_message->parameters, P_ENABLE, true);
			set_bit8(&torque_message->parameters, P_BRAKE, modo_selecionado.freio_regen);
			set_bit8(&torque_message->parameters, P_RUNSTOP, true);
			torque_message->torque_ref[R_MOTOR] = 0;
			torque_message->torque_ref[L_MOTOR] = 0;
			torque_message->neg_torque_ref[R_MOTOR] = REG_BRAKING_TORQUE;
			torque_message->neg_torque_ref[L_MOTOR] = REG_BRAKING_TORQUE;
			torque_message->speed_ref[R_MOTOR] = 0;
			torque_message->speed_ref[L_MOTOR] = 0;
			break;
		case S_ACCELERATE_E:
			set_bit8(&torque_message->parameters, P_ENABLE, true);
			set_bit8(&torque_message->parameters, P_BRAKE, false);
			set_bit8(&torque_message->parameters, P_RUNSTOP, true);
			//TODO: Mudar para nova lógica de envio de mensagem de torque ao inversor
			torque_message->neg_torque_ref[R_MOTOR] = 0;
			torque_message->neg_torque_ref[L_MOTOR] = 0;
			torque_message->speed_ref[R_MOTOR] = modo_selecionado.vel_max;
			torque_message->speed_ref[L_MOTOR] = modo_selecionado.vel_max;

			//if (modo_selecionado.traction_control == true) tc_system();
			//else torque_vectoring();
			break;

		case S_DISABLE_E:
			set_bit8(&torque_message->parameters, P_ENABLE, false);
			set_bit8(&torque_message->parameters, P_BRAKE, false);
			set_bit8(&torque_message->parameters, P_RUNSTOP, false);
			torque_message->torque_ref[R_MOTOR] = 0;
			torque_message->torque_ref[L_MOTOR] = 0;
			torque_message->neg_torque_ref[R_MOTOR] = 0;
			torque_message->neg_torque_ref[L_MOTOR] = 0;
			torque_message->speed_ref[R_MOTOR] = 0;
			torque_message->speed_ref[L_MOTOR] = 0;
			break;
	}
}

void controle(void *argument) {

	//veloc_total = (speed_t_total[0] + speed_t_total[1] + speed_t_total[2] + speed_t_total[3]) / 4;
	vehicle_state_e vehicle_state;

	//mensagem de referência de torque. Contem qual valor de torque se deseja e se é desejado desabilitar
	ref_torque_t ref_torque_message;
	//mensagem de torque completa para ser enviada ao inversor
	torque_message_t torque_message;


	for (;;) {

		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		switch(osMessageQueueGet(q_ref_torque_messageHandle, &ref_torque_message, 0, CONTROLE_DELAY)) {
		case osOK:

			torque_message.torque_ref[R_MOTOR] = ref_torque_message.ref_torque[R_MOTOR];
			torque_message.torque_ref[L_MOTOR] = ref_torque_message.ref_torque[L_MOTOR];

			update_state(ref_torque_message.disable);
			update_state_parameters(&torque_message);


			osMessageQueuePut(q_torque_messageHandle, &torque_message, 0, 0U);

			log_data(ID_REF_TORQUE_R_MOTOR, torque_message.torque_ref[R_MOTOR]);
			log_data(ID_REF_TORQUE_L_MOTOR, torque_message.torque_ref[L_MOTOR]);

			break;
		case osErrorTimeout:
			update_state(ref_torque_message.disable);
			update_state_parameters(&torque_message);

			osMessageQueuePut(q_torque_messageHandle, &torque_message, 0, 0U);
			break;
		default:
			break;
		}



	}

}
