/*
 * odometer.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */
/*
 * Velocidade tem formato: (10*km/s)
 * Ver wheel_speed.c
 *
 *
 */
#include "datalogging/odometer.h"
#include "util/global_definitions.h"
#include "cmsis_os.h"
#include "util/util.h"


static inline uint32_t calculate_distance(uint32_t speed_avg);

void odometer_calc()

{
	uint32_t front_speed_avg = 0;
    uint32_t dist_traveled = 0;
	for (;;)
    {
		#ifdef DEBUG_ECU
        	extern void brkpt();
        	brkpt();
		#endif

     WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
     front_speed_avg = ((wheel_speeds[FRONT_RIGHT]+wheel_speeds[FRONT_LEFT])/2);
     dist_traveled = calculate_distance(front_speed_avg);
     /*
      * dist_traveled (m)
      * Save variable in flash memory
      */
     osDelay(AVG_TIME);
    }

}

static inline uint32_t calculate_distance(uint32_t speed_avg)
{
	/*
	 * AVG_TIME (ms)
	 * speed_avg (10*km/s)
	 */
	return ((100* speed_avg) * (AVG_TIME/1000));
}

