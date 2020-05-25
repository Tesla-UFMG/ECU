/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */


void throttle_read(void *argument) {
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

//	return (acelerador);
}

