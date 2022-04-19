/*
 * odometer.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_ODOMETER_H_
#define INC_ODOMETER_H_


//Delay of thread execution
#define AVG_TIME 100

//Maximum saves
//If 100 saves are done each test and considering a 10k flash memory lifespan, ECU should last about 100 tests of 10km traveled
#define MAX_SAVE_TIMES 100


enum {TOTAL_DIST, PARTIAL_DIST};

#endif /* INC_ODOMETER_H_ */
