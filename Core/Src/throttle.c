/*
 * throttle.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "throttle.h"
#include "datalog_handler.h"

#include "error_treatment.h"

static uint16_t APPS1value;
static uint16_t APPS2value;

void throttle_read(void *argument) {
	uint16_t BSE;
	uint16_t apps1_calc;
	uint16_t aux_throttle_percent = 0;
	for (;;) {
		#ifdef DEBUG_ECU
		extern void brkpt();
		brkpt();
		#endif

		APPS1value = ADC_DMA_buffer[APPS1_E];
		APPS2value = ADC_DMA_buffer[APPS2_E];
		BSE   = ADC_DMA_buffer[BRAKE_E];
		apps1_calc = 0;

		if (APPS2 < 260) {
			aux_throttle_percent = 0;
		} else if (APPS2 >= 260 && APPS2 < 467) {
			aux_throttle_percent = 1.162 * APPS2 - 342.9;
		} else if (APPS2 >= 467 && APPS2 < 1065) {
			aux_throttle_percent = 0.3344 * APPS2 + 43.8;
		} else if (APPS2 >= 1065 && APPS2 < 2253) {
			aux_throttle_percent = 0.1684 * APPS2 + 220.7;
		} else if (APPS2 >= 2253 && APPS2 < 3211) {
			aux_throttle_percent = 0.2087 * APPS2 + 129.9;
		} else if (APPS2 >= 3211 && APPS2 < 3720) {
			aux_throttle_percent = 0.6598 * APPS2 - 1319;
		} else if (APPS2 >= 3720)
			aux_throttle_percent = 0;


		if (aux_throttle_percent >= 0 && aux_throttle_percent < 200)
			apps1_calc = 2212;
		else if (aux_throttle_percent >= 200 && aux_throttle_percent < 400)
			apps1_calc = (uint16_t) (1.679 * aux_throttle_percent + 1876);
		else if (aux_throttle_percent >= 400 && aux_throttle_percent < 600)
			apps1_calc = (uint16_t) (2.621 * aux_throttle_percent + 1499);
		else if (aux_throttle_percent >= 600 && aux_throttle_percent < 800)
			apps1_calc = (uint16_t) (2.212 * aux_throttle_percent + 1745);
		else if (aux_throttle_percent >= 800 && aux_throttle_percent < 1135)
			apps1_calc = (uint16_t) (1.515 * aux_throttle_percent + 2302);


		if (APPS1 > 3900 || APPS1 < 1802.24)
			aux_throttle_percent = 0;
		else if (APPS1 < apps1_calc * (1-APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0)
			  || APPS1 > apps1_calc * (1+APPS_PLAUSIBILITY_PERCENTAGE_TOLERANCE/100.0))
			aux_throttle_percent = 0;

		if (aux_throttle_percent > 1000)
			aux_throttle_percent = 1000;


		if (aux_throttle_percent < 250 && BSE < 2200) {
			//TODO: limpar flag de erro de APPS?
//			*flag_error = 0;
//			apps_t_flag = 1;
		}

		brake_status = BSE > 2200;

		log_data(ID_BRAKE, brake_status);


		check_for_errors(checkForAppsErrors, APPS_ERROR_FLAG);

		throttle_percent = aux_throttle_percent;

		log_data(ID_THROTTLE, throttle_percent);

		//TODO: Averiguar duracao do delay
		osDelay(100);

	}

//	return (acelerador);
}


bool checkForAppsErrors()
{
	APPS1value
	/*if (apps > 30)
	{
		return true;
	}
	else
	{
		return false;
	}

	*/

	return true;
}

