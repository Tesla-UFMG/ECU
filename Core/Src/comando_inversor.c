/*
 * comando_inversor.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */


void comando_inversor(void *argument) {
	paramControl = (habilita) + (runstop << 1) + (freiar << 2) + (acelerar << 3) + (rev << 4);

	if (flag_estado_comando_inversor==0 &&(selected_motors == MOTOR_DIR || selected_motors == MOTORESQ_DIR)) {
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
		flag_estado_comando_inversor=1;
	}

	if (flag_estado_comando_inversor==1 &&(selected_motors == MOTOR_ESQ || selected_motors == MOTORESQ_DIR)) {
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
		flag_estado_comando_inversor =2;
	}

	//MSG para verificar a integridade da comunicacao
	if (flag_estado_comando_inversor==2){

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
