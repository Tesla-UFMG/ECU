/*
 * odometer.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "main.h"
#include "datalogging/odometer.h"
#include "CAN/CAN_IDs.h"
#include "CAN/general_can.h"
#include "cmsis_os.h"
#include "util/global_definitions.h"
#include "util/util.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "util/flash_sector_h7.h"


//Function to calculate distance traveled
static inline uint32_t calculate_distance(uint32_t speed_avg);

void odometer_calc()

{
	//Useful variables declaration
	uint16_t save_counter = 0;
	uint32_t front_speed_avg = 0;
	uint32_t delay_dist_traveled = 0;
    uint32_t dist_sent = 0;
    uint32_t flash_distance[8];
    bool first_exe = true;

    //Distance struct variable created. Partial distance initialized
    DIST_TRAVELED_t dist_traveled;
    dist_traveled.distances[1] = 0;

    //Getting CAN Id's
    CAN_ID_t can_id_p = get_CAN_ID_from_internal(6);
    CAN_ID_t can_id_t = get_CAN_ID_from_internal(7);

    //Initializing flash_distance array
    for (uint8_t i = 0; i< 8; ++i)
    {
    	flash_distance[i] = 0;
    }

    //If it is the first code execution
    if (first_exe)
          {
            //Read distance from flash just once
      	  Flash_Read_Data(0x080E0000, &(dist_traveled.distances[0]), 1);

      	  //In case the memory position has never been used/saved
      	  if ( (dist_traveled.distances[0]) == 0x00000000 )
      		  dist_traveled.distances[0] = 0;

      	  //Store in the global variable
      	   set_global_var(DIST_TRAVELED, &dist_traveled);

		  //Send total distance data
		  general_can_transmit(can_id_t.id, (uint16_t*)&dist_traveled.distances[0]);

		  //Send partial distance data
		  general_can_transmit(can_id_p.id, (uint16_t*)&dist_traveled.distances[1]);

      	  //No longer the first execution
      	   first_exe = false;
          }

    for (;;)
    {
		#ifdef DEBUG_ECU
        	extern void brkpt();
        	brkpt();
		#endif

    //Use global defined struct to read wheel speeds
     WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);

     //Calculate the front wheel speed average due to reduced change of locking up compared to rear
     front_speed_avg = (uint32_t)((wheel_speeds.speed[FRONT_RIGHT]+wheel_speeds.speed[FRONT_LEFT])/2);

     //Calculate the partial distance traveled from the front tires average speed
     delay_dist_traveled = calculate_distance(front_speed_avg);

    //Read the global variable
     dist_traveled = get_global_var_value(DIST_TRAVELED);

     //Add the delay distance traveled to the total
     dist_traveled.distances[0] += delay_dist_traveled;
     dist_traveled.distances[1] += delay_dist_traveled;

     //Write in the global variable
     set_global_var(DIST_TRAVELED, &dist_traveled);

     //Area for CAN and HMI processing

     //If the distance is littler than 100 meters send partial distance data every meter traveled
     if (dist_traveled.distances[1] < 100)
     {
    	 //Send using CAN the distance traveled in meters
    	 general_can_transmit(can_id_p.id, (uint16_t*)&dist_traveled.distances[1]);

    	 //Record the distance value sent
    	 dist_sent = dist_traveled.distances[1];
     }
     else
     {
    	 //After the distance surpass 100 meters, send partial distance data only when other 100 meters are traveled
    	 if ((dist_traveled.distances[1] - dist_sent) > 100)
    	 {
    		 //Send using CAN
    		 general_can_transmit(can_id_p.id, (uint16_t*)&dist_traveled.distances[1]);

    		 //Record the distance value sent
    		 dist_sent = dist_traveled.distances[1];

    		 //Send total distance data when 100 meters are traveled
        	 general_can_transmit(can_id_t.id, (uint16_t*)&dist_traveled.distances[0]);

        	 //Edit flash distance array to save in the memory
    		 flash_distance[0] = dist_traveled.distances[0];

    		 //In order to avoid saving multiple times and wearing the embedded flash, save up to 100 times
    		 if (save_counter < MAX_SAVE_TIMES)
    		 {
    			 //Function call to save data
    			 Flash_Write_Data(0x080E0000, flash_distance, 8);

    			 //Increment of counter
    			 save_counter ++;
    		 }
    		 else
    		 {
    			 //Could do something
    		 }
    	 }
     }

     //Delay in thread execution. 5 seconds
     osDelay(AVG_TIME);
    }
}

static inline uint32_t calculate_distance(uint32_t speed_avg)
{
	/*
	 * AVG_TIME (ms)
	 * speed_avg (10*km/h)
	 * 1/36000 is a correction factor to make distance in meters (m)
	 */
	return ( ((1/36000)* speed_avg) * (AVG_TIME));
}

