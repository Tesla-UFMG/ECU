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
#include "util/CMSIS_extra/global_variables_handler.h"


static inline uint32_t calculate_distance(uint32_t speed_avg);

bool first_exe = true;

void odometer_calc()

{
	uint32_t front_speed_avg = 0;
    uint32_t partial_dist_traveled = 0;
    DIST_TRAVELED_t dist_traveled = 0;
	for (;;)
    {
		#ifdef DEBUG_ECU
        	extern void brkpt();
        	brkpt();
		#endif
    
    if (!first_exe)
        {
            /*
              * Read distance from flash just once
              * dist_traveled = flash
            */
           first_exe = false;
        }
    //Use global defined struct to read wheel speeds
     WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
     //Calculate the front wheel speed average due to reduced change of locking up compared to rear
     front_speed_avg = ((wheel_speeds.speed[FRONT_RIGHT]+wheel_speeds.speed[FRONT_LEFT])/2);
     partial_dist_traveled = calculate_distance(front_speed_avg);
     //Criar variável global para guardar velocidade e somente em determinados momentos salvar na flash?
     dist_traveled = get_global_var_value(DIST_TRAVELED);
     //Read from global variable and add partial distance traveled
     dist_traveled += partial_dist_traveled;
     //Write in the global variable
     set_global_var(DIST_TRAVELED, &dist_traveled);
      /*
      * dist_traveled (m)
      * Save variable in flash memory in certain occasions
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

