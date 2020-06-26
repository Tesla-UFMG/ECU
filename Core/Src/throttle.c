/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */


void throttle_read(void *argument) {
	uint16_t APPS1;
	uint16_t APPS2;
	uint16_t BSE;
	uint16_t apps1_calc;
	for (;;) {
		APPS1 = ADC_DMA_buffer[APPS1_E];
		APPS2 = ADC_DMA_buffer[APPS2_E];
		BSE   = ADC_DMA_buffer[BRAKE_E];
		apps1_calc = 0;

		if (APPS2 < 260) {
			throttle_percent = 0;
		} else if (APPS2 >= 260 && APPS2 < 467) {
			throttle_percent = 1.162 * APPS2 - 342.9;
		} else if (APPS2 >= 467 && APPS2 < 1065) {
			throttle_percent = 0.3344 * APPS2 + 43.8;
		} else if (APPS2 >= 1065 && APPS2 < 2253) {
			throttle_percent = 0.1684 * APPS2 + 220.7;
		} else if (APPS2 >= 2253 && APPS2 < 3211) {
			throttle_percent = 0.2087 * APPS2 + 129.9;
		} else if (APPS2 >= 3211 && APPS2 < 3720) {
			throttle_percent = 0.6598 * APPS2 - 1319;
		} else if (APPS2 >= 3720)
			throttle_percent = 0;


		if (throttle_percent >= 0 && throttle_percent < 200)
			apps1_calc = 2212;
		else if (throttle_percent >= 200 && throttle_percent < 400)
			apps1_calc = (uint16_t) (1.679 * acelerador + 1876);
		else if (throttle_percent >= 400 && throttle_percent < 600)
			apps1_calc = (uint16_t) (2.621 * acelerador + 1499);
		else if (throttle_percent >= 600 && throttle_percent < 800)
			apps1_calc = (uint16_t) (2.212 * acelerador + 1745);
		else if (throttle_percent >= 800 && throttle_percent < 1135)
			apps1_calc = (uint16_t) (1.515 * acelerador + 2302);


		if (APPS1 > 3900 || APPS1 < 1802.24)
			throttle_percent = 0;
		else if (APPS1 < apps1_calc * (1-APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0)
			  || APPS1 > apps1_calc * (1+APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0))
			throttle_percent = 0;

		if (throttle_percent > 1000)
			throttle_percent = 1000;


		if (throttle_percent < 250 && BSE < 2200) {
			//TODO: limpar flag de erro de APPS?
//			*flag_error = 0;
//			apps_t_flag = 1;
		}

		brake_status = BSE > 2200;

		if (((throttle_percent > 300 && BSE > 2200) )) {
			//TODO: sinalizar evento de erro de APPS
			throttle_percent = 0;
			osEventFlagsSet(ECU_control_event_id, APPS_ERROR_FLAG);
		}

		//TODO: Averiguar duracao do delay
		osDelay(100);

	}

//	return (acelerador);
}

