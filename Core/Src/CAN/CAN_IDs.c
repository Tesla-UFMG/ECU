/*
 * CAN_IDs.c
 *
 *  Created on: Dec 9, 2020
 *      Author: renanmoreira
 */
#include "CAN/CAN_IDs.h"

CAN_ID_t CAN_ID_map[CAN_ID_QUAN];



void initialize_CAN_IDs() {
	CAN_ID_map[ID_ECU_TIMER] = (CAN_ID_t)INTERNAL_ID_ECU_TIMER;
}
