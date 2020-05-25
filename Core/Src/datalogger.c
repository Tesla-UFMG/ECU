/*
 * datalogger.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */


void datalogger(void *argument) {
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
